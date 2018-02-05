/*
 * Your License or Copyright can go here
 */

#include "ITKImageProcessingBase.h"

#include "SIMPLib/Geometry/ImageGeom.h"

#include "ITKImageProcessing/ITKImageProcessingConstants.h"
#include "ITKImageProcessing/ITKImageProcessingVersion.h"
// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
ITKImageProcessingBase::ITKImageProcessingBase()
: ITKImageBase()
, m_NewCellArrayName("")
, m_SaveAsNewArray(true)
, m_NewCellArray(nullptr)
{
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
ITKImageProcessingBase::~ITKImageProcessingBase() = default;

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
  return "Image Processing";
}
