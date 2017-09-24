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

#ifndef PSYCHE_RANGE_H__
#define PSYCHE_RANGE_H__

#include "Symbol.h"
#include <cstdint>
#include <iostream>
#include <memory>
#include <limits>
#include <utility>

namespace psyche {

class AbstractValue;

/*!
 * \brief The Range class
 */
class Range final
{
public:
    Range(std::unique_ptr<AbstractValue> lower,
          std::unique_ptr<AbstractValue> upper);
    Range(const Range&);
    Range(Range&&) = default;
    Range& operator=(const Range&);
    Range& operator=(Range&&) = default;

    friend std::ostream& operator<<(std::ostream& os, const Range& range);

private:
    friend class RangeAnalysis;

    std::unique_ptr<AbstractValue> lower_;
    std::unique_ptr<AbstractValue> upper_;
};

/*!
 * \brief The AbstractValue class
 */
class AbstractValue
{
public:
    virtual ~AbstractValue() = default;
    virtual std::unique_ptr<AbstractValue> clone() const = 0;

    struct Bound
    {
        enum Kind : uint8_t { Defined, Undefined };
        Bound() : kind_(Undefined) {}
        Bound(int64_t number) : kind_(Defined), number_(number) {}
        Kind kind_;
        int64_t number_;
    };

    virtual Bound evaluate() = 0;

protected:
    AbstractValue() = default;
};

class IntegerValue final : public AbstractValue
{
public:
    IntegerValue(int64_t value)
        : value_(value)
    {}

    std::unique_ptr<AbstractValue> clone() const override;
    Bound evaluate() override { return Bound(value_); }

private:
    int64_t value_;
};

class SymbolValue final : public AbstractValue
{
public:
    SymbolValue(const psyche::Symbol* symbol)
        : symbol_(symbol)
    {}

    std::unique_ptr<AbstractValue> clone() const override;
    Bound evaluate() override;

private:
    const psyche::Symbol* symbol_;
};

class CompositeValue final : public AbstractValue
{
public:
    enum Operation : std::uint8_t
    {
        Addition,
        Minimum,
        Maximum
    };

    CompositeValue(std::unique_ptr<AbstractValue> a,
                   std::unique_ptr<AbstractValue> b,
                   Operation op);

    std::unique_ptr<AbstractValue> clone() const override;
    Bound evaluate() override;

private:
    std::unique_ptr<AbstractValue> a_;
    std::unique_ptr<AbstractValue> b_;
    Operation op_;
};

inline std::ostream& operator<<(std::ostream& os, const Range& range)
{
    const auto& l = range.lower_->evaluate();
    const auto& u = range.upper_->evaluate();
    auto show = [&os] (const AbstractValue::Bound& bound, char sign) {
        if (bound.kind_ == AbstractValue::Bound::Undefined)
            os << sign << std::numeric_limits<double>::infinity();
        else
            os << bound.number_;
    };
    os << "[";
    show(l, '-');
    os << ", ";
    show(u, '+');
    os << "]";
    return os;
}

} // namespace psyche

#endif
