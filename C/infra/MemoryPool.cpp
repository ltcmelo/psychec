// Copyright (c) 2016/17/18/19/20/21/22 Leandro T. C. Melo <ltcmelo@gmail.com>
// Copyright (c) 2008 Roberto Raggi <roberto.raggi@gmail.com>
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
// THE SOFTWARE.

#include "MemoryPool.h"

#include <cstdlib>
#include <cstring>

using namespace psy;
using namespace C;

MemoryPool::MemoryPool()
    : blocks_(0)
    , allocatedBlocks_(0)
    , blockCount_(-1)
    , ptr_(0)\
    , end_(0)
{}

MemoryPool::~MemoryPool()
{
    if (blocks_) {
        for (int i = 0; i < allocatedBlocks_; ++i) {
            if (char* b = blocks_[i])
                std::free(b);
        }
        std::free(blocks_);
    }
}

void MemoryPool::reset()
{
    blockCount_ = -1;
    ptr_ = end_ = 0;
}

void* MemoryPool::allocate_helper(size_t size)
{
    if (++blockCount_ == allocatedBlocks_) {
        if (!allocatedBlocks_)
            allocatedBlocks_ = DEFAULT_BLOCK_COUNT;
        else
            allocatedBlocks_ *= 2;

        blocks_ = (char**)realloc(blocks_, sizeof(char*) * allocatedBlocks_);

        for (int index = blockCount_; index < allocatedBlocks_; ++index)
            blocks_[index] = 0;
    }

    char*& block = blocks_[blockCount_];
    if (!block)
        block = (char *) std::malloc(BLOCK_SIZE);

    ptr_ = block;
    end_ = ptr_ + BLOCK_SIZE;

    void* addr = ptr_;
    ptr_ += size;

    return addr;
}
