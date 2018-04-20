

#include "ITKImageProcessingGuiPlugin.h"

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
ITKImageProcessingGuiPlugin::ITKImageProcessingGuiPlugin()
: ITKImageProcessingPlugin()
{
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
ITKImageProcessingGuiPlugin::~ITKImageProcessingGuiPlugin() = default;

#include "ITKImageProcessing/Gui/FilterParameterWidgets/RegisterKnownFilterParameterWidgets.cpp"
