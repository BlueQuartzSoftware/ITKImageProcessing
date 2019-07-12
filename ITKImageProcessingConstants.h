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
#pragma once

#include <QtCore/QString>

#include "SIMPLib/DataArrays/DataArray.hpp"

#define ZEISS_HDF_SUPPORT 1

/**
 * @brief This namespace is used to define some Constants for the plugin itself.
 */
namespace ITKImageProcessingConstants
{
const QString ITKImageProcessingPluginFile("ITKImageProcessingPlugin");
const QString ITKImageProcessingPluginDisplayName("ITKImageProcessing Plugin");
const QString ITKImageProcessingBaseName("ITKImageProcessing");

namespace FilterGroups
{
const QString ITKImageProcessingFilters("ITKImageProcessing");
}

// define pixels for dream3d variable types
using Int8PixelType = int8_t;
using UInt8PixelType = uint8_t;
using Int16PixelType = int16_t;
using UInt16PixelType = uint16_t;
using Int32PixelType = int32_t;
using UInt32PixelType = uint32_t;
using Int64PixelType = int64_t;
using UInt64PixelType = uint64_t;
using FloatPixelType = float;
using DoublePixelType = double;

// define default pixel type
#if ImageProcessing_BitDepth == 8
using DefaultPixelType = UInt8PixelType;
using DefaultArrayType = DataArray<DefaultPixelType>;
#elif ImageProcessing_BitDepth == 16
using DefaultPixelType = UInt16PixelType;
using DefaultArrayType = UInt16ArrayType;
#elif ImageProcessing_BitDepth == 32
using DefaultPixelType = FloatPixelType;
using DefaultArrayType = FloatArrayType;
#else
using DefaultPixelType = UInt8PixelType;
using DefaultArrayType = UInt8ArrayType;
#endif

namespace BlueQuartz
{
const QString VendorName("BlueQuartz Software, LLC");
const QString URL("http://www.bluequartz.net");
const QString Copyright("(C) 2016-2019 BlueQuartz Software, LLC");
} // namespace BlueQuartz

namespace FilterGroups
{
const QString ZeissImportFilters("Zeiss Import");
}

namespace Xml
{
static const QString Tags("Tags");
static const QString Count("Count");
static const QString Scaling("Scaling");
} // namespace Xml

namespace PolynomialOrder
{
const unsigned int NumConsts2ndOrder = 6;
}

} // namespace ITKImageProcessingConstants

namespace Zeiss
{
namespace ZenXml
{
const QString ExportDocument("ExportDocument");
const QString Image("Image");
const QString Filename("Filename");
const QString Bounds("Bounds");
const QString StartX("StartX");
const QString SizeX("SizeX");
const QString StartY("StartY");
const QString SizeY("SizeY");
const QString StartC("StartC");
const QString StartS("StartS");
const QString StartB("StartB");
const QString StartM("StartM");
const QString Zoom("Zoom");
const QString Z("Z");
const QString T("T");
} // namespace ZenXml
} // namespace Zeiss

namespace ITKImageProcessing
{

namespace Montage
{
const QString k_AMName("CellAM");
const QString k_AAName("ImageData");
const QString k_AAGrayScaleName("GrayImageData");

const QString k_AttributeArrayNames("AttributeArrayNames");
const QString k_DataContaineNameDefaultName("Mosaic");
const QString k_DataContainerPrefixDefaultName("Mosaic_");
const QString k_TileAttributeMatrixDefaultName("Tile Data");
const QString k_TileDataArrayDefaultName("Image");
const QString k_GrayScaleTempArrayName("gray_scale_temp");

const QString k_MontageDataContainerDefaultName("Mosaic");
const QString k_MontageAttributeMatrixDefaultName("Mosaic Data");
const QString k_MontageDataArrayDefaultName("Image");

const QString k_TileCorrectedDefaultName("Corrected Image");

const QString k_BackgroundDataContainerDefaultName("Illumination Correction");
const QString k_BackgroundAttributeMatrixDefaultName("Illumination Data");
const QString k_BackgroundDataArrayDefaultName("Image");
const QString k_MontageInfoReplaceKeyword("@UPDATE_ROW_COLUMN@");
} // namespace Montage

} // namespace ITKImageProcessing
/**
 * @brief Use this namespace to define any custom GUI widgets that collect FilterParameters
 * for a filter. Do NOT define general reusable widgets here.
 */
namespace FilterParameterWidgetType
{
/* const QString SomeCustomWidget("SomeCustomWidget"); */
}
