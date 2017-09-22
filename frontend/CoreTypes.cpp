// Copyright (c) 2008 Roberto Raggi <roberto.raggi@gmail.com>
//
// Modifications:
// Copyright (c) 2016,17 Leandro T. C. Melo (ltcmelo@gmail.com)
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
#include "TypeVisitor.h"
#include "Names.h"
#include <algorithm>

using namespace psyche;

void UndefinedType::accept0(TypeVisitor *visitor)
{ visitor->visit(this); }

void VoidType::accept0(TypeVisitor *visitor)
{ visitor->visit(this); }

PointerToMemberType::PointerToMemberType(const Name *memberName, const FullySpecifiedType &elementType)
    : _memberName(memberName),
      _elementType(elementType)
{ }

PointerToMemberType::~PointerToMemberType()
{ }

const Name *PointerToMemberType::memberName() const
{ return _memberName; }

FullySpecifiedType PointerToMemberType::elementType() const
{ return _elementType; }

void PointerToMemberType::accept0(TypeVisitor *visitor)
{ visitor->visit(this); }

PointerType::PointerType(const FullySpecifiedType &elementType)
    : _elementType(elementType)
{ }

PointerType::~PointerType()
{ }

void PointerType::accept0(TypeVisitor *visitor)
{ visitor->visit(this); }

FullySpecifiedType PointerType::elementType() const
{ return _elementType; }

ReferenceType::ReferenceType(const FullySpecifiedType &elementType, bool rvalueRef)
    : _elementType(elementType), _rvalueReference(rvalueRef)
{ }

ReferenceType::~ReferenceType()
{ }

void ReferenceType::accept0(TypeVisitor *visitor)
{ visitor->visit(this); }

FullySpecifiedType ReferenceType::elementType() const
{ return _elementType; }

bool ReferenceType::isRvalueReference() const
{ return _rvalueReference; }

IntegerType::IntegerType(int kind)
    : _kind(kind)
{ }

IntegerType::~IntegerType()
{ }

void IntegerType::accept0(TypeVisitor *visitor)
{ visitor->visit(this); }

int IntegerType::kind() const
{ return _kind; }

unsigned int IntegerType::rank() const
{
    switch (_kind) {
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

FloatType::FloatType(int kind)
    : _kind(kind)
{ }

FloatType::~FloatType()
{ }

void FloatType::accept0(TypeVisitor *visitor)
{ visitor->visit(this); }

int FloatType::kind() const
{ return _kind; }

ArrayType::ArrayType(const FullySpecifiedType &elementType, unsigned size)
    : _elementType(elementType), _size(size)
{ }

ArrayType::~ArrayType()
{ }

void ArrayType::accept0(TypeVisitor *visitor)
{ visitor->visit(this); }

FullySpecifiedType ArrayType::elementType() const
{ return _elementType; }

unsigned ArrayType::size() const
{ return _size; }

NamedType::NamedType(const Name *name)
    : _name(name)
{ }

NamedType::~NamedType()
{ }

const Name *NamedType::name() const
{ return _name; }

void NamedType::accept0(TypeVisitor *visitor)
{ visitor->visit(this); }
