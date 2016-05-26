#ifndef _ITKInPlaceImageToDream3DDataFilter_hxx
#define _ITKInPlaceImageToDream3DDataFilter_hxx

#include "itkInPlaceImageToDream3DDataFilter.h"

namespace itk
{
  
template<typename PixelType, unsigned int VDimension>
InPlaceImageToDream3DDataFilter<PixelType,VDimension>
::InPlaceImageToDream3DDataFilter()
{
}


template<typename PixelType, unsigned int VDimension>
InPlaceImageToDream3DDataFilter<PixelType, VDimension>
::~InPlaceImageToDream3DDataFilter()
{
}



template<typename PixelType, unsigned int VDimension>
void
InPlaceImageToDream3DDataFilter<PixelType, VDimension>
::GenerateData()
{
  DecoratorType *outputPtr = this->GetOutput();
  DataContainer::Pointer dataContainer = outputPtr->Get();
  ImagePointer inputPtr = dynamic_cast<ImageType*>(this->GetInput(0));
  // Create data array
  QVector<size_t> cDims(1, 0);
  cDims[0] = 1;
  IGeometry::Pointer geom = dataContainer->getGeometry();
  ImageGeom::Pointer imageGeom = std::dynamic_pointer_cast<ImageGeom>(geom);
  size_t dims[3];
  imageGeom->getDimensions(dims[0], dims[1], dims[2]);
  QVector<size_t> tDims(3, 1);
  qCopy(dims, dims + 3, tDims.begin());
  AttributeMatrix::Pointer ma;
  if (dataContainer->doesAttributeMatrixExist(this->GetMatrixArrayName().c_str()))
  {
    // Check that size matches
    ma = dataContainer->getAttributeMatrix(this->GetMatrixArrayName().c_str());
    size_t numofTuples = ma->getNumTuples();
    QVector<size_t> matDims = ma->getTupleDimensions();
    if (matDims != tDims)
    {
      itkExceptionMacro("Tuples dimension of existing matrix array do not match image size.");
    }
    if (ma->getType() != SIMPL::AttributeMatrixType::Cell)
    {
      itkExceptionMacro("Attribute matrix is not of type SIMPL::AttributeMatrixType::Cell.");
    }
  }
  else
  {
    ma = dataContainer->createAndAddAttributeMatrix(tDims, this->GetMatrixArrayName().c_str(), 
                                                    SIMPL::AttributeMatrixType::Cell);
  }
  // Checks if doesAttributeArray exists
  if (ma->doesAttributeArrayExist(this->GetDataArrayName().c_str()))
  {
    itkExceptionMacro("Existing data array with same name.");
  }
  else
  {
    inputPtr->SetBufferedRegion(inputPtr->GetLargestPossibleRegion());
    inputPtr->GetPixelContainer()->SetContainerManageMemory(false);
    typename DataArrayPixelType::Pointer data = DataArrayPixelType::WrapPointer(inputPtr->GetBufferPointer(),
                               imageGeom->getNumberOfElements(), cDims, this->GetDataArrayName().c_str(), true);
    ma->addAttributeArray(this->GetDataArrayName().c_str(), data);
  }
  outputPtr->Set(dataContainer);
}


} // end of itk namespace

#endif
