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
#include "DetermineStitching.h"


#include "itkImage.h"

#include "SIMPLib/ITK/itkBridge.h"

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
DetermineStitching::DetermineStitching() = default;

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
DetermineStitching::~DetermineStitching() = default;

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//Finds the max value in a vector ... possibly redunant since it is one line ...
template<typename T>
T FindMaxValue(QVector<T> inputVector)
{
  typename QVector<T>::iterator it = std::max_element(inputVector.begin(), inputVector.end());
  return *it;
}

//This helper function takes the tile list and creates a new vector that orders the tiles as though they are in comb order. So a tile set collected
//in a comb fashion (along the rows first) will have the values in the new vector match the original index. This is a helper so that we can always stitch the
//tiles the same way regardless of how they were collected.

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
QVector<size_t> DetermineStitching::ReturnIndexForCombOrder(QVector<qint32> xTileList, QVector<qint32> yTileList, size_t numXtiles, size_t numYtiles)
{

  QVector<size_t> newIndices(xTileList.size(), 0);
  size_t count;

  for (size_t iter = 0; iter < xTileList.size(); iter++)
  {
    count = 0;
    for (size_t j = 0; j < numYtiles; j++)
    {
      for (size_t i = 0; i < numXtiles; i++)
      {
        if (xTileList[iter] == i && yTileList[iter] == j)
        {
          newIndices[iter] = count;
        }
        count ++;
      }
    }
  }

  return newIndices;

}


// This function takes the input mode (which is set in the ui) and based on it's value determines how exactly the
// order of images should be layed out
// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
QVector<size_t> DetermineStitching::ReturnProperIndex(int InputMode, int xDims, int yDims)
{
  int numTiles = xDims * yDims;
  QVector<size_t> newIndices(numTiles);
  // Current information about the combo box on the UI:
  // Index 0: Top Down Comb
  // Index 1: Top Down Snake

  // The goal is to get this information in Top Down Comb order
  int counter = 0;
  int linestart = 0;
  switch (InputMode)
  {
  case 0:
    // If it's already in top down comb order then we don't have a problem. Return a vector filled with values from 0 - numtiles
    for (int i = 0; i < numTiles; i++)
    {
      newIndices[i] = i;
    }
    break;
  case 1:
    // Column comb order
    counter = 0;
    for (int x = 0; x < xDims; x++)
    {
      for (int y = 0; y < yDims; y++)
      {
        newIndices[x + y*xDims] = counter;
        counter++;
      }

    }
    break;
  case 2:
    // We're in top down snake order
    // for a 3 x 3 matrix it should look like
    // A B C
    // F E D
    // G H I
    for (int y = 0; y < yDims; y++)
    {
      for (int x = 0; x < xDims; x++)
      {
        if (y % 2 == 0) // If it's an even row, apply Comb order
        {
          newIndices[(x + (y*xDims))] = x + (y*xDims);
        }
        else // If it's an odd row apply reverse comb order
        {
          /*
          Matrix' Below: Targeted - Target With Numbers - Index Numbers

          A B		0 1		0 1
          D C		3 2		2 3
          E F		4 5		4 5
          H G		7 6		6 7

          x = 0;				Mess around with these variables at will to make sure you understand how this equation works
          y = 1;
          xDims = 2;

          Indices[0+(1*2)] = (((1+1) * 2) - 0) - 1		==		Indices[2] = 3
          */
          newIndices[x + y*xDims] = (((1 + y) * xDims) - x) - 1;
        }

      }
    }

    break;

  case 3:
    // Column Snake order
    counter = 0;
    for (int x = 0; x < xDims; x++)
    {
      for (int y = 0; y < yDims; y++)
      {
        if (y % 2 == 0)
        {
          newIndices[counter] = x + y*xDims;

        }
        else
        {
          linestart = (yDims * (xDims - 1)) + y;
          newIndices[linestart - (x*yDims)] = x + y*xDims;

        }

        counter++;
      }
    }

    break;
  default:
    // If it's not an option fill in a comb order and hope for the best
    for (int i = 0; i < numTiles; i++)
    {
      newIndices[i] = i;
    }
    break;


  }

  return newIndices;
}
