/*
 * Your License or Copyright Information can go here
 */

#include "CalculateBackground.h"

#include <string.h>

#include <set>

#include <QtCore/QString>
#include <QtCore/QFileInfo>
#include <QtCore/QDir>
#include <QtCore/QSharedPointer>
#include <QtCore/QString>
#include <QtCore/QVector>
#include <QtCore/QTextStream>

#include "DREAM3DLib/Common/FilterManager.h"
#include "DREAM3DLib/Common/IFilterFactory.hpp"
#include "DREAM3DLib/FilterParameters/AbstractFilterParametersWriter.h"

#include "DREAM3DLib/FilterParameters/DataBundleSelectionFilterParameter.h"
#include "DREAM3DLib/FilterParameters/AttributeMatrixSelectionFilterParameter.h"
#include "DREAM3DLib/FilterParameters/IntFilterParameter.h"
#include "DREAM3DLib/FilterParameters/StringFilterParameter.h"
#include "DREAM3DLib/FilterParameters/BooleanFilterParameter.h"
#include "DREAM3DLib/FilterParameters/AbstractFilterParametersReader.h"
#include "DREAM3DLib/FilterParameters/SeparatorFilterParameter.h"



#include "ZeissImport/ZeissImportConstants.h"
#include "ZeissImport/ZeissXml/ZeissTagMapping.h"
#include <Eigen/Dense>


#define ZIF_PRINT_DBG_MSGS 0



// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
CalculateBackground::CalculateBackground() :
  AbstractFilter(),
  m_VolumeDataContainerName(""),
  m_BackgroundAttributeMatrixName(""),
  m_CellAttributeMatrixName(DREAM3D::Defaults::CellAttributeMatrixName),
  m_ImageDataArrayPath("", "", ""),
  m_AttributeMatrixName(DREAM3D::Defaults::DataContainerName, DREAM3D::Defaults::CellFeatureAttributeMatrixName, ""),
  m_DataContainerBundleName(""),
  m_BackgroundImageArrayName(getDataContainerBundleName() + "BackgroundImage"),
  m_lowThresh(0),
  m_highThresh(255),
  m_SubtractBackground(false),
  m_DivideBackground(false)
{
  setupFilterParameters();
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
CalculateBackground::~CalculateBackground()
{
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void CalculateBackground::setupFilterParameters()
{
  FilterParameterVector parameters;
  //parameters.push_back(DataBundleSelectionFilterParameter::New("DataContainerBundle Name", "DataContainerBundleName", getDataContainerBundleName(), FilterParameter::Uncategorized));
  {
    AttributeMatrixSelectionFilterParameter::DataStructureRequirements req;
    parameters.push_back(AttributeMatrixSelectionFilterParameter::New("Input AttributeMatrix Name", "AttributeMatrixName", getAttributeMatrixName(), FilterParameter::RequiredArray, req));
  }
  parameters.push_back(IntFilterParameter::New("Lowest allowed Image value (Image Value)", "lowThresh", getlowThresh(), FilterParameter::Parameter));
  parameters.push_back(IntFilterParameter::New("Highest allowed Image value (Image Value)", "highThresh", gethighThresh(), FilterParameter::Parameter));
  //    parameters.push_back(SeparatorFilterParameter::New("Created Information", FilterParameter::Uncategorized));
  //    parameters.push_back(StringFilterParameter::New("Volume Data Container", "VolumeDataContainerName", getVolumeDataContainerName(), FilterParameter::Uncategorized));
  parameters.push_back(StringFilterParameter::New("Background Attribute Matrix", "BackgroundAttributeMatrixName", getBackgroundAttributeMatrixName(), FilterParameter::CreatedArray));
  parameters.push_back(StringFilterParameter::New("Background Image Array Name", "BackgroundImageArrayName", getBackgroundImageArrayName(), FilterParameter::CreatedArray));
  parameters.push_back(BooleanFilterParameter::New("Subtract Background from Current Images", "SubtractBackground", getSubtractBackground(), FilterParameter::Parameter));
  parameters.push_back(BooleanFilterParameter::New("Divide Background from Current Images", "DivideBackground", getDivideBackground(), FilterParameter::Parameter));

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
int CalculateBackground::writeFilterParameters(AbstractFilterParametersWriter* writer, int index)
{
  writer->openFilterGroup(this, index);
  DREAM3D_FILTER_WRITE_PARAMETER(FilterVersion)
  DREAM3D_FILTER_WRITE_PARAMETER(AttributeMatrixName)
  //    DREAM3D_FILTER_WRITE_PARAMETER(VolumeDataContainerName)
  DREAM3D_FILTER_WRITE_PARAMETER(BackgroundAttributeMatrixName)
  DREAM3D_FILTER_WRITE_PARAMETER(BackgroundImageArrayName)
  //    DREAM3D_FILTER_WRITE_PARAMETER(DataContainerBundleName)
  DREAM3D_FILTER_WRITE_PARAMETER(lowThresh)
  DREAM3D_FILTER_WRITE_PARAMETER(highThresh)
  DREAM3D_FILTER_WRITE_PARAMETER(SubtractBackground)
  DREAM3D_FILTER_WRITE_PARAMETER(DivideBackground)
  writer->closeFilterGroup();
  return ++index; // we want to return the next index that was just written to
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void CalculateBackground::dataCheck()
{
  setErrorCondition(0);
  DataArrayPath tempPath;

  QString ss;

  AttributeMatrix::Pointer am = getDataContainerArray()->getAttributeMatrix(m_AttributeMatrixName);

  if (am.get() == NULL)
  {
    setErrorCondition(-76000);
    notifyErrorMessage(getHumanLabel(), "The attribute matrix has not been selected properly", -76000);
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

    imagePtr = boost::dynamic_pointer_cast<DataArray<uint8_t> >(iDataArray);
    //        QVector<IDataArray::Pointer> pointerList;

    //        pointerList[i] = iDataArray;


    //        imagePtr = boost::dynamic_pointer_cast<DataArray<uint8_t> >(pointerList[i]);


    if(NULL == imagePtr)
    {
      setErrorCondition(-76001);
      notifyErrorMessage(getHumanLabel(), "The data was not found", -76001);
    }


  }

  if(m_SubtractBackground == true && m_DivideBackground == true)
  {
    setErrorCondition(-76002);
    notifyErrorMessage(getHumanLabel(), "Cannot choose BOTH subtract and divide. Choose one or neither.", -76002);
  }


  if(getErrorCondition() < 0) { return; }
  m_totalPoints = imagePtr->getNumberOfTuples();

  setDataContainerName(getAttributeMatrixName().getDataContainerName());
  DataContainer::Pointer m = getDataContainerArray()->getPrereqDataContainer<AbstractFilter>(this, getDataContainerName(), false);
  if(getErrorCondition() < 0 || NULL == m) { return; }

  QVector<size_t> tDims(1, 0);
  AttributeMatrix::Pointer backgroundAttrMat = m->createNonPrereqAttributeMatrix<AbstractFilter>(this, getBackgroundAttributeMatrixName(), tDims, DREAM3D::AttributeMatrixType::Cell);
  if(getErrorCondition() < 0) { return; }


  // Background Image array
  dims[0] = 1;
  tempPath.update(getDataContainerName(), getBackgroundAttributeMatrixName(), getBackgroundImageArrayName() );
  m_BackgroundImagePtr = getDataContainerArray()->createNonPrereqArrayFromPath<DataArray<double>, AbstractFilter>(this, tempPath, 0, dims); /* Assigns the shared_ptr<> to an instance variable that is a weak_ptr<> */
  if( NULL != m_BackgroundImagePtr.lock().get() ) /* Validate the Weak Pointer wraps a non-NULL pointer to a DataArray<T> object */
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
  uint8_t* image = NULL;

  std::vector<double> background(m_totalPoints, 0);
  std::vector<double> counter(m_totalPoints, 0);


  // getting the fist data container just to get the dimensions of each image.
  DataContainer::Pointer m = getDataContainerArray()->getDataContainer(getDataContainerName());


  QVector<size_t> udims;
  udims = am->getTupleDimensions();

#if (CMP_SIZEOF_SIZE_T == 4)
  typedef int32_t DimType;
#else
  typedef int64_t DimType;
#endif
  DimType dims[3] =
  {
    static_cast<DimType>(udims[0]),
    static_cast<DimType>(udims[1]),
    static_cast<DimType>(udims[2]),
  };

  // run through all the data containers (images) and add them up to be averaged after the loop
  for(size_t i = 0; i < names.size(); i++)
  {
    m_ImageDataArrayPath.update(getDataContainerName(), getAttributeMatrixName().getAttributeMatrixName(), names[i]);
    iDataArray = getDataContainerArray()->getPrereqIDataArrayFromPath<DataArray<uint8_t>, AbstractFilter>(this, m_ImageDataArrayPath);
    imagePtr = boost::dynamic_pointer_cast<DataArray<uint8_t> >(iDataArray);
    if(NULL != imagePtr.get())
    {
      //            int64_t totalPoints = imagePtr->getNumberOfTuples();
      image = imagePtr->getPointer(0);
      for(int64_t t = 0; t < m_totalPoints; t++)
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

  for (int64_t j = 0; j < m_totalPoints; j++)
  {
    background[j] = double(background[j] /= (counter[j]));
  }


  // Fit the background to a second order polynomial
  // p are the coefficients p[0] + p[1]*x + p[2]*y +p[3]*xy + p[4]*x^2 + p[5]*y^2
  Eigen::MatrixXd A(m_totalPoints, ZeissImportConstants::PolynomialOrder::NumConsts2ndOrder);
  Eigen::VectorXd B(m_totalPoints);

  for(int i = 0; i < m_totalPoints; ++i)
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
  if( NULL != m_BackgroundImagePtr.lock().get() ) /* Validate the Weak Pointer wraps a non-NULL pointer to a DataArray<T> object */
  { m_BackgroundImage = m_BackgroundImagePtr.lock()->getPointer(0); } /* Now assign the raw pointer to data from the DataArray<T> object */


  Eigen::VectorXd Bcalc(m_totalPoints);
  double average = 0;

  Bcalc = A * p;
  average = Bcalc.mean();
  Bcalc = Bcalc - Eigen::VectorXd::Constant(m_totalPoints, average);

  for(int i = 0; i < m_totalPoints; ++i)
  {
    m_BackgroundImage[i] = Bcalc(i);
  }

  if(m_SubtractBackground == true)
  {
    for(size_t i = 0; i < names.size(); i++)
    {
      m_ImageDataArrayPath.update(getDataContainerName(), getAttributeMatrixName().getAttributeMatrixName(), names[i]);
      iDataArray = getDataContainerArray()->getPrereqIDataArrayFromPath<DataArray<uint8_t>, AbstractFilter>(this, m_ImageDataArrayPath);
      imagePtr = boost::dynamic_pointer_cast<DataArray<uint8_t> >(iDataArray);
      if(NULL != imagePtr.get())
      {
        image = imagePtr->getPointer(0);

        for(int64_t t = 0; t < m_totalPoints; t++)
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

  if(m_DivideBackground == true)
  {
    for(size_t i = 0; i < names.size(); i++)
    {
      m_ImageDataArrayPath.update(getDataContainerName(), getAttributeMatrixName().getAttributeMatrixName(), names[i]);
      iDataArray = getDataContainerArray()->getPrereqIDataArrayFromPath<DataArray<uint8_t>, AbstractFilter>(this, m_ImageDataArrayPath);
      imagePtr = boost::dynamic_pointer_cast<DataArray<uint8_t> >(iDataArray);
      if(NULL != imagePtr.get())
      {
        image = imagePtr->getPointer(0);

        for(int64_t t = 0; t < m_totalPoints; t++)
        {
          if (static_cast<uint8_t>(image[t]) >= m_lowThresh && static_cast<uint8_t>(image[t])  <= m_highThresh)
          {
            image[t] = image[t] / Bcalc(t);

          }
        }

      }
    }
  }

  /* Let the GUI know we are done with this filter */
  notifyStatusMessage(getHumanLabel(), "Complete");
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
const QString CalculateBackground::getCompiledLibraryName()
{
  return ZeissImportConstants::ZeissImportBaseName;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
const QString CalculateBackground::getGroupName()
{
  return DREAM3D::FilterGroups::Unsupported;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
const QString CalculateBackground::getHumanLabel()
{
  return "Calculate Background";
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
const QString CalculateBackground::getSubGroupName()
{
  return "Misc";
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
AbstractFilter::Pointer CalculateBackground::newFilterInstance(bool copyFilterParameters)
{
  /*
  * write code to optionally copy the filter parameters from the current filter into the new instance
  */
  CalculateBackground::Pointer filter = CalculateBackground::New();
  if(true == copyFilterParameters)
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
    //    DREAM3D_COPY_INSTANCEVAR(OutputFile)
    //    DREAM3D_COPY_INSTANCEVAR(ZStartIndex)
    //    DREAM3D_COPY_INSTANCEVAR(ZEndIndex)
    //    DREAM3D_COPY_INSTANCEVAR(ZResolution)
  }
  return filter;
}

