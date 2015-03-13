/*
 * Your License or Copyright Information can go here
 */

#ifndef _ZeissImportFilter_H_
#define _ZeissImportFilter_H_


#include <QtCore/QString>
#include <QtCore/QDateTime>
#include <QtXml/QDomDocument>

#include "DREAM3DLib/DREAM3DLib.h"
#include "DREAM3DLib/Common/DREAM3DSetGetMacros.h"
#include "DREAM3DLib/Common/AbstractFilter.h"

#include "ZeissImport/ZeissXml/ZeissTagsXmlSection.h"

// our PIMPL private class
class ZeissImportFilterPrivate;

/**
 * @class ZeissImportFilter ZeissImportFilter.h ZeissImport/ZeissImportFilters/ZeissImportFilter.h
 * @brief
 * @author
 * @date
 * @version 1.0
 */
class ZeissImportFilter : public AbstractFilter
{
  Q_OBJECT /* Need this for Qt's signals and slots mechanism to work */
	  Q_DECLARE_PRIVATE(ZeissImportFilter)

  public:
    DREAM3D_SHARED_POINTERS(ZeissImportFilter)
    DREAM3D_STATIC_NEW_MACRO(ZeissImportFilter)
    DREAM3D_TYPE_MACRO_SUPER(ZeissImportFilter, AbstractFilter)

    virtual ~ZeissImportFilter();

//    DREAM3D_FILTER_PARAMETER(QString, CellAttributeMatrixName)
//    Q_PROPERTY(QString CellAttributeMatrixName READ getCellAttributeMatrixName WRITE setCellAttributeMatrixName)

    DREAM3D_FILTER_PARAMETER(QString, InputFile)
    Q_PROPERTY(QString InputFile READ getInputFile WRITE setInputFile)

    DREAM3D_FILTER_PARAMETER(QString, DataContainerName)
    Q_PROPERTY(QString DataContainerName READ getDataContainerName WRITE setDataContainerName)

    DREAM3D_FILTER_PARAMETER(QString, ImageAttributeMatrixName)
    Q_PROPERTY(QString ImageAttributeMatrixName READ getImageAttributeMatrixName WRITE setImageAttributeMatrixName)

    DREAM3D_FILTER_PARAMETER(QString, ImageDataArrayPrefix)
    Q_PROPERTY(QString ImageDataArrayPrefix READ getImageDataArrayPrefix WRITE setImageDataArrayPrefix)

    DREAM3D_FILTER_PARAMETER(bool, ConvertToGrayScale)
    Q_PROPERTY(bool ConvertToGrayScale READ getConvertToGrayScale WRITE setConvertToGrayScale)

    DREAM3D_FILTER_PARAMETER(FloatVec3_t, ColorWeights)
    Q_PROPERTY(FloatVec3_t ColorWeights READ getColorWeights WRITE setColorWeights)

	DREAM3D_FILTER_PARAMETER(bool, FileWasRead)
	Q_PROPERTY(bool FileWasRead READ getFileWasRead)

    /**
     * @brief getCompiledLibraryName Returns the name of the Library that this filter is a part of
     * @return
     */
    virtual const QString getCompiledLibraryName();

    /**
    * @brief This returns a string that is displayed in the GUI. It should be readable
    * and understandable by humans.
    */
    virtual const QString getHumanLabel();

    /**
    * @brief This returns the group that the filter belonds to. You can select
    * a different group if you want. The string returned here will be displayed
    * in the GUI for the filter
    */
    virtual const QString getGroupName();

    /**
    * @brief This returns a string that is displayed in the GUI and helps to sort the filters into
    * a subgroup. It should be readable and understandable by humans.
    */
    virtual const QString getSubGroupName();

    /**
    * @brief This method will instantiate all the end user settable options/parameters
    * for this filter
    */
    virtual void setupFilterParameters();

    /**
    * @brief This method will write the options to a file
    * @param writer The writer that is used to write the options to a file
    * @param index The index that the data should be written to
    * @return Returns the next index for writing
    */
    virtual int writeFilterParameters(AbstractFilterParametersWriter* writer, int index);

    /**
    * @brief This method will read the options from a file
    * @param reader The reader that is used to read the options from a file
    * @param index The index to read the information from
    */
    virtual void readFilterParameters(AbstractFilterParametersReader* reader, int index);

   /**
    * @brief Reimplemented from @see AbstractFilter class
    */
    virtual void execute();

    /**
    * @brief This function runs some sanity checks on the DataContainer and inputs
    * in an attempt to ensure the filter can process the inputs.
    */
    virtual void preflight();

    /**
     * @brief newFilterInstance Returns a new instance of the filter optionally copying the filter parameters from the
     * current filter to the new instance.
     * @param copyFilterParameters
     * @return
     */
    virtual AbstractFilter::Pointer newFilterInstance(bool copyFilterParameters);

	DREAM3D_PIMPL_PROPERTY_DECL(QDomElement, Root)
	DREAM3D_PIMPL_PROPERTY_DECL(ZeissTagsXmlSection::Pointer, RootTagsSection)
	DREAM3D_PIMPL_PROPERTY_DECL(QString, InputFile_Cache)
	DREAM3D_PIMPL_PROPERTY_DECL(QDateTime, LastRead)

  signals:
    /**
     * @brief updateFilterParameters This is emitted when the filter requests all the latest Filter Parameters need to be
     * pushed from a user facing control such as the FilterParameter Widget
     * @param filter The filter to push the values into
     */
    void updateFilterParameters(AbstractFilter* filter);

    /**
     * @brief parametersChanged This signal can be emitted when any of the filter parameters are changed internally.
     */
    void parametersChanged();

    /**
     * @brief preflightAboutToExecute Emitted just before the dataCheck() is called. This can change if needed.
     */
    void preflightAboutToExecute();

    /**
     * @brief preflightExecuted Emitted just after the dataCheck() is called. Typically. This can change if needed.
     */
    void preflightExecuted();

  protected:
    ZeissImportFilter();

    /**
    * @brief Checks for the appropriate parameter values and availability of arrays in the data container
    */
    void dataCheck();

    /**
     * @brief readMetaXml
     * @param device
     * @return
     */
    int readMetaXml(QIODevice* device);

    /**
     * @brief parseRootTagsSection
     * @param tags
     * @return
     */
    ZeissTagsXmlSection::Pointer parseTagsSection(QDomElement& tags);

    /**
     * @brief parseImages
     * @param rootTags
     */
    void parseImages(QDomElement& root, ZeissTagsXmlSection::Pointer rootTagsSection);

    /**
     * @brief getImageDimensions
     * @param photoTagsSection
     * @return
     */
    QVector<size_t> getImageDimensions(ZeissTagsXmlSection::Pointer photoTagsSection);

    /**
     * @brief generateMetaDataAttributeMatrix
     * @param photoTagsSection
     * @param imageCount
     * @return
     */
    void addMetaData(AttributeMatrix::Pointer metaAm, ZeissTagsXmlSection::Pointer photoTagsSection, int index);

    /**
     * @brief generateMetaDataAttributeMatrix -root data
     * @param rootTagsSection
     * @param imageCount
     * @return
     */

    void addRootMetaData(AttributeMatrix::Pointer metaAm, ZeissTagsXmlSection::Pointer rootTagsSection, int index);

    /**
     * @brief generateDataArrays
     * @param imageName
     * @param pTag
     * @param dcName
     */
    void importImage(const QString &imageName, const QString &pTag, const QString &dcName);

    /**
     * @brief convertToGrayScale
     * @param imageName
     * @param pTag
     * @param dcName
     */
    void convertToGrayScale(const QString &imageName, const QString &pTag, const QString &dcName);


  private:
	  QScopedPointer<ZeissImportFilterPrivate> const d_ptr;

    /* Your private class instance variables go here. You can use several preprocessor macros to help
     * make sure you have all the variables defined correctly. Those are "DEFINE_REQUIRED_DATAARRAY_VARIABLE()"
     * and  DEFINE_CREATED_DATAARRAY_VARIABLE() which are defined in DREAM3DGetSetMacros.h
     */

    ZeissImportFilter(const ZeissImportFilter&); // Copy Constructor Not Implemented
    void operator=(const ZeissImportFilter&); // Operator '=' Not Implemented
};

#endif /* _ZeissImportFilter_H_ */
