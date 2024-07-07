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
#include "symbols/DeclarationSymbolKind.h"
#include "symbols/TypeDeclarationSymbolKind.h"
#include "symbols/ObjectDeclarationSymbolKind.h"
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

struct DeclSummary;

struct TySummary
{
    TySummary(DeclSummary& declSummary);
    TySummary(TySummary&& ty) = default;
    TySummary(const TySummary& ty) = default;
    ~TySummary();

    DeclSummary& Void(CVR cvr = CVR::None);
    DeclSummary& Basic(BasicTypeKind basicTyK, CVR cvr = CVR::None);
    DeclSummary& Typedef(std::string name, CVR cvr = CVR::None);
    DeclSummary& Tag(std::string tag, TagTypeKind tagTyK, CVR cvr = CVR::None);
    DeclSummary& Derived(TypeKind tyKind, CVR cvr = CVR::None, Decay decay = Decay::None);
    DeclSummary& nestAsReturn();
    TySummary& addParam();
    TySummary& atParam();

    DeclSummary& decl_;

    std::string nameOrTag_;
    TypeKind tyK_;
    BasicTypeKind basicTyK_;
    TagTypeKind tagTyK_;
    CVR CVR_;

    std::vector<TypeKind> derivTyKs_;
    std::vector<CVR> derivTyCVRs_;
    std::vector<Decay> derivPtrTyDecay_;
    std::shared_ptr<TySummary> nestedRetTy_;
    std::vector<TySummary> parmsTys_;
};

struct DeclSummary
{
    DeclSummary();

    DeclSummary& Value(std::string name,
                       ObjectDeclarationSymbolKind valK,
                       ScopeKind scopeK = ScopeKind::File);
    DeclSummary& Function(std::string funcName,
                          ScopeKind scopeK = ScopeKind::File);
    DeclSummary& Type(std::string typedefOrTag, TypeDeclarationSymbolKind tyDeclSymK);

    DeclSummary& withScopeKind(ScopeKind scopeK);
    DeclSummary& withNameSpace(NameSpace ns);

    std::string ident_;
    DeclarationSymbolKind declSymK_;
    ObjectDeclarationSymbolKind objDeclSymK_;
    TypeDeclarationSymbolKind tyDeclSymK_;
    ScopeKind scopeK_;
    NameSpace ns_;

    TySummary Ty;
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

    std::vector<DeclSummary> bindings_;
    Expectation& binding(DeclSummary b);

    bool checkScope_;
    std::vector<int> scopePath_;
    Expectation& scopePath(std::vector<int>&& v);
};

} // C
} // psy

#endif
