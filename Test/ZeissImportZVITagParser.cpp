
#include <QtCore/QFile>
#include <QtCore/QString>
#include <QtCore/QTextStream>

#include "ITKImageProcessing/ITKImageProcessingConstants.h"
#include "ITKImageProcessing/Test/TestFileLocations.h"

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
QString getZVITagsFile()
{
  return ITKImageProcessingConstants::Tools::TagsTextFile;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
QString cleanupValue(const QString val)
{
  QString value = val;
  value = value.replace(" ", "");
  value = value.replace("(", "");
  value = value.replace(")", "");
  value = value.replace("!", "");
  value = value.replace("-", "_");
  value = value.replace(".", "_");
  value = value.replace("[", "_");
  value = value.replace("]", "_");
  if(value.startsWith("0"))
  {
    value = value.replace("0", "Zero");
  }
  if(value.startsWith("1"))
  {
    value = value.replace("1", "One");
  }
  if(value.startsWith("2"))
  {
    value = value.replace("2", "Two");
  }
  if(value.startsWith("3"))
  {
    value = value.replace("3", "Three");
  }
  if(value.startsWith("4"))
  {
    value = value.replace("4", "Four");
  }
  if(value.startsWith("5"))
  {
    value = value.replace("5", "Five");
  }
  if(value.startsWith("6"))
  {
    value = value.replace("6", "Six");
  }
  if(value.startsWith("7"))
  {
    value = value.replace("7", "Seven");
  }
  if(value.startsWith("8"))
  {
    value = value.replace("8", "Eight");
  }
  if(value.startsWith("9"))
  {
    value = value.replace("9", "Nine");
  }
  return value;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
QStringList GenerateZeissMetaXMLConstants()
{
  QString contents;

  {
    // Read the Source File
    QFileInfo fi(getZVITagsFile());
    QFile source(getZVITagsFile());
    source.open(QFile::ReadOnly);
    contents = source.readAll();
    source.close();
  }
  QStringList outLines;
  QStringList strLines;
  QStringList list = contents.split(QRegExp("\\n"));
  QStringListIterator sourceLines(list);

  while(sourceLines.hasNext())
  {
    QString line = sourceLines.next();

    int spaceIdx = line.indexOf(' ', 0);
    QString tagId = line.left(spaceIdx);
    QString value = line.mid(spaceIdx + 1);
    value = cleanupValue(value);

    QString out = QString("    static const int %1Id = %2;\n").arg(value).arg(tagId);

    outLines.append(out);

    out = QString("    const QString %1 (\"%1\"); //Gets Mapped to Id %2\n").arg(value).arg(tagId);
    strLines.append(out);
  }

  outLines.append("\n");

  return outLines + strLines;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void makeHeaderFile(const QString filename)
{
  QFileInfo fi2(filename);

  QFile hOut(filename);

  hOut.open(QFile::WriteOnly);

  QTextStream stream(&hOut);

  QStringList outLines = GenerateZeissMetaXMLConstants();

  stream << "/** THIS FILE WAS AUTO-GENERATED FROM THE ZVI_TAGS.TXT FILE WHICH WAS MANUALLY GENERATED FROM\n";
  stream << "* THE ZVI DOCUMENTATION FILE THAT IS DOWNLOADED FROM CARL ZEISS UNDER LICENSE.\n";
  stream << "*/\n\n";
  stream << "#ifndef _ZEISSTAGMAPPINGConstants_H_"
         << "\n";
  stream << "#define _ZEISSTAGMAPPINGConstants_H_"
         << "\n";
  stream << "\n";
  stream << "#include <QtCore/QString>\n\n";
  stream << "namespace Zeiss {\n  namespace MetaXML { \n";

  stream << outLines.join("");

  stream << "  }\n}\n";
  stream << "#endif\n";

  hOut.close();
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
QStringList GenerateZeissInitIdNameMap()
{
  QString contents;
  {
    // Read the Source File
    QFileInfo fi(getZVITagsFile());
    QFile source(getZVITagsFile());
    source.open(QFile::ReadOnly);
    contents = source.readAll();
    source.close();
  }
  QStringList outLines;
  QStringList strLines;
  QStringList list = contents.split(QRegExp("\\n"));
  QStringListIterator sourceLines(list);

  while(sourceLines.hasNext())
  {
    QString line = sourceLines.next();

    int spaceIdx = line.indexOf(' ', 0);
    QString tagId = line.left(spaceIdx);
    QString value = line.mid(spaceIdx + 1);
    value = cleanupValue(value);

    QString out = QString("  _idNameMap[Zeiss::MetaXML::%1Id] = Zeiss::MetaXML::%1;\n").arg(value);
    outLines.append(out);
  }

  outLines.append("\n");

  return outLines + strLines;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
QStringList GenerateZeissInitNameIdMap()
{
  QString contents;
  {
    // Read the Source File
    QFileInfo fi(getZVITagsFile());
    QFile source(getZVITagsFile());
    source.open(QFile::ReadOnly);
    contents = source.readAll();
    source.close();
  }
  QStringList outLines;
  QStringList strLines;
  QStringList list = contents.split(QRegExp("\\n"));
  QStringListIterator sourceLines(list);

  while(sourceLines.hasNext())
  {
    QString line = sourceLines.next();

    int spaceIdx = line.indexOf(' ', 0);
    QString tagId = line.left(spaceIdx);
    QString value = line.mid(spaceIdx + 1);
    value = cleanupValue(value);

    QString out = QString("  _nameIdMap[Zeiss::MetaXML::%1] = Zeiss::MetaXML::%1Id;\n").arg(value);
    outLines.append(out);
  }

  outLines.append("\n");

  return outLines + strLines;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
QStringList GenerateZeissInitFactoryMap()
{
  QString contents;
  {
    // Read the Source File
    QFileInfo fi(getZVITagsFile());
    QFile source(getZVITagsFile());
    source.open(QFile::ReadOnly);
    contents = source.readAll();
    source.close();
  }
  QStringList outLines;
  QStringList strLines;
  QStringList list = contents.split(QRegExp("\\n"));
  QStringListIterator sourceLines(list);

  while(sourceLines.hasNext())
  {
    QString line = sourceLines.next();

    int spaceIdx = line.indexOf(' ', 0);
    QString tagId = line.left(spaceIdx);
    QString value = line.mid(spaceIdx + 1);
    value = cleanupValue(value);

    QString out = QString("  _idFactoryMap[Zeiss::MetaXML::%1Id] = StringZeissMetaFactory::NewZeissMetaFactory();\n").arg(value);
    outLines.append(out);
  }

  outLines.append("\n");

  return outLines + strLines;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void makeCPPFile(const QString filename)
{
  QFileInfo fi2(filename);

  QFile hOut(filename);

  hOut.open(QFile::WriteOnly);

  QTextStream stream(&hOut);

  QStringList outLines = GenerateZeissInitIdNameMap();

  stream << "/** THIS FILE WAS AUTO-GENERATED FROM THE ZVI_TAGS.TXT FILE WHICH WAS MANUALLY GENERATED FROM\n";
  stream << "* THE ZVI DOCUMENTATION FILE THAT IS DOWNLOADED FROM CARL ZEISS UNDER LICENSE.\n";
  stream << "*/\n\n";
  stream << "// -----------------------------------------------------------------------------\n";
  stream << "//\n";
  stream << "// -----------------------------------------------------------------------------\n";
  stream << "void ZeissTagMapping::initIdNameMap()\n{\n";
  stream << outLines.join("");
  stream << "}\n\n";

  outLines = GenerateZeissInitNameIdMap();
  stream << "// -----------------------------------------------------------------------------\n";
  stream << "//\n";
  stream << "// -----------------------------------------------------------------------------\n";
  stream << "void ZeissTagMapping::initNameIdMap()\n{\n";
  stream << outLines.join("");
  stream << "}\n\n";

  outLines = GenerateZeissInitFactoryMap();
  stream << "// -----------------------------------------------------------------------------\n";
  stream << "//\n";
  stream << "// -----------------------------------------------------------------------------\n";
  stream << "void ZeissTagMapping::initFactoryMap()\n{\n";
  stream << outLines.join("");
  stream << "}\n\n";

  hOut.close();
}

int main(int argc, char* argv[])
{

  makeHeaderFile(ITKImageProcessingConstants::Tools::ZeissTagMappingConstantsFile);

  makeCPPFile(ITKImageProcessingConstants::Tools::ZeissInitIdNameMapFile);

  return 0;
}
