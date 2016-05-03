#ifndef _ITKImageToDream3DDataFilter_h
#define _ITKImageToDream3DDataFilter_h

#include "itkImage.h"
#include "itkProcessObject.h"

#include "SIMPLib/DataContainers/DataContainerArray.h"


namespace itk
{
	template<typename PixelType, unsigned int VDimension>
	class ImageToDream3DDataFilter: public ProcessObject
	{
	public:
		/** Standard class typedefs. */
		typedef ImageToDream3DDataFilter                                Self;
		typedef SmartPointer<Self>                                         Pointer;
		typedef typename Image<PixelType, VDimension> ImageType;
		typedef typename ImageType::Pointer ImagePointer;
		typedef typename DataArray<PixelType> DataArrayPixelType;
		/** Method for creation through the object factory. */
		itkNewMacro(Self);
		void SetDataArrayPath(DataArrayPath dataArray);
		itkGetMacro(DataArrayPath, DataArrayPath);
		virtual void SetInput(const ImageType *image);
		void Update();
		DataContainer::Pointer GetOutput();
	protected:
		void Check();
		ImageToDream3DDataFilter();
		virtual ~ImageToDream3DDataFilter();
	private:
		ImageToDream3DDataFilter(const ImageToDream3DDataFilter&); // Copy Constructor Not Implemented
		void operator=(const ImageToDream3DDataFilter&); // Operator '=' Not Implemented
		DataContainer::Pointer m_DataContainer;
		DataArrayPath m_DataArrayPath;
		TimeStamp m_OutputUpdated;
	};// end of class ImageToDream3DDataFilter
} //end of itk namespace

#ifndef ITK_MANUAL_INSTANTIATION
#include "itkImageToDream3DDataFilter.hxx"
#endif

#endif