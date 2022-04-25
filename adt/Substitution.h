// Copyright (c) 2019/20/21/22 Leandro T. C. Melo <ltcmelo@gmail.com>
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

#ifndef PSYCHE_SUBSTITUTION_H__
#define PSYCHE_SUBSTITUTION_H__

#include <string>

namespace psy {

template <class T>
class Substitution
{
public:
    Substitution()
        : from_()
        , to_()
    {}

    Substitution(const T& from, const T& to)
        : from_(from)
        , to_(to)
    {}

    const T& from() const { return from_; }
    const T& to() const { return to_; }

    static Substitution<T> Trivial;

private:
    T from_;
    T to_;
};

template <class SubT, class ContentT>
ContentT applyAll(const Substitution<SubT>& sub, const ContentT& input);

template <class SeqT, class ContentT>
ContentT applyAll(const SeqT& seq, const ContentT& input);

template <class SubT, class ContentT>
ContentT applyOnce(const Substitution<SubT>& sub, const ContentT& input);

template <class SeqT, class ContentT>
ContentT applyOnce(const SeqT& seq, const ContentT& input);

template <class T>
bool operator==(const Substitution<T>& s1, const Substitution<T>& s2)
{
    return s1.from() == s2.from() && s1.to() == s2.to();
}

template <class T>
bool operator!=(const Substitution<T>& s1, const Substitution<T>& s2)
{
    return !(s1 == s2);
}

} // psy

#endif
