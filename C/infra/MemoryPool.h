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

#ifndef PSYCHE_C_MEMORY_POOL_H__
#define PSYCHE_C_MEMORY_POOL_H__

#include "API.h"

#include <cstddef>

namespace psy {
namespace C {

class PSY_C_NON_API MemoryPool
{
public:
    MemoryPool();
    ~MemoryPool();

    // Unavailable
    MemoryPool(const MemoryPool&) = delete;
    void operator=(const MemoryPool&) = delete;

    void reset();

    void* allocate(size_t size)
    {
        size = (size + 7) & ~7;
        if (ptr_ && (ptr_ + size < end_)) {
            void *addr = ptr_;
            ptr_ += size;
            return addr;
        }
        return allocate_helper(size);
    }

private:
    void* allocate_helper(size_t size);

    char** blocks_;
    int allocatedBlocks_;
    int blockCount_;
    char* ptr_;
    char* end_;

    enum
    {
        BLOCK_SIZE = 8 * 1024,
        DEFAULT_BLOCK_COUNT = 8
    };
};

} // C
} // psy

#endif
