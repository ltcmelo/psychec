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

#include "TestExpectation.h"

using namespace psy;
using namespace  C;

Binding& Binding::nameAndKind(std::string name, ValueKind kind)
{
    name_ = std::move(name);
    kind_ = kind;
    return *this;
}

Binding& Binding::specType(std::string name, TypeKind kind, BuiltinTypeKind builtinKind, CVR cvr)
{
    specTyName_ = std::move(name);
    specTyKind_ = kind;
    specBuiltinTyKind_ = builtinKind;
    specCVR_ = cvr;
    return *this;
}

Binding& Binding::type(TypeKind kind, CVR cvr)
{
    tyKinds_.push_back(kind);
    CVRs_.push_back(cvr);
    return *this;
}

Expectation::Expectation()
    : numE_(0)
    , numW_(0)
    , isAmbiguous_(false)
{}

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
    isAmbiguous_ = true;
    ambiguityText_ = std::move(s);
    return *this;
}

Expectation& Expectation::binding(Binding b)
{
    bindings_.emplace_back(std::move(b));
    return *this;
}

Expectation& Expectation::AST(std::vector<SyntaxKind>&& v)
{
    syntaxKinds_ = std::move(v);
    return *this;
}

Expectation&
Expectation::obj(const std::string& valSymName,
                               ValueKind valKind,
                               const std::string& tySymName,
                               TypeKind tyKind,
                               BuiltinTypeKind builtTyKind)
{
    objs_.push_back(std::make_tuple(valSymName,
                                    valKind,
                                    tySymName,
                                    tyKind,
                                    builtTyKind));
    return *this;
}

Expectation&
Expectation::qualObj(const std::string& valSymName,
                     ValueKind valKind,
                     const std::string& tySymName,
                     Qual qual,
                     TypeKind tyKind,
                     BuiltinTypeKind builtTyKind)
{
    qualObjs_.push_back(std::make_tuple(valSymName,
                                        valKind,
                                        tySymName,
                                        qual,
                                        tyKind,
                                        builtTyKind));
    return *this;
}

Expectation&
Expectation::objPtr_1(const std::string& valSymName,
                                    ValueKind valKind,
                                    TypeKind refedTyKind,
                                    BuiltinTypeKind refedTyBuiltTyKind)
{
    objsPtr_1_.push_back(std::make_tuple(valSymName,
                                         valKind,
                                         refedTyKind,
                                         refedTyBuiltTyKind));
    return *this;
}

Expectation&
Expectation::qualPtr_1(const std::string& valSymName,
                                     ValueKind valKind,
                                     Qual qual,
                                     TypeKind refedTyKind,
                                     BuiltinTypeKind refedTyBuiltTyKind)
{
    qualPtr_1_.push_back(std::make_tuple(valSymName,
                                         valKind,
                                         qual,
                                         refedTyKind,
                                         refedTyBuiltTyKind));
    return *this;
}

Expectation&
Expectation::qualObjPtr_1(const std::string& valSymName,
                                        ValueKind valKind,
                                        Qual qual,
                                        TypeKind refedTyKind,
                                        BuiltinTypeKind refedTyBuiltTyKind)
{
    qualObjsPtr_1_.push_back(std::make_tuple(valSymName,
                                             valKind,
                                             qual,
                                             refedTyKind,
                                             refedTyBuiltTyKind));
    return *this;
}

Expectation&
Expectation::qualObjQualPtr_1(const std::string& valSymName,
                                            ValueKind valKind,
                                            Qual qual,
                                            Qual qualPtr,
                                            TypeKind refedTyKind,
                                            BuiltinTypeKind refedTyBuiltTyKind)
{
    qualObjsQualPtr_1_.push_back(std::make_tuple(valSymName,
                                                 valKind,
                                                 qual,
                                                 qualPtr,
                                                 refedTyKind,
                                                 refedTyBuiltTyKind));
    return *this;
}

Expectation&
Expectation::arr_1(const std::string& valSymName,
                                 ValueKind valKind,
                                 TypeKind elemTyKind,
                                 BuiltinTypeKind elemTyBuiltTyKind)
{
    arr_1_.push_back(std::make_tuple(valSymName,
                                     valKind,
                                     elemTyKind,
                                     elemTyBuiltTyKind));
    return *this;
}

Expectation&
Expectation::arr_1_ofQualTy(const std::string& valSymName,
                                          ValueKind valKind,
                                          Qual qual,
                                          TypeKind elemTyKind,
                                          BuiltinTypeKind elemTyBuiltTyKind)
{
    arr_1_ofQualTy_.push_back(std::make_tuple(valSymName,
                                              valKind,
                                              qual,
                                              elemTyKind,
                                              elemTyBuiltTyKind));
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
