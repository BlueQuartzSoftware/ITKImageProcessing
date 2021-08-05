/* ============================================================================
 * Copyright (c) 2009-2016 BlueQuartz Software, LLC
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 * Redistributions of source code must retain the above copyright notice, this
 * list of conditions and the following disclaimer.
 *
 * Redistributions in binary form must reproduce the above copyright notice, this
 * list of conditions and the following disclaimer in the documentation and/or
 * other materials provided with the distribution.
 *
 * Neither the name of BlueQuartz Software, the US Air Force, nor the names of its
 * contributors may be used to endorse or promote products derived from this software
 * without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE
 * USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 * The code contained herein was partially funded by the following contracts:
 *    United States Air Force Prime Contract FA8650-10-D-5210
 *
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */

#pragma once

#include <QtCore/QObject>
#include <QtCore/QSettings>

#include "SIMPLib/Plugin/ISIMPLibPlugin.h"

#include "ITKImageProcessing/ITKImageProcessingDLLExport.h"

/**
 * @brief The ITKImageProcessingPlugin class
 */
class ITKImageProcessing_EXPORT ITKImageProcessingPlugin : public QObject, public ISIMPLibPlugin

{
  Q_OBJECT
  Q_INTERFACES(ISIMPLibPlugin)
  Q_PLUGIN_METADATA(IID "net.bluequartz.dream3d.ITKImageProcessingPlugin")

public:
  ITKImageProcessingPlugin();
  ~ITKImageProcessingPlugin() override;

  /**
   * @brief get list of supported read extensions as QString
   */
  static QString getListSupportedReadExtensions();

  /**
   * @brief get list of supported write extensions as QString
   */
  static QString getListSupportedWriteExtensions();

  /**
   * @brief get list of supported 2D file extensions as QStringList
   */
  static QStringList getList2DSupportedFileExtensions();

  /**
   * @brief set environment variables required to find java executable and java packages packaged in DREAM3D
   */
  static void setSCIFIOEnvironmentVariables();

  /**
   * @brief Returns the name of the plugin that appears on the file system.
   *
   * Note that if the build is a debug build there will be a _Plugin postfix
   * to the filename.
   */
  QString getPluginFileName() override;

  /**
   * @brief getPluginDisplayName The name that should be used for human facing
   * labels and display strings
   * @return
   */
  QString getPluginDisplayName() override;

  /**
   * @brief getPluginBaseName The Name of the plugin.
   *
   * This typically will NOT have the Plugin suffix.
   * @return
   */
  QString getPluginBaseName() override;

  /**
   * @brief getVersion Returns the version
   */
  QString getVersion() override;

  /**
   * @brief getCompatibilityVersion Returns the compatibility version
   */
  QString getCompatibilityVersion() override;

  /**
   * @brief getVendor Returns the name of the vendor
   */
  QString getVendor() override;

  /**
   * @brief getURL Returns the URL of the plugin
   */
  QString getURL() override;

  /**
   * @brief getLocationReturns the location of the plugin
   */
  QString getLocation() override;

  /**
   * @brief getDescription Returns the description of the plugin
   * PLEASE UPDATE YOUR PLUGIN'S DESCRIPTION FILE.
   * It is located at ITKImageProcessing/Resources/ITKImageProcessing/ITKImageProcessingDescription.txt
   */
  QString getDescription() override;

  /**
   * @brief getCopyright Returns the copyright of the plugin
   */
  QString getCopyright() override;

  /**
   * @brief getLicense Returns the license of the plugin
   * PLEASE UPDATE YOUR PLUGIN'S LICENSE FILE.
   * It is located at ITKImageProcessing/Resources/ITKImageProcessing/ITKImageProcessingLicense.txt
   */
  QString getLicense() override;

  /**
   * @brief getFilters Returns the filters of the plugin
   */
  QList<QString> getFilters() override;

  /**
   * @brief getThirdPartyLicenses Returns the third party licenses of the plugin
   */
  QMap<QString, QString> getThirdPartyLicenses() override;

  /**
   * @brief getDidLoad Returns the load status of the plugin
   */
  bool getDidLoad() override;

  /**
   * @brief setDidLoad Sets the load status of the plugin
   * @param didLoad Boolean value to set status
   */
  void setDidLoad(bool didLoad) override;

  /**
   * @brief setLocation Sets the location of the plugin on the file system.
   * This is required so that we can cache the file path information
   * as the plugin is loaded.
   * @param filePath File path
   */
  void setLocation(QString filePath) override;

  /**
   * @brief registerFilterWidgets Register all the filters with the FilterWidgetFactory
   * @param fwm FilterWidgetManager instance pointer
   */
  void registerFilterWidgets(FilterWidgetManager* fwm) override;

  /**
   * @brief registerFilters Registers the filters that this plugin implements with the Filter Manager that is passed in
   * @param fm FilterManager instance pointer
   */
  void registerFilters(FilterManager* fm) override;

  /**
   * @brief writeSettings Writes the settings in the input gui to the Application's preference file
   * @param prefs QSettings reference variable
   */
  void writeSettings(QSettings& prefs) override;

  /**
   * @brief readSettings Reads the settings from the Application's preference file and sets
   * the input GUI widgets accordingly.
   * @param prefs QSettings reference variable
   */
  void readSettings(QSettings& prefs) override;

private:
  QString m_Version;
  QString m_CompatibilityVersion;
  QString m_Vendor;
  QString m_URL;
  QString m_Location;
  QString m_Description;
  QString m_Copyright;
  QList<QString> m_Filters;
  bool m_DidLoad;

  ITKImageProcessingPlugin(const ITKImageProcessingPlugin&) = delete; // Copy Constructor Not Implemented
  void operator=(const ITKImageProcessingPlugin&) = delete;           // Move assignment Not Implemented
};
