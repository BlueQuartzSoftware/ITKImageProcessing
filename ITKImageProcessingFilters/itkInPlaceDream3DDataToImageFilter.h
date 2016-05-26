#ifndef _itkInPlaceDream3DDataToImageFilter_h
#define _itkInPlaceDream3DDataToImageFilter_h

#include "itkDream3DDataToImageFilter.h"

namespace itk
{
	template< typename PixelType, unsigned int VDimension >
	class InPlaceDream3DDataToImageFilter : public Dream3DDataToImageFilter<PixelType, VDimension>
	{
	public:
		/** Standard class typedefs. */
		typedef InPlaceDream3DDataToImageFilter                           Self;
		typedef SmartPointer<Self>                                 Pointer;
		typedef typename itk::Image<PixelType, VDimension>              ImageType;
		typedef typename ImageType::Pointer                        ImagePointer;
		typedef typename ::DataArray<PixelType>                      DataArrayPixelType;
		typedef typename itk::Dream3DDataToImageFilter<PixelType, VDimension>  Superclass;
		/** Method for creation through the object factory. */
		itkNewMacro(Self);
	protected:
		InPlaceDream3DDataToImageFilter();
		virtual ~InPlaceDream3DDataToImageFilter();
		virtual void GenerateData() ITK_OVERRIDE;

	private:
		InPlaceDream3DDataToImageFilter(const InPlaceDream3DDataToImageFilter&); // Copy Constructor Not Implemented
		void operator=(const InPlaceDream3DDataToImageFilter&); // Operator '=' Not Implemented
	};
}//end of itk namespace

#ifndef ITK_MANUAL_INSTANTIATION
#include "itkInPlaceDream3DDataToImageFilter.hxx"
#endif

#endif
