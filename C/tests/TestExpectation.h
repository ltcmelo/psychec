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
#include "binder/Scope.h"
#include "symbols/SymbolKind.h"
#include "symbols/ValueKind.h"
#include "symbols/TypeKind.h"
#include "symbols/TypeKind_Builtin.h"
#include "symbols/TypeKind_Named.h"
#include "tests/TestRunner.h"

#include <functional>
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
    None
};

struct BindingSummary
{
    BindingSummary();

    BindingSummary& Value(std::string name,
                          ValueKind valK,
                          ScopeKind scopeK = ScopeKind::UNSPECIFIED);
    BindingSummary& Type(std::string name, TypeKind tyK);
    BindingSummary& Function(std::string funcName,
                             ScopeKind scopeK = ScopeKind::UNSPECIFIED);

    BindingSummary& specType(std::string name,
                             NamedTypeKind tyNameK,
                             BuiltinTypeKind builtinTypeKind = BuiltinTypeKind::UNSPECIFIED,
                             CVR cvr = CVR::None);
    BindingSummary& derivType(TypeKind tyKind, CVR cvr = CVR::None);

    std::string name_;
    SymbolKind symK_;
    ValueKind valK_;
    TypeKind tyK_;
    ScopeKind scopeK_;

    std::string specTyName_;
    NamedTypeKind specTyK_;
    BuiltinTypeKind specTyBuiltinK_;
    CVR specTyCVR_;
    std::vector<TypeKind> derivTyKs_;
    std::vector<CVR> derivTyCVRs_;
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

    bool isAmbiguous_;
    std::string ambiguityText_;
    Expectation& ambiguity(std::string s = "");

    std::vector<SyntaxKind> syntaxKinds_;
    Expectation& AST(std::vector<SyntaxKind>&& v);

    std::vector<BindingSummary> bindings_;
    Expectation& binding(BindingSummary b);
};

} // C
} // psy

#endif
