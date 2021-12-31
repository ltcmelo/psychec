// Copyright (c) 2021 Leandro T. C. Melo <ltcmelo@gmail.com>
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

#ifndef PSYCHE_C_SCOPE_H__
#define PSYCHE_C_SCOPE_H__

#include "API.h"
#include "APIFwds.h"

#include <memory>
#include <unordered_map>
#include <vector>

namespace psy {
namespace C {

/**
 * \brief The Scope class.
 *
 * \remark 6.2.1
 */
class PSY_C_API Scope
{
protected:
    Scope();

public:
    /**
     * The BlockScopes of \c this Scope.
     */
    std::vector<const BlockScope*> blocks() const;

private:
    friend class Binder;

    BlockScope* makeBlock__();

    std::vector<std::unique_ptr<BlockScope>> blocks__;
};

} // C
} // psy

#endif
