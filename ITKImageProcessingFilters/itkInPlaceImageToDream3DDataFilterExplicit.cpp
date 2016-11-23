#include "ITKImageProcessing/ITKImageProcessingFilters/Dream3DTemplateAliasMacro.h"
#include "ITKImageProcessing/ITKImageProcessingFilters/itkInPlaceImageToDream3DDataFilter.h"
#include <itkRGBAPixel.h>
#include <itkRGBPixel.h>
#include <itkVector.h>
#include <iostream>


// Explicitly implement specialized templated GetComponentsDimensions() function to return a
// a different value depending on the template type:
// - RGBAPixel images return cDims(1, 4)
// - Vector images return cDims(Dimension, 1) with Dimension being the dimension of the vector
// These templated functions are explicit for all the floating and integer types:
// - double
// - float
// - int8_t
// - uint8_t
// - int16_t
// - uint16_t
// - int32_t
// - uint32_t
// - int64_t
// - uint64_t
//
// The generic implementation of this function is defined in InPlaceImageToDream3DDataFilter.h
// and returns cDims(1,1).
//
// These functions need to be defined outside of the header files (.h and .hxx) to avoid multiple definitions
// of the functions. This file needs to be included in the project, both in the plugin compilation and in
// the tests compilation. If it is not included, the plugin will compile but some tests will fail since
// they will call the generic implementation of the function instead of the specialized one that will because
// missing.

namespace itk
{

#define ExplicitSpecializationAll()       \
  ExplicitSpecializationPixelType(RGBA)   \
  ExplicitSpecializationPixelType(Vector)

#define ExplicitSpecializationPixelType(PixelType) \
  ExplicitSpecialization(PixelType, double)        \
  ExplicitSpecialization(PixelType, float)         \
  ExplicitSpecialization(PixelType, int8_t)        \
  ExplicitSpecialization(PixelType, uint8_t)       \
  ExplicitSpecialization(PixelType, int16_t)       \
  ExplicitSpecialization(PixelType, uint16_t)      \
  ExplicitSpecialization(PixelType, int32_t)       \
  ExplicitSpecialization(PixelType, uint32_t)      \
  ExplicitSpecialization(PixelType, int64_t)       \
  ExplicitSpecialization(PixelType, uint64_t)

#define ExplicitSpecialization(PixelType, ComponentType) \
  ExplicitSpecialization0(PixelType, ComponentType, DREAM3D_USE_##ComponentType)
  
#define ExplicitSpecialization0(PixelType, ComponentType, ComponentType_value) \
  ExplicitSpecialization1(PixelType, ComponentType, ComponentType_value)
  
#define ExplicitSpecialization1(PixelType, ComponentType, ComponentType_value) \
  ExplicitSpecialization_##ComponentType_value(PixelType, ComponentType)
  
#define ExplicitSpecialization_0(PixelType, ComponentType) // do nothing, use non-specialized template

#define ExplicitSpecialization_1(PixelType, ComponentType) \
  ExplicitSpecialization##PixelType(ComponentType,1)       \
  ExplicitSpecialization##PixelType(ComponentType,2)       \
  ExplicitSpecialization##PixelType(ComponentType,3)

#define ExplicitSpecializationRGBA(ComponentType,Dimension)           \
template<>                                                            \
QVector<size_t>                                                       \
InPlaceImageToDream3DDataFilter<RGBAPixel<ComponentType>, Dimension>  \
::GetComponentsDimensions()                                           \
{                                                                     \
  QVector<size_t> cDims(1, 4);                                        \
  return cDims;                                                       \
}

#define ExplicitSpecializationVector(ComponentType,Dimension)                \
template<>                                                                   \
QVector<size_t>                                                              \
InPlaceImageToDream3DDataFilter<Vector<ComponentType,Dimension>, Dimension>  \
::GetComponentsDimensions()                                                  \
{                                                                            \
  QVector<size_t> cDims(Dimension, 1);                                       \
  return cDims;                                                              \
}

ExplicitSpecializationAll()

} // end of itk namespace
