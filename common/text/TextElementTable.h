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

#ifndef PSYCHE_TEXT_ELEMENT_TABLE_H__
#define PSYCHE_TEXT_ELEMENT_TABLE_H__

#include <cstdlib>
#include <cstring>

namespace psy {

template <class ElemT>
class TextElementTable
{
public:
    TextElementTable(const TextElementTable&) = delete;
    void operator=(const TextElementTable&) = delete;

    TextElementTable()
       : elements_(nullptr)
       , count_(-1)
       , allocated_(0)
       , buckets_(nullptr)
       , bucketCount_(0)
    {}

    ~TextElementTable()
    {
        reset();
    }

    typedef ElemT* const* iterator;
    iterator begin() const { return elements_; }
    iterator end() const { return elements_ + count_ + 1; }

    bool empty() const { return count_ == -1; }
    unsigned int size() const { return count_ + 1; }
    const ElemT* at(unsigned int idx) const { return elements_[idx]; }

    const ElemT* find(const char* chars, unsigned int size) const
    {
        if (buckets_) {
            unsigned int h = ElemT::hashCode(chars, size);
            ElemT* elem = buckets_[h % bucketCount_];
            for (; elem; elem = static_cast<ElemT*>(elem->next_)) {
                if (elem->size() == size && !std::strncmp(elem->c_str(), chars, size))
                    return elem;
            }
        }
        return nullptr;
    }

    const ElemT* findOrInsert(const char *chars, unsigned int size)
    {
        ElemT* elem = const_cast<ElemT*>(find(chars, size));
        if (elem)
            return elem;

        if (++count_ == allocated_) {
            if (!allocated_)
                allocated_ = 4;
            else
                allocated_ <<= 1;
            elements_ = (ElemT**) std::realloc(elements_, sizeof(ElemT*)* allocated_);
        }

        elem = new ElemT(chars, size);
        elements_[count_] = elem;

        if (!buckets_ || count_ * 5 >= bucketCount_ * 3)
            rehash();
        else {
            unsigned h = elem->hashCode() % bucketCount_;
            elem ->next_ = buckets_[h];
            buckets_[h] = elem;
        }

        return elem;
    }

    void reset()
    {
        if (elements_) {
            ElemT** last = elements_ + count_ + 1;
            for (ElemT** it = elements_; it != last; ++it)
                delete *it;
            std::free(elements_);
        }

        if (buckets_)
            std::free(buckets_);

        elements_ = 0;
        buckets_ = 0;
        allocated_ = 0;
        count_ = -1;
        bucketCount_ = 0;
    }

private:
    void rehash()
    {
       if (buckets_)
           std::free(buckets_);

       if (!bucketCount_)
           bucketCount_ = 4;
       else
           bucketCount_ <<= 1;

       buckets_ = (ElemT**)std::calloc(bucketCount_, sizeof(ElemT*));

       ElemT** last = elements_ + (count_ + 1);
       for (ElemT** it = elements_; it != last; ++it) {
           ElemT* cur = *it;
           unsigned int h = cur->hashCode() % bucketCount_;
           cur->next_ = buckets_[h];
           buckets_[h] = cur;
       }
    }

    ElemT** elements_;
    int count_;
    int allocated_;
    ElemT** buckets_;
    int bucketCount_;
};

} // psy

#endif
