// Copyright (c) 2016-20 Leandro T. C. Melo <ltcmelo@gmail.com>
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

#ifndef PSYCHE_SYNTAX_AMBIGUITY_H__
#define PSYCHE_SYNTAX_AMBIGUITY_H__

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
