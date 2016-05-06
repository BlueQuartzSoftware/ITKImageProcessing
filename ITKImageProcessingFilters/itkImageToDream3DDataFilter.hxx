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
	// Create the output. We use static_cast<> here because we know the default
	// output must be of type DecoratorType
	typename DecoratorType::Pointer output =
		static_cast< DecoratorType * >(this->MakeOutput(0).GetPointer());
	this->ProcessObject::SetNumberOfRequiredOutputs(1);
	this->ProcessObject::SetNthOutput(0, output.GetPointer());
	this->SetNumberOfRequiredInputs(1);
}

template< typename PixelType, unsigned int VDimension>
ProcessObject::DataObjectPointer
ImageToDream3DDataFilter< PixelType, VDimension >
::MakeOutput(ProcessObject::DataObjectPointerArraySizeType)
{
	return DecoratorType::New().GetPointer();
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
::GenerateOutputInformation()
{
	DataContainer::Pointer dataContainer = DataContainer::New();
	dataContainer = DataContainer::New(m_DataArrayPath.getDataContainerName());
	if (!dataContainer)
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
	ImagePointer inputPtr = dynamic_cast<ImageType*>(this->GetInput(0));
	ImageType::PointType origin = inputPtr->GetOrigin();
	ImageType::SpacingType spacing = inputPtr->GetSpacing();
	ImageType::SizeType size = inputPtr->GetLargestPossibleRegion().GetSize();
	//Configure global settings
	QVector<float> torigin(3, 0);
	QVector<float> tspacing(3, 1);
	QVector<size_t> tDims(3, 1);
	for (size_t i = 0; i < VDimension; i++)
	{
		torigin[i] = origin[i];
		tspacing[i] = spacing[i];
		tDims[i] = size[i];
	}
	image->setOrigin(torigin[0], torigin[1], torigin[2]);
	image->setResolution(tspacing[0], tspacing[1], tspacing[2]);
	image->setDimensions(tDims[0], tDims[1], tDims[2]);
	dataContainer->setGeometry(image);
	DecoratorType *outputPtr = this->GetOutput();
	outputPtr->Set(dataContainer);
}

template<typename PixelType, unsigned int VDimension>
void
ImageToDream3DDataFilter<PixelType, VDimension>
::GenerateData()
{
	DecoratorType *outputPtr = this->GetOutput();
	DataContainer::Pointer dataContainer = outputPtr->Get();
	ImagePointer inputPtr = dynamic_cast<ImageType*>(this->GetInput(0));
	// Create data array
	QVector<size_t> cDims(1, 0);
	cDims[0] = 1;
	IGeometry::Pointer geom = dataContainer->getGeometry();
	ImageGeom::Pointer imageGeom = std::dynamic_pointer_cast<ImageGeom>(geom);
	size_t dims[3];
	imageGeom->getDimensions(dims[0], dims[1], dims[2]);
	QVector<size_t> tDims(3, 1);
	qCopy(dims, dims + 3, tDims.begin());
	AttributeMatrix::Pointer ma = dataContainer->createAndAddAttributeMatrix(tDims, m_DataArrayPath.getAttributeMatrixName(), SIMPL::AttributeMatrixType::Cell);
	inputPtr->SetBufferedRegion(inputPtr->GetLargestPossibleRegion());
	DataArray<PixelType>::Pointer data = DataArray<PixelType>::WrapPointer(inputPtr->GetBufferPointer(), imageGeom->getNumberOfElements(), cDims, m_DataArrayPath.getDataArrayName(), false);
	ma->addAttributeArray(m_DataArrayPath.getDataArrayName(), data);
	outputPtr->Set(dataContainer);
}

// Check that m_DataArrayPath has been initialized correctly
template<typename PixelType, unsigned int VDimension>
void
ImageToDream3DDataFilter<PixelType, VDimension>
::VerifyPreconditions()
{
	//Test only works if image if of dimension 2 or 3
	if (VDimension != 2 && VDimension != 3)
	{
		itkExceptionMacro("Dimension must be 2 or 3.");
	}
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
	Superclass::VerifyPreconditions();
}

/**
*
*/
template<typename PixelType, unsigned int VDimension>
typename ImageToDream3DDataFilter<PixelType, VDimension>::DecoratorType*
ImageToDream3DDataFilter<PixelType, VDimension>
::GetOutput()
{
	return itkDynamicCastInDebugMode< DecoratorType * >(this->GetPrimaryOutput());
}

} // end of itk namespace

#endif