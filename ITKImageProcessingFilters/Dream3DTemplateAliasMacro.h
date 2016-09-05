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
#define Dream3DTemplateAliasMacroCase(typeN, call, var_type, errorCondition)                         \
  Dream3DTemplateAliasMacroCase0(typeN, call, var_type, errorCondition, DREAM3D_USE_##typeN)
#define Dream3DTemplateAliasMacroCase0(typeN, call, var_type, errorCondition, value)                 \
  Dream3DTemplateAliasMacroCase1(typeN, call, var_type, errorCondition, value)
#define Dream3DTemplateAliasMacroCase1(typeN, call, var_type, errorCondition, value)                 \
  Dream3DTemplateAliasMacroCase_##value(typeN, call, var_type, errorCondition, QUOTE(typeN))
#define Dream3DTemplateAliasMacroCase_0(typeN, call, var_type, errorCondition, quotedType)           \
  if( var_type.compare(quotedType) == 0 )                                                            \
  {                                                                                                  \
    setErrorCondition(errorCondition);\
    QString errorMessage = QString("Unsupported pixel type: %1.").arg(quotedType);                   \
    notifyErrorMessage(getHumanLabel(), errorMessage, getErrorCondition());                          \
  }
#define Dream3DTemplateAliasMacroCase_1(typeN, call, var_type, errorCondition, quotedType)           \
  if( var_type.compare(quotedType) == 0 )                                                            \
  {                                                                                                  \
    call<typeN>();                                                                                   \
  }


// Define a macro to dispatch calls to a template instantiated over
// the aliased scalar types.
#define Dream3DTemplateAliasMacro(call, var_type, errorCondition)                                  \
  Dream3DTemplateAliasMacroCase(double, call, var_type, errorCondition)                            \
  else Dream3DTemplateAliasMacroCase(float, call, var_type, errorCondition)                        \
  else Dream3DTemplateAliasMacroCase(int8_t, call, var_type, errorCondition)                       \
  else Dream3DTemplateAliasMacroCase(uint8_t, call, var_type, errorCondition)                      \
  else Dream3DTemplateAliasMacroCase(int16_t, call, var_type, errorCondition)                      \
  else Dream3DTemplateAliasMacroCase(uint16_t, call, var_type, errorCondition)                     \
  else Dream3DTemplateAliasMacroCase(int32_t, call, var_type, errorCondition)                      \
  else Dream3DTemplateAliasMacroCase(uint32_t, call, var_type, errorCondition)                     \
  else Dream3DTemplateAliasMacroCase(int64_t, call, var_type, errorCondition)                      \
  else Dream3DTemplateAliasMacroCase(uint64_t, call, var_type, errorCondition)

// Define a macro that is specific to Dream3D and dispatches calls to a template
// instantiated over the aliased scalar type based on the type of a data array
// which is saved in the filter's data container array.
#define Dream3DArraySwitchMacro(call, path, errorCondition)\
  {                                                                                                  \
    IDataArray::Pointer array =                                                                      \
      getDataContainerArray()->getPrereqIDataArrayFromPath<IDataArray, AbstractFilter>(this, path);  \
    if (getErrorCondition() >= 0)                                                                    \
    {                                                                                                \
      QString type = array->getTypeAsString();                                                       \
      Dream3DTemplateAliasMacro(call, type, errorCondition);                                         \
    }                                                                                                \
  }

#endif