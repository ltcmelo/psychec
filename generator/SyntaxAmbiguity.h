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

#ifndef PSYCHE_SYNTAXAMBIGUITY_H__
#define PSYCHE_SYNTAXAMBIGUITY_H__

#include "Name.h"
#include "Names.h"

namespace psyche {

/*!
 * \brief The AmbiguityInfo class
 */
class AmbiguityInfo
{
public:
    enum class Resolution : char
    {
        Unknown,
        DefinitelyDeclaration,
        DefinitelyExpression
    };

    enum class Variety : char
    {
        OneArgCall_X_VarDecl,    //! a ( b );
        MulExpr_X_PointerDecl    //! a * b;
    };

    AmbiguityInfo(Variety variety,
                  unsigned line,
                  Resolution status = Resolution::Unknown)
        : variety_(variety)
        , line_(line)
        , resolution_(status)
        , lhs_(nullptr)
        , rhs_(nullptr)
    {}

    Variety variety() const { return variety_; }

    void resolveAs(AmbiguityInfo::Resolution resolution);
    Resolution resolution() const { return resolution_; }

    void setLhs(const CPlusPlus::Name* name) { lhs_ = name; }
    const CPlusPlus::Name* lhs() const { return lhs_; }
    void setRhs(const CPlusPlus::Name* name) { rhs_ = name; }
    const CPlusPlus::Name* rhs() const { return rhs_; }

    const unsigned line() const { return line_; }

private:
    Variety variety_;
    const unsigned line_;
    Resolution resolution_;
    const CPlusPlus::Name* lhs_;
    const CPlusPlus::Name* rhs_;
};

inline void AmbiguityInfo::resolveAs(AmbiguityInfo::Resolution resolution)
{
    resolution_ = resolution;
}

} // namespace psyche

#endif
