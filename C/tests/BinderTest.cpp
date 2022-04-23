// Copyright (c) 2020/21 Leandro T. C. Melo <ltcmelo@gmail.com>
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
#include "symbols/Symbols.h"
#include "syntax/SyntaxLexemes.h"
#include "syntax/SyntaxNamePrinter.h"
#include "syntax/SyntaxNodes.h"

#define DEBUG_BINDING_SEARCH

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
    std::cout << "\n\t\treject " << to_string(*sym) << "\t(" << msg << ")";
#endif
    return false;
}

void UNMATCHED_EXPECTATION(std::string msg)
{
#ifdef DEBUG_BINDING_SEARCH
    std::cout << "\n\t\t\t" << msg << "!";
#endif
}

bool typeMatchesCVR(const TypeSymbol* tySym, CVR cvr)
{
    switch (cvr) {
        case CVR::Const:
            if (!tySym->isConstQualified()) {
                UNMATCHED_EXPECTATION("missing const");
                return false;
            }
            break;

        case CVR::Volatile:
            if (!tySym->isVolatileQualified()) {
                UNMATCHED_EXPECTATION("missing volatile");
                return false;
            }
            break;

        case CVR::ConstAndVolatile:
            if (!(tySym->isConstQualified())
                    || !(tySym->isVolatileQualified())) {
                UNMATCHED_EXPECTATION("missing const volatile");
                return false;
            }
            break;

        case CVR::Restrict:
            if (!tySym->isRestrictQualified()) {
                UNMATCHED_EXPECTATION("missing restrict");
                return false;
            }
            break;

        case CVR::None:
            if (tySym->isConstQualified()) {
                UNMATCHED_EXPECTATION("spurious const");
                return false;
            }
            if (tySym->isVolatileQualified()) {
                UNMATCHED_EXPECTATION("spurious volatile");
                return false;
            }
            if (tySym->isRestrictQualified()) {
                UNMATCHED_EXPECTATION("spurious restrict");
                return false;
            }
            break;
    }

    return true;
}

bool typeMatchesBinding(const TypeSymbol* tySym, const BindingSummary& binding)
{
    for (auto i = binding.derivTyKs_.size(); i > 0; --i) {
        auto derivTyK = binding.derivTyKs_[i - 1];
        if (derivTyK != tySym->typeKind()) {
            UNMATCHED_EXPECTATION("derived type kind mismatch");
            return false;
        }

        auto derivTyCVR = binding.derivTyCVRs_[i - 1];
        if (!typeMatchesCVR(tySym, derivTyCVR)) {
            UNMATCHED_EXPECTATION("derived type CVR mismatch");
            return false;
        }

        switch (tySym->typeKind()) {
            case TypeKind::Array:
                tySym = tySym->asArrayType()->elementType();
                break;

            case TypeKind::Pointer:
                tySym = tySym->asPointerType()->referencedType();
                break;

            default:
                PSYCHE_TEST_FAIL("unexpected");
                return false;
        }
    }

    const NamedTypeSymbol* namedTySym = tySym->asNamedType();
    if (!namedTySym) {
        UNMATCHED_EXPECTATION("not a named type");
        return false;
    }

    if (namedTySym->name() == nullptr) {
        UNMATCHED_EXPECTATION("null type name");
        return false;
    }

    if (namedTySym->name()->text() != binding.specTyName_) {
        UNMATCHED_EXPECTATION("type name mismatch");
        return false;
    }

    if (namedTySym->namedTypeKind() != binding.specTyK_) {
        UNMATCHED_EXPECTATION("type kind mismatch");
        return false;
    }

    if (binding.specTyBuiltinK_ != BuiltinTypeKind::None) {
        if (!tySym->asNamedType()) {
            UNMATCHED_EXPECTATION("not a builtin");
            return false;
        }

        if (tySym->asNamedType()->builtinTypeKind() != binding.specTyBuiltinK_) {
            UNMATCHED_EXPECTATION("builtin kind mismatch");
            return false;
        }
    }

    if (!typeMatchesCVR(tySym, binding.specTyCVR_)) {
        UNMATCHED_EXPECTATION("CVR mismatch");
        return false;
    }

    return true;
}

bool functionMatchesBinding(const FunctionSymbol* funcSym, const BindingSummary& binding)
{
    if (!funcSym->name())
        return REJECT_CANDIDATE(funcSym, "empty name");

    if (funcSym->name()->text() != binding.name_)
        return REJECT_CANDIDATE(funcSym, "name mistmatch");

    if (funcSym->type() == nullptr)
        return REJECT_CANDIDATE(funcSym, "null type");

    if (funcSym->type()->typeKind() != TypeKind::Function)
        return REJECT_CANDIDATE(funcSym, "not a function type");

    const FunctionTypeSymbol* funcTySym = funcSym->type()->asFunctionType();

    if (!typeMatchesBinding(funcTySym->returnType(), binding))
        return REJECT_CANDIDATE(funcSym, "return type mismatch");

    return true;
}

bool valueMatchesBinding(const ValueSymbol* valSym, const BindingSummary& binding)
{
    if (valSym->valueKind() != binding.valK_)
        return REJECT_CANDIDATE(valSym, "value kind mismatch");

    if (!valSym->name())
        return REJECT_CANDIDATE(valSym, "empty name");

    if (valSym->name()->text() != binding.name_)
        return REJECT_CANDIDATE(valSym, "name mistmatch");

    if (valSym->type() == nullptr)
        return REJECT_CANDIDATE(valSym, "null type");

    if (!typeMatchesBinding(valSym->type(), binding))
        return REJECT_CANDIDATE(valSym, "type mismatch");

    return true;
}

bool symbolMatchesBinding(const std::unique_ptr<Symbol>& sym, const BindingSummary& binding)
{
    const Symbol* candSym = sym.get();

    if (candSym->kind() != binding.symK_)
        return REJECT_CANDIDATE(candSym, "symbol kind mismatch");

    switch (binding.symK_)
    {
        case SymbolKind::Value:
            return valueMatchesBinding(candSym->asValue(), binding);

        case SymbolKind::Type: {
            if (candSym->asType()->typeKind() != binding.tyK_)
                return false;
            throw std::runtime_error("");
        }

        case SymbolKind::Function:
            return functionMatchesBinding(candSym->asFunction(), binding);

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
                    return sym->kind() == SymbolKind::LinkUnit;
                });
    if (sym == nullptr)
        PSYCHE_TEST_FAIL("link unit not found");

    for (const auto& binding : X.bindings_) {
        using namespace std::placeholders;

        auto pred = std::bind(symbolMatchesBinding, _1, binding);
        auto sym = compilation->assembly()->findSymDEF(pred);

        if (sym == nullptr) {
            auto s = "no symbol matches the expectation: "
                    + binding.name_ + " " + to_string(binding.symK_);
            PSYCHE_TEST_FAIL(s);
        }
    }
}
