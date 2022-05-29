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

#ifndef PSYCHE_C_LIST_H__
#define PSYCHE_C_LIST_H__

#include "API.h"

#include "Managed.h"

namespace psy {
namespace C {

/**
 * \brief The List class \b template.
 *
 * A singly linked-list.
 */
template <class ValueT,
          class DerivedListT>
class PSY_C_NON_API List : public Managed
{
public:
    List()
        : value(ValueT())
        , next(nullptr)
    {}

    List(const ValueT& value)
        : value(value)
        , next(nullptr)
    {}

    DerivedListT** skipToLast()
    {
        DerivedListT** cur = &next;
        while (*cur)
            cur = &(*cur)->next;
        return &(*cur);
    }

    ValueT lastValue() const
    {
        ValueT v = nullptr;
        for (auto it = const_cast<DerivedListT*>(static_cast<const DerivedListT*>(this));
                it;
                it = it->next) {
            if (it->value)
                v = it->value;
        }
        return v;
    }

    ValueT value;
    DerivedListT* next;

private:
    // Unavailable
    List(const List&) = delete;
    List& operator=(const List&) = delete;
};

} // C
} // psy

#endif
