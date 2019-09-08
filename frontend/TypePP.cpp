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

#include "TypePP.h"

#include "ConstraintSyntax.h"
#include "CoreTypes.h"
#include "Literals.h"
#include "Symbols.h"
#include <cassert>

using namespace psyche;

template <class SyntaxT>
TypePP<SyntaxT>::TypePP()
    : scope_(nullptr)
{}

template <class SyntaxT>
std::string TypePP<SyntaxT>::print(const FullySpecifiedType& fullType, const Scope* scope)
{
    text_.clear();
    scope_ = scope;
    visitType(fullType);
    return text_;
}

template <class SyntaxT>
void TypePP<SyntaxT>::visitType(const FullySpecifiedType& fullType)
{
    if (fullType.isUnsigned())
        text_.append(" unsigned ");
    accept(fullType.type());
    if (fullType.isConst())
        text_.append(" const ");
    if (fullType.isVolatile())
        text_.append(" volatile ");
}

template <class SyntaxT>
void TypePP<SyntaxT>::visit(UndefinedType* )
{
    text_.append("int");
}

template <class SyntaxT>
void TypePP<SyntaxT>::visit(VoidType* )
{
    text_.append("void");
}

template <class SyntaxT>
void TypePP<SyntaxT>::visit(IntegerType* ty)
{
    switch (ty->kind()) {
    case IntegerType::Char:
        text_.append("char");
        break;

    case IntegerType::Short:
        text_.append("short");
        break;

    case IntegerType::Long:
        text_.append("long");
        break;

    case IntegerType::LongLong:
        text_.append("long long");
        break;

    default:
        text_.append("int");
        break;
    }
}

template <class SyntaxT>
void TypePP<SyntaxT>::visit(FloatType* ty)
{
    switch (ty->kind()) {
    case FloatType::Float:
        text_.append("float");
        break;

    case FloatType::LongDouble:
        text_.append("long double");
        break;

    default:
        text_.append("double");
        break;
    }
}

template <class SyntaxT>
void TypePP<SyntaxT>::visit(PointerToMemberType* ty)
{
    text_.append(".*");
    visitType(ty->elementType());
}

template <class SyntaxT>
void TypePP<SyntaxT>::visit(PointerType* ty)
{
    visitType(ty->elementType());
    text_.append("*");
}

template <class SyntaxT>
void TypePP<SyntaxT>::visit(ReferenceType* )
{
    text_.append("&");
}

template <class SyntaxT>
void TypePP<SyntaxT>::visit(ArrayType* ty)
{
    visitType(ty->elementType());
    text_.append("*"); // We treat arrays as pointers.
}

template <class SyntaxT>
void TypePP<SyntaxT>::name(const Name* name)
{
    const Identifier* ident = nullptr;
    if (name->asTaggedNameId()) {
        auto tag = name->asTaggedNameId()->tag();
        if (tag == TaggedNameId::Struct)
            text_.append("struct ");
        else if (tag == TaggedNameId::Enum)
            text_.append("enum ");
        else
            text_.append("union ");
        ident = name->asTaggedNameId()->name()->identifier();
    } else {
        ident = name->asNameId()->identifier();
    }
    text_.append(ident->begin(), ident->end());
}

template <class SyntaxT>
void TypePP<SyntaxT>::visit(NamedType* ty)
{
    name(ty->name());
}

template <class SyntaxT>
void TypePP<SyntaxT>::visit(QuantifiedType* ty)
{
    name(ty->name());
    if (ty->kind() == QuantifiedType::Existential) {
        text_.append("_Ex");
        text_.append(std::to_string(ty->label()));
    }
}

template <class SyntaxT>
void TypePP<SyntaxT>::funcParam(Function* ty)
{
    text_.append(SyntaxT::paramLDelim());
    if (ty->argumentCount()) {
        visitType(ty->argumentAt(0)->type());
        for (auto i = 1u; i < ty->argumentCount(); ++i) {
            text_.append(",");
            visitType(ty->argumentAt(i)->type());
        }
    }
    text_.append(SyntaxT::paramRDelim());
}

template <class SyntaxT>
void TypePP<SyntaxT>::funcRet(Function* ty)
{
    if (ty->returnType()->isNamedType()) {
        const Name* name = ty->returnType()->asNamedType()->name();
        assert(name->isNameId() && "expected a simple name");
        const Identifier* id = name->asNameId()->identifier();
        text_.append(std::string(id->begin(), id->end()));
    } else {
        visitType(ty->returnType());
    }
}

template<class SyntaxT>
void TypePP<SyntaxT>::func(Function* ty, RetParam)
{
    funcRet(ty);
    if (ty->hasArguments()) {
        text_.append(SyntaxT::retParamSep());
        funcParam(ty);
    }
}

template<class SyntaxT>
void TypePP<SyntaxT>::func(Function* ty, ParamRet)
{
    if (ty->hasArguments()) {
        funcParam(ty);
        text_.append(SyntaxT::retParamSep());
    }
    funcRet(ty);
}

template <class SyntaxT>
void TypePP<SyntaxT>::visit(Function *ty)
{
    text_.append(SyntaxT::funcLDelim());
    func(ty, typename SyntaxT::SigOrder());
    text_.append(SyntaxT::funcRDelim());
}

template <class SyntaxT>
void TypePP<SyntaxT>::visit(Class* ty)
{
    const Name* name = ty->name();
    std::string declName;
    if (name && (name->asNameId() || name->asTaggedNameId())) {
        const Identifier* id = name->identifier();
        declName.assign(id->begin(), id->end());
    } else {
        declName.assign("");
    }

    text_.append("struct ");
    text_.append(declName);
    text_.append("{ ");
    for (unsigned i = 0; i < ty->memberCount(); ++i) {
        const Name* name = ty->memberAt(i)->name();
        if (!name || name->asEmptyName())
            continue;

        visitType(ty->memberAt(i)->type());

        assert((name->isNameId() || name->isTaggedNameId()) && "expected a simple name");
        const Identifier* id = name->identifier();
        text_.append(" ");
        text_.append(id->chars(), id->size());
        text_.append("; ");
    }
    text_.append("}");
}

template <class SyntaxT>
void TypePP<SyntaxT>::visit(Enum*)
{
    text_.append("enum ");
}

template <class SyntaxT>
void TypePP<SyntaxT>::visit(ForwardClassDeclaration*)
{
    text_.append("<forward>");
}

namespace psyche {
    // Explicit instantiations.
    template class TypePP<ConstraintSyntax>;
    template class TypePP<CSyntax>;
}
