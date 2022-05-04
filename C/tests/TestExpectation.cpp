// Copyright (c) 2020/21/22 Leandro T. C. Melo <ltcmelo@gmail.com>
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

#include "TestExpectation.h"

using namespace psy;
using namespace  C;

TypeSpecSummary::TypeSpecSummary(DeclSummary& declSummary)
    : declSummary_(declSummary)
{
}

DeclSummary& TypeSpecSummary::basis(std::string name,
                                   NamedTypeKind tyNameK,
                                   BuiltinTypeKind builtinTypeKind,
                                   CVR cvr)
{
    specTyName_ = std::move(name);
    specTyK_ = tyNameK;
    specTyBuiltinK_ = builtinTypeKind;
    specTyCVR_ = cvr;
    return declSummary_;
}

DeclSummary &TypeSpecSummary::deriv(TypeKind tyKind, CVR cvr)
{
    derivTyKs_.push_back(tyKind);
    derivTyCVRs_.push_back(cvr);
    return declSummary_;;
}

DeclSummary::DeclSummary()
    : valK_(ValueKind::UNSPECIFIED)
    , tyK_(TypeKind::UNSPECIFIED)
    , scopeK_(ScopeKind::UNSPECIFIED)
    , TypeSpec(*this)
{}

DeclSummary& DeclSummary::Value(std::string name, ValueKind valK, ScopeKind scopeK)
{
    name_ = std::move(name);
    symK_ = SymbolKind::Value;
    valK_ = valK;
    scopeK_ = scopeK;
    return *this;
}

DeclSummary& DeclSummary::Type(std::string name, TypeKind tyK)
{
    name_ = std::move(name);
    symK_ = SymbolKind::Type;
    tyK_ = tyK;
    return *this;
}

DeclSummary& DeclSummary::Function(std::string funcName, ScopeKind scopeK)
{
    name_ = std::move(funcName);
    symK_ = SymbolKind::Function;
    scopeK_ = scopeK;
    return *this;
}

TypeSpecSummary& DeclSummary::WithParameter()
{
    parmsTySpecs_.emplace_back(*this);
    return parmsTySpecs_.back();
}

Expectation::Expectation()
    : numE_(0)
    , numW_(0)
    , isAmbiguous_(false)
{}

Expectation& Expectation::setErrorCnt(int numE)
{
    numE_ = numE;
    return *this;
}

Expectation& Expectation::setWarnCnt(int numW)
{
    numW_ = numW;
    return *this;
}

Expectation& Expectation::ambiguity(std::string s)
{
    isAmbiguous_ = true;
    ambiguityText_ = std::move(s);
    return *this;
}

Expectation& Expectation::binding(DeclSummary b)
{
    bindings_.emplace_back(std::move(b));
    return *this;
}

Expectation& Expectation::AST(std::vector<SyntaxKind>&& v)
{
    syntaxKinds_ = std::move(v);
    return *this;
}

Expectation& Expectation::diagnostic(ErrorOrWarn v, std::string descriptorId)
{
    if (v == ErrorOrWarn::Error) {
        ++numE_;
        if (!descriptorId.empty())
            descriptorsE_.push_back(descriptorId);
    }
    else {
        ++numW_;
        if (!descriptorId.empty())
            descriptorsW_.push_back(descriptorId);
    }
    return *this;
}
