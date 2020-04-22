/* ============================================================================
 * Copyright (c) 2019 BlueQuartz Software, LLC
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
 * Neither the names of any of the BlueQuartz Software contributors
 * may be used to endorse or promote products derived from this software without
 * specific prior written permission.
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
#pragma once

#include <memory>

#include <QtCore/QTextStream>
#include <QtXml/QDomElement>

#include "SIMPLib/Filtering/AbstractFilter.h"


#include "ITKImageProcessing/ITKImageProcessingPlugin.h"

class ZeissTagsXmlSection;
class QIODevice;

/**
 * @brief The AxioVisionV4ToTileConfiguration class. See [Filter documentation](@ref metaxmltofijiconfig) for details.
 */
class ITKImageProcessing_EXPORT AxioVisionV4ToTileConfiguration : public AbstractFilter
{
  Q_OBJECT

  // Start Python bindings declarations
  PYB11_BEGIN_BINDINGS(AxioVisionV4ToTileConfiguration SUPERCLASS AbstractFilter)
  PYB11_FILTER()
  PYB11_SHARED_POINTERS(AxioVisionV4ToTileConfiguration)
  PYB11_FILTER_NEW_MACRO(AxioVisionV4ToTileConfiguration)
  PYB11_PROPERTY(QString InputFile READ getInputFile WRITE setInputFile)
  PYB11_PROPERTY(QString OutputFile READ getOutputFile WRITE setOutputFile)
  PYB11_END_BINDINGS()
  // End Python bindings declarations

public:
    using Self = AxioVisionV4ToTileConfiguration;
    using Pointer = std::shared_ptr<Self>;
    using ConstPointer = std::shared_ptr<const Self>;
    using WeakPointer = std::weak_ptr<Self>;
    using ConstWeakPointer = std::weak_ptr<const Self>;
    static Pointer NullPointer();

    static std::shared_ptr<AxioVisionV4ToTileConfiguration> New();

    /**
    * @brief Returns the name of the class for AxioVisionV4ToTileConfiguration
    */
    QString getNameOfClass() const override;
    /**
    * @brief Returns the name of the class for AxioVisionV4ToTileConfiguration
    */
    static QString ClassName();


  ~AxioVisionV4ToTileConfiguration() override;

    /**
    * @brief Setter property for InputFile
    */
    void setInputFile(const QString& value); 
    /**
    * @brief Getter property for InputFile
    * @return Value of InputFile
    */
    QString getInputFile() const;
  Q_PROPERTY(QString InputFile READ getInputFile WRITE setInputFile)

    /**
    * @brief Setter property for OutputFile
    */
    void setOutputFile(const QString& value); 
    /**
    * @brief Getter property for OutputFile
    * @return Value of OutputFile
    */
    QString getOutputFile() const;
  Q_PROPERTY(QString OutputFile READ getOutputFile WRITE setOutputFile)

  /**
   * @brief getCompiledLibraryName Reimplemented from @see AbstractFilter class
   */
  QString getCompiledLibraryName() const override;

  /**
   * @brief getBrandingString Returns the branding string for the filter, which is a tag
   * used to denote the filter's association with specific plugins
   * @return Branding string
   */
  QString getBrandingString() const override;

  /**
   * @brief getFilterVersion Returns a version string for this filter. Default
   * value is an empty string.
   * @return
   */
  QString getFilterVersion() const override;

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
   * @brief execute Reimplemented from @see AbstractFilter class
   */
  void execute() override;



protected:
  AxioVisionV4ToTileConfiguration();

  /**
   * @brief dataCheck Checks for the appropriate parameter values and availability of arrays
   */
  void dataCheck() override;

  /**
   * @brief Initializes all the private instance variables.
   */
  void initialize();

  /**
   * @brief readMetaXml
   * @param device
   */
  void readMetaXml(QIODevice* device);

  /**
   * @brief parseImages
   * @param root
   * @param rootTagsSection
   */
  void parseImages(QDomElement& root, ZeissTagsXmlSection* rootTagsSection);

  void initializeImageGeom(const QDomElement& root, ZeissTagsXmlSection* photoTagsSection, QTextStream& out);

private:
    QString m_InputFile = {};
    QString m_OutputFile = {};

public:
  /* Rule of 5: All special member functions should be defined if any are defined.
   * https://github.com/isocpp/CppCoreGuidelines/blob/master/CppCoreGuidelines.md#c21-if-you-define-or-delete-any-default-operation-define-or-delete-them-all
   */
  AxioVisionV4ToTileConfiguration(const AxioVisionV4ToTileConfiguration&) = delete;            // Copy Constructor Not Implemented
  AxioVisionV4ToTileConfiguration& operator=(const AxioVisionV4ToTileConfiguration&) = delete; // Copy Assignment Not Implemented
  AxioVisionV4ToTileConfiguration(AxioVisionV4ToTileConfiguration&&) = delete;                 // Move Constructor Not Implemented
  AxioVisionV4ToTileConfiguration& operator=(AxioVisionV4ToTileConfiguration&&) = delete;      // Move Assignment Not Implemented

};

