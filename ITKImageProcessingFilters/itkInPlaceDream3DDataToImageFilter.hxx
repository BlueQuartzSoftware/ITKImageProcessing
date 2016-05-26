#ifndef _itkInPlaceDream3DDataToImageFilter_hxx
#define _itkInPlaceDream3DDataToImageFilter_hxx

#include "itkInPlaceDream3DDataToImageFilter.h"

namespace itk
{

template< typename PixelType, unsigned int VDimension >
InPlaceDream3DDataToImageFilter<PixelType, VDimension>::InPlaceDream3DDataToImageFilter()
{
}

template< typename PixelType, unsigned int VDimension >
InPlaceDream3DDataToImageFilter<PixelType, VDimension>::~InPlaceDream3DDataToImageFilter()
{
}

template< typename PixelType, unsigned int VDimension>
void
InPlaceDream3DDataToImageFilter< PixelType, VDimension >
::GenerateData()
{
  // Get data pointer
  AttributeMatrix::Pointer ma = m_DataContainer->getAttributeMatrix(this->GetMatrixArrayName().c_str());
  IDataArray::Pointer dataArray = ma->getAttributeArray(this->GetDataArrayName().c_str());
  dataArray->releaseOwnership();
  const bool pixelContainerWillOwnTheBuffer = true;
  this->SetImportPointer(static_cast<PixelType*>(dataArray->getVoidPointer(0)), dataArray->getSize(),
    pixelContainerWillOwnTheBuffer);
  Superclass::Superclass::GenerateData();
}

}// end of itk namespace
#endif
