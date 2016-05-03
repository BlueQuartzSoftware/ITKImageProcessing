#ifndef _ITKImageToDream3DDataFilter_hxx
#define _ITKImageToDream3DDataFilter_hxx

#include "itkImageToDream3DDataFilter.h"
#include "SIMPLib/Geometry/ImageGeom.h"

namespace itk
{
	template<typename PixelType, unsigned int VDimension>
	ImageToDream3DDataFilter<PixelType,VDimension>
	::ImageToDream3DDataFilter()
	{
		m_DataArrayPath.update(SIMPL::Defaults::ImageDataContainerName,
			SIMPL::Defaults::CellAttributeMatrixName,
			SIMPL::CellData::ImageData);
		m_DataContainer = DataContainer::NullPointer();
		m_OutputUpdated.Modified();
	}

	template<typename PixelType, unsigned int VDimension>
	ImageToDream3DDataFilter<PixelType, VDimension>
	::~ImageToDream3DDataFilter()
	{
	}

	template< typename PixelType, unsigned int VDimension>
	void
	ImageToDream3DDataFilter< PixelType, VDimension >
	::SetInput(const ImageType *input)
	{
		// Process object is not const-correct so the const_cast is required here
		this->ProcessObject::SetNthInput(0,
			const_cast< ImageType * >(input));
	}

	template< typename PixelType, unsigned int VDimension>
	void
	ImageToDream3DDataFilter< PixelType, VDimension >
	::SetDataArrayPath(DataArrayPath dataArray)
	{
		if (!(dataArray == m_DataArrayPath))
		{
			m_DataArrayPath = dataArray;
			this->Modified();
		}
	}


	template<typename PixelType, unsigned int VDimension>
	void
	ImageToDream3DDataFilter<PixelType, VDimension>
	::Update()
	{
		//Test only works if image if of dimension 2 or 3
		if (VDimension != 2 && VDimension != 3)
		{
			itkExceptionMacro("Dimension must be 2 or 3.");
		}
		m_DataContainer = DataContainer::NullPointer(); // In case of an error, we want to return the NULL pointer
		ImagePointer inputPtr = dynamic_cast<ImageType*>(this->GetInput(0));
		this->Check();
		if (!inputPtr)
		{
			itkExceptionMacro("Input image not set");
		}
		m_DataContainer = DataContainer::New(m_DataArrayPath.getDataContainerName());
		if (!m_DataContainer)
		{
			itkExceptionMacro("Could not create data container");
		}
		//// Create image geometry (data container)
		ImageGeom::Pointer image = ImageGeom::CreateGeometry(SIMPL::Geometry::ImageGeometry);
		if (!image)
		{
			itkExceptionMacro("Could not create image geometry");
		}
		// Get Input image properties
		ImageType::PointType origin = inputPtr->GetOrigin();
		ImageType::SpacingType spacing = inputPtr->GetSpacing();
		ImageType::SizeType size = inputPtr->GetLargestPossibleRegion().GetSize();
		//Configure global settings
		QVector<float> torigin(3, 0);
		QVector<float> tspacing(3, 1);
		size_t numberOfTuples = 1;
		QVector<size_t> tDims(3, 1);
		for (size_t i = 0; i < VDimension; i++)
		{
			torigin[i] = origin[i];
			tspacing[i] = spacing[i];
			tDims[i] = size[i];
			numberOfTuples *= tDims[i];
		}
		image->setOrigin(torigin[0], torigin[1], torigin[2]);
		image->setResolution(tspacing[0], tspacing[1], tspacing[2]);
		image->setDimensions(tDims[0], tDims[1], tDims[2]);
		m_DataContainer->setGeometry(image);

		//// // Create data array
		QVector<size_t> cDims(1, 0);
		cDims[0] = 1;
		AttributeMatrix::Pointer ma = m_DataContainer->createAndAddAttributeMatrix(tDims, m_DataArrayPath.getAttributeMatrixName(), SIMPL::AttributeMatrixType::Cell);
		inputPtr->SetBufferedRegion(inputPtr->GetLargestPossibleRegion());
		DataArray<PixelType>::Pointer data = DataArray<PixelType>::WrapPointer(inputPtr->GetBufferPointer(), numberOfTuples, cDims, m_DataArrayPath.getDataArrayName(), false);
		ma->addAttributeArray(m_DataArrayPath.getDataArrayName(), data);
		m_OutputUpdated.Modified();
	}

	// Check that m_DataArrayPath has been initialized correctly
	template<typename PixelType, unsigned int VDimension>
	void
	ImageToDream3DDataFilter<PixelType, VDimension>
	::Check()
	{
		if (m_DataArrayPath.getDataContainerName().contains('/'))
		{
			itkExceptionMacro("DataContainerName contains a '/'");
		}
		if (m_DataArrayPath.getAttributeMatrixName().contains('/'))
		{
			itkExceptionMacro("AttributeMatrixName contains a '/'");
		}
		if (m_DataArrayPath.getDataArrayName().contains('/'))
		{
			itkExceptionMacro("DataArrayName contains a '/'");
		}
	}

	template<typename PixelType, unsigned int VDimension>
	DataContainer::Pointer
	ImageToDream3DDataFilter<PixelType, VDimension>
	::GetOutput()
	{
		if (this->GetInput(0) && this->GetMTime() > m_OutputUpdated )
		{
			this->Update();
		}
		return m_DataContainer;
	}

} // end of itk namespace

#endif