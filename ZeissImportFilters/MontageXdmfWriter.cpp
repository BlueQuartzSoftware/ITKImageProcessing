/* ============================================================================
 * Copyright (c) 2014 Michael A. Jackson (BlueQuartz Software)
 * All rights reserved.
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
 * Neither the name of Michael A. Jackson, BlueQuartz Software nor the names of
 * its contributors may be used to endorse or promote products derived from this
 * software without specific prior written permission.
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
 *
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */

#include "MontageXdmfWriter.h"


#include <fstream>

#include <QtCore/QtDebug>
#include <QtCore/QFileInfo>
#include <QtCore/QDir>
#include <QtCore/QFile>


#include "DREAM3DLib/DataArrays/StringDataArray.hpp"

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
MontageXdmfWriter::MontageXdmfWriter() :
  AbstractFilter(),
  m_OutputFile(),
  m_DataArrayNamesPath(),
  m_OriginsDataArrayPath(),
  m_ResolutionsDataArrayPath(),
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
  parameters.push_back(FilterParameter::New("Resolutions Data Array", "ResolutionsDataArrayPath", FilterParameterWidgetType::DataArraySelectionWidget, getResolutionsDataArrayPath(), false, ""));
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
  setResolutionsDataArrayPath(reader->readDataArrayPath("ResolutionsDataArrayPath", getResolutionsDataArrayPath()));
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
  DREAM3D_FILTER_WRITE_PARAMETER(FilterVersion)

  DREAM3D_FILTER_WRITE_PARAMETER(DREAM3DFileName)
      DREAM3D_FILTER_WRITE_PARAMETER(OriginsDataArrayPath)
       DREAM3D_FILTER_WRITE_PARAMETER(ResolutionsDataArrayPath)
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

  FloatArrayType::Pointer coordinates = getDataContainerArray()->getPrereqIDataArrayFromPath<FloatArrayType, AbstractFilter>(this, getOriginsDataArrayPath());
  StringDataArray::Pointer names = getDataContainerArray()->getPrereqIDataArrayFromPath<StringDataArray, AbstractFilter>(this, getDataArrayNamesPath());

  float origin[3] = { 0.0f, 0.0f, 0.0f };
  float resolution[3] = {1.0f, 1.0f, 1.0f };

  // Loop over each Array Name
  for(size_t i = 0; i < names->getNumberOfTuples(); i++)
  {

    // Note we need the
    origin[0] = coordinates->getComponent(i, 0);
    origin[1] = coordinates->getComponent(i, 1);
    origin[2] = 0.0f;
    // Need to get the Resolutions here from some array

    // Write the XML "Grid" tag to the file
    writeGrid(out, names->getValue(i), origin, resolution);

  }


  writeFooter(out);

  ofile.close();


  /* Let the GUI know we are done with this filter */
  notifyStatusMessage(getHumanLabel(), "Complete");
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void MontageXdmfWriter::writeGrid(QTextStream& out, const QString& arrayName, float *origin, float *resolution)
{
#if 0
<Grid Name="Zeiss Axio Vision Tile_p000" GridType="Uniform">
    <Topology TopologyType="3DCoRectMesh" Dimensions="2 969 1293 "></Topology>
    <Geometry Type="ORIGIN_DXDYDZ">
      <!-- Origin -->
      <DataItem Format="XML" Dimensions="3">0 0 0</DataItem>
      <!-- DxDyDz (Spacing/Resolution) -->
      <DataItem Format="XML" Dimensions="3">1 1 1</DataItem>
    </Geometry>
    <Attribute Name="ImageData" AttributeType="Vector" Center="Cell">
      <DataItem Format="HDF" Dimensions="1 968 1292 3" NumberType="UChar" Precision="1" >
        S200-1-C.dream3d:/DataContainers/Zeiss Axio Vision Tile_p000/CellData/ImageData
      </DataItem>
    </Attribute>
  </Grid>
  #endif
  AttributeMatrix::Pointer dataAm = getDataContainerArray()->getAttributeMatrix(getAttributeMatrixName());

  QVector<size_t> tDims = dataAm->getTupleDimensions();
  QString tDimsStr;
  for(qint32 d = tDims.size() - 1; d >= 0; d--)
  {
    tDimsStr = tDimsStr + QString::number(tDims[d] + 1) + " ";
  }
  QString indent("    ");

  out << indent << "<Grid Name=\""<< arrayName << "\" GridType=\"Uniform\">" << "\n";
  out << indent << "  <Topology TopologyType=\"3DCoRectMesh\" Dimensions=\""<< tDimsStr << "\"></Topology>" << "\n";

  writeGeometrySection(out, origin, resolution);

  DataArrayPath dap = getAttributeMatrixName();
  dap.setDataArrayName(arrayName);

  tDimsStr.clear();
  for(qint32 d = tDims.size() - 1; d >= 0; d--)
  {
    tDimsStr = tDimsStr + QString::number(tDims[d]) + " ";
  }

  out << indent << "  <Attribute Name=\"Montage\" AttributeType=\"Scalar\" Center=\"Cell\">" << "\n";
  out << indent << "    <DataItem Format=\"HDF\" Dimensions=\"" << tDimsStr << "\" NumberType=\"UChar\" Precision=\"1\" >" << "\n";
  out << indent << "      " << getDREAM3DFileName() << ":/DataContainers/" << dap.serialize("/") << "\n";
  out << indent << "    </DataItem>" << "\n";
  out << indent << "  </Attribute>" << "\n";
  out << indent << "</Grid>" << "\n\n";
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void MontageXdmfWriter::writeGeometrySection(QTextStream& out, float* origin, float* resolution)
{
  #if 0
  <Geometry Type="ORIGIN_DXDYDZ">
      <!-- Origin -->
      <DataItem Format="XML" Dimensions="3">0 0 0</DataItem>
      <!-- DxDyDz (Spacing/Resolution) -->
      <DataItem Format="XML" Dimensions="3">1 1 1</DataItem>
    </Geometry>
    #endif
      QString indent("      ");

    out << indent << "<Geometry Type=\"ORIGIN_DXDYDZ\">" << "\n";
    out << indent << "  <DataItem Format=\"XML\" Dimensions=\"3\">" << origin[2] << " " << origin[1] << " " << origin[0] << "</DataItem>  <!-- Origin Z-Y-X Order-->" << "\n";
    out << indent << "  <DataItem Format=\"XML\" Dimensions=\"3\">" << resolution[2] << " " << resolution[1] << " " << resolution[0] << "</DataItem>  <!-- Resolution Z-Y-X Order -->" << "\n";
    out << indent << "</Geometry>" << "\n";
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void MontageXdmfWriter::writeHeader(QTextStream& out)
{
  out << "<?xml version=\"1.0\"?>" << "\n";
  out << "<!DOCTYPE Xdmf SYSTEM \"Xdmf.dtd\"[]>" << "\n";
  out << "  <Xdmf xmlns:xi=\"http://www.w3.org/2003/XInclude\" Version=\"2.2\">" << "\n";
  out << "   <Domain>" << "\n";
  out << "    <Grid CollectionType=\"Spatial\" GridType=\"Collection\" Name=\"FieldData\">" << "\n";
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void MontageXdmfWriter::writeFooter(QTextStream& out)
{
  out << "   </Grid>  <!-- End Spatial Collection Type Grid -->" << "\n";
  out << "  </Domain>" << "\n";
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

