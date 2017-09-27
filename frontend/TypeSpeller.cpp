/******************************************************************************
 Copyright (c) 2016-17 Leandro T. C. Melo (ltcmelo@gmail.com)

 This library is free software; you can redistribute it and/or modify it under
 the terms of the GNU Lesser General Public License as published by the Free
 Software Foundation; either version 2.1 of the License, or (at your option)
 any later version.

 This library is distributed in the hope that it will be useful, but WITHOUT
 ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public License
 for more details.

 You should have received a copy of the GNU Lesser General Public License along
 with this library; if not, write to the Free Software Foundation, Inc., 51
 Franklin Street, Fifth Floor, Boston, MA  02110-1301 USA
 *****************************************************************************/

#include "TypeSpeller.h"
#include "CoreTypes.h"
#include "ConstraintSyntax.h"
#include "Literals.h"
#include "Symbols.h"
#include <cassert>

using namespace psyche;

template <class SyntaxT>
TypeSpeller<SyntaxT>::TypeSpeller()
    : scope_(nullptr)
{}

template <class SyntaxT>
std::string TypeSpeller<SyntaxT>::spell(const FullySpecifiedType& fullType,
                                        const Scope* scope)
{
    spelling_.clear();
    scope_ = scope;
    visitType(fullType);
    return spelling_;
}

template <class SyntaxT>
void TypeSpeller<SyntaxT>::visitType(const FullySpecifiedType& fullType)
{
    if (fullType.isUnsigned())
        spelling_.append(" unsigned ");
    accept(fullType.type());
    if (fullType.isConst())
        spelling_.append(" const ");
    if (fullType.isVolatile())
        spelling_.append(" volatile ");
}

template <class SyntaxT>
void TypeSpeller<SyntaxT>::visit(UndefinedType *)
{
    spelling_.append("int");
}

template <class SyntaxT>
void TypeSpeller<SyntaxT>::visit(VoidType *)
{
    spelling_.append("void");
}

template <class SyntaxT>
void TypeSpeller<SyntaxT>::visit(IntegerType *ty)
{
    switch (ty->kind()) {
    case IntegerType::Char:
        spelling_.append("char");
        break;

    case IntegerType::Short:
        spelling_.append("short");
        break;

    case IntegerType::Long:
        spelling_.append("long");
        break;

    case IntegerType::LongLong:
        spelling_.append("long long");
        break;

    default:
        spelling_.append("int");
        break;
    }
}

template <class SyntaxT>
void TypeSpeller<SyntaxT>::visit(FloatType *ty)
{
    switch (ty->kind()) {
    case FloatType::Float:
        spelling_.append("float");
        break;

    case FloatType::LongDouble:
        spelling_.append("long double");
        break;

    default:
        spelling_.append("double");
        break;
    }
}

template <class SyntaxT>
void TypeSpeller<SyntaxT>::visit(PointerToMemberType *ty)
{
    spelling_.append(".*");
    visitType(ty->elementType());
}

template <class SyntaxT>
void TypeSpeller<SyntaxT>::visit(PointerType *ty)
{
    visitType(ty->elementType());
    spelling_.append("*");
}

template <class SyntaxT>
void TypeSpeller<SyntaxT>::visit(ReferenceType *)
{
    spelling_.append("&");
}

template <class SyntaxT>
void TypeSpeller<SyntaxT>::visit(ArrayType *ty)
{
    visitType(ty->elementType());
    spelling_.append("*"); // We treat arrays as pointers.
}

template <class SyntaxT>
void TypeSpeller<SyntaxT>::visit(NamedType *ty)
{
    const Identifier *id = nullptr;
    if (ty->name()->asTaggedNameId()) {
        id = ty->name()->asTaggedNameId()->name()->identifier();
        auto tag = ty->name()->asTaggedNameId()->tag();
        if (tag == TaggedNameId::Struct)
            spelling_.append("struct ");
        else if (tag == TaggedNameId::Enum)
            spelling_.append("enum ");
        else
            spelling_.append("union ");
    } else {
        id = ty->name()->asNameId()->identifier();
    }
    std::string name(id->begin(), id->end());
    spelling_.append(name);
}

template <class SyntaxT>
void TypeSpeller<SyntaxT>::funcParam(Function* ty)
{
    spelling_.append(SyntaxT::paramLDelim());
    if (ty->argumentCount()) {
        visitType(ty->argumentAt(0)->type());
        for (auto i = 1u; i < ty->argumentCount(); ++i) {
            spelling_.append(",");
            visitType(ty->argumentAt(i)->type());
        }
    }
    spelling_.append(SyntaxT::paramRDelim());
}

template <class SyntaxT>
void TypeSpeller<SyntaxT>::funcRet(Function* ty)
{
    if (ty->returnType()->isNamedType()) {
        const Name *name = ty->returnType()->asNamedType()->name();
        assert(name->isNameId() && "expected a simple name");
        const Identifier *id = name->asNameId()->identifier();
        spelling_.append(std::string(id->begin(), id->end()));
    } else {
        visitType(ty->returnType());
    }
}

template<class SyntaxT>
void TypeSpeller<SyntaxT>::func(Function* ty, RetParam)
{
    funcRet(ty);
    if (ty->hasArguments()) {
        spelling_.append(SyntaxT::retParamSep());
        funcParam(ty);
    }
}

template<class SyntaxT>
void TypeSpeller<SyntaxT>::func(Function* ty, ParamRet)
{
    if (ty->hasArguments()) {
        funcParam(ty);
        spelling_.append(SyntaxT::retParamSep());
    }
    funcRet(ty);
}

template <class SyntaxT>
void TypeSpeller<SyntaxT>::visit(Function *ty)
{
    spelling_.append(SyntaxT::funcLDelim());
    func(ty, typename SyntaxT::SigOrder());
    spelling_.append(SyntaxT::funcRDelim());
}

template <class SyntaxT>
void TypeSpeller<SyntaxT>::visit(Class *ty)
{
    const Name *name = ty->name();
    std::string declName;
    if (name && (name->asNameId() || name->asTaggedNameId())) {
        const Identifier *id = name->identifier();
        declName.assign(id->begin(), id->end());
    } else {
        declName.assign("");
    }

    spelling_.append("struct ");
    spelling_.append(declName);
    spelling_.append("{ ");
    for (unsigned i = 0; i < ty->memberCount(); ++i) {
        const Name *name = ty->memberAt(i)->name();
        if (!name || name->asAnonymousNameId())
            continue;

        visitType(ty->memberAt(i)->type());

        assert((name->isNameId() || name->isTaggedNameId()) && "expected a simple name");
        const Identifier *id = name->identifier();
        spelling_.append(" ");
        spelling_.append(id->chars(), id->size());
        spelling_.append("; ");
    }
    spelling_.append("}");
}

template <class SyntaxT>
void TypeSpeller<SyntaxT>::visit(Enum *)
{
    spelling_.append("enum ");
}

template <class SyntaxT>
void TypeSpeller<SyntaxT>::visit(ForwardClassDeclaration *)
{
    spelling_.append("<forward>");
}

namespace psyche {
    // Explicit instantiations.
    template class TypeSpeller<ConstraintSyntax>;
    template class TypeSpeller<CSyntax>;
}
