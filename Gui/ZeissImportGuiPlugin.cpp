

#include "ZeissImportGuiPlugin.h"

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
ZeissImportGuiPlugin::ZeissImportGuiPlugin()
: ZeissImportPlugin()
{
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
ZeissImportGuiPlugin::~ZeissImportGuiPlugin() = default;

#include "ZeissImport/Gui/FilterParameterWidgets/RegisterKnownFilterParameterWidgets.cpp"
