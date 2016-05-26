#ifndef _ITKImageToDream3DDataFilter_hxx
#define _ITKImageToDream3DDataFilter_hxx

#include "itkImageToDream3DDataFilter.h"
#include "SIMPLib/Geometry/ImageGeom.h"
#include <QString>

namespace itk
{
	
template<typename PixelType, unsigned int VDimension>
ImageToDream3DDataFilter<PixelType,VDimension>
::ImageToDream3DDataFilter()
{
	m_DataArrayName = (SIMPL::Defaults::CellAttributeMatrixName).toStdString();
	m_MatrixArrayName = (SIMPL::CellData::ImageData).toStdString();
	// Create the output. We use static_cast<> here because we know the default
	// output must be of type DecoratorType
	typename DecoratorType::Pointer output =
		static_cast< DecoratorType * >(this->MakeOutput(0).GetPointer());
	this->ProcessObject::SetNumberOfRequiredOutputs(1);
	this->ProcessObject::SetNthOutput(0, output.GetPointer());
	this->SetNumberOfRequiredInputs(1);
	this->SetDataContainer(DataContainer::NullPointer());
}

template< typename PixelType, unsigned int VDimension>
void
ImageToDream3DDataFilter< PixelType, VDimension >
::SetDataContainer(DataContainer::Pointer dc)
{
	DecoratorType *outputPtr = this->GetOutput();
	outputPtr->Set(dc);
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


template<typename PixelType, unsigned int VDimension>
void
ImageToDream3DDataFilter<PixelType, VDimension>
::GenerateOutputInformation()
{
	DecoratorType *outputPtr = this->GetOutput();
	DataContainer::Pointer dataContainer = outputPtr->Get();
	float tol = 0.000001;
	float torigin[3];
	float tspacing[3];
	size_t tDims[3];
	// Get Input image properties
	ImagePointer inputPtr = dynamic_cast<ImageType*>(this->GetInput(0));
	typename ImageType::PointType origin = inputPtr->GetOrigin();
	typename ImageType::SpacingType spacing = inputPtr->GetSpacing();
	typename ImageType::SizeType size = inputPtr->GetLargestPossibleRegion().GetSize();
	//// Create image geometry (data container)
	ImageGeom::Pointer imageGeom;
	IGeometry::Pointer geom = dataContainer->getGeometry();
	if (geom)
	{
		imageGeom = std::dynamic_pointer_cast<ImageGeom>(geom);
		if (!imageGeom)
		{
			itkExceptionMacro("Data container contains a geometry that is not ImageGeometry");
		}
		imageGeom->getOrigin(torigin[0], torigin[1], torigin[2]);
		imageGeom->getResolution(tspacing[0], tspacing[1], tspacing[2]);
		imageGeom->getDimensions(tDims[0], tDims[1], tDims[2]);
		for( size_t i=0; i < 3; i++ )
		{
			if (abs(torigin[i] - origin[i]) > tol
				|| abs(tspacing[i] - spacing[i]) > tol
				|| tDims[i] != size[i]
				)
			{
				itkExceptionMacro("Data container contains a geometry that do not match image properties");
			}
		}
	}
	else
	{
		imageGeom = ImageGeom::CreateGeometry(SIMPL::Geometry::ImageGeometry);
		if (!imageGeom)
		{
			itkExceptionMacro("Could not create image geometry");
		}
		for (size_t i = 0; i < VDimension; i++)
		{
			torigin[i] = origin[i];
			tspacing[i] = spacing[i];
			tDims[i] = size[i];
		}
		imageGeom->setOrigin(torigin[0], torigin[1], torigin[2]);
		imageGeom->setResolution(tspacing[0], tspacing[1], tspacing[2]);
		imageGeom->setDimensions(tDims[0], tDims[1], tDims[2]);
		dataContainer->setGeometry(imageGeom);
	}
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
	AttributeMatrix::Pointer ma;
	if (dataContainer->doesAttributeMatrixExist(m_MatrixArrayName.c_str()))
	{
		// Check that size matches
		ma = dataContainer->getAttributeMatrix(m_MatrixArrayName.c_str());
		size_t numofTuples = ma->getNumTuples();
		QVector<size_t> matDims = ma->getTupleDimensions();
		if (matDims != tDims)
		{
			itkExceptionMacro("Tuples dimension of existing matrix array do not match image size.");
		}
		if (ma->getType() != SIMPL::AttributeMatrixType::Cell)
		{
			itkExceptionMacro("Attribute matrix is not of type SIMPL::AttributeMatrixType::Cell.");
		}
	}
	else
	{
		ma = dataContainer->createAndAddAttributeMatrix(tDims, m_MatrixArrayName.c_str(), SIMPL::AttributeMatrixType::Cell);
	}
	// Checks if doesAttributeArray exists
	if (ma->doesAttributeArrayExist(m_DataArrayName.c_str()))
	{
		itkExceptionMacro("Existing data array with same name.");
	}
	else
	{
		DataArrayPixelType::Pointer data;
		data = DataArrayPixelType::FromPointer(inputPtr->GetBufferPointer(), imageGeom->getNumberOfElements(), m_DataArrayName.c_str(), true);
		ma->addAttributeArray(m_DataArrayName.c_str(), data);
	}
	outputPtr->Set(dataContainer);
}

// Check that names has been initialized correctly
template<typename PixelType, unsigned int VDimension>
void
ImageToDream3DDataFilter<PixelType, VDimension>
::CheckValidArrayPathComponentName(std::string var)
{
	if (var.find('/') != std::string::npos)
	{
		itkExceptionMacro("Name contains a '/'");
	}
	if (var.empty())
	{
		itkExceptionMacro("Name is empty");
	}
}


// Check that the inputs have been initialized correctly
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
	CheckValidArrayPathComponentName(m_MatrixArrayName);
	CheckValidArrayPathComponentName(m_DataArrayName);
	// Verify data container
	DecoratorType *outputPtr = this->GetOutput();
	if (!outputPtr->Get())
	{
		itkExceptionMacro("Data container not set");
	}
	//
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
