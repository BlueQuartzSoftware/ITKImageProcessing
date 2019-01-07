/*
 * Your License or Copyright Information can go here
 */

#pragma once

#include <QtCore/QFile>

#include "SIMPLib/Common/SIMPLibSetGetMacros.h"
#include "SIMPLib/DataContainers/DataContainerArrayProxy.h"
#include "SIMPLib/Filtering/AbstractFilter.h"
#include "SIMPLib/FilterParameters/IntVec3FilterParameter.h"
#include "SIMPLib/Geometry/CompositeTransformContainer.h"
#include "SIMPLib/Geometry/ImageGeom.h"
#include "SIMPLib/Geometry/TransformContainer.h"
#include "SIMPLib/SIMPLib.h"

#include "SIMPLib/ITK/itkImageReaderHelper.h"

#include <itkImageFileReader.h>
#include "itkParseTileConfiguration.h"
#include "itkAffineTransform.h"
#include "itkCompositeTransform.h"

#include "ITKImageProcessing/ITKImageProcessingDLLExport.h"

using AffineType = itk::AffineTransform<double, 3>;
using CompositeTransform = itk::CompositeTransform<double, 3>;

 /**
  * @brief The ImportRegisteredImageMontage class. See [Filter documentation](@ref importregisteredimagemontage) for details.
  */
class ITKImageProcessing_EXPORT GenerateMontageConfiguration : public AbstractFilter
{
	Q_OBJECT
	PYB11_CREATE_BINDINGS(GenerateMontageConfiguration SUPERCLASS AbstractFilter)
	PYB11_PROPERTY(IntVec3_t MontageSize READ getMontageSize WRITE setMontageSize)
  PYB11_PROPERTY(QStringList, ImageDataContainers READ getImageDataContainers WRITE setImageDataContainers)
	PYB11_PROPERTY(QString CommonAttributeMatrixName READ getCommonAttributeMatrixName WRITE setCommonAttributeMatrixName)
	PYB11_PROPERTY(QString CommonDataArrayName READ getCommonDataArrayName WRITE setCommonDataArrayName)
public:
	SIMPL_SHARED_POINTERS(GenerateMontageConfiguration)
	SIMPL_FILTER_NEW_MACRO(GenerateMontageConfiguration)
	SIMPL_TYPE_MACRO_SUPER_OVERRIDE(GenerateMontageConfiguration, AbstractFilter)

	~GenerateMontageConfiguration() override;

	SIMPL_FILTER_PARAMETER(IntVec3_t, MontageSize)
	Q_PROPERTY(IntVec3_t MontageSize READ getMontageSize WRITE setMontageSize)

  SIMPL_FILTER_PARAMETER(QStringList, ImageDataContainers)
  Q_PROPERTY(QStringList ImageDataContainers READ getImageDataContainers WRITE setImageDataContainers)

	SIMPL_FILTER_PARAMETER(QString, CommonAttributeMatrixName)
	Q_PROPERTY(QString CommonAttributeMatrixName READ getCommonAttributeMatrixName WRITE setCommonAttributeMatrixName)

  SIMPL_FILTER_PARAMETER(bool, StitchMontage)
  Q_PROPERTY(bool StitchMontage READ getStitchMontage WRITE setStitchMontage)

	SIMPL_FILTER_PARAMETER(QString, CommonDataArrayName)
	Q_PROPERTY(QString CommonDataArrayName READ getCommonDataArrayName WRITE setCommonDataArrayName)

  SIMPL_FILTER_PARAMETER(QString, MontageDataContainerName)
  Q_PROPERTY(QString MontageDataContainerName READ getMontageDataContainerName WRITE setMontageDataContainerName)

  SIMPL_FILTER_PARAMETER(QString, MontageAttributeMatrixName)
  Q_PROPERTY(QString MontageAttributeMatrixName READ getMontageAttributeMatrixName WRITE setMontageAttributeMatrixName)

  SIMPL_FILTER_PARAMETER(QString, MontageDataArrayName)
  Q_PROPERTY(QString MontageDataArrayName READ getMontageDataArrayName WRITE setMontageDataArrayName)

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
	 * @brief execute Reimplemented from @see AbstractFilter class
	 */
	void execute() override;

	/**
	 * @brief preflight Reimplemented from @see AbstractFilter class
	 */
	void preflight() override;

signals:
	/**
	 * @brief updateFilterParameters Emitted when the Filter requests all the latest Filter parameters
	 * be pushed from a user-facing control (such as a widget)
	 * @param filter Filter instance pointer
	 */
	void updateFilterParameters(AbstractFilter* filter);

	/**
	 * @brief parametersChanged Emitted when any Filter parameter is changed internally
	 */
	void parametersChanged();

	/**
	 * @brief preflightAboutToExecute Emitted just before calling dataCheck()
	 */
	void preflightAboutToExecute();

	/**
	 * @brief preflightExecuted Emitted just after calling dataCheck()
	 */
	void preflightExecuted();

protected:
	GenerateMontageConfiguration();

	/**
	 * @brief dataCheck Checks for the appropriate parameter values and availability of arrays
	 */
	void dataCheck();

	/**
	 * @brief Initializes all the private instance variables.
	 */
	void initialize();

	/**
	 * @brief Create the Fiji data structure
	 */
  void createFijiDataStructure();

	/**
	 * @brief Generate the montage
	 */
  template<typename PixelType, typename AccumulatePixelType> void generateMontage(int peakMethodToUse = 1, unsigned streamSubdivisions = 1);

	/**
	 * @brief Get the image from the appropriate data container
	 * @param y 
	 * @param x 
	 */
	DataContainer::Pointer GetImageDataContainer(int y, int x);

	/**
	 * @brief Get the transform container from an affine transform
	 * @param itkAffine
	 */
	typename TransformContainer::Pointer GetTransformContainerFromITKAffineTransform(const AffineType::Pointer& itkAffine);

private:
  itk::TileLayout2D m_StageTiles;
  itk::TileLayout2D m_ActualTiles;
	unsigned int m_xMontageSize;
	unsigned int m_yMontageSize;
	QString m_dataContainerPrefix;

	/**
	 * @brief Include the declarations of the ITKImageReader helper functions that are common
	 * to a few different filters across different plugins.
	 */
	ITK_IMAGE_READER_HELPER_ImageDataArrayName() ITK_IMAGE_READER_HELPER_DECL()

public:
	GenerateMontageConfiguration(const GenerateMontageConfiguration&) = delete; // Copy Constructor Not Implemented
	GenerateMontageConfiguration(GenerateMontageConfiguration&&) = delete;                   // Move Constructor Not Implemented
	GenerateMontageConfiguration& operator=(const GenerateMontageConfiguration&) = delete;   // Copy Assignment Not Implemented
	GenerateMontageConfiguration& operator=(GenerateMontageConfiguration&&) = delete;        // Move Assignment Not Implemented
};

