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
#pragma once


#include <QtCore/QString>
#include <QtXml/QDomDocument>

#include "SIMPLib/Common/SIMPLibSetGetMacros.h"
#include "SIMPLib/Filtering/AbstractFilter.h"
#include "SIMPLib/Filtering/FilterManager.h"
#include "SIMPLib/Filtering/IFilterFactory.hpp"
#include "SIMPLib/Geometry/IGeometryGrid.h"
#include "SIMPLib/SIMPLib.h"

#include "ZeissImport/ZeissXml/ZeissTagsXmlSection.h"

#include "ZeissImport/ZeissImportDLLExport.h"

/**
 * @class CalculateBackground CalculateBackground.h ZeissImport/CalculateBackgrounds/CalculateBackground.h
 * @brief
 * @author
 * @date
 * @version 1.0
 */
class ZeissImport_EXPORT CalculateBackground : public AbstractFilter
{
  Q_OBJECT
  PYB11_CREATE_BINDINGS(CalculateBackground SUPERCLASS AbstractFilter)
  PYB11_PROPERTY(QStringList DataContainers READ getDataContainers WRITE setDataContainers)
  PYB11_PROPERTY(QString CellAttributeMatrixName READ getCellAttributeMatrixName WRITE setCellAttributeMatrixName)
  PYB11_PROPERTY(DataArrayPath ImageDataArrayName READ getImageDataArrayName WRITE setImageDataArrayName)

  PYB11_PROPERTY(DataArrayPath OutputDataContainerPath READ getOutputDataContainerPath WRITE setOutputDataContainerPath)
  PYB11_PROPERTY(DataArrayPath OutputCellAttributeMatrixPath READ getOutputCellAttributeMatrixPath WRITE setOutputCellAttributeMatrixPath)
  PYB11_PROPERTY(DataArrayPath OutputImageArrayPath READ getOutputImageArrayPath WRITE setOutputImageArrayPath)

  PYB11_PROPERTY(uint lowThresh READ getlowThresh WRITE setlowThresh)
  PYB11_PROPERTY(uint highThresh READ gethighThresh WRITE sethighThresh)
  PYB11_PROPERTY(bool SubtractBackground READ getSubtractBackground WRITE setSubtractBackground)
  PYB11_PROPERTY(bool DivideBackground READ getDivideBackground WRITE setDivideBackground)

public:
  SIMPL_SHARED_POINTERS(CalculateBackground)
  SIMPL_FILTER_NEW_MACRO(CalculateBackground)
  SIMPL_TYPE_MACRO_SUPER_OVERRIDE(CalculateBackground, AbstractFilter)

  ~CalculateBackground() override;

  SIMPL_INSTANCE_STRING_PROPERTY(DataContainerName)

  SIMPL_FILTER_PARAMETER(QStringList, DataContainers)
  Q_PROPERTY(QStringList DataContainers READ getDataContainers WRITE setDataContainers)

  SIMPL_FILTER_PARAMETER(QString, CellAttributeMatrixName)
  Q_PROPERTY(QString CellAttributeMatrixName READ getCellAttributeMatrixName WRITE setCellAttributeMatrixName)

  SIMPL_FILTER_PARAMETER(QString, ImageDataArrayName)
  Q_PROPERTY(QString ImageDataArrayName READ getImageDataArrayName WRITE setImageDataArrayName)

  SIMPL_FILTER_PARAMETER(DataArrayPath, OutputDataContainerPath)
  Q_PROPERTY(DataArrayPath OutputDataContainerPath READ getOutputDataContainerPath WRITE setOutputDataContainerPath)

  SIMPL_FILTER_PARAMETER(DataArrayPath, OutputCellAttributeMatrixPath)
  Q_PROPERTY(DataArrayPath OutputCellAttributeMatrixPath READ getOutputCellAttributeMatrixPath WRITE setOutputCellAttributeMatrixPath)

  SIMPL_FILTER_PARAMETER(DataArrayPath, OutputImageArrayPath)
  Q_PROPERTY(DataArrayPath OutputImageArrayPath READ getOutputImageArrayPath WRITE setOutputImageArrayPath)

  SIMPL_FILTER_PARAMETER(uint, lowThresh)
  Q_PROPERTY(uint lowThresh READ getlowThresh WRITE setlowThresh)

  SIMPL_FILTER_PARAMETER(uint, highThresh)
  Q_PROPERTY(uint highThresh READ gethighThresh WRITE sethighThresh)

  SIMPL_FILTER_PARAMETER(bool, SubtractBackground)
  Q_PROPERTY(int SubtractBackground READ getSubtractBackground WRITE setSubtractBackground)

  SIMPL_FILTER_PARAMETER(bool, DivideBackground)
  Q_PROPERTY(int DivideBackground READ getDivideBackground WRITE setDivideBackground)

#if 0
  SIMPL_FILTER_PARAMETER(bool, Polynomial)
  Q_PROPERTY(int Polynomial READ getPolynomial WRITE setPolynomial)
#endif

  SIMPL_FILTER_PARAMETER(bool, GaussianBlur)
  Q_PROPERTY(int GaussianBlur READ getGaussianBlur WRITE setGaussianBlur)

  SIMPL_FILTER_PARAMETER(float, GaussianStdVariation)
  Q_PROPERTY(float GaussianStdVariation READ getGaussianStdVariation WRITE setGaussianStdVariation)

  /**
   * @brief getCompiledLibraryName Reimplemented from @see AbstractFilter class
   */
  const QString getCompiledLibraryName() const override;

  /**
   * @brief getBrandingString Returns the branding string for the filter, which is a tag
   * used to denote the filter's association with specific plugins
   * @return Branding string
   */
  const QString getBrandingString() const override;

  /**
   * @brief getFilterVersion Returns a version string for this filter. Default
   * value is an empty string.
   * @return
   */
  const QString getFilterVersion() const override;

  /**
   * @brief newFilterInstance Reimplemented from @see AbstractFilter class
   */
  AbstractFilter::Pointer newFilterInstance(bool copyFilterParameters) const override;

  /**
   * @brief getGroupName Reimplemented from @see AbstractFilter class
   */
  const QString getGroupName() const override;

  /**
   * @brief getSubGroupName Reimplemented from @see AbstractFilter class
   */
  const QString getSubGroupName() const override;

  /**
   * @brief getUuid Return the unique identifier for this filter.
   * @return A QUuid object.
   */
  const QUuid getUuid() override;

  /**
   * @brief getHumanLabel Reimplemented from @see AbstractFilter class
   */
  const QString getHumanLabel() const override;

  /**
   * @brief setupFilterParameters Reimplemented from @see AbstractFilter class
   */
  void setupFilterParameters() override;

  /**
   * @brief execute Reimplemented from @see AbstractFilter class
   */
  void execute() override;

  /**
   * @brief preflight Reimplemented from @see AbstractFilter class
   */
  void preflight() override;

signals:
  /**
   * @brief updateFilterParameters This is emitted when the filter requests all the latest Filter Parameters need to be
   * pushed from a user facing control such as the FilterParameter Widget
   * @param filter The filter to push the values into
   */
  void updateFilterParameters(AbstractFilter* filter);

  /**
   * @brief parametersChanged This signal can be emitted when any of the filter parameters are changed internally.
   */
  void parametersChanged();

  /**
   * @brief preflightAboutToExecute Emitted just before the dataCheck() is called. This can change if needed.
   */
  void preflightAboutToExecute();

  /**
   * @brief preflightExecuted Emitted just after the dataCheck() is called. Typically. This can change if needed.
   */
  void preflightExecuted();

protected:
  enum class ArrayType
  {
    Error = -1,
    UInt8,
    UInt16,
    Float32
  };
  enum class GeomType
  {
    Error = -1,
    ImageGeom,
    RectGridGeom
  };

  CalculateBackground();

  /**
   * @brief dataCheck Checks for the appropriate parameter values and availability of arrays
   */
  void dataCheck();

  /**
   * @brief Initializes all the private instance variables.
   */
  void initialize();

  /**
   * @brief Returns an enum value specifying which supported array type the incoming data falls into.
   * @return
   */
  ArrayType getArrayType();
  
  /**
   * @brief Returns an enum value specifying which supported geometry type the incoming data falls into.
   * @return
   */
  GeomType getGeomType();

  /**
   * @brief Calls the corresponding checkInputArrays based on the array and geometry type.
   * @param arrayType
   * @param geomType
   * @return
   */
  IGeometryGrid::Pointer checkInputArrays(ArrayType arrayType, GeomType geomType);

  /**
   * @brief Calls the corresponding calculateOutputValues based on the array and geometry type.
   * @param arrayType
   * @param geomType
   * @return
   */
  void calculateOutputValues(ArrayType arrayType, GeomType geomType);

  /**
   * @brief Checks the input arrays' type and geometry type and returns a deep copy of the geometry.
   * @return
   */
  template<typename DataArrayType, typename GeometryType>
  std::shared_ptr<GeometryType> checkInputArrays()
  {
    DataContainerArray::Pointer dca = getDataContainerArray();
    QVector<size_t> cDims = { 1 };
    typename GeometryType::Pointer outputGridGeom = GeometryType::NullPointer();

    // Ensure each DataContainer has the proper path to the image data and the image data is grayscale
    for(const auto& dcName : m_DataContainers)
    {
      DataArrayPath imageArrayPath(dcName, m_CellAttributeMatrixName, m_ImageDataArrayName);
      typename DataArrayType::Pointer imageData = dca->getPrereqArrayFromPath<DataArrayType, AbstractFilter>(this, imageArrayPath, cDims);
      if(imageData.get() == nullptr)
      {
        QString msg;
        QTextStream out(&msg);
        out << "Attribute Array Path: " << imageArrayPath.serialize() << " is not of the appropriate type and components{1} (Grayscale) data. Please select a pattern of AttributeArray Paths that are gray scale images";
        setErrorCondition(-53000, msg);
      }

      if(getErrorCode() >= 0)
      {
        typename GeometryType::Pointer gridGeom = dca->getDataContainer(dcName)->getGeometryAs<GeometryType>();
        if(gridGeom.get() != nullptr)
        {
          if(outputGridGeom.get() == nullptr)
          {
            outputGridGeom = std::dynamic_pointer_cast<GeometryType>(gridGeom->deepCopy());
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
    return outputGridGeom;
  }

  /**
   * @brief Calculates the output values using the templated output IDataArray output type
   */
  template<typename OutArrayType, typename GeomType, typename AccumType>
  void calculateOutputValues()
  {
    DataContainerArray::Pointer dca = getDataContainerArray();

    DataContainer::Pointer outputDc = dca->getDataContainer(getOutputDataContainerPath());
    AttributeMatrix::Pointer outputAttrMat = outputDc->getAttributeMatrix(getOutputCellAttributeMatrixPath());
    typename DataArray<OutArrayType>::Pointer outputArrayPtr = outputAttrMat->getAttributeArrayAs<DataArray<OutArrayType>>(m_OutputImageArrayPath.getDataArrayName());
    typename DataArray<OutArrayType>& outputArray = *(outputArrayPtr);

    typename GeomType::Pointer outputGeom = outputDc->getGeometryAs<GeomType>();
    SizeVec3Type dims;
    outputGeom->getDimensions(dims);

    typename DataArray<AccumType>::Pointer accumulateArrayPtr = typename DataArray<AccumType>::CreateArray(outputArrayPtr->getNumberOfTuples(), "Accumulation Array", true);
    accumulateArrayPtr->initializeWithZeros();
    typename DataArray<AccumType>& accumArray = *accumulateArrayPtr;
    size_t numTuples = accumArray.getNumberOfTuples();

    SizeTArrayType::Pointer countArrayPtr = SizeTArrayType::CreateArray(outputArrayPtr->getNumberOfTuples(), "Count Array", true);
    SizeTArrayType& counter = *(countArrayPtr);

    for(const auto& dcName : m_DataContainers)
    {
      DataArrayPath imageArrayPath(dcName, m_CellAttributeMatrixName, m_ImageDataArrayName);
      typename DataArray<OutArrayType>& imageArray = *(dca->getAttributeMatrix(imageArrayPath)->getAttributeArrayAs<DataArray<OutArrayType>>(imageArrayPath.getDataArrayName()));

      for(size_t t = 0; t < numTuples; t++)
      {
        if(imageArray[t] >= m_lowThresh && imageArray[t] <= m_highThresh)
        {
          accumArray[t] += imageArray[t];
          counter[t]++;
        }
      }
    }

    // average the background values by the number of counts (counts will be the number of images unless the threshold
    // values do not include all the possible image values
    // (i.e. for an 8 bit image, if we only include values from 0 to 100, not every image value will be counted)
    for(int j = 0; j < numTuples; j++)
    {
      accumArray[j] /= counter[j];
    }

#if 0
    // This block was previously disabled and divided on both sides of the for loop copying values into the output array.
    // The first part performs required work for polynomial operations.
    // The first part is required for SubtractBackground and DivideBackground operations.
    if(getPolynomial())
    {
      int xval = 0;
      int yval = 0;
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
    } // Polynomial
#endif

    // Assign output array values
    for(int i = 0; i < numTuples; ++i)
    {
      outputArray[i] = static_cast<OutArrayType>(accumArray[i]);
    }

    // Blur
    if(getGaussianBlur())
    {
      FilterManager* filtManager = FilterManager::Instance();
      IFilterFactory::Pointer factory = filtManager->getFactoryFromClassName("ItkDiscreteGaussianBlur");
      if(nullptr != factory.get())
      {
        AbstractFilter::Pointer filter = factory->create();
        if(nullptr != filter.get())
        {
          QVariant var;
          var.setValue(getOutputImageArrayPath());
          filter->setDataContainerArray(getDataContainerArray());
          filter->setProperty("SelectedCellArrayPath", var);
          filter->setProperty("SaveAsNewArray", false);
          filter->setProperty("Stdev", getGaussianStdVariation());
          filter->execute();
        }
      }
      else
      {
        setErrorCondition(-53009, "ItkDiscreteGaussianBlur filter not found.");
      }
    } // Blur

    // Edit the input data
    if(m_SubtractBackground)
    {
      for(const auto& dcName : m_DataContainers)
      {
        DataArrayPath imageDataPath(dcName, m_CellAttributeMatrixName, m_ImageDataArrayName);
        auto iDataArray = getDataContainerArray()->getPrereqIDataArrayFromPath<DataArray<OutArrayType>, AbstractFilter>(this, imageDataPath);
        auto imagePtr = std::dynamic_pointer_cast<DataArray<OutArrayType>>(iDataArray);
        size_t totalPoints = imagePtr->getNumberOfComponents();
        if(nullptr != imagePtr.get())
        {
          typename auto* image = imagePtr->getPointer(0);

          for(size_t t = 0; t < totalPoints; t++)
          {
            if((image[t] >= m_lowThresh) && (image[t] <= m_highThresh))
            {
              image[t] -= outputArray[t];

              if(image[t] < 0) { image[t] = 0; }
              if(image[t] > 255) { image[t] = 255; }
            }
          }
        }
      }
    } // Subtract Background

    if(m_DivideBackground)
    {
      for(const auto& dcName : m_DataContainers)
      {
        DataArrayPath imageDataPath(dcName, m_CellAttributeMatrixName, m_ImageDataArrayName);
        auto iDataArray = getDataContainerArray()->getPrereqIDataArrayFromPath<DataArray<OutArrayType>, AbstractFilter>(this, imageDataPath);
        auto imagePtr = std::dynamic_pointer_cast<DataArray<OutArrayType>>(iDataArray);
        size_t totalPoints = imagePtr->getNumberOfComponents();
        if(nullptr != imagePtr.get())
        {
          typename auto* image = imagePtr->getPointer(0);

          for(size_t t = 0; t < totalPoints; t++)
          {
            if((image[t] >= m_lowThresh) && (image[t] <= m_highThresh))
            {
              image[t] /= outputArray[t];
            }
          }
        }
      }
    } // Divide Background

  }

private:
  //  int64_t m_TotalPoints;

  //  DEFINE_DATAARRAY_VARIABLE(double, BackgroundImage)

public:
  CalculateBackground(const CalculateBackground&) = delete;            // Copy Constructor Not Implemented
  CalculateBackground(CalculateBackground&&) = delete;                 // Move Constructor Not Implemented
  CalculateBackground& operator=(const CalculateBackground&) = delete; // Copy Assignment Not Implemented
  CalculateBackground& operator=(CalculateBackground&&) = delete;      // Move Assignment Not Implemented
};

