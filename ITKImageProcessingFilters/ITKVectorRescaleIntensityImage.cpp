/* ============================================================================
 * Copyright (c) 2019 BlueQuartz Software, LLC
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
 *
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
#include "ITKVectorRescaleIntensityImage.h"

#if(ITK_VERSION_MAJOR == 5) && (ITK_VERSION_MINOR >= 1)
#include <itkCommonEnums.h>
#else
#include <itkImageIOBase.h>
#endif

#include "SIMPLib/Common/Constants.h"
#include "SIMPLib/FilterParameters/AbstractFilterParametersReader.h"
#include "SIMPLib/FilterParameters/ChoiceFilterParameter.h"
#include "SIMPLib/FilterParameters/DataArraySelectionFilterParameter.h"
#include "SIMPLib/FilterParameters/DoubleFilterParameter.h"
#include "SIMPLib/FilterParameters/LinkedBooleanFilterParameter.h"
#include "SIMPLib/FilterParameters/SeparatorFilterParameter.h"
#include "SIMPLib/FilterParameters/StringFilterParameter.h"
#include "SIMPLib/Geometry/ImageGeom.h"

#define DREAM3D_USE_Scalar 0
#define DREAM3D_USE_Vector 1
#include "SIMPLib/ITK/Dream3DTemplateAliasMacro.h"

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
ITKVectorRescaleIntensityImage::ITKVectorRescaleIntensityImage() = default;

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
ITKVectorRescaleIntensityImage::~ITKVectorRescaleIntensityImage() = default;

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void ITKVectorRescaleIntensityImage::setupFilterParameters()
{
  FilterParameterVectorType parameters;

  {
    ChoiceFilterParameter::Pointer parameter = ChoiceFilterParameter::New();
    parameter->setHumanLabel("Output Component Type");
    parameter->setPropertyName("OutputType");
    parameter->setSetterCallback(SIMPL_BIND_SETTER(ITKVectorRescaleIntensityImage, this, OutputType));
    parameter->setGetterCallback(SIMPL_BIND_GETTER(ITKVectorRescaleIntensityImage, this, OutputType));

    QVector<QString> choices;
    choices.push_back("unsigned char");
    choices.push_back("char");
    choices.push_back("unsigned short");
    choices.push_back("short");
    choices.push_back("unsigned int");
    choices.push_back("int");
    choices.push_back("unsigned long");
    choices.push_back("long");
    choices.push_back("long long");
    choices.push_back("unsigned long long");
    choices.push_back("float");
    choices.push_back("double");
    parameter->setChoices(choices);
    parameter->setCategory(FilterParameter::Category::Parameter);
    parameters.push_back(parameter);
  }

  parameters.push_back(SIMPL_NEW_DOUBLE_FP("OutputMaximumMagnitude", OutputMaximumMagnitude, FilterParameter::Category::Parameter, ITKVectorRescaleIntensityImage));

  QStringList linkedProps;
  linkedProps << "NewCellArrayName";
  parameters.push_back(SeparatorFilterParameter::Create("Cell Data", FilterParameter::Category::RequiredArray));
  {
    DataArraySelectionFilterParameter::RequirementType req =
        DataArraySelectionFilterParameter::CreateRequirement(SIMPL::Defaults::AnyPrimitive, SIMPL::Defaults::AnyComponentSize, AttributeMatrix::Type::Cell, IGeometry::Type::Image);
    parameters.push_back(SIMPL_NEW_DA_SELECTION_FP("Attribute Array to filter", SelectedCellArrayPath, FilterParameter::Category::RequiredArray, ITKVectorRescaleIntensityImage, req));
  }
  parameters.push_back(SeparatorFilterParameter::Create("Cell Data", FilterParameter::Category::CreatedArray));
  parameters.push_back(SIMPL_NEW_STRING_FP("Filtered Array", NewCellArrayName, FilterParameter::Category::CreatedArray, ITKVectorRescaleIntensityImage));

  setFilterParameters(parameters);
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void ITKVectorRescaleIntensityImage::readFilterParameters(AbstractFilterParametersReader* reader, int index)
{
  reader->openFilterGroup(this, index);
  setSelectedCellArrayPath(reader->readDataArrayPath("SelectedCellArrayPath", getSelectedCellArrayPath()));
  setNewCellArrayName(reader->readString("NewCellArrayName", getNewCellArrayName()));
  setOutputMaximumMagnitude(reader->readValue("OutputMaximumMagnitude", getOutputMaximumMagnitude()));

  reader->closeFilterGroup();
}

template <typename OutputPixelType>
void ITKVectorRescaleIntensityImage::CheckEntryBounds(double value, QString name)
{
  double lowest = static_cast<double>(std::numeric_limits<OutputPixelType>::lowest());
  double max = static_cast<double>(std::numeric_limits<OutputPixelType>::max());
  if(value < lowest || value > max)
  {
    QString errorMessage = name + QString(" must be greater or equal than %1 and lesser or equal than %2 and an integer");
    setErrorCondition(-1, errorMessage.arg(lowest).arg(max));
  }
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
template <typename InputPixelType, typename OutputPixelType, unsigned int Dimension>
void ITKVectorRescaleIntensityImage::dataCheckImpl()
{
  // Check consistency of parameters
  CheckEntryBounds<OutputPixelType>(m_OutputMaximumMagnitude, "OutputMaximumMagnitude");
  if(getErrorCode() < 0)
  {
    return;
  }
  // OutputPixelType is based on scalar types. Create corresponding vector pixel type.
  typedef itk::Vector<OutputPixelType, InputPixelType::Dimension> VectorOutputPixelType;
  ITKImageProcessingBase::dataCheckImpl<InputPixelType, VectorOutputPixelType, Dimension>();
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void ITKVectorRescaleIntensityImage::dataCheck()
{
  clearErrorCode();
  clearWarningCode();

#if(ITK_VERSION_MAJOR == 5) && (ITK_VERSION_MINOR >= 1)
  itk::CommonEnums::IOComponent castingType = static_cast<itk::CommonEnums::IOComponent>(m_OutputType + 1);
#else
  itk::ImageIOBase::IOComponentType castingType = static_cast<itk::ImageIOBase::IOComponentType>(m_OutputType + 1);
#endif
  Dream3DArraySwitchOutputComponentMacro(this->dataCheckImpl, castingType, getSelectedCellArrayPath(), -4);
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------

template <typename InputPixelType, typename OutputPixelType, unsigned int Dimension>
void ITKVectorRescaleIntensityImage::filter()
{
  typedef itk::Image<InputPixelType, Dimension> InputImageType;
  // OutputPixelType is based on scalar types. Create corresponding vector pixel type.
  typedef itk::Vector<OutputPixelType, InputPixelType::Dimension> VectorOutputPixelType;
  typedef itk::Image<VectorOutputPixelType, Dimension> OutputImageType;
  // define filter
  typedef itk::VectorRescaleIntensityImageFilter<InputImageType, OutputImageType> FilterType;
  typename FilterType::Pointer filter = FilterType::New();
  filter->SetOutputMaximumMagnitude(static_cast<double>(m_OutputMaximumMagnitude));
  this->ITKImageProcessingBase::filter<InputPixelType, VectorOutputPixelType, Dimension, FilterType>(filter);
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void ITKVectorRescaleIntensityImage::filterInternal()
{
#if(ITK_VERSION_MAJOR == 5) && (ITK_VERSION_MINOR >= 1)
  itk::CommonEnums::IOComponent castingType = static_cast<itk::CommonEnums::IOComponent>(m_OutputType + 1);
#else
  itk::ImageIOBase::IOComponentType castingType = static_cast<itk::ImageIOBase::IOComponentType>(m_OutputType + 1);
#endif
  Dream3DArraySwitchOutputComponentMacro(this->filter, castingType, getSelectedCellArrayPath(), -4);
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
AbstractFilter::Pointer ITKVectorRescaleIntensityImage::newFilterInstance(bool copyFilterParameters) const
{
  ITKVectorRescaleIntensityImage::Pointer filter = ITKVectorRescaleIntensityImage::New();
  if(copyFilterParameters)
  {
    copyFilterParameterInstanceVariables(filter.get());
  }
  return filter;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
QString ITKVectorRescaleIntensityImage::getHumanLabel() const
{
  return "ITK::Vector Rescale Intensity Image Filter";
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
QUuid ITKVectorRescaleIntensityImage::getUuid() const
{
  return QUuid("{bc1051ba-6c67-5391-809b-48627ed47fa7}");
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
QString ITKVectorRescaleIntensityImage::getSubGroupName() const
{
  return "ITK IntensityTransformation";
}

// -----------------------------------------------------------------------------
ITKVectorRescaleIntensityImage::Pointer ITKVectorRescaleIntensityImage::NullPointer()
{
  return Pointer(static_cast<Self*>(nullptr));
}

// -----------------------------------------------------------------------------
std::shared_ptr<ITKVectorRescaleIntensityImage> ITKVectorRescaleIntensityImage::New()
{
  struct make_shared_enabler : public ITKVectorRescaleIntensityImage
  {
  };
  std::shared_ptr<make_shared_enabler> val = std::make_shared<make_shared_enabler>();
  val->setupFilterParameters();
  return val;
}

// -----------------------------------------------------------------------------
QString ITKVectorRescaleIntensityImage::getNameOfClass() const
{
  return QString("ITKVectorRescaleIntensityImage");
}

// -----------------------------------------------------------------------------
QString ITKVectorRescaleIntensityImage::ClassName()
{
  return QString("ITKVectorRescaleIntensityImage");
}

// -----------------------------------------------------------------------------
void ITKVectorRescaleIntensityImage::setOutputMaximumMagnitude(double value)
{
  m_OutputMaximumMagnitude = value;
}

// -----------------------------------------------------------------------------
double ITKVectorRescaleIntensityImage::getOutputMaximumMagnitude() const
{
  return m_OutputMaximumMagnitude;
}

// -----------------------------------------------------------------------------
void ITKVectorRescaleIntensityImage::setOutputType(int value)
{
  m_OutputType = value;
}

// -----------------------------------------------------------------------------
int ITKVectorRescaleIntensityImage::getOutputType() const
{
  return m_OutputType;
}
