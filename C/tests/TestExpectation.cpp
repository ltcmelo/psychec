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
    , nestedRetTySpec_(nullptr)
    , namedTyK_(NamedTypeKind::UNSPECIFIED)
    , specTyBuiltinK_(BuiltinTypeKind::UNSPECIFIED)
    , specTyCVR_(CVR::None)
{}

TypeSpecSummary::~TypeSpecSummary()
{}

DeclSummary& TypeSpecSummary::basis(std::string name,
                                    NamedTypeKind namedTyK,
                                    BuiltinTypeKind builtinTypeKind,
                                    CVR cvr)
{
    specTyName_ = std::move(name);
    namedTyK_ = namedTyK;
    specTyBuiltinK_ = builtinTypeKind;
    specTyCVR_ = cvr;
    return declSummary_;
}

DeclSummary& TypeSpecSummary::deriv(TypeKind tyKind, CVR cvr, Decay decay)
{
    derivTyKs_.push_back(tyKind);
    derivTyCVRs_.push_back(cvr);
    derivPtrTyDecay_.push_back(decay);
    return declSummary_;
}

TypeSpecSummary& TypeSpecSummary::Parameter()
{
    parmsTySpecs_.emplace_back(declSummary_);
    return parmsTySpecs_.back();
}

TypeSpecSummary& TypeSpecSummary::_AtParam_()
{
    return parmsTySpecs_.back();
}

DeclSummary& TypeSpecSummary::NestAsReturn()
{
    nestedRetTySpec_.reset(new TypeSpecSummary(*this));

    specTyName_.clear();
    namedTyK_ = NamedTypeKind::UNSPECIFIED;
    specTyBuiltinK_ = BuiltinTypeKind::UNSPECIFIED;
    specTyCVR_ = CVR::None;
    derivTyKs_.clear();
    derivTyCVRs_.clear();
    derivPtrTyDecay_.clear();
    parmsTySpecs_.clear();

    return declSummary_;
}

DeclSummary::DeclSummary()
    : valK_(ValueKind::UNSPECIFIED)
    , tyK_(TypeKind::UNSPECIFIED)
    , scopeK_(ScopeKind::UNSPECIFIED)
    , TySpec(*this)
{}

DeclSummary& DeclSummary::Value(std::string name, ValueKind valK, ScopeKind scopeK)
{
    name_ = std::move(name);
    symK_ = SymbolKind::Value;
    valK_ = valK;
    scopeK_ = scopeK;
    return *this;
}

DeclSummary& DeclSummary::Type(std::string name, NamedTypeKind namedTyDeclK)
{
    name_ = std::move(name);
    symK_ = SymbolKind::Type;
    namedTyDeclK_ = namedTyDeclK;
    return *this;
}

DeclSummary& DeclSummary::Function(std::string funcName, ScopeKind scopeK)
{
    name_ = std::move(funcName);
    symK_ = SymbolKind::Function;
    scopeK_ = scopeK;
    return *this;
}

DeclSummary& DeclSummary::withScopeKind(ScopeKind scopeK)
{
    scopeK_ = scopeK;
    return *this;
}

DeclSummary& DeclSummary::withNameSpaceKind(NameSpaceKind nsK)
{
    nsK_ = nsK;
    return *this;
}

DeclSummary& DeclSummary::withNameKind(SymbolNameKind nameK)
{
    nameK_ = nameK;
    return *this;
}

DeclSummary& DeclSummary::withTagChoice(TagSymbolName::TagChoice tagChoice)
{
    tagChoice_ = tagChoice;
    return *this;
}

Expectation::Expectation()
    : numE_(0)
    , numW_(0)
    , continueTestDespiteOfErrors_(false)
    , containsAmbiguity_(false)
    , unfinishedParse_(false)
{}

Expectation &Expectation::ContinueTestDespiteOfErrors()
{
    continueTestDespiteOfErrors_ = true;
    return *this;
}

Expectation &Expectation::unfinishedParse()
{
    unfinishedParse_ = true;
    return *this;
}

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
    containsAmbiguity_ = true;
    ambiguityText_ = std::move(s);
    return *this;
}

Expectation& Expectation::binding(DeclSummary b)
{
    bindings_.push_back(b);
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
