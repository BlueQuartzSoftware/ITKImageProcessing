/*
 * Your License or Copyright can go here
 */

#pragma once


#include "SIMPLib/Common/SIMPLibSetGetMacros.h"
#include "SIMPLib/Filtering/AbstractFilter.h"
#include "SIMPLib/SIMPLib.h"

#include "SIMPLib/ITK/itkDream3DFilterInterruption.h"
#include "SIMPLib/ITK/itkDream3DImage.h"
#include "SIMPLib/ITK/itkInPlaceDream3DDataToImageFilter.h"
#include "SIMPLib/ITK/itkInPlaceImageToDream3DDataFilter.h"

#include "itkImageToImageFilter.h"
#include <itkCastImageFilter.h>
#include <itkNumericTraits.h>

#include "ITKImageProcessing/ITKImageProcessingDLLExport.h"

// There is a known overflow for the templates but the code seems to work correctly so disable the warning
#if (_MSC_VER >= 1)
#pragma warning(disable:4756)
#endif

/**
 * @brief The ITKImageBase class. See [Filter documentation](@ref ITKImageBase) for details.
 */
class ITKImageProcessing_EXPORT ITKImageBase : public AbstractFilter
{
  Q_OBJECT
  PYB11_CREATE_BINDINGS(ITKImageBase SUPERCLASS AbstractFilter)

public:
  SIMPL_SHARED_POINTERS(ITKImageBase)
  SIMPL_FILTER_NEW_MACRO(ITKImageBase)
  SIMPL_TYPE_MACRO_SUPER_OVERRIDE(ITKImageBase, AbstractFilter)

  ~ITKImageBase() override;

  /**
   * @brief execute Reimplemented from @see AbstractFilter class
   */
  void execute() override;

  /**
  * @brief preflight Reimplemented from @see AbstractFilter class
  */
  void preflight() override;

  /**
   * @brief CastVec3ToITK Input type should be FloatVec3Type or IntVec3Type, Output
     type should be some kind of ITK "array" (itk::Size, itk::Index,...)
   */
  template <typename InputType, typename OutputType, typename ComponentType> OutputType CastVec3ToITK(const InputType& inputVec3, unsigned int dimension) const
  {
    OutputType output;
    if(dimension > 0)
    {
      output[0] = static_cast<ComponentType>(inputVec3[0]);
      if(dimension > 1)
      {
        output[1] = static_cast<ComponentType>(inputVec3[1]);
        if(dimension > 2)
        {
          output[2] = static_cast<ComponentType>(inputVec3[2]);
        }
      }
    }
    return output;
  }
  /**
   * @brief StaticCast Performs a static cast on a value. 'unused' type is to match 'CastStdToVec3' template format to simplify template declaration in conversion script.
   */
  template <typename InputType, typename OutputType, typename unused> OutputType StaticCastScalar(const InputType& val) const
  {
    return static_cast<OutputType>(val);
  }

  /**
   * @brief CastStdToVec3 Input type should be std::vector<float> or std::vector<int>
     and Output type should be FloatVec3Type or IntVec3Type
   */
  template <typename InputType, typename OutputType, typename ComponentType> OutputType CastStdToVec3(const InputType& inputVector) const
  {
    OutputType outputVec3;
    if(inputVector.size() > 0)
    {
      outputVec3[0] = static_cast<ComponentType>(inputVector[0]);
      if(inputVector.size() > 1)
      {
        outputVec3[1] = static_cast<ComponentType>(inputVector[1]);
        if(inputVector.size() > 2)
        {
          outputVec3[2] = static_cast<ComponentType>(inputVector[2]);
        }
      }
    }
    return outputVec3;
  }

signals:
  /**
   * @brief updateFilterParameters Emitted when the Filter requests all the latest Filter parameters
   * be pushed from a user-facing control (such as a widget)
   * @param filter Filter instance pointer
   */
  void updateFilterParameters(AbstractFilter* filter);

  /**
   * @brief parametersChanged Emitted when any Filter parameter is changed internally
   */
  void parametersChanged();

  /**
   * @brief preflightAboutToExecute Emitted just before calling dataCheck()
   */
  void preflightAboutToExecute();

  /**
   * @brief preflightExecuted Emitted just after calling dataCheck()
   */
  void preflightExecuted();

protected:
  ITKImageBase();

  /**
   * @brief dataCheck Checks for the appropriate parameter values and availability of arrays
   */

  virtual void dataCheckInternal();

  /**
   * @brief imageCheck checks if data array contains an image.
   */
  template <typename PixelType, unsigned int Dimension> void imageCheck(const DataArrayPath& array_path)
  {
    using ValueType = typename itk::NumericTraits<PixelType>::ValueType;
    // Check data array
    typename DataArray<ValueType>::WeakPointer cellArrayPtr;
    ValueType* cellArray;

    std::vector<size_t> dims = ITKDream3DHelper::GetComponentsDimensions<PixelType>();
    cellArrayPtr = getDataContainerArray()->getPrereqArrayFromPath<DataArray<ValueType>, AbstractFilter>(
        this, array_path, dims); /* Assigns the shared_ptr<> to an instance variable that is a weak_ptr<> */
    if(nullptr != cellArrayPtr.lock()) /* Validate the Weak Pointer wraps a non-nullptr pointer to a DataArray<T> object */
    {
      cellArray = cellArrayPtr.lock()->getPointer(0);
    } /* Now assign the raw pointer to data from the DataArray<T> object */
    if(getErrorCode() < 0)
    {
      return;
    }

    ImageGeom::Pointer image = getDataContainerArray()->getDataContainer(array_path.getDataContainerName())->getPrereqGeometry<ImageGeom, AbstractFilter>(this);
    if(nullptr == image)
    {
      setErrorCondition(-1, "Array path does not contain image geometry.");
    }
  }

  /**
  * @brief Applies the filter
  */
  template <typename InputPixelType, typename OutputPixelType, unsigned int Dimension, typename FilterType>
  void filter(FilterType* filter, const std::string& outputArrayName, bool saveAsNewArray, const DataArrayPath& selectedArray)
  {
    try
    {
      DataContainer::Pointer dc = getDataContainerArray()->getDataContainer(selectedArray.getDataContainerName());

      using OutputImageType = itk::Dream3DImage<OutputPixelType, Dimension>;
      using toITKType = itk::InPlaceDream3DDataToImageFilter<InputPixelType, Dimension>;
      // Create a Bridge to wrap an existing DREAM.3D array with an ItkImage container
      typename toITKType::Pointer toITK = toITKType::New();
      toITK->SetInput(dc);
      toITK->SetInPlace(true);
      toITK->SetAttributeMatrixArrayName(selectedArray.getAttributeMatrixName().toStdString());
      toITK->SetDataArrayName(selectedArray.getDataArrayName().toStdString());

      itk::Dream3DFilterInterruption::Pointer interruption = itk::Dream3DFilterInterruption::New();
      interruption->SetFilter(this);

      // Set up filter
      filter->SetInput(toITK->GetOutput());
      filter->AddObserver(itk::ProgressEvent(), interruption);
      filter->Update();

      typename OutputImageType::Pointer image = OutputImageType::New();
      image = filter->GetOutput();
      image->DisconnectPipeline();

      if(!saveAsNewArray)
      {
        AttributeMatrix::Pointer attrMat = dc->getAttributeMatrix(selectedArray.getAttributeMatrixName());
        // Remove the original input data array
        attrMat->removeAttributeArray(selectedArray.getDataArrayName());
      }

      using toDream3DType = itk::InPlaceImageToDream3DDataFilter<OutputPixelType, Dimension>;
      typename toDream3DType::Pointer toDream3DFilter = toDream3DType::New();
      toDream3DFilter->SetInput(image);
      toDream3DFilter->SetInPlace(true);
      toDream3DFilter->SetAttributeMatrixArrayName(selectedArray.getAttributeMatrixName().toStdString());
      toDream3DFilter->SetDataArrayName(outputArrayName);
      toDream3DFilter->SetDataContainer(dc);
      toDream3DFilter->Update();
    } catch(itk::ExceptionObject& err)
    { 
      if(!getCancel())
      {
        QString errorMessage = "ITK exception was thrown while filtering input image: %1";
        setErrorCondition(-55555, errorMessage.arg(err.GetDescription()));
      }
      return;
    }
  }

  /**
  * @brief Applies the filter, casting the input to float
  */

  template <typename InputPixelType, typename OutputPixelType, unsigned int Dimension, typename FilterType, typename FloatImageType>
  void filterCastToFloat(FilterType* filter, const std::string& outputArrayName, bool saveAsNewArray, const DataArrayPath& selectedArray)
  {
    try
    {
      DataContainer::Pointer dc = getDataContainerArray()->getDataContainer(selectedArray.getDataContainerName());

      using toITKType = itk::InPlaceDream3DDataToImageFilter<InputPixelType, Dimension>;
      // Create a Bridge to wrap an existing DREAM.3D array with an ItkImage container
      typename toITKType::Pointer toITK = toITKType::New();
      toITK->SetInput(dc);
      toITK->SetInPlace(true);
      toITK->SetAttributeMatrixArrayName(selectedArray.getAttributeMatrixName().toStdString());
      toITK->SetDataArrayName(selectedArray.getDataArrayName().toStdString());

      itk::Dream3DFilterInterruption::Pointer interruption = itk::Dream3DFilterInterruption::New();
      interruption->SetFilter(this);

      using InputImageType = typename toITKType::ImageType;
      using CasterToType = itk::CastImageFilter<InputImageType, FloatImageType>;
      typename CasterToType::Pointer casterTo = CasterToType::New();
      casterTo->SetInput(toITK->GetOutput());

      // Set up filter
      filter->SetInput(casterTo->GetOutput());
      filter->AddObserver(itk::ProgressEvent(), interruption);

      using OutputImageType = itk::Dream3DImage<OutputPixelType, Dimension>;
      using CasterFromType = itk::CastImageFilter<FloatImageType, OutputImageType>;
      typename CasterFromType::Pointer casterFrom = CasterFromType::New();
      casterFrom->SetInput(filter->GetOutput());
      casterFrom->Update();

      typename OutputImageType::Pointer image = OutputImageType::New();
      image = casterFrom->GetOutput();
      image->DisconnectPipeline();

      if(!saveAsNewArray)
      {
        AttributeMatrix::Pointer attrMat = dc->getAttributeMatrix(selectedArray.getAttributeMatrixName());
        // Remove the original input data array
        attrMat->removeAttributeArray(selectedArray.getDataArrayName());
      }

      using toDream3DType = itk::InPlaceImageToDream3DDataFilter<OutputPixelType, Dimension>;
      typename toDream3DType::Pointer toDream3DFilter = toDream3DType::New();
      toDream3DFilter->SetInput(image);
      toDream3DFilter->SetInPlace(true);
      toDream3DFilter->SetAttributeMatrixArrayName(selectedArray.getAttributeMatrixName().toStdString());
      toDream3DFilter->SetDataArrayName(outputArrayName);
      toDream3DFilter->SetDataContainer(dc);
      toDream3DFilter->Update();
    } catch(itk::ExceptionObject& err)
    {
      QString errorMessage = "ITK exception was thrown while filtering input image: %1";
      setErrorCondition(-55556, errorMessage.arg(err.GetDescription()));
      return;
    }
  }

  /**
  * @brief CheckIntegerEntry: Input types can only be of certain types (float, double, bool, int).
    For the other type, we have to use one of this primitive type, and verify that the
    value corresponds to what is expected.
    The 3rd parameter, 'bool' is given to match the definition of CheckVectorEntry. This allows
    to use a dictionary in Python to choose between the 2 functions.
  */
  template <typename VarType, typename SubsType> void CheckIntegerEntry(SubsType value, const QString& name, bool /* b */)
  {
    using ValueType = typename itk::NumericTraits<VarType>::ValueType;
    SubsType lowest = static_cast<SubsType>(std::numeric_limits<ValueType>::lowest());
    SubsType max = static_cast<SubsType>(std::numeric_limits<ValueType>::max());
    if(value < lowest || value > max || value != floor(value))
    {
      QString errorMessage = name + QString(" must be greater or equal than %1 and lesser or equal than %2 and an integer");
      setErrorCondition(-1, errorMessage.arg(lowest).arg(max));
    }
  }

  /**
  * @brief CheckVectorEntry: Vector input types can only be of certain types (float or int).
    For the other type, we have to use one of this primitive type, and verify that the
    value corresponds to what is expected.
  */
  template <typename VarType, typename SubsType> void CheckVectorEntry(SubsType value, const QString& name, bool integer)
  {
    using ValueType = typename itk::NumericTraits<VarType>::ValueType;
    auto lowest = static_cast<float>(std::numeric_limits<ValueType>::lowest());
    auto max = static_cast<float>(std::numeric_limits<ValueType>::max());
    if(value[0] < lowest || value[0] > max || (integer && value[0] != floor(value[0])) || value[1] < lowest || value[1] > max || (integer && value[1] != floor(value[1])) || value[2] < lowest ||
       value[2] > max || (integer && value[2] != floor(value[2])))
    {
      QString errorMessage = name + QString(" values must be greater or equal than %1 and lesser or equal than %2");
      if(integer)
      {
        errorMessage += QString(" and integers");
      }
      setErrorCondition(-1, errorMessage.arg(lowest).arg(max));
    }
  }

  /**
  * @brief Check if image type corresponds to requirements
  */
  bool checkImageType(const QVector<QString>& types, const DataArrayPath& path)
  {
    IDataArray::Pointer ptr = getDataContainerArray()->getPrereqIDataArrayFromPath<IDataArray, AbstractFilter>(this, path);
    if(nullptr != ptr)
    {
      if(types.indexOf(ptr->getTypeAsString()) != -1)
      {
        return true;
      }
      QString errorMessage = "Wrong data type in %1. Expected %2. Try CastImageFilter or RescaleImageFilter to convert input data to a supported type.";
      QString stringTypes = types.toList().join(",");
      setErrorCondition(-12, errorMessage.arg(path.serialize()).arg(stringTypes));
    }
    // If no data container, return false, but do not set any error condition.
    return false;
  }

  /**
  * @brief Applies the filter
  */
  virtual void filterInternal();

  /**
   * @brief Initializes all the private instance variables.
   */
  void initialize();

public:
  ITKImageBase(const ITKImageBase&) = delete;            // Copy Constructor Implemented
  ITKImageBase(ITKImageBase&&) = delete;                 // Move Constructor Not Implemented
  ITKImageBase& operator=(const ITKImageBase&) = delete; // Copy Assignment Not Implemented
  ITKImageBase& operator=(ITKImageBase&&) = delete;      // Move Assignment Not Implemented
};

