#ifndef _ITKInPlaceImageToDream3DDataFilter_h
#define _ITKInPlaceImageToDream3DDataFilter_h

#include "itkDream3DImage.h"
#include "itkProcessObject.h"
#include "itkSimpleDataObjectDecorator.h"
#include "SIMPLib/DataContainers/DataContainerArray.h"


namespace itk
{
  template<typename PixelType, unsigned int VDimension>
  class InPlaceImageToDream3DDataFilter: public ProcessObject
  {
  public:
    /** Standard class typedefs. */
    typedef InPlaceImageToDream3DDataFilter                                  Self;
    typedef ProcessObject                                                    Superclass;
    typedef SmartPointer<Self>                                               Pointer;
    typedef typename itk::Dream3DImage<PixelType, VDimension>               ImageType;
    typedef typename ImageType::Pointer                                      ImagePointer;
    typedef typename ::DataArray<PixelType>                                  DataArrayPixelType;
    typedef typename itk::SimpleDataObjectDecorator<DataContainer::Pointer>  DecoratorType;
    typedef DecoratorType::Pointer                                           DecoratorPointer;
    /** Method for creation through the object factory. */
    itkNewMacro(Self);
    itkTypeMacro(InPlaceImageToDream3DDataFilter, ProcessObject);
    virtual void SetInput(const ImageType *image);
    void SetDataContainer(DataContainer::Pointer dc);
    virtual void GenerateData() ITK_OVERRIDE;
    virtual void GenerateOutputInformation() ITK_OVERRIDE;
    DecoratorType* GetOutput();
    ProcessObject::DataObjectPointer MakeOutput(ProcessObject::DataObjectPointerArraySizeType) override;
    itkSetMacro(DataArrayName, std::string);
    itkGetMacro(DataArrayName, std::string);
    itkSetMacro(AttributeMatrixArrayName, std::string);
    itkGetMacro(AttributeMatrixArrayName, std::string);
    virtual void SetDataArrayPath( DataArrayPath dataArrayPath );
    itkSetMacro( InPlace, bool );
    itkGetConstMacro(InPlace, bool);
    itkBooleanMacro(InPlace);
  protected:
    virtual void VerifyPreconditions() ITK_OVERRIDE;
    InPlaceImageToDream3DDataFilter();
    virtual ~InPlaceImageToDream3DDataFilter();
    void CheckValidArrayPathComponentName(std::string var);
  private:
    InPlaceImageToDream3DDataFilter(const InPlaceImageToDream3DDataFilter&); // Copy Constructor Not Implemented
    void operator=(const InPlaceImageToDream3DDataFilter&); // Operator '=' Not Implemented
    std::string m_DataArrayName;
    std::string m_AttributeMatrixArrayName;
    bool m_InPlace; // enable the possibility of in-place
  };// end of class InPlaceImageToDream3DDataFilter
} //end of itk namespace

#ifndef ITK_MANUAL_INSTANTIATION
#include "itkInPlaceImageToDream3DDataFilter.hxx"
#endif

#endif
