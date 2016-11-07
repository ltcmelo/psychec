/******************************************************************************
 * Copyright (c) 2016 Leandro T. C. Melo (ltcmelo@gmail.com)
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

#include "Range.h"
#include "Assert.h"
#include <algorithm>
#include <functional>

using namespace psyche;

Range::Range(std::unique_ptr<AbstractValue> lower,
             std::unique_ptr<AbstractValue> upper)
    : lower_(std::move(lower))
    , upper_(std::move(upper))
{}

Range::Range(const Range &range)
    : lower_(range.lower_->clone())
    , upper_(range.upper_->clone())
{}

Range& Range::operator=(const Range& range)
{
    lower_ = range.lower_->clone();
    upper_ = range.upper_->clone();
    return *this;
}

std::unique_ptr<AbstractValue> IntegerValue::clone() const
{
    return std::make_unique<IntegerValue>(value_);
}

std::unique_ptr<AbstractValue> SymbolValue::clone() const
{
    return std::make_unique<SymbolValue>(symbol_);
}

AbstractValue::Bound SymbolValue::evaluate()
{
    // TODO: Lookup symbol.
    return Bound();
}

CompositeValue::CompositeValue(std::unique_ptr<AbstractValue> a,
                               std::unique_ptr<AbstractValue> b,
                               Operation op)
    : a_(std::move(a)), b_(std::move(b)), op_(op)
{}

std::unique_ptr<AbstractValue> CompositeValue::clone() const
{
    return std::make_unique<CompositeValue>(a_->clone(), b_->clone(), op_);
}

AbstractValue::Bound CompositeValue::evaluate()
{
    const Bound& a = a_->evaluate();
    const Bound& b = b_->evaluate();
    auto check = a.kind_ == AbstractValue::Bound::Undefined
            || b.kind_ == AbstractValue::Bound::Undefined;
    switch (op_) {
    case Addition:
        return check ? Bound() : Bound(a.number_ + b.number_);

    case Minimum:
        return check ? Bound() : Bound(std::min(a.number_, b.number_));

    case Maximum:
        return check ? Bound() : Bound(std::max(a.number_, b.number_));

    default:
        PSYCHE_ASSERT(false, return Bound(), "invalid enumerator");
        return Bound();
    }
}
