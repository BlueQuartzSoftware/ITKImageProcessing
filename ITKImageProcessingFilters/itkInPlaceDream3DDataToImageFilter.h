#ifndef _itkInPlaceDream3DDataToImageFilter_h
#define _itkInPlaceDream3DDataToImageFilter_h

#include <itkImportImageFilter.h>

namespace itk
{
  template< typename PixelType, unsigned int VDimension >
  class InPlaceDream3DDataToImageFilter : public ImageSource< itk::Image<PixelType, VDimension> >
  {
  public:
    /** Standard class typedefs. */
    typedef InPlaceDream3DDataToImageFilter                                 Self;
    typedef SmartPointer<Self>                                              Pointer;
    typedef typename itk::Image<PixelType, VDimension>                      ImageType;
    typedef typename ImageType::Pointer                                     ImagePointer;
    typedef typename ::DataArray<PixelType>                                 DataArrayPixelType;
    typedef typename itk::ImageSource< itk::Image<PixelType, VDimension> >  Superclass;
    typedef itk::SizeValueType                                              SizeValueType;
    typedef ImportImageContainer< SizeValueType, PixelType >                ImportImageContainerType;
    typedef Image< PixelType, VDimension >                                  OutputImageType;
    typedef typename OutputImageType::Pointer                               OutputImagePointer;

    /** Method for creation through the object factory. */
    itkNewMacro(Self);
    itkTypeMacro(InPlaceDream3DDataToImageFilter, ImportImageFilter);
    virtual void SetInput(DataContainer::Pointer &dc);
    itkSetMacro(DataArrayName, std::string);
    itkGetMacro(DataArrayName, std::string);
    itkSetMacro(MatrixArrayName, std::string);
    itkGetMacro(MatrixArrayName, std::string);
    itkSetMacro( InPlace, bool );
    itkGetConstMacro( InPlace, bool );
    itkBooleanMacro( InPlace );
  protected:
    InPlaceDream3DDataToImageFilter();
    virtual ~InPlaceDream3DDataToImageFilter();
    virtual void VerifyPreconditions() ITK_OVERRIDE;
    virtual void GenerateOutputInformation() ITK_OVERRIDE;
    virtual void GenerateData() ITK_OVERRIDE;
    DataContainer::Pointer m_DataContainer;

  private:
    InPlaceDream3DDataToImageFilter(const InPlaceDream3DDataToImageFilter&); // Copy Constructor Not Implemented
    void operator=(const InPlaceDream3DDataToImageFilter&); // Operator '=' Not Implemented
    std::string m_DataArrayName;
    std::string m_MatrixArrayName;
    typename ImportImageContainerType::Pointer m_ImportImageContainer;
    bool m_InPlace; // enable the possibility of in-place
  };
}//end of itk namespace

#ifndef ITK_MANUAL_INSTANTIATION
#include "itkInPlaceDream3DDataToImageFilter.hxx"
#endif

#endif
