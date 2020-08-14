/* ============================================================================
 * Copyright (c) 2019 BlueQuartz Software, LLC
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
 * Neither the names of any of the BlueQuartz Software contributors
 * may be used to endorse or promote products derived from this software without
 * specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS AS IS
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

#include <array>

#include <itkConfigure.h>
#define SIMPL_ITK_VERSION_CHECK (ITK_VERSION_MAJOR == 4)
#if SIMPL_ITK_VERSION_CHECK
#ifdef __clang__
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wself-assign-field"
#endif
#endif

#include <itkSingleValuedCostFunction.h>

namespace FFTDewarpHelper
{
using PixelIndex = std::array<int64_t, 2>;
using ParametersType = itk::SingleValuedCostFunction::ParametersType;

/**
 * @brief Constructs a PixelTypei from x and y parameters
 * @param x
 * @param y
 * @return
 */
PixelIndex pixelIndex(int64_t x, int64_t y);
PixelIndex pixelIndex(size_t x, size_t y);
PixelIndex pixelIndex(double x, double y);

/**
 * @brief Returns the number of parameters for either X or Y
 * @return
 */
constexpr uint32_t getReqPartialParameterSize()
{
  return 7;
}

/**
 * @brief Returns the total number of parameters for both X and Y.
 * @return
 */
constexpr uint32_t getReqParameterSize()
{
  return 14;
}

/**
 * @brief Returns the old pixel index from which to extract the value for the given parameters.
 * @param
 */
PixelIndex getOldIndex(PixelIndex newIndex, PixelIndex offset, const ParametersType& parameters);
int64_t px(PixelIndex newIndex, PixelIndex offset, const ParametersType& parameters);
int64_t py(PixelIndex newIndex, PixelIndex offset, const ParametersType& parameters);
} // namespace FFTDewarpHelper

#if SIMPL_ITK_VERSION_CHECK
#ifdef __clang__
#pragma clang diagnostic pop
#endif
#endif
#undef SIMPL_ITK_VERSION_CHECK
