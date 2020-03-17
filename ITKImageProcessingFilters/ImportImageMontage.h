/*
 * Your License or Copyright Information can go here
 */

#pragma once

#include <memory>

#include <QtCore/QFile>

#include "SIMPLib/SIMPLib.h"
#include "SIMPLib/DataArrays/StringDataArray.h"
#include "SIMPLib/FilterParameters/FileListInfoFilterParameter.h"
#include "SIMPLib/FilterParameters/FloatVec3FilterParameter.h"
#include "SIMPLib/Filtering/AbstractFilter.h"

#include "SIMPLib/ITK/itkImageReaderHelper.h"

#include <itkImageFileReader.h>

#include "ITKImageProcessing/ITKImageProcessingDLLExport.h"

/**
 * @brief The ImportImageMontage class. See [Filter documentation](@ref importimagemontage) for details.
 */
class ITKImageProcessing_EXPORT ImportImageMontage : public AbstractFilter
{
  Q_OBJECT

#ifdef SIMPL_ENABLE_PYTHON
  PYB11_CREATE_BINDINGS(ImportImageMontage SUPERCLASS AbstractFilter)
  PYB11_SHARED_POINTERS(ImportImageMontage)
  PYB11_FILTER_NEW_MACRO(ImportImageMontage)
  PYB11_FILTER_PARAMETER(DataArrayPath, DataContainerName)
  PYB11_FILTER_PARAMETER(QString, CellAttributeMatrixName)
  PYB11_FILTER_PARAMETER(QString, MetaDataAttributeMatrixName)
  PYB11_FILTER_PARAMETER(FloatVec3Type, Origin)
  PYB11_FILTER_PARAMETER(FloatVec3Type, Spacing)
  PYB11_FILTER_PARAMETER(StackFileListInfo, InputFileListInfo)
  PYB11_PROPERTY(DataArrayPath DataContainerName READ getDataContainerName WRITE setDataContainerName)
  PYB11_PROPERTY(QString CellAttributeMatrixName READ getCellAttributeMatrixName WRITE setCellAttributeMatrixName)
  PYB11_PROPERTY(QString MetaDataAttributeMatrixName READ getMetaDataAttributeMatrixName WRITE setMetaDataAttributeMatrixName)
  PYB11_PROPERTY(FloatVec3Type Origin READ getOrigin WRITE setOrigin)
  PYB11_PROPERTY(FloatVec3Type Spacing READ getSpacing WRITE setSpacing)
  PYB11_PROPERTY(StackFileListInfo InputFileListInfo READ getInputFileListInfo WRITE setInputFileListInfo)
#endif

public:
    using Self = ImportImageMontage;
    using Pointer = std::shared_ptr<Self>;
    using ConstPointer = std::shared_ptr<const Self>;
    using WeakPointer = std::weak_ptr<Self>;
    using ConstWeakPointer = std::weak_ptr<const Self>;
    static Pointer NullPointer();

    static std::shared_ptr<ImportImageMontage> New();

    /**
    * @brief Returns the name of the class for ImportImageMontage
    */
    QString getNameOfClass() const override;
    /**
    * @brief Returns the name of the class for ImportImageMontage
    */
    static QString ClassName();


  ~ImportImageMontage() override;

    /**
    * @brief Setter property for DataContainerName
    */
    void setDataContainerName(const DataArrayPath& value); 
    /**
    * @brief Getter property for DataContainerName
    * @return Value of DataContainerName
    */
    DataArrayPath getDataContainerName() const;

  Q_PROPERTY(DataArrayPath DataContainerName READ getDataContainerName WRITE setDataContainerName)

    /**
    * @brief Setter property for CellAttributeMatrixName
    */
    void setCellAttributeMatrixName(const QString& value); 
    /**
    * @brief Getter property for CellAttributeMatrixName
    * @return Value of CellAttributeMatrixName
    */
    QString getCellAttributeMatrixName() const;

  Q_PROPERTY(QString CellAttributeMatrixName READ getCellAttributeMatrixName WRITE setCellAttributeMatrixName)

    /**
    * @brief Setter property for MetaDataAttributeMatrixName
    */
    void setMetaDataAttributeMatrixName(const QString& value); 
    /**
    * @brief Getter property for MetaDataAttributeMatrixName
    * @return Value of MetaDataAttributeMatrixName
    */
    QString getMetaDataAttributeMatrixName() const;

  Q_PROPERTY(QString MetaDataAttributeMatrixName READ getMetaDataAttributeMatrixName WRITE setMetaDataAttributeMatrixName)

    /**
    * @brief Setter property for Origin
    */
    void setOrigin(const FloatVec3Type& value); 
    /**
    * @brief Getter property for Origin
    * @return Value of Origin
    */
    FloatVec3Type getOrigin() const;

  Q_PROPERTY(FloatVec3Type Origin READ getOrigin WRITE setOrigin)

    /**
    * @brief Setter property for Spacing
    */
    void setSpacing(const FloatVec3Type& value); 
    /**
    * @brief Getter property for Spacing
    * @return Value of Spacing
    */
    FloatVec3Type getSpacing() const;

  Q_PROPERTY(FloatVec3Type Spacing READ getSpacing WRITE setSpacing)

    /**
    * @brief Setter property for InputFileListInfo
    */
  void setInputFileListInfo(const StackFileListInfo& value);
  /**
   * @brief Getter property for InputFileListInfo
   * @return Value of InputFileListInfo
   */
  StackFileListInfo getInputFileListInfo() const;

  Q_PROPERTY(StackFileListInfo InputFileListInfo READ getInputFileListInfo WRITE setInputFileListInfo)

  /**
   * @brief getCompiledLibraryName Reimplemented from @see AbstractFilter class
   */
  QString getCompiledLibraryName() const override;

  /**
   * @brief getFilterVersion Returns a version string for this filter. Default
   * value is an empty string.
   * @return
   */
  QString getFilterVersion() const override;

  /**
   * @brief getBrandingString Returns the branding string for the filter, which is a tag
   * used to denote the filter's association with specific plugins
   * @return Branding string
   */
  QString getBrandingString() const override;

  /**
   * @brief newFilterInstance Reimplemented from @see AbstractFilter class
   */
  AbstractFilter::Pointer newFilterInstance(bool copyFilterParameters) const override;

  /**
   * @brief getGroupName Reimplemented from @see AbstractFilter class
   */
  QString getGroupName() const override;

  /**
   * @brief getSubGroupName Reimplemented from @see AbstractFilter class
   */
  QString getSubGroupName() const override;

  /**
   * @brief getUuid Return the unique identifier for this filter.
   * @return A QUuid object.
   */
  QUuid getUuid() const override;

  /**
   * @brief getHumanLabel Reimplemented from @see AbstractFilter class
   */
  QString getHumanLabel() const override;

  /**
   * @brief setupFilterParameters Reimplemented from @see AbstractFilter class
   */
  void setupFilterParameters() override;

  /**
   * @brief readFilterParameters Reimplemented from @see AbstractFilter class
   */
  void readFilterParameters(AbstractFilterParametersReader* reader, int index) override;

  /**
   * @brief execute Reimplemented from @see AbstractFilter class
   */
  void execute() override;

protected:
  ImportImageMontage();

  /**
   * @brief parseRegistrationFile Parses the ASCII file with the registration coordinates
   * @param reader QFile to read
   * @return Integer error value
   */
  int32_t parseRegistrationFile(QFile& reader);

  /**
   * @brief dataCheck Checks for the appropriate parameter values and availability of arrays
   */
  void dataCheck();

  /**
   * @brief Initializes all the private instance variables.
   */
  void initialize();

private:
    DataArrayPath m_DataContainerName = {};
    QString m_CellAttributeMatrixName = {};
    QString m_MetaDataAttributeMatrixName = {};
    FloatVec3Type m_Origin = {};
    FloatVec3Type m_Spacing = {};
    StackFileListInfo m_InputFileListInfo = {};
    std::weak_ptr<DataArray<float>>  m_RegistrationCoordinatesPtr;
    float* m_RegistrationCoordinates = nullptr;



  StringDataArray::WeakPointer m_AttributeArrayNamesPtr;
  QFile m_InStream;
  int32_t m_NumImages;
  QVector<QString> m_ArrayNames;
  std::vector<float> m_Coords;

  /**
   * @brief Include the declarations of the ITKImageReader helper functions that are common
   * to a few different filters across different plugins.
   */
  ITK_IMAGE_READER_HELPER_ImageDataArrayName() ITK_IMAGE_READER_HELPER_DECL()

public:
  ImportImageMontage(const ImportImageMontage&) = delete; // Copy Constructor Not Implemented
  ImportImageMontage(ImportImageMontage&&) = delete;                   // Move Constructor Not Implemented
  ImportImageMontage& operator=(const ImportImageMontage&) = delete;   // Copy Assignment Not Implemented
  ImportImageMontage& operator=(ImportImageMontage&&) = delete;        // Move Assignment Not Implemented
};

