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
ZeissImportPlugin::ZeissImportPlugin()
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

