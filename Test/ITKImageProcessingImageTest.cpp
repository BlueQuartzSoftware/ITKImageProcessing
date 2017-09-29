/* ============================================================================
* Copyright (c) 2009-2016 BlueQuartz Software, LLC
*
* Redistribution and use in source and binary forms, with or without modification,
* are permitted provided that the following conditions are met:
*
* Redistributions of source code must retain the above copyright notice, this
* list of conditions and the following disclaimer.
*
* Redistributions in binary form must reproduce the above copyright notice, this
* list of conditions and the following disclaimer in the documentation and/or
* other materials provided with the distribution.
*
* Neither the name of BlueQuartz Software, the US Air Force, nor the names of its
* contributors may be used to endorse or promote products derived from this software
* without specific prior written permission.
*
* THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
* AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
* IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
* DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
* FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
* DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
* SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
* CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
* OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE
* USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*
* The code contained herein was partially funded by the followig contracts:
*    United States Air Force Prime Contract FA8650-10-D-5210
*
* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
#include <QtCore/QCoreApplication>
#include <QtCore/QFile>

#include "SIMPLib/Common/SIMPLibSetGetMacros.h"
#include "SIMPLib/DataArrays/DataArray.hpp"
#include "SIMPLib/SIMPLib.h"

#include "SIMPLib/Common/UnitTestSupport.hpp"
#include "SIMPLib/Filtering/QMetaObjectUtilities.h"
#include "SIMPLib/Plugin/ISIMPLibPlugin.h"
#include "SIMPLib/Plugin/SIMPLibPluginLoader.h"

#include "ITKImageProcessing/ITKImageProcessingFilters/itkGetComponentsDimensions.h"
#include "ITKImageProcessing/ITKImageProcessingFilters/itkInPlaceDream3DDataToImageFilter.h"
#include "ITKImageProcessing/ITKImageProcessingFilters/itkInPlaceImageToDream3DDataFilter.h"

#include "ITKImageProcessingTestFileLocations.h"

#include <itkImageDuplicator.h>
#include <itkImageRegionIterator.h>

class ITKImageProcessingImageTest
{

public:
  ITKImageProcessingImageTest()
  {
  }
  virtual ~ITKImageProcessingImageTest()
  {
  }

  // -----------------------------------------------------------------------------
  //  Test methods
  // -----------------------------------------------------------------------------
  int CreateDream3DDataToImageFilters()
  {
    typedef itk::InPlaceDream3DDataToImageFilter<float, 3> ToITK_float_3_Type;
    ToITK_float_3_Type::Pointer toITK_float_3 = ToITK_float_3_Type::New();
    typedef itk::InPlaceDream3DDataToImageFilter<itk::RGBAPixel<float>, 3> ToITK_rgba_float_3_Type;
    ToITK_rgba_float_3_Type::Pointer toITK_rgba_float_3 = ToITK_rgba_float_3_Type::New();
    typedef itk::InPlaceDream3DDataToImageFilter<itk::RGBAPixel<unsigned char>, 3> ToITK_rgba_uchar_3_Type;
    ToITK_rgba_uchar_3_Type::Pointer toITK_rgba_uchar_3 = ToITK_rgba_uchar_3_Type::New();
    typedef itk::InPlaceDream3DDataToImageFilter<itk::RGBPixel<float>, 3> ToITK_rgb_float_3_Type;
    ToITK_rgb_float_3_Type::Pointer toITK_rgb_float_3 = ToITK_rgb_float_3_Type::New();
    return EXIT_SUCCESS;
  }

  int TestConvertITKToDream3DAndBackToITKImages()
  {
    TestComponentImages<2>();
    TestComponentImages<3>();
    return EXIT_SUCCESS;
  }

  template <unsigned int Dimensions> void TestComponentImages()
  {
    TestPixelTypeImages<uint8_t, Dimensions>();
    TestPixelTypeImages<int8_t, Dimensions>();
    TestPixelTypeImages<float, Dimensions>();
    TestPixelTypeImages<double, Dimensions>();
    TestPixelTypeImages<uint32_t, Dimensions>();
    TestPixelTypeImages<int32_t, Dimensions>();
  }

  template <typename ComponentType, unsigned int Dimensions> void TestPixelTypeImages()
  {
    typedef typename itk::RGBAPixel<ComponentType> RGBAType;
    TestImages<RGBAType, Dimensions>();
    typedef typename itk::RGBPixel<ComponentType> RGBType;
    TestImages<RGBType, Dimensions>();
    typedef typename itk::Vector<ComponentType, Dimensions> VectorType;
    TestImages<VectorType, Dimensions>();
  }

  template <typename PixelType, unsigned int Dimensions> void TestImages()
  {
    // Create image
    typedef typename itk::Dream3DImage<PixelType, Dimensions> ImageType;
    typename ImageType::Pointer image = ImageType::New();
    typename ImageType::SizeType size;
    size.Fill(10);
    image->SetRegions(size);
    image->Allocate();
    image->FillBuffer(itk::NumericTraits<PixelType>::max());
    // Copy image to save it as baseline
    typedef typename itk::ImageDuplicator<ImageType> DuplicatorType;
    typename DuplicatorType::Pointer duplicator = DuplicatorType::New();
    duplicator->SetInputImage(image);
    duplicator->Update();
    typename ImageType::Pointer clonedImage = duplicator->GetOutput();
    // Convert image to DREAM.3D data
    DataArrayPath path("DataContainer", "AttributeMatrix", "DataArray");
    DataContainer::Pointer container = DataContainer::New(path.getDataContainerName());
    typedef typename itk::InPlaceImageToDream3DDataFilter<PixelType, Dimensions> toDream3DType;
    typename toDream3DType::Pointer toDream3D = toDream3DType::New();
    toDream3D->SetInput(image);
    toDream3D->SetInPlace(true);
    toDream3D->SetAttributeMatrixArrayName(path.getAttributeMatrixName().toStdString());
    toDream3D->SetDataArrayName(path.getDataArrayName().toStdString());
    toDream3D->SetDataContainer(container);
    toDream3D->Update();
    // Convert back to ITK format
    typedef typename itk::InPlaceDream3DDataToImageFilter<PixelType, Dimensions> ToITKType;
    typename ToITKType::Pointer toITK = ToITKType::New();
    toITK->SetInput(container);
    toITK->SetAttributeMatrixArrayName(path.getAttributeMatrixName().toStdString());
    toITK->SetDataArrayName(path.getDataArrayName().toStdString());
    toITK->SetInPlace(true);
    toITK->Update();
    // Compare baseline and output
    typedef typename itk::ImageRegionIterator<ImageType> IteratorType;
    IteratorType it(toITK->GetOutput(), toITK->GetOutput()->GetLargestPossibleRegion());
    IteratorType itb(clonedImage, clonedImage->GetLargestPossibleRegion());
    for(it.GoToBegin(), itb.GoToBegin(); !it.IsAtEnd() && !itb.IsAtEnd(); ++it, ++itb)
    {
      if(it.Get() != itb.Get())
      {
        std::string s("Values not equal");
        DREAM3D_TEST_THROW_EXCEPTION(s)
      }
    }
  }

  int CreateImageToDream3DDataFilters()
  {
    typedef itk::InPlaceImageToDream3DDataFilter<float, 3> toDream3D_float_3_Type;
    toDream3D_float_3_Type::Pointer toDream3D_float_3 = toDream3D_float_3_Type::New();
    typedef itk::InPlaceImageToDream3DDataFilter<itk::RGBAPixel<float>, 3> toDream3D_rgba_float_3_Type;
    toDream3D_rgba_float_3_Type::Pointer toDream3D_rgba_float_3 = toDream3D_rgba_float_3_Type::New();
    typedef itk::InPlaceImageToDream3DDataFilter<itk::RGBAPixel<unsigned char>, 3> toDream3D_rgba_uchar_3_Type;
    toDream3D_rgba_uchar_3_Type::Pointer toDream3D_rgba_uchar_3 = toDream3D_rgba_uchar_3_Type::New();

    typedef itk::InPlaceImageToDream3DDataFilter<itk::RGBPixel<float>, 3> toDream3D_rgb_float_3_Type;
    toDream3D_rgb_float_3_Type::Pointer toDream3D_rgb_float_3 = toDream3D_rgb_float_3_Type::New();

    return EXIT_SUCCESS;
  }

  int GetComponentsDimensions()
  {
    QVector<size_t> cDims;
    QString error;
    cDims = ITKDream3DHelper::GetComponentsDimensions<unsigned char>();
    if(cDims.size() != 1 || cDims[0] != 1)
    {
      error = QString("Scalar images should have a component dimension of [1]. Found components of size %1 with first value %2").arg(cDims.size()).arg(cDims[0]);
      DREAM3D_TEST_THROW_EXCEPTION(error.toStdString());
    }
    cDims = ITKDream3DHelper::GetComponentsDimensions<itk::RGBPixel<unsigned char>>();
    if(cDims.size() != 1 || cDims[0] != 3)
    {
      error = QString("RGB images should have a component dimension of [3]. Found components of size %1 with first value %2").arg(cDims.size()).arg(cDims[0]);
      DREAM3D_TEST_THROW_EXCEPTION(error.toStdString());
    }
    cDims = ITKDream3DHelper::GetComponentsDimensions<itk::RGBAPixel<unsigned char>>();
    if(cDims.size() != 1 || cDims[0] != 4)
    {
      error = QString("RGBA images should have a component dimension of [4]. Found components of size %1 with first value %2").arg(cDims.size()).arg(cDims[0]);
      DREAM3D_TEST_THROW_EXCEPTION(error.toStdString());
    }
    const unsigned int vecDim = 3;
    cDims = ITKDream3DHelper::GetComponentsDimensions<itk::Vector<unsigned char, vecDim>>();
    if(cDims.size() != vecDim || cDims[0] != 1 || cDims[1] != 1 || cDims[2] != 1)
    {
      error = QString("Vector images should have a component dimension of (%1,1). Found components of size %2 with first value %3").arg(vecDim).arg(cDims.size()).arg(cDims[0]);
      DREAM3D_TEST_THROW_EXCEPTION(error.toStdString());
    }
    return EXIT_SUCCESS;
  }

  // -----------------------------------------------------------------------------
  //
  // -----------------------------------------------------------------------------
  void operator()()
  {
    int err = EXIT_SUCCESS;

    DREAM3D_REGISTER_TEST(CreateDream3DDataToImageFilters());
    DREAM3D_REGISTER_TEST(CreateImageToDream3DDataFilters());
    DREAM3D_REGISTER_TEST(TestConvertITKToDream3DAndBackToITKImages());
    DREAM3D_REGISTER_TEST(GetComponentsDimensions());
  }

private:
  ITKImageProcessingImageTest(const ITKImageProcessingImageTest&); // Copy Constructor Not Implemented
  void operator=(const ITKImageProcessingImageTest&);              // Operator '=' Not Implemented
};
