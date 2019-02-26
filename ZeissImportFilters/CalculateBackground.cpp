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
#include "SIMPLib/FilterParameters/AttributeMatrixSelectionFilterParameter.h"
#include "SIMPLib/FilterParameters/BooleanFilterParameter.h"
#include "SIMPLib/FilterParameters/IntFilterParameter.h"
#include "SIMPLib/FilterParameters/SeparatorFilterParameter.h"
#include "SIMPLib/FilterParameters/StringFilterParameter.h"
#include "SIMPLib/Filtering/FilterManager.h"

#include "ZeissImport/ZeissImportConstants.h"
#include "ZeissImport/ZeissImportVersion.h"
#include "ZeissImport/ZeissXml/ZeissTagMapping.h"


#include <Eigen/Dense>


#define ZIF_PRINT_DBG_MSGS 0

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
CalculateBackground::CalculateBackground()
: m_VolumeDataContainerName("")
, m_BackgroundAttributeMatrixName("")
, m_CellAttributeMatrixName(SIMPL::Defaults::CellAttributeMatrixName)
, m_ImageDataArrayPath("", "", "")
, m_AttributeMatrixName(SIMPL::Defaults::DataContainerName, SIMPL::Defaults::CellFeatureAttributeMatrixName, "")
, m_DataContainerBundleName("")
, m_BackgroundImageArrayName(getDataContainerBundleName() + "BackgroundImage")
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
  //parameters.push_back(DataBundleSelectionFilterParameter::New("DataContainerBundle Name", "DataContainerBundleName", getDataContainerBundleName(), FilterParameter::Uncategorized, SIMPL_BIND_SETTER(CalculateBackground, this, DataContainerBundleName), SIMPL_BIND_GETTER(CalculateBackground, this, DataContainerBundleName)));
  {
    AttributeMatrixSelectionFilterParameter::RequirementType req;
    parameters.push_back(SIMPL_NEW_AM_SELECTION_FP("Input AttributeMatrix Name", AttributeMatrixName, FilterParameter::RequiredArray, CalculateBackground, req));
  }
  parameters.push_back(SIMPL_NEW_INTEGER_FP("Lowest allowed Image value (Image Value)", lowThresh, FilterParameter::Parameter, CalculateBackground));
  parameters.push_back(SIMPL_NEW_INTEGER_FP("Highest allowed Image value (Image Value)", highThresh, FilterParameter::Parameter, CalculateBackground));
  //    parameters.push_back(SeparatorFilterParameter::New("Created Information", FilterParameter::Uncategorized));
  //    parameters.push_back(SIMPL_NEW_DC_CREATION_FP("Volume Data Container", VolumeDataContainerName, FilterParameter::Uncategorized, CalculateBackground));
  parameters.push_back(SIMPL_NEW_STRING_FP("Background Attribute Matrix", BackgroundAttributeMatrixName, FilterParameter::CreatedArray, CalculateBackground));
  parameters.push_back(SIMPL_NEW_STRING_FP("Background Image Array Name", BackgroundImageArrayName, FilterParameter::CreatedArray, CalculateBackground));
  parameters.push_back(SIMPL_NEW_BOOL_FP("Subtract Background from Current Images", SubtractBackground, FilterParameter::Parameter, CalculateBackground));
  parameters.push_back(SIMPL_NEW_BOOL_FP("Divide Background from Current Images", DivideBackground, FilterParameter::Parameter, CalculateBackground));

  setFilterParameters(parameters);
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void CalculateBackground::readFilterParameters(AbstractFilterParametersReader* reader, int index)
{
  reader->openFilterGroup(this, index);
  setAttributeMatrixName(reader->readDataArrayPath("AttributeMatrixName", getAttributeMatrixName()));
  //    setVolumeDataContainerName(reader->readString("VolumeDataContainerName", getVolumeDataContainerName() ) );
  setBackgroundAttributeMatrixName(reader->readString("BackgroundAttributeMatrixName", getBackgroundAttributeMatrixName()));
  setBackgroundImageArrayName(reader->readString("BackgroundImageArrayName", getBackgroundImageArrayName()));
  setDataContainerBundleName(reader->readString("DataContainerBundleName", getDataContainerBundleName() ) );
  setlowThresh(reader->readValue("lowThresh", getlowThresh()) );
  sethighThresh(reader->readValue("highThresh", gethighThresh()) );
  setSubtractBackground(reader->readValue("SubtractBackground", getSubtractBackground()));
  setDivideBackground(reader->readValue("DivideBackground", getDivideBackground()));
  reader->closeFilterGroup();
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void CalculateBackground::initialize()
{
  m_TotalPoints = 0;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void CalculateBackground::dataCheck()
{
  setErrorCondition(0);
  setWarningCondition(0);
  initialize();

  DataArrayPath tempPath;

  QString ss;

  AttributeMatrix::Pointer am = getDataContainerArray()->getAttributeMatrix(m_AttributeMatrixName);

  if (am.get() == nullptr)
  {
    setErrorCondition(-76000);
    notifyErrorMessage(getHumanLabel(), "The Attribute Matrix for property 'Input AttributeMatrix Name' has not been selected properly", -76000);
    return;
  }

  QList<QString> names = am->getAttributeArrayNames();


  QVector<size_t> dims(1, 1);


  UInt8ArrayType::Pointer imagePtr = UInt8ArrayType::NullPointer();
  IDataArray::Pointer iDataArray = IDataArray::NullPointer();

  for(int i = 0; i < names.size(); i++)
  {
    m_ImageDataArrayPath.update(getAttributeMatrixName().getDataContainerName(), getAttributeMatrixName().getAttributeMatrixName(), names[i]);
    iDataArray = getDataContainerArray()->getPrereqIDataArrayFromPath<DataArray<uint8_t>, AbstractFilter>(this, m_ImageDataArrayPath);

    imagePtr = std::dynamic_pointer_cast<DataArray<uint8_t> >(iDataArray);
    //        QVector<IDataArray::Pointer> pointerList;

    //        pointerList[i] = iDataArray;


    //        imagePtr = std::dynamic_pointer_cast<DataArray<uint8_t> >(pointerList[i]);


    if(nullptr == imagePtr)
    {
      setErrorCondition(-76001);
      notifyErrorMessage(getHumanLabel(), "The data was not found", -76001);
    }


  }

  if(m_SubtractBackground && m_DivideBackground)
  {
    setErrorCondition(-76002);
    notifyErrorMessage(getHumanLabel(), "Cannot choose BOTH subtract and divide. Choose one or neither.", -76002);
  }


  if(getErrorCondition() < 0) { return; }
  m_TotalPoints = imagePtr->getNumberOfTuples();

  setDataContainerName(getAttributeMatrixName().getDataContainerName());
  DataContainer::Pointer m = getDataContainerArray()->getPrereqDataContainer(this, getDataContainerName(), false);
  if(getErrorCondition() < 0 || nullptr == m) { return; }

  QVector<size_t> tDims(1, 0);
  AttributeMatrix::Pointer backgroundAttrMat = m->createNonPrereqAttributeMatrix(this, getBackgroundAttributeMatrixName(), tDims, AttributeMatrix::Type::Cell);
  if(getErrorCondition() < 0) { return; }


  // Background Image array
  dims[0] = 1;
  tempPath.update(getDataContainerName(), getBackgroundAttributeMatrixName(), getBackgroundImageArrayName() );
  m_BackgroundImagePtr = getDataContainerArray()->createNonPrereqArrayFromPath<DataArray<double>, AbstractFilter>(this, tempPath, 0, dims); /* Assigns the shared_ptr<> to an instance variable that is a weak_ptr<> */
  if(nullptr != m_BackgroundImagePtr.lock())                          /* Validate the Weak Pointer wraps a non-nullptr pointer to a DataArray<T> object */
  { m_BackgroundImage = m_BackgroundImagePtr.lock()->getPointer(0); } /* Now assign the raw pointer to data from the DataArray<T> object */
  if(getErrorCondition() < 0) { return; }




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
  if(getErrorCondition() < 0) { return; }
  setErrorCondition(0);
  setWarningCondition(0);

  /* If some error occurs this code snippet can report the error up the call chain*/
  if (err < 0)
  {
    QString ss = QObject::tr("Error Importing a Zeiss AxioVision file set.");
    setErrorCondition(-90000);
    notifyErrorMessage(getHumanLabel(), ss, getErrorCondition());
    return;
  }

  AttributeMatrix::Pointer am = getDataContainerArray()->getAttributeMatrix(m_AttributeMatrixName);


  QList<QString> names = am->getAttributeArrayNames();

  UInt8ArrayType::Pointer imagePtr = UInt8ArrayType::NullPointer();
  IDataArray::Pointer iDataArray = IDataArray::NullPointer();
  uint8_t* image = nullptr;

  std::vector<double> background(m_TotalPoints, 0);
  std::vector<double> counter(m_TotalPoints, 0);


  // getting the fist data container just to get the dimensions of each image.
  DataContainer::Pointer m = getDataContainerArray()->getDataContainer(getDataContainerName());


  QVector<size_t> udims;
  udims = am->getTupleDimensions();


  int64_t dims[3] =
  {
    static_cast<int64_t>(udims[0]),
    static_cast<int64_t>(udims[1]),
    static_cast<int64_t>(udims[2]),
  };

  // run through all the data containers (images) and add them up to be averaged after the loop
  for(size_t i = 0; i < names.size(); i++)
  {
    m_ImageDataArrayPath.update(getDataContainerName(), getAttributeMatrixName().getAttributeMatrixName(), names[i]);
    iDataArray = getDataContainerArray()->getPrereqIDataArrayFromPath<DataArray<uint8_t>, AbstractFilter>(this, m_ImageDataArrayPath);
    imagePtr = std::dynamic_pointer_cast<DataArray<uint8_t> >(iDataArray);
    if(nullptr != imagePtr.get())
    {
      //            int64_t totalPoints = imagePtr->getNumberOfTuples();
      image = imagePtr->getPointer(0);
      for(int64_t t = 0; t < m_TotalPoints; t++)
      {

        if (static_cast<uint8_t>(image[t]) >= m_lowThresh && static_cast<uint8_t>(image[t])  <= m_highThresh)
        {
          background[t] = background[t] + static_cast<double>(image[t]);
          counter[t]++;
        }
      }
    }
  }

  // average the background values by the number of counts (counts will be the number of images unless the threshold values do not include all the possible image values
  // (i.e. for an 8 bit image, if we only include values from 0 to 100, not every image value will be counted)

  for (int64_t j = 0; j < m_TotalPoints; j++)
  {
    background[j] = double(background[j] /= (counter[j]));
  }


  // Fit the background to a second order polynomial
  // p are the coefficients p[0] + p[1]*x + p[2]*y +p[3]*xy + p[4]*x^2 + p[5]*y^2
  Eigen::MatrixXd A(m_TotalPoints, ZeissImportConstants::PolynomialOrder::NumConsts2ndOrder);
  Eigen::VectorXd B(m_TotalPoints);

  for(int i = 0; i < m_TotalPoints; ++i)
  {
    xval = int(i / dims[0]);
    yval = int(i % dims[0]);
    B(i) = background[i];
    A(i, 0) = 1;
    A(i, 1) = xval;
    A(i, 2) = yval;
    A(i, 3) = xval * yval;
    A(i, 4) = xval * xval;
    A(i, 5) = yval * yval;
  }

  notifyStatusMessage(getHumanLabel(), "Fitting a polynomial to data. May take a while to solve if images are large");
  Eigen::VectorXd p = A.colPivHouseholderQr().solve(B);

  QVector<size_t> tDims(3);
  tDims[0] = dims[0];
  tDims[1] = dims[1];
  tDims[2] = dims[2];
  m->getAttributeMatrix(getBackgroundAttributeMatrixName())->resizeAttributeArrays(tDims);
  if(nullptr != m_BackgroundImagePtr.lock())                          /* Validate the Weak Pointer wraps a non-nullptr pointer to a DataArray<T> object */
  { m_BackgroundImage = m_BackgroundImagePtr.lock()->getPointer(0); } /* Now assign the raw pointer to data from the DataArray<T> object */


  Eigen::VectorXd Bcalc(m_TotalPoints);
  double average = 0;

  Bcalc = A * p;
  average = Bcalc.mean();
  Bcalc = Bcalc - Eigen::VectorXd::Constant(m_TotalPoints, average);

  for(int i = 0; i < m_TotalPoints; ++i)
  {
    m_BackgroundImage[i] = Bcalc(i);
  }

  if(m_SubtractBackground)
  {
    for(size_t i = 0; i < names.size(); i++)
    {
      m_ImageDataArrayPath.update(getDataContainerName(), getAttributeMatrixName().getAttributeMatrixName(), names[i]);
      iDataArray = getDataContainerArray()->getPrereqIDataArrayFromPath<DataArray<uint8_t>, AbstractFilter>(this, m_ImageDataArrayPath);
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
      m_ImageDataArrayPath.update(getDataContainerName(), getAttributeMatrixName().getAttributeMatrixName(), names[i]);
      iDataArray = getDataContainerArray()->getPrereqIDataArrayFromPath<DataArray<uint8_t>, AbstractFilter>(this, m_ImageDataArrayPath);
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

