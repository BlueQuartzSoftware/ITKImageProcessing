

#include "ITKImageProcessingGuiPlugin.h"

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
ITKImageProcessingGuiPlugin::ITKImageProcessingGuiPlugin() = default;

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
ITKImageProcessingGuiPlugin::~ITKImageProcessingGuiPlugin() = default;

#include "ITKImageProcessing/Gui/FilterParameterWidgets/RegisterKnownFilterParameterWidgets.cpp"
