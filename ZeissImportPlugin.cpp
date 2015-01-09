/*
 * Your License or Copyright Information can go here
 */


#include "ZeissImportPlugin.h"

#include "DREAM3DLib/Common/FilterManager.h"
#include "DREAM3DLib/Common/IFilterFactory.hpp"
#include "DREAM3DLib/Common/FilterFactory.hpp"


#include "ZeissImport/moc_ZeissImportPlugin.cpp"

Q_EXPORT_PLUGIN2(ZeissImportPlugin, ZeissImportPlugin)

namespace Detail
{
  const QString ZeissImportPluginFile("ZeissImportPlugin");
  const QString ZeissImportPluginDisplayName("ZeissImportPlugin");
  const QString ZeissImportPluginBaseName("ZeissImportPlugin");
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
ZeissImportPlugin::ZeissImportPlugin() :
m_Version(""),
m_CompatibilityVersion(""),
m_Vendor(""),
m_Group(""),
m_URL(""),
m_Location(""),
m_Platforms(QList<QString>()),
m_Description(""),
m_Copyright(""),
m_License(""),
m_Dependencies(QList<QString>())
{

}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
ZeissImportPlugin::~ZeissImportPlugin()
{
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
QString ZeissImportPlugin::getPluginName()
{
  return (Detail::ZeissImportPluginDisplayName);
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
QString ZeissImportPlugin::getVersion()
{
  return "";
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
QString ZeissImportPlugin::getCompatibilityVersion()
{
  return "";
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
QString ZeissImportPlugin::getVendor()
{
  return "";
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
QString ZeissImportPlugin::getGroup()
{
  return "";
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
QString ZeissImportPlugin::getURL()
{
  return "";
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
QString ZeissImportPlugin::getLocation()
{
  return "";
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
QList<QString> ZeissImportPlugin::getPlatforms()
{
  QList<QString> empty;
  return empty;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
QString ZeissImportPlugin::getDescription()
{
  return "";
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
QString ZeissImportPlugin::getCopyright()
{
  return "";
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
QString ZeissImportPlugin::getLicense()
{
  return "";
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
QList<QString> ZeissImportPlugin::getDependencies()
{
  QList<QString> empty;
  return empty;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void ZeissImportPlugin::writeSettings(QSettings& prefs)
{

}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void ZeissImportPlugin::readSettings(QSettings& prefs)
{

}

#include "ZeissImportFilters/RegisterKnownFilters.cpp"

#include "FilterParameterWidgets/RegisterKnownFilterParameterWidgets.cpp"

