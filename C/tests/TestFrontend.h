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

#ifndef PSYCHE_C_TEST_FRONTEND_H__
#define PSYCHE_C_TEST_FRONTEND_H__

#include "API.h"
#include "SyntaxTree.h"

#include "compilation/Compilation.h"
#include "symbols/BuiltinTypeKind.h"
#include "symbols/SymbolKind.h"
#include "symbols/ValueKind.h"
#include "symbols/TypeKind.h"
#include "tests/TestRunner.h"

#include <functional>
#include <string>
#include <tuple>
#include <utility>
#include <vector>

#define CROSS_REFERENCE_TEST(CASE) { auto x = &CASE; (void)x; } \

namespace psy {
namespace C {

class TestFrontend : public TestRunner
{
public:
    struct Expectation
    {
        Expectation(int numE = 0, int numW = 0)
            : numE_(numE)
            , numW_(numW)
            , hasAmbiguity_(false)
        {}

        void setErrorCnt(int numE);
        void setWarnCnt(int numW);

        enum class ErrorOrWarn
        {
            Error,
            Warn
        };

        Expectation& addDiagnostic(ErrorOrWarn v, std::string descriptorId = "");

        int numE_;
        int numW_;
        std::vector<std::string> descriptorsE_;
        std::vector<std::string> descriptorsW_;

        Expectation& AST(std::vector<SyntaxKind>&& v);
        Expectation& replicateAmbiguity(const std::string& s = "");

        bool hasAmbiguity_;
        std::string ambiguousText_;
        std::vector<SyntaxKind> syntaxKinds_;


        // objects

        using ObjData = std::tuple<std::string,
                                   ValueKind,
                                   std::string,
                                   TypeKind,
                                   BuiltinTypeKind>;
        std::vector<ObjData> objs_;

        Expectation& obj(const std::string& valSymName,
                         ValueKind valKind,
                         const std::string& tySymName,
                         TypeKind tyKind,
                         BuiltinTypeKind builtTyKind = BuiltinTypeKind::None);


        // qualified type objects

        enum class Qual
        {
            Const,
            Volatile,
            Restrict,
            ConstAndVolatile,
        };

        using QualObjData = std::tuple<std::string,
                                       ValueKind,
                                       std::string,
                                       Qual,
                                       TypeKind,
                                       BuiltinTypeKind>;
        std::vector<QualObjData> qualObjs_;

        Expectation& qualObj(const std::string& valSymName,
                             ValueKind valKind,
                             const std::string& tySymName,
                             Qual qual,
                             TypeKind tyKind,
                             BuiltinTypeKind builtTyKind = BuiltinTypeKind::None);


        // pointer objects

        using ObjPtr_1_Data = std::tuple<std::string,
                                         ValueKind,
                                         TypeKind,
                                         BuiltinTypeKind>;
        std::vector<ObjPtr_1_Data> objsPtr_1_;

        Expectation& objPtr_1(const std::string& valSymName,
                              ValueKind valKind,
                              TypeKind refedTyKind,
                              BuiltinTypeKind refedTyBuiltTyKind = BuiltinTypeKind::None);

        // qualified type pointer objects

        using QualObjPtr_1_Data = std::tuple<std::string,
                                             ValueKind,
                                             Qual,
                                             TypeKind,
                                             BuiltinTypeKind>;
        std::vector<QualObjPtr_1_Data> qualObjsPtr_1_;

        Expectation& qualObjPtr_1(const std::string& valSymName,
                                  ValueKind valKind,
                                  Qual qual,
                                  TypeKind refedTyKind,
                                  BuiltinTypeKind refedTyBuiltTyKind = BuiltinTypeKind::None);

    };

protected:
    TestFrontend();
    ~TestFrontend();

    bool checkErrorAndWarn(Expectation X);

    void parse(std::string text,
               Expectation X = Expectation(),
               SyntaxTree::SyntaxCategory cat = SyntaxTree::SyntaxCategory::Unspecified);
    void parseDeclaration(std::string text,
                          Expectation X = Expectation());
    void parseExpression(std::string text,
                         Expectation X = Expectation());
    void parseStatement(std::string text,
                        Expectation X = Expectation());
    void bind(std::string text,
              Expectation X = Expectation());
    void typeCheck(std::string text);

    std::unique_ptr<SyntaxTree> tree_;
    std::unique_ptr<Compilation> compilation_;
};

} // C
} // psy

#endif
