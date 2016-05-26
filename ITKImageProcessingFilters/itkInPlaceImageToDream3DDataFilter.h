#ifndef _ITKInPlaceImageToDream3DDataFilter_h
#define _ITKInPlaceImageToDream3DDataFilter_h

#include "itkImageToDream3DDataFilter.h"

namespace itk
{
	template<typename PixelType, unsigned int VDimension>
	class InPlaceImageToDream3DDataFilter : public ImageToDream3DDataFilter<PixelType,VDimension>
	{
	public:
		/** Standard class typedefs. */
		typedef InPlaceImageToDream3DDataFilter                                    Self;
		typedef ImageToDream3DDataFilter<PixelType,VDimension>                     Superclass;
		typedef SmartPointer<Self>                                          Pointer;
		typedef typename itk::Image<PixelType, VDimension>                       ImageType;
		typedef typename ImageType::Pointer                                 ImagePointer;
		typedef typename ::DataArray<PixelType>                               DataArrayPixelType;
		typedef typename itk::SimpleDataObjectDecorator<DataContainer::Pointer>  DecoratorType;
		typedef DecoratorType::Pointer                                      DecoratorPointer;
		/** Method for creation through the object factory. */
		itkNewMacro(Self);
		virtual void GenerateData() ITK_OVERRIDE;
	protected:
		InPlaceImageToDream3DDataFilter();
		virtual ~InPlaceImageToDream3DDataFilter();
	private:
		InPlaceImageToDream3DDataFilter(const InPlaceImageToDream3DDataFilter&); // Copy Constructor Not Implemented
		void operator=(const InPlaceImageToDream3DDataFilter&); // Operator '=' Not Implemented
	};// end of class InPlaceImageToDream3DDataFilter
} //end of itk namespace

#ifndef ITK_MANUAL_INSTANTIATION
#include "itkInPlaceImageToDream3DDataFilter.hxx"
#endif

#endif
