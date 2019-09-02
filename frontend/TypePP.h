// Copyright (c) 2016 Leandro T. C. Melo <ltcmelo@gmail.com>
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

#ifndef PSYCHE_TYPE_PP_H__
#define PSYCHE_TYPE_PP_H__

#include "FrontendConfig.h"

#include "CoreTypes.h"
#include "CSyntax.h"
#include "Literals.h"
#include "Symbols.h"
#include "TypeVisitor.h"
#include <string>

namespace psyche {

template <class SyntaxT>
class CFE_API TypePP : public TypeVisitor
{
public:
    TypePP();

    std::string print(const FullySpecifiedType& fullType, const Scope* scope);

protected:
    void visitType(const FullySpecifiedType&);

    void visit(UndefinedType*) override;
    void visit(VoidType*) override;
    void visit(IntegerType*) override;
    void visit(FloatType*) override;
    void visit(PointerToMemberType*) override;
    void visit(PointerType*) override;
    void visit(ReferenceType*) override;
    void visit(ArrayType*) override;
    void visit(NamedType*) override;
    void visit(QuantifiedType*) override;
    void visit(Function*) override;
    void visit(Class*) override;
    void visit(Enum*) override;
    void visit(ForwardClassDeclaration*) override;

    void func(Function*, RetParam);
    void func(Function*, ParamRet);
    virtual void funcParam(Function*);
    virtual void funcRet(Function*);

    void name(const Name*);

    std::string text_;
    const Scope* scope_;
};

} // namespace psyche

#endif
