#ifndef _ITKImageToDream3DDataFilter_h
#define _ITKImageToDream3DDataFilter_h

#include "itkImage.h"
#include "itkProcessObject.h"
#include "itkSimpleDataObjectDecorator.h"
#include "SIMPLib/DataContainers/DataContainerArray.h"


namespace itk
{
	template<typename PixelType, unsigned int VDimension>
	class ImageToDream3DDataFilter: public ProcessObject
	{
	public:
		/** Standard class typedefs. */
		typedef ImageToDream3DDataFilter                                    Self;
		typedef ProcessObject                                               Superclass;
		typedef SmartPointer<Self>                                          Pointer;
		typedef typename Image<PixelType, VDimension>                       ImageType;
		typedef typename ImageType::Pointer                                 ImagePointer;
		typedef typename DataArray<PixelType>                               DataArrayPixelType;
		typedef typename SimpleDataObjectDecorator<DataContainer::Pointer>  DecoratorType;
		typedef DecoratorType::Pointer                                      DecoratorPointer;
		/** Method for creation through the object factory. */
		itkNewMacro(Self);
		virtual void SetInput(const ImageType *image);
		void SetDataContainer(DataContainer::Pointer &dc);
		virtual void GenerateData() ITK_OVERRIDE;
		virtual void GenerateOutputInformation() ITK_OVERRIDE;
		DecoratorType* GetOutput();
		ProcessObject::DataObjectPointer MakeOutput(ProcessObject::DataObjectPointerArraySizeType);
		itkSetMacro(DataArrayName, std::string);
		itkGetMacro(DataArrayName, std::string);
		itkSetMacro(MatrixArrayName, std::string);
		itkGetMacro(MatrixArrayName, std::string);
	protected:
		virtual void VerifyPreconditions() ITK_OVERRIDE;
		ImageToDream3DDataFilter();
		virtual ~ImageToDream3DDataFilter();
		void CheckValidArrayPathComponentName(std::string var);
	private:
		ImageToDream3DDataFilter(const ImageToDream3DDataFilter&); // Copy Constructor Not Implemented
		void operator=(const ImageToDream3DDataFilter&); // Operator '=' Not Implemented
		std::string m_DataArrayName;
		std::string m_MatrixArrayName;
	};// end of class ImageToDream3DDataFilter
} //end of itk namespace

#ifndef ITK_MANUAL_INSTANTIATION
#include "itkImageToDream3DDataFilter.hxx"
#endif

#endif