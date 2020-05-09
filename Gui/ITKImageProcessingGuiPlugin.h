#pragma once

#include "ITKImageProcessing/ITKImageProcessingPlugin.h"

class ITKImageProcessingGuiPlugin : public ITKImageProcessingPlugin
{
  Q_OBJECT
  Q_INTERFACES(ISIMPLibPlugin)
  Q_PLUGIN_METADATA(IID "net.bluequartz.dream3d.ITKImageProcessingGuiPlugin")

public:
  ITKImageProcessingGuiPlugin();
  ~ITKImageProcessingGuiPlugin() override;

  /**
   * @brief Register all the filters with the FilterWidgetFactory
   */
  void registerFilterWidgets(FilterWidgetManager* fwm) override;

public:
  ITKImageProcessingGuiPlugin(const ITKImageProcessingGuiPlugin&) = delete;            // Copy Constructor Not Implemented
  ITKImageProcessingGuiPlugin(ITKImageProcessingGuiPlugin&&) = delete;                 // Move Constructor Not Implemented
  ITKImageProcessingGuiPlugin& operator=(const ITKImageProcessingGuiPlugin&) = delete; // Copy Assignment Not Implemented
  ITKImageProcessingGuiPlugin& operator=(ITKImageProcessingGuiPlugin&&) = delete;      // Move Assignment Not Implemented
};
