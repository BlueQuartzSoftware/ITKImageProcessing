#ifndef _itkInPlaceDream3DDataToImageFilter_hxx
#define _itkInPlaceDream3DDataToImageFilter_hxx

#include "itkInPlaceDream3DDataToImageFilter.h"
#include "SIMPLib/Geometry/ImageGeom.h"

namespace itk
{

template< typename PixelType, unsigned int VDimension >
InPlaceDream3DDataToImageFilter<PixelType, VDimension>::InPlaceDream3DDataToImageFilter()
{
  m_DataContainer = DataContainer::NullPointer();
  m_InPlace = true;
}

template< typename PixelType, unsigned int VDimension >
InPlaceDream3DDataToImageFilter<PixelType, VDimension>::~InPlaceDream3DDataToImageFilter()
{
}

template< typename PixelType, unsigned int VDimension>
void
InPlaceDream3DDataToImageFilter< PixelType, VDimension >
::SetInput(DataContainer::Pointer &dc)
{
  if (!(dc == m_DataContainer))
  {
    m_DataContainer = dc;
    this->Modified();
  }
}

template< typename PixelType, unsigned int VDimension>
void
InPlaceDream3DDataToImageFilter< PixelType, VDimension >
::VerifyPreconditions()
{
  if (m_DataContainer == DataContainer::NullPointer() )
  {
    itkExceptionMacro("DataContainer not set!");
  }
  IGeometry::Pointer geom = m_DataContainer->getGeometry();
  // Verifies that data container geometry is ImageGeom
  QString geomTypeName = geom->getGeometryTypeAsString();
  if (geomTypeName != "ImageGeometry")
  {
    itkExceptionMacro("DataContainer geometry is not ImageGeometry. It is " + geomTypeName.toStdString());
  }
  // Verifies that data container contains matrix array
  if (!m_DataContainer->doesAttributeMatrixExist(m_MatrixArrayName.c_str()))
  {
    itkExceptionMacro("Matrix array (" + m_MatrixArrayName + ") does not exist");
  }
  AttributeMatrix::Pointer ma = m_DataContainer->getAttributeMatrix(m_MatrixArrayName.c_str());
  // Verifies that matrix array contains data array
  if (!ma->doesAttributeArrayExist(m_DataArrayName.c_str()))
  {
    itkExceptionMacro("Attribute array (" + m_DataArrayName + ") does not exist");
  }
  Superclass::VerifyPreconditions();
}

template< typename PixelType, unsigned int VDimension>
void
InPlaceDream3DDataToImageFilter< PixelType, VDimension >
::GenerateOutputInformation()
{
  IGeometry::Pointer geom = m_DataContainer->getGeometry();
  ImageGeom* imageGeom = dynamic_cast<ImageGeom*>(geom.get());
  if (!imageGeom)
  {
    // This should never happen as VerifyPreconditions() verifies that the geometry of
    // the data container is ImageGeom.
    itkExceptionMacro("Error casting geometry!!!");
  }
  // Get data container properties
  float torigin[3];
  float tspacing[3];
  size_t tDims[3];
  imageGeom->getOrigin(torigin[0], torigin[1], torigin[2]);
  imageGeom->getResolution(tspacing[0], tspacing[1], tspacing[2]);
  imageGeom->getDimensions(tDims[0], tDims[1], tDims[2]);
  typename ImageType::PointType origin;
  typename ImageType::SizeType size;
  typename ImageType::SpacingType spacing;
  typename ImageType::DirectionType direction;
  direction.SetIdentity();
  for (size_t i = 0; i < VDimension; i++)
  {
    spacing[i] = tspacing[i];
    origin[i] = torigin[i];
    size[i] = tDims[i];
  }
  // Set ImportImageFilter properties
  this->SetOrigin(origin);
  this->SetSpacing(spacing);
  this->SetRegion(size);
  this->SetDirection(direction);
  Superclass::GenerateOutputInformation();
}

template< typename PixelType, unsigned int VDimension>
void
InPlaceDream3DDataToImageFilter< PixelType, VDimension >
::GenerateData()
{
  // Get data pointer
  AttributeMatrix::Pointer ma = m_DataContainer->getAttributeMatrix(m_MatrixArrayName.c_str());
  IDataArray::Pointer dataArray = ma->getAttributeArray(m_DataArrayName.c_str());
  const bool pixelContainerWillOwnTheBuffer = true;
  size_t size = dataArray->getSize();
  PixelType *buffer;
  if(m_InPlace)
  {
    dataArray->releaseOwnership();
    buffer = static_cast<PixelType*>(dataArray->getVoidPointer( 0 ));
  }
  else
  {
    buffer = new PixelType[size];
    ::memcpy( buffer, static_cast<PixelType*>(dataArray->getVoidPointer( 0 )), size * sizeof( PixelType ) );
  }
  this->SetImportPointer( buffer, size, pixelContainerWillOwnTheBuffer );
  Superclass::GenerateData();
}

}// end of itk namespace
#endif
