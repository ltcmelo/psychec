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

#ifndef CFE_CORETYPES_H
#define CFE_CORETYPES_H

#include "FrontendConfig.h"
#include "Type.h"
#include "FullySpecifiedType.h"

namespace psyche {

class CFE_API UndefinedType : public Type
{
public:
    static UndefinedType *instance()
    {
        static UndefinedType t;
        return &t;
    }

    virtual const UndefinedType *asUndefinedType() const
    { return this; }

    virtual UndefinedType *asUndefinedType()
    { return this; }

protected:
    virtual void accept0(TypeVisitor *visitor);
};

class CFE_API VoidType: public Type
{
public:
    virtual const VoidType *asVoidType() const
    { return this; }

    virtual VoidType *asVoidType()
    { return this; }

protected:
    virtual void accept0(TypeVisitor *visitor);
};

class CFE_API IntegerType: public Type
{
public:
    enum Kind {
        Char,
        Char16,
        Char32,
        WideChar,
        Bool,
        Short,
        Int,
        Long,
        LongLong
    };

public:
    IntegerType(int kind);
    virtual ~IntegerType();

    int kind() const;

    unsigned int rank() const;

    virtual IntegerType *asIntegerType()
    { return this; }

    virtual const IntegerType *asIntegerType() const
    { return this; }

protected:
    virtual void accept0(TypeVisitor *visitor);

private:
    int _kind;
};

class CFE_API FloatType: public Type
{
public:
    enum Kind {
        Float,
        Double,
        LongDouble
    };

public:
    FloatType(int kind);
    virtual ~FloatType();

    int kind() const;

    virtual const FloatType *asFloatType() const
    { return this; }

    virtual FloatType *asFloatType()
    { return this; }

protected:
    virtual void accept0(TypeVisitor *visitor);

private:
    int _kind;
};

class CFE_API PointerType: public Type
{
public:
    PointerType(const FullySpecifiedType &elementType);
    virtual ~PointerType();

    FullySpecifiedType elementType() const;

    virtual const PointerType *asPointerType() const
    { return this; }

    virtual PointerType *asPointerType()
    { return this; }

protected:
    virtual void accept0(TypeVisitor *visitor);

private:
    FullySpecifiedType _elementType;
};

class CFE_API PointerToMemberType: public Type
{
public:
    PointerToMemberType(const Name *memberName, const FullySpecifiedType &elementType);
    virtual ~PointerToMemberType();

    const Name *memberName() const;
    FullySpecifiedType elementType() const;

    virtual const PointerToMemberType *asPointerToMemberType() const
    { return this; }

    virtual PointerToMemberType *asPointerToMemberType()
    { return this; }

protected:
    virtual void accept0(TypeVisitor *visitor);

private:
    const Name *_memberName;
    FullySpecifiedType _elementType;
};

class CFE_API ReferenceType: public Type
{
public:
    ReferenceType(const FullySpecifiedType &elementType, bool rvalueRef);
    virtual ~ReferenceType();

    FullySpecifiedType elementType() const;
    bool isRvalueReference() const;

    virtual const ReferenceType *asReferenceType() const
    { return this; }

    virtual ReferenceType *asReferenceType()
    { return this; }

protected:
    virtual void accept0(TypeVisitor *visitor);

private:
    FullySpecifiedType _elementType;
    bool _rvalueReference;
};

class CFE_API ArrayType: public Type
{
public:
    ArrayType(const FullySpecifiedType &elementType, unsigned size);
    virtual ~ArrayType();

    FullySpecifiedType elementType() const;
    unsigned size() const;

    virtual const ArrayType *asArrayType() const
    { return this; }

    virtual ArrayType *asArrayType()
    { return this; }

protected:
    virtual void accept0(TypeVisitor *visitor);

private:
    FullySpecifiedType _elementType;
    unsigned _size;
};

class CFE_API NamedType: public Type
{
public:
    NamedType(const Name *name);
    virtual ~NamedType();

    const Name *name() const;

    virtual const NamedType *asNamedType() const
    { return this; }

    virtual NamedType *asNamedType()
    { return this; }

protected:
    virtual void accept0(TypeVisitor *visitor);

private:
    const Name *_name;
};

} // namespace psyche

#endif // CFE_CORETYPES_H
