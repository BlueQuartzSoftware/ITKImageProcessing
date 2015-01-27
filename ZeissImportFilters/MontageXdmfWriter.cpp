#include "MontageXdmfWriter.h"


#include <fstream>

#include <QtCore/QtDebug>
#include <QtCore/QFileInfo>
#include <QtCore/QDir>
#include <QtCore/QFile>

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
MontageXdmfWriter::MontageXdmfWriter() :
  AbstractFilter(),
  m_OutputFile(),
  m_DataArrayNamesPath(),
  m_OriginsDataArrayPath(),
  m_AttributeMatrixName(),
  m_DREAM3DFileName()
{
  setupFilterParameters();
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
MontageXdmfWriter::~MontageXdmfWriter()
{

}


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void MontageXdmfWriter::setupFilterParameters()
{
  FilterParameterVector parameters;

  parameters.push_back(FilterParameter::New("Name of DREAM3D File", "DREAM3DFileName", FilterParameterWidgetType::StringWidget, getDREAM3DFileName(), false, ""));
  parameters.push_back(FilterParameter::New("Origins Data Array", "OriginsDataArrayPath", FilterParameterWidgetType::DataArraySelectionWidget, getOriginsDataArrayPath(), false, ""));
  parameters.push_back(FilterParameter::New("Data Array Names", "DataArrayNamesPath", FilterParameterWidgetType::DataArraySelectionWidget, getDataArrayNamesPath(), false, ""));

  parameters.push_back(FilterParameter::New("Attribute Matrix Name", "AttributeMatrixName", FilterParameterWidgetType::AttributeMatrixSelectionWidget, getAttributeMatrixName(), false, ""));
  parameters.push_back(FileSystemFilterParameter::New("Output File", "OutputFile", FilterParameterWidgetType::OutputFileWidget, getOutputFile(), false, "", "*.xdmf", "XDMF Files"));

  setFilterParameters(parameters);
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void MontageXdmfWriter::readFilterParameters(AbstractFilterParametersReader* reader, int index)
{
  reader->openFilterGroup(this, index);
  setDREAM3DFileName(reader->readString("DREAM3DFileName", getDREAM3DFileName()));
  setOriginsDataArrayPath(reader->readDataArrayPath("OriginsDataArrayPath", getOriginsDataArrayPath()));
  setDataArrayNamesPath(reader->readDataArrayPath("DataArrayNamesPath", getDataArrayNamesPath()));
  setAttributeMatrixName(reader->readDataArrayPath("AttributeMatrixName", getAttributeMatrixName()));
  setOutputFile(reader->readString("OutputFile", getOutputFile()));
  reader->closeFilterGroup();
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
int MontageXdmfWriter::writeFilterParameters(AbstractFilterParametersWriter* writer, int index)
{
  writer->openFilterGroup(this, index);

  DREAM3D_FILTER_WRITE_PARAMETER(DREAM3DFileName)
      DREAM3D_FILTER_WRITE_PARAMETER(OriginsDataArrayPath)
      DREAM3D_FILTER_WRITE_PARAMETER(DataArrayNamesPath)
      DREAM3D_FILTER_WRITE_PARAMETER(AttributeMatrixName)
      DREAM3D_FILTER_WRITE_PARAMETER(OutputFile)

      writer->closeFilterGroup();
  return ++index; // we want to return the next index that was just written to
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void MontageXdmfWriter::dataCheck()
{
  setErrorCondition(0);

  if (getOutputFile().isEmpty() == true)
  {
    QString ss = QObject::tr( ": The output file must be set before executing this filter.");
    notifyErrorMessage(getHumanLabel(), ss, -1000);
    setErrorCondition(-1);
  }
  QFileInfo fi(getOutputFile());
  QDir parentPath = fi.path();
  if (parentPath.exists() == false)
  {
    QString ss = QObject::tr( "The directory path for the output file does not exist.");
    notifyWarningMessage(getHumanLabel(), ss, -1);
  }

  if (fi.suffix().compare("") == 0)
  {
    setOutputFile(getOutputFile().append(".xdmf"));
  }


}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void MontageXdmfWriter::preflight()
{
  // These are the REQUIRED lines of CODE to make sure the filter behaves correctly
  setInPreflight(true); // Set the fact that we are preflighting.
  emit preflightAboutToExecute(); // Emit this signal so that other widgets can do one file update
  emit updateFilterParameters(this); // Emit this signal to have the widgets push their values down to the filter
  dataCheck(); // Run our DataCheck to make sure everthing is setup correctly
  emit preflightExecuted(); // We are done preflighting this filter
  setInPreflight(false); // Inform the system this filter is NOT in preflight mode anymore.
}



// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void MontageXdmfWriter::execute()
{
  int err = 0;
  int xval = 0;
  int yval = 0;
  // typically run your dataCheck function to make sure you can get that far and all your variables are initialized
  dataCheck();
  // Check to make sure you made it through the data check. Errors would have been reported already so if something
  // happens to fail in the dataCheck() then we simply return
  if(getErrorCondition() < 0) { return; }
  setErrorCondition(0);


  QFile ofile(getOutputFile());
  bool didOpen = ofile.open(QFile::WriteOnly);
  if(!didOpen)
  {
    QString ss = QObject::tr("Opening XDMF file for writing. Is the file already open? Do you have write permissions to the file or parent folder");
    setErrorCondition(-3400);
    notifyErrorMessage(getHumanLabel(), ss, getErrorCondition());
    return;
  }

  QTextStream out(&ofile);

  writeHeader(out);




  writeFooter(out);

  ofile.close();


  /* Let the GUI know we are done with this filter */
  notifyStatusMessage(getHumanLabel(), "Complete");
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void MontageXdmfWriter::writeHeader(QTextStream& out)
{
  out << "<?xml version=\"1.0\"?>" << "\n";
  out << "<!DOCTYPE Xdmf SYSTEM \"Xdmf.dtd\"[]>" << "\n";
  out << "<Xdmf xmlns:xi=\"http://www.w3.org/2003/XInclude\" Version=\"2.2\">" << "\n";
  out << "<Domain>" << "\n";
  out << "<Grid CollectionType=\"Spatial\" GridType=\"Collection\" Name=\"FieldData\">" << "\n";
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void MontageXdmfWriter::writeFooter(QTextStream& out)
{
  out << "</Grid>" << "\n";
  out << "</Domain>" << "\n";
  out << "</Xdmf>" << "\n";
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
const QString MontageXdmfWriter::getCompiledLibraryName()
{
  return ZeissImport::ZeissImportBaseName;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
const QString MontageXdmfWriter::getGroupName()
{
  return "IO";
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
const QString MontageXdmfWriter::getHumanLabel()
{
  return "Write Xdmf Montage";
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
const QString MontageXdmfWriter::getSubGroupName()
{
  return "Output";
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
AbstractFilter::Pointer MontageXdmfWriter::newFilterInstance(bool copyFilterParameters)
{
  MontageXdmfWriter::Pointer filter = MontageXdmfWriter::New();
  if(true == copyFilterParameters)
  {
    copyFilterParameterInstanceVariables(filter.get());
  }
  return filter;
}

