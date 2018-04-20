#pragma once

#include "ZeissImport/ZeissImportPlugin.h"

class ZeissImportGuiPlugin : public ZeissImportPlugin
{
  Q_OBJECT
  Q_INTERFACES(ISIMPLibPlugin)
  Q_PLUGIN_METADATA(IID "net.bluequartz.dream3d.ZeissImportGuiPlugin")

public:
  ZeissImportGuiPlugin();
  ~ZeissImportGuiPlugin() override;

public:
  ZeissImportGuiPlugin(const ZeissImportGuiPlugin&) = delete;            // Copy Constructor Not Implemented
  ZeissImportGuiPlugin(ZeissImportGuiPlugin&&) = delete;                 // Move Constructor
  ZeissImportGuiPlugin& operator=(const ZeissImportGuiPlugin&) = delete; // Copy Assignment Not Implemented
  ZeissImportGuiPlugin& operator=(ZeissImportGuiPlugin&&) = delete;      // Move Assignment Not Implemented
};
