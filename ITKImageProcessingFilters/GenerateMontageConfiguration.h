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
	PYB11_PROPERTY(DataContainerArrayProxy InputDataContainerArrayProxy READ getInputDataContainerArrayProxy WRITE setInputDataContainerArrayProxy)
    PYB11_PROPERTY(DataArrayPath ImageDataArrayPath READ getImageDataArrayPath WRITE setImageDataArrayPath)

	PYB11_METHOD(DataContainerArrayProxy readDataContainerArrayStructure ARGS path)
public:
	SIMPL_SHARED_POINTERS(GenerateMontageConfiguration)
	SIMPL_FILTER_NEW_MACRO(GenerateMontageConfiguration)
	SIMPL_TYPE_MACRO_SUPER_OVERRIDE(GenerateMontageConfiguration, AbstractFilter)

	~GenerateMontageConfiguration() override;

	SIMPL_FILTER_PARAMETER(IntVec3_t, MontageSize)
	Q_PROPERTY(IntVec3_t MontageSize READ getMontageSize WRITE setMontageSize)

	SIMPL_FILTER_PARAMETER(DataContainerArrayProxy, InputDataContainerArrayProxy)
	Q_PROPERTY(DataContainerArrayProxy InputDataContainerArrayProxy READ getInputDataContainerArrayProxy WRITE setInputDataContainerArrayProxy)

	SIMPL_FILTER_PARAMETER(DataArrayPath, ImageDataArrayPath)
	Q_PROPERTY(DataArrayPath ImageDataArrayPath READ getImageDataArrayPath WRITE setImageDataArrayPath)

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
	void CreateFijiDataStructure();

	/**
	 * @brief Generate the montage
	 */
	template< typename PixelType, typename AccumulatePixelType > void GenerateMontage(int peakMethodToUse);

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
	itk::TileLayout2D m_tiles;
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

