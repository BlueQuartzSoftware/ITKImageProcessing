/*
 * Your License or Copyright Information can go here
 */

#pragma once

#include "SIMPLib/Common/SIMPLibSetGetMacros.h"
#include "SIMPLib/DataArrays/StringDataArray.h"
#include "SIMPLib/Filtering/AbstractFilter.h"
#include "SIMPLib/SIMPLib.h"

#include "ITKImageProcessing/ITKImageProcessingConstants.h"
#include "ITKImageProcessing/ITKImageProcessingPlugin.h"

/**
 * @class DetermineStitchingCoordinatesGeneric DetermineStitchingCoordinatesGeneric.h ZeissImport/ZeissImportFilters/DetermineStitchingCoordinatesGeneric.h
 * @brief
 * @author
 * @date
 * @version 1.0
 */
class ITKImageProcessing_EXPORT ITKRefineTileCoordinates : public AbstractFilter
{
  Q_OBJECT
  PYB11_CREATE_BINDINGS(ITKRefineTileCoordinates SUPERCLASS AbstractFilter)

  PYB11_PROPERTY(QStringList DataContainers READ getDataContainers WRITE setDataContainers)
  PYB11_PROPERTY(QString CellAttributeMatrixName READ getCellAttributeMatrixName WRITE setCellAttributeMatrixName)
  PYB11_PROPERTY(QString ImageDataArrayName READ getImageDataArrayName WRITE setImageDataArrayName)

  PYB11_PROPERTY(int ImportMode READ getImportMode WRITE setImportMode)
  PYB11_PROPERTY(int XTileDim READ getXTileDim WRITE setXTileDim)
  PYB11_PROPERTY(int YTileDim READ getYTileDim WRITE setYTileDim)
  PYB11_PROPERTY(float OverlapPer READ getOverlapPer WRITE setOverlapPer)
  PYB11_PROPERTY(bool ApplyRefinedOrigin READ getApplyRefinedOrigin WRITE setApplyRefinedOrigin)

public:
  SIMPL_SHARED_POINTERS(ITKRefineTileCoordinates)
  SIMPL_FILTER_NEW_MACRO(ITKRefineTileCoordinates)
  SIMPL_TYPE_MACRO_SUPER_OVERRIDE(ITKRefineTileCoordinates, AbstractFilter)

  ~ITKRefineTileCoordinates() override;

  SIMPL_FILTER_PARAMETER(QStringList, DataContainers)
  Q_PROPERTY(QStringList DataContainers READ getDataContainers WRITE setDataContainers)

  SIMPL_FILTER_PARAMETER(QString, CellAttributeMatrixName)
  Q_PROPERTY(QString CellAttributeMatrixName READ getCellAttributeMatrixName WRITE setCellAttributeMatrixName)

  SIMPL_FILTER_PARAMETER(QString, ImageDataArrayName)
  Q_PROPERTY(QString ImageDataArrayName READ getImageDataArrayName WRITE setImageDataArrayName)

  SIMPL_FILTER_PARAMETER(int, ImportMode)
  Q_PROPERTY(int ImportMode READ getImportMode WRITE setImportMode)

  SIMPL_FILTER_PARAMETER(int, XTileDim)
  Q_PROPERTY(int XTileDim READ getXTileDim WRITE setXTileDim)

  SIMPL_FILTER_PARAMETER(int, YTileDim)
  Q_PROPERTY(int YTileDim READ getYTileDim WRITE setYTileDim)

  SIMPL_FILTER_PARAMETER(float, OverlapPer)
  Q_PROPERTY(float OverlapPer READ getOverlapPer WRITE setOverlapPer)

  SIMPL_FILTER_PARAMETER(bool, ApplyRefinedOrigin)
  Q_PROPERTY(bool ApplyRefinedOrigin READ getApplyRefinedOrigin WRITE setApplyRefinedOrigin)

  //  SIMPL_FILTER_PARAMETER(bool, UseZeissMetaData)
  //  Q_PROPERTY(bool UseZeissMetaData READ getUseZeissMetaData WRITE setUseZeissMetaData)

  //  SIMPL_FILTER_PARAMETER(DataArrayPath, MetaDataAttributeMatrixName)
  //  Q_PROPERTY(DataArrayPath MetaDataAttributeMatrixName READ getMetaDataAttributeMatrixName WRITE setMetaDataAttributeMatrixName)

  //  SIMPL_FILTER_PARAMETER(QString, TileCalculatedInfoAttributeMatrixName)
  //  Q_PROPERTY(QString TileCalculatedInfoAttributeMatrixName READ getTileCalculatedInfoAttributeMatrixName WRITE setTileCalculatedInfoAttributeMatrixName)

  //  SIMPL_FILTER_PARAMETER(QString, StitchedCoordinatesArrayName)
  //  Q_PROPERTY(QString StitchedCoordinatesArrayName READ getStitchedCoordinatesArrayName WRITE setStitchedCoordinatesArrayName)

  //  SIMPL_FILTER_PARAMETER(QString, StitchedArrayNames)
  //  Q_PROPERTY(QString StitchedArrayNames READ getStitchedArrayNames WRITE setStitchedArrayNames)

  /**
   * @brief getCompiledLibraryName Returns the name of the Library that this filter is a part of
   * @return
   */
  const QString getCompiledLibraryName() const override;

  /**
   * @brief This returns a string that is displayed in the GUI. It should be readable
   * and understandable by humans.
   */
  const QString getHumanLabel() const override;

  /**
   * @brief This returns the group that the filter belonds to. You can select
   * a different group if you want. The string returned here will be displayed
   * in the GUI for the filter
   */
  const QString getGroupName() const override;

  /**
   * @brief This returns a string that is displayed in the GUI and helps to sort the filters into
   * a subgroup. It should be readable and understandable by humans.
   */
  const QString getSubGroupName() const override;

  /**
   * @brief getUuid Return the unique identifier for this filter.
   * @return A QUuid object.
   */
  const QUuid getUuid() override;

  /**
   * @brief This method will instantiate all the end user settable options/parameters
   * for this filter
   */
  void setupFilterParameters() override;

  /**
   * @brief Reimplemented from @see AbstractFilter class
   */
  void execute() override;

  /**
   * @brief This function runs some sanity checks on the DataContainer and inputs
   * in an attempt to ensure the filter can process the inputs.
   */
  void preflight() override;

  /**
   * @brief newFilterInstance Returns a new instance of the filter optionally copying the filter parameters from the
   * current filter to the new instance.
   * @param copyFilterParameters
   * @return
   */
  AbstractFilter::Pointer newFilterInstance(bool copyFilterParameters) const override;

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
  enum class ArrayType
  {
    Error = -1,
    UInt8,
    UInt16,
    Float32
  };
  enum class GeomType
  {
    Error = -1,
    ImageGeom,
    RectGridGeom
  };

  /**
   * @brief ITKRefineTileCoordinates
   */
  ITKRefineTileCoordinates();

  /**
   * @brief dataCheck Checks for the appropriate parameter values and availability of arrays
   */
  void dataCheck();

  /**
   * @brief Initializes all the private instance variables.
   */
  void initialize();

  /**
   * @brief Returns an enum value specifying which supported array type the incoming data falls into.
   * @return
   */
  ArrayType getArrayType();

  /**
   * @brief Returns an enum value specifying which supported geometry type the incoming data falls into.
   * @return
   */
  GeomType getGeomType();

private:
  std::vector<ITKImageProcessingConstants::DefaultPixelType*> m_PointerList;
  DEFINE_DATAARRAY_VARIABLE(ITKImageProcessingConstants::DefaultPixelType, SelectedCellArray)
  DEFINE_DATAARRAY_VARIABLE(float, StitchedCoordinates)
  StringDataArray::WeakPointer m_DataArrayNamesForStitchedCoordinatesPtr;

public:
  ITKRefineTileCoordinates(const ITKRefineTileCoordinates&) = delete; // Copy Constructor Not Implemented
  ITKRefineTileCoordinates(ITKRefineTileCoordinates&&) = delete;      // Move Constructor Not Implemented
  ITKRefineTileCoordinates& operator=(const ITKRefineTileCoordinates&) = delete; // Copy Assignment Not Implemented
  ITKRefineTileCoordinates& operator=(ITKRefineTileCoordinates&&) = delete;      // Move Assignment Not Implemented
};

