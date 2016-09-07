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

#include "ITKImageProcessingPlugin.h"

#include <QtCore/QFile>
#include <QtCore/QFileInfo>

#include "SIMPLib/Common/FilterManager.h"
#include "SIMPLib/Common/IFilterFactory.hpp"
#include "SIMPLib/Common/FilterFactory.hpp"

#include "ITKImageProcessing/ITKImageProcessingConstants.h"

// Include the MOC generated file for this class
#include "moc_ITKImageProcessingPlugin.cpp"

#include "itkSCIFIOImageIOFactory.h"
#include <itkJPEGImageIOFactory.h>
#include <itkMetaImageIOFactory.h>
#include <itkNrrdImageIOFactory.h>
#include <itkPNGImageIOFactory.h>
#include <itkTIFFImageIOFactory.h>
#include <itkNiftiImageIOFactory.h>
#include <itkPNGImageIOFactory.h>
#include <itkGiplImageIOFactory.h>
#include <itkVTKImageIOFactory.h>
#include <itkStimulateImageIOFactory.h>
#include <itkBioRadImageIOFactory.h>
#include <itkGE4ImageIOFactory.h>
#include <itkGE5ImageIOFactory.h>

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
ITKImageProcessingPlugin::ITKImageProcessingPlugin() :
m_Version("0.1.0"),                            // Initialize ITKImageProcessing's Version Number Here
m_CompatibilityVersion("0.1.0"), // Initialize ITKImageProcessing's Compatibility Version Number Here
m_Vendor("Vendor Name"),                                // Initialize ITKImageProcessing's Vendor Name Here
m_URL("URL"),                                           // Initialize Company URL Here
m_Location("Location"),                                 // Initialize ITKImageProcessing library Location Here
m_Description("Description"),                           // Initialize ITKImageProcessing's Description Here
m_Copyright("Copyright"),                               // Initialize ITKImageProcessing's Copyright Here
m_Filters(QList<QString>()),                        // Initialize ITKImageProcessing's List of Dependencies Here
m_DidLoad(false)
{
  itk::JPEGImageIOFactory::RegisterOneFactory();
  itk::NrrdImageIOFactory::RegisterOneFactory();
  itk::PNGImageIOFactory::RegisterOneFactory();
  itk::TIFFImageIOFactory::RegisterOneFactory();
  itk::MetaImageIOFactory::RegisterOneFactory();
  itk::NiftiImageIOFactory::RegisterOneFactory();
  itk::GiplImageIOFactory::RegisterOneFactory();
  itk::VTKImageIOFactory::RegisterOneFactory();
  itk::StimulateImageIOFactory::RegisterOneFactory();
  itk::BioRadImageIOFactory::RegisterOneFactory();
  itk::GE4ImageIOFactory::RegisterOneFactory();
  itk::GE5ImageIOFactory::RegisterOneFactory();
  itk::SCIFIOImageIOFactory::RegisterOneFactory();
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
ITKImageProcessingPlugin::~ITKImageProcessingPlugin()
{
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
QString ITKImageProcessingPlugin::getListSupportedFileExtensions()
{
  QString supportedExtensions = "*.png *.mhd *.mha *.nrrd *.nhdr *.tif *.jpg *.jpeg *.nii *.nii.gz *.gipl *.gipl.gz *.hdr";
  return supportedExtensions;
}


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
QString ITKImageProcessingPlugin::getPluginName()
{
  return (ITKImageProcessingConstants::ITKImageProcessingPluginDisplayName);
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
QString ITKImageProcessingPlugin::getVersion()
{
  return m_Version;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
QString ITKImageProcessingPlugin::getCompatibilityVersion()
{
  return m_CompatibilityVersion;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
QString ITKImageProcessingPlugin::getVendor()
{
  return m_Vendor;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
QString ITKImageProcessingPlugin::getURL()
{
  return m_URL;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
QString ITKImageProcessingPlugin::getLocation()
{
  return m_Location;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
QString ITKImageProcessingPlugin::getDescription()
{
  /* PLEASE UPDATE YOUR PLUGIN'S DESCRIPTION FILE.
  It is located at ITKImageProcessing/Resources/ITKImageProcessing/ITKImageProcessingDescription.txt */

  QFile licenseFile(":/ITKImageProcessing/ITKImageProcessingDescription.txt");
  QFileInfo licenseFileInfo(licenseFile);
  QString text = "<<--Description was not read-->>";

  if ( licenseFileInfo.exists() )
  {
    if ( licenseFile.open(QIODevice::ReadOnly | QIODevice::Text) )
    {
      QTextStream in(&licenseFile);
      text = in.readAll();
    }
  }
  return text;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
QString ITKImageProcessingPlugin::getCopyright()
{
  return m_Copyright;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
QString ITKImageProcessingPlugin::getLicense()
{
  /* PLEASE UPDATE YOUR PLUGIN'S LICENSE FILE.
  It is located at ITKImageProcessing/Resources/ITKImageProcessing/ITKImageProcessingLicense.txt */

  QFile licenseFile(":/ITKImageProcessing/ITKImageProcessingLicense.txt");
  QFileInfo licenseFileInfo(licenseFile);
  QString text = "<<--License was not read-->>";

  if ( licenseFileInfo.exists() )
  {
    if ( licenseFile.open(QIODevice::ReadOnly | QIODevice::Text) )
    {
      QTextStream in(&licenseFile);
      text = in.readAll();
    }
  }
  return text;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
QMap<QString, QString> ITKImageProcessingPlugin::getThirdPartyLicenses()
{
  QMap<QString, QString> licenseMap;
  QList<QString> fileStrList;
  fileStrList.push_back(":/ThirdParty/HDF5.txt");
  fileStrList.push_back(":/ThirdParty/Boost.txt");
  fileStrList.push_back(":/ThirdParty/Qt.txt");
  fileStrList.push_back(":/ThirdParty/Qwt.txt");

  for (QList<QString>::iterator iter = fileStrList.begin(); iter != fileStrList.end(); iter++)
  {
    QFile file(*iter);
    QFileInfo licenseFileInfo(file);

    if ( licenseFileInfo.exists() )
    {
      if ( file.open(QIODevice::ReadOnly | QIODevice::Text) )
      {
        QTextStream in(&file);
        licenseMap.insert(licenseFileInfo.baseName(), in.readAll());
      }
    }
  }

  return licenseMap;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
bool ITKImageProcessingPlugin::getDidLoad()
{
  return m_DidLoad;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void ITKImageProcessingPlugin::setDidLoad(bool didLoad)
{
  m_DidLoad = didLoad;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void ITKImageProcessingPlugin::setLocation(QString filePath)
{
  m_Location = filePath;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void ITKImageProcessingPlugin::writeSettings(QSettings& prefs)
{
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void ITKImageProcessingPlugin::readSettings(QSettings& prefs)
{
}

#include "ITKImageProcessingFilters/RegisterKnownFilters.cpp"

#include "FilterParameterWidgets/RegisterKnownFilterParameterWidgets.cpp"

