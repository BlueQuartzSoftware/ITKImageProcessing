/*
 * Your License or Copyright can go here
 */

#ifndef _ITKImageProcessingBase_h_
#define _ITKImageProcessingBase_h_

#include "ITKImageBase.h"

// The sitkExplicitITK.h header must be AFTER any ITK includes above or
// the code will not compile on Windows. Further, windows does not seem
// to have the symbol loading issues that macOS has so lets just #define
// around it for macOS systems only.
#if defined(__APPLE__)
//  #include "sitkExplicitITK.h"
#endif

/**
 * @brief The ITKImageProcessingBase class. See [Filter documentation](@ref ITKImageProcessingBase) for details.
 */
class ITKImageProcessingBase : public ITKImageBase
{
  Q_OBJECT

public:
  // SIMPL_SHARED_POINTERS(ITKImageProcessingBase)
  // SIMPL_TYPE_MACRO_SUPER_OVERRIDE(ITKImageProcessingBase, AbstractFilter)

  virtual ~ITKImageProcessingBase();

  SIMPL_FILTER_PARAMETER(DataArrayPath, SelectedCellArrayPath)
  Q_PROPERTY(DataArrayPath SelectedCellArrayPath READ getSelectedCellArrayPath WRITE setSelectedCellArrayPath)

  SIMPL_FILTER_PARAMETER(QString, NewCellArrayName)
  Q_PROPERTY(QString NewCellArrayName READ getNewCellArrayName WRITE setNewCellArrayName)

  SIMPL_FILTER_PARAMETER(bool, SaveAsNewArray)
  Q_PROPERTY(bool SaveAsNewArray READ getSaveAsNewArray WRITE setSaveAsNewArray)
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
   * @brief getGroupName Reimplemented from @see AbstractFilter class
   */
  const QString getGroupName() const override;
  /**
   * @brief newFilterInstance Reimplemented from @see AbstractFilter class
   */
  virtual AbstractFilter::Pointer newFilterInstance(bool copyFilterParameters) const override = 0;
  /**
   * @brief getHumanLabel Reimplemented from @see AbstractFilter class
   */
  virtual const QString getHumanLabel() const override = 0;

  /**
   * @brief setupFilterParameters Reimplemented from @see AbstractFilter class
   */
  virtual void setupFilterParameters() override = 0;

  /**
   * @brief readFilterParameters Reimplemented from @see AbstractFilter class
   */
  virtual void readFilterParameters(AbstractFilterParametersReader* reader, int index) override = 0;

protected:
  ITKImageProcessingBase();

  /**
   * @brief dataCheck Checks for the appropriate parameter values and availability of arrays
   */
  template <typename InputPixelType, typename OutputPixelType, unsigned int Dimension> void dataCheck()
  {
    // typedef typename itk::NumericTraits<InputPixelType>::ValueType InputValueType;
    typedef typename itk::NumericTraits<OutputPixelType>::ValueType OutputValueType;
    // Check data array
    imageCheck<InputPixelType, Dimension>(getSelectedCellArrayPath());

    if(getErrorCondition() < 0)
    {
      return;
    }
    QVector<size_t> outputDims = ITKDream3DHelper::GetComponentsDimensions<OutputPixelType>();
    if(m_SaveAsNewArray == true)
    {
      DataArrayPath tempPath;
      tempPath.update(getSelectedCellArrayPath().getDataContainerName(), getSelectedCellArrayPath().getAttributeMatrixName(), getNewCellArrayName());
      m_NewCellArrayPtr = getDataContainerArray()->createNonPrereqArrayFromPath<DataArray<OutputValueType>, AbstractFilter, OutputValueType>(
          this, tempPath, 0, outputDims);           /* Assigns the shared_ptr<> to an instance variable that is a weak_ptr<> */
      if(nullptr != m_NewCellArrayPtr.lock())       /* Validate the Weak Pointer wraps a non-nullptr pointer to a DataArray<T> object */
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
  template <typename InputPixelType, typename OutputPixelType, unsigned int Dimension, typename FilterType> void filter(FilterType* filter)
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
  template <typename InputPixelType, typename OutputPixelType, unsigned int Dimension, typename FilterType, typename FloatImageType> void filterCastToFloat(FilterType* filter)
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
  DEFINE_IDATAARRAY_VARIABLE(NewCellArray)

  ITKImageProcessingBase(const ITKImageProcessingBase&);   // Copy Constructor Not Implemented
  void operator=(const ITKImageProcessingBase&) = delete;  // Move assignment Not Implemented
};

#endif /* _ITKImageProcessingBase_H_ */
