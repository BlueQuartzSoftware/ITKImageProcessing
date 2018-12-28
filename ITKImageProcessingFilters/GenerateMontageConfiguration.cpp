/*
 * Your License or Copyright Information can go here
 */

#include "GenerateMontageConfiguration.h"

#include <QtCore/QDir>

#include "SIMPLib/Common/Constants.h"
#include "SIMPLib/FilterParameters/DataContainerArrayProxyFilterParameter.h"
#include "SIMPLib/FilterParameters/DataArraySelectionFilterParameter.h"
#include "SIMPLib/Geometry/ImageGeom.h"
#include "SIMPLib/SIMPLibVersion.h"
#include "SIMPLib/ITK/Dream3DTemplateAliasMacro.h"
#include "SIMPLib/ITK/itkDream3DImage.h"
#include "SIMPLib/ITK/itkTransformToDream3DTransformContainer.h"
#include "SIMPLib/ITK/itkTransformToDream3DITransformContainer.h"
#include "SIMPLib/ITK/itkInPlaceDream3DDataToImageFilter.h"
#include "SIMPLib/ITK/itkDream3DFilterInterruption.h"

#include "ITKImageProcessing/ITKImageProcessingConstants.h"
#include "ITKImageProcessing/ITKImageProcessingVersion.h"

#include "itkTileMontage.h"

#include "itkTileMergeImageFilter.h"

 // -----------------------------------------------------------------------------
 //
 // -----------------------------------------------------------------------------
GenerateMontageConfiguration::GenerateMontageConfiguration()
{
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
GenerateMontageConfiguration::~GenerateMontageConfiguration() = default;

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void GenerateMontageConfiguration::initialize()
{
	setErrorCondition(0);
	setWarningCondition(0);
	setCancel(false);
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void GenerateMontageConfiguration::setupFilterParameters()
{
	QVector<FilterParameter::Pointer> parameters;

	parameters.push_back(SIMPL_NEW_INT_VEC3_FP("Montage Size", MontageSize, FilterParameter::Parameter, GenerateMontageConfiguration));
	{
		DataContainerArrayProxyFilterParameter::Pointer parameter = DataContainerArrayProxyFilterParameter::New();
		parameter->setHumanLabel("Image Data Containers");
		parameter->setPropertyName("InputDataContainerArrayProxy");
		parameter->setSetterCallback(SIMPL_BIND_SETTER(GenerateMontageConfiguration, this, InputDataContainerArrayProxy));
		parameter->setGetterCallback(SIMPL_BIND_GETTER(GenerateMontageConfiguration, this, InputDataContainerArrayProxy));

		parameter->setDefaultFlagValue(Qt::Unchecked);
		parameter->setCategory(FilterParameter::RequiredArray);
		parameters.push_back(parameter);
	}

	{
		DataArraySelectionFilterParameter::RequirementType req;
		parameters.push_back(SIMPL_NEW_DA_SELECTION_FP("Image Data Array Path", ImageDataArrayPath, FilterParameter::RequiredArray, GenerateMontageConfiguration, req));
	}
	setFilterParameters(parameters);
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void GenerateMontageConfiguration::dataCheck()
{
	setErrorCondition(0);
	setWarningCondition(0);
	initialize();

	QString ss;
	int32_t err = 0;

	m_xMontageSize = m_MontageSize.x;
	m_yMontageSize = m_MontageSize.y;
	
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void GenerateMontageConfiguration::preflight()
{
	setInPreflight(true);
	emit preflightAboutToExecute();
	emit updateFilterParameters(this);
	dataCheck();
	emit preflightExecuted();
	setInPreflight(false);
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void GenerateMontageConfiguration::execute()
{
	setErrorCondition(0);
	setWarningCondition(0);
	dataCheck();
	if (getErrorCondition() < 0)
	{
		return;
	}

	// Gather ImageData and create Fiji Config Data Structure
	CreateFijiDataStructure();
	if (m_tiles.size() > 0)
	{
		// Pass to ITK and generate montage
		// ITK returns a new Fiji data structure to DREAM3D
		// Store FIJI DS into SIMPL Transform DS inside the Geometry
		GenerateMontage< unsigned char, unsigned int >(0);
	}
	/* Let the GUI know we are done with this filter */
	notifyStatusMessage(getHumanLabel(), "Complete");
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void GenerateMontageConfiguration::CreateFijiDataStructure()
{
	DataContainerArray* dca = getDataContainerArray().get();
	// Loop over the data containers until we find the proper data container
	QList<DataContainerProxy> containers = m_InputDataContainerArrayProxy.dataContainers.values();
	QMutableListIterator<DataContainerProxy> containerIter(containers);
	QStringList dcList;
	bool dataContainerPrefixChanged = false;
	if (containers.size() != (m_xMontageSize * m_yMontageSize))
	{
		return;
	}
	else
	{
		m_tiles.resize(m_yMontageSize);
		for (int i = 0; i < m_yMontageSize; i++)
		{
			m_tiles[i].resize(m_xMontageSize);
		}
	}
	while (containerIter.hasNext())
	{
		DataContainerProxy dcProxy = containerIter.next();
		dcList.push_back(dcProxy.name);
		DataContainer::Pointer dcItem = dca->getPrereqDataContainer(this, dcProxy.name);
		if (getErrorCondition() < 0 || dcItem.get() == nullptr)
		{
			continue;
		}
		ImageGeom::Pointer image = dcItem->getGeometryAs<ImageGeom>();

		// Extract row and column data from the data container name
		QString filename = ""; // Need to find this?
		int indexOfUnderscore = dcProxy.name.lastIndexOf("_");
		if (!dataContainerPrefixChanged)
		{
			m_dataContainerPrefix = dcProxy.name.left(indexOfUnderscore);
			dataContainerPrefixChanged = true;
		}
		QString rowCol = dcProxy.name.right(dcProxy.name.size() - indexOfUnderscore - 1);
		rowCol = rowCol.right(rowCol.size() - 1); // Remove 'r'
		QStringList rowCol_Split = rowCol.split("c"); // Split by 'c'
		int row = rowCol_Split[0].toInt();
		int col = rowCol_Split[1].toInt();
		itk::Tile2D tile;
		tile.Position[0] = col;
		tile.Position[0] = row;
		tile.FileName = filename.toStdString();

		m_tiles[row][col] = tile;

	}
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
template< typename PixelType, typename AccumulatePixelType >
void GenerateMontageConfiguration::GenerateMontage(int peakMethodToUse)
{
	using ScalarPixelType = typename itk::NumericTraits< PixelType >::ValueType;
	constexpr unsigned Dimension = 2;
	using AffineType = itk::AffineTransform< double, 3 >;
	using PointType = itk::Point<double, Dimension>;
	using VectorType = itk::Vector<double, Dimension>;
	using TransformType = itk::TranslationTransform< double, Dimension >;
	using ScalarImageType = itk::Image< ScalarPixelType, Dimension >;
	using OriginalImageType = itk::Image< PixelType, Dimension >; // possibly RGB instead of scalar
	using MontageType = itk::TileMontage< ScalarImageType >;
	using PCMType = itk::PhaseCorrelationImageRegistrationMethod<ScalarImageType, ScalarImageType>;
	typename ScalarImageType::SpacingType sp;
	sp.Fill(1.0); // assume unit spacing

	using PeakInterpolationType = typename itk::MaxPhaseCorrelationOptimizer<PCMType>::PeakInterpolationMethod;
	using PeakFinderUnderlying = typename std::underlying_type<PeakInterpolationType>::type;
	auto peakMethod = static_cast<PeakFinderUnderlying>(peakMethodToUse);

	typename MontageType::Pointer montage = MontageType::New();
	montage->SetMontageSize({ m_xMontageSize, m_yMontageSize });
	montage->GetModifiablePCM()->SetPaddingMethod(PCMType::PaddingMethod::MirrorWithExponentialDecay);
	montage->GetModifiablePCMOptimizer()->SetPeakInterpolationMethod(static_cast<PeakInterpolationType>(peakMethod));
	montage->SetOriginAdjustment(m_tiles[1][1].Position);
	montage->SetForcedSpacing(sp);

	typename MontageType::TileIndexType ind;
	for (unsigned y = 0; y < m_yMontageSize; y++)
	{
		ind[1] = y;
		for (unsigned x = 0; x < m_xMontageSize; x++)
		{
			ind[0] = x;
			typedef itk::InPlaceDream3DDataToImageFilter <ScalarPixelType, Dimension > toITKType;
			typename toITKType::Pointer toITK = toITKType::New();
			DataContainer::Pointer imageDC = GetImageDataContainer(y, x);
			toITK->SetInput(imageDC);
			toITK->SetInPlace(true);
			toITK->SetAttributeMatrixArrayName(getImageDataArrayPath().getAttributeMatrixName().toStdString());
			toITK->SetDataArrayName(getImageDataArrayPath().getDataArrayName().toStdString());
			toITK->Update();
			itk::Dream3DFilterInterruption::Pointer interruption = itk::Dream3DFilterInterruption::New();
			interruption->SetFilter(this);
			montage->AddObserver(itk::ProgressEvent(), interruption);
			montage->SetInputTile(ind, toITK->GetOutput());
		}
	}

	notifyStatusMessage(getHumanLabel(), "Doing the tile registrations");
	//itk::SimpleFilterWatcher fw(montage, "montage");
	montage->Update();
	notifyStatusMessage(getHumanLabel(), "Finished the tile registrations");

	for (unsigned y = 0; y < m_yMontageSize; y++)
	{
		ind[1] = y;
		for (unsigned x = 0; x < m_xMontageSize; x++)
		{
			ind[0] = x;
			const TransformType* regTr = montage->GetOutputTransform(ind);
			DataContainer::Pointer imageDC = GetImageDataContainer(y, x);
			ImageGeom::Pointer image = imageDC->getGeometryAs<ImageGeom>();

			// Create an ITK affine transform as a reference
			AffineType::Pointer itkAffine = AffineType::New();
			AffineType::TranslationType t;
			t.Fill(0);
			for (unsigned i = 0; i < TransformType::SpaceDimension; i++)
			{
				t[i] = regTr->GetOffset()[i];
			}
			itkAffine->SetTranslation(t);

			TransformContainer::Pointer transformContainer = GetTransformContainerFromITKAffineTransform(itkAffine);
			auto containerParameters = transformContainer->getParameters();
			auto containerFixedParameters = transformContainer->getFixedParameters();
			using FilterType = itk::TransformToDream3DITransformContainer<double, 3>;

			FilterType::Pointer filter = FilterType::New();
			filter->SetInput(itkAffine);
			filter->Update();
			::ITransformContainer::Pointer convertedITransformContainer = filter->GetOutput()->Get();
			::TransformContainer::Pointer convertedTransformContainer = std::dynamic_pointer_cast<::TransformContainer>(convertedITransformContainer);

			image->setTransformContainer(convertedITransformContainer);
		}
	}
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
DataContainer::Pointer GenerateMontageConfiguration::GetImageDataContainer(int y, int x)
{
	DataContainerArray* dca = getDataContainerArray().get();
	// Loop over the data containers until we find the proper data container
	QList<DataContainerProxy> containers = m_InputDataContainerArrayProxy.dataContainers.values();
	QMutableListIterator<DataContainerProxy> containerIter(containers);
	QStringList dcList;
	while (containerIter.hasNext())
	{
		DataContainerProxy dcProxy = containerIter.next();
		dcList.push_back(dcProxy.name);
		DataContainer::Pointer dcItem = dca->getPrereqDataContainer(this, dcProxy.name);
		if (getErrorCondition() < 0 || dcItem.get() == nullptr)
		{
			continue;
		}

		QString rowCol = dcProxy.name.right(dcProxy.name.size() - dcProxy.name.lastIndexOf("_") - 1);
		rowCol = rowCol.right(rowCol.size() - 1); // Remove 'r'
		QStringList rowCol_Split = rowCol.split("c"); // Split by 'c'
		int row = rowCol_Split[0].toInt();
		int col = rowCol_Split[1].toInt();
		if (row == y && col == x)
		{
			return dcItem;
		}
	}
	return nullptr;
}


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
typename TransformContainer::Pointer GenerateMontageConfiguration::GetTransformContainerFromITKAffineTransform(const AffineType::Pointer& itkAffine)
{
	auto parameters = itkAffine->GetParameters();
	auto fixedParameters = itkAffine->GetFixedParameters();
	auto itkAffineName = itkAffine->GetTransformTypeAsString();

	std::vector<::TransformContainer::ParametersValueType> dream3DParameters(parameters.GetSize());
	std::vector<::TransformContainer::ParametersValueType> dream3DFixedParameters(fixedParameters.GetSize());
	for (size_t p = 0; p < dream3DParameters.size(); p++)
	{
		dream3DParameters[p] = parameters[p];
	}
	for (size_t p = 0; p < dream3DFixedParameters.size(); p++)
	{
		dream3DFixedParameters[p] = fixedParameters[p];
	}
	// Create a SIMPL transform container and manually convert the ITK transform to
	// a SIMPL transform container.
	TransformContainer::Pointer transformContainer = TransformContainer::New();
	transformContainer->setTransformTypeAsString(itkAffineName);
	transformContainer->setMovingName("");
	transformContainer->setReferenceName("World");
	transformContainer->setFixedParameters(dream3DFixedParameters);
	transformContainer->setParameters(dream3DParameters);
	return transformContainer;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
AbstractFilter::Pointer GenerateMontageConfiguration::newFilterInstance(bool copyFilterParameters) const
{
	GenerateMontageConfiguration::Pointer filter = GenerateMontageConfiguration::New();
	if (copyFilterParameters)
	{
		filter->setFilterParameters(getFilterParameters());
		// We are going to hand copy all of the parameters because the other way of copying the parameters are going to
		// miss some of them because we are not enumerating all of them.
	}
	return filter;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
const QString GenerateMontageConfiguration::getCompiledLibraryName() const
{
	return ITKImageProcessingConstants::ITKImageProcessingBaseName;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
const QString GenerateMontageConfiguration::getBrandingString() const
{
	return "ITKImageProcessing";
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
const QString GenerateMontageConfiguration::getFilterVersion() const
{
	QString version;
	QTextStream vStream(&version);
	vStream << ITKImageProcessing::Version::Major() << "." << ITKImageProcessing::Version::Minor() << "." << ITKImageProcessing::Version::Patch();
	return version;
}
// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
const QString GenerateMontageConfiguration::getGroupName() const
{
	return SIMPL::FilterGroups::IOFilters;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
const QUuid GenerateMontageConfiguration::getUuid()
{
	return QUuid("{4388723b-cc16-3477-ac6f-fe0107107e74}");
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
const QString GenerateMontageConfiguration::getSubGroupName() const
{
	return SIMPL::FilterSubGroups::GenerationFilters;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
const QString GenerateMontageConfiguration::getHumanLabel() const
{
	return "Generate Montage Configuration";
}

#define ITK_IMAGE_READER_CLASS_NAME GenerateMontageConfiguration

#include "SIMPLib/ITK/itkGetComponentsDimensions.h"
#include "SIMPLib/ITK/itkInPlaceImageToDream3DDataFilter.h"

#include "SIMPLib/ITK/itkImageReaderHelper.cpp"
