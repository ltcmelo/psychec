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

#include "FullySpecifiedType.h"

#include "CoreTypes.h"
#include "Type.h"

using namespace psyche;

FullySpecifiedType::FullySpecifiedType(Type* type)
    : type_(type)
    , flags_(0)
{
    if (!type)
        type_ = UndefinedType::instance();
}

FullySpecifiedType::~FullySpecifiedType()
{ }

bool FullySpecifiedType::isValid() const
{ return type_ != UndefinedType::instance(); }

Type* FullySpecifiedType::type() const
{ return type_; }

void FullySpecifiedType::setType(Type* type)
{ type_ = type; }

FullySpecifiedType FullySpecifiedType::strippedType() const
{
    FullySpecifiedType ty = *this;

    ty.setFriend(false);
    ty.setRegister(false);
    ty.setStatic(false);
    ty.setExtern(false);
    ty.setMutable(false);
    ty.setTypedef(false);

    ty.setInline(false);
    ty.setVirtual(false);
    ty.setOverride(false);
    ty.setFinal(false);
    ty.setExplicit(false);

    ty.setDeprecated(false);
    ty.setUnavailable(false);

    return ty;
}

FullySpecifiedType FullySpecifiedType::coreType() const
{
    if (type()->asArrayType())
        return type()->asArrayType()->elementType().coreType();

    if (type()->asPointerType())
        return type()->asPointerType()->elementType().coreType();

    if (type()->asReferenceType())
        return type()->asReferenceType()->elementType().coreType();

    return *this;
}

bool FullySpecifiedType::isConst() const
{ return f.isConst_; }

void FullySpecifiedType::setConst(bool isConst)
{ f.isConst_ = isConst; }

bool FullySpecifiedType::isVolatile() const
{ return f.isVolatile_; }

void FullySpecifiedType::setVolatile(bool isVolatile)
{ f.isVolatile_ = isVolatile; }

bool FullySpecifiedType::isSigned() const
{ return f.isSigned_; }

void FullySpecifiedType::setSigned(bool isSigned)
{ f.isSigned_ = isSigned; }

bool FullySpecifiedType::isUnsigned() const
{ return f.isUnsigned_; }

void FullySpecifiedType::setUnsigned(bool isUnsigned)
{ f.isUnsigned_ = isUnsigned; }

bool FullySpecifiedType::isFriend() const
{ return f._isFriend; }

void FullySpecifiedType::setFriend(bool isFriend)
{ f._isFriend = isFriend; }

bool FullySpecifiedType::isAuto() const
{ return f._isAuto; }

void FullySpecifiedType::setAuto(bool isAuto)
{ f._isAuto = isAuto; }

bool FullySpecifiedType::isRegister() const
{ return f._isRegister; }

void FullySpecifiedType::setRegister(bool isRegister)
{ f._isRegister = isRegister; }

bool FullySpecifiedType::isStatic() const
{ return f._isStatic; }

void FullySpecifiedType::setStatic(bool isStatic)
{ f._isStatic = isStatic; }

bool FullySpecifiedType::isExtern() const
{ return f._isExtern; }

void FullySpecifiedType::setExtern(bool isExtern)
{ f._isExtern = isExtern; }

bool FullySpecifiedType::isMutable() const
{ return f._isMutable; }

void FullySpecifiedType::setMutable(bool isMutable)
{ f._isMutable = isMutable; }

bool FullySpecifiedType::isTypedef() const
{ return f._isTypedef; }

void FullySpecifiedType::setTypedef(bool isTypedef)
{ f._isTypedef = isTypedef; }

bool FullySpecifiedType::isInline() const
{ return f._isInline; }

void FullySpecifiedType::setInline(bool isInline)
{ f._isInline = isInline; }

bool FullySpecifiedType::isVirtual() const
{ return f._isVirtual; }

void FullySpecifiedType::setVirtual(bool isVirtual)
{ f._isVirtual = isVirtual; }

bool FullySpecifiedType::isOverride() const
{ return f._isOverride; }

void FullySpecifiedType::setOverride(bool isOverride)
{ f._isOverride = isOverride; }

bool FullySpecifiedType::isFinal() const
{ return f._isFinal; }

void FullySpecifiedType::setFinal(bool isFinal)
{ f._isFinal = isFinal; }

bool FullySpecifiedType::isExplicit() const
{ return f._isExplicit; }

void FullySpecifiedType::setExplicit(bool isExplicit)
{ f._isExplicit = isExplicit; }

bool FullySpecifiedType::isDeprecated() const
{ return f.isDeprecated_; }

void FullySpecifiedType::setDeprecated(bool isDeprecated)
{ f.isDeprecated_ = isDeprecated; }

bool FullySpecifiedType::isUnavailable() const
{ return f.isUnavailable_; }

void FullySpecifiedType::setUnavailable(bool isUnavailable)
{ f.isUnavailable_ = isUnavailable; }

Type& FullySpecifiedType::operator*()
{ return *type_; }

FullySpecifiedType::operator bool() const
{ return type_ != UndefinedType::instance(); }

const Type& FullySpecifiedType::operator*() const
{ return *type_; }

Type* FullySpecifiedType::operator->()
{ return type_; }

const Type* FullySpecifiedType::operator->() const
{ return type_; }

bool FullySpecifiedType::operator == (const FullySpecifiedType& other) const
{ return type_ == other.type_ && flags_ == other.flags_; }

bool FullySpecifiedType::operator != (const FullySpecifiedType& other) const
{ return ! operator ==(other); }

bool FullySpecifiedType::operator < (const FullySpecifiedType& other) const
{
    if (type_ == other.type_)
        return flags_ < other.flags_;
    return type_ < other.type_;
}

unsigned FullySpecifiedType::flags() const
{ return flags_; }

void FullySpecifiedType::setFlags(unsigned flags)
{ flags_ = flags; }
