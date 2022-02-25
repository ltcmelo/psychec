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

#ifndef PSYCHE_C_TEST_EXPECTATION_H__
#define PSYCHE_C_TEST_EXPECTATION_H__

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

struct Binding
{
    Binding& nameAndKind(std::string name, ValueKind kind);
    Binding& specType(std::string name,
                      TypeKind kind,
                      BuiltinTypeKind builtinKind = BuiltinTypeKind::None,
                      CVR cvr = CVR::None);
    Binding& type(TypeKind kind, CVR cvr = CVR::None);

    std::string name_;
    ValueKind kind_;
    std::string specTyName_;
    TypeKind specTyKind_;
    BuiltinTypeKind specBuiltinTyKind_;
    CVR specCVR_;
    std::vector<TypeKind> tyKinds_;
    std::vector<CVR> CVRs_;
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

    std::vector<Binding> bindings_;
    Expectation& binding(Binding b);

    enum class Qual
    {
        Const,
        Volatile,
        Restrict,
        ConstAndVolatile,
    };

    /*
     * objects
     */
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

    /*
     * objects of a qualified type
     */
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


    /*
     * pointers
     */
    using ObjPtr_1_Data = std::tuple<std::string,
                                     ValueKind,
                                     TypeKind,
                                     BuiltinTypeKind>;
    std::vector<ObjPtr_1_Data> objsPtr_1_;

    Expectation& objPtr_1(const std::string& valSymName,
                          ValueKind valKind,
                          TypeKind refedTyKind,
                          BuiltinTypeKind refedTyBuiltTyKind = BuiltinTypeKind::None);

    /*
     * qualified pointers
     */
    using QualPtr_1_Data = std::tuple<std::string,
                                      ValueKind,
                                      Qual,
                                      TypeKind,
                                      BuiltinTypeKind>;
    std::vector<QualPtr_1_Data> qualPtr_1_;

    Expectation& qualPtr_1(const std::string& valSymName,
                           ValueKind valKind,
                           Qual qual,
                           TypeKind refedTyKind,
                           BuiltinTypeKind refedTyBuiltTyKind = BuiltinTypeKind::None);

    /*
     * pointers to objects with qualified type
     */
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

    /*
     * qualified pointers to objects with qualified type
     */
    using QualObjQualPtr_1_Data = std::tuple<std::string,
                                         ValueKind,
                                         Qual,
                                         Qual,
                                         TypeKind,
                                         BuiltinTypeKind>;
    std::vector<QualObjQualPtr_1_Data> qualObjsQualPtr_1_;

    Expectation& qualObjQualPtr_1(const std::string& valSymName,
                                  ValueKind valKind,
                                  Qual qual,
                                  Qual qualPtr,
                                  TypeKind refedTyKind,
                                  BuiltinTypeKind refedTyBuiltTyKind = BuiltinTypeKind::None);

    /*
     * arrays
     */
    using Arr_1_Data = std::tuple<std::string,
                                  ValueKind,
                                  TypeKind,
                                  BuiltinTypeKind>;
    std::vector<Arr_1_Data> arr_1_;
    Expectation& arr_1(const std::string& valSymName,
                       ValueKind valKind,
                       TypeKind elemTyKind,
                       BuiltinTypeKind elemTyBuiltTyKind = BuiltinTypeKind::None);

    /*
     * arrays of objects with qualified type
     */
    using Arr_1_QualTyData = std::tuple<std::string,
                                        ValueKind,
                                        Qual,
                                        TypeKind,
                                        BuiltinTypeKind>;
    std::vector<Arr_1_QualTyData> arr_1_ofQualTy_;
    Expectation& arr_1_ofQualTy(const std::string& valSymName,
                                ValueKind valKind,
                                Qual qual,
                                TypeKind elemTyKind,
                                BuiltinTypeKind elemTyBuiltTyKind = BuiltinTypeKind::None);
};

} // C
} // psy

#endif