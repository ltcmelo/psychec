/******************************************************************************
 * Copyright (c) 2016 Leandro T. C. Melo (ltcmelo@gmail.com)
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301
 * USA
 *****************************************************************************/

#include "TypeNameSpeller.h"
#include "Assert.h"
#include "ConstraintStreamWriter.h"
#include "CoreTypes.h"
#include "Literals.h"
#include "Symbols.h"
#include "FreshVarSupply.h"
#include "Utils.h"

using namespace CPlusPlus;
using namespace psyche;

TypeNameSpeller::TypeNameSpeller(ConstraintStreamWriter *writer)
    : scope_(nullptr)
{}

std::string TypeNameSpeller::spellTypeName(const FullySpecifiedType& fullType,
                                           const Scope* scope)
{
    spelling_.clear();
    scope_ = scope;
    visitType(fullType);
    return spelling_;
}

void TypeNameSpeller::visitType(const FullySpecifiedType& fullType)
{
    if (fullType.isUnsigned())
        spelling_.append(" unsigned ");
    accept(fullType.type());
    if (fullType.isConst())
        spelling_.append(" const ");
}

void TypeNameSpeller::visit(UndefinedType *)
{
    spelling_.append("int");
}

void TypeNameSpeller::visit(VoidType *)
{
    spelling_.append("void");
}

void TypeNameSpeller::visit(IntegerType *ty)
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

void TypeNameSpeller::visit(FloatType *ty)
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

void TypeNameSpeller::visit(PointerToMemberType *ty)
{
    spelling_.append(".*");
    visitType(ty->elementType());
}

void TypeNameSpeller::visit(PointerType *ty)
{
    visitType(ty->elementType());
    spelling_.append("*");
}

void TypeNameSpeller::visit(ReferenceType *)
{
    spelling_.append("&");
}

void TypeNameSpeller::visit(ArrayType *ty)
{
    visitType(ty->elementType());
    spelling_.append("*"); // We treat arrays as pointers.
}

void TypeNameSpeller::visit(NamedType *ty)
{
    const Identifier *id = ty->name()->asNameId()->identifier();
    std::string name(id->begin(), id->end());
    auto spec = ty->elaborateSpec();
    if (spec == NamedType::Enum)
        spelling_.append("enum ");
    else if (spec != NamedType::None)
        spelling_.append("struct ");
    spelling_.append(name);
}

void TypeNameSpeller::visit(Function *ty)
{
    spelling_.append("(");
    for (int i = 0; i < ty->argumentCount(); i++) {
        visitType(ty->argumentAt(i)->type());
        spelling_.append(",");
    }

    if (ty->returnType()->isNamedType()) {
        const Name *name = ty->returnType()->asNamedType()->name();
        PSYCHE_ASSERT(name->isNameId(), return, "expected a simple name");
        const Identifier *id = name->asNameId()->identifier();
        spelling_.append(std::string(id->begin(), id->end()));
    } else {
        visitType(ty->returnType());
    }
    spelling_.append(")");
}

void TypeNameSpeller::visit(Class *ty)
{
    const Name *name = ty->name();
    std::string declName;
    if (name && name->asNameId()) {
        const Identifier *id = name->asNameId()->identifier();
        declName.assign(id->begin(), id->end());
    } else {
        declName.assign("anonymous__");
    }

    spelling_.append("struct ");
    spelling_.append(declName);
    spelling_.append("{ ");
    for (unsigned i = 0; i < ty->memberCount(); ++i) {
        Symbol* member = ty->memberAt(i);
        visitType(member->type());
        const Name *name = member->name();
        PSYCHE_ASSERT(name->isNameId(), return, "expected a simple name");
        const Identifier *id = name->asNameId()->identifier();
        spelling_.append(" ");
        spelling_.append(id->chars(), id->size());
        spelling_.append("; ");
    }
    spelling_.append("}");
}

void TypeNameSpeller::visit(Enum *)
{
    spelling_.append("enum ");
}

void TypeNameSpeller::visit(ForwardClassDeclaration *)
{
    spelling_.append("<forward>");
}
