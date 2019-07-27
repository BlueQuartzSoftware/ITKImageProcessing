/* ============================================================================
 * Copyright (c) 2009-2019 BlueQuartz Software, LLC
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
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
#pragma once


#include "SIMPLib/SIMPLib.h"
#include "SIMPLib/Common/SIMPLibSetGetMacros.h"

#include "ITKImageProcessing/ITKImageProcessingDLLExport.h"
#include "ITKImageProcessing/ZeissXml/ZeissMetaEntry.h"


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
class ITKImageProcessing_EXPORT ZeissMetaFactory
{
  public:
    SIMPL_SHARED_POINTERS(ZeissMetaFactory);

    virtual ~ZeissMetaFactory() = default;

    virtual AbstractZeissMetaData::Pointer createMetaEntry() = 0;
  protected:
    ZeissMetaFactory() = default;

  public:
    ZeissMetaFactory(const ZeissMetaFactory&) = delete; // Copy Constructor Not Implemented
    ZeissMetaFactory(ZeissMetaFactory&&) = delete;      // Move Constructor Not Implemented
    ZeissMetaFactory& operator=(const ZeissMetaFactory&) = delete; // Copy Assignment Not Implemented
    ZeissMetaFactory& operator=(ZeissMetaFactory&&) = delete;      // Move Assignment Not Implemented
};

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
class ITKImageProcessing_EXPORT Int32ZeissMetaFactory : public ZeissMetaFactory
{
  public:
    SIMPL_SHARED_POINTERS(Int32ZeissMetaFactory);
    SIMPL_NEW_SUPERCLASS(Int32ZeissMetaFactory, ZeissMetaFactory);

    AbstractZeissMetaData::Pointer createMetaEntry() override;

  protected:
    Int32ZeissMetaFactory() = default;

  public:
    Int32ZeissMetaFactory(const Int32ZeissMetaFactory&) = delete; // Copy Constructor Not Implemented
    Int32ZeissMetaFactory(Int32ZeissMetaFactory&&) = delete;      // Move Constructor Not Implemented
    Int32ZeissMetaFactory& operator=(const Int32ZeissMetaFactory&) = delete; // Copy Assignment Not Implemented
    Int32ZeissMetaFactory& operator=(Int32ZeissMetaFactory&&) = delete;      // Move Assignment Not Implemented
};

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
class ITKImageProcessing_EXPORT Int64ZeissMetaFactory : public ZeissMetaFactory
{
  public:
    SIMPL_SHARED_POINTERS(Int64ZeissMetaFactory);
    SIMPL_NEW_SUPERCLASS(Int64ZeissMetaFactory, ZeissMetaFactory);

    AbstractZeissMetaData::Pointer createMetaEntry() override;

  protected:
    Int64ZeissMetaFactory() = default;

  public:
    Int64ZeissMetaFactory(const Int64ZeissMetaFactory&) = delete; // Copy Constructor Not Implemented
    Int64ZeissMetaFactory(Int64ZeissMetaFactory&&) = delete;      // Move Constructor Not Implemented
    Int64ZeissMetaFactory& operator=(const Int64ZeissMetaFactory&) = delete; // Copy Assignment Not Implemented
    Int64ZeissMetaFactory& operator=(Int64ZeissMetaFactory&&) = delete;      // Move Assignment Not Implemented
};



// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
class ITKImageProcessing_EXPORT FloatZeissMetaFactory : public ZeissMetaFactory
{
  public:
    SIMPL_SHARED_POINTERS(FloatZeissMetaFactory);
    SIMPL_NEW_SUPERCLASS(FloatZeissMetaFactory, ZeissMetaFactory);

    AbstractZeissMetaData::Pointer createMetaEntry() override;

  protected:
    FloatZeissMetaFactory() = default;

  public:
    FloatZeissMetaFactory(const FloatZeissMetaFactory&) = delete; // Copy Constructor Not Implemented
    FloatZeissMetaFactory(FloatZeissMetaFactory&&) = delete;      // Move Constructor Not Implemented
    FloatZeissMetaFactory& operator=(const FloatZeissMetaFactory&) = delete; // Copy Assignment Not Implemented
    FloatZeissMetaFactory& operator=(FloatZeissMetaFactory&&) = delete;      // Move Assignment Not Implemented
};

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
class ITKImageProcessing_EXPORT StringZeissMetaFactory : public ZeissMetaFactory
{
  public:
    SIMPL_SHARED_POINTERS(StringZeissMetaFactory);
    SIMPL_NEW_SUPERCLASS(StringZeissMetaFactory, ZeissMetaFactory);

    AbstractZeissMetaData::Pointer createMetaEntry() override;

  protected:
    StringZeissMetaFactory() = default;

  public:
    StringZeissMetaFactory(const StringZeissMetaFactory&) = delete; // Copy Constructor Not Implemented
    StringZeissMetaFactory(StringZeissMetaFactory&&) = delete;      // Move Constructor Not Implemented
    StringZeissMetaFactory& operator=(const StringZeissMetaFactory&) = delete; // Copy Assignment Not Implemented
    StringZeissMetaFactory& operator=(StringZeissMetaFactory&&) = delete;      // Move Assignment Not Implemented
};
