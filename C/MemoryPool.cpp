// Copyright (c) 2016/17/18/19/20/21 Leandro T. C. Melo <ltcmelo@gmail.com>
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
    : blocks__(0)
    , allocatedBlocks__(0)
    , blockCount__(-1)
    , ptr__(0)\
    , end__(0)
{}

MemoryPool::~MemoryPool()
{
    if (blocks__) {
        for (int i = 0; i < allocatedBlocks__; ++i) {
            if (char* b = blocks__[i])
                std::free(b);
        }
        std::free(blocks__);
    }
}

void MemoryPool::reset()
{
    blockCount__ = -1;
    ptr__ = end__ = 0;
}

void* MemoryPool::allocate_helper(size_t size)
{
    if (++blockCount__ == allocatedBlocks__) {
        if (!allocatedBlocks__)
            allocatedBlocks__ = DEFAULT_BLOCK_COUNT;
        else
            allocatedBlocks__ *= 2;

        blocks__ = (char**)realloc(blocks__, sizeof(char*) * allocatedBlocks__);

        for (int index = blockCount__; index < allocatedBlocks__; ++index)
            blocks__[index] = 0;
    }

    char*& block = blocks__[blockCount__];
    if (!block)
        block = (char *) std::malloc(BLOCK_SIZE);

    ptr__ = block;
    end__ = ptr__ + BLOCK_SIZE;

    void* addr = ptr__;
    ptr__ += size;

    return addr;
}
