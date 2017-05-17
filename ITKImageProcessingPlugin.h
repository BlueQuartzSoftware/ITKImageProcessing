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
* The code contained herein was partially funded by the followig contracts:
*    United States Air Force Prime Contract FA8650-10-D-5210
*
* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */

#ifndef _ITKImageProcessing_H_
#define _ITKImageProcessing_H_

#include <QtCore/QObject>
#include <QtCore/QSettings>

#include "SIMPLib/Plugin/ISIMPLibPlugin.h"

/**
 * @brief The ITKImageProcessingPlugin class
 */
class ITKImageProcessingPlugin : public QObject, public ISIMPLibPlugin
{
  Q_OBJECT
  Q_INTERFACES(ISIMPLibPlugin)
  Q_PLUGIN_METADATA(IID "com.your.domain.ITKImageProcessingPlugin")

public:
  ITKImageProcessingPlugin();
  virtual ~ITKImageProcessingPlugin();

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
   * @brief getPluginName Returns the name of the plugin
   */
  virtual QString getPluginName();

  /**
   * @brief getVersion Returns the version
   */
  virtual QString getVersion();

  /**
  * @brief getCompatibilityVersion Returns the compatibility version
  */
  virtual QString getCompatibilityVersion();

  /**
  * @brief getVendor Returns the name of the vendor
  */
  virtual QString getVendor();

  /**
   * @brief getURL Returns the URL of the plugin
   */
  virtual QString getURL();

  /**
   * @brief getLocationReturns the location of the plugin
   */
  virtual QString getLocation();

  /**
   * @brief getDescription Returns the description of the plugin
   * PLEASE UPDATE YOUR PLUGIN'S DESCRIPTION FILE.
   * It is located at ITKImageProcessing/Resources/ITKImageProcessing/ITKImageProcessingDescription.txt
   */
  virtual QString getDescription();

  /**
   * @brief getCopyright Returns the copyright of the plugin
   */
  virtual QString getCopyright();

  /**
   * @brief getLicense Returns the license of the plugin
   * PLEASE UPDATE YOUR PLUGIN'S LICENSE FILE.
   * It is located at ITKImageProcessing/Resources/ITKImageProcessing/ITKImageProcessingLicense.txt
   */
  virtual QString getLicense();

  /**
   * @brief getFilters Returns the filters of the plugin
   */
  virtual QList<QString> getFilters();

  /**
   * @brief getThirdPartyLicenses Returns the third party licenses of the plugin
   */
  virtual QMap<QString, QString> getThirdPartyLicenses();

  /**
   * @brief getDidLoad Returns the load status of the plugin
   */
  virtual bool getDidLoad();

  /**
   * @brief setDidLoad Sets the load status of the plugin
   * @param didLoad Boolean value to set status
   */
  virtual void setDidLoad(bool didLoad);

  /**
   * @brief setLocation Sets the location of the plugin on the file system.
   * This is required so that we can cache the file path information
   * as the plugin is loaded.
   * @param filePath File path
   */
  virtual void setLocation(QString filePath);

  /**
   * @brief registerFilterWidgets Register all the filters with the FilterWidgetFactory
   * @param fwm FilterWidgetManager instance pointer
   */
  virtual void registerFilterWidgets(FilterWidgetManager* fwm);

  /**
  * @brief registerFilters Registers the filters that this plugin implements with the Filter Manager that is passed in
  * @param fm FilterManager instance pointer
  */
  virtual void registerFilters(FilterManager* fm);

  /**
   * @brief writeSettings Writes the settings in the input gui to the Application's preference file
   * @param prefs QSettings reference variable
   */
  virtual void writeSettings(QSettings& prefs);

  /**
   * @brief readSettings Reads the settings from the Application's preference file and sets
   * the input GUI widgets accordingly.
   * @param prefs QSettings reference variable
   */
  virtual void readSettings(QSettings& prefs);

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

  ITKImageProcessingPlugin(const ITKImageProcessingPlugin&); // Copy Constructor Not Implemented
  void operator=(const ITKImageProcessingPlugin&);           // Operator '=' Not Implemented
};

#endif /* _ITKImageProcessing_H_ */
