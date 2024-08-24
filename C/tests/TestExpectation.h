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

#ifndef PSYCHE_C_TEST_EXPECTATION_H__
#define PSYCHE_C_TEST_EXPECTATION_H__

#include "compilation/Compilation.h"
#include "binder/NameSpace.h"
#include "binder/Scope.h"
#include "symbols/SymbolKind.h"
#include "symbols/DeclarationKind.h"
#include "symbols/TagTypeDeclarationKind.h"
#include "symbols/TypeDeclarationKind.h"
#include "symbols/ObjectDeclarationKind.h"
#include "tests/TestSuite.h"
#include "types/Type_ALL.h"

#include <functional>
#include <memory>
#include <string>
#include <tuple>
#include <utility>
#include <vector>

namespace psy {
namespace C {

enum class CVR
{
    Const,
    Volatile,
    Restrict,
    ConstAndVolatile,
    ConstAndRestrict,
    Atomic,
    None
};

enum class Decay
{
    None,
    FromArrayToPointer,
    FromFunctionToFunctionPointer
};

struct Decl;

struct Ty
{
    Ty(Decl& Decl);
    Ty(Ty&& ty) = default;
    Ty(const Ty& ty) = default;
    ~Ty();

    Decl& Void(CVR cvr = CVR::None);
    Decl& Basic(BasicTypeKind basicTyK, CVR cvr = CVR::None);
    Decl& Typedef(std::string typedefName, CVR cvr = CVR::None);
    Decl& Tag(std::string tag, TagTypeKind tagTyK, CVR cvr = CVR::None);
    Decl& Derived(TypeKind tyK, CVR cvr = CVR::None, Decay decay = Decay::None);
    Decl& nestAsReturn();
    Ty& addParam();
    Ty& atParam();

    Decl& decl_;
    std::string ident_;
    TypeKind tyK_;
    BasicTypeKind basicTyK_;
    TagTypeKind tagTyK_;
    CVR CVR_;
    std::vector<TypeKind> derivTyKs_;
    std::vector<CVR> derivTyCVRs_;
    std::vector<Decay> derivPtrTyDecay_;
    std::shared_ptr<Ty> nestedRetTy_;
    std::vector<Ty> parmsTys_;
};

struct Decl
{
    Decl();

    Decl& Object(std::string name,
                 ObjectDeclarationKind objDeclSymK,
                 ScopeKind scopeK = ScopeKind::File);
    Decl& Function(std::string name, ScopeKind scopeK = ScopeKind::File);
    Decl& Type(std::string typedefName);
    Decl& Type(std::string tag, TagTypeDeclarationKind tagTyDeclK);
    Decl& withScopeKind(ScopeKind scopeK);
    Decl& inNameSpace(NameSpace ns);

    std::string ident_;
    DeclarationKind declSymK_;
    ObjectDeclarationKind objDeclSymK_;
    TypeDeclarationKind tyDeclSymK_;
    TagTypeDeclarationKind tagTyDeclK_;
    ScopeKind scopeK_;
    NameSpace ns_;
    Ty ty_;
};

struct Expectation
{
    Expectation();

    enum class ErrorOrWarn
    {
        Error,
        Warn
    };

    int numE_;
    int numW_;
    std::vector<std::string> descriptorsE_;
    std::vector<std::string> descriptorsW_;
    Expectation& setErrorCnt(int numE);
    Expectation& setWarnCnt(int numW);
    Expectation& diagnostic(ErrorOrWarn v, std::string descriptorId = "");

    bool continueTestDespiteOfErrors_;
    Expectation& ContinueTestDespiteOfErrors();

    bool containsAmbiguity_;
    std::string ambiguityText_;
    Expectation& ambiguity(std::string s = "");

    bool unfinishedParse_;
    Expectation& unfinishedParse();

    std::vector<SyntaxKind> syntaxKinds_;
    Expectation& AST(std::vector<SyntaxKind>&& v);

    std::vector<Decl> declarations_;
    Expectation& declaration(Decl d);
    Expectation& declaration(Ty t);

    bool checkScope_;
    std::vector<int> scopePath_;
    Expectation& scopePath(std::vector<int>&& v);
};

} // C
} // psy

#endif
