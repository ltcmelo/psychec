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
        auto p = n.find("case0");
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

bool REJECT(const Symbol* sym, std::string msg)
{
#ifdef DEBUG_BINDING_SEARCH
    std::cout << "\n\t\treject " << to_string(*sym) << "\t(" << msg << ")";
#endif
    return false;
}

bool hasExpectedCVR(CVR cvr, const TypeSymbol* tySym, const Symbol* candSym)
{
    switch (cvr) {
        case CVR::Const:
            if (!tySym->isConstQualified())
                return REJECT(candSym, "not const");
            break;

        case CVR::Volatile:
            if (!tySym->isVolatileQualified())
                return REJECT(candSym, "not volatile");
            break;

        case CVR::ConstAndVolatile:
            if (!(tySym->isConstQualified())
                    || !(tySym->isVolatileQualified())) {
                return REJECT(candSym, "not const/volatile");
            }
            break;

        case CVR::Restrict:
            if (!tySym->isRestrictQualified())
                return REJECT(candSym, "not restrict");
            break;

        case CVR::None:
            if (tySym->isConstQualified())
                return REJECT(candSym, "const");
            if (tySym->isVolatileQualified())
                return REJECT(candSym, "volatile");
            if (tySym->isRestrictQualified())
                return REJECT(candSym, "restrict");
            break;
    }

    return true;
}

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

        auto pred = [&binding] (const auto& p)
        {
            const Symbol* candSym = p.get();

            if (candSym->kind() != binding.symK_)
                return REJECT(candSym, "symbol kind mismatch");

            switch (binding.symK_)
            {
                case SymbolKind::Value: {
                    if (candSym->asValue()->valueKind() != binding.valK_)
                        return REJECT(candSym, "value kind mismatch");

                    const ValueSymbol* valSym = candSym->asValue();

                    if (!valSym->name())
                        return REJECT(candSym, "empty name");

                    if (valSym->name()->text() != binding.name_)
                        return REJECT(candSym, "name mistmatch");

                    if (valSym->type() == nullptr)
                        return REJECT(candSym, "null type");

                    const TypeSymbol* tySym = valSym->type();
                    for (auto i = binding.derivTyKs_.size(); i > 0; --i) {
                        auto derivTyK = binding.derivTyKs_[i - 1];
                        if (derivTyK != tySym->typeKind())
                            return REJECT(candSym, "(derived) type kind mismatch");

                        auto derivTyCVR = binding.derivTyCVRs_[i - 1];
                        if (!hasExpectedCVR(derivTyCVR, tySym, candSym))
                            return false;

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
                    if (namedTySym->name() == nullptr)
                        return REJECT(candSym, "null type name");

                    if (namedTySym->name()->text() != binding.specTyName_)
                        return REJECT(candSym, std::string("type name mismatch ") + namedTySym->name()->text() + " " + binding.specTyName_);

                    if (namedTySym->namedTypeKind() != binding.specTyK_)
                        return REJECT(candSym, "type kind mismatch");

                    if (binding.specTyBuiltinK_ != BuiltinTypeKind::None) {
                        if (!tySym->asNamedType())
                            return REJECT(candSym, "not a builtin");

                        if (tySym->asNamedType()->builtinTypeKind() != binding.specTyBuiltinK_)
                            return REJECT(candSym, "builtin kind mismatch");
                    }

                    if (!hasExpectedCVR(binding.specTyCVR_, tySym, candSym))
                        return false;

                    return true;
                }

                case SymbolKind::Type: {
                    if (candSym->asType()->typeKind() != binding.tyK_)
                        return false;
                    throw std::runtime_error("");
                }

                case SymbolKind::Function: {
                    if (!candSym->asFunction())
                        return false;
                    break;
                }

                default:
                    PSYCHE_TEST_FAIL("unknkown symbol kind");
                    return false;
            }

            return false;
        };

        auto sym = compilation->assembly()->findSymDEF(pred);

        if (sym == nullptr) {
            auto s = "incorrect/non-existing binding: "
                    + binding.name_ + " " + to_string(binding.valK_);
            PSYCHE_TEST_FAIL(s);
        }
    }
}
