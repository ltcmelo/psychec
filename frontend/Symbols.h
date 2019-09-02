// Copyright (c) 2008 Roberto Raggi <roberto.raggi@gmail.com>
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

#ifndef PSYCHE_SYMBOLS_H__
#define PSYCHE_SYMBOLS_H__

#include "FrontendConfig.h"

#include "FullySpecifiedType.h"
#include "Scope.h"
#include "Symbol.h"
#include "Type.h"
#include <vector>

namespace psyche {

class CFE_API UsingNamespaceDirective: public Symbol
{
public:
    UsingNamespaceDirective(TranslationUnit *translationUnit, unsigned sourceLocation, const Name* name);
    UsingNamespaceDirective(Clone *clone, Subst *subst, UsingNamespaceDirective *original);
    virtual ~UsingNamespaceDirective();

    // Symbol's interface
    virtual FullySpecifiedType type() const;

    virtual const UsingNamespaceDirective *asUsingNamespaceDirective() const
    { return this; }

    virtual UsingNamespaceDirective *asUsingNamespaceDirective()
    { return this; }

protected:
    virtual void visitSymbol0(SymbolVisitor* visitor);
};

class CFE_API UsingDeclaration: public Symbol
{
public:
    UsingDeclaration(TranslationUnit *translationUnit, unsigned sourceLocation, const Name* name);
    UsingDeclaration(Clone *clone, Subst *subst, UsingDeclaration *original);
    virtual ~UsingDeclaration();

    // Symbol's interface
    virtual FullySpecifiedType type() const;

    virtual const UsingDeclaration *asUsingDeclaration() const
    { return this; }

    virtual UsingDeclaration *asUsingDeclaration()
    { return this; }

protected:
    virtual void visitSymbol0(SymbolVisitor* visitor);
};

class CFE_API NamespaceAlias: public Symbol
{
public:
    NamespaceAlias(TranslationUnit *translationUnit, unsigned sourceLocation, const Name* name);
    NamespaceAlias(Clone *clone, Subst *subst, NamespaceAlias *original);
    virtual ~NamespaceAlias();

    const Name* namespaceName() const;
    void setNamespaceName(const Name* namespaceName);

    // Symbol's interface
    virtual FullySpecifiedType type() const;

    virtual const NamespaceAlias *asNamespaceAlias() const
    { return this; }

    virtual NamespaceAlias *asNamespaceAlias()
    { return this; }

protected:
    virtual void visitSymbol0(SymbolVisitor* visitor);

private:
    const Name* _namespaceName;
};

class CFE_API Declaration: public Symbol
{
public:
    Declaration(TranslationUnit *translationUnit, unsigned sourceLocation, const Name* name);
    Declaration(Clone *clone, Subst *subst, Declaration *original);
    virtual ~Declaration();

    void setType(const FullySpecifiedType &type);
    void setInitializer(StringLiteral const* initializer);

    // Symbol's interface
    virtual FullySpecifiedType type() const;
    const StringLiteral* getInitializer() const;

    virtual const Declaration *asDeclaration() const
    { return this; }

    virtual Declaration *asDeclaration()
    { return this; }

    virtual EnumeratorDeclaration *asEnumeratorDeclarator()
    { return 0; }

    virtual const EnumeratorDeclaration *asEnumeratorDeclarator() const
    { return 0; }

protected:
    virtual void visitSymbol0(SymbolVisitor* visitor);

private:
    FullySpecifiedType _type;
    const StringLiteral* _initializer;
};

class CFE_API EnumeratorDeclaration: public Declaration
{
public:
    EnumeratorDeclaration(TranslationUnit *translationUnit, unsigned sourceLocation, const Name* name);
    virtual ~EnumeratorDeclaration();

    const StringLiteral* constantValue() const;
    void setConstantValue(const StringLiteral* constantValue);

    virtual EnumeratorDeclaration *asEnumeratorDeclarator()
    { return this; }

    virtual const EnumeratorDeclaration *asEnumeratorDeclarator() const
    { return this; }

private:
    const StringLiteral* _constantValue;
};

class CFE_API Argument: public Symbol
{
public:
    Argument(TranslationUnit *translationUnit, unsigned sourceLocation, const Name* name);
    Argument(Clone *clone, Subst *subst, Argument *original);
    virtual ~Argument();

    void setType(const FullySpecifiedType &type);

    bool hasInitializer() const;

    const StringLiteral* initializer() const;
    void setInitializer(const StringLiteral* initializer);

    // Symbol's interface
    virtual FullySpecifiedType type() const;

    virtual const Argument *asArgument() const
    { return this; }

    virtual Argument *asArgument()
    { return this; }

protected:
    virtual void visitSymbol0(SymbolVisitor* visitor);

private:
    const StringLiteral* _initializer;
    FullySpecifiedType _type;
};

class CFE_API TypenameArgument: public Symbol
{
public:
    TypenameArgument(TranslationUnit *translationUnit, unsigned sourceLocation, const Name* name);
    TypenameArgument(Clone *clone, Subst *subst, TypenameArgument *original);
    virtual ~TypenameArgument();

    void setType(const FullySpecifiedType &type);

    // Symbol's interface
    virtual FullySpecifiedType type() const;

    virtual const TypenameArgument *asTypenameArgument() const
    { return this; }

    virtual TypenameArgument *asTypenameArgument()
    { return this; }

protected:
    virtual void visitSymbol0(SymbolVisitor* visitor);

private:
    FullySpecifiedType _type;
};

class CFE_API Block: public Scope
{
public:
    Block(TranslationUnit *translationUnit, unsigned sourceLocation);
    Block(Clone *clone, Subst *subst, Block *original);
    virtual ~Block();

    // Symbol's interface
    virtual FullySpecifiedType type() const;

    virtual const Block *asBlock() const
    { return this; }

    virtual Block *asBlock()
    { return this; }

protected:
    virtual void visitSymbol0(SymbolVisitor* visitor);
};

class CFE_API ForwardClassDeclaration: public Symbol, public Type
{
public:
    ForwardClassDeclaration(TranslationUnit *translationUnit, unsigned sourceLocation, const Name* name);
    ForwardClassDeclaration(Clone *clone, Subst *subst, ForwardClassDeclaration *original);
    virtual ~ForwardClassDeclaration();

    // Symbol's interface
    virtual FullySpecifiedType type() const;

    virtual const ForwardClassDeclaration *asForwardClassDeclaration() const
    { return this; }

    virtual ForwardClassDeclaration *asForwardClassDeclaration()
    { return this; }

    // Type's interface
    virtual const ForwardClassDeclaration *asForwardClassDeclarationType() const
    { return this; }

    virtual ForwardClassDeclaration *asForwardClassDeclarationType()
    { return this; }

protected:
    virtual void visitSymbol0(SymbolVisitor* visitor);
    virtual void accept0(TypeVisitor* visitor);
};

class CFE_API Enum: public Scope, public Type
{
public:
    Enum(TranslationUnit *translationUnit, unsigned sourceLocation, const Name* name);
    Enum(Clone *clone, Subst *subst, Enum *original);
    virtual ~Enum();

    bool isScoped() const;
    void setScoped(bool scoped);

    // Symbol's interface
    virtual FullySpecifiedType type() const;

    virtual const Enum *asEnum() const
    { return this; }

    virtual Enum *asEnum()
    { return this; }

    // Type's interface
    virtual const Enum *asEnumType() const
    { return this; }

    virtual Enum *asEnumType()
    { return this; }

protected:
    virtual void visitSymbol0(SymbolVisitor* visitor);
    virtual void accept0(TypeVisitor* visitor);

private:
    bool _isScoped;
};

class CFE_API Function: public Scope, public Type
{
public:
    enum MethodKey {
        NormalMethod,
        SlotMethod,
        SignalMethod,
        InvokableMethod
    };

public:
    Function(TranslationUnit *translationUnit, unsigned sourceLocation, const Name* name);
    Function(Clone *clone, Subst *subst, Function *original);
    virtual ~Function();

    bool isNormal() const;
    bool isSignal() const;
    bool isSlot() const;
    bool isInvokable() const;
    int methodKey() const;
    void setMethodKey(int key);

    FullySpecifiedType returnType() const;
    void setReturnType(const FullySpecifiedType &returnType);

    /** Convenience function that returns whether the function returns something (including void). */
    bool hasReturnType() const;

    unsigned argumentCount() const;
    Symbol* argumentAt(unsigned index) const;

    /** Convenience function that returns whether the function receives any arguments. */
    bool hasArguments() const;
    unsigned minimumArgumentCount() const;

    bool isVirtual() const;
    void setVirtual(bool isVirtual);

    bool isOverride() const;
    void setOverride(bool isOverride);

    bool isFinal() const;
    void setFinal(bool isFinal);

    bool isVariadic() const;
    void setVariadic(bool isVariadic);

    bool isConst() const;
    void setConst(bool isConst);

    bool isVolatile() const;
    void setVolatile(bool isVolatile);

    bool isPureVirtual() const;
    void setPureVirtual(bool isPureVirtual);

    bool isAmbiguous() const; // internal
    void setAmbiguous(bool isAmbiguous); // internal

    bool isGeneric() const;
    void setIsGeneric(bool isGeneric);

    bool maybeValidPrototype(unsigned actualArgumentCount) const;

    // Symbol's interface
    virtual FullySpecifiedType type() const;

    virtual const Function *asFunction() const
    { return this; }

    virtual Function *asFunction()
    { return this; }

    // Type's interface
    virtual const Function *asFunctionType() const
    { return this; }

    virtual Function *asFunctionType()
    { return this; }

protected:
    virtual void visitSymbol0(SymbolVisitor* visitor);
    virtual void accept0(TypeVisitor* visitor);

private:
    FullySpecifiedType _returnType;
    struct Flags {
        unsigned _isVirtual: 1;
        unsigned _isOverride: 1;
        unsigned _isFinal: 1;
        unsigned _isVariadic: 1;
        unsigned _isPureVirtual: 1;
        unsigned _isConst: 1;
        unsigned _isVolatile: 1;
        unsigned _isAmbiguous: 1;
        unsigned _isGeneric: 1;
        unsigned _methodKey: 3;
    };
    union {
        unsigned _flags;
        Flags f;
    };
};

class CFE_API Template: public Scope, public Type
{
public:
    Template(TranslationUnit *translationUnit, unsigned sourceLocation, const Name* name);
    Template(Clone *clone, Subst *subst, Template *original);
    virtual ~Template();

    unsigned templateParameterCount() const;
    Symbol* templateParameterAt(unsigned index) const;
    Symbol* declaration() const;

    // Symbol's interface
    virtual FullySpecifiedType type() const;

    virtual const Template *asTemplate() const
    { return this; }

    virtual Template *asTemplate()
    { return this; }

    // Type's interface
    virtual const Template *asTemplateType() const
    { return this; }

    virtual Template *asTemplateType()
    { return this; }

protected:
    virtual void visitSymbol0(SymbolVisitor* visitor);
    virtual void accept0(TypeVisitor* visitor);
};


class CFE_API Namespace: public Scope, public Type
{
public:
    Namespace(TranslationUnit *translationUnit, unsigned sourceLocation, const Name* name);
    Namespace(Clone *clone, Subst *subst, Namespace *original);
    virtual ~Namespace();

    // Symbol's interface
    virtual FullySpecifiedType type() const;

    virtual const Namespace *asNamespace() const
    { return this; }

    virtual Namespace *asNamespace()
    { return this; }

    // Type's interface
    virtual const Namespace *asNamespaceType() const
    { return this; }

    virtual Namespace *asNamespaceType()
    { return this; }

    bool isInline() const
    { return _isInline; }

    void setInline(bool onoff)
    { _isInline = onoff; }

protected:
    virtual void visitSymbol0(SymbolVisitor* visitor);
    virtual void accept0(TypeVisitor* visitor);

private:
    bool _isInline;
};

class CFE_API BaseClass: public Symbol
{
public:
    BaseClass(TranslationUnit *translationUnit, unsigned sourceLocation, const Name* name);
    BaseClass(Clone *clone, Subst *subst, BaseClass *original);
    virtual ~BaseClass();

    bool isVirtual() const;
    void setVirtual(bool isVirtual);

    bool isVariadic() const;
    void setVariadic(bool isVariadic);

    // Symbol's interface
    virtual FullySpecifiedType type() const;
    void setType(const FullySpecifiedType &type);

    virtual const BaseClass *asBaseClass() const
    { return this; }

    virtual BaseClass *asBaseClass()
    { return this; }

protected:
    virtual void visitSymbol0(SymbolVisitor* visitor);

private:
    bool _isVariadic;
    bool _isVirtual;
    FullySpecifiedType _type;
};

class CFE_API Class: public Scope, public Type
{
public:
    Class(TranslationUnit *translationUnit, unsigned sourceLocation, const Name* name);
    Class(Clone *clone, Subst *subst, Class *original);
    virtual ~Class();

    enum Key {
        ClassKey = 0,
        StructKey,
        UnionKey
    };

    bool isClass() const;
    bool isStruct() const;
    bool isUnion() const;
    Key classKey() const;
    void setClassKey(Key key);

    unsigned baseClassCount() const;
    BaseClass *baseClassAt(unsigned index) const;
    void addBaseClass(BaseClass *baseClass);

    // Symbol's interface
    virtual FullySpecifiedType type() const;

    virtual const Class *asClass() const
    { return this; }

    virtual Class *asClass()
    { return this; }

    // Type's interface
    virtual const Class *asClassType() const
    { return this; }

    virtual Class *asClassType()
    { return this; }

protected:
    virtual void visitSymbol0(SymbolVisitor* visitor);
    virtual void accept0(TypeVisitor* visitor);

private:
    Key _key;
    std::vector<BaseClass *> _baseClasses;
};

} // namespace psyche


#endif
