// Based on the same paradigm as VTKTemplateAliasMacro.h, in
// VTK (www.vtk.org), we create macros that allow to easily
// choose which template version of a function to run.

// Dream3DTemplateAliasMacro is used in a switch statement to
// automatically generate duplicate code for all enabled scalar types.
// The code can be written to create ITK filters in Dream3D.
//
// Example usage:
//
//  Dream3DArraySwitchMacro(dataCheck, getSelectedCellArrayPath(),-4);
//  
//  with:
//   dataCheck: templated function to be called (one template parameter that specifies the data type).
//   getSelectedCellArrayPath(): DataArrayPath that specifies the data array to extract the data type from.
//   -4: errorCondition printed if type is not handled

#ifndef _Dream3DTemplateAliasMacro_h
#define _Dream3DTemplateAliasMacro_h

#include <QString>
#include "SIMPLib/Common/AbstractFilter.h"
#include <itkVector.h>
#include <itkRGBAPixel.h>
#include <itkImageIOBase.h>

// Allow individual switching of support for each scalar size/signedness.
// These could be made advanced user options to be configured by CMake.
#define DREAM3D_USE_int8_t     1
#define DREAM3D_USE_uint8_t    1
#define DREAM3D_USE_int16_t    1
#define DREAM3D_USE_uint16_t   1
#define DREAM3D_USE_int32_t    1
#define DREAM3D_USE_uint32_t   1
#define DREAM3D_USE_int64_t    1
#define DREAM3D_USE_uint64_t   1
#define DREAM3D_USE_float      1
#define DREAM3D_USE_double     1
#define DREAM3D_USE_Vector     0
#define DREAM3D_USE_RGBA       0
//--------------------------------------------------------------------------

// Define helper macros to switch types on and off.
#define Q(x) #x
#define QUOTE(x) Q(x)

// Expand the value of typeIN to be able to concatenate it with 'DREAM3D_USE' to check if the type is accepted by the filter
#define Dream3DTemplateAliasMacroCase(typeIN, typeOUT, call, var_type, tDims, errorCondition, isTypeOUT, typeOUTTypename)                    \
  Dream3DTemplateAliasMacroCase0(typeIN, typeOUT, call, var_type, tDims, errorCondition, DREAM3D_USE_##typeIN, isTypeOUT, typeOUTTypename)
#define Dream3DTemplateAliasMacroCase0(typeIN, typeOUT, call, var_type, tDims, errorCondition, value, isTypeOUT, typeOUTTypename)            \
  Dream3DTemplateAliasMacroCase1(typeIN, typeOUT, call, var_type, tDims, errorCondition, value, isTypeOUT, typeOUTTypename)
#define Dream3DTemplateAliasMacroCase1(typeIN, typeOUT, call, var_type, tDims, errorCondition, value, isTypeOUT, typeOUTTypename)            \
  Dream3DTemplateAliasMacroCase_##value(typeIN, typeOUT, call, var_type, tDims, errorCondition, QUOTE(typeIN), isTypeOUT, typeOUTTypename)
// Type is not accepted, throw an error message.
#define Dream3DTemplateAliasMacroCase_0(typeIN, typeOUT, call, var_type, tDims, errorCondition, quotedType, isTypeOUT, typeOUTTypename)      \
  if( var_type.compare(quotedType) == 0 )                                                                                                    \
  {                                                                                                                                          \
    setErrorCondition(errorCondition);                                                                                                       \
    QString errorMessage = QString("Unsupported pixel type: %1.").arg(quotedType);                                                           \
    notifyErrorMessage(getHumanLabel(), errorMessage, getErrorCondition());                                                                  \
  }
// Type is accepted, select the dimension of the input and output images
#define Dream3DTemplateAliasMacroCase_1(typeIN, typeOUT, call, var_type, tDims, errorCondition, quotedType, isTypeOUT, typeOUTTypename)      \
  if( var_type.compare(quotedType) == 0 )                                                                                                    \
  {                                                                                                                                          \
    if (tDims[2] == 1)                                                                                                                       \
    {                                                                                                                                        \
      /* 2D image */                                                                                                                         \
      Dream3DTemplateAliasMacroPixelType(typeIN, typeOUT, call, errorCondition, isTypeOUT, typeOUTTypename, 2)                               \
    }                                                                                                                                        \
    else                                                                                                                                     \
    {                                                                                                                                        \
      /* 3D */                                                                                                                               \
      Dream3DTemplateAliasMacroPixelType(typeIN, typeOUT, call, errorCondition, isTypeOUT, typeOUTTypename, 3)                               \
    }                                                                                                                                        \
  }
// Handles vector, RGBA, and scalar images
#define Dream3DTemplateAliasMacroPixelType(typeIN, typeOUT, call, errorCondition, isTypeOUT, typeOUTTypename, dimension)                          \
    QVector<size_t> cDims = ptr->getComponentDimensions();                                                                                        \
    if(cDims.size() > 1)                                                                                                                          \
    {                                                                                                                                             \
      Dream3DTemplateAliasMacroCaseVectorImage0(typeIN, typeOUT, call, errorCondition, isTypeOUT, typeOUTTypename, dimension, DREAM3D_USE_Vector);\
    }                                                                                                                                             \
    else                                                                                                                                          \
    {                                                                                                                                             \
      if(cDims[0] == 1)                                                                                                                           \
      {                                                                                                                                           \
        Dream3DTemplateAliasMacroCase_1_##isTypeOUT(typeIN, typeOUT, call, typeOUTTypename, dimension);                                           \
      }                                                                                                                                           \
      else if(cDims[0] == 4)                                                                                                                      \
      {                                                                                                                                           \
        Dream3DTemplateAliasMacroCaseRGBAImage0(typeIN, typeOUT, call, errorCondition, isTypeOUT, typeOUTTypename, dimension, DREAM3D_USE_RGBA);  \
      }                                                                                                                                           \
      else                                                                                                                                        \
      {                                                                                                                                           \
        setErrorCondition(errorCondition);                                                                                                        \
        notifyErrorMessage(getHumanLabel(), QString("Size of tuple not handled:%1").arg(cDims[0]), getErrorCondition());                          \
      }                                                                                                                                           \
    }
// Replaces typeOUT by typeIN if no typeOUT is given
#define Dream3DTemplateAliasMacroCase_1_0(typeIN, typeOUT, call, typeOUTTypename, dimension)                                                 \
  Dream3DTemplateAliasMacroCaseIf(typeIN, typeIN, call, typeOUTTypename, dimension)
// Otherwise, just forward the given arguments to the next macro
#define Dream3DTemplateAliasMacroCase_1_1(typeIN, typeOUT, call, typeOUTTypename, dimension)                                                 \
  DefineInputImageType##typeOUTTypename(typeIN, dimension)                                                                                   \
  Dream3DTemplateAliasMacroCaseIf(typeIN, typeOUT, call, typeOUTTypename, dimension)
// Call the given function, templated with typeIN, typeOUT, and dimension.
#define Dream3DTemplateAliasMacroCaseIf(typeIN, typeOUT, call, typeOUTTypename, dimension)                                                   \
  call<typeIN, typeOUT, dimension>();                                                                                                        \

// 3 possible pixel type: C-type, InputImageType, and TImageType
#define DefineInputImageType0(typeIN, dimension) // Do nothing, no type to defined
#define DefineInputImageType1(typeIN, dimension) \
  typedef itk::Dream3DImage<typeIN, dimension> InputImageType;
#define DefineInputImageType2(typeIN, dimension) \
  typedef itk::Dream3DImage<typeIN, dimension> TImageType;

///////////////////////////////////////////////////////////////////////////////////////////////
//                                    Handles vector images                                  //
///////////////////////////////////////////////////////////////////////////////////////////////
// Define the input and output pixel types, depending on 'isTypeOUT' value. This is required for
// vectors because it is not possible to pass a macro argument that contains a comma.
#define DefineVectorPixelTypes_0(typeIN,typeOUT,vDim)  \
  typedef itk::Vector<typeIN,vDim> InputPixelType;     \
  typedef itk::Vector<typeIN,vDim> OutputPixelType;
#define DefineVectorPixelTypes_1(typeIN,typeOUT,vDim)  \
  typedef itk::Vector<typeIN,vDim> InputPixelType;     \
  typedef itk::Vector<typeOUT,vDim> OutputPixelType;
// Expands the value of 'Vector' that is 0 if the filter does not accept 'Vector' images, and '1' if it does.
#define Dream3DTemplateAliasMacroCaseVectorImage0(typeIN, typeOUT, call, errorCondition, isTypeOUT, typeOUTTypename, dimension, Vector) \
  Dream3DTemplateAliasMacroCaseVectorImage1(typeIN, typeOUT, call, errorCondition, isTypeOUT, typeOUTTypename, dimension, Vector)
#define Dream3DTemplateAliasMacroCaseVectorImage1(typeIN, typeOUT, call, errorCondition, isTypeOUT, typeOUTTypename, dimension, Vector) \
  Dream3DTemplateAliasMacroCaseVectorImage1_##Vector(typeIN, typeOUT, call, errorCondition, isTypeOUT, typeOUTTypename, dimension)
// Vector images not accepted, throw an error message if a vector image is given.
#define Dream3DTemplateAliasMacroCaseVectorImage1_0(typeIN, typeOUT, call, errorCondition, isTypeOUT, typeOUTTypename, dimension)       \
  setErrorCondition(errorCondition);                                                                                                    \
  notifyErrorMessage(getHumanLabel(), "Vector not supported", getErrorCondition());
// Vector images: Call the given function with the correct dimension after defining the input and output vector types.
#define Dream3DTemplateAliasMacroCaseVectorImage1_1(typeIN, typeOUT, call, errorCondition, isTypeOUT, typeOUTTypename, dimension)       \
      if(cDims.size() == 2)                                                                                                             \
      {                                                                                                                                 \
        DefineVectorPixelTypes_##isTypeOUT(typeIN,typeOUT,2);                                                                           \
        Dream3DTemplateAliasMacroCaseIf(InputPixelType, OutputPixelType, call, typeOUTTypename, dimension);                             \
      }                                                                                                                                 \
      else if(cDims.size() == 3)                                                                                                        \
      {                                                                                                                                 \
        DefineVectorPixelTypes_##isTypeOUT(typeIN,typeOUT,3);                                                                           \
        Dream3DTemplateAliasMacroCaseIf(InputPixelType, OutputPixelType, call, typeOUTTypename, dimension);                             \
      }                                                                                                                                 \
      else                                                                                                                              \
      {                                                                                                                                 \
        setErrorCondition(errorCondition);                                                                                              \
        notifyErrorMessage(getHumanLabel(), "Vector dimension not supported", getErrorCondition());                                     \
      }
//////////////////////////////////////////////////////////////////////////////
//                          Handles RGBA images                             //
//////////////////////////////////////////////////////////////////////////////
// Expand 'RGBA' argument that will be used to call the appropriate macro, depending if the filter accepts 'RGBA' images or not.
#define  Dream3DTemplateAliasMacroCaseRGBAImage0(typeIN, typeOUT, call, errorCondition, isTypeOUT, typeOUTTypename, dimension, RGBA)    \
  Dream3DTemplateAliasMacroCaseRGBAImage1(typeIN, typeOUT, call, errorCondition, isTypeOUT, typeOUTTypename, dimension, RGBA)
#define   Dream3DTemplateAliasMacroCaseRGBAImage1(typeIN, typeOUT, call, errorCondition, isTypeOUT, typeOUTTypename, dimension, RGBA)   \
  Dream3DTemplateAliasMacroCaseRGBAImage1_##RGBA(typeIN, typeOUT, call, errorCondition, isTypeOUT, typeOUTTypename, dimension)
// If RGBA not accepted by the current filter, prints an error message
#define Dream3DTemplateAliasMacroCaseRGBAImage1_0(typeIN, typeOUT, call, errorCondition, isTypeOUT, typeOUTTypename, dimension)         \
  setErrorCondition(errorCondition);                                                                                                    \
  notifyErrorMessage(getHumanLabel(), "RGBA not supported", getErrorCondition());
// If RGBA accepted by current filter, call the macro that will call the given function
#define Dream3DTemplateAliasMacroCaseRGBAImage1_1(typeIN, typeOUT, call, errorCondition, isTypeOUT, typeOUTTypename, dimension)         \
  Dream3DTemplateAliasMacroCase_1_##isTypeOUT(itk::RGBAPixel<typeIN>, itk::RGBAPixel<typeOUT>, call, typeOUTTypename, dimension)

  
// Define a macro to dispatch calls to a template instantiated over
// the aliased scalar types.
#define Dream3DTemplateAliasMacro(call, var_type, typeOUT, tDims, errorCondition, isTypeOUT, typeOUTTypename)              \
  Dream3DTemplateAliasMacroCase(double, typeOUT, call, var_type, tDims, errorCondition, isTypeOUT, typeOUTTypename)        \
  else Dream3DTemplateAliasMacroCase(float, typeOUT, call, var_type, tDims, errorCondition, isTypeOUT, typeOUTTypename)    \
  else Dream3DTemplateAliasMacroCase(int8_t, typeOUT, call, var_type, tDims, errorCondition, isTypeOUT, typeOUTTypename)   \
  else Dream3DTemplateAliasMacroCase(uint8_t, typeOUT, call, var_type, tDims, errorCondition, isTypeOUT, typeOUTTypename)  \
  else Dream3DTemplateAliasMacroCase(int16_t, typeOUT, call, var_type, tDims, errorCondition, isTypeOUT, typeOUTTypename)  \
  else Dream3DTemplateAliasMacroCase(uint16_t, typeOUT, call, var_type, tDims, errorCondition, isTypeOUT, typeOUTTypename) \
  else Dream3DTemplateAliasMacroCase(int32_t, typeOUT, call, var_type, tDims, errorCondition, isTypeOUT, typeOUTTypename)  \
  else Dream3DTemplateAliasMacroCase(uint32_t, typeOUT, call, var_type, tDims, errorCondition, isTypeOUT, typeOUTTypename) \
  else Dream3DTemplateAliasMacroCase(int64_t, typeOUT, call, var_type, tDims, errorCondition, isTypeOUT, typeOUTTypename)  \
  else Dream3DTemplateAliasMacroCase(uint64_t, typeOUT, call, var_type, tDims, errorCondition, isTypeOUT, typeOUTTypename)

// Define a macro that is specific to Dream3D and dispatches calls to a template
// instantiated over the aliased scalar type based on the type of a data array
// which is saved in the filter's data container array.
#define Dream3DArraySwitchMacroLongOutputType(call, path, errorCondition, typeOUT, isTypeOUT, typeOUTTypename) \
  {                                                                                                            \
    IDataArray::Pointer ptr =                                                                                  \
      getDataContainerArray()->getPrereqIDataArrayFromPath<IDataArray, AbstractFilter>(this, path);            \
    if( ptr.get() != nullptr)                                                                                  \
    {                                                                                                          \
      ImageGeom::Pointer imageGeometry =                                                                       \
      getDataContainerArray()->getPrereqGeometryFromDataContainer<ImageGeom, AbstractFilter>(                  \
      this, path.getDataContainerName());                                                                      \
      if (imageGeometry.get() != nullptr)                                                                      \
      {                                                                                                        \
        QVector<size_t> tDims(3, 0);                                                                           \
        imageGeometry->getDimensions(tDims[0], tDims[1], tDims[2]);                                            \
        if (getErrorCondition() >= 0)                                                                          \
        {                                                                                                      \
          QString type = ptr->getTypeAsString();                                                               \
          Dream3DTemplateAliasMacro(call, type, typeOUT, tDims, errorCondition, isTypeOUT, typeOUTTypename);   \
        }                                                                                                      \
      }                                                                                                        \
      else                                                                                                     \
      {                                                                                                        \
        setErrorCondition(errorCondition);                                                                     \
        notifyErrorMessage(getHumanLabel(), "Geometry not found", getErrorCondition());                        \
      }                                                                                                        \
    }                                                                                                          \
    else                                                                                                       \
    {                                                                                                          \
      setErrorCondition(errorCondition);                                                                       \
      notifyErrorMessage(getHumanLabel(), "Array not found", getErrorCondition());                             \
    }                                                                                                          \
  }

// Define a macro that is specific to Dream3D and dispatches calls to a template
// instantiated over the aliased scalar type based on the type of a data array
// which is saved in the filter's data container array.
#define Dream3DArraySwitchMacroOutputType(call, path, errorCondition, typeOUT, typeOUTTypename)   \
    Dream3DArraySwitchMacroLongOutputType(call, path, errorCondition, typeOUT, 1, typeOUTTypename)
// Define a macro that is specific to Dream3D and dispatches calls to a template
// instantiated over the aliased scalar type based on the type of a data array
// which is saved in the filter's data container array. Output type is of the same
// type as the input data array.
#define Dream3DArraySwitchMacro(call, path, errorCondition) \
  Dream3DArraySwitchMacroLongOutputType(call, path, errorCondition, "", 0, 0)

// Define macro to select the output image component type.
// Subtract 1 to enum values because 'type' values are expected to start at 0 while
// itk::ImageIOBase::IOComponentType '0' value is UNKNOWNCOMPONENTTYPE and therefore
// should be skipped.
#define Dream3DArraySwitchOutputComponentMacro(call, type, path, errorCondition) \
   switch(type)\
   {\
     case itk::ImageIOBase::IOComponentType::UCHAR-1:\
             Dream3DArraySwitchMacroOutputType(call, path, -4,uint8_t,0);\
             break;\
     case itk::ImageIOBase::IOComponentType::CHAR-1:\
             Dream3DArraySwitchMacroOutputType(call, path, -4,int8_t,0);\
             break;\
     case itk::ImageIOBase::IOComponentType::USHORT-1:\
             Dream3DArraySwitchMacroOutputType(call, path, -4,uint16_t,0);\
             break;\
     case itk::ImageIOBase::IOComponentType::SHORT-1:\
             Dream3DArraySwitchMacroOutputType(call, path, -4,int16_t,0);\
             break;\
     case itk::ImageIOBase::IOComponentType::UINT-1:\
             Dream3DArraySwitchMacroOutputType(call, path, -4,uint32_t,0);\
             break;\
     case itk::ImageIOBase::IOComponentType::INT-1:\
             Dream3DArraySwitchMacroOutputType(call, path, -4,int32_t,0);\
             break;\
     case itk::ImageIOBase::IOComponentType::ULONG-1:\
             Dream3DArraySwitchMacroOutputType(call, path, -4,uint64_t,0);\
             break;\
     case itk::ImageIOBase::IOComponentType::LONG-1:\
             Dream3DArraySwitchMacroOutputType(call, path, -4,int64_t,0);\
             break;\
     case itk::ImageIOBase::IOComponentType::FLOAT-1:\
             Dream3DArraySwitchMacroOutputType(call, path, -4,float,0);\
             break;\
     case itk::ImageIOBase::IOComponentType::DOUBLE-1:\
             Dream3DArraySwitchMacroOutputType(call, path, -4,double,0);\
             break;\
     default:\
             setErrorCondition(-4);\
             notifyErrorMessage(getHumanLabel(), "Unsupported pixel component", errorCondition);\
             break;\
   }

#endif
