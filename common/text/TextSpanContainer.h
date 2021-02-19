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

#ifndef PSYCHE_TEXT_SPAN_CONTAINER_H__
#define PSYCHE_TEXT_SPAN_CONTAINER_H__

#include <cstdlib>
#include <cstring>

namespace psy {

template <class TextSpanT>
class TextSpanContainer
{
public:
    TextSpanContainer()
       : spans_(nullptr)
       , count_(-1)
       , allocated_(0)
       , buckets_(nullptr)
       , bucketCount_(0)
    {}

    ~TextSpanContainer()
    {
        reset();
    }

    TextSpanContainer(const TextSpanContainer& other) = delete;
    void operator=(const TextSpanContainer &other) = delete;

    typedef TextSpanT* const* iterator;

    iterator begin() const { return spans_; }
    iterator end() const { return spans_ + count_ + 1; }

    bool empty() const { return count_ == -1; }
    unsigned size() const { return count_ + 1; }
    const TextSpanT* at(unsigned spanIdx) const { return spans_[spanIdx]; }

    const TextSpanT* find(const char* chars, unsigned size) const
    {
        if (buckets_) {
            unsigned h = TextSpanT::hashCode(chars, size);
            TextSpanT* span = buckets_[h % bucketCount_];
            for (; span; span = static_cast<TextSpanT*>(span->next_)) {
                if (span->size() == size && !std::strncmp(span->c_str(), chars, size))
                    return span;
            }
        }
        return nullptr;
    }

    const TextSpanT* findOrInsert(const char *chars, unsigned size)
    {
        TextSpanT* span = const_cast<TextSpanT*>(find(chars, size));
        if (span)
            return span;

        if (++count_ == allocated_) {
            if (!allocated_)
                allocated_ = 4;
            else
                allocated_ <<= 1;
            spans_ = (TextSpanT**) std::realloc(spans_, sizeof(TextSpanT*)* allocated_);
        }

        span = new TextSpanT(chars, size);
        spans_[count_] = span;

        if (!buckets_ || count_ * 5 >= bucketCount_ * 3)
            rehash();
        else {
            unsigned h = span->hashCode() % bucketCount_;
            span ->next_ = buckets_[h];
            buckets_[h] = span;
        }

        return span;
    }

    void reset()
    {
        if (spans_) {
            TextSpanT** last = spans_ + count_ + 1;
            for (TextSpanT** it = spans_; it != last; ++it)
                delete *it;
            std::free(spans_);
        }

        if (buckets_)
            std::free(buckets_);

        spans_ = 0;
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

       buckets_ = (TextSpanT**)std::calloc(bucketCount_, sizeof(TextSpanT*));

       TextSpanT** last = spans_ + (count_ + 1);
       for (TextSpanT** it = spans_; it != last; ++it) {
           TextSpanT* cur = *it;
           unsigned h = cur->hashCode() % bucketCount_;
           cur->next_ = buckets_[h];
           buckets_[h] = cur;
       }
    }

    TextSpanT** spans_;
    int count_;
    int allocated_;
    TextSpanT** buckets_;
    int bucketCount_;
};

} // psy

#endif
