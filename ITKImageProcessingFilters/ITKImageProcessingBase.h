/*
 * Your License or Copyright can go here
 */

#pragma once

#include <memory>

#include "SIMPLib/SIMPLib.h"

class IDataArray;
using IDataArrayWkPtrType = std::weak_ptr<IDataArray>;

#include "ITKImageBase.h"

#include "ITKImageProcessing/ITKImageProcessingDLLExport.h"

/**
 * @brief The ITKImageProcessingBase class. See [Filter documentation](@ref ITKImageProcessingBase) for details.
 */
class ITKImageProcessing_EXPORT ITKImageProcessingBase : public ITKImageBase
{
  Q_OBJECT
  // Start Python bindings declarations
  PYB11_BEGIN_BINDINGS(ITKImageProcessingBase SUPERCLASS AbstractFilter)
  PYB11_SHARED_POINTERS(ITKImageProcessingBase)
  PYB11_PROPERTY(DataArrayPath SelectedCellArrayPath READ getSelectedCellArrayPath WRITE setSelectedCellArrayPath)
  PYB11_PROPERTY(QString NewCellArrayName READ getNewCellArrayName WRITE setNewCellArrayName)
  PYB11_PROPERTY(bool SaveAsNewArray READ getSaveAsNewArray WRITE setSaveAsNewArray)
  PYB11_END_BINDINGS()
  // End Python bindings declarations

public:
  using Self = ITKImageProcessingBase;
  using Pointer = std::shared_ptr<Self>;
  using ConstPointer = std::shared_ptr<const Self>;
  using WeakPointer = std::weak_ptr<Self>;
  using ConstWeakPointer = std::weak_ptr<const Self>;
  static Pointer NullPointer();

  /**
   * @brief Returns the name of the class for ITKImageProcessingBase
   */
  QString getNameOfClass() const override;
  /**
   * @brief Returns the name of the class for ITKImageProcessingBase
   */
  static QString ClassName();

  ~ITKImageProcessingBase() override;

  /**
   * @brief Setter property for SelectedCellArrayPath
   */
  void setSelectedCellArrayPath(const DataArrayPath& value);
  /**
   * @brief Getter property for SelectedCellArrayPath
   * @return Value of SelectedCellArrayPath
   */
  DataArrayPath getSelectedCellArrayPath() const;
  Q_PROPERTY(DataArrayPath SelectedCellArrayPath READ getSelectedCellArrayPath WRITE setSelectedCellArrayPath)

  /**
   * @brief Setter property for NewCellArrayName
   */
  void setNewCellArrayName(const QString& value);
  /**
   * @brief Getter property for NewCellArrayName
   * @return Value of NewCellArrayName
   */
  QString getNewCellArrayName() const;
  Q_PROPERTY(QString NewCellArrayName READ getNewCellArrayName WRITE setNewCellArrayName)

  /**
   * @brief Setter property for SaveAsNewArray
   */
  void setSaveAsNewArray(bool value);
  /**
   * @brief Getter property for SaveAsNewArray
   * @return Value of SaveAsNewArray
   */
  bool getSaveAsNewArray() const;
  Q_PROPERTY(bool SaveAsNewArray READ getSaveAsNewArray WRITE setSaveAsNewArray)

  /**
   * @brief getCompiledLibraryName Reimplemented from @see AbstractFilter class
   */
  QString getCompiledLibraryName() const override;

  /**
   * @brief getBrandingString Returns the branding string for the filter, which is a tag
   * used to denote the filter's association with specific plugins
   * @return Branding string
   */
  QString getBrandingString() const override;

  /**
   * @brief getFilterVersion Returns a version string for this filter. Default
   * value is an empty string.
   * @return
   */
  QString getFilterVersion() const override;

  /**
   * @brief getGroupName Reimplemented from @see AbstractFilter class
   */
  QString getGroupName() const override;

  /**
   * @brief getHumanLabel Reimplemented from @see AbstractFilter class
   */
  QString getHumanLabel() const override;

  /**
   * @brief setupFilterParameters Reimplemented from @see AbstractFilter class
   */
  void setupFilterParameters() override;

  /**
   * @brief readFilterParameters Reimplemented from @see AbstractFilter class
   */
  void readFilterParameters(AbstractFilterParametersReader* reader, int index) override;

protected:
  ITKImageProcessingBase();

  /**
   * @brief dataCheck Checks for the appropriate parameter values and availability of arrays
   */
  template <typename InputPixelType, typename OutputPixelType, unsigned int Dimension>
  void dataCheckImpl()
  {
    // typedef typename itk::NumericTraits<InputPixelType>::ValueType InputValueType;
    typedef typename itk::NumericTraits<OutputPixelType>::ValueType OutputValueType;
    // Check data array
    imageCheck<InputPixelType, Dimension>(getSelectedCellArrayPath());

    if(getErrorCode() < 0)
    {
      return;
    }
    std::vector<size_t> outputDims = ITKDream3DHelper::GetComponentsDimensions<OutputPixelType>();
    if(m_SaveAsNewArray)
    {
      DataArrayPath tempPath;
      tempPath.update(getSelectedCellArrayPath().getDataContainerName(), getSelectedCellArrayPath().getAttributeMatrixName(), getNewCellArrayName());
      m_NewCellArrayPtr =
          getDataContainerArray()->createNonPrereqArrayFromPath<DataArray<OutputValueType>>(this, tempPath, 0, outputDims); /* Assigns the shared_ptr<> to an instance variable that is a weak_ptr<> */
      if(nullptr != m_NewCellArrayPtr.lock()) /* Validate the Weak Pointer wraps a non-nullptr pointer to a DataArray<T> object */
      {
        m_NewCellArray = m_NewCellArrayPtr.lock()->getVoidPointer(0);
      } /* Now assign the raw pointer to data from the DataArray<T> object */
    }
    else
    {
      m_NewCellArrayPtr = DataArray<OutputValueType>::NullPointer();
      m_NewCellArray = nullptr;
    }
  }

  /**
   * @brief Applies the filter
   */
  template <typename InputPixelType, typename OutputPixelType, unsigned int Dimension, typename FilterType>
  void filter(FilterType* filter)
  {
    std::string outputArrayName = getSelectedCellArrayPath().getDataArrayName().toStdString();

    if(getSaveAsNewArray())
    {
      outputArrayName = getNewCellArrayName().toStdString();
    }
    ITKImageBase::filter<InputPixelType, OutputPixelType, Dimension, FilterType>(filter, outputArrayName, getSaveAsNewArray(), getSelectedCellArrayPath());
  }

  /**
   * @brief Applies the filter, casting the input to float
   */
  template <typename InputPixelType, typename OutputPixelType, unsigned int Dimension, typename FilterType, typename FloatImageType>
  void filterCastToFloat(FilterType* filter)
  {
    std::string outputArrayName = getSelectedCellArrayPath().getDataArrayName().toStdString();

    if(getSaveAsNewArray())
    {
      outputArrayName = getNewCellArrayName().toStdString();
    }
    ITKImageBase::filterCastToFloat<InputPixelType, OutputPixelType, Dimension, FilterType, FloatImageType>(filter, outputArrayName, getSaveAsNewArray(), getSelectedCellArrayPath());
  }

  /**
   * @brief Initializes all the private instance variables.
   */
  void initialize();

private:
  IDataArrayWkPtrType m_NewCellArrayPtr;
  void* m_NewCellArray = nullptr;

  DataArrayPath m_SelectedCellArrayPath = {};
  QString m_NewCellArrayName = {};
  bool m_SaveAsNewArray = {};

public:
  ITKImageProcessingBase(const ITKImageProcessingBase&) = delete;            // Copy Constructor Not Implemented
  ITKImageProcessingBase(ITKImageProcessingBase&&) = delete;                 // Move Constructor Not Implemented
  ITKImageProcessingBase& operator=(const ITKImageProcessingBase&) = delete; // Copy Assignment Not Implemented
  ITKImageProcessingBase& operator=(ITKImageProcessingBase&&) = delete;      // Move Assignment Not Implemented
};
