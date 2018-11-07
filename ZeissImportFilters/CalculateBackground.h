/*
 * Your License or Copyright Information can go here
 */

#pragma once


#include <QtCore/QString>
#include <QtXml/QDomDocument>

#include "SIMPLib/Common/SIMPLibSetGetMacros.h"
#include "SIMPLib/Filtering/AbstractFilter.h"
#include "SIMPLib/SIMPLib.h"

#include "ZeissImport/ZeissXml/ZeissTagsXmlSection.h"

#include "ZeissImport/ZeissImportDLLExport.h"

/**
 * @class CalculateBackground CalculateBackground.h ZeissImport/CalculateBackgrounds/CalculateBackground.h
 * @brief
 * @author
 * @date
 * @version 1.0
 */
class ZeissImport_EXPORT CalculateBackground : public AbstractFilter
{
  Q_OBJECT
  PYB11_CREATE_BINDINGS(CalculateBackground SUPERCLASS AbstractFilter)
  PYB11_PROPERTY(QString VolumeDataContainerName READ getVolumeDataContainerName WRITE setVolumeDataContainerName)
  PYB11_PROPERTY(QString BackgroundAttributeMatrixName READ getBackgroundAttributeMatrixName WRITE setBackgroundAttributeMatrixName)
  PYB11_PROPERTY(QString CellAttributeMatrixName READ getCellAttributeMatrixName WRITE setCellAttributeMatrixName)
  PYB11_PROPERTY(DataArrayPath ImageDataArrayPath READ getImageDataArrayPath WRITE setImageDataArrayPath)
  PYB11_PROPERTY(DataArrayPath AttributeMatrixName READ getAttributeMatrixName WRITE setAttributeMatrixName)
  PYB11_PROPERTY(QString DataContainerBundleName READ getDataContainerBundleName WRITE setDataContainerBundleName)
  PYB11_PROPERTY(QString BackgroundImageArrayName READ getBackgroundImageArrayName WRITE setBackgroundImageArrayName)
  PYB11_PROPERTY(uint lowThresh READ getlowThresh WRITE setlowThresh)
  PYB11_PROPERTY(uint highThresh READ gethighThresh WRITE sethighThresh)
  PYB11_PROPERTY(bool SubtractBackground READ getSubtractBackground WRITE setSubtractBackground)
  PYB11_PROPERTY(bool DivideBackground READ getDivideBackground WRITE setDivideBackground)

public:
  SIMPL_SHARED_POINTERS(CalculateBackground)
  SIMPL_FILTER_NEW_MACRO(CalculateBackground)
  SIMPL_TYPE_MACRO_SUPER_OVERRIDE(CalculateBackground, AbstractFilter)

  ~CalculateBackground() override;

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
  const QString getCompiledLibraryName() const override;

  /**
   * @brief getBrandingString Returns the branding string for the filter, which is a tag
   * used to denote the filter's association with specific plugins
   * @return Branding string
   */
  const QString getBrandingString() const override;

  /**
   * @brief getFilterVersion Returns a version string for this filter. Default
   * value is an empty string.
   * @return
   */
  const QString getFilterVersion() const override;

  /**
   * @brief newFilterInstance Reimplemented from @see AbstractFilter class
   */
  AbstractFilter::Pointer newFilterInstance(bool copyFilterParameters) const override;

  /**
   * @brief getGroupName Reimplemented from @see AbstractFilter class
   */
  const QString getGroupName() const override;

  /**
   * @brief getSubGroupName Reimplemented from @see AbstractFilter class
   */
  const QString getSubGroupName() const override;

  /**
   * @brief getUuid Return the unique identifier for this filter.
   * @return A QUuid object.
   */
  const QUuid getUuid() override;

  /**
   * @brief getHumanLabel Reimplemented from @see AbstractFilter class
   */
  const QString getHumanLabel() const override;

  /**
   * @brief setupFilterParameters Reimplemented from @see AbstractFilter class
   */
  void setupFilterParameters() override;

  /**
   * @brief readFilterParameters Reimplemented from @see AbstractFilter class
   */
  void readFilterParameters(AbstractFilterParametersReader* reader, int index);

  /**
   * @brief execute Reimplemented from @see AbstractFilter class
   */
  void execute() override;

  /**
   * @brief preflight Reimplemented from @see AbstractFilter class
   */
  void preflight() override;

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
   * @brief dataCheck Checks for the appropriate parameter values and availability of arrays
   */
  void dataCheck();

  /**
   * @brief Initializes all the private instance variables.
   */
  void initialize();

private:
  int64_t m_TotalPoints;

  DEFINE_DATAARRAY_VARIABLE(double, BackgroundImage)

public:
  CalculateBackground(const CalculateBackground&) = delete;            // Copy Constructor Not Implemented
  CalculateBackground(CalculateBackground&&) = delete;                 // Move Constructor Not Implemented
  CalculateBackground& operator=(const CalculateBackground&) = delete; // Copy Assignment Not Implemented
  CalculateBackground& operator=(CalculateBackground&&) = delete;      // Move Assignment Not Implemented
};

