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

#include "FFTDewarpHelper.h"

// ----------------------------------------------------------------------------
FFTDewarpHelper::PixelIndex FFTDewarpHelper::pixelIndex(int64_t x, int64_t y)
{
  return PixelIndex{ x, y };
}

// ----------------------------------------------------------------------------
FFTDewarpHelper::PixelIndex FFTDewarpHelper::pixelIndex(size_t x, size_t y)
{
  return PixelIndex{ static_cast<int64_t>(x), static_cast<int64_t>(y) };
}

// ----------------------------------------------------------------------------
FFTDewarpHelper::PixelIndex FFTDewarpHelper::pixelIndex(double x, double y)
{
  int64_t xi = static_cast<int64_t>(std::floor(x));
  int64_t yi = static_cast<int64_t>(std::floor(y));
  return PixelIndex{ xi, yi };
}

// ----------------------------------------------------------------------------
FFTDewarpHelper::PixelIndex FFTDewarpHelper::getOldIndex(PixelIndex newIndex, PixelIndex offset, const ParametersType& parameters)
{
  PixelIndex oldCoords;
  oldCoords[0] = px(newIndex, offset, parameters);
  oldCoords[1] = py(newIndex, offset, parameters);
  return oldCoords;
}

// ----------------------------------------------------------------------------
int64_t FFTDewarpHelper::px(PixelIndex newIndex, PixelIndex offset, const ParametersType& parameters)
{
  const double newXPrime = newIndex[0] - offset[0];
  const double newYPrime = newIndex[1] - offset[1];

  double oldXPrime = 0.0;
  oldXPrime += parameters[0] * newXPrime;
  oldXPrime += parameters[1] * newYPrime;
  oldXPrime += parameters[2] * newXPrime * newXPrime;
  oldXPrime += parameters[3] * newYPrime * newYPrime;
  oldXPrime += parameters[4] * newXPrime * newYPrime;
  oldXPrime += parameters[5] * newXPrime * newXPrime * newYPrime;
  oldXPrime += parameters[6] * newXPrime * newYPrime * newYPrime;

  return static_cast<int64_t>(std::floor(oldXPrime + offset[0]));
}

// ----------------------------------------------------------------------------
int64_t FFTDewarpHelper::py(PixelIndex newIndex, PixelIndex offset, const ParametersType& parameters)
{
  const double newXPrime = newIndex[0] - offset[0];
  const double newYPrime = newIndex[1] - offset[1];

  double oldYPrime = 0.0;
  // const size_t yOffset = getReqPartialParameterSize();
  oldYPrime += parameters[7] * newXPrime;
  oldYPrime += parameters[8] * newYPrime;
  oldYPrime += parameters[9] * newXPrime * newXPrime;
  oldYPrime += parameters[10] * newYPrime * newYPrime;
  oldYPrime += parameters[11] * newXPrime * newYPrime;
  oldYPrime += parameters[12] * newXPrime * newXPrime * newYPrime;
  oldYPrime += parameters[13] * newXPrime * newYPrime * newYPrime;

  return static_cast<int64_t>(std::floor(oldYPrime + offset[1]));
}
