#ifndef _itkDream3DDataToImageFilter_hxx
#define _itkDream3DDataToImageFilter_hxx

#include "itkDream3DDataToImageFilter.h"
#include "SIMPLib/Geometry/ImageGeom.h"

namespace itk
{

template< typename PixelType, unsigned int VDimension >
Dream3DDataToImageFilter<PixelType, VDimension>::Dream3DDataToImageFilter()
{
	m_DataContainer = DataContainer::NullPointer();
}

template< typename PixelType, unsigned int VDimension >
Dream3DDataToImageFilter<PixelType, VDimension>::~Dream3DDataToImageFilter()
{
}

template< typename PixelType, unsigned int VDimension>
void
Dream3DDataToImageFilter< PixelType, VDimension >
::SetInput(DataContainer::Pointer dc)
{
	if (!(dc == m_DataContainer))
	{
		m_DataContainer = dc;
		this->Modified();
	}
}

template< typename PixelType, unsigned int VDimension>
void
Dream3DDataToImageFilter< PixelType, VDimension >
::Check()
{
	if (m_DataContainer == DataContainer::NullPointer() )
	{
		itkExceptionMacro("DataContainer not set!");
	}
	IGeometry::Pointer geom = m_DataContainer->getGeometry();
	// Verifies that data container geometry is ImageGeom
	QString geomTypeName = geom->getGeometryTypeAsString();
	if (geomTypeName != "ImageGeometry")
	{
		itkExceptionMacro("DataContainer geometry is not ImageGeometry. It is " + geomTypeName.toStdString());
	}
	// Verifies that data container contains matrix array
	if (!m_DataContainer->doesAttributeMatrixExist(m_MatrixArrayName.c_str()))
	{
		itkExceptionMacro("Matrix array (" + m_MatrixArrayName + ") does not exist");
	}
	AttributeMatrix::Pointer ma = m_DataContainer->getAttributeMatrix(m_MatrixArrayName.c_str());
	// Verifies that matrix array contains data array
	if (!ma->doesAttributeArrayExist(m_DataArrayName.c_str()))
	{
		itkExceptionMacro("Attribute array (" + m_DataArrayName + ") does not exist");
	}
}

template< typename PixelType, unsigned int VDimension>
void
Dream3DDataToImageFilter< PixelType, VDimension >::GenerateOutputInformation()
{
	this->Check();
	IGeometry::Pointer geom = m_DataContainer->getGeometry();
	ImageGeom* imageGeom = dynamic_cast<ImageGeom*>(geom.get());
	if (!imageGeom)
	{
		// This should never happen as Check() verifies that the geometry of
		// the data container is ImageGeom.
		itkExceptionMacro("Error casting geometry!!!");
	}
	// Get data container properties
	float torigin[3];
	float tspacing[3];
	size_t tDims[3];
	imageGeom->getOrigin(torigin[0], torigin[1], torigin[2]);
	imageGeom->getResolution(tspacing[0], tspacing[1], tspacing[2]);
	imageGeom->getDimensions(tDims[0], tDims[1], tDims[2]);
	ImageType::PointType origin;
	ImageType::SizeType size;
	ImageType::SpacingType spacing;
	ImageType::DirectionType direction;
	direction.SetIdentity();
	for (size_t i = 0; i < VDimension; i++)
	{
		spacing[i] = tspacing[i];
		origin[i] = torigin[i];
		size[i] = tDims[i];
	}
	// Set ImportImageFilter properties
	this->SetOrigin(origin);
	this->SetSpacing(spacing);
	this->SetRegion(size);
	this->SetDirection(direction);
	ImportImageFilter::GenerateOutputInformation();
}

template< typename PixelType, unsigned int VDimension>
void
Dream3DDataToImageFilter< PixelType, VDimension >::GenerateData()
{
	this->Check();
	// Get data pointer
	AttributeMatrix::Pointer ma = m_DataContainer->getAttributeMatrix(m_MatrixArrayName.c_str());
	IDataArray::Pointer dataArray = ma->getAttributeArray(m_DataArrayName.c_str());
	const bool importImageFilterWillOwnTheBuffer = false;
	this->SetImportPointer(static_cast<float*>(dataArray->getVoidPointer(0)), dataArray->getSize(),
		importImageFilterWillOwnTheBuffer);
	ImportImageFilter::GenerateData();
}

}// end of itk namespace
#endif