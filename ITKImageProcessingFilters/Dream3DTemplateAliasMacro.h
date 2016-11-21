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
//--------------------------------------------------------------------------

// Define helper macros to switch types on and off.
#define Q(x) #x
#define QUOTE(x) Q(x)
/*#define DREAM3D_USE_typename   1 // No error at compile time since it is difficult to know what exact type is used in this case.
#define TypeOUT_1(type) //empty, does nothing: this output type is "ON"
#define TypeOUT_0(type)\
  static_assert(false, "You shouldn't use that."); // Error at compile time. If defined, output type is hardcoded in filter.
#define TypeOUT_DREAM3D0(var, type)\
  TypeOUT_DREAM3D1(var, type)
#define TypeOUT_DREAM3D1(var, type)\
  TypeOUT_##var(type)
#define TypeOUT_Define_1(var)\
  TypeOUT_DREAM3D0(DREAM3D_USE_##var, var)
#define TypeOUT_Define_0(var) //empty, does nothing: No output type specified, so no need to check anything
We assume TypeOUT is handled. If not, there will be an error at some point.
TypeOUT_Define_##isTypeOUT(typeOUT)*/

#define Dream3DTemplateAliasMacroCase(typeIN, typeOUT, call, var_type, dimensions, errorCondition, isTypeOUT, typeOUTTypename) \
  Dream3DTemplateAliasMacroCase0(typeIN, typeOUT, call, var_type, dimensions, errorCondition, DREAM3D_USE_##typeIN, isTypeOUT, typeOUTTypename)
#define Dream3DTemplateAliasMacroCase0(typeIN, typeOUT, call, var_type, dimensions, errorCondition, value, isTypeOUT, typeOUTTypename) \
  Dream3DTemplateAliasMacroCase1(typeIN, typeOUT, call, var_type, dimensions, errorCondition, value, isTypeOUT, typeOUTTypename)
#define Dream3DTemplateAliasMacroCase1(typeIN, typeOUT, call, var_type, dimensions, errorCondition, value, isTypeOUT, typeOUTTypename) \
  Dream3DTemplateAliasMacroCase_##value(typeIN, typeOUT, call, var_type, dimensions, errorCondition, QUOTE(typeIN), isTypeOUT, typeOUTTypename)
#define Dream3DTemplateAliasMacroCase_0(typeIN, typeOUT, call, var_type, dimensions, errorCondition, quotedType, isTypeOUT, typeOUTTypename) \
  if( var_type.compare(quotedType) == 0 )                                                                                                    \
  {                                                                                                                                          \
    setErrorCondition(errorCondition);                                                                                                       \
    QString errorMessage = QString("Unsupported pixel type: %1.").arg(quotedType);                                                           \
    notifyErrorMessage(getHumanLabel(), errorMessage, getErrorCondition());                                                                  \
  }

#define Dream3DTemplateAliasMacroCase_1(typeIN, typeOUT, call, var_type, dimensions, errorCondition, quotedType, isTypeOUT, typeOUTTypename) \
  Dream3DTemplateAliasMacroCase_1_##isTypeOUT(typeIN, typeOUT, call, var_type, dimensions, errorCondition, quotedType, typeOUTTypename)

#define Dream3DTemplateAliasMacroCase_1_0(typeIN, typeOUT, call, var_type, dimensions, errorCondition, quotedType, typeOUTTypename) \
  Dream3DTemplateAliasMacroCaseIf(typeIN, typeIN, call, var_type, dimensions, errorCondition, quotedType, 0)
#define Dream3DTemplateAliasMacroCase_1_1(typeIN, typeOUT, call, var_type, dimensions, errorCondition, quotedType, typeOUTTypename) \
  Dream3DTemplateAliasMacroCaseIf(typeIN, typeOUT, call, var_type, dimensions, errorCondition, quotedType, typeOUTTypename)
// 3 possible pixel type: C-type, InputImageType, and TImageType
#define DefineInputImageType0(typeIN, dimension) // Do nothing, no type to defined
#define DefineInputImageType1(typeIN, dimension) \
  typedef typename itk::Dream3DImage<typeIN, dimension> InputImageType;
#define DefineInputImageType2(typeIN, dimension) \
  typedef typename itk::Dream3DImage<typeIN, dimension> TImageType;

#define Dream3DTemplateAliasMacroCaseIf(typeIN, typeOUT, call, var_type, dimensions, errorCondition, quotedType, typeOUTTypename) \
if( var_type.compare(quotedType) == 0 )                                                                                           \
  {                                                                                                                               \
    if (dimensions[2] == 1)                                                                                                       \
    {                                                                                                                             \
      /* 2D image */                                                                                                              \
      DefineInputImageType##typeOUTTypename(typeIN, 2)                                                                            \
      call<typeIN, typeOUT, 2>();                                                                                                 \
    }                                                                                                                             \
    else                                                                                                                          \
    {                                                                                                                             \
      /* 3D */                                                                                                                    \
      DefineInputImageType##typeOUTTypename(typeIN, 3)                                                                            \
      call<typeIN, typeOUT, 3>();                                                                                                 \
    }                                                                                                                             \
  }
  
// Define a macro to dispatch calls to a template instantiated over
// the aliased scalar types.
#define Dream3DTemplateAliasMacro(call, var_type, typeOUT, dimensions, errorCondition, isTypeOUT, typeOUTTypename)              \
  Dream3DTemplateAliasMacroCase(double, typeOUT, call, var_type, dimensions, errorCondition, isTypeOUT, typeOUTTypename)        \
  else Dream3DTemplateAliasMacroCase(float, typeOUT, call, var_type, dimensions, errorCondition, isTypeOUT, typeOUTTypename)    \
  else Dream3DTemplateAliasMacroCase(int8_t, typeOUT, call, var_type, dimensions, errorCondition, isTypeOUT, typeOUTTypename)   \
  else Dream3DTemplateAliasMacroCase(uint8_t, typeOUT, call, var_type, dimensions, errorCondition, isTypeOUT, typeOUTTypename)  \
  else Dream3DTemplateAliasMacroCase(int16_t, typeOUT, call, var_type, dimensions, errorCondition, isTypeOUT, typeOUTTypename)  \
  else Dream3DTemplateAliasMacroCase(uint16_t, typeOUT, call, var_type, dimensions, errorCondition, isTypeOUT, typeOUTTypename) \
  else Dream3DTemplateAliasMacroCase(int32_t, typeOUT, call, var_type, dimensions, errorCondition, isTypeOUT, typeOUTTypename)  \
  else Dream3DTemplateAliasMacroCase(uint32_t, typeOUT, call, var_type, dimensions, errorCondition, isTypeOUT, typeOUTTypename) \
  else Dream3DTemplateAliasMacroCase(int64_t, typeOUT, call, var_type, dimensions, errorCondition, isTypeOUT, typeOUTTypename)  \
  else Dream3DTemplateAliasMacroCase(uint64_t, typeOUT, call, var_type, dimensions, errorCondition, isTypeOUT, typeOUTTypename)

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
#define Dream3DArraySwitchMacroOutputType(call, path, errorCondition, typeOUT, typeOUTTypename) \
    Dream3DArraySwitchMacroLongOutputType(call, path, errorCondition, typeOUT, 1, typeOUTTypename)
// Define a macro that is specific to Dream3D and dispatches calls to a template
// instantiated over the aliased scalar type based on the type of a data array
// which is saved in the filter's data container array. Output type is of the same
// type as the input data array.
#define Dream3DArraySwitchMacro(call, path, errorCondition) \
  Dream3DArraySwitchMacroLongOutputType(call, path, errorCondition, "", 0, 0)

#endif
