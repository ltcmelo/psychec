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

Ty::Ty(Decl& Decl)
    : decl_(Decl)
    , CVR_(CVR::None)
    , nestedRetTy_(nullptr)
{}

Ty::~Ty()
{}

Decl& Ty::Void(CVR cvr)
{
    ident_ = "void";
    tyK_ = TypeKind::Void;
    CVR_ = cvr;
    return decl_;
}

Decl& Ty::Basic(BasicTypeKind basicTyK, CVR cvr)
{
    tyK_ = TypeKind::Basic;
    basicTyK_ = basicTyK;
    CVR_ = cvr;
    return decl_;
}

Decl& Ty::Typedef(std::string typedefName, CVR cvr)
{
    ident_ = std::move(typedefName);
    tyK_ = TypeKind::Typedef;
    CVR_ = cvr;
    return decl_;
}

Decl& Ty::Tag(std::string tag,
              TagTypeKind tagTyK,
              CVR cvr)
{
    ident_ = std::move(tag);
    tyK_ = TypeKind::Tag;
    tagTyK_ = tagTyK;
    CVR_ = cvr;
    return decl_;
}

Decl& Ty::Derived(TypeKind tyK, CVR cvr, Decay decay)
{
    derivTyKs_.push_back(tyK);
    derivTyCVRs_.push_back(cvr);
    derivPtrTyDecay_.push_back(decay);
    return decl_;
}

Decl& Ty::nestAsReturn()
{
    nestedRetTy_.reset(new Ty(*this));
    ident_.clear();
    CVR_ = CVR::None;
    derivTyKs_.clear();
    derivTyCVRs_.clear();
    derivPtrTyDecay_.clear();
    parmsTys_.clear();
    return decl_;
}

Ty& Ty::addParam()
{
    parmsTys_.emplace_back(decl_);
    return parmsTys_.back();
}

Ty& Ty::atParam()
{
    return parmsTys_.back();
}

Decl::Decl()
    : ty_(*this)
{}

Decl& Decl::Object(std::string name, ObjectDeclarationSymbolKind objDeclSymK, ScopeKind scopeK)
{
    ident_ = std::move(name);
    declSymK_ = DeclarationSymbolKind::Object;
    objDeclSymK_ = objDeclSymK;
    scopeK_ = scopeK;
    ns_ = NameSpace::OrdinaryIdentifiers;
    return *this;
}

Decl& Decl::Type(std::string typedefNameOrTag, TypeDeclarationSymbolKind tyDeclSymK)
{
    ident_ = std::move(typedefNameOrTag);
    declSymK_ = DeclarationSymbolKind::Type;
    tyDeclSymK_ = tyDeclSymK;
    return *this;
}

Decl& Decl::Function(std::string name, ScopeKind scopeK)
{
    ident_ = std::move(name);
    declSymK_ = DeclarationSymbolKind::Function;
    scopeK_ = scopeK;
    ns_ = NameSpace::OrdinaryIdentifiers;
    return *this;
}

Decl& Decl::withScopeKind(ScopeKind scopeK)
{
    scopeK_ = scopeK;
    return *this;
}

Decl& Decl::inNameSpace(NameSpace ns)
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
    , checkScope_(false)
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

Expectation& Expectation::declaration(Decl b)
{
    declarations_.push_back(b);
    return *this;
}

Expectation& Expectation::declaration(Ty ty)
{
    declarations_.push_back(ty.decl_);
    return *this;
}

Expectation& Expectation::scopePath(std::vector<int>&& v)
{
    checkScope_ = true;
    scopePath_ = std::move(v);
    std::reverse(scopePath_.begin(), scopePath_.end());
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
