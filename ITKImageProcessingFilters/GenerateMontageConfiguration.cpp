/*
 * Your License or Copyright Information can go here
 */

#include "GenerateMontageConfiguration.h"

#include <QtCore/QDir>

#include "SIMPLib/Common/Constants.h"
#include "SIMPLib/FilterParameters/MultiDataContainerSelectionFilterParameter.h"
#include "SIMPLib/FilterParameters/LinkedBooleanFilterParameter.h"
#include "SIMPLib/FilterParameters/StringFilterParameter.h"
#include "SIMPLib/FilterParameters/DataArraySelectionFilterParameter.h"
#include "SIMPLib/Geometry/ImageGeom.h"
#include "SIMPLib/SIMPLibVersion.h"
#include "SIMPLib/ITK/Dream3DTemplateAliasMacro.h"
#include "SIMPLib/ITK/itkDream3DImage.h"
#include "SIMPLib/ITK/itkTransformToDream3DTransformContainer.h"
#include "SIMPLib/ITK/itkTransformToDream3DITransformContainer.h"
#include "SIMPLib/ITK/itkInPlaceDream3DDataToImageFilter.h"
#include "SIMPLib/ITK/itkInPlaceImageToDream3DDataFilter.h"
#include "SIMPLib/ITK/itkDream3DFilterInterruption.h"

#include "ITKImageProcessing/ITKImageProcessingConstants.h"
#include "ITKImageProcessing/ITKImageProcessingVersion.h"

#include "itkTileMontage.h"
#include "itkTxtTransformIOFactory.h"
#include "itkTileMergeImageFilter.h"
#include "itkStreamingImageFilter.h"
#include "itkCommand.h"

class ITKProgressObserver : public itk::Command
{
  public:
    void Execute(itk::Object* caller, const itk::EventObject& event) override
    {
      Execute((const itk::Object*)caller, event);
    }

    void Execute(const itk::Object* caller, const itk::EventObject& event) override
    {
      if(!itk::ProgressEvent().CheckEvent(&event))
      {
        return;
      }
      const auto* processObject = dynamic_cast<const itk::ProcessObject*>(caller);
      if(!processObject)
      {
        return;
      }

      QString progressStr = QString::number(processObject->GetProgress() * 100);
      QString ss = QObject::tr("%1: %2%").arg(m_MessagePrefix).arg(progressStr);
      m_Filter->notifyStatusMessage(m_Filter->getHumanLabel(), ss);
    }

    void setMessagePrefix(const QString &prefix)
    {
      m_MessagePrefix = prefix;
    }

    void setFilter(AbstractFilter* filter)
    {
      m_Filter = filter;
    }

  private:
    AbstractFilter* m_Filter = nullptr;
    QString m_MessagePrefix;
};

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
    QStringList linkedProps;
    linkedProps << "MontageDataContainerName";
    linkedProps << "MontageAttributeMatrixName";
    linkedProps << "MontageDataArrayName";
    parameters.push_back(SIMPL_NEW_LINKED_BOOL_FP("Stitch Montage", StitchMontage, FilterParameter::Parameter, GenerateMontageConfiguration, linkedProps));
  }

  {
    MultiDataContainerSelectionFilterParameter::RequirementType req =
    MultiDataContainerSelectionFilterParameter::CreateRequirement(SIMPL::Defaults::AnyPrimitive, SIMPL::Defaults::AnyComponentSize, AttributeMatrix::Type::Cell, IGeometry::Type::Image);
    parameters.push_back(SIMPL_NEW_MDC_SELECTION_FP("Image Data Containers", ImageDataContainers, FilterParameter::RequiredArray, GenerateMontageConfiguration, req));
  }
  parameters.push_back(SIMPL_NEW_STRING_FP("Common Attribute Matrix", CommonAttributeMatrixName, FilterParameter::RequiredArray, GenerateMontageConfiguration));
  parameters.push_back(SIMPL_NEW_STRING_FP("Common Data Array", CommonDataArrayName, FilterParameter::RequiredArray, GenerateMontageConfiguration));

  parameters.push_back(SIMPL_NEW_STRING_FP("Montage Data Container Name", MontageDataContainerName, FilterParameter::CreatedArray, GenerateMontageConfiguration));
  parameters.push_back(SIMPL_NEW_STRING_FP("Montage Attribute Matrix Name", MontageAttributeMatrixName, FilterParameter::CreatedArray, GenerateMontageConfiguration));
  parameters.push_back(SIMPL_NEW_STRING_FP("Montage Data Array Name", MontageDataArrayName, FilterParameter::CreatedArray, GenerateMontageConfiguration));


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
  int err = 0;

	m_xMontageSize = m_MontageSize.x;
	m_yMontageSize = m_MontageSize.y;

  if (m_xMontageSize <= 0 || m_yMontageSize <= 0)
	{
    setErrorCondition(-11000);
		QString ss = QObject::tr("The Montage Size x and y values must be greater than 0");
		notifyErrorMessage(getHumanLabel(), ss, getErrorCondition());
		return;
	}

  // Test to make sure at least one data container is selected
	if (getImageDataContainers().size() < 1)
	{
    setErrorCondition(-11001);
		QString ss = QObject::tr("At least one Data Container must be selected");
		notifyErrorMessage(getHumanLabel(), ss, getErrorCondition());
		return;
	}

  if (getCommonAttributeMatrixName().isEmpty())
  {
    setErrorCondition(-11003);
    QString ss = QObject::tr("Common Attribute Matrix is empty.");
    notifyErrorMessage(getHumanLabel(), ss, getErrorCondition());
    return;
  }

  if (getCommonDataArrayName().isEmpty())
  {
    setErrorCondition(-11004);
    QString ss = QObject::tr("Common Data Array is empty.");
    notifyErrorMessage(getHumanLabel(), ss, getErrorCondition());
    return;
  }

	// Test to see that the image data containers are correct
  int dcCount = m_ImageDataContainers.size();
  for (int i = 0; i < dcCount; i++)
	{
    QString dcName = m_ImageDataContainers[i];

    DataArrayPath testPath;
    testPath.setDataContainerName(dcName);
		testPath.setAttributeMatrixName(getCommonAttributeMatrixName());
		testPath.setDataArrayName(getCommonDataArrayName());

    AttributeMatrix::Pointer imageDataAM = getDataContainerArray()->getPrereqAttributeMatrixFromPath(this, testPath, err);
    if (getErrorCondition() < 0 || err < 0)
    {
      return;
    }

    IDataArray::Pointer ptr = getDataContainerArray()->getPrereqIDataArrayFromPath<IDataArray, AbstractFilter>(this, testPath);
    if (getErrorCondition() < 0)
    {
      return;
    }

    QVector<size_t> imageDataTupleDims = imageDataAM->getTupleDimensions();
    if (imageDataTupleDims.size() < 2)
    {
      QString ss = QObject::tr("Image Data Array at path '%1' must have at least 2 tuple dimensions.").arg(testPath.serialize("/"));
      setErrorCondition(-11005);
      notifyErrorMessage(getHumanLabel(), ss, getErrorCondition());
      return;
    }

    ImageGeom::Pointer image = getDataContainerArray()->getPrereqGeometryFromDataContainer<ImageGeom, AbstractFilter>(this, testPath.getDataContainerName());
    if (getErrorCondition() < 0)
    {
      return;
    }
	}	

  if (m_StitchMontage)
  {
    DataArrayPath dap(getMontageDataContainerName(), getMontageAttributeMatrixName(), getMontageDataArrayName());

    DataContainer::Pointer dc = getDataContainerArray()->createNonPrereqDataContainer(this, getMontageDataContainerName());
    if (getErrorCondition() < 0)
    {
      return;
    }

//    size_t montageArrayXSize = imageDataTupleDims[0] * m_xMontageSize;
//    size_t montageArrayYSize = imageDataTupleDims[1] * m_yMontageSize;

//    ImageGeom::Pointer imageGeom = ImageGeom::New();
//    imageGeom->setName("MontageGeometry");
//    imageGeom->setDimensions(montageArrayXSize, montageArrayYSize, 1);
//    dc->setGeometry(imageGeom);

//    QString ss = QObject::tr("The image geometry dimensions of data container '%1' are projected to be (%2, %3, %4).  This is assuming "
//                             "0% overlap between tiles, so the actual geometry dimensions after executing the stitching algorithm may be smaller.")
//        .arg(dc->getName()).arg(montageArrayXSize).arg(montageArrayYSize).arg(1);
//    setWarningCondition(-3001);
//    notifyWarningMessage(getHumanLabel(), ss, getWarningCondition());

//    QVector<size_t> tDims = {montageArrayXSize, montageArrayYSize, 1};

//    AttributeMatrix::Pointer am = dc->createNonPrereqAttributeMatrix(this, dap.getAttributeMatrixName(), tDims, AttributeMatrix::Type::Cell);
//    if (getErrorCondition() < 0)
//    {
//      return;
//    }

//    ss = QObject::tr("The tuple dimensions of attribute matrix '%1' are projected to be (%2, %3, %4).  This is assuming "
//                     "0% overlap between tiles, so the actual geometry dimensions after executing the stitching algorithm may be smaller.")
//        .arg(am->getName()).arg(montageArrayXSize).arg(montageArrayYSize).arg(1);
//    setWarningCondition(-3002);
//    notifyWarningMessage(getHumanLabel(), ss, getWarningCondition());

//    if (getMontageDataArrayName().isEmpty())
//    {
//      QString ss = QObject::tr("The Montage Data Array Name field is empty.");
//      setErrorCondition(-3003);
//      notifyErrorMessage(getHumanLabel(), ss, getErrorCondition());
//      return;
//    }

//    IDataArray::Pointer da = imageDataPtr->createNewArray(montageArrayXSize * montageArrayYSize, QVector<size_t>(1, 1), getMontageDataArrayName(), !getInPreflight());
//    am->addAttributeArray(da->getName(), da);

//    ss = QObject::tr("The number of elements of montage data array '%1' is projected to be %2.  This is assuming "
//                     "0% overlap between tiles, so the actual geometry dimensions after executing the stitching algorithm may be smaller.")
//        .arg(da->getName()).arg(QLocale::system().toString(static_cast<int>(da->getNumberOfTuples())));
//    setWarningCondition(-3004);
//    notifyWarningMessage(getHumanLabel(), ss, getWarningCondition());
  }
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

  createFijiDataStructure();
  if (getErrorCondition() < 0)
  {
    return;
  }

  if (m_StageTiles.size() > 0)
	{
		// Pass to ITK and generate montage
		// ITK returns a new Fiji data structure to DREAM3D
		// Store FIJI DS into SIMPL Transform DS inside the Geometry
    generateMontage< unsigned char, unsigned int >();
	}
	/* Let the GUI know we are done with this filter */
	notifyStatusMessage(getHumanLabel(), "Complete");
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void GenerateMontageConfiguration::createFijiDataStructure()
{
	DataContainerArray* dca = getDataContainerArray().get();
	// Loop over the data containers until we find the proper data container
  QMutableListIterator<QString> dcNameIter(m_ImageDataContainers);
	QStringList dcList;
	bool dataContainerPrefixChanged = false;
  if (m_ImageDataContainers.size() != (m_xMontageSize * m_yMontageSize))
	{
		return;
	}
	else
	{
    m_StageTiles.resize(m_yMontageSize);
		for (int i = 0; i < m_yMontageSize; i++)
		{
      m_StageTiles[i].resize(m_xMontageSize);
		}
	}

  while (dcNameIter.hasNext())
	{
    QString dcName = dcNameIter.next();
    dcList.push_back(dcName);
    DataContainer::Pointer dcItem = dca->getPrereqDataContainer(this, dcName);
		if (getErrorCondition() < 0 || dcItem.get() == nullptr)
		{
			continue;
		}
    ImageGeom::Pointer image = dcItem->getGeometryAs<ImageGeom>();

		// Extract row and column data from the data container name
		QString filename = ""; // Need to find this?
		int indexOfUnderscore = dcName.lastIndexOf("_");
		if (!dataContainerPrefixChanged)
		{
			m_dataContainerPrefix = dcName.left(indexOfUnderscore);
			dataContainerPrefixChanged = true;
		}
		QString rowCol = dcName.right(dcName.size() - indexOfUnderscore - 1);
		rowCol = rowCol.right(rowCol.size() - 1); // Remove 'r'
		QStringList rowCol_Split = rowCol.split("c"); // Split by 'c'
		int row = rowCol_Split[0].toInt();
		int col = rowCol_Split[1].toInt();
		itk::Tile2D tile;
    tile.Position[0] = col;
    tile.Position[1] = row;
    tile.FileName = "";   // This code gets its data from memory, not from a file

    m_StageTiles[row][col] = tile;
	}
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
template< typename PixelType, typename AccumulatePixelType >
void GenerateMontageConfiguration::generateMontage(int peakMethodToUse, unsigned streamSubdivisions)
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

  ITKProgressObserver* progressObs = new ITKProgressObserver();
  progressObs->setFilter(this);

	using PeakInterpolationType = typename itk::MaxPhaseCorrelationOptimizer<PCMType>::PeakInterpolationMethod;
	using PeakFinderUnderlying = typename std::underlying_type<PeakInterpolationType>::type;
	auto peakMethod = static_cast<PeakFinderUnderlying>(peakMethodToUse);

  // Create tile montage
	typename MontageType::Pointer montage = MontageType::New();
	montage->SetMontageSize({ m_xMontageSize, m_yMontageSize });
	montage->GetModifiablePCM()->SetPaddingMethod(PCMType::PaddingMethod::MirrorWithExponentialDecay);
	montage->GetModifiablePCMOptimizer()->SetPeakInterpolationMethod(static_cast<PeakInterpolationType>(peakMethod));
  montage->SetOriginAdjustment(m_StageTiles[1][1].Position);
	montage->SetForcedSpacing(sp);

  // Set tile image data from DREAM3D structure into tile montage
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
			// Check the resolution and fix if necessary
			SIMPL::Tuple3FVec resolution = imageDC->getGeometryAs<ImageGeom>()->getResolution();
			if (std::get<0>(resolution) < 1 || std::get<1>(resolution) < 1)
			{
				imageDC->getGeometryAs<ImageGeom>()->setResolution(1, 1, 1);
			}

			toITK->SetInput(imageDC);
			toITK->SetInPlace(true);
			toITK->SetAttributeMatrixArrayName(getCommonAttributeMatrixName().toStdString());
			toITK->SetDataArrayName(getCommonDataArrayName().toStdString());
			toITK->Update();
			itk::Dream3DFilterInterruption::Pointer interruption = itk::Dream3DFilterInterruption::New();
			interruption->SetFilter(this);
			montage->AddObserver(itk::ProgressEvent(), interruption);
			montage->SetInputTile(ind, toITK->GetOutput());
		}
	}

  // Execute the tile registrations
	notifyStatusMessage(getHumanLabel(), "Doing the tile registrations");

  progressObs->setMessagePrefix("Registering Tiles");
  montage->AddObserver(itk::ProgressEvent(), progressObs);

	montage->Update();

  montage->RemoveAllObservers();
	notifyStatusMessage(getHumanLabel(), "Finished the tile registrations");

  // Store tile registration transforms in DREAM3D data containers
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

			image->setTransformContainer(convertedTransformContainer);
		}
	}

  if (m_StitchMontage)
  {
    // Stitch the montage together
    using Resampler = itk::TileMergeImageFilter<OriginalImageType, AccumulatePixelType>;
    typename Resampler::Pointer resampleF = Resampler::New();
    resampleF->SetMontage(montage);

    using Dream3DImageType = itk::Dream3DImage<PixelType, Dimension>;
    using StreamingFilterType = itk::StreamingImageFilter<OriginalImageType, Dream3DImageType>;
    typename StreamingFilterType::Pointer streamingFilter = StreamingFilterType::New();
    streamingFilter->SetInput(resampleF->GetOutput());
    streamingFilter->SetNumberOfStreamDivisions(streamSubdivisions);

    notifyStatusMessage(getHumanLabel(), "Resampling tiles into the stitched image");

    progressObs->setMessagePrefix("Stitching Tiles Together");
    resampleF->AddObserver(itk::ProgressEvent(), progressObs);

    streamingFilter->Update();
    notifyStatusMessage(getHumanLabel(), "Finished resampling tiles");
    notifyStatusMessage(getHumanLabel(), "Converting into DREAM3D data structure");

    resampleF->RemoveAllObservers();

    // Convert montaged image into DREAM3D data structure
    DataArrayPath dataArrayPath(getMontageDataContainerName(), getMontageAttributeMatrixName(), getMontageDataArrayName());
    DataContainer::Pointer container = getDataContainerArray()->getDataContainer(dataArrayPath.getDataContainerName());

    using ToDream3DType = itk::InPlaceImageToDream3DDataFilter<PixelType, Dimension>;
    typename ToDream3DType::Pointer toDream3DFilter = ToDream3DType::New();
    toDream3DFilter->SetInput(streamingFilter->GetOutput());
    toDream3DFilter->SetInPlace(true);
    toDream3DFilter->SetAttributeMatrixArrayName(dataArrayPath.getAttributeMatrixName().toStdString());
    toDream3DFilter->SetDataArrayName(dataArrayPath.getDataArrayName().toStdString());
    toDream3DFilter->SetDataContainer(container);
    toDream3DFilter->Update();
  }

  delete progressObs;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
DataContainer::Pointer GenerateMontageConfiguration::GetImageDataContainer(int y, int x)
{
	DataContainerArray* dca = getDataContainerArray().get();
	// Loop over the data containers until we find the proper data container
  QMutableListIterator<QString> dcNameIter(m_ImageDataContainers);
	QStringList dcList;
  while (dcNameIter.hasNext())
	{
    QString dcName = dcNameIter.next();
		dcList.push_back(dcName);
		DataContainer::Pointer dcItem = dca->getPrereqDataContainer(this, dcName);
		if (getErrorCondition() < 0 || dcItem.get() == nullptr)
		{
			continue;
		}

		QString rowCol = dcName.right(dcName.size() - dcName.lastIndexOf("_") - 1);
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
