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

#include "ZeissImport/ZeissImportConstants.h"
#include "ZeissImport/ZeissXml/ZeissTagMapping.h"
#include <Eigen/Dense>


#define ZIF_PRINT_DBG_MSGS 0



// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
CalculateBackground::CalculateBackground() :
    AbstractFilter(),
    m_VolumeDataContainerName("ZeissBundleBackground"),
    m_BackgroundAttributeMatrixName("Background"),
    m_CellAttributeMatrixName(DREAM3D::Defaults::CellAttributeMatrixName),
    m_PolynomialCoefficientsArrayName("PolynomialCoefficients"),
    m_DataContainerBundleName(""),
    m_lowThresh(0),
    m_highThresh(255)

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
    parameters.push_back(FilterParameter::New("DataContainerBundle Name", "DataContainerBundleName", FilterParameterWidgetType::DataBundleSelectionWidget, getDataContainerBundleName(), true));
    parameters.push_back(FilterParameter::New("lowest allowed Image value", "lowThresh", FilterParameterWidgetType::IntWidget, getlowThresh(), false, "Image Value"));
    parameters.push_back(FilterParameter::New("highest allowed Image value", "highThresh", FilterParameterWidgetType::IntWidget, gethighThresh(), false, "Image Value"));
    parameters.push_back(FilterParameter::New("Created Information", "", FilterParameterWidgetType::SeparatorWidget, "", true));
    parameters.push_back(FilterParameter::New("Volume Data Container", "VolumeDataContainerName", FilterParameterWidgetType::StringWidget, getVolumeDataContainerName(), true, ""));
    parameters.push_back(FilterParameter::New("Background Attribute Matrix", "BackgroundAttributeMatrixName", FilterParameterWidgetType::StringWidget, getBackgroundAttributeMatrixName(), true, ""));
    parameters.push_back(FilterParameter::New("Polynomial Coefficients", "PolynomialCoefficientsArrayName", FilterParameterWidgetType::StringWidget, getPolynomialCoefficientsArrayName(), true, ""));

    setFilterParameters(parameters);
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void CalculateBackground::readFilterParameters(AbstractFilterParametersReader* reader, int index)
{
    reader->openFilterGroup(this, index);
    setVolumeDataContainerName(reader->readString("VolumeDataContainerName", getVolumeDataContainerName() ) );
    setBackgroundAttributeMatrixName(reader->readString("BackgroundAttributeMatrixName", getBackgroundAttributeMatrixName()));
    setPolynomialCoefficientsArrayName(reader->readString("PolynomialCoefficientsArrayName", getPolynomialCoefficientsArrayName()));
    setDataContainerBundleName(reader->readString("DataContainerBundleName", getDataContainerBundleName() ) );
    setlowThresh(reader->readValue("lowThresh", getlowThresh()) );
    sethighThresh(reader->readValue("highThresh", gethighThresh()) );
    reader->closeFilterGroup();
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
int CalculateBackground::writeFilterParameters(AbstractFilterParametersWriter* writer, int index)
{
    writer->openFilterGroup(this, index);
    DREAM3D_FILTER_WRITE_PARAMETER(VolumeDataContainerName)
    DREAM3D_FILTER_WRITE_PARAMETER(BackgroundAttributeMatrixName)
    DREAM3D_FILTER_WRITE_PARAMETER(PolynomialCoefficientsArrayName)
    DREAM3D_FILTER_WRITE_PARAMETER(DataContainerBundleName)
    DREAM3D_FILTER_WRITE_PARAMETER(lowThresh)
    DREAM3D_FILTER_WRITE_PARAMETER(highThresh)
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


    QVector<size_t> dims(1, 3);
    IDataContainerBundle::Pointer dcb = getDataContainerArray()->getDataContainerBundle(m_DataContainerBundleName);
    if(NULL == dcb.get())
    {
        setErrorCondition(-76000);
        notifyErrorMessage(getHumanLabel(), "The data bundle has not been selected properly", -76000);
        return;
    }
    QVector<QString> dcList = dcb->getDataContainerNames();

    UInt8ArrayType::Pointer imagePtr = UInt8ArrayType::NullPointer();
    IDataArray::Pointer iDataArray = IDataArray::NullPointer();

    for(int i = 0; i < dcList.size(); i++)
    {
        m_ImageDataArrayPath.update(dcList[i], "CellData", "ImageData");
        iDataArray = getDataContainerArray()->getPrereqArrayFromPath<DataArray<uint8_t>, AbstractFilter>(this, getImageDataArrayPath(), dims);
        imagePtr = boost::dynamic_pointer_cast<DataArray<uint8_t> >(iDataArray);


        if(NULL == imagePtr)
        {
            setErrorCondition(-76001);
            notifyErrorMessage(getHumanLabel(), "The data was not found", -76001);
        }
    }

    m_totalPoints = imagePtr->getNumberOfTuples();


    VolumeDataContainer* m = getDataContainerArray()->createNonPrereqDataContainer<VolumeDataContainer, AbstractFilter>(this, getVolumeDataContainerName());
    if(getErrorCondition() < 0){ return; }
    QVector<size_t> tDims(1, 0);
    AttributeMatrix::Pointer backgroundAttrMat = m->createNonPrereqAttributeMatrix<AbstractFilter>(this, getBackgroundAttributeMatrixName(), tDims, DREAM3D::AttributeMatrixType::Cell);
    if(getErrorCondition() < 0){ return; }

    dims[0] = 1;
    tempPath.update(getVolumeDataContainerName(), getBackgroundAttributeMatrixName(), getPolynomialCoefficientsArrayName() );
    m_PolynomialCoefficientsPtr = getDataContainerArray()->createNonPrereqArrayFromPath<DataArray<double>, AbstractFilter>(this, tempPath, 0, dims); /* Assigns the shared_ptr<> to an instance variable that is a weak_ptr<> */
    if( NULL != m_PolynomialCoefficientsPtr.lock().get() ) /* Validate the Weak Pointer wraps a non-NULL pointer to a DataArray<T> object */
    { m_PolynomialCoefficients = m_PolynomialCoefficientsPtr.lock()->getPointer(0); } /* Now assign the raw pointer to data from the DataArray<T> object */


    if(getErrorCondition() < 0){ return; }


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



    UInt8ArrayType::Pointer imagePtr = UInt8ArrayType::NullPointer();
    IDataArray::Pointer iDataArray = IDataArray::NullPointer();
    uint8_t* image;

    std::vector<double> background(m_totalPoints, 0);
    std::vector<double> counter(m_totalPoints,0);

    IDataContainerBundle::Pointer dcb = getDataContainerArray()->getDataContainerBundle(m_DataContainerBundleName);
    QVector<QString> dcList = dcb->getDataContainerNames();
    // getting the fist data container just to get the dimensions of each image.
    VolumeDataContainer* m2 = getDataContainerArray()->getDataContainerAs<VolumeDataContainer>(dcList[0]);

    size_t udims[3] = {0, 0, 0};
    m2->getDimensions(udims);

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
    for(size_t i = 0; i < dcList.size(); i++)
    {
        m_ImageDataArrayPath.update(dcList[i], "CellData", "ImageData");
        iDataArray = getDataContainerArray()->getExistingPrereqArrayFromPath<DataArray<uint8_t>, AbstractFilter>(this, m_ImageDataArrayPath);


        imagePtr = boost::dynamic_pointer_cast<DataArray<uint8_t> >(iDataArray);

        if(NULL != imagePtr.get())
        {
//            int64_t totalPoints = imagePtr->getNumberOfTuples();

            for(size_t t = 0; t < m_totalPoints; t++)
            {
                image = imagePtr->getPointer(t);


                if (static_cast<uint64_t>(image[t]) >= m_lowThresh && static_cast<uint64_t>(image[t])  <= m_highThresh)
                {
                   background[t] = background[t] + static_cast<double>(image[t]);
                   counter[t]++;
                }
            }

        }



    }

    // average the background values by the number of counts (counts will be the number of images unless the threshold values do not include all the possible image values
    // (i.e. for an 8 bit image, if we only include values from 0 to 100, not every image value will be counted)

    for (size_t j=0; j<m_totalPoints; j++)
    {
        background[j] = double(background[j] /= (counter[j]));
    }


    // Fit the background to a second order polynomial
    // p are the coefficients p[0] + p[1]*x + p[2]*y +p[3]*xy + p[4]*x^2 + p[5]*y^2
    Eigen::MatrixXd A(m_totalPoints, 6);
    Eigen::VectorXd B(m_totalPoints);

    for(int i=0; i < m_totalPoints; ++i)
    {
       xval = int(i/dims[0]);
       yval = int(i % dims[0]);
       B(i) = background[i];
       A(i, 0) = 1;
       A(i, 1) = xval;
       A(i, 2) = yval;
       A(i, 3) = xval*yval;
       A(i, 4) = xval*xval;
       A(i, 5) = yval*yval;
    }

   notifyStatusMessage(getHumanLabel(), "Fitting a polynomial to data. May take a while to solve if images are large");
   Eigen::VectorXd p = A.colPivHouseholderQr().solve(B);

   QVector<size_t> tDims(1, 6);
   VolumeDataContainer* m = getDataContainerArray()->getDataContainerAs<VolumeDataContainer>(getVolumeDataContainerName());
   m->getAttributeMatrix(getBackgroundAttributeMatrixName())->resizeAttributeArrays(tDims);
   if( NULL != m_PolynomialCoefficientsPtr.lock().get() ) /* Validate the Weak Pointer wraps a non-NULL pointer to a DataArray<T> object */
   { m_PolynomialCoefficients = m_PolynomialCoefficientsPtr.lock()->getPointer(0); } /* Now assign the raw pointer to data from the DataArray<T> object */

   for(size_t i=0; i < 6; i++)
   {
       m_PolynomialCoefficients[i] = p[i];
   }


   /* Let the GUI know we are done with this filter */
    notifyStatusMessage(getHumanLabel(), "Complete");
}






// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
const QString CalculateBackground::getCompiledLibraryName()
{
    return ZeissImport::ZeissImportBaseName;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
const QString CalculateBackground::getGroupName()
{
    return "ZeissImport";
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
    return "IO";
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

