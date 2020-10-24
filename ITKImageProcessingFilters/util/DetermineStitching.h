/* ============================================================================
 * Copyright (c) 2014 Michael A. Jackson (BlueQuartz Software)
 * All rights reserved.
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
 * Neither the name of Michael A. Jackson, BlueQuartz Software nor the names of
 * its contributors may be used to endorse or promote products derived from this
 * software without specific prior written permission.
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
 *
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
#pragma once

#include <itkConfigure.h>
#define SIMPL_ITK_VERSION_CHECK (ITK_VERSION_MAJOR == 4)
#if SIMPL_ITK_VERSION_CHECK
#ifdef __clang__
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wself-assign-field"
#endif
#endif

#include <vector>

#include <QtCore/QDir>
#include <QtCore/QString>

#include "SIMPLib/SIMPLib.h"
#include "SIMPLib/Common/SIMPLibSetGetMacros.h"
#include "SIMPLib/DataArrays/DataArray.hpp"
#include "SIMPLib/DataArrays/IDataArray.h"
#include "SIMPLib/Filtering/AbstractFilter.h"
#include "SIMPLib/ITK/itkBridge.h"
#include "SIMPLib/ITK/itkSupportConstants.h"

#include "itkChangeInformationImageFilter.h"
#include "itkImageFileWriter.h"
#include "itkMaskedFFTNormalizedCorrelationImageFilter.h"
#include "itkMinimumMaximumImageCalculator.h"
#include "itkRegionOfInterestImageFilter.h"

#define WRITE_DEBUG_IMAGES 0

/**
 * @brief The DetermineStitching class
 */
class DetermineStitching
{
public:
  virtual ~DetermineStitching();

  /**
   * @brief FindGlobalOrigins
   * @param xTileCount
   * @param yTileCount
   * @param ImportMode
   * @param overlapPer
   * @param dataArrayList
   * @param udims
   * @param sampleOrigin
   * @param spacing
   * @return
   */
  template <typename T, unsigned int ImageDimension>
  static FloatArrayType::Pointer FindGlobalOrigins(AbstractFilter* filter, int xTileCount, int yTileCount, int importMode, float overlapPer,
                                                   const std::vector<typename DataArray<T>::Pointer>& dataArrayList, const SizeVec3Type& udims, const FloatVec3Type& sampleOrigin,
                                                   const FloatVec3Type& spacing, const QStringList& dataContainerNames)
  {
    // The assumption is that we are working on a 2D image where the XY dims are in the 0 and 1 index of the udims variable
    size_t totalPoints = udims[0] * udims[1];

    std::vector<size_t> cDims(1, 2); // a dimension for the xvalues and one for the y values
    std::vector<size_t> tDims(1);

    int numTiles = xTileCount * yTileCount;
    tDims[0] = numTiles;

    FloatArrayType::Pointer xyStitchedGlobalListPtr = FloatArrayType::CreateArray(tDims, cDims, "xyGlobalList", true);
    FloatArrayType::Pointer xyStitchedGlobalListPtr_orig = FloatArrayType::CreateArray(tDims, cDims, "xyGlobalList_orig", true);

    int32_t numXtiles = xTileCount;

    // Make sure the combIndexList is large enough to store every value
    std::vector<size_t> combIndexList(numTiles);

    combIndexList = ReturnProperIndex(importMode, xTileCount, yTileCount);

    using PixelType = T;
    using ImageType = itk::Image<PixelType, ImageDimension>;
    using ImportFilterType = itk::ImportImageFilter<PixelType, ImageDimension>;

    ImageType* currentImage = nullptr;
    ImageType* leftImage = nullptr;
    ImageType* aboveImage = nullptr;

    typename ImportFilterType::Pointer importFilter;
    typename ImportFilterType::Pointer importFilter2;

    std::vector<float> cropSpecsIm1Im2(12, 0);
    std::vector<float> newXYOrigin(2, 0);
    std::vector<float> newXYOrigin2(2, 0);

    // set the stitched global coordinates of the first tile to the top left corner
    xyStitchedGlobalListPtr->setValue(0, 0);
    xyStitchedGlobalListPtr->setValue(1, 0);
    xyStitchedGlobalListPtr_orig->setValue(0, 0);
    xyStitchedGlobalListPtr_orig->setValue(1, 0);

    // helper variables to store previous stitched global values
    float previousXleft = 0;
    float previousYleft = 0;

    float previousXtop = 0;
    float previousYtop = 0;

    float newXfromleft = 0;
    float newYfromleft = 0;

    float newXfromtop = 0;
    float newYfromtop = 0;

    using ItkBridgeType = ItkBridge<T>;

    // IMPORTANT:
    // This method is set up a bit weird so hopefully this will help
    // cropSpecIm1Im2 is a rather important variable so it's good to understand what each value means
    // The first 6 values in the array are the crop origin that we'll be looking at (starts at the top left)
    // The last 6 values in the array are the crop dimensions (size) that we'll look at (goes down to bottom right)
    // The CropAndCrossCorrelate method crops the images and compares them with each other looking for a spike in similar values (a simplified explanation)
    // The loop compares 2 images at once, so if you have a 3 x 3 image, the loop will only go through 8 times. You'll compare the second image with the first image
    //		first thing so the first if statement will only trigger twice before triggering the second if statement

    for(size_t i = 1; i < combIndexList.size(); i++)
    {
      if(filter->getCancel())
      {
        break;
      }

      filter->notifyStatusMessage(QString("Refining Origin For Image %1").arg(i));
      if(i < numXtiles) // if the image is in the top row of images, we need only the image to the left
      {
#if WRITE_DEBUG_IMAGES
        qDebug() << "LEFT: DataContainer: " << dataContainerNames[combIndexList[i]] << ", " << dataContainerNames[combIndexList[i - 1]];
        qDebug() << "\t" << combIndexList[i] << "\t" << combIndexList[i - 1];
#endif
        importFilter = ItkBridgeType::template Dream3DtoITKImportFilterDataArray<PixelType>(totalPoints, udims, sampleOrigin, spacing, dataArrayList[combIndexList[i]]);
        currentImage = importFilter->GetOutput();
        importFilter2 = ItkBridgeType::template Dream3DtoITKImportFilterDataArray<PixelType>(totalPoints, udims, sampleOrigin, spacing, dataArrayList[combIndexList[i - 1]]);
        leftImage = importFilter2->GetOutput();

        // Determine the windows to be cross correlated depending on the rough overlap as found from the global coordinates
        cropSpecsIm1Im2[0] =
            udims[0] -
            (udims[0] * (overlapPer / 100)); // Width of the image * the percentage (say 20%) = the size of the crop we're looking at. Subtract that from the width of the image and you have the origin
        cropSpecsIm1Im2[1] = 0;              // left image Y Origin
        cropSpecsIm1Im2[2] = 0;              // left image Z Origin
        cropSpecsIm1Im2[3] = 0;              // current image X Origin
        cropSpecsIm1Im2[4] = 0;              // current image Y Origin
        cropSpecsIm1Im2[5] = 0;              // current image Z Origin

        cropSpecsIm1Im2[6] = udims[0] * (overlapPer / 100); // left image X Size
        cropSpecsIm1Im2[7] = udims[1];                      // left image Y Size
        cropSpecsIm1Im2[8] = 1;                             // left image Z Size
        cropSpecsIm1Im2[9] = udims[0] * (overlapPer / 100); // current image X Size
        cropSpecsIm1Im2[10] = udims[1];                     // current image Y Size
        cropSpecsIm1Im2[11] = 1;                            // current image Z Size

        // Cross correlate the image windows and return the local shifts between the two images
        newXYOrigin = CropAndCrossCorrelate(cropSpecsIm1Im2, currentImage, leftImage);

        previousXleft = xyStitchedGlobalListPtr->getValue(2 * (i - 1));
        previousYleft = xyStitchedGlobalListPtr->getValue(2 * (i - 1) + 1);

        newXfromleft = previousXleft + cropSpecsIm1Im2[0] + newXYOrigin[0];
        newYfromleft = previousYleft + newXYOrigin[1];

        xyStitchedGlobalListPtr->setValue(2 * i, newXfromleft);
        xyStitchedGlobalListPtr->setValue(2 * i + 1, newYfromleft);
      }
      else if(i % numXtiles == 0) // if the image is in the first (left most) column of images, we only need the top image
      {
#if WRITE_DEBUG_IMAGES
        qDebug() << "TOP: DataContainer: " << dataContainerNames[combIndexList[i]] << ", " << dataContainerNames[combIndexList[i - numXtiles]];
        qDebug() << "\t" << combIndexList[i] << "\t" << combIndexList[i - numXtiles];
#endif
        importFilter = ItkBridgeType::template Dream3DtoITKImportFilterDataArray<PixelType>(totalPoints, udims, sampleOrigin, spacing, dataArrayList[combIndexList[i]]);
        currentImage = importFilter->GetOutput();
        importFilter2 = ItkBridgeType::template Dream3DtoITKImportFilterDataArray<PixelType>(totalPoints, udims, sampleOrigin, spacing, dataArrayList[combIndexList[i - numXtiles]]);
        aboveImage = importFilter2->GetOutput();

        // Determine the windows to be cross correlated depending on the rough overlap as found from the global coordinates
        cropSpecsIm1Im2[0] = 0; // top image X Origin
        cropSpecsIm1Im2[1] =
            udims[1] - (udims[1] * (overlapPer / 100)); // yGlobCoordsList[combIndexList[i]] - xyStitchedGlobalListPtr->getValue(2*(i-numXtiles)+1) - yGlobCoordsList[0]; //top image Y Origin
        cropSpecsIm1Im2[2] = 0;                         // top image Z Origin
        cropSpecsIm1Im2[3] = 0;                         // current image X Origin
        cropSpecsIm1Im2[4] = 0;                         // current image Y Origin
        cropSpecsIm1Im2[5] = 0;                         // current image Z Origin

        cropSpecsIm1Im2[6] = udims[0];                       // top image X Size
        cropSpecsIm1Im2[7] = udims[1] * (overlapPer / 100);  // top image Y Size
        cropSpecsIm1Im2[8] = 1;                              // top image Z Size
        cropSpecsIm1Im2[9] = udims[0];                       // current image X Size
        cropSpecsIm1Im2[10] = udims[1] * (overlapPer / 100); // current image Y Size
        cropSpecsIm1Im2[11] = 1;                             // current image Z Size

        // Cross correlate the image windows and return the local shifts between the two images
        newXYOrigin = CropAndCrossCorrelate(cropSpecsIm1Im2, currentImage, aboveImage);

        previousXtop = xyStitchedGlobalListPtr->getValue(2 * (i - numXtiles));
        previousYtop = xyStitchedGlobalListPtr->getValue(2 * (i - numXtiles) + 1);

        // Add the local shifts to the preivous global value to get the current stitched global shift
        newXfromtop = previousXtop + newXYOrigin[0];
        newYfromtop = previousYtop + newXYOrigin[1] + cropSpecsIm1Im2[1];

        xyStitchedGlobalListPtr->setValue(2 * i, newXfromtop);
        xyStitchedGlobalListPtr->setValue(2 * i + 1, newYfromtop);
      }
      else // for all other images, we need to match to the top and the left
      {
#if WRITE_DEBUG_IMAGES
        qDebug() << "TOP/LEFT: DataContainer: " << dataContainerNames[combIndexList[i]] << ", " << dataContainerNames[combIndexList[i - numXtiles]];
        qDebug() << "\t" << combIndexList[i] << "\t" << combIndexList[i - numXtiles];
#endif
        /// TOP IMAGE FIRST
        importFilter = ItkBridgeType::template Dream3DtoITKImportFilterDataArray<PixelType>(totalPoints, udims, sampleOrigin, spacing, dataArrayList[combIndexList[i]]);
        currentImage = importFilter->GetOutput();
        importFilter2 = ItkBridgeType::template Dream3DtoITKImportFilterDataArray<PixelType>(totalPoints, udims, sampleOrigin, spacing, dataArrayList[combIndexList[i - numXtiles]]);
        aboveImage = importFilter2->GetOutput();

        // Determine the windows to be cross correlated depending on the rough overlap as found from the global coordinates
        cropSpecsIm1Im2[0] = 0; // top image X Origin
        cropSpecsIm1Im2[1] =
            udims[1] - (udims[1] * (overlapPer / 100)); // yGlobCoordsList[combIndexList[i]] - xyStitchedGlobalListPtr->getValue(2*(i-numXtiles)+1) - yGlobCoordsList[0]; //top image Y Origin
        cropSpecsIm1Im2[2] = 0;                         // top image Z Origin
        cropSpecsIm1Im2[3] = 0;                         // current image X Origin
        cropSpecsIm1Im2[4] = 0;                         // current image Y Origin
        cropSpecsIm1Im2[5] = 0;                         // current image Z Origin

        cropSpecsIm1Im2[6] = udims[0];                       // top image X Size
        cropSpecsIm1Im2[7] = udims[1] * (overlapPer / 100);  // top image Y Size
        cropSpecsIm1Im2[8] = 1;                              // top image Z Size
        cropSpecsIm1Im2[9] = udims[0];                       // current image X Size
        cropSpecsIm1Im2[10] = udims[1] * (overlapPer / 100); // current image Y Size
        cropSpecsIm1Im2[11] = 1;                             // current image Z Size

        // Cross correlate the image windows and return the local shifts between the two images
        newXYOrigin2 = CropAndCrossCorrelate(cropSpecsIm1Im2, currentImage, aboveImage);

        previousXtop = xyStitchedGlobalListPtr->getValue(2 * (i - numXtiles));
        previousYtop = xyStitchedGlobalListPtr->getValue(2 * (i - numXtiles) + 1);

        // Add the local shifts to the preivous global value to get the current stitched global shift
        newXfromtop = previousXtop + newXYOrigin2[0];
        newYfromtop = previousYtop + newXYOrigin2[1] + cropSpecsIm1Im2[1];

#if WRITE_DEBUG_IMAGES
        qDebug() << "TOP/LEFT: DataContainer: " << dataContainerNames[combIndexList[i]] << ", " << dataContainerNames[combIndexList[i - 1]];
        qDebug() << "\t" << combIndexList[i] << "\t" << combIndexList[i - 1];
#endif
        // BOTTOM IMAGE NEXT
        importFilter = ITKUtilitiesType::template Dream3DtoITKImportFilterDataArray<PixelType>(totalPoints, udims, sampleOrigin, spacing, dataArrayList[combIndexList[i]]);
        currentImage = importFilter->GetOutput();
        importFilter2 = ITKUtilitiesType::template Dream3DtoITKImportFilterDataArray<PixelType>(totalPoints, udims, sampleOrigin, spacing, dataArrayList[combIndexList[i - 1]]);
        leftImage = importFilter2->GetOutput();

        cropSpecsIm1Im2[0] = udims[0] - (udims[0] * (overlapPer / 100)); // xGlobCoordsList[combIndexList[i]] - xyStitchedGlobalListPtr->getValue(2*(i-1)) - xGlobCoordsList[0]; //left image X Origin
        cropSpecsIm1Im2[1] = 0;                                          // left image Y Origin
        cropSpecsIm1Im2[2] = 0;                                          // left image Z Origin
        cropSpecsIm1Im2[3] = 0;                                          // current image X Origin
        cropSpecsIm1Im2[4] = 0;                                          // current image Y Origin
        cropSpecsIm1Im2[5] = 0;                                          // current image Z Origin

        cropSpecsIm1Im2[6] = udims[0] * (overlapPer / 100); // left image X Size
        cropSpecsIm1Im2[7] = udims[1];                      // left image Y Size
        cropSpecsIm1Im2[8] = 1;                             // left image Z Size
        cropSpecsIm1Im2[9] = udims[0] * (overlapPer / 100); // current image X Size
        cropSpecsIm1Im2[10] = udims[1];                     // current image Y Size
        cropSpecsIm1Im2[11] = 1;                            // current image Z Size

        // Cross correlate the image windows and return the local shifts between the two images
        newXYOrigin = CropAndCrossCorrelate(cropSpecsIm1Im2, currentImage, leftImage);

        previousXleft = xyStitchedGlobalListPtr->getValue(2 * (i - 1));
        previousYleft = xyStitchedGlobalListPtr->getValue(2 * (i - 1) + 1);

        // Add the local shifts to the preivous global value to get the current stitched global shift
        newXfromleft = previousXleft + cropSpecsIm1Im2[0] + newXYOrigin[0];
        newYfromleft = previousYleft + newXYOrigin[1];

        // AVERAGE the two new locations
        xyStitchedGlobalListPtr->setValue(2 * i, (newXfromtop + newXfromleft) / 2.0);
        xyStitchedGlobalListPtr->setValue(2 * i + 1, (newYfromtop + newYfromleft) / 2.0);
      }

      // Put the values found in from going in the comb order into another data array which represents the original order the images came in as
      xyStitchedGlobalListPtr_orig->setValue(2 * combIndexList[i], xyStitchedGlobalListPtr->getValue(2 * i));
      xyStitchedGlobalListPtr_orig->setValue(2 * combIndexList[i] + 1, xyStitchedGlobalListPtr->getValue(2 * i + 1));
    }

    return xyStitchedGlobalListPtr_orig;
  }

  /**
   * @brief ReturnIndexForCombOrder
   * @param xTileList
   * @param yTileList
   * @param numXtiles
   * @param numYtiles
   * @return
   */
  static std::vector<size_t> ReturnIndexForCombOrder(QVector<qint32> xTileList, QVector<qint32> yTileList, size_t numXtiles, size_t numYtiles);

  /**
   * @brief ReturnProperIndex
   * @param InputMode
   * @param Numtiles
   */
  static std::vector<size_t> ReturnProperIndex(int InputMode, int xDims, int yDims);

  /**
   * @brief CropAndCrossCorrelate
   * @param cropSpecsIm1Im2
   * @param currentImage
   * @param fixedImage
   * @return
   */
  // clang-format off
  template<typename PixelType, unsigned int ImageDimension>
  static std::vector<float> CropAndCrossCorrelate(std::vector<float> cropSpecsIm1Im2,
                                              itk::Image<PixelType, ImageDimension>* currentImage,
                                              itk::Image<PixelType, ImageDimension>* fixedImage)
  {
    // clang-format on
    std::vector<float> newXYOrigin(3, 0);
    using ImageType = itk::Image<PixelType, ImageDimension>;
    using FloatImageType = itk::Image<float, ImageDimension>;

    //////FIRST IMAGE CROP
    ImageProcessingConstants::UInt8ImageType::RegionType cropRegion;

    cropRegion.SetIndex(0, cropSpecsIm1Im2[0]);
    cropRegion.SetIndex(1, cropSpecsIm1Im2[1]);
    cropRegion.SetIndex(2, cropSpecsIm1Im2[2]);

    cropRegion.SetSize(0, cropSpecsIm1Im2[6]);
    cropRegion.SetSize(1, cropSpecsIm1Im2[7]);
    cropRegion.SetSize(2, cropSpecsIm1Im2[8]);

    // Extract window from first image
    using exImFilterType = itk::RegionOfInterestImageFilter<ImageType, ImageType>;
    typename exImFilterType::Pointer exImfilter = exImFilterType::New();
    exImfilter->SetRegionOfInterest(cropRegion);
    exImfilter->SetInput(fixedImage);
    exImfilter->Update();
    ImageType* fixedImageWindow = exImfilter->GetOutput();

    /////////////////////SECOND IMAGE CROP
    typename ImageType::RegionType cropRegion2;
    cropRegion2.SetIndex(0, cropSpecsIm1Im2[3]);
    cropRegion2.SetIndex(1, cropSpecsIm1Im2[4]);
    cropRegion2.SetIndex(2, cropSpecsIm1Im2[5]);

    cropRegion2.SetSize(0, cropSpecsIm1Im2[9]);
    cropRegion2.SetSize(1, cropSpecsIm1Im2[10]);
    cropRegion2.SetSize(2, cropSpecsIm1Im2[11]);

    // Extract window from second image
    typename exImFilterType::Pointer exImfilter2 = exImFilterType::New();
    exImfilter2->SetRegionOfInterest(cropRegion2);
    exImfilter2->SetInput(currentImage);
    exImfilter2->Update();
    ImageType* currentImageWindow = exImfilter2->GetOutput();

    /// CHANGE THE ORIGIN OF THE IMAGES SO THEY MATCH - the cross correlation filter will not work unless this is done
    using ChangeInfoFilterType = itk::ChangeInformationImageFilter<ImageType>;
    typename ImageType::PointType origin = fixedImageWindow->GetOrigin();
    origin[0] = 0;
    origin[1] = 0;
    origin[2] = 0;

    // First the Fixed image
    typename ChangeInfoFilterType::Pointer changeInfo = ChangeInfoFilterType::New();
    changeInfo->SetInput(fixedImageWindow);
    changeInfo->SetOutputOrigin(origin);
    changeInfo->ChangeOriginOn();
    changeInfo->UpdateOutputInformation();
    ImageType* fixedImageWindow2 = changeInfo->GetOutput();

    // Next the Current Image
    typename ChangeInfoFilterType::Pointer changeInfo2 = ChangeInfoFilterType::New();
    changeInfo2->SetInput(currentImageWindow);
    changeInfo2->SetOutputOrigin(origin);
    changeInfo2->ChangeOriginOn();
    changeInfo2->UpdateOutputInformation();
    ImageType* currentImageWindow2 = changeInfo2->GetOutput();

    /////WRITING THE IMAGES FOR TESTING
#if WRITE_DEBUG_IMAGES
    using WriterType = itk::ImageFileWriter<ImageType>;

    QString imagePath = QDir::homePath() + QDir::separator() + "Desktop" + QDir::separator() + "ImageWindow_Fixed.tif";
    typename WriterType::Pointer writer = WriterType::New();
    writer->SetFileName(imagePath.toLatin1().constData());
    writer->SetInput(fixedImageWindow2);
    writer->Update();

    imagePath = QDir::homePath() + QDir::separator() + "Desktop" + QDir::separator() + "ImageWindow_Moving.tif";
    typename WriterType::Pointer writer2 = WriterType::New();
    writer2->SetFileName(imagePath.toLatin1().constData());
    writer2->SetInput(currentImageWindow2);
    writer2->Update();
#endif

    // CROSS CORRELATE THE 2 WINDOWS.
    // Note: It is much faster to cross correlate the extracted windows than to cross correlate the full windows with a mask applied
    using XCFilterType = itk::MaskedFFTNormalizedCorrelationImageFilter<ImageType, FloatImageType, ImageType>;
    typename XCFilterType::Pointer xCorrFilter = XCFilterType::New();
    xCorrFilter->SetFixedImage(fixedImageWindow2);
    xCorrFilter->SetMovingImage(currentImageWindow2);
    xCorrFilter->SetRequiredFractionOfOverlappingPixels(0.5); // currently require that the windows overlap at least 50percent. Might want to make this a user controlled variable
    xCorrFilter->Update();
    ImageProcessingConstants::FloatImageType* xcoutputImage = xCorrFilter->GetOutput();

    // Create and initialize the calculator
    using MinMaxCalculatorType = itk::MinimumMaximumImageCalculator<FloatImageType>;
    typename MinMaxCalculatorType::Pointer calculator = MinMaxCalculatorType::New();
    calculator->SetImage(xcoutputImage);
    calculator->Compute();

    //    // Return minimum of intensity
    //    float minimumResult = calculator->GetMinimum();
    //    std::cout << "The Minimum intensity value is : " << minimumResult << std::endl;
    //    std::cout << "Its index position is : " << calculator->GetIndexOfMinimum() << std::endl;

    //    float maximumResult = calculator->GetMaximum();
    //    std::cout << "The Maximum intensity value is : " << maximumResult << std::endl;
    //    std::cout << "Its index position is : " << calculator->GetIndexOfMaximum() << std::endl;
    //    std::cout << "Real Max" << xcoutputImage->GetPixel(calculator->GetIndexOfMaximum()) << std::endl;
    if(calculator->GetMinimum() == calculator->GetMaximum())
    {
      newXYOrigin[0] = 0.0f;
      newXYOrigin[1] = 0.0f;
    }
    else
    {
      newXYOrigin[0] = float(calculator->GetIndexOfMaximum()[0]) - float(fixedImageWindow2->GetLargestPossibleRegion().GetSize()[0]);
      newXYOrigin[1] = float(calculator->GetIndexOfMaximum()[1]) - float(fixedImageWindow2->GetLargestPossibleRegion().GetSize()[1]);
    }
    newXYOrigin[2] = calculator->GetMaximum(); // add this for when more than one image pair has to be xcorrelated - want ot use this value to find best fit location

    // xcoutputImage->SetPixel(calculator->GetIndexOfMaximum(), 0); //just testing to make the brightest pixel dark so I could see which one it is
#if WRITE_DEBUG_IMAGES
    using nWriterType = itk::ImageFileWriter<ImageProcessingConstants::FloatImageType>;
    nWriterType::Pointer writer3 = nWriterType::New();
    imagePath = QDir::homePath() + QDir::separator() + "Desktop" + QDir::separator() + "ImageWindow_CrossCorrelation.tif";
    writer3->SetFileName(imagePath.toLatin1().constData());
    writer3->SetInput(xcoutputImage);
    writer3->Update();
#endif
    return newXYOrigin;
  }

protected:
  DetermineStitching();

public:
  DetermineStitching(const DetermineStitching&) = delete;            // Copy Constructor Not Implemented
  DetermineStitching(DetermineStitching&&) = delete;                 // Move Constructor Not Implemented
  DetermineStitching& operator=(const DetermineStitching&) = delete; // Copy Assignment Not Implemented
  DetermineStitching& operator=(DetermineStitching&&) = delete;      // Move Assignment Not Implemented
};

#if SIMPL_ITK_VERSION_CHECK
#ifdef __clang__
#pragma clang diagnostic pop
#endif
#endif
#undef SIMPL_ITK_VERSION_CHECK
