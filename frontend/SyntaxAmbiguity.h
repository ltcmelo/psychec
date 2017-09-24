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

#include "FrontendConfig.h"
#include "Name.h"
#include "Names.h"

namespace psyche {

/*!
 * \brief The SyntaxAmbiguity class
 */
class CFE_API SyntaxAmbiguity
{
public:
    enum class Resolution : char
    {
        Unknown,
        IsDecl,
        IsExpr,
        IsCoercion
    };

    enum class Variety : char
    {
        OneArgCall_X_VarDecl,    //! a(b);
        MulExpr_X_PointerDecl,   //! a*b;
        // TODO: Implement remaining case below.
        BinExpr_X_Coercion       //! (a)*b; (a)-b;
    };

    SyntaxAmbiguity(Variety variety,
                    unsigned line,
                    Resolution r = Resolution::Unknown)
        : variety_(variety)
        , line_(line)
        , resolution_(r)
        , lhs_(nullptr)
        , rhs_(nullptr)
    {}

    Variety variety() const { return variety_; }

    void applyResolution(SyntaxAmbiguity::Resolution r) { resolution_ = r; }
    Resolution resolution() const { return resolution_; }

    void setLhs(const psyche::Name* name) { lhs_ = name; }
    const psyche::Name* lhs() const { return lhs_; }
    void setRhs(const psyche::Name* name) { rhs_ = name; }
    const psyche::Name* rhs() const { return rhs_; }

    const unsigned line() const { return line_; }

private:
    Variety variety_;
    const unsigned line_;
    Resolution resolution_;
    const psyche::Name* lhs_;
    const psyche::Name* rhs_;
};

} // namespace psyche

#endif
