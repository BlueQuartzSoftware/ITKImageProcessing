#ifndef _itkDream3DDataToImageFilter_h
#define _itkDream3DDataToImageFilter_h

#include <itkImportImageFilter.h>

namespace itk
{
	template< typename PixelType, unsigned int VDimension >
	class Dream3DDataToImageFilter : public ImportImageFilter<PixelType,VDimension>
	{
	public:
		/** Standard class typedefs. */
		typedef Dream3DDataToImageFilter                           Self;
		typedef SmartPointer<Self>                                 Pointer;
		typedef typename itk::Image<PixelType, VDimension>              ImageType;
		typedef typename ImageType::Pointer                        ImagePointer;
		typedef typename ::DataArray<PixelType>                      DataArrayPixelType;
		typedef typename itk::ImportImageFilter<PixelType, VDimension>  Superclass;
		/** Method for creation through the object factory. */
		itkNewMacro(Self);
		virtual void SetInput(DataContainer::Pointer &dc);
		itkSetMacro(DataArrayName, std::string);
		itkGetMacro(DataArrayName, std::string);
		itkSetMacro(MatrixArrayName, std::string);
		itkGetMacro(MatrixArrayName, std::string);
	protected:
		Dream3DDataToImageFilter();
		virtual ~Dream3DDataToImageFilter();
		virtual void VerifyPreconditions() ITK_OVERRIDE;
		virtual void GenerateOutputInformation() ITK_OVERRIDE;
		virtual void GenerateData() ITK_OVERRIDE;

	private:
		Dream3DDataToImageFilter(const Dream3DDataToImageFilter&); // Copy Constructor Not Implemented
		void operator=(const Dream3DDataToImageFilter&); // Operator '=' Not Implemented
		DataContainer::Pointer m_DataContainer;
		std::string m_DataArrayName;
		std::string m_MatrixArrayName;
	};
}//end of itk namespace

#ifndef ITK_MANUAL_INSTANTIATION
#include "itkDream3DDataToImageFilter.hxx"
#endif

#endif
