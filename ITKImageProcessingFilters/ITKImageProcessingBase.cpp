#include "ITKImageProcessingBase.h"

#include "SIMPLib/FilterParameters/AbstractFilterParametersReader.h"

#include "ITKImageProcessing/ITKImageProcessingConstants.h"
#include "ITKImageProcessing/ITKImageProcessingVersion.h"

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
ITKImageProcessingBase::ITKImageProcessingBase() = default;

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
ITKImageProcessingBase::~ITKImageProcessingBase() = default;

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void ITKImageProcessingBase::setupFilterParameters()
{
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
const QString ITKImageProcessingBase::getCompiledLibraryName() const
{
  return ITKImageProcessingConstants::ITKImageProcessingBaseName;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
const QString ITKImageProcessingBase::getBrandingString() const
{
  return "ITKImageProcessing";
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
const QString ITKImageProcessingBase::getFilterVersion() const
{
  QString version;
  QTextStream vStream(&version);
  vStream << ITKImageProcessing::Version::Major() << "." << ITKImageProcessing::Version::Minor() << "." << ITKImageProcessing::Version::Patch();
  return version;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
const QString ITKImageProcessingBase::getGroupName() const
{
  return "ITK Image Processing";
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
AbstractFilter::Pointer ITKImageProcessingBase::newFilterInstance(bool copyFilterParameters) const
{
  ITKImageProcessingBase::Pointer filter = ITKImageProcessingBase::New();
  if(copyFilterParameters)
  {
    copyFilterParameterInstanceVariables(filter.get());
  }
  return filter;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
const QString ITKImageProcessingBase::getHumanLabel() const
{
  return "ITK::Image Processing Base";
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void ITKImageProcessingBase::readFilterParameters(AbstractFilterParametersReader* reader, int index)
{
  reader->openFilterGroup(this, index);
  reader->closeFilterGroup();
}
