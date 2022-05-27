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

#include "BinderTest.h"

#include "compilation/Assembly.h"
#include "compilation/Compilation.h"
#include "compilation/SemanticModel.h"
#include "symbols/Symbol.h"
#include "parser/Unparser.h"
#include "symbols/Symbol_ALL.h"
#include "syntax/SyntaxLexeme_ALL.h"
#include "syntax/SyntaxNamePrinter.h"
#include "syntax/SyntaxNodes.h"

//#define DEBUG_BINDING_SEARCH

using namespace psy;
using namespace C;

const std::string BinderTest::Name = "BINDER";

void BinderTest::testAll()
{
    // TEMPORARY
    std::vector<TestFunction> active;
    for (auto testData : tests_) {
        auto n = std::string(testData.second);
        auto p = n.find("case09");
        if (p != std::string::npos) {
            std::cout << "\t\tskip (TEMP) " << n << std::endl;
            continue;
        }
        active.push_back(testData);
    }

    return run<BinderTest>(active);
}

void BinderTest::setUp()
{}

void BinderTest::tearDown()
{}

namespace {

bool REJECT_CANDIDATE(const Symbol* sym, std::string msg)
{
#ifdef DEBUG_BINDING_SEARCH
    std::cout << "\n\t\tREJECT " << to_string(*sym) << " DUE TO " << msg;
#endif
    return false;
}

void DETAIL_MISMATCH(std::string msg)
{
#ifdef DEBUG_BINDING_SEARCH
    std::cout << "\n\t\t\tmismatch detail: " << msg;
#endif
}

bool CVRMatches(const TypeSymbol* tySym, CVR cvr)
{
    switch (cvr) {
        case CVR::Const:
            if (!tySym->isConstQualified()) {
                DETAIL_MISMATCH("missing const");
                return false;
            }
            break;

        case CVR::Volatile:
            if (!tySym->isVolatileQualified()) {
                DETAIL_MISMATCH("missing volatile");
                return false;
            }
            break;

        case CVR::ConstAndVolatile:
            if (!(tySym->isConstQualified())
                    || !(tySym->isVolatileQualified())) {
                DETAIL_MISMATCH("missing const volatile");
                return false;
            }
            break;

        case CVR::Restrict:
            if (!tySym->isRestrictQualified()) {
                DETAIL_MISMATCH("missing restrict");
                return false;
            }
            break;

        case CVR::None:
            if (tySym->isConstQualified()) {
                DETAIL_MISMATCH("spurious const");
                return false;
            }
            if (tySym->isVolatileQualified()) {
                DETAIL_MISMATCH("spurious volatile");
                return false;
            }
            if (tySym->isRestrictQualified()) {
                DETAIL_MISMATCH("spurious restrict");
                return false;
            }
            break;
    }

    return true;
}

bool namedTypeMatches(const NamedTypeSymbol* namedTySym, const TypeSpecSummary& tySpec)
{
    if (namedTySym->name() == nullptr) {
        DETAIL_MISMATCH("null type name");
        return false;
    }

    if (namedTySym->name()->text() != tySpec.specTyName_) {
        DETAIL_MISMATCH("type name");
        return false;
    }

    if (namedTySym->namedTypeKind() != tySpec.namedTyK_) {
        DETAIL_MISMATCH("type kind");
        return false;
    }

    if (tySpec.specTyBuiltinK_ != BuiltinTypeKind::UNSPECIFIED) {
        if (namedTySym->namedTypeKind() != NamedTypeKind::Builtin) {
            DETAIL_MISMATCH("not a builtin");
            return false;
        }

        if (namedTySym->builtinTypeKind() != tySpec.specTyBuiltinK_) {
            DETAIL_MISMATCH("builtin kind");
            return false;
        }
    }

    if (!CVRMatches(namedTySym, tySpec.specTyCVR_)) {
        DETAIL_MISMATCH("CVR");
        return false;
    }

    return true;
}

bool typeMatches(const TypeSymbol* tySym, const TypeSpecSummary& tySpec)
{
    for (auto i = tySpec.derivTyKs_.size(); i > 0; --i) {
        auto derivTyK = tySpec.derivTyKs_[i - 1];
        if (derivTyK != tySym->typeKind()) {
            DETAIL_MISMATCH("derived type kind");
            return false;
        }

        auto derivTyCVR = tySpec.derivTyCVRs_[i - 1];
        if (!CVRMatches(tySym, derivTyCVR)) {
            DETAIL_MISMATCH("derived type CVR");
            return false;
        }

        auto derivPtrTyDecay = tySpec.derivPtrTyDecay_[i - 1];

        switch (tySym->typeKind()) {
            case TypeKind::Array:
                if (derivPtrTyDecay != Decay::None) {
                    DETAIL_MISMATCH("can't happen for array");
                    return false;
                }
                tySym = tySym->asArrayType()->elementType();
                break;

            case TypeKind::Pointer:
                switch (derivPtrTyDecay) {
                    case Decay::None:
                        if (tySym->asPointerType()->arisesFromFunctionDecay()
                                    || tySym->asPointerType()->arisesFromArrayDecay()) {
                            DETAIL_MISMATCH("pointer type is decayed from array/function");
                            return false;
                        }
                        break;

                    case Decay::FromFunctionToFunctionPointer:
                        if (!tySym->asPointerType()->arisesFromFunctionDecay()) {
                            DETAIL_MISMATCH("pointer type isn't (function) decayed");
                            return false;
                        }
                        break;

                    case Decay::FromArrayToPointer:
                        if (!tySym->asPointerType()->arisesFromArrayDecay()) {
                            DETAIL_MISMATCH("pointer type isn't (array) decayed");
                            return false;
                        }
                        break;
                }
                tySym = tySym->asPointerType()->referencedType();
                break;

            case TypeKind::Function: {
                if (derivPtrTyDecay != Decay::None) {
                    DETAIL_MISMATCH("can't happen for function");
                    return false;
                }

                auto funcTySym = tySym->asFunctionType();
                auto parms = funcTySym->parameterTypes();
                if (parms.size() != tySpec.parmsTySpecs_.size()) {
                    DETAIL_MISMATCH("number of parameters");
                    return false;
                }

                for (auto i = 0U; i < parms.size(); ++i) {
                    const TypeSymbol* parmTySym = parms[i];
                    if (!typeMatches(parmTySym, tySpec.parmsTySpecs_[i])) {
                        DETAIL_MISMATCH("parameter type mismatch");
                        return false;
                    }
                }

                tySym = funcTySym->returnType();
                if (!tySpec.nestedRetTySpec_)
                    break;

                if (!typeMatches(tySym, *tySpec.nestedRetTySpec_.get())) {
                    DETAIL_MISMATCH("nested return type mismatch");
                    return false;
                }
                return true;
            }

            default:
                PSYCHE_TEST_FAIL("unexpected");
                return false;
        }
    }

    const NamedTypeSymbol* namedTySym = tySym->asNamedType();
    if (!namedTySym) {
        DETAIL_MISMATCH("not a named type");
        return false;
    }

    return namedTypeMatches(namedTySym, tySpec);
}

bool functionMatchesBinding(const FunctionSymbol* funcSym, const DeclSummary& decl)
{
    if (!funcSym->name())
        return REJECT_CANDIDATE(funcSym, "empty name");

    if (funcSym->name()->text() != decl.name_)
        return REJECT_CANDIDATE(funcSym, "name mismatch");

    if (funcSym->type() == nullptr)
        return REJECT_CANDIDATE(funcSym, "null type");

    if (funcSym->type()->typeKind() != TypeKind::Function)
        return REJECT_CANDIDATE(funcSym, "not a function type");

    if (!typeMatches(funcSym->type(), decl.TySpec))
        return REJECT_CANDIDATE(funcSym, "type mismatch");

    return true;
}

bool valueMatchesBinding(const ValueSymbol* valSym, const DeclSummary& decl)
{
    if (valSym->valueKind() != decl.valK_)
        return REJECT_CANDIDATE(valSym, "value kind mismatch");

    if (!valSym->name())
        return REJECT_CANDIDATE(valSym, "empty name");

    if (valSym->name()->text() != decl.name_)
        return REJECT_CANDIDATE(valSym, "name mismatch");

    if (valSym->type() == nullptr)
        return REJECT_CANDIDATE(valSym, "null type");

    if (!typeMatches(valSym->type(), decl.TySpec))
        return REJECT_CANDIDATE(valSym, "type mismatch");

    return true;
}

bool typeMatchesBinding(const TypeSymbol* tySym, const DeclSummary& decl)
{
    const NamedTypeSymbol* namedTySym = tySym->asNamedType();
    if (!namedTySym)
        return REJECT_CANDIDATE(tySym, "not a declarable type");

    if (namedTySym->namedTypeKind() != decl.namedTyDeclK_)
        return REJECT_CANDIDATE(tySym, "named type kind mismatch");

    if (!namedTySym->name())
        return REJECT_CANDIDATE(tySym, "empty type name");

    if (namedTySym->name()->kind() != decl.nameK_)
        return REJECT_CANDIDATE(tySym, "name kind mismatch");

    if (namedTySym->name()->kind() == SymbolNameKind::Tagged) {
        if (namedTySym->name()->asTagSymbolName()->kind() != decl.tagK_)
            return REJECT_CANDIDATE(tySym, "tag kind mismatch");
    }

    if (namedTySym->name()->text() != decl.name_)
        return REJECT_CANDIDATE(tySym, "type name mismatch");

    return true;
}

bool symbolMatchesBinding(const std::unique_ptr<Symbol>& sym, const DeclSummary& summary)
{
    const Symbol* candSym = sym.get();

    if (candSym->kind() != summary.symK_)
        return REJECT_CANDIDATE(candSym, "symbol kind mismatch");

    if (summary.scopeK_ != ScopeKind::UNSPECIFIED) {
        if (candSym->scope()->kind() != summary.scopeK_)
            return REJECT_CANDIDATE(candSym, "scope kind mismatch");
    }

//    if (summary.nsK_ != NameSpaceKind::UNSPECIFIED) {
//        if (candSym->nameSpace()->kind() != summary.nsK_)
//            return REJECT_CANDIDATE(candSym, "name space kind mismatch");
//    }

    switch (summary.symK_)
    {
        case SymbolKind::Value:
            return valueMatchesBinding(candSym->asValue(), summary);

        case SymbolKind::Type: {
            return typeMatchesBinding(candSym->asType(), summary);
        }

        case SymbolKind::Function:
            return functionMatchesBinding(candSym->asFunction(), summary);

        default:
            PSYCHE_TEST_FAIL("unknkown symbol kind");
            return false;
    }

    return false;
};

} // anonymous

void BinderTest::bind(std::string text, Expectation X)
{
    parse(text);
    auto compilation = Compilation::create(tree_->filePath());
    compilation->addSyntaxTrees({ tree_.get() });
    compilation->semanticModel(tree_.get());

    if (!checkErrorAndWarn(X))
        return;

    auto sym = compilation->assembly()->findSymDEF(
                [] (const auto& sym) {
                    return sym->kind() == SymbolKind::Library;
                });
    if (sym == nullptr)
        PSYCHE_TEST_FAIL("link unit not found");

    for (const auto& binding : X.bindings_) {
#ifdef DEBUG_BINDING_SEARCH
        std::cout << "\n\t\t...";
#endif
        using namespace std::placeholders;

        auto pred = std::bind(symbolMatchesBinding, _1, binding);
        auto sym = compilation->assembly()->findSymDEF(pred);

        if (sym == nullptr) {
            auto s = "no symbol matches the expectation: "
                    + binding.name_ + " " + to_string(binding.symK_);
            PSYCHE_TEST_FAIL(s);
        }

#ifdef DEBUG_BINDING_SEARCH
        std::cout << "\n\t\tmatch! ";
#endif

    }
}
