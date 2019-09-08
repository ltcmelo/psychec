/******************************************************************************
 * Copyright (c) 2019 Leandro T. C. Melo (ltcmelo@gmail.com)
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301
 * USA
 *****************************************************************************/

#ifndef PSYCHE_SUBSTITUTION_H__
#define PSYCHE_SUBSTITUTION_H__

#include <string>

namespace psyche {

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
ContentT apply(const Substitution<SubT>& sub, const ContentT& input);

template <class SeqT, class ContentT>
ContentT apply(const SeqT& seq, const ContentT& input);

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

} // namespace psyche

#endif
