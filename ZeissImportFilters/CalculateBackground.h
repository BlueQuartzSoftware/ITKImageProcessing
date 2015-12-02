/*
 * Your License or Copyright Information can go here
 */

#ifndef _CalculateBackground_H_
#define _CalculateBackground_H_


#include <QtCore/QString>
#include <QtXml/QDomDocument>

#include "SIMPLib/SIMPLib.h"
#include "SIMPLib/Common/SIMPLibSetGetMacros.h"
#include "SIMPLib/Common/AbstractFilter.h"

#include "ZeissImport/ZeissXml/ZeissTagsXmlSection.h"

/**
 * @class CalculateBackground CalculateBackground.h ZeissImport/CalculateBackgrounds/CalculateBackground.h
 * @brief
 * @author
 * @date
 * @version 1.0
 */
class CalculateBackground : public AbstractFilter
{
  Q_OBJECT /* Need this for Qt's signals and slots mechanism to work */

  public:
    SIMPL_SHARED_POINTERS(CalculateBackground)
    SIMPL_STATIC_NEW_MACRO(CalculateBackground)
    SIMPL_TYPE_MACRO_SUPER(CalculateBackground, AbstractFilter)

    virtual ~CalculateBackground();

    SIMPL_INSTANCE_STRING_PROPERTY(DataContainerName)


    SIMPL_FILTER_PARAMETER(QString, VolumeDataContainerName)
    Q_PROPERTY(QString VolumeDataContainerName READ getVolumeDataContainerName WRITE setVolumeDataContainerName)

    SIMPL_FILTER_PARAMETER(QString, BackgroundAttributeMatrixName)
    Q_PROPERTY(QString BackgroundAttributeMatrixName READ getBackgroundAttributeMatrixName WRITE setBackgroundAttributeMatrixName)

    SIMPL_FILTER_PARAMETER(QString, CellAttributeMatrixName)
    Q_PROPERTY(QString CellAttributeMatrixName READ getCellAttributeMatrixName WRITE setCellAttributeMatrixName)

    SIMPL_FILTER_PARAMETER(DataArrayPath, ImageDataArrayPath)
    Q_PROPERTY(DataArrayPath ImageDataArrayPath READ getImageDataArrayPath WRITE setImageDataArrayPath)

    SIMPL_FILTER_PARAMETER(DataArrayPath, AttributeMatrixName)
    Q_PROPERTY(DataArrayPath AttributeMatrixName READ getAttributeMatrixName WRITE setAttributeMatrixName)

    SIMPL_FILTER_PARAMETER(QString, DataContainerBundleName)
    Q_PROPERTY(QString DataContainerBundleName READ getDataContainerBundleName WRITE setDataContainerBundleName)

    SIMPL_FILTER_PARAMETER(QString, BackgroundImageArrayName)
    Q_PROPERTY(QString BackgroundImageArrayName READ getBackgroundImageArrayName WRITE setBackgroundImageArrayName)

    SIMPL_FILTER_PARAMETER(uint, lowThresh)
    Q_PROPERTY(uint lowThresh READ getlowThresh WRITE setlowThresh)

    SIMPL_FILTER_PARAMETER(uint, highThresh)
    Q_PROPERTY(uint highThresh READ gethighThresh WRITE sethighThresh)

    SIMPL_FILTER_PARAMETER(bool, SubtractBackground)
    Q_PROPERTY(int SubtractBackground READ getSubtractBackground WRITE setSubtractBackground)

    SIMPL_FILTER_PARAMETER(bool, DivideBackground)
    Q_PROPERTY(int DivideBackground READ getDivideBackground WRITE setDivideBackground)

    /**
     * @brief getCompiledLibraryName Reimplemented from @see AbstractFilter class
     */
    virtual const QString getCompiledLibraryName();

    /**
     * @brief getBrandingString Returns the branding string for the filter, which is a tag
     * used to denote the filter's association with specific plugins
     * @return Branding string
    */
    virtual const QString getBrandingString();

    /**
     * @brief getFilterVersion Returns a version string for this filter. Default
     * value is an empty string.
     * @return
     */
    virtual const QString getFilterVersion();

    /**
     * @brief newFilterInstance Reimplemented from @see AbstractFilter class
     */
    virtual AbstractFilter::Pointer newFilterInstance(bool copyFilterParameters);

    /**
     * @brief getGroupName Reimplemented from @see AbstractFilter class
     */
    virtual const QString getGroupName();

    /**
     * @brief getSubGroupName Reimplemented from @see AbstractFilter class
     */
    virtual const QString getSubGroupName();

    /**
     * @brief getHumanLabel Reimplemented from @see AbstractFilter class
     */
    virtual const QString getHumanLabel();

    /**
     * @brief setupFilterParameters Reimplemented from @see AbstractFilter class
     */
    virtual void setupFilterParameters();

    /**
     * @brief writeFilterParameters Reimplemented from @see AbstractFilter class
     */
    virtual int writeFilterParameters(AbstractFilterParametersWriter* writer, int index);

    /**
     * @brief readFilterParameters Reimplemented from @see AbstractFilter class
     */
    virtual void readFilterParameters(AbstractFilterParametersReader* reader, int index);

    /**
     * @brief execute Reimplemented from @see AbstractFilter class
     */
    virtual void execute();

    /**
    * @brief preflight Reimplemented from @see AbstractFilter class
    */
    virtual void preflight();


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
    CalculateBackground();

    /**
    * @brief Checks for the appropriate parameter values and availability of arrays in the data container
    */
    void dataCheck();

    /**
     * @brief readMetaXml
     * @param device
     * @return
     */


    /**
     * @brief parseRootTagsSection
     * @param tags
     * @return
     */

    /**
     * @brief parseImages
     * @param rootTags
     */

  private:
    /* Your private class instance variables go here. You can use several preprocessor macros to help
     * make sure you have all the variables defined correctly. Those are "DEFINE_DATAARRAY_VARIABLE()"
     * and  DEFINE_DATAARRAY_VARIABLE() which are defined in DREAM3DGetSetMacros.h
     */

    int64_t m_totalPoints;
//    uint64_t m_lowThresh;
//    uint64_t m_highThresh;

    DEFINE_DATAARRAY_VARIABLE(double, BackgroundImage)

    CalculateBackground(const CalculateBackground&); // Copy Constructor Not Implemented
    void operator=(const CalculateBackground&); // Operator '=' Not Implemented
};

#endif /* _CalculateBackground_H_ */
