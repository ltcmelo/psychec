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

TySummary::TySummary(DeclSummary& declSummary)
    : decl_(declSummary)
    , CVR_(CVR::None)
    , nestedRetTy_(nullptr)
{}

TySummary::~TySummary()
{}

DeclSummary& TySummary::Void(CVR cvr)
{
    nameOrTag_ = "void";
    tyK_ = TypeKind::Void;
    CVR_ = cvr;
    return decl_;
}

DeclSummary& TySummary::Basic(BasicTypeKind basicTyK, CVR cvr)
{
    tyK_ = TypeKind::Basic;
    basicTyK_ = basicTyK;
    CVR_ = cvr;
    return decl_;
}

DeclSummary& TySummary::Typedef(std::string name,
                                CVR cvr)
{
    nameOrTag_ = std::move(name);
    tyK_ = TypeKind::Typedef;
    CVR_ = cvr;
    return decl_;
}

DeclSummary& TySummary::Tag(std::string tag,
                            TagTypeKind tagTyK,
                            CVR cvr)
{
    nameOrTag_ = std::move(tag);
    tyK_ = TypeKind::Tag;
    tagTyK_ = tagTyK;
    CVR_ = cvr;
    return decl_;
}

DeclSummary& TySummary::Derived(TypeKind tyKind, CVR cvr, Decay decay)
{
    derivTyKs_.push_back(tyKind);
    derivTyCVRs_.push_back(cvr);
    derivPtrTyDecay_.push_back(decay);
    return decl_;
}

TySummary& TySummary::addParam()
{
    parmsTys_.emplace_back(decl_);
    return parmsTys_.back();
}

TySummary& TySummary::atParam()
{
    return parmsTys_.back();
}

DeclSummary& TySummary::nestAsReturn()
{
    nestedRetTy_.reset(new TySummary(*this));

    nameOrTag_.clear();
    CVR_ = CVR::None;
    derivTyKs_.clear();
    derivTyCVRs_.clear();
    derivPtrTyDecay_.clear();
    parmsTys_.clear();

    return decl_;
}

DeclSummary::DeclSummary()
    : Ty(*this)
{}

DeclSummary& DeclSummary::Value(std::string name, ObjectDeclarationSymbolKind valK, ScopeKind scopeK)
{
    id_ = std::move(name);
    declK_ = DeclarationSymbolKind::Object;
    valDeclK_ = valK;
    scopeK_ = scopeK;
    ns_ = NameSpace::OrdinaryIdentifiers;
    return *this;
}

DeclSummary& DeclSummary::Type(std::string typedefOrTag, TypeDeclarationSymbolKind tyDeclSymK)
{
    id_ = std::move(typedefOrTag);
    declK_ = DeclarationSymbolKind::Type;
    tyDeclSymK_ = tyDeclSymK;
    return *this;
}

DeclSummary& DeclSummary::Function(std::string funcName, ScopeKind scopeK)
{
    id_ = std::move(funcName);
    declK_ = DeclarationSymbolKind::Function;
    scopeK_ = scopeK;
    ns_ = NameSpace::OrdinaryIdentifiers;
    return *this;
}

DeclSummary& DeclSummary::withScopeKind(ScopeKind scopeK)
{
    scopeK_ = scopeK;
    return *this;
}

DeclSummary& DeclSummary::withNameSpace(NameSpace ns)
{
    ns_ = ns;
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
