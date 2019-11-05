/*
 * Your License or Copyright Information can go here
 */

#pragma once

#include <memory>

#include "SIMPLib/SIMPLib.h"
#include "SIMPLib/DataArrays/StringDataArray.h"
#include "SIMPLib/Common/SIMPLArray.hpp"
#include "SIMPLib/Filtering/AbstractFilter.h"

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

#ifdef SIMPL_ENABLE_PYTHON
  PYB11_CREATE_BINDINGS(ITKRefineTileCoordinates SUPERCLASS AbstractFilter)
  PYB11_SHARED_POINTERS(ITKRefineTileCoordinates)
  PYB11_FILTER_NEW_MACRO(ITKRefineTileCoordinates)
  PYB11_FILTER_PARAMETER(IntVec3Type, MontageSize)
  PYB11_FILTER_PARAMETER(QStringList, DataContainers)
  PYB11_FILTER_PARAMETER(QString, CommonAttributeMatrixName)
  PYB11_FILTER_PARAMETER(QString, CommonDataArrayName)
  PYB11_FILTER_PARAMETER(int, ImportMode)
  PYB11_FILTER_PARAMETER(float, TileOverlap)
  PYB11_FILTER_PARAMETER(bool, ApplyRefinedOrigin)
  PYB11_PROPERTY(IntVec3Type MontageSize READ getMontageSize WRITE setMontageSize)
  PYB11_PROPERTY(QStringList DataContainers READ getDataContainers WRITE setDataContainers)
  PYB11_PROPERTY(QString CommonAttributeMatrixName READ getCommonAttributeMatrixName WRITE setCommonAttributeMatrixName)
  PYB11_PROPERTY(QString CommonDataArrayName READ getCommonDataArrayName WRITE setCommonDataArrayName)
  PYB11_PROPERTY(int ImportMode READ getImportMode WRITE setImportMode)
  PYB11_PROPERTY(float TileOverlap READ getTileOverlap WRITE setTileOverlap)
  PYB11_PROPERTY(bool ApplyRefinedOrigin READ getApplyRefinedOrigin WRITE setApplyRefinedOrigin)
#endif

public:
    using Self = ITKRefineTileCoordinates;
    using Pointer = std::shared_ptr<Self>;
    using ConstPointer = std::shared_ptr<const Self>;
    using WeakPointer = std::weak_ptr<Self>;
    using ConstWeakPointer = std::weak_ptr<Self>;
    static Pointer NullPointer();

    static std::shared_ptr<ITKRefineTileCoordinates> New();

    /**
    * @brief Returns the name of the class for ITKRefineTileCoordinates
    */
    QString getNameOfClass() const override;
    /**
    * @brief Returns the name of the class for ITKRefineTileCoordinates
    */
    static QString ClassName();


  ~ITKRefineTileCoordinates() override;

    /**
    * @brief Setter property for MontageSize
    */
    void setMontageSize(const IntVec3Type& value); 
    /**
    * @brief Getter property for MontageSize
    * @return Value of MontageSize
    */
    IntVec3Type getMontageSize() const;

  Q_PROPERTY(IntVec3Type MontageSize READ getMontageSize WRITE setMontageSize)

    /**
    * @brief Setter property for DataContainers
    */
    void setDataContainers(const QStringList& value); 
    /**
    * @brief Getter property for DataContainers
    * @return Value of DataContainers
    */
    QStringList getDataContainers() const;

  Q_PROPERTY(QStringList DataContainers READ getDataContainers WRITE setDataContainers)

    /**
    * @brief Setter property for CommonAttributeMatrixName
    */
    void setCommonAttributeMatrixName(const QString& value); 
    /**
    * @brief Getter property for CommonAttributeMatrixName
    * @return Value of CommonAttributeMatrixName
    */
    QString getCommonAttributeMatrixName() const;

  Q_PROPERTY(QString CommonAttributeMatrixName READ getCommonAttributeMatrixName WRITE setCommonAttributeMatrixName)

    /**
    * @brief Setter property for CommonDataArrayName
    */
    void setCommonDataArrayName(const QString& value); 
    /**
    * @brief Getter property for CommonDataArrayName
    * @return Value of CommonDataArrayName
    */
    QString getCommonDataArrayName() const;

  Q_PROPERTY(QString CommonDataArrayName READ getCommonDataArrayName WRITE setCommonDataArrayName)

    /**
    * @brief Setter property for ImportMode
    */
    void setImportMode(int value); 
    /**
    * @brief Getter property for ImportMode
    * @return Value of ImportMode
    */
    int getImportMode() const;

  Q_PROPERTY(int ImportMode READ getImportMode WRITE setImportMode)

    /**
    * @brief Setter property for TileOverlap
    */
    void setTileOverlap(float value); 
    /**
    * @brief Getter property for TileOverlap
    * @return Value of TileOverlap
    */
    float getTileOverlap() const;

  Q_PROPERTY(float TileOverlap READ getTileOverlap WRITE setTileOverlap)

    /**
    * @brief Setter property for ApplyRefinedOrigin
    */
    void setApplyRefinedOrigin(bool value); 
    /**
    * @brief Getter property for ApplyRefinedOrigin
    * @return Value of ApplyRefinedOrigin
    */
    bool getApplyRefinedOrigin() const;

  Q_PROPERTY(bool ApplyRefinedOrigin READ getApplyRefinedOrigin WRITE setApplyRefinedOrigin)

  /**
   * @brief getCompiledLibraryName Returns the name of the Library that this filter is a part of
   * @return
   */
  QString getCompiledLibraryName() const override;

  /**
   * @brief This returns a string that is displayed in the GUI. It should be readable
   * and understandable by humans.
   */
  QString getHumanLabel() const override;

  /**
   * @brief This returns the group that the filter belonds to. You can select
   * a different group if you want. The string returned here will be displayed
   * in the GUI for the filter
   */
  QString getGroupName() const override;

  /**
   * @brief This returns a string that is displayed in the GUI and helps to sort the filters into
   * a subgroup. It should be readable and understandable by humans.
   */
  QString getSubGroupName() const override;

  /**
   * @brief getUuid Return the unique identifier for this filter.
   * @return A QUuid object.
   */
  QUuid getUuid() const override;

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
    IntVec3Type m_MontageSize = {};
    QStringList m_DataContainers = {};
    QString m_CommonAttributeMatrixName = {};
    QString m_CommonDataArrayName = {};
    int m_ImportMode = {};
    float m_TileOverlap = {};
    bool m_ApplyRefinedOrigin = {};
    std::weak_ptr<DataArray<ITKImageProcessingConstants::DefaultPixelType>>  m_SelectedCellArrayPtr;
    ITKImageProcessingConstants::DefaultPixelType* m_SelectedCellArray = nullptr;
    std::weak_ptr<DataArray<float>>  m_StitchedCoordinatesPtr;
    float* m_StitchedCoordinates = nullptr;

  std::vector<ITKImageProcessingConstants::DefaultPixelType*> m_PointerList;


  StringDataArray::WeakPointer m_DataArrayNamesForStitchedCoordinatesPtr;

public:
  ITKRefineTileCoordinates(const ITKRefineTileCoordinates&) = delete; // Copy Constructor Not Implemented
  ITKRefineTileCoordinates(ITKRefineTileCoordinates&&) = delete;      // Move Constructor Not Implemented
  ITKRefineTileCoordinates& operator=(const ITKRefineTileCoordinates&) = delete; // Copy Assignment Not Implemented
  ITKRefineTileCoordinates& operator=(ITKRefineTileCoordinates&&) = delete;      // Move Assignment Not Implemented
};

