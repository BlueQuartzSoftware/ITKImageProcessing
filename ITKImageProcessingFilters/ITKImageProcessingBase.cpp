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
QString ITKImageProcessingBase::getCompiledLibraryName() const
{
  return ITKImageProcessingConstants::ITKImageProcessingBaseName;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
QString ITKImageProcessingBase::getBrandingString() const
{
  return "ITKImageProcessing";
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
QString ITKImageProcessingBase::getFilterVersion() const
{
  QString version;
  QTextStream vStream(&version);
  vStream << ITKImageProcessing::Version::Major() << "." << ITKImageProcessing::Version::Minor() << "." << ITKImageProcessing::Version::Patch();
  return version;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
QString ITKImageProcessingBase::getGroupName() const
{
  return "ITK Image Processing";
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
QString ITKImageProcessingBase::getHumanLabel() const
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

// -----------------------------------------------------------------------------
ITKImageProcessingBase::Pointer ITKImageProcessingBase::NullPointer()
{
  return Pointer(static_cast<Self*>(nullptr));
}

// -----------------------------------------------------------------------------
QString ITKImageProcessingBase::getNameOfClass() const
{
  return QString("ITKImageProcessingBase");
}

// -----------------------------------------------------------------------------
QString ITKImageProcessingBase::ClassName()
{
  return QString("ITKImageProcessingBase");
}

// -----------------------------------------------------------------------------
void ITKImageProcessingBase::setSelectedCellArrayPath(const DataArrayPath& value)
{
  m_SelectedCellArrayPath = value;
}

// -----------------------------------------------------------------------------
DataArrayPath ITKImageProcessingBase::getSelectedCellArrayPath() const
{
  return m_SelectedCellArrayPath;
}

// -----------------------------------------------------------------------------
void ITKImageProcessingBase::setNewCellArrayName(const QString& value)
{
  m_NewCellArrayName = value;
}

// -----------------------------------------------------------------------------
QString ITKImageProcessingBase::getNewCellArrayName() const
{
  return m_NewCellArrayName;
}

// -----------------------------------------------------------------------------
void ITKImageProcessingBase::setSaveAsNewArray(bool value)
{
  m_SaveAsNewArray = value;
}

// -----------------------------------------------------------------------------
bool ITKImageProcessingBase::getSaveAsNewArray() const
{
  return m_SaveAsNewArray;
}


