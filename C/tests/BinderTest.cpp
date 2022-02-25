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

    for (auto objData : X.objs_) {
        auto valSymName = std::get<0>(objData);
        auto valKind = std::get<1>(objData);
        auto tySymName = std::get<2>(objData);
        auto tyKind = std::get<3>(objData);
        auto builtTyKind = std::get<4>(objData);

        auto sym = compilation->assembly()->findSymDEF(
                [&] (const auto& v) {
                    const Symbol* sym = v.get();
                    if (sym->kind() != SymbolKind::Value)
                        return false;

                    const ValueSymbol* actualSym = sym->asValue();
                    if (!(actualSym->name() != nullptr
                           && to_string(*actualSym->name()) == valSymName
                           && actualSym->valueKind() == valKind
                           && actualSym->type() != nullptr
                           && actualSym->type()->name() != nullptr
                           && to_string(*actualSym->type()->name()) == tySymName
                           && actualSym->type()->typeKind() == tyKind))
                        return false;

                    if (tyKind == TypeKind::Builtin) {
                        const NamedTypeSymbol* namedTySym = actualSym->type()->asNamedType();
                        if (!(namedTySym->builtinTypeKind() == builtTyKind))
                            return false;
                    }
                    else {
                        if (builtTyKind != BuiltinTypeKind::None)
                            return false;
                    }

                    return true;
                });

        if (sym == nullptr) {
            auto s = "cannot find "
                    + to_string(valKind) + " " + valSymName + " "
                    + to_string(tyKind) + " " + to_string(builtTyKind) + " "
                    + (tySymName == "" ? "<unnamed>" : tySymName);
            PSYCHE_TEST_FAIL(s);
        }
    }

    for (auto qualObjData : X.qualObjs_) {
        auto valSymName = std::get<0>(qualObjData);
        auto valKind = std::get<1>(qualObjData);
        auto tySymName = std::get<2>(qualObjData);
        auto qual = std::get<3>(qualObjData);
        auto tyKind = std::get<4>(qualObjData);
        auto builtTyKind = std::get<5>(qualObjData);

        auto sym = compilation->assembly()->findSymDEF(
                [&] (const auto& v) {
                    const Symbol* sym = v.get();
                    if (sym->kind() != SymbolKind::Value)
                        return false;

                    const ValueSymbol* actualSym = sym->asValue();
                    if (!(actualSym->name() != nullptr
                           && to_string(*actualSym->name()) == valSymName
                           && actualSym->valueKind() == valKind
                           && actualSym->type() != nullptr
                           && actualSym->type()->name() != nullptr
                           && to_string(*actualSym->type()->name()) == tySymName
                           && actualSym->type()->typeKind() == tyKind))
                        return false;

                    switch (qual) {
                        case Expectation::Qual::Const:
                            if (!actualSym->type()->isConstQualified())
                                return false;
                            break;

                        case Expectation::Qual::Volatile:
                            if (!actualSym->type()->isVolatileQualified())
                                return false;
                            break;

                        case Expectation::Qual::ConstAndVolatile:
                            if (!(actualSym->type()->isConstQualified())
                                    || !(actualSym->type()->isVolatileQualified()))
                            break;

                        case Expectation::Qual::Restrict:
                            if (!actualSym->type()->isRestrictQualified())
                                return false;
                            break;
                    }

                    if (tyKind == TypeKind::Builtin) {
                        const NamedTypeSymbol* namedTySym = actualSym->type()->asNamedType();
                        if (!(namedTySym->builtinTypeKind() == builtTyKind))
                            return false;
                    }
                    else {
                        if (builtTyKind != BuiltinTypeKind::None)
                            return false;
                    }

                    return true;
                });

        if (sym == nullptr) {
            auto s = "cannot find "
                    + to_string(valKind) + " " + valSymName + " "
                    + std::to_string((int)qual) + " "
                    + to_string(tyKind) + " " + to_string(builtTyKind) + " "
                    + (tySymName == "" ? "<unnamed>" : tySymName);
            PSYCHE_TEST_FAIL(s);
        }
    }

    for (auto objPtr_1_Data : X.objsPtr_1_) {
        auto valSymName = std::get<0>(objPtr_1_Data);
        auto valKind = std::get<1>(objPtr_1_Data);
        auto refedTyKind = std::get<2>(objPtr_1_Data);
        auto refedTyBuiltTyKind = std::get<3>(objPtr_1_Data);

        auto sym = compilation->assembly()->findSymDEF(
                [&] (const auto& v) {
                    const Symbol* sym = v.get();
                    if (sym->kind() != SymbolKind::Value)
                        return false;

                    const ValueSymbol* actualSym = sym->asValue();
                    if (!(actualSym->name() != nullptr
                           && to_string(*actualSym->name()) == valSymName
                           && actualSym->valueKind() == valKind
                           && actualSym->type() != nullptr
                           && actualSym->type()->typeKind() == TypeKind::Pointer))
                        return false;

                    const PointerTypeSymbol* ptrTySym = actualSym->type()->asPointerType();
                    const TypeSymbol* refedTySym = ptrTySym->referencedType();

                    if (refedTySym->typeKind() != refedTyKind)
                        return false;

                    if (refedTyKind == TypeKind::Builtin) {
                        const NamedTypeSymbol* namedTySym = ptrTySym->referencedType()->asNamedType();
                        if (!(namedTySym->builtinTypeKind() == refedTyBuiltTyKind))
                            return false;
                    }
                    else {
                        if (refedTyBuiltTyKind != BuiltinTypeKind::None)
                            return false;
                    }

                    return true;
                });

        if (sym == nullptr) {
            auto s = "cannot find "
                    + to_string(valKind) + " " + valSymName + " "
                    + to_string(refedTyKind) + " " + to_string(refedTyBuiltTyKind);
            PSYCHE_TEST_FAIL(s);
        }
    }

    for (auto qualPtr_1_Data : X.qualPtr_1_) {
        auto valSymName = std::get<0>(qualPtr_1_Data);
        auto valKind = std::get<1>(qualPtr_1_Data);
        auto qual = std::get<2>(qualPtr_1_Data);
        auto refedTyKind = std::get<3>(qualPtr_1_Data);
        auto refedTyBuiltTyKind = std::get<4>(qualPtr_1_Data);

        auto sym = compilation->assembly()->findSymDEF(
                [&] (const auto& v) {
                    const Symbol* sym = v.get();
                    if (sym->kind() != SymbolKind::Value)
                        return false;

                    const ValueSymbol* actualSym = sym->asValue();
                    if (!(actualSym->name() != nullptr
                           && to_string(*actualSym->name()) == valSymName
                           && actualSym->valueKind() == valKind
                           && actualSym->type() != nullptr
                           && actualSym->type()->typeKind() == TypeKind::Pointer))
                        return false;

                    const PointerTypeSymbol* ptrTySym = actualSym->type()->asPointerType();
                    switch (qual) {
                        case Expectation::Qual::Const:
                            if (!ptrTySym->isConstQualified())
                                return false;
                            break;

                        case Expectation::Qual::Volatile:
                            if (!ptrTySym->isVolatileQualified())
                                return false;
                            break;

                        case Expectation::Qual::ConstAndVolatile:
                            if (!(ptrTySym->isConstQualified())
                                    || !(ptrTySym->isVolatileQualified()))
                            break;

                        case Expectation::Qual::Restrict:
                            if (!ptrTySym->isRestrictQualified())
                                return false;
                            break;
                    }

                    const TypeSymbol* refedTySym = ptrTySym->referencedType();

                    if (refedTySym->typeKind() != refedTyKind)
                        return false;

                    if (refedTyKind == TypeKind::Builtin) {
                        const NamedTypeSymbol* namedTySym = refedTySym->asNamedType();
                        if (!(namedTySym->builtinTypeKind() == refedTyBuiltTyKind))
                            return false;
                    }
                    else {
                        if (refedTyBuiltTyKind != BuiltinTypeKind::None)
                            return false;
                    }

                    return true;
                });

        if (sym == nullptr) {
            auto s = "cannot find "
                    + to_string(valKind) + " " + valSymName + " "
                    + to_string(refedTyKind) + " " + to_string(refedTyBuiltTyKind);
            PSYCHE_TEST_FAIL(s);
        }
    }

    for (auto qualObjPtr_1_Data : X.qualObjsPtr_1_) {
        auto valSymName = std::get<0>(qualObjPtr_1_Data);
        auto valKind = std::get<1>(qualObjPtr_1_Data);
        auto qual = std::get<2>(qualObjPtr_1_Data);
        auto refedTyKind = std::get<3>(qualObjPtr_1_Data);
        auto refedTyBuiltTyKind = std::get<4>(qualObjPtr_1_Data);

        auto sym = compilation->assembly()->findSymDEF(
                [&] (const auto& v) {
                    const Symbol* sym = v.get();
                    if (sym->kind() != SymbolKind::Value)
                        return false;

                    const ValueSymbol* actualSym = sym->asValue();
                    if (!(actualSym->name() != nullptr
                           && to_string(*actualSym->name()) == valSymName
                           && actualSym->valueKind() == valKind
                           && actualSym->type() != nullptr
                           && actualSym->type()->typeKind() == TypeKind::Pointer))
                        return false;

                    const PointerTypeSymbol* ptrTySym = actualSym->type()->asPointerType();
                    const NamedTypeSymbol* namedTySym = ptrTySym->referencedType()->asNamedType();

                    switch (qual) {
                        case Expectation::Qual::Const:
                            if (!namedTySym->isConstQualified())
                                return false;
                            break;

                        case Expectation::Qual::Volatile:
                            if (!namedTySym->isVolatileQualified())
                                return false;
                            break;

                        case Expectation::Qual::ConstAndVolatile:
                            if (!(namedTySym->isConstQualified())
                                    || !(namedTySym->isVolatileQualified()))
                            break;

                        case Expectation::Qual::Restrict:
                            if (!namedTySym->isRestrictQualified())
                                return false;
                            break;
                    }

                    if (namedTySym->typeKind() != refedTyKind)
                        return false;

                    if (refedTyKind == TypeKind::Builtin) {
                        if (!(namedTySym->builtinTypeKind() == refedTyBuiltTyKind))
                            return false;
                    }
                    else {
                        if (refedTyBuiltTyKind != BuiltinTypeKind::None)
                            return false;
                    }

                    return true;
                });

        if (sym == nullptr) {
            auto s = "cannot find "
                    + to_string(valKind) + " " + valSymName + " "
                    + to_string(refedTyKind) + " " + to_string(refedTyBuiltTyKind);
            PSYCHE_TEST_FAIL(s);
        }
    }

    for (auto qualObjQualPtr_1_Data : X.qualObjsQualPtr_1_) {
        auto valSymName = std::get<0>(qualObjQualPtr_1_Data);
        auto valKind = std::get<1>(qualObjQualPtr_1_Data);
        auto qual = std::get<2>(qualObjQualPtr_1_Data);
        auto qualPtr = std::get<3>(qualObjQualPtr_1_Data);
        auto refedTyKind = std::get<4>(qualObjQualPtr_1_Data);
        auto refedTyBuiltTyKind = std::get<5>(qualObjQualPtr_1_Data);

        auto sym = compilation->assembly()->findSymDEF(
                [&] (const auto& v) {
                    const Symbol* sym = v.get();
                    if (sym->kind() != SymbolKind::Value)
                        return false;

                    const ValueSymbol* actualSym = sym->asValue();
                    if (!(actualSym->name() != nullptr
                           && to_string(*actualSym->name()) == valSymName
                           && actualSym->valueKind() == valKind
                           && actualSym->type() != nullptr
                           && actualSym->type()->typeKind() == TypeKind::Pointer))
                        return false;

                    const PointerTypeSymbol* ptrTySym = actualSym->type()->asPointerType();
                    const NamedTypeSymbol* namedTySym = ptrTySym->referencedType()->asNamedType();

                    switch (qualPtr) {
                        case Expectation::Qual::Const:
                            if (!ptrTySym->isConstQualified())
                                return false;
                            break;

                        case Expectation::Qual::Volatile:
                            if (!ptrTySym->isVolatileQualified())
                                return false;
                            break;

                        case Expectation::Qual::ConstAndVolatile:
                            if (!(ptrTySym->isConstQualified())
                                    || !(ptrTySym->isVolatileQualified()))
                            break;

                        case Expectation::Qual::Restrict:
                            if (!ptrTySym->isRestrictQualified())
                                return false;
                            break;
                    }

                    switch (qual) {
                        case Expectation::Qual::Const:
                            if (!namedTySym->isConstQualified())
                                return false;
                            break;

                        case Expectation::Qual::Volatile:
                            if (!namedTySym->isVolatileQualified())
                                return false;
                            break;

                        case Expectation::Qual::ConstAndVolatile:
                            if (!(namedTySym->isConstQualified())
                                    || !(namedTySym->isVolatileQualified()))
                            break;

                        case Expectation::Qual::Restrict:
                            if (!namedTySym->isRestrictQualified())
                                return false;
                            break;
                    }

                    if (namedTySym->typeKind() != refedTyKind)
                        return false;

                    if (refedTyKind == TypeKind::Builtin) {
                        if (!(namedTySym->builtinTypeKind() == refedTyBuiltTyKind))
                            return false;
                    }
                    else {
                        if (refedTyBuiltTyKind != BuiltinTypeKind::None)
                            return false;
                    }

                    return true;
                });

        if (sym == nullptr) {
            auto s = "cannot find "
                    + to_string(valKind) + " " + valSymName + " "
                    + to_string(refedTyKind) + " " + to_string(refedTyBuiltTyKind);
            PSYCHE_TEST_FAIL(s);
        }
    }

    for (auto arr_1_Data : X.arr_1_) {
        auto valSymName = std::get<0>(arr_1_Data);
        auto valKind = std::get<1>(arr_1_Data);
        auto elemTyKind = std::get<2>(arr_1_Data);
        auto elemTyBuiltTyKind = std::get<3>(arr_1_Data);

        auto sym = compilation->assembly()->findSymDEF(
                [&] (const auto& v) {
                    const Symbol* sym = v.get();
                    if (sym->kind() != SymbolKind::Value)
                        return false;

                    const ValueSymbol* actualSym = sym->asValue();
                    if (!(actualSym->name() != nullptr
                           && to_string(*actualSym->name()) == valSymName
                           && actualSym->valueKind() == valKind
                           && actualSym->type() != nullptr
                           && actualSym->type()->typeKind() == TypeKind::Array))
                        return false;

                    const ArrayTypeSymbol* arrTySym = actualSym->type()->asArrayType();
                    const TypeSymbol* elemTySym = arrTySym->elementType();

                    if (elemTySym->typeKind() != elemTyKind)
                        return false;

                    if (elemTyKind == TypeKind::Builtin) {
                        const NamedTypeSymbol* namedTySym = elemTySym->asNamedType();
                        if (!(namedTySym->builtinTypeKind() == elemTyBuiltTyKind))
                            return false;
                    }
                    else {
                        if (elemTyBuiltTyKind != BuiltinTypeKind::None)
                            return false;
                    }

                    return true;
                });

        if (sym == nullptr) {
            auto s = "cannot find "
                    + to_string(valKind) + " " + valSymName + " "
                    + to_string(elemTyKind) + " " + to_string(elemTyBuiltTyKind);
            PSYCHE_TEST_FAIL(s);
        }
    }

    for (auto arr_1_QualTyData : X.arr_1_ofQualTy_) {
        auto valSymName = std::get<0>(arr_1_QualTyData);
        auto valKind = std::get<1>(arr_1_QualTyData);
        auto qual = std::get<2>(arr_1_QualTyData);
        auto elemTyKind = std::get<3>(arr_1_QualTyData);
        auto elemTyBuiltTyKind = std::get<4>(arr_1_QualTyData);

        auto sym = compilation->assembly()->findSymDEF(
                [&] (const auto& v) {
                    const Symbol* sym = v.get();
                    if (sym->kind() != SymbolKind::Value)
                        return false;

                    const ValueSymbol* actualSym = sym->asValue();
                    if (!(actualSym->name() != nullptr
                           && to_string(*actualSym->name()) == valSymName
                           && actualSym->valueKind() == valKind
                           && actualSym->type() != nullptr
                           && actualSym->type()->typeKind() == TypeKind::Array))
                        return false;

                    const ArrayTypeSymbol* arrTySym = actualSym->type()->asArrayType();
                    const TypeSymbol* elemTySym = arrTySym->elementType();

                    if (elemTySym->typeKind() != elemTyKind)
                        return false;

                    switch (qual) {
                        case Expectation::Qual::Const:
                            if (!elemTySym->isConstQualified())
                                return false;
                            break;

                        case Expectation::Qual::Volatile:
                            if (!elemTySym->isVolatileQualified())
                                return false;
                            break;

                        case Expectation::Qual::ConstAndVolatile:
                            if (!(elemTySym->isConstQualified())
                                    || !(elemTySym->isVolatileQualified()))
                            break;

                        case Expectation::Qual::Restrict:
                            if (!elemTySym->isRestrictQualified())
                                return false;
                            break;
                    }

                    if (elemTyKind == TypeKind::Builtin) {
                        const NamedTypeSymbol* namedTySym = arrTySym->elementType()->asNamedType();
                        if (!(namedTySym->builtinTypeKind() == elemTyBuiltTyKind))
                            return false;
                    }
                    else {
                        if (elemTyBuiltTyKind != BuiltinTypeKind::None)
                            return false;
                    }

                    return true;
                });

        if (sym == nullptr) {
            auto s = "cannot find "
                    + to_string(valKind) + " " + valSymName + " "
                    + to_string(elemTyKind) + " " + to_string(elemTyBuiltTyKind);
            PSYCHE_TEST_FAIL(s);
        }
    }
}
