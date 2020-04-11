// Copyright (c) 2008 Roberto Raggi <roberto.raggi@gmail.com>
// Copyright (c) 2016-20 Leandro T. C. Melo <ltcmelo@gmail.com>
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

#include "CoreTypes.h"

#include "Names.h"
#include "TypeVisitor.h"
#include <algorithm>

using namespace psyche;

void UndefinedType::accept0(TypeVisitor* visitor)
{ visitor->visit(this); }

void VoidType::accept0(TypeVisitor* visitor)
{ visitor->visit(this); }

PointerToMemberType::PointerToMemberType(const Name* memberName, const FullySpecifiedType &elementType)
    : memberName_(memberName)
    , elementType_(elementType)
{}

PointerToMemberType::~PointerToMemberType()
{}

const Name* PointerToMemberType::memberName() const
{ return memberName_; }

FullySpecifiedType PointerToMemberType::elementType() const
{ return elementType_; }

void PointerToMemberType::accept0(TypeVisitor* visitor)
{ visitor->visit(this); }

PointerType::PointerType(const FullySpecifiedType &elementType)
    : elementType_(elementType)
{}

PointerType::~PointerType()
{}

void PointerType::accept0(TypeVisitor* visitor)
{ visitor->visit(this); }

FullySpecifiedType PointerType::elementType() const
{ return elementType_; }

ReferenceType::ReferenceType(const FullySpecifiedType &elementType)
    : elementType_(elementType)
{}

ReferenceType::~ReferenceType()
{}

void ReferenceType::accept0(TypeVisitor* visitor)
{ visitor->visit(this); }

FullySpecifiedType ReferenceType::elementType() const
{ return elementType_; }

IntegerType::IntegerType(Kind kind)
    : kind_(kind)
{}

IntegerType::~IntegerType()
{}

void IntegerType::accept0(TypeVisitor* visitor)
{ visitor->visit(this); }

IntegerType::Kind IntegerType::kind() const
{ return kind_; }

unsigned int IntegerType::rank() const
{
    switch (kind_) {
    case Bool:
        return 1;

    case Char:
    case Char16:
    case Char32:
        return 2;

    case Short:
        return 3;

    case Int:
        return 4;

    case Long:
        return 5;

    case LongLong:
        return 6;

    default:
        return 0;
    }
}

FloatType::FloatType(Kind kind)
    : kind_(kind)
{}

FloatType::~FloatType()
{}

void FloatType::accept0(TypeVisitor* visitor)
{ visitor->visit(this); }

FloatType::Kind FloatType::kind() const
{ return kind_; }

ArrayType::ArrayType(const FullySpecifiedType &elementType, unsigned size)
    : elementType_(elementType)
    , size_(size)
{}

ArrayType::~ArrayType()
{}

void ArrayType::accept0(TypeVisitor* visitor)
{ visitor->visit(this); }

FullySpecifiedType ArrayType::elementType() const
{ return elementType_; }

unsigned ArrayType::size() const
{ return size_; }

NamedType::NamedType(const Name* name)
    : name_(name)
{}

NamedType::~NamedType()
{}

const Name* NamedType::name() const
{ return name_; }

void NamedType::accept0(TypeVisitor* visitor)
{ visitor->visit(this); }

QuantifiedType::QuantifiedType(const Name* name, QuantifiedType::Kind kind, int label)
    : name_(name)
    , kind_(kind)
    , label_(label)
{}

QuantifiedType::~QuantifiedType()
{}

const Name* QuantifiedType::name() const
{ return name_; }

QuantifiedType::Kind QuantifiedType::kind() const
{ return kind_; }

int QuantifiedType::label() const
{ return label_; }

void QuantifiedType::accept0(TypeVisitor* visitor)
{ visitor->visit(this); }
