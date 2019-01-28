/* ============================================================================
* Copyright (c) 2009-2016 BlueQuartz Software, LLC
*
* Redistribution and use in source and binary forms, with or without modification,
* are permitted provided that the following conditions are met:
*
* Redistributions of source code must retain the above copyright notice, this
* list of conditions and the following disclaimer.
*
* Redistributions in binary form must reproduce the above copyright notice, this
* list of conditions and the following disclaimer in the documentation and/or
* other materials provided with the distribution.
*
* Neither the name of BlueQuartz Software, the US Air Force, nor the names of its
* contributors may be used to endorse or promote products derived from this software
* without specific prior written permission.
*
* THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
* AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
* IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
* DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
* FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
* DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
* SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
* CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
* OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE
* USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*
* The code contained herein was partially funded by the followig contracts:
*    United States Air Force Prime Contract FA8650-07-D-5800
*    United States Air Force Prime Contract FA8650-10-D-5210
*    United States Prime Contract Navy N00173-07-C-2068
*
* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */

#include "ITKStitchMontage.h"

#include <sstream>

#include <QtCore/QDir>

#include "SIMPLib/Common/Constants.h"
#include "SIMPLib/FilterParameters/FloatFilterParameter.h"
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
#include "SIMPLib/ITK/itkDream3DITransformContainerToTransform.h"
#include "SIMPLib/ITK/itkDream3DTransformContainerToTransform.h"
#include "SIMPLib/ITK/itkInPlaceDream3DDataToImageFilter.h"
#include "SIMPLib/ITK/itkInPlaceImageToDream3DDataFilter.h"
#include "SIMPLib/ITK/itkDream3DFilterInterruption.h"
#include "SIMPLib/ITK/itkProgressObserver.hpp"

#include "ITKImageProcessing/ITKImageProcessingConstants.h"
#include "ITKImageProcessing/ITKImageProcessingVersion.h"

#include "itkTileMontage.h"
#include "itkTxtTransformIOFactory.h"
#include "itkTileMergeImageFilter.h"
#include "itkStreamingImageFilter.h"
#include "itkImageFileWriter.h"

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
ITKStitchMontage::ITKStitchMontage()
{
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
ITKStitchMontage::~ITKStitchMontage() = default;

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void ITKStitchMontage::initialize()
{
	setErrorCondition(0);
	setWarningCondition(0);
	setCancel(false);
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void ITKStitchMontage::setupFilterParameters()
{
	QVector<FilterParameter::Pointer> parameters;

  parameters.push_back(SIMPL_NEW_INT_VEC3_FP("Montage Size", MontageSize, FilterParameter::Parameter, ITKStitchMontage));
  
  {
	  QStringList linkedProps;
	  linkedProps << "TileOverlap";
    parameters.push_back(SIMPL_NEW_LINKED_BOOL_FP("Manual Tile Overlap", ManualTileOverlap, FilterParameter::Parameter, ITKStitchMontage, linkedProps));
  }

  {
    MultiDataContainerSelectionFilterParameter::RequirementType req =
    MultiDataContainerSelectionFilterParameter::CreateRequirement(SIMPL::Defaults::AnyPrimitive, SIMPL::Defaults::AnyComponentSize, AttributeMatrix::Type::Cell, IGeometry::Type::Image);
    parameters.push_back(SIMPL_NEW_MDC_SELECTION_FP("Image Data Containers", ImageDataContainers, FilterParameter::RequiredArray, ITKStitchMontage, req));
  }
  parameters.push_back(SIMPL_NEW_STRING_FP("Common Attribute Matrix", CommonAttributeMatrixName, FilterParameter::RequiredArray, ITKStitchMontage));
  parameters.push_back(SIMPL_NEW_STRING_FP("Common Data Array", CommonDataArrayName, FilterParameter::RequiredArray, ITKStitchMontage));

  parameters.push_back(SIMPL_NEW_STRING_FP("Montage Data Container Name", MontageDataContainerName, FilterParameter::CreatedArray, ITKStitchMontage));
  parameters.push_back(SIMPL_NEW_STRING_FP("Montage Attribute Matrix Name", MontageAttributeMatrixName, FilterParameter::CreatedArray, ITKStitchMontage));
  parameters.push_back(SIMPL_NEW_STRING_FP("Montage Data Array Name", MontageDataArrayName, FilterParameter::CreatedArray, ITKStitchMontage));

  parameters.push_back(SIMPL_NEW_FLOAT_FP("Tile Overlap (Percent)", TileOverlap, FilterParameter::RequiredArray, ITKStitchMontage));


	setFilterParameters(parameters);
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void ITKStitchMontage::dataCheck()
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

  QString dcName = m_ImageDataContainers[0];

  DataArrayPath testPath;
  testPath.setDataContainerName(dcName);
  testPath.setAttributeMatrixName(getCommonAttributeMatrixName());
  testPath.setDataArrayName(getCommonDataArrayName());

  AttributeMatrix::Pointer imageDataAM = getDataContainerArray()->getPrereqAttributeMatrixFromPath(this, testPath, err);
  if (getErrorCondition() < 0 || err < 0)
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

  IDataArray::Pointer imagePtr = getDataContainerArray()->getPrereqIDataArrayFromPath<IDataArray, AbstractFilter>(this, testPath);
  if (getErrorCondition() < 0)
  {
    return;
  }

  for (int i = 0; i < dcCount; i++)
  {
    QString dcName = m_ImageDataContainers[i];

    DataArrayPath testPath;
    testPath.setDataContainerName(dcName);
    testPath.setAttributeMatrixName(getCommonAttributeMatrixName());
    testPath.setDataArrayName(getCommonDataArrayName());

    ImageGeom::Pointer image = getDataContainerArray()->getPrereqGeometryFromDataContainer<ImageGeom, AbstractFilter>(this, testPath.getDataContainerName());
    if (getErrorCondition() < 0)
    {
      return;
    }

    if (getManualTileOverlap() && (getTileOverlap() < 0.0f || getTileOverlap() > 100.0f))
    {
      setErrorCondition(-11006);
      QString ss = QObject::tr("Tile Overlap must be between 0.0 and 100.0.");
      notifyErrorMessage(getHumanLabel(), ss, getErrorCondition());
      return;
    }
  }

  if (getMontageDataContainerName().isEmpty())
  {
    setErrorCondition(-11007);
    QString ss = QObject::tr("Montage Data Container is empty.");
    notifyErrorMessage(getHumanLabel(), ss, getErrorCondition());
    return;
  }

  if (getMontageAttributeMatrixName().isEmpty())
  {
    setErrorCondition(-11008);
    QString ss = QObject::tr("Montage Attribute Matrix is empty.");
    notifyErrorMessage(getHumanLabel(), ss, getErrorCondition());
    return;
  }

  if (getMontageDataArrayName().isEmpty())
  {
    setErrorCondition(-11009);
    QString ss = QObject::tr("Montage Data Array is empty.");
    notifyErrorMessage(getHumanLabel(), ss, getErrorCondition());
    return;
  }

  DataArrayPath dap(getMontageDataContainerName(), getMontageAttributeMatrixName(), getMontageDataArrayName());

  DataContainer::Pointer dc = getDataContainerArray()->createNonPrereqDataContainer(this, getMontageDataContainerName());
  if (getErrorCondition() < 0)
  {
    return;
  }

  size_t montageArrayXSize = imageDataTupleDims[0] * m_xMontageSize;
  size_t montageArrayYSize = imageDataTupleDims[1] * m_yMontageSize;

  ImageGeom::Pointer imageGeom = ImageGeom::New();
  imageGeom->setName("MontageGeometry");
  imageGeom->setDimensions(montageArrayXSize, montageArrayYSize, 1);
  dc->setGeometry(imageGeom);

  ss = QObject::tr("The image geometry dimensions of data container '%1' are projected to be (%2, %3, %4).  This is assuming "
                           "0% overlap between tiles, so the actual geometry dimensions after executing the stitching algorithm may be smaller.")
      .arg(dc->getName()).arg(montageArrayXSize).arg(montageArrayYSize).arg(1);
  setWarningCondition(-3001);
  notifyWarningMessage(getHumanLabel(), ss, getWarningCondition());

  QVector<size_t> tDims = {montageArrayXSize, montageArrayYSize, 1};

  AttributeMatrix::Pointer am = dc->createNonPrereqAttributeMatrix(this, dap.getAttributeMatrixName(), tDims, AttributeMatrix::Type::Cell);
  if (getErrorCondition() < 0)
  {
    return;
  }

  ss = QObject::tr("The tuple dimensions of attribute matrix '%1' are projected to be (%2, %3, %4).  This is assuming "
                   "0% overlap between tiles, so the actual geometry dimensions after executing the stitching algorithm may be smaller.")
      .arg(am->getName()).arg(montageArrayXSize).arg(montageArrayYSize).arg(1);
  setWarningCondition(-3002);
  notifyWarningMessage(getHumanLabel(), ss, getWarningCondition());

  if (getMontageDataArrayName().isEmpty())
  {
    QString ss = QObject::tr("The Montage Data Array Name field is empty.");
    setErrorCondition(-3003);
    notifyErrorMessage(getHumanLabel(), ss, getErrorCondition());
    return;
  }

  IDataArray::Pointer da = imagePtr->createNewArray(montageArrayXSize * montageArrayYSize, QVector<size_t>(1, 1), getMontageDataArrayName(), !getInPreflight());
  am->addAttributeArray(da->getName(), da);

  ss = QObject::tr("The number of elements of montage data array '%1' is projected to be %2.  This is assuming "
                   "0% overlap between tiles, so the actual geometry dimensions after executing the stitching algorithm may be smaller.")
      .arg(da->getName()).arg(QLocale::system().toString(static_cast<int>(da->getNumberOfTuples())));
  setWarningCondition(-3004);
  notifyWarningMessage(getHumanLabel(), ss, getWarningCondition());
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void ITKStitchMontage::preflight()
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
void ITKStitchMontage::execute()
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
    DataArrayPath dap(m_ImageDataContainers[0], getCommonAttributeMatrixName(), getCommonDataArrayName());
    DataContainer::Pointer dc = getDataContainerArray()->getDataContainer(m_ImageDataContainers[0]);
    AttributeMatrix::Pointer am = dc->getAttributeMatrix(getCommonAttributeMatrixName());
    IDataArray::Pointer da = am->getAttributeArray(getCommonDataArrayName());

    int numOfComponents = da->getNumberOfComponents();

    if (numOfComponents == 3)
    {
      stitchMontage< itk::RGBPixel< unsigned char >, itk::RGBPixel< unsigned int > >();
    }
    else if (numOfComponents == 4)
    {
      stitchMontage< itk::RGBAPixel< unsigned char >, itk::RGBAPixel< unsigned int > >();
    }
    else if (numOfComponents == 1)
    {
      stitchMontage< unsigned char, unsigned int >();
    }
    else
    {
      setErrorCondition(-5000);
      notifyErrorMessage(getHumanLabel(), "The common data array's image type is not recognized.  Supported image types"
                                          " are grayscale (1-component), RGB (3-component), and RGBA (4-component)",
                         getErrorCondition());
    }
	}
	/* Let the GUI know we are done with this filter */
	notifyStatusMessage(getHumanLabel(), "Complete");
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void ITKStitchMontage::createFijiDataStructure()
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
		for (unsigned i = 0; i < m_yMontageSize; i++)
		{
        m_StageTiles[i].resize(m_xMontageSize);
		}
	}

  float tileOverlapFactor = ((100.0 - getTileOverlap()) / 100.0);

  QVector<size_t> cDims;
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
	SIMPL::Tuple3SVec dimensions = image->getDimensions();
	SIMPL::Tuple3FVec origin = image->getOrigin();

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
  		if ((std::get<0>(origin) == 0.0f && std::get<1>(origin) == 0.0f) || getManualTileOverlap())
		{
			tile.Position[0] = col * (tileOverlapFactor * std::get<0>(dimensions));
			tile.Position[1] = row * (tileOverlapFactor * std::get<1>(dimensions));
			image->setOrigin(tile.Position[0], tile.Position[1], 0.0f);
		}
		else
		{
			tile.Position[0] = std::get<0>(origin);
			tile.Position[1] = std::get<1>(origin);
		}
    tile.FileName = "";   // This code gets its data from memory, not from a file

    m_StageTiles[row][col] = tile;

    int err;
    AttributeMatrix::Pointer am = dcItem->getPrereqAttributeMatrix(this, getCommonAttributeMatrixName(), err);
	}
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
template< typename PixelType, typename AccumulatePixelType >
void ITKStitchMontage::stitchMontage(int peakMethodToUse, unsigned streamSubdivisions)
{
  using ScalarPixelType = typename itk::NumericTraits< PixelType >::ValueType;
  constexpr unsigned Dimension = 2;
  using PointType = itk::Point<double, Dimension>;
  using ScalarImageType = itk::Dream3DImage< ScalarPixelType, Dimension >;
  using OriginalImageType = itk::Dream3DImage< PixelType, Dimension >; // possibly RGB instead of scalar
  using MontageType = itk::TileMontage< ScalarImageType >;
  using TransformType = itk::TranslationTransform< double, Dimension >;
  typename ScalarImageType::SpacingType sp;
  sp.Fill(1.0); // assume unit spacing

  typename MontageType::TileIndexType ind;

  PointType originAdjustment = m_StageTiles[m_xMontageSize - 1][m_yMontageSize - 1].Position;

  // Stitch the montage together
  using Resampler = itk::TileMergeImageFilter<OriginalImageType, AccumulatePixelType>;
  typename Resampler::Pointer resampleF = Resampler::New();
  //resampleF->SetMontage(montage); // doesn't compile, because montage is expected
  // to be templated using itk::Image, not itk::Dream3DImage

  resampleF->SetMontageSize({m_xMontageSize, m_yMontageSize});
  resampleF->SetOriginAdjustment(originAdjustment);
  resampleF->SetForcedSpacing(sp);

  for(unsigned y = 0; y < m_yMontageSize; y++)
  {
    ind[1] = y;
    for(unsigned x = 0; x < m_xMontageSize; x++)
    {
      ind[0] = x;
      typedef itk::InPlaceDream3DDataToImageFilter<PixelType, Dimension> toITKType;
      typename toITKType::Pointer toITK = toITKType::New();
      DataContainer::Pointer imageDC = GetImageDataContainer(y, x);
      // Check the resolution and fix if necessary
      ImageGeom::Pointer geom = imageDC->getGeometryAs<ImageGeom>();
      geom->setResolution(1, 1, 1);
      geom->setOrigin(0, 0, 0);

      toITK->SetInput(imageDC);
      toITK->SetInPlace(true);
      toITK->SetAttributeMatrixArrayName(getCommonAttributeMatrixName().toStdString());
      toITK->SetDataArrayName(getCommonDataArrayName().toStdString());
      toITK->Update();

      typename OriginalImageType::Pointer image = toITK->GetOutput();

      resampleF->SetInputTile(ind, image);

      using FilterType = itk::Dream3DITransformContainerToTransform<double, 3>;
      ::ITransformContainer::Pointer transformContainer = geom->getTransformContainer();
      FilterType::Pointer filter = FilterType::New();
      filter->SetInput(transformContainer);
      filter->Update();

      AffineType::Pointer itkAffine = dynamic_cast<AffineType*>(filter->GetOutput()->Get().GetPointer());
      AffineType::TranslationType t = itkAffine->GetTranslation();
      TransformType::Pointer regTr = TransformType::New();
      auto offset = regTr->GetOffset();
      for (unsigned i = 0; i < TransformType::SpaceDimension; i++)
      {
        offset[i] = t[i];
      }

      regTr->SetOffset(offset);

      resampleF->SetTileTransform(ind, regTr);
    }
  }

  using Dream3DImageType = itk::Dream3DImage<PixelType, Dimension>;
  using StreamingFilterType = itk::StreamingImageFilter<OriginalImageType, Dream3DImageType>;
  typename StreamingFilterType::Pointer streamingFilter = StreamingFilterType::New();
  streamingFilter->SetInput(resampleF->GetOutput());
  streamingFilter->SetNumberOfStreamDivisions(streamSubdivisions);

  notifyStatusMessage(getHumanLabel(), "Resampling tiles into the stitched image");

  itk::ProgressObserver* progressObs = new itk::ProgressObserver();
  progressObs->setFilter(this);
  progressObs->setMessagePrefix("Stitching Tiles Together");
  resampleF->AddObserver(itk::ProgressEvent(), progressObs);

  resampleF->Update();
  notifyStatusMessage(getHumanLabel(), "Finished resampling tiles");
  notifyStatusMessage(getHumanLabel(), "Converting into DREAM3D data structure");

  resampleF->RemoveAllObservers();

  // Convert montaged image into DREAM3D data structure
  DataArrayPath dataArrayPath(getMontageDataContainerName(), getMontageAttributeMatrixName(), getMontageDataArrayName());
  DataContainer::Pointer container = getDataContainerArray()->getDataContainer(dataArrayPath.getDataContainerName());

  using ToDream3DType = itk::InPlaceImageToDream3DDataFilter<PixelType, Dimension>;
  typename ToDream3DType::Pointer toDream3DFilter = ToDream3DType::New();
  toDream3DFilter->SetInput(resampleF->GetOutput());
  toDream3DFilter->SetInPlace(true);
  toDream3DFilter->SetAttributeMatrixArrayName(dataArrayPath.getAttributeMatrixName().toStdString());
  toDream3DFilter->SetDataArrayName(dataArrayPath.getDataArrayName().toStdString());
  toDream3DFilter->SetDataContainer(container);
  toDream3DFilter->Update();

  delete progressObs;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
DataContainer::Pointer ITKStitchMontage::GetImageDataContainer(int y, int x)
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
typename TransformContainer::Pointer ITKStitchMontage::GetTransformContainerFromITKAffineTransform(const AffineType::Pointer& itkAffine)
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
AbstractFilter::Pointer ITKStitchMontage::newFilterInstance(bool copyFilterParameters) const
{
  ITKStitchMontage::Pointer filter = ITKStitchMontage::New();
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
const QString ITKStitchMontage::getCompiledLibraryName() const
{
	return ITKImageProcessingConstants::ITKImageProcessingBaseName;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
const QString ITKStitchMontage::getBrandingString() const
{
	return "ITKImageProcessing";
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
const QString ITKStitchMontage::getFilterVersion() const
{
	QString version;
	QTextStream vStream(&version);
	vStream << ITKImageProcessing::Version::Major() << "." << ITKImageProcessing::Version::Minor() << "." << ITKImageProcessing::Version::Patch();
	return version;
}
// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
const QString ITKStitchMontage::getGroupName() const
{
	return SIMPL::FilterGroups::IOFilters;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
const QUuid ITKStitchMontage::getUuid()
{
  return QUuid("{fa4efd40-f4a6-5524-9fc6-e1f8bbb2c42f}");
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
const QString ITKStitchMontage::getSubGroupName() const
{
	return SIMPL::FilterSubGroups::GenerationFilters;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
const QString ITKStitchMontage::getHumanLabel() const
{
	return "Generate Montage Configuration";
}
