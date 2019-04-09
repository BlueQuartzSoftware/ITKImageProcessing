/* ============================================================================
 * Copyright (c) 2009-2019 BlueQuartz Software, LLC
 * All rights reserved.
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
 * Neither the names of any of the BlueQuartz Software contributors
 * may be used to endorse or promote products derived from this software without
 * specific prior written permission.
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
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
#include "CalculateBackground.h"

#include <cstring>

#include <set>

#include <QtCore/QString>
#include <QtCore/QFileInfo>
#include <QtCore/QDir>
#include <QtCore/QSharedPointer>
#include <QtCore/QString>
#include <QtCore/QVector>
#include <QtCore/QTextStream>

#include "SIMPLib/FilterParameters/AbstractFilterParametersReader.h"
#include "SIMPLib/FilterParameters/AttributeMatrixCreationFilterParameter.h"
#include "SIMPLib/FilterParameters/AttributeMatrixSelectionFilterParameter.h"
#include "SIMPLib/FilterParameters/BooleanFilterParameter.h"
#include "SIMPLib/FilterParameters/DataArrayCreationFilterParameter.h"
#include "SIMPLib/FilterParameters/DataContainerCreationFilterParameter.h"
#include "SIMPLib/FilterParameters/IntFilterParameter.h"
#include "SIMPLib/FilterParameters/MultiDataContainerSelectionFilterParameter.h"
#include "SIMPLib/FilterParameters/SeparatorFilterParameter.h"
#include "SIMPLib/FilterParameters/StringFilterParameter.h"
#include "SIMPLib/Filtering/FilterManager.h"
#include "SIMPLib/Geometry/IGeometryGrid.h"
#include "SIMPLib/Geometry/ImageGeom.h"

#include "ZeissImport/ZeissImportConstants.h"
#include "ZeissImport/ZeissImportVersion.h"
#include "ZeissImport/ZeissXml/ZeissTagMapping.h"


#include <Eigen/Dense>

/* Create Enumerations to allow the created Attribute Arrays to take part in renaming */
enum createdPathID : RenameDataPath::DataID_t
{
  DataContainerID10 = 10,
  AttributeMatrixID20 = 20,
  DataArrayID30 = 30,
};

#define ZIF_PRINT_DBG_MSGS 0

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
CalculateBackground::CalculateBackground()
: m_DataContainers("")
, m_CellAttributeMatrixName(SIMPL::Defaults::CellAttributeMatrixName)
, m_ImageDataArrayName("")
, m_OutputDataContainerPath("Background")
, m_OutputCellAttributeMatrixPath("Background", "Background Data", "")
, m_OutputImageArrayPath("Background", "Background Data", "Background Image")
, m_lowThresh(0)
, m_highThresh(255)
, m_SubtractBackground(false)
, m_DivideBackground(false)
{
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
CalculateBackground::~CalculateBackground() = default;

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void CalculateBackground::setupFilterParameters()
{
  FilterParameterVectorType parameters;

  MultiDataContainerSelectionFilterParameter::RequirementType req;
  req.dcGeometryTypes.push_back(IGeometry::Type::Image);
  req.dcGeometryTypes.push_back(IGeometry::Type::RectGrid);
  parameters.push_back(SIMPL_NEW_MDC_SELECTION_FP("Select Image Data Containers", DataContainers, FilterParameter::Parameter, CalculateBackground, req));
  parameters.push_back(SIMPL_NEW_STRING_FP("Input Attribute Matrix Name", CellAttributeMatrixName, FilterParameter::RequiredArray, CalculateBackground));
  parameters.push_back(SIMPL_NEW_STRING_FP("Input Image Array Name", ImageDataArrayName, FilterParameter::RequiredArray, CalculateBackground));

  parameters.push_back(SIMPL_NEW_DC_CREATION_FP("Created Data Container", OutputDataContainerPath, FilterParameter::CreatedArray, CalculateBackground));
  AttributeMatrixCreationFilterParameter::RequirementType cellAmReq;
  cellAmReq.dcGeometryTypes.push_back(IGeometry::Type::Image);
  cellAmReq.dcGeometryTypes.push_back(IGeometry::Type::RectGrid);
  parameters.push_back(SIMPL_NEW_AM_CREATION_FP("Created Background Attribute Matrix", OutputCellAttributeMatrixPath, FilterParameter::CreatedArray, CalculateBackground, cellAmReq));

  DataArrayCreationFilterParameter::RequirementType imageReq;
  imageReq.dcGeometryTypes.push_back(IGeometry::Type::Image);
  imageReq.dcGeometryTypes.push_back(IGeometry::Type::RectGrid);
  imageReq.amTypes.push_back(AttributeMatrix::Type::Cell);
  parameters.push_back(SIMPL_NEW_DA_CREATION_FP("Created Background Image Array Name", OutputImageArrayPath, FilterParameter::CreatedArray, CalculateBackground, imageReq));

  parameters.push_back(SIMPL_NEW_INTEGER_FP("Lowest allowed Image value (Image Value)", lowThresh, FilterParameter::Parameter, CalculateBackground));
  parameters.push_back(SIMPL_NEW_INTEGER_FP("Highest allowed Image value (Image Value)", highThresh, FilterParameter::Parameter, CalculateBackground));

  parameters.push_back(SIMPL_NEW_BOOL_FP("Subtract Background from Current Images", SubtractBackground, FilterParameter::Parameter, CalculateBackground));
  parameters.push_back(SIMPL_NEW_BOOL_FP("Divide Background from Current Images", DivideBackground, FilterParameter::Parameter, CalculateBackground));

  setFilterParameters(parameters);
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void CalculateBackground::initialize()
{
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void CalculateBackground::dataCheck()
{
  clearErrorCode();
  clearWarningCode();
  initialize();

  DataArrayPath tempPath;

  DataContainerArray::Pointer dca = getDataContainerArray();
  QVector<size_t> cDims = {1};

  ImageGeom::Pointer outputImageGeom = ImageGeom::NullPointer();
  QString inputDcName;

  // Ensure each DataContainer has the proper path to the image data and the image data is grayscale
  for(const auto& dcName : m_DataContainers)
  {
    if(inputDcName.isEmpty())
    {
      inputDcName = dcName;
    }
    DataArrayPath imageArrayPath(dcName, m_CellAttributeMatrixName, m_ImageDataArrayName);
    UInt8ArrayType::Pointer imageData = dca->getPrereqArrayFromPath<UInt8ArrayType, AbstractFilter>(this, imageArrayPath, cDims);
    if(imageData.get() == nullptr)
    {
      QString msg;
      QTextStream out(&msg);
      out << "Attribute Array Path: " << imageArrayPath.serialize() << " is not UInt8{1} (Grayscale) data. Please select a pattern of AttributeArray Paths that are gray scale images";
      setErrorCondition(-53000, msg);
    }

    if(getErrorCode() >= 0)
    {
      ImageGeom::Pointer imageGeom = dca->getDataContainer(dcName)->getGeometryAs<ImageGeom>();
      if(imageGeom.get() != nullptr)
      {
        if(outputImageGeom.get() == nullptr)
        {
          outputImageGeom = std::dynamic_pointer_cast<ImageGeom>(imageGeom->deepCopy());
        }
      }
      else
      {
        QString msg;
        QTextStream out(&msg);
        out << "DataContainer: " << dcName << " needs to have an ImageGeometry assigned. There is either no geometry assign to the Data Container or the Geometry is not of type ImageGeom.";
        setErrorCondition(-53001, msg);
      }
    }
  }

  if(getErrorCode() < 0)
  {
    return;
  }

  DataContainer::Pointer outputDc = dca->createNonPrereqDataContainer(this, getOutputDataContainerPath(), DataContainerID10);
  if(getErrorCode() < 0)
  {
    return;
  }

  // Check for empty list. If list is empty then the OutputGeometry was never formed and it wont help to go on..
  if(m_DataContainers.isEmpty())
  {
    return;
  }

  outputDc->setGeometry(outputImageGeom);

  QVector<size_t> tDims = {0, 0, 0};
  std::tie(tDims[0], tDims[1], tDims[2]) = outputImageGeom->getDimensions();

  AttributeMatrix::Pointer outputAttrMat = outputDc->createNonPrereqAttributeMatrix(this, getOutputCellAttributeMatrixPath(), tDims, AttributeMatrix::Type::Cell, AttributeMatrixID20);

  UInt8ArrayType::Pointer outputImageArray = outputAttrMat->createNonPrereqArray<UInt8ArrayType>(this, m_OutputImageArrayPath.getDataArrayName(), 0, cDims, DataArrayID30);
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void CalculateBackground::preflight()
{
  // These are the REQUIRED lines of CODE to make sure the filter behaves correctly
  setInPreflight(true); // Set the fact that we are preflighting.
  emit preflightAboutToExecute(); // Emit this signal so that other widgets can do one file update
  emit updateFilterParameters(this); // Emit this signal to have the widgets push their values down to the filter
  dataCheck(); // Run our DataCheck to make sure everthing is setup correctly
  emit preflightExecuted(); // We are done preflighting this filter
  setInPreflight(false); // Inform the system this filter is NOT in preflight mode anymore.
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void CalculateBackground::execute()
{
  int err = 0;
  int xval = 0;
  int yval = 0;
  // typically run your dataCheck function to make sure you can get that far and all your variables are initialized
  dataCheck();
  // Check to make sure you made it through the data check. Errors would have been reported already so if something
  // happens to fail in the dataCheck() then we simply return
  if(getErrorCode() < 0)
  {
    return;
  }

  DataContainerArray::Pointer dca = getDataContainerArray();

  DataContainer::Pointer outputDc = dca->getDataContainer(getOutputDataContainerPath());
  AttributeMatrix::Pointer outputAttrMat = outputDc->getAttributeMatrix(getOutputCellAttributeMatrixPath());
  UInt8ArrayType::Pointer outputArrayPtr = outputAttrMat->getAttributeArrayAs<UInt8ArrayType>(m_OutputImageArrayPath.getDataArrayName());
  UInt8ArrayType& outputArray = *(outputArrayPtr);

  ImageGeom::Pointer outputGeom = outputDc->getGeometryAs<ImageGeom>();
  SizeVec3Type dims;
  outputGeom->getDimensions(dims);

  UInt64ArrayType::Pointer accumulateArrayPtr = UInt64ArrayType::CreateArray(outputArrayPtr->getNumberOfTuples(), "Accumulation Array", true);
  accumulateArrayPtr->initializeWithZeros();
  UInt64ArrayType& accumArray = *accumulateArrayPtr;
  size_t numTuples = accumArray.getNumberOfTuples();

  SizeTArrayType::Pointer countArrayPtr = SizeTArrayType::CreateArray(outputArrayPtr->getNumberOfTuples(), "Count Array", true);
  SizeTArrayType& counter = *(countArrayPtr);

  for(const auto& dcName : m_DataContainers)
  {
    DataArrayPath imageArrayPath(dcName, m_CellAttributeMatrixName, m_ImageDataArrayName);

    UInt8ArrayType& imageArray = *(dca->getAttributeMatrix(imageArrayPath)->getAttributeArrayAs<UInt8ArrayType>(imageArrayPath.getDataArrayName()));

    for(size_t t = 0; t < numTuples; t++)
    {
      if(imageArray[t] >= m_lowThresh && imageArray[t] <= m_highThresh)
      {
        accumArray[t] = accumArray[t] + imageArray[t];
        counter[t]++;
      }
    }
  }

  // average the background values by the number of counts (counts will be the number of images unless the threshold
  // values do not include all the possible image values
  // (i.e. for an 8 bit image, if we only include values from 0 to 100, not every image value will be counted)
  for(size_t j = 0; j < numTuples; j++)
  {
    accumArray[j] = accumArray[j] /= counter[j];
  }

#if 0
  // Fit the background to a second order polynomial
  // p are the coefficients p[0] + p[1]*x + p[2]*y +p[3]*xy + p[4]*x^2 + p[5]*y^2
  Eigen::MatrixXd A(numTuples, ZeissImportConstants::PolynomialOrder::NumConsts2ndOrder);
  Eigen::VectorXd B(numTuples);

  for(size_t i = 0; i < numTuples; ++i)
  {
    xval = static_cast<int>(i / dims[0]);
    yval = static_cast<int>(i % dims[0]);
    B(i) = static_cast<double>(accumArray[i]);
    A(i, 0) = 1.0;
    A(i, 1) = static_cast<double>(xval);
    A(i, 2) = static_cast<double>(yval);
    A(i, 3) = static_cast<double>(xval * yval);
    A(i, 4) = static_cast<double>(xval * xval);
    A(i, 5) = static_cast<double>(yval * yval);
  }

  notifyStatusMessage("Fitting a polynomial to data. May take a while to solve if images are large");
  Eigen::VectorXd p = A.colPivHouseholderQr().solve(B);

  QVector<size_t> tDims(3);
  tDims[0] = dims[0];
  tDims[1] = dims[1];
  tDims[2] = dims[2];

  //  m->getAttributeMatrix(getOutputCellAttributeMatrixPath())->resizeAttributeArrays(tDims);
  //  if(nullptr != m_BackgroundImagePtr.lock())                          /* Validate the Weak Pointer wraps a non-nullptr pointer to a DataArray<T> object */
  //  { m_BackgroundImage = m_BackgroundImagePtr.lock()->getPointer(0); } /* Now assign the raw pointer to data from the DataArray<T> object */

  Eigen::VectorXd Bcalc(numTuples);
  double average = 0;

  Bcalc = A * p;
  average = Bcalc.mean();
  Bcalc = Bcalc - Eigen::VectorXd::Constant(numTuples, average);

  for(int i = 0; i < numTuples; ++i)
  {
    accumArray[i] = Bcalc(i);
  }
#endif

  for(int i = 0; i < numTuples; ++i)
  {
    outputArray[i] = static_cast<uint8_t>(accumArray[i]);
  }

#if 0
  if(m_SubtractBackground)
  {
    for(size_t i = 0; i < names.size(); i++)
    {
      m_ImageDataArrayName.update(getDataContainerName(), getAttributeMatrixName().getAttributeMatrixName(), names[i]);
      iDataArray = getDataContainerArray()->getPrereqIDataArrayFromPath<DataArray<uint8_t>, AbstractFilter>(this, m_ImageDataArrayName);
      imagePtr = std::dynamic_pointer_cast<DataArray<uint8_t> >(iDataArray);
      if(nullptr != imagePtr.get())
      {
        image = imagePtr->getPointer(0);

        for(int64_t t = 0; t < m_TotalPoints; t++)
        {
          if (static_cast<uint8_t>(image[t]) >= m_lowThresh && static_cast<uint8_t>(image[t])  <= m_highThresh)
          {
            image[t] = image[t] - Bcalc(t);

            if (image[t] < 0) {image[t] = 0;}
            if (image[t] > 255) {image[t] = 255;}

          }
        }

      }
    }
  }

  if(m_DivideBackground)
  {
    for(size_t i = 0; i < names.size(); i++)
    {
      m_ImageDataArrayName.update(getDataContainerName(), getAttributeMatrixName().getAttributeMatrixName(), names[i]);
      iDataArray = getDataContainerArray()->getPrereqIDataArrayFromPath<DataArray<uint8_t>, AbstractFilter>(this, m_ImageDataArrayName);
      imagePtr = std::dynamic_pointer_cast<DataArray<uint8_t> >(iDataArray);
      if(nullptr != imagePtr.get())
      {
        image = imagePtr->getPointer(0);

        for(int64_t t = 0; t < m_TotalPoints; t++)
        {
          if (static_cast<uint8_t>(image[t]) >= m_lowThresh && static_cast<uint8_t>(image[t])  <= m_highThresh)
          {
            image[t] = image[t] / Bcalc(t);

          }
        }

      }
    }
  }
#endif
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
const QString CalculateBackground::getCompiledLibraryName() const
{
  return ZeissImportConstants::ZeissImportBaseName;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
const QString CalculateBackground::getBrandingString() const
{
  return "ZeissImport";
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
const QString CalculateBackground::getFilterVersion() const
{
  QString version;
  QTextStream vStream(&version);
  vStream <<  ZeissImport::Version::Major() << "." << ZeissImport::Version::Minor() << "." << ZeissImport::Version::Patch();
  return version;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
const QString CalculateBackground::getGroupName() const
{
  return SIMPL::FilterGroups::Unsupported;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
const QString CalculateBackground::getHumanLabel() const
{
  return "Calculate Background";
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
const QUuid CalculateBackground::getUuid()
{
  return QUuid("{a48f7a51-0ca9-584f-a0ca-4bfebdc41d7c}");
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
const QString CalculateBackground::getSubGroupName() const
{
  return "Misc";
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
AbstractFilter::Pointer CalculateBackground::newFilterInstance(bool copyFilterParameters) const
{
  /*
  * write code to optionally copy the filter parameters from the current filter into the new instance
  */
  CalculateBackground::Pointer filter = CalculateBackground::New();
  if(copyFilterParameters)
  {
    /* If the filter uses all the standard Filter Parameter Widgets you can probabaly get
     * away with using this method to copy the filter parameters from the current instance
     * into the new instance
     */
    copyFilterParameterInstanceVariables(filter.get());
    /* If your filter is using a lot of custom FilterParameterWidgets @see ReadH5Ebsd then you
     * may need to copy each filter parameter explicitly plus any other instance variables that
     * are needed into the new instance. Here is some example code from ReadH5Ebsd
     */
    //    SIMPL_COPY_INSTANCEVAR(OutputFile)
    //    SIMPL_COPY_INSTANCEVAR(ZStartIndex)
    //    SIMPL_COPY_INSTANCEVAR(ZEndIndex)
    //    SIMPL_COPY_INSTANCEVAR(ZResolution)
  }
  return filter;
}

