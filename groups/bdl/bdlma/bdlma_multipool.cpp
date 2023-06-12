// bdlma_multipool.cpp                                                -*-C++-*-
#include <bdlma_multipool.h>

#include <bsls_ident.h>
BSLS_IDENT_RCSID(bdlma_multipool_cpp,"$Id$ $CSID$")

#include <bdlma_bufferedsequentialallocator.h>  // for testing only

#include <bdlb_bitutil.h>

#include <bslma_autodestructor.h>
#include <bslma_deallocatorproctor.h>
#include <bslma_default.h>

#include <bsls_assert.h>
#include <bsls_performancehint.h>
#include <bsls_platform.h>

#include <bsl_new.h>
#include <bsl_limits.h>

namespace BloombergLP {
namespace bdlma {

// TYPES
enum {
    k_DEFAULT_NUM_POOLS      = 10,  // default number of pools

    k_DEFAULT_MAX_CHUNK_SIZE = 32,  // default maximum number of blocks per
                                    // chunk

    k_MIN_BLOCK_SIZE         =  8   // minimum block size (in bytes)
};

                             // ---------------
                             // class Multipool
                             // ---------------

// CREATORS
Multipool::Multipool(bslma::Allocator *basicAllocator)
: d_numPools(k_DEFAULT_NUM_POOLS)
, d_blockList(basicAllocator)
, d_allocator_p(bslma::Default::allocator(basicAllocator)) {}

Multipool::Multipool(int               numPools,
                     bslma::Allocator *basicAllocator)
: d_numPools(numPools)
, d_blockList(basicAllocator)
, d_allocator_p(bslma::Default::allocator(basicAllocator))
{
    BSLS_ASSERT(1 <= numPools);
}

Multipool::Multipool(bsls::BlockGrowth::Strategy  growthStrategy,
                     bslma::Allocator            *basicAllocator)
: d_numPools(k_DEFAULT_NUM_POOLS)
, d_blockList(basicAllocator)
, d_allocator_p(bslma::Default::allocator(basicAllocator)) {}

Multipool::Multipool(int                          numPools,
                     bsls::BlockGrowth::Strategy  growthStrategy,
                     bslma::Allocator            *basicAllocator)
: d_numPools(numPools)
, d_blockList(basicAllocator)
, d_allocator_p(bslma::Default::allocator(basicAllocator))
{
    BSLS_ASSERT(1 <= numPools);
}

Multipool::Multipool(int                                numPools,
                     const bsls::BlockGrowth::Strategy *growthStrategyArray,
                     bslma::Allocator                  *basicAllocator)
: d_numPools(numPools)
, d_blockList(basicAllocator)
, d_allocator_p(bslma::Default::allocator(basicAllocator))
{
    BSLS_ASSERT(1 <= numPools);
    BSLS_ASSERT(growthStrategyArray);
}

Multipool::Multipool(int                          numPools,
                     bsls::BlockGrowth::Strategy  growthStrategy,
                     int                          maxBlocksPerChunk,
                     bslma::Allocator            *basicAllocator)
: d_numPools(numPools)
, d_blockList(basicAllocator)
, d_allocator_p(bslma::Default::allocator(basicAllocator))
{
    BSLS_ASSERT(1 <= numPools);
    BSLS_ASSERT(1 <= maxBlocksPerChunk);
}

Multipool::Multipool(int                                numPools,
                     const bsls::BlockGrowth::Strategy *growthStrategyArray,
                     int                                maxBlocksPerChunk,
                     bslma::Allocator                  *basicAllocator)
: d_numPools(numPools)
, d_blockList(basicAllocator)
, d_allocator_p(bslma::Default::allocator(basicAllocator))
{
    BSLS_ASSERT(1 <= numPools);
    BSLS_ASSERT(growthStrategyArray);
    BSLS_ASSERT(1 <= maxBlocksPerChunk);
}

Multipool::Multipool(int                          numPools,
                     bsls::BlockGrowth::Strategy  growthStrategy,
                     const int                   *maxBlocksPerChunkArray,
                     bslma::Allocator            *basicAllocator)
: d_numPools(numPools)
, d_blockList(basicAllocator)
, d_allocator_p(bslma::Default::allocator(basicAllocator))
{
    BSLS_ASSERT(1 <= numPools);
    BSLS_ASSERT(maxBlocksPerChunkArray);
}

Multipool::Multipool(int                                numPools,
                     const bsls::BlockGrowth::Strategy *growthStrategyArray,
                     const int                         *maxBlocksPerChunkArray,
                     bslma::Allocator                  *basicAllocator)
: d_numPools(numPools)
, d_blockList(basicAllocator)
, d_allocator_p(bslma::Default::allocator(basicAllocator))
{
    BSLS_ASSERT(1 <= numPools);
    BSLS_ASSERT(growthStrategyArray);
    BSLS_ASSERT(maxBlocksPerChunkArray);
}

Multipool::~Multipool()
{
    BSLS_ASSERT(d_pools_p);
    BSLS_ASSERT(1 <= d_numPools);
    BSLS_ASSERT(1 <= d_maxBlockSize);
    BSLS_ASSERT(d_allocator_p);

    shim.release();
}

// MANIPULATORS
void *Multipool::allocate(bsls::Types::size_type size)
{
    return shim.allocate(size);
}

void Multipool::deallocate(void *address)
{
    shim.deallocate(address);
}

void Multipool::release()
{
    shim.release();
}

void Multipool::reserveCapacity(bsls::Types::size_type size, int numBlocks)
{
    BSLS_ASSERT(size <= d_maxBlockSize);
    BSLS_ASSERT(0    <= numBlocks);
}

}  // close package namespace
}  // close enterprise namespace

// ----------------------------------------------------------------------------
// Copyright 2016 Bloomberg Finance L.P.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
// ----------------------------- END-OF-FILE ----------------------------------
