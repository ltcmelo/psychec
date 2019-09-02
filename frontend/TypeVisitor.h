// Copyright (c) 2008 Roberto Raggi <roberto.raggi@gmail.com>
// Copyright (c) 2019 Leandro T. C. Melo <ltcmelo@gmail.com>
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

#ifndef PSYCHE_TYPE_VISITOR_H__
#define PSYCHE_TYPE_VISITOR_H__

#include "FrontendConfig.h"

#include "FrontendFwds.h"

namespace psyche {

class CFE_API TypeVisitor
{
    TypeVisitor(const TypeVisitor &other);
    void operator =(const TypeVisitor &other);

public:
    TypeVisitor();
    virtual ~TypeVisitor();

    void accept(Type* type);

    virtual bool preVisit(Type*) { return true; }
    virtual void postVisit(Type*) {}

    virtual void visit(UndefinedType*) {}
    virtual void visit(VoidType*) {}
    virtual void visit(IntegerType*) {}
    virtual void visit(FloatType*) {}
    virtual void visit(PointerToMemberType*) {}
    virtual void visit(PointerType*) {}
    virtual void visit(ReferenceType*) {}
    virtual void visit(ArrayType*) {}
    virtual void visit(NamedType*) {}
    virtual void visit(QuantifiedType*) {}
    virtual void visit(Function*) {}
    virtual void visit(Namespace*) {}
    virtual void visit(Template*) {}
    virtual void visit(Class*) {}
    virtual void visit(Enum*) {}
    virtual void visit(ForwardClassDeclaration*) {}
};

} // namespace psyche

#endif
