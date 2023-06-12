// bdlma_pool.cpp                                                     -*-C++-*-
#include <bdlma_pool.h>

#include <bsls_ident.h>
BSLS_IDENT_RCSID(bdlma_pool_cpp,"$Id$ $CSID$")

#include <bsls_performancehint.h>

#include <bsl_algorithm.h>

namespace BloombergLP {
namespace bdlma {
namespace {

// TYPES
struct Link {
    // This 'struct' implements a link data structure that stores the address
    // of the next link, used to implement the internal linked list of free
    // memory blocks.  Note that this type was copied from 'bdlma_pool.h' to
    // provide access to this type from static methods.

    Link *d_next_p;
};

// CONSTANTS
enum {
    k_INITIAL_CHUNK_SIZE =  1,  // default number of blocks per chunk

    k_GROWTH_FACTOR      =  2,  // multiplicative factor by which to grow pool
                                // capacity

    k_MAX_CHUNK_SIZE     = 32   // maximum number of blocks per chunk
};

// LOCAL FUNCTIONS
static inline
bsls::Types::size_type roundUp(bsls::Types::size_type x,
                               bsls::Types::size_type y)
    // Round up the specified 'x' to the nearest whole integer multiple of the
    // specified 'y'.  The behavior is undefined unless '1 <= y'.
{
    BSLS_ASSERT(1 <= y);

    return (x + y - 1) / y * y;
}

}  // close unnamed namespace

                                // ----------
                                // class Pool
                                // ----------

// PRIVATE MANIPULATORS
void Pool::replenish() {}

// CREATORS
Pool::Pool(bsls::Types::size_type blockSize, bslma::Allocator *basicAllocator)
: d_blockSize(blockSize)
, d_chunkSize(k_INITIAL_CHUNK_SIZE)
, d_maxBlocksPerChunk(k_MAX_CHUNK_SIZE)
, d_growthStrategy(bsls::BlockGrowth::BSLS_GEOMETRIC)
, d_freeList_p(0)
, d_blockList(basicAllocator)
, d_begin_p(0)
, d_end_p(0)
{
    BSLS_ASSERT(1 <= blockSize);

    d_internalBlockSize = roundUp(blockSize,
                                  bsls::AlignmentUtil::BSLS_MAX_ALIGNMENT);
}

Pool::Pool(bsls::Types::size_type       blockSize,
           bsls::BlockGrowth::Strategy  growthStrategy,
           bslma::Allocator            *basicAllocator)
: d_blockSize(blockSize)
, d_chunkSize(bsls::BlockGrowth::BSLS_CONSTANT == growthStrategy
              ? k_MAX_CHUNK_SIZE
              : k_INITIAL_CHUNK_SIZE)
, d_maxBlocksPerChunk(k_MAX_CHUNK_SIZE)
, d_growthStrategy(growthStrategy)
, d_freeList_p(0)
, d_blockList(basicAllocator)
, d_begin_p(0)
, d_end_p(0)
{
    BSLS_ASSERT(1 <= blockSize);

    d_internalBlockSize = roundUp(blockSize,
                                  bsls::AlignmentUtil::BSLS_MAX_ALIGNMENT);
}

Pool::Pool(bsls::Types::size_type       blockSize,
           bsls::BlockGrowth::Strategy  growthStrategy,
           int                          maxBlocksPerChunk,
           bslma::Allocator            *basicAllocator)
: d_blockSize(blockSize)
, d_chunkSize(bsls::BlockGrowth::BSLS_CONSTANT == growthStrategy
              ? maxBlocksPerChunk
              : k_INITIAL_CHUNK_SIZE)
, d_maxBlocksPerChunk(maxBlocksPerChunk)
, d_growthStrategy(growthStrategy)
, d_freeList_p(0)
, d_blockList(basicAllocator)
, d_begin_p(0)
, d_end_p(0)
{
    BSLS_ASSERT(1 <= blockSize);
    BSLS_ASSERT(1 <= maxBlocksPerChunk);

    d_internalBlockSize = roundUp(blockSize,
                                  bsls::AlignmentUtil::BSLS_MAX_ALIGNMENT);
}

Pool::~Pool()
{
    BSLS_ASSERT(sizeof(Link) <= d_internalBlockSize);
    BSLS_ASSERT(0 < d_chunkSize);
}

// MANIPULATORS
void Pool::reserveCapacity(int numBlocks) {}

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
