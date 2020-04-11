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

#ifndef PSYCHE_AST_H__
#define PSYCHE_AST_H__

#include "FrontendConfig.h"

#include "ASTFwds.h"
#include "FullySpecifiedType.h"
#include "MemoryPool.h"
#include "SyntaxAmbiguity.h"
#include <iostream>
#include <memory>

namespace psyche {

template <class PtrT, class DerivedListT>
class CFE_API BaseList: public Managed
{
    BaseList(const BaseList& other) = delete;
    void operator =(const BaseList& other) = delete;

public:
    BaseList()
        : value(PtrT())
        , next(nullptr)
    {}

    BaseList(const PtrT& value)
        : value(value)
        , next(nullptr)
    {}

    unsigned firstToken() const
    {
        if (value)
            return value->firstToken();
        return 0;
    }

    unsigned lastToken() const
    {
        PtrT lv = lastValue();
        if (lv)
            return lv->lastToken();
        return 0;
    }

    PtrT lastValue() const
    {
        PtrT lastValue = nullptr;
        for (const DerivedListT* it = (DerivedListT*)this; it; it = it->next) {
            if (it->value)
                lastValue = it->value;
        }
        return lastValue;
    }

    PtrT value;
    DerivedListT* next;
};

template <class PtrT>
class CFE_API List: public BaseList<PtrT, List<PtrT>>
{
public:
    using BaseList<PtrT, List<PtrT>>::BaseList;
};

template <class PtrT>
class CFE_API SepList: public BaseList<PtrT, SepList<PtrT>>
{
public:
    using BaseList<PtrT, SepList<PtrT>>::BaseList;

    unsigned delim_token = 0;
};

class CFE_API AST: public Managed
{
    AST(const AST& other) = delete;
    void operator =(const AST& other) = delete;

public:
    AST();
    virtual ~AST();

    void accept(ASTVisitor* visitor);

    static void accept(AST* ast, ASTVisitor* visitor)
    { if (ast) ast->accept(visitor); }

    template <class PtrT, class DerivedListT>
    static void accept(BaseList<PtrT, DerivedListT>* it, ASTVisitor* visitor)
    {
        for (; it; it = it->next)
            accept(it->value, visitor);
    }

    static bool match(AST* ast, AST* pattern, ASTMatcher* matcher);
    bool match(AST* pattern, ASTMatcher* matcher);

    template <class PtrT, class DerivedListT>
    static bool match(BaseList<PtrT, DerivedListT>* it,
                      BaseList<PtrT, DerivedListT>* patternIt,
                      ASTMatcher* matcher)
    {
        while (it && patternIt) {
            if (! match(it->value, patternIt->value, matcher))
                return false;

            it = it->next;
            patternIt = patternIt->next;
        }

        if (! it && ! patternIt)
            return true;

        return false;
    }

    virtual unsigned firstToken() const = 0;
    virtual unsigned lastToken() const = 0;

    virtual AST* clone(MemoryPool *pool) const = 0;

    virtual AccessDeclarationAST* asAccessDeclaration() { return 0; }
    virtual AliasDeclarationAST* asAliasDeclaration() { return 0; }
    virtual AlignmentSpecifierAST* asAlignmentSpecifier() { return 0; }
    virtual AlignofExpressionAST* asAlignofExpression() { return 0; }
    virtual AmbiguousStatementAST* asAmbiguousStatement() { return 0; }
    virtual EmptyNameAST* asEmptyName() { return 0; }
    virtual ArrayAccessAST* asArrayAccess() { return 0; }
    virtual ArrayDeclaratorAST* asArrayDeclarator() { return 0; }
    virtual ArrayInitializerAST* asArrayInitializer() { return 0; }
    virtual AsmDefinitionAST* asAsmDefinition() { return 0; }
    virtual AttributeSpecifierAST* asAttributeSpecifier() { return 0; }
    virtual BaseSpecifierAST* asBaseSpecifier() { return 0; }
    virtual BinaryExpressionAST* asBinaryExpression() { return 0; }
    virtual BitfieldDeclaratorAST* asBitfieldDeclarator() { return 0; }
    virtual BoolLiteralAST* asBoolLiteral() { return 0; }
    virtual BracedInitializerAST* asBracedInitializer() { return 0; }
    virtual BracketDesignatorAST* asBracketDesignator() { return 0; }
    virtual BreakStatementAST* asBreakStatement() { return 0; }
    virtual CallAST* asCall() { return 0; }
    virtual CaptureAST* asCapture() { return 0; }
    virtual CaseStatementAST* asCaseStatement() { return 0; }
    virtual CastExpressionAST* asCastExpression() { return 0; }
    virtual CatchClauseAST* asCatchClause() { return 0; }
    virtual ClassSpecifierAST* asClassSpecifier() { return 0; }
    virtual CompoundExpressionAST* asCompoundExpression() { return 0; }
    virtual CompoundLiteralAST* asCompoundLiteral() { return 0; }
    virtual CompoundStatementAST* asCompoundStatement() { return 0; }
    virtual ConditionAST* asCondition() { return 0; }
    virtual ConditionalExpressionAST* asConditionalExpression() { return 0; }
    virtual ContinueStatementAST* asContinueStatement() { return 0; }
    virtual ConversionFunctionIdAST* asConversionFunctionId() { return 0; }
    virtual CoreDeclaratorAST* asCoreDeclarator() { return 0; }
    virtual CppCastExpressionAST* asCppCastExpression() { return 0; }
    virtual CtorInitializerAST* asCtorInitializer() { return 0; }
    virtual DeclarationAST* asDeclaration() { return 0; }
    virtual DeclarationStatementAST* asDeclarationStatement() { return 0; }
    virtual DeclaratorAST* asDeclarator() { return 0; }
    virtual DeclaratorIdAST* asDeclaratorId() { return 0; }
    virtual DecltypeSpecifierAST* asDecltypeSpecifier() { return 0; }
    virtual DeleteExpressionAST* asDeleteExpression() { return 0; }
    virtual DesignatedInitializerAST* asDesignatedInitializer() { return 0; }
    virtual DesignatorAST* asDesignator() { return 0; }
    virtual DestructorNameAST* asDestructorName() { return 0; }
    virtual DoStatementAST* asDoStatement() { return 0; }
    virtual DotDesignatorAST* asDotDesignator() { return 0; }
    virtual DynamicExceptionSpecificationAST* asDynamicExceptionSpecification() { return 0; }
    virtual TaggedNameAST* asTaggedName() { return 0; }
    virtual ElaboratedTypeSpecifierAST* asElaboratedTypeSpecifier() { return 0; }
    virtual EmptyDeclarationAST* asEmptyDeclaration() { return 0; }
    virtual EnumSpecifierAST* asEnumSpecifier() { return 0; }
    virtual EnumeratorAST* asEnumerator() { return 0; }
    virtual ExceptionDeclarationAST* asExceptionDeclaration() { return 0; }
    virtual ExceptionSpecificationAST* asExceptionSpecification() { return 0; }
    virtual ExpressionAST* asExpression() { return 0; }
    virtual ExpressionListParenAST* asExpressionListParen() { return 0; }
    virtual ExpressionOrDeclarationStatementAST* asExpressionOrDeclarationStatement() { return 0; }
    virtual ExpressionStatementAST* asExpressionStatement() { return 0; }
    virtual ForStatementAST* asForStatement() { return 0; }
    virtual ForeachStatementAST* asForeachStatement() { return 0; }
    virtual FunctionDeclaratorAST* asFunctionDeclarator() { return 0; }
    virtual FunctionDefinitionAST* asFunctionDefinition() { return 0; }
    virtual GenericsDeclarationAST* asGenericsDeclaration() { return 0; }
    virtual GnuAttributeAST* asGnuAttribute() { return 0; }
    virtual GnuAttributeSpecifierAST* asGnuAttributeSpecifier() { return 0; }
    virtual GotoStatementAST* asGotoStatement() { return 0; }
    virtual IdExpressionAST* asIdExpression() { return 0; }
    virtual IfStatementAST* asIfStatement() { return 0; }
    virtual LabeledStatementAST* asLabeledStatement() { return 0; }
    virtual LambdaCaptureAST* asLambdaCapture() { return 0; }
    virtual LambdaDeclaratorAST* asLambdaDeclarator() { return 0; }
    virtual LambdaExpressionAST* asLambdaExpression() { return 0; }
    virtual LambdaIntroducerAST* asLambdaIntroducer() { return 0; }
    virtual LinkageBodyAST* asLinkageBody() { return 0; }
    virtual LinkageSpecificationAST* asLinkageSpecification() { return 0; }
    virtual MemInitializerAST* asMemInitializer() { return 0; }
    virtual MemberAccessAST* asMemberAccess() { return 0; }
    virtual NameAST* asName() { return 0; }
    virtual NamedTypeSpecifierAST* asNamedTypeSpecifier() { return 0; }
    virtual NamespaceAST* asNamespace() { return 0; }
    virtual NamespaceAliasDefinitionAST* asNamespaceAliasDefinition() { return 0; }
    virtual NestedDeclaratorAST* asNestedDeclarator() { return 0; }
    virtual NestedExpressionAST* asNestedExpression() { return 0; }
    virtual NestedNameSpecifierAST* asNestedNameSpecifier() { return 0; }
    virtual NewArrayDeclaratorAST* asNewArrayDeclarator() { return 0; }
    virtual NewExpressionAST* asNewExpression() { return 0; }
    virtual NewTypeIdAST* asNewTypeId() { return 0; }
    virtual NoExceptOperatorExpressionAST* asNoExceptOperatorExpression() { return 0; }
    virtual NoExceptSpecificationAST* asNoExceptSpecification() { return 0; }
    virtual NumericLiteralAST* asNumericLiteral() { return 0; }
    virtual OperatorAST* asOperator() { return 0; }
    virtual OperatorFunctionIdAST* asOperatorFunctionId() { return 0; }
    virtual ParameterDeclarationAST* asParameterDeclaration() { return 0; }
    virtual ParameterDeclarationClauseAST* asParameterDeclarationClause() { return 0; }
    virtual PointerAST* asPointer() { return 0; }
    virtual PointerLiteralAST* asPointerLiteral() { return 0; }
    virtual PointerToMemberAST* asPointerToMember() { return 0; }
    virtual PostIncrDecrAST* asPostIncrDecr() { return 0; }
    virtual PostfixAST* asPostfix() { return 0; }
    virtual PostfixDeclaratorAST* asPostfixDeclarator() { return 0; }
    virtual PtrOperatorAST* asPtrOperator() { return 0; }
    virtual QualifiedNameAST* asQualifiedName() { return 0; }
    virtual QuantifiedTypeSpecifierAST* asQuantifiedTypeSpecifier() { return 0; }
    virtual RangeBasedForStatementAST* asRangeBasedForStatement() { return 0; }
    virtual ReferenceAST* asReference() { return 0; }
    virtual ReturnStatementAST* asReturnStatement() { return 0; }
    virtual SimpleDeclarationAST* asSimpleDeclaration() { return 0; }
    virtual SimpleNameAST* asSimpleName() { return 0; }
    virtual SimpleSpecifierAST* asSimpleSpecifier() { return 0; }
    virtual SizeofExpressionAST* asSizeofExpression() { return 0; }
    virtual SpecifierAST* asSpecifier() { return 0; }
    virtual StatementAST* asStatement() { return 0; }
    virtual StaticAssertDeclarationAST* asStaticAssertDeclaration() { return 0; }
    virtual StringLiteralAST* asStringLiteral() { return 0; }
    virtual SwitchStatementAST* asSwitchStatement() { return 0; }
    virtual TemplateDeclarationAST* asTemplateDeclaration() { return 0; }
    virtual TemplateIdAST* asTemplateId() { return 0; }
    virtual TemplateTypeParameterAST* asTemplateTypeParameter() { return 0; }
    virtual ThisExpressionAST* asThisExpression() { return 0; }
    virtual ThrowExpressionAST* asThrowExpression() { return 0; }
    virtual TrailingReturnTypeAST* asTrailingReturnType() { return 0; }
    virtual TranslationUnitAST* asTranslationUnit() { return 0; }
    virtual TryBlockStatementAST* asTryBlockStatement() { return 0; }
    virtual TypeConstructorCallAST* asTypeConstructorCall() { return 0; }
    virtual TypeIdAST* asTypeId() { return 0; }
    virtual TypeidExpressionAST* asTypeidExpression() { return 0; }
    virtual TypenameCallExpressionAST* asTypenameCallExpression() { return 0; }
    virtual TypenameTypeParameterAST* asTypenameTypeParameter() { return 0; }
    virtual TypeofSpecifierAST* asTypeofSpecifier() { return 0; }
    virtual UnaryExpressionAST* asUnaryExpression() { return 0; }
    virtual UsingAST* asUsing() { return 0; }
    virtual UsingDirectiveAST* asUsingDirective() { return 0; }
    virtual WhileStatementAST* asWhileStatement() { return 0; }

protected:
    virtual void accept0(ASTVisitor* visitor) = 0;
    virtual bool match0(AST* , ASTMatcher *) = 0;
};

class CFE_API StatementAST: public AST
{
public:
    StatementAST()
    {}

    virtual StatementAST* asStatement() { return this; }

    virtual StatementAST* clone(MemoryPool *pool) const = 0;
};

class CFE_API ExpressionAST: public AST
{
public:
    ExpressionAST()
    {}

    virtual ExpressionAST* asExpression() { return this; }

    virtual ExpressionAST* clone(MemoryPool *pool) const = 0;
};

class CFE_API DeclarationAST: public AST
{
public:
    DeclarationAST()
    {}

    virtual DeclarationAST* asDeclaration() { return this; }

    virtual DeclarationAST* clone(MemoryPool *pool) const = 0;
};

class CFE_API NameAST: public AST
{
public: // annotations
    const Name* name;

public:
    NameAST()
        : name(0)
    {}

    virtual NameAST* asName() { return this; }

    virtual NameAST* clone(MemoryPool *pool) const = 0;
};

class CFE_API SpecifierAST: public AST
{
public:
    SpecifierAST()
    {}

    virtual SpecifierAST* asSpecifier() { return this; }

    virtual SpecifierAST* clone(MemoryPool *pool) const = 0;
};

class CFE_API PtrOperatorAST: public AST
{
public:
    PtrOperatorAST()
    {}

    virtual PtrOperatorAST* asPtrOperator() { return this; }

    virtual PtrOperatorAST* clone(MemoryPool *pool) const = 0;
};

class CFE_API PostfixAST: public ExpressionAST
{
public:
    PostfixAST()
    {}

    virtual PostfixAST* asPostfix() { return this; }

    virtual PostfixAST* clone(MemoryPool *pool) const = 0;
};

class CFE_API CoreDeclaratorAST: public AST
{
public:
    CoreDeclaratorAST()
    {}

    virtual CoreDeclaratorAST* asCoreDeclarator() { return this; }

    virtual CoreDeclaratorAST* clone(MemoryPool *pool) const = 0;
};

class CFE_API PostfixDeclaratorAST: public AST
{
public:
    PostfixDeclaratorAST()
    {}

    virtual PostfixDeclaratorAST* asPostfixDeclarator() { return this; }

    virtual PostfixDeclaratorAST* clone(MemoryPool *pool) const = 0;
};

class CFE_API SimpleSpecifierAST: public SpecifierAST
{
public:
    unsigned specifier_token;

public:
    SimpleSpecifierAST()
        : specifier_token(0)
    {}

    virtual SimpleSpecifierAST* asSimpleSpecifier() { return this; }

    virtual unsigned firstToken() const;
    virtual unsigned lastToken() const;

    virtual SimpleSpecifierAST* clone(MemoryPool *pool) const;

protected:
    virtual void accept0(ASTVisitor* visitor);
    virtual bool match0(AST* , ASTMatcher *);
};

class CFE_API AttributeSpecifierAST: public SpecifierAST
{
public:
    AttributeSpecifierAST()
    {}

    virtual AttributeSpecifierAST* asAttributeSpecifier() { return this; }

    virtual AttributeSpecifierAST* clone(MemoryPool *pool) const = 0;
};

class CFE_API AlignmentSpecifierAST: public AttributeSpecifierAST
{
public:
    unsigned align_token;
    unsigned lparen_token;
    ExpressionAST* typeIdExprOrAlignmentExpr;
    unsigned ellipses_token;
    unsigned rparen_token;

public:
    AlignmentSpecifierAST()
        : align_token(0)
        , lparen_token(0)
        , typeIdExprOrAlignmentExpr(0)
        , ellipses_token(0)
        , rparen_token(0)
    {}

    virtual AlignmentSpecifierAST* asAlignmentSpecifier() { return this; }

    virtual unsigned firstToken() const;
    virtual unsigned lastToken() const;

    virtual AlignmentSpecifierAST* clone(MemoryPool *pool) const;

protected:
    virtual void accept0(ASTVisitor* visitor);
    virtual bool match0(AST* , ASTMatcher *);
};

class CFE_API GenericsDeclarationAST : public DeclarationAST
{
public:
    unsigned generics_token;
    DeclarationAST* declaration;

    GenericsDeclarationAST()
        : generics_token(0)
        , declaration(0)
    {}

    virtual GenericsDeclarationAST* asGenericsDeclaration() { return this; }

    virtual unsigned firstToken() const;
    virtual unsigned lastToken() const;

    virtual GenericsDeclarationAST* clone(MemoryPool *pool) const;

protected:
    virtual void accept0(ASTVisitor* visitor);
    virtual bool match0(AST* , ASTMatcher *);
};

class CFE_API GnuAttributeSpecifierAST: public AttributeSpecifierAST
{
public:
    unsigned attribute_token;
    unsigned first_lparen_token;
    unsigned second_lparen_token;
    GnuAttributeListAST* attribute_list;
    unsigned first_rparen_token;
    unsigned second_rparen_token;

public:
    GnuAttributeSpecifierAST()
        : attribute_token(0)
        , first_lparen_token(0)
        , second_lparen_token(0)
        , attribute_list(0)
        , first_rparen_token(0)
        , second_rparen_token(0)
    {}

    virtual GnuAttributeSpecifierAST* asGnuAttributeSpecifier() { return this; }

    virtual unsigned firstToken() const;
    virtual unsigned lastToken() const;

    virtual GnuAttributeSpecifierAST* clone(MemoryPool *pool) const;

protected:
    virtual void accept0(ASTVisitor* visitor);
    virtual bool match0(AST* , ASTMatcher *);
};

class CFE_API GnuAttributeAST: public AST
{
public:
    unsigned identifier_token;
    unsigned lparen_token;
    unsigned tag_token;
    ExpressionListAST* expression_list;
    unsigned rparen_token;

public:
    GnuAttributeAST()
        : identifier_token(0)
        , lparen_token(0)
        , tag_token(0)
        , expression_list(0)
        , rparen_token(0)
    {}

    virtual GnuAttributeAST* asGnuAttribute() { return this; }

    virtual unsigned firstToken() const;
    virtual unsigned lastToken() const;

    virtual GnuAttributeAST* clone(MemoryPool *pool) const;

protected:
    virtual void accept0(ASTVisitor* visitor);
    virtual bool match0(AST* , ASTMatcher *);
};

class CFE_API TypeofSpecifierAST: public SpecifierAST
{
public:
    unsigned typeof_token;
    unsigned lparen_token;
    ExpressionAST* expression;
    unsigned rparen_token;

public:
    TypeofSpecifierAST()
        : typeof_token(0)
        , lparen_token(0)
        , expression(0)
        , rparen_token(0)
    {}

    virtual TypeofSpecifierAST* asTypeofSpecifier() { return this; }

    virtual unsigned firstToken() const;
    virtual unsigned lastToken() const;

    virtual TypeofSpecifierAST* clone(MemoryPool *pool) const;

protected:
    virtual void accept0(ASTVisitor* visitor);
    virtual bool match0(AST* , ASTMatcher *);
};

class CFE_API DecltypeSpecifierAST: public SpecifierAST
{
public:
    unsigned decltype_token;
    unsigned lparen_token;
    ExpressionAST* expression;
    unsigned rparen_token;

public:
    DecltypeSpecifierAST()
        : decltype_token(0)
        , lparen_token(0)
        , expression(0)
        , rparen_token(0)
    {}

    virtual DecltypeSpecifierAST* asDecltypeSpecifier() { return this; }

    virtual unsigned firstToken() const;
    virtual unsigned lastToken() const;

    virtual DecltypeSpecifierAST* clone(MemoryPool *pool) const;

protected:
    virtual void accept0(ASTVisitor* visitor);
    virtual bool match0(AST* , ASTMatcher *);
};

class CFE_API DeclaratorAST: public AST
{
public:
    SpecifierListAST* attribute_list;
    PtrOperatorListAST* ptr_operator_list;
    CoreDeclaratorAST* core_declarator;
    PostfixDeclaratorListAST* postfix_declarator_list;
    SpecifierListAST* post_attribute_list;
    unsigned equal_token;
    ExpressionAST* initializer;

public:
    DeclaratorAST()
        : attribute_list(0)
        , ptr_operator_list(0)
        , core_declarator(0)
        , postfix_declarator_list(0)
        , post_attribute_list(0)
        , equal_token(0)
        , initializer(0)
    {}

    virtual DeclaratorAST* asDeclarator() { return this; }

    virtual unsigned firstToken() const;
    virtual unsigned lastToken() const;

    virtual DeclaratorAST* clone(MemoryPool *pool) const;

protected:
    virtual void accept0(ASTVisitor* visitor);
    virtual bool match0(AST* , ASTMatcher *);
};

class CFE_API SimpleDeclarationAST: public DeclarationAST
{
public:
    SpecifierListAST* decl_specifier_list;
    DeclaratorListAST* declarator_list;
    unsigned semicolon_token;

public:
    List<Symbol*>* symbols;

public:
    SimpleDeclarationAST()
        : decl_specifier_list(0)
        , declarator_list(0)
        , semicolon_token(0)
        , symbols(0)
    {}

    virtual SimpleDeclarationAST* asSimpleDeclaration() { return this; }

    virtual unsigned firstToken() const;
    virtual unsigned lastToken() const;

    virtual SimpleDeclarationAST* clone(MemoryPool *pool) const;

protected:
    virtual void accept0(ASTVisitor* visitor);
    virtual bool match0(AST* , ASTMatcher *);
};

class CFE_API EmptyDeclarationAST: public DeclarationAST
{
public:
    unsigned semicolon_token;

public:
    EmptyDeclarationAST()
        : semicolon_token(0)
    {}

    virtual EmptyDeclarationAST* asEmptyDeclaration() { return this; }

    virtual unsigned firstToken() const;
    virtual unsigned lastToken() const;

    virtual EmptyDeclarationAST* clone(MemoryPool *pool) const;

protected:
    virtual void accept0(ASTVisitor* visitor);
    virtual bool match0(AST* , ASTMatcher *);
};

class CFE_API AccessDeclarationAST: public DeclarationAST
{
public:
    unsigned access_specifier_token;
    unsigned slots_token;
    unsigned colon_token;

public:
    AccessDeclarationAST()
        : access_specifier_token(0)
        , slots_token(0)
        , colon_token(0)
    {}

    virtual AccessDeclarationAST* asAccessDeclaration() { return this; }

    virtual unsigned firstToken() const;
    virtual unsigned lastToken() const;

    virtual AccessDeclarationAST* clone(MemoryPool *pool) const;

protected:
    virtual void accept0(ASTVisitor* visitor);
    virtual bool match0(AST* , ASTMatcher *);
};

class CFE_API AsmDefinitionAST: public DeclarationAST
{
public:
    unsigned asm_token;
    unsigned qualifier_token;
    unsigned lparen_token;
    ExpressionAST* string_literal;
    // ### asm operand list
    unsigned rparen_token;
    unsigned semicolon_token;

public:
    AsmDefinitionAST()
        : asm_token(0)
        , qualifier_token(0)
        , lparen_token(0)
        , string_literal(0)
        , rparen_token(0)
        , semicolon_token(0)
    {}

    virtual AsmDefinitionAST* asAsmDefinition() { return this; }

    virtual unsigned firstToken() const;
    virtual unsigned lastToken() const;

    virtual AsmDefinitionAST* clone(MemoryPool *pool) const;

protected:
    virtual void accept0(ASTVisitor* visitor);
    virtual bool match0(AST* , ASTMatcher *);
};

class CFE_API BaseSpecifierAST: public AST
{
public:
    unsigned virtual_token;
    unsigned access_specifier_token;
    NameAST* name;
    unsigned ellipsis_token;

public: // annotations
    BaseClass *symbol;

public:
    BaseSpecifierAST()
        : virtual_token(0)
        , access_specifier_token(0)
        , name(0)
        , ellipsis_token(0)
        , symbol(0)
    {}

    virtual BaseSpecifierAST* asBaseSpecifier() { return this; }

    virtual unsigned firstToken() const;
    virtual unsigned lastToken() const;

    virtual BaseSpecifierAST* clone(MemoryPool *pool) const;

protected:
    virtual void accept0(ASTVisitor* visitor);
    virtual bool match0(AST* , ASTMatcher *);
};

class CFE_API IdExpressionAST: public ExpressionAST
{
public:
    NameAST* name;

public:
    IdExpressionAST()
        : name(0)
    {}

    virtual IdExpressionAST* asIdExpression() { return this; }

    virtual unsigned firstToken() const;
    virtual unsigned lastToken() const;

    virtual IdExpressionAST* clone(MemoryPool *pool) const;

protected:
    virtual void accept0(ASTVisitor* visitor);
    virtual bool match0(AST* , ASTMatcher *);
};

class CFE_API CompoundExpressionAST: public ExpressionAST
{
public:
    unsigned lparen_token;
    CompoundStatementAST* statement;
    unsigned rparen_token;

public:
    CompoundExpressionAST()
        : lparen_token(0)
        , statement(0)
        , rparen_token(0)
    {}

    virtual CompoundExpressionAST* asCompoundExpression() { return this; }

    virtual unsigned firstToken() const;
    virtual unsigned lastToken() const;

    virtual CompoundExpressionAST* clone(MemoryPool *pool) const;

protected:
    virtual void accept0(ASTVisitor* visitor);
    virtual bool match0(AST* , ASTMatcher *);
};

class CFE_API CompoundLiteralAST: public ExpressionAST
{
public:
    unsigned lparen_token;
    ExpressionAST* type_id;
    unsigned rparen_token;
    ExpressionAST* initializer;

public:
    CompoundLiteralAST()
        : lparen_token(0)
        , type_id(0)
        , rparen_token(0)
        , initializer(0)
    {}

    virtual CompoundLiteralAST* asCompoundLiteral() { return this; }

    virtual unsigned firstToken() const;
    virtual unsigned lastToken() const;

    virtual CompoundLiteralAST* clone(MemoryPool *pool) const;

protected:
    virtual void accept0(ASTVisitor* visitor);
    virtual bool match0(AST* , ASTMatcher *);
};

class CFE_API BinaryExpressionAST: public ExpressionAST
{
public:
    ExpressionAST* left_expression;
    unsigned binary_op_token;
    ExpressionAST* right_expression;

public:
    BinaryExpressionAST()
        : left_expression(0)
        , binary_op_token(0)
        , right_expression(0)
    {}

    virtual BinaryExpressionAST* asBinaryExpression() { return this; }

    virtual unsigned firstToken() const;
    virtual unsigned lastToken() const;

    virtual BinaryExpressionAST* clone(MemoryPool *pool) const;

protected:
    virtual void accept0(ASTVisitor* visitor);
    virtual bool match0(AST* , ASTMatcher *);
};

class CFE_API CastExpressionAST: public ExpressionAST
{
public:
    unsigned lparen_token;
    ExpressionAST* type_id;
    unsigned rparen_token;
    ExpressionAST* expression;

public: // annotations
    FullySpecifiedType expression_type;

public:
    CastExpressionAST()
        : lparen_token(0)
        , type_id(0)
        , rparen_token(0)
        , expression(0)
    {}

    virtual CastExpressionAST* asCastExpression() { return this; }

    virtual unsigned firstToken() const;
    virtual unsigned lastToken() const;

    virtual CastExpressionAST* clone(MemoryPool *pool) const;

protected:
    virtual void accept0(ASTVisitor* visitor);
    virtual bool match0(AST* , ASTMatcher *);
};

class CFE_API ClassSpecifierAST: public SpecifierAST
{
public:
    unsigned classkey_token;
    SpecifierListAST* attribute_list;
    NameAST* name;
    unsigned final_token;
    unsigned colon_token;
    BaseSpecifierListAST* base_clause_list;
    unsigned dot_dot_dot_token;
    unsigned lbrace_token;
    DeclarationListAST* member_specifier_list;
    unsigned rbrace_token;

public: // annotations
    Class *symbol;

public:
    ClassSpecifierAST()
        : classkey_token(0)
        , attribute_list(0)
        , name(0)
        , final_token(0)
        , colon_token(0)
        , base_clause_list(0)
        , dot_dot_dot_token(0)
        , lbrace_token(0)
        , member_specifier_list(0)
        , rbrace_token(0)
        , symbol(0)
    {}

    virtual ClassSpecifierAST* asClassSpecifier() { return this; }

    virtual unsigned firstToken() const;
    virtual unsigned lastToken() const;

    virtual ClassSpecifierAST* clone(MemoryPool *pool) const;

protected:
    virtual void accept0(ASTVisitor* visitor);
    virtual bool match0(AST* , ASTMatcher *);
};

class CFE_API CaseStatementAST: public StatementAST
{
public:
    unsigned case_token;
    ExpressionAST* expression;
    unsigned colon_token;
    StatementAST* statement;

public:
    CaseStatementAST()
        : case_token(0)
        , expression(0)
        , colon_token(0)
        , statement(0)
    {}

    virtual CaseStatementAST* asCaseStatement() { return this; }

    virtual unsigned firstToken() const;
    virtual unsigned lastToken() const;

    virtual CaseStatementAST* clone(MemoryPool *pool) const;

protected:
    virtual void accept0(ASTVisitor* visitor);
    virtual bool match0(AST* , ASTMatcher *);
};

class CFE_API CompoundStatementAST: public StatementAST
{
public:
    unsigned lbrace_token;
    StatementListAST* statement_list;
    unsigned rbrace_token;

public: // annotations
    Block *symbol;

public:
    CompoundStatementAST()
        : lbrace_token(0)
        , statement_list(0)
        , rbrace_token(0)
        , symbol(0)
    {}

    virtual CompoundStatementAST* asCompoundStatement() { return this; }

    virtual unsigned firstToken() const;
    virtual unsigned lastToken() const;

    virtual CompoundStatementAST* clone(MemoryPool *pool) const;

protected:
    virtual void accept0(ASTVisitor* visitor);
    virtual bool match0(AST* , ASTMatcher *);
};

class CFE_API ConditionAST: public ExpressionAST
{
public:
    SpecifierListAST* type_specifier_list;
    DeclaratorAST* declarator;

public:
    ConditionAST()
        : type_specifier_list(0)
        , declarator(0)
    {}

    virtual ConditionAST* asCondition() { return this; }

    virtual unsigned firstToken() const;
    virtual unsigned lastToken() const;

    virtual ConditionAST* clone(MemoryPool *pool) const;

protected:
    virtual void accept0(ASTVisitor* visitor);
    virtual bool match0(AST* , ASTMatcher *);
};

class CFE_API ConditionalExpressionAST: public ExpressionAST
{
public:
    ExpressionAST* condition;
    unsigned question_token;
    ExpressionAST* left_expression;
    unsigned colon_token;
    ExpressionAST* right_expression;

public:
    ConditionalExpressionAST()
        : condition(0)
        , question_token(0)
        , left_expression(0)
        , colon_token(0)
        , right_expression(0)
    {}

    virtual ConditionalExpressionAST* asConditionalExpression() { return this; }

    virtual unsigned firstToken() const;
    virtual unsigned lastToken() const;

    virtual ConditionalExpressionAST* clone(MemoryPool *pool) const;

protected:
    virtual void accept0(ASTVisitor* visitor);
    virtual bool match0(AST* , ASTMatcher *);
};

class CFE_API CppCastExpressionAST: public ExpressionAST
{
public:
    unsigned cast_token;
    unsigned less_token;
    ExpressionAST* type_id;
    unsigned greater_token;
    unsigned lparen_token;
    ExpressionAST* expression;
    unsigned rparen_token;

public:
    CppCastExpressionAST()
        : cast_token(0)
        , less_token(0)
        , type_id(0)
        , greater_token(0)
        , lparen_token(0)
        , expression(0)
        , rparen_token(0)
    {}

    virtual CppCastExpressionAST* asCppCastExpression() { return this; }

    virtual unsigned firstToken() const;
    virtual unsigned lastToken() const;

    virtual CppCastExpressionAST* clone(MemoryPool *pool) const;

protected:
    virtual void accept0(ASTVisitor* visitor);
    virtual bool match0(AST* , ASTMatcher *);
};

class CFE_API CtorInitializerAST: public AST
{
public:
    unsigned colon_token;
    MemInitializerListAST* member_initializer_list;
    unsigned dot_dot_dot_token;

public:
    CtorInitializerAST()
        : colon_token(0)
        , member_initializer_list(0)
        , dot_dot_dot_token(0)
    {}

    virtual CtorInitializerAST* asCtorInitializer() { return this; }

    virtual unsigned firstToken() const;
    virtual unsigned lastToken() const;

    virtual CtorInitializerAST* clone(MemoryPool *pool) const;

protected:
    virtual void accept0(ASTVisitor* visitor);
    virtual bool match0(AST* , ASTMatcher *);
};

class CFE_API DeclarationStatementAST: public StatementAST
{
public:
    DeclarationAST* declaration;

public:
    DeclarationStatementAST()
        : declaration(0)
    {}

    virtual DeclarationStatementAST* asDeclarationStatement() { return this; }

    virtual unsigned firstToken() const;
    virtual unsigned lastToken() const;

    virtual DeclarationStatementAST* clone(MemoryPool *pool) const;

protected:
    virtual void accept0(ASTVisitor* visitor);
    virtual bool match0(AST* , ASTMatcher *);
};

class CFE_API DeclaratorIdAST: public CoreDeclaratorAST
{
public:
    unsigned dot_dot_dot_token;
    NameAST* name;

public:
    DeclaratorIdAST()
        : dot_dot_dot_token(0)
        , name(0)
    {}

    virtual DeclaratorIdAST* asDeclaratorId() { return this; }

    virtual unsigned firstToken() const;
    virtual unsigned lastToken() const;

    virtual DeclaratorIdAST* clone(MemoryPool *pool) const;

protected:
    virtual void accept0(ASTVisitor* visitor);
    virtual bool match0(AST* , ASTMatcher *);
};

class CFE_API NestedDeclaratorAST: public CoreDeclaratorAST
{
public:
    unsigned lparen_token;
    DeclaratorAST* declarator;
    unsigned rparen_token;

public:
    NestedDeclaratorAST()
        : lparen_token(0)
        , declarator(0)
        , rparen_token(0)
    {}

    virtual NestedDeclaratorAST* asNestedDeclarator() { return this; }

    virtual unsigned firstToken() const;
    virtual unsigned lastToken() const;

    virtual NestedDeclaratorAST* clone(MemoryPool *pool) const;

protected:
    virtual void accept0(ASTVisitor* visitor);
    virtual bool match0(AST* , ASTMatcher *);
};

class CFE_API BitfieldDeclaratorAST: public PostfixDeclaratorAST
{
public:
    unsigned colon_token;

public:
    BitfieldDeclaratorAST()
        : colon_token(0)
    {}

    virtual BitfieldDeclaratorAST* asBitfieldDeclarator() { return this; }

    virtual unsigned firstToken() const;
    virtual unsigned lastToken() const;

    virtual BitfieldDeclaratorAST* clone(MemoryPool *pool) const;

protected:
    virtual void accept0(ASTVisitor* visitor);
    virtual bool match0(AST* , ASTMatcher *);
};

class CFE_API FunctionDeclaratorAST: public PostfixDeclaratorAST
{
public:
    unsigned lparen_token;
    ParameterDeclarationClauseAST* parameter_declaration_clause;
    unsigned rparen_token;
    unsigned psychec_omission_token;
    SpecifierListAST* cv_qualifier_list;
    unsigned ref_qualifier_token;
    ExceptionSpecificationAST* exception_specification;
    TrailingReturnTypeAST* trailing_return_type;
    // Some FunctionDeclarators can also be interpreted as an initializer, like for 'A b(c);'
    ExpressionAST* as_cpp_initializer;

public: // annotations
    Function *symbol;

public:
    FunctionDeclaratorAST()
        : lparen_token(0)
        , parameter_declaration_clause(0)
        , rparen_token(0)
        , psychec_omission_token(0)
        , cv_qualifier_list(0)
        , ref_qualifier_token(0)
        , exception_specification(0)
        , trailing_return_type(0)
        , as_cpp_initializer(0)
        , symbol(0)
    {}

    virtual FunctionDeclaratorAST* asFunctionDeclarator() { return this; }

    virtual unsigned firstToken() const;
    virtual unsigned lastToken() const;

    virtual FunctionDeclaratorAST* clone(MemoryPool *pool) const;

protected:
    virtual void accept0(ASTVisitor* visitor);
    virtual bool match0(AST* , ASTMatcher *);
};

class CFE_API ArrayDeclaratorAST: public PostfixDeclaratorAST
{
public:
    unsigned lbracket_token;
    ExpressionAST* expression;
    unsigned rbracket_token;

public:
    ArrayDeclaratorAST()
        : lbracket_token(0)
        , expression(0)
        , rbracket_token(0)
    {}

    virtual ArrayDeclaratorAST* asArrayDeclarator() { return this; }

    virtual unsigned firstToken() const;
    virtual unsigned lastToken() const;

    virtual ArrayDeclaratorAST* clone(MemoryPool *pool) const;

protected:
    virtual void accept0(ASTVisitor* visitor);
    virtual bool match0(AST* , ASTMatcher *);
};

class CFE_API DeleteExpressionAST: public ExpressionAST
{
public:
    unsigned scope_token;
    unsigned delete_token;
    unsigned lbracket_token;
    unsigned rbracket_token;
    ExpressionAST* expression;

public:
    DeleteExpressionAST()
        : scope_token(0)
        , delete_token(0)
        , lbracket_token(0)
        , rbracket_token(0)
        , expression(0)
    {}

    virtual DeleteExpressionAST* asDeleteExpression() { return this; }

    virtual unsigned firstToken() const;
    virtual unsigned lastToken() const;

    virtual DeleteExpressionAST* clone(MemoryPool *pool) const;

protected:
    virtual void accept0(ASTVisitor* visitor);
    virtual bool match0(AST* , ASTMatcher *);
};

class CFE_API DoStatementAST: public StatementAST
{
public:
    unsigned do_token;
    StatementAST* statement;
    unsigned while_token;
    unsigned lparen_token;
    ExpressionAST* expression;
    unsigned rparen_token;
    unsigned semicolon_token;

public:
    DoStatementAST()
        : do_token(0)
        , statement(0)
        , while_token(0)
        , lparen_token(0)
        , expression(0)
        , rparen_token(0)
        , semicolon_token(0)
    {}

    virtual DoStatementAST* asDoStatement() { return this; }

    virtual unsigned firstToken() const;
    virtual unsigned lastToken() const;

    virtual DoStatementAST* clone(MemoryPool *pool) const;

protected:
    virtual void accept0(ASTVisitor* visitor);
    virtual bool match0(AST* , ASTMatcher *);
};

class CFE_API NamedTypeSpecifierAST: public SpecifierAST
{
public:
    NameAST* name;

public:
    NamedTypeSpecifierAST()
        : name(0)
    {}

    virtual NamedTypeSpecifierAST* asNamedTypeSpecifier() { return this; }

    virtual unsigned firstToken() const;
    virtual unsigned lastToken() const;

    virtual NamedTypeSpecifierAST* clone(MemoryPool *pool) const;

protected:
    virtual void accept0(ASTVisitor* visitor);
    virtual bool match0(AST* , ASTMatcher *);
};

class CFE_API ElaboratedTypeSpecifierAST: public SpecifierAST
{
public:
    SpecifierListAST* attribute_list;
    TaggedNameAST* name;

public:
    ElaboratedTypeSpecifierAST()
        : attribute_list(0)
        , name(0)
    {}

    virtual ElaboratedTypeSpecifierAST* asElaboratedTypeSpecifier() { return this; }

    virtual unsigned firstToken() const;
    virtual unsigned lastToken() const;

    virtual ElaboratedTypeSpecifierAST* clone(MemoryPool *pool) const;

protected:
    virtual void accept0(ASTVisitor* visitor);
    virtual bool match0(AST* , ASTMatcher *);
};

class CFE_API EnumSpecifierAST: public SpecifierAST
{
public:
    unsigned enum_token;
    unsigned key_token; // struct, class or 0
    NameAST* name;
    unsigned colon_token; // can be 0 if there is no enum-base
    SpecifierListAST* type_specifier_list; // ditto
    unsigned lbrace_token;
    EnumeratorListAST* enumerator_list;
    unsigned stray_comma_token;
    unsigned rbrace_token;

public: // annotations
    Enum *symbol;

public:
    EnumSpecifierAST()
        : enum_token(0)
        , key_token(0)
        , name(0)
        , colon_token(0)
        , type_specifier_list(0)
        , lbrace_token(0)
        , enumerator_list(0)
        , stray_comma_token(0)
        , rbrace_token(0)
        , symbol(0)
    {}

    virtual EnumSpecifierAST* asEnumSpecifier() { return this; }

    virtual unsigned firstToken() const;
    virtual unsigned lastToken() const;

    virtual EnumSpecifierAST* clone(MemoryPool *pool) const;

protected:
    virtual void accept0(ASTVisitor* visitor);
    virtual bool match0(AST* , ASTMatcher *);
};

class CFE_API EnumeratorAST: public AST
{
public:
    unsigned identifier_token;
    SpecifierListAST* attribute_list;
    unsigned equal_token;
    ExpressionAST* expression;

public:
    EnumeratorAST()
        : identifier_token(0)
        , attribute_list(0)
        , equal_token(0)
        , expression(0)
    {}

    virtual EnumeratorAST* asEnumerator() { return this; }

    virtual unsigned firstToken() const;
    virtual unsigned lastToken() const;

    virtual EnumeratorAST* clone(MemoryPool *pool) const;

protected:
    virtual void accept0(ASTVisitor* visitor);
    virtual bool match0(AST* , ASTMatcher *);
};

class CFE_API ExceptionDeclarationAST: public DeclarationAST
{
public:
    SpecifierListAST* type_specifier_list;
    DeclaratorAST* declarator;
    unsigned dot_dot_dot_token;

public:
    ExceptionDeclarationAST()
        : type_specifier_list(0)
        , declarator(0)
        , dot_dot_dot_token(0)
    {}

    virtual ExceptionDeclarationAST* asExceptionDeclaration() { return this; }

    virtual unsigned firstToken() const;
    virtual unsigned lastToken() const;

    virtual ExceptionDeclarationAST* clone(MemoryPool *pool) const;

protected:
    virtual void accept0(ASTVisitor* visitor);
    virtual bool match0(AST* , ASTMatcher *);
};

class CFE_API ExceptionSpecificationAST: public AST
{
public:
    ExceptionSpecificationAST()
    {}

    virtual ExceptionSpecificationAST* asExceptionSpecification() { return this; }

    virtual ExceptionSpecificationAST* clone(MemoryPool *pool) const = 0;
};

class CFE_API DynamicExceptionSpecificationAST: public ExceptionSpecificationAST
{
public:
    unsigned throw_token;
    unsigned lparen_token;
    unsigned dot_dot_dot_token;
    ExpressionListAST* type_id_list;
    unsigned rparen_token;

public:
    DynamicExceptionSpecificationAST()
        : throw_token(0)
        , lparen_token(0)
        , dot_dot_dot_token(0)
        , type_id_list(0)
        , rparen_token(0)
    {}

    virtual DynamicExceptionSpecificationAST* asDynamicExceptionSpecification() { return this; }

    virtual unsigned firstToken() const;
    virtual unsigned lastToken() const;

    virtual DynamicExceptionSpecificationAST* clone(MemoryPool *pool) const;

protected:
    virtual void accept0(ASTVisitor* visitor);
    virtual bool match0(AST* , ASTMatcher *);
};

class CFE_API NoExceptSpecificationAST: public ExceptionSpecificationAST
{
public:
    unsigned noexcept_token;
    unsigned lparen_token;
    ExpressionAST* expression;
    unsigned rparen_token;

public:
    NoExceptSpecificationAST()
        : noexcept_token(0)
        , lparen_token(0)
        , expression(0)
        , rparen_token(0)
    {}

    virtual NoExceptSpecificationAST* asNoExceptSpecification() { return this; }

    virtual unsigned firstToken() const;
    virtual unsigned lastToken() const;

    virtual NoExceptSpecificationAST* clone(MemoryPool *pool) const;

protected:
    virtual void accept0(ASTVisitor* visitor);
    virtual bool match0(AST* , ASTMatcher *);
};

class CFE_API ExpressionOrDeclarationStatementAST: public StatementAST
{
public:
    ExpressionStatementAST* expression;
    DeclarationStatementAST* declaration;

public:
    ExpressionOrDeclarationStatementAST()
        : expression(0)
        , declaration(0)
    {}

    virtual ExpressionOrDeclarationStatementAST* asExpressionOrDeclarationStatement() { return this; }

    virtual unsigned firstToken() const;
    virtual unsigned lastToken() const;

    virtual ExpressionOrDeclarationStatementAST* clone(MemoryPool *pool) const;

protected:
    virtual void accept0(ASTVisitor* visitor);
    virtual bool match0(AST* , ASTMatcher *);
};

class CFE_API ExpressionStatementAST: public StatementAST
{
public:
    ExpressionAST* expression;
    unsigned semicolon_token;

public:
    ExpressionStatementAST()
        : expression(0)
        , semicolon_token(0)
    {}

    virtual ExpressionStatementAST* asExpressionStatement() { return this; }

    virtual unsigned firstToken() const;
    virtual unsigned lastToken() const;

    virtual ExpressionStatementAST* clone(MemoryPool *pool) const;

protected:
    virtual void accept0(ASTVisitor* visitor);
    virtual bool match0(AST* , ASTMatcher *);
};

class CFE_API AmbiguousStatementAST : public StatementAST
{
public:
    DeclarationStatementAST* declarationStmt;
    ExpressionStatementAST* expressionStmt;
    std::unique_ptr<psyche::SyntaxAmbiguity> ambiguity;
    std::vector<Declaration*> suspiciousDecls;

public:
    AmbiguousStatementAST()
        : declarationStmt(0)
        , expressionStmt(0)
    {}

    virtual AmbiguousStatementAST* asAmbiguousStatement() { return this; }

    virtual unsigned firstToken() const;
    virtual unsigned lastToken() const;

    virtual AmbiguousStatementAST* clone(MemoryPool *pool) const;

protected:
    virtual void accept0(ASTVisitor* visitor);
    virtual bool match0(AST* , ASTMatcher *);
};

class CFE_API FunctionDefinitionAST: public DeclarationAST
{
public:
    SpecifierListAST* decl_specifier_list;
    DeclaratorAST* declarator;
    CtorInitializerAST* ctor_initializer;
    StatementAST* function_body;

public: // annotations
    Function *symbol;

public:
    FunctionDefinitionAST()
        : decl_specifier_list(0)
        , declarator(0)
        , ctor_initializer(0)
        , function_body(0)
        , symbol(0)
    {}

    virtual FunctionDefinitionAST* asFunctionDefinition() { return this; }

    virtual unsigned firstToken() const;
    virtual unsigned lastToken() const;

    virtual FunctionDefinitionAST* clone(MemoryPool *pool) const;

protected:
    virtual void accept0(ASTVisitor* visitor);
    virtual bool match0(AST* , ASTMatcher *);
};

class CFE_API ForeachStatementAST: public StatementAST
{
public:
    unsigned foreach_token;
    unsigned lparen_token;
    // declaration
    SpecifierListAST* type_specifier_list;
    DeclaratorAST* declarator;
    // or an expression
    ExpressionAST* initializer;
    unsigned comma_token;
    ExpressionAST* expression;
    unsigned rparen_token;
    StatementAST* statement;

public: // annotations
    Block *symbol;

public:
    ForeachStatementAST()
        : foreach_token(0)
        , lparen_token(0)
        , type_specifier_list(0)
        , declarator(0)
        , initializer(0)
        , comma_token(0)
        , expression(0)
        , rparen_token(0)
        , statement(0)
        , symbol(0)
    {}

    virtual ForeachStatementAST* asForeachStatement() { return this; }

    virtual unsigned firstToken() const;
    virtual unsigned lastToken() const;

    virtual ForeachStatementAST* clone(MemoryPool *pool) const;

protected:
    virtual void accept0(ASTVisitor* visitor);
    virtual bool match0(AST* , ASTMatcher *);
};

class CFE_API RangeBasedForStatementAST : public StatementAST
{
public:
    unsigned for_token;
    unsigned lparen_token;
    // declaration
    SpecifierListAST* type_specifier_list;
    DeclaratorAST* declarator;
    // or an expression
    unsigned colon_token;
    ExpressionAST* expression;
    unsigned rparen_token;
    StatementAST* statement;

public: // annotations
    Block *symbol;

public:
    RangeBasedForStatementAST()
        : for_token(0)
        , lparen_token(0)
        , type_specifier_list(0)
        , declarator(0)
        , colon_token(0)
        , expression(0)
        , rparen_token(0)
        , statement(0)
        , symbol(0)
    {}

    virtual RangeBasedForStatementAST* asRangeBasedForStatement() { return this; }

    virtual unsigned firstToken() const;
    virtual unsigned lastToken() const;

    virtual RangeBasedForStatementAST* clone(MemoryPool *pool) const;

protected:
    virtual void accept0(ASTVisitor* visitor);
    virtual bool match0(AST* , ASTMatcher *);
};

class CFE_API ForStatementAST: public StatementAST
{
public:
    unsigned for_token;
    unsigned lparen_token;
    StatementAST* initializer;
    ExpressionAST* condition;
    unsigned semicolon_token;
    ExpressionAST* expression;
    unsigned rparen_token;
    StatementAST* statement;

public: // annotations
    Block *symbol;

public:
    ForStatementAST()
        : for_token(0)
        , lparen_token(0)
        , initializer(0)
        , condition(0)
        , semicolon_token(0)
        , expression(0)
        , rparen_token(0)
        , statement(0)
        , symbol(0)
    {}

    virtual ForStatementAST* asForStatement() { return this; }

    virtual unsigned firstToken() const;
    virtual unsigned lastToken() const;

    virtual ForStatementAST* clone(MemoryPool *pool) const;

protected:
    virtual void accept0(ASTVisitor* visitor);
    virtual bool match0(AST* , ASTMatcher *);
};

class CFE_API IfStatementAST: public StatementAST
{
public:
    unsigned if_token;
    unsigned lparen_token;
    ExpressionAST* condition;
    unsigned rparen_token;
    StatementAST* statement;
    unsigned else_token;
    StatementAST* else_statement;

public: // annotations
    Block *symbol;

public:
    IfStatementAST()
        : if_token(0)
        , lparen_token(0)
        , condition(0)
        , rparen_token(0)
        , statement(0)
        , else_token(0)
        , else_statement(0)
        , symbol(0)
    {}

    virtual IfStatementAST* asIfStatement() { return this; }

    virtual unsigned firstToken() const;
    virtual unsigned lastToken() const;

    virtual IfStatementAST* clone(MemoryPool *pool) const;

protected:
    virtual void accept0(ASTVisitor* visitor);
    virtual bool match0(AST* , ASTMatcher *);
};

class CFE_API ArrayInitializerAST: public ExpressionAST
{
public:
    unsigned lbrace_token;
    ExpressionListAST* expression_list;
    unsigned rbrace_token;

public:
    ArrayInitializerAST()
        : lbrace_token(0)
        , expression_list(0)
        , rbrace_token(0)
    {}

    virtual ArrayInitializerAST* asArrayInitializer() { return this; }

    virtual unsigned firstToken() const;
    virtual unsigned lastToken() const;

    virtual ArrayInitializerAST* clone(MemoryPool *pool) const;

protected:
    virtual void accept0(ASTVisitor* visitor);
    virtual bool match0(AST* , ASTMatcher *);
};

class CFE_API LabeledStatementAST: public StatementAST
{
public:
    unsigned label_token;
    unsigned colon_token;
    StatementAST* statement;

public:
    LabeledStatementAST()
        : label_token(0)
        , colon_token(0)
        , statement(0)
    {}

    virtual LabeledStatementAST* asLabeledStatement() { return this; }

    virtual unsigned firstToken() const;
    virtual unsigned lastToken() const;

    virtual LabeledStatementAST* clone(MemoryPool *pool) const;

protected:
    virtual void accept0(ASTVisitor* visitor);
    virtual bool match0(AST* , ASTMatcher *);
};

class CFE_API LinkageBodyAST: public DeclarationAST
{
public:
    unsigned lbrace_token;
    DeclarationListAST* declaration_list;
    unsigned rbrace_token;

public:
    LinkageBodyAST()
        : lbrace_token(0)
        , declaration_list(0)
        , rbrace_token(0)
    {}

    virtual LinkageBodyAST* asLinkageBody() { return this; }
    virtual unsigned firstToken() const;
    virtual unsigned lastToken() const;

    virtual LinkageBodyAST* clone(MemoryPool *pool) const;

protected:
    virtual void accept0(ASTVisitor* visitor);
    virtual bool match0(AST* , ASTMatcher *);
};

class CFE_API LinkageSpecificationAST: public DeclarationAST
{
public:
    unsigned extern_token;
    unsigned extern_type_token;
    DeclarationAST* declaration;

public:
    LinkageSpecificationAST()
        : extern_token(0)
        , extern_type_token(0)
        , declaration(0)
    {}

    virtual LinkageSpecificationAST* asLinkageSpecification() { return this; }

    virtual unsigned firstToken() const;
    virtual unsigned lastToken() const;

    virtual LinkageSpecificationAST* clone(MemoryPool *pool) const;

protected:
    virtual void accept0(ASTVisitor* visitor);
    virtual bool match0(AST* , ASTMatcher *);
};

class CFE_API MemInitializerAST: public AST
{
public:
    NameAST* name;
    // either a BracedInitializerAST or a ExpressionListParenAST
    ExpressionAST* expression;

public:
    MemInitializerAST()
        : name(0)
        , expression(0)
    {}

    virtual MemInitializerAST* asMemInitializer() { return this; }

    virtual unsigned firstToken() const;
    virtual unsigned lastToken() const;

    virtual MemInitializerAST* clone(MemoryPool *pool) const;

protected:
    virtual void accept0(ASTVisitor* visitor);
    virtual bool match0(AST* , ASTMatcher *);
};

class CFE_API NestedNameSpecifierAST: public AST
{
public:
    NameAST* class_or_namespace_name;
    unsigned scope_token;

public:
    NestedNameSpecifierAST()
        : class_or_namespace_name(0)
        , scope_token(0)
    {}

    virtual NestedNameSpecifierAST* asNestedNameSpecifier() { return this; }

    virtual unsigned firstToken() const;
    virtual unsigned lastToken() const;

    virtual NestedNameSpecifierAST* clone(MemoryPool *pool) const;

protected:
    virtual void accept0(ASTVisitor* visitor);
    virtual bool match0(AST* , ASTMatcher *);
};

class CFE_API QualifiedNameAST: public NameAST
{
public:
    unsigned global_scope_token;
    NestedNameSpecifierListAST* nested_name_specifier_list;
    NameAST* unqualified_name;

public:
    QualifiedNameAST()
        : global_scope_token(0)
        , nested_name_specifier_list(0)
        , unqualified_name(0)
    {}

    virtual QualifiedNameAST* asQualifiedName() { return this; }

    virtual unsigned firstToken() const;
    virtual unsigned lastToken() const;

    virtual QualifiedNameAST* clone(MemoryPool *pool) const;

protected:
    virtual void accept0(ASTVisitor* visitor);
    virtual bool match0(AST* , ASTMatcher *);
};

class CFE_API QuantifiedTypeSpecifierAST: public SpecifierAST
{
public:
    unsigned quantifier_token;
    unsigned lparen_token;
    NameAST* name;
    unsigned rparen_token;

public:
    QuantifiedTypeSpecifierAST()
        : quantifier_token(0)
        , lparen_token(0)
        , name(0)
        , rparen_token(0)
    {}

    virtual QuantifiedTypeSpecifierAST* asQuantifiedTypeSpecifier() { return this; }

    virtual unsigned firstToken() const;
    virtual unsigned lastToken() const;

    virtual QuantifiedTypeSpecifierAST* clone(MemoryPool *pool) const;

protected:
    virtual void accept0(ASTVisitor* visitor);
    virtual bool match0(AST* , ASTMatcher *);
};

class CFE_API TaggedNameAST: public NameAST
{
public:
    unsigned tag_token;
    NameAST* core_name;

public:
    TaggedNameAST()
        : tag_token(0)
        , core_name(0)
    {}

    virtual TaggedNameAST* asTaggedName() { return this; }

    virtual unsigned firstToken() const;
    virtual unsigned lastToken() const;

    virtual TaggedNameAST* clone(MemoryPool *pool) const;

protected:
    virtual void accept0(ASTVisitor* visitor);
    virtual bool match0(AST* , ASTMatcher *);
};

class CFE_API OperatorFunctionIdAST: public NameAST
{
public:
    unsigned operator_token;
    OperatorAST* op;

public:
    OperatorFunctionIdAST()
        : operator_token(0)
        , op(0)
    {}

    virtual OperatorFunctionIdAST* asOperatorFunctionId() { return this; }

    virtual unsigned firstToken() const;
    virtual unsigned lastToken() const;

    virtual OperatorFunctionIdAST* clone(MemoryPool *pool) const;

protected:
    virtual void accept0(ASTVisitor* visitor);
    virtual bool match0(AST* , ASTMatcher *);
};

class CFE_API ConversionFunctionIdAST: public NameAST
{
public:
    unsigned operator_token;
    SpecifierListAST* type_specifier_list;
    PtrOperatorListAST* ptr_operator_list;

public:
    ConversionFunctionIdAST()
        : operator_token(0)
        , type_specifier_list(0)
        , ptr_operator_list(0)
    {}

    virtual ConversionFunctionIdAST* asConversionFunctionId() { return this; }

    virtual unsigned firstToken() const;
    virtual unsigned lastToken() const;

    virtual ConversionFunctionIdAST* clone(MemoryPool *pool) const;

protected:
    virtual void accept0(ASTVisitor* visitor);
    virtual bool match0(AST* , ASTMatcher *);
};

class CFE_API EmptyNameAST: public NameAST
{
public:
    EmptyNameAST()
    {}

    virtual EmptyNameAST* asEmptyName() { return this; }
    virtual unsigned firstToken() const { return 0; }
    virtual unsigned lastToken() const { return 0; }

    virtual EmptyNameAST* clone(MemoryPool *pool) const;

protected:
    virtual void accept0(ASTVisitor* visitor);
    virtual bool match0(AST* , ASTMatcher *);
};

class CFE_API SimpleNameAST: public NameAST
{
public:
    unsigned identifier_token;

public:
    SimpleNameAST()
        : identifier_token(0)
    {}

    virtual SimpleNameAST* asSimpleName() { return this; }

    virtual unsigned firstToken() const;
    virtual unsigned lastToken() const;

    virtual SimpleNameAST* clone(MemoryPool *pool) const;

protected:
    virtual void accept0(ASTVisitor* visitor);
    virtual bool match0(AST* , ASTMatcher *);
};

class CFE_API DestructorNameAST: public NameAST
{
public:
    unsigned tilde_token;
    NameAST* unqualified_name;

public:
    DestructorNameAST()
        : tilde_token(0)
        , unqualified_name(0)
    {}

    virtual DestructorNameAST* asDestructorName() { return this; }

    virtual unsigned firstToken() const;
    virtual unsigned lastToken() const;

    virtual DestructorNameAST* clone(MemoryPool *pool) const;

protected:
    virtual void accept0(ASTVisitor* visitor);
    virtual bool match0(AST* , ASTMatcher *);
};

class CFE_API TemplateIdAST: public NameAST
{
public:
    unsigned template_token;
    unsigned identifier_token;
    unsigned less_token;
    ExpressionListAST* template_argument_list;
    unsigned greater_token;

public:
    TemplateIdAST()
        : template_token(0)
        , identifier_token(0)
        , less_token(0)
        , template_argument_list(0)
        , greater_token(0)
    {}

    virtual TemplateIdAST* asTemplateId() { return this; }

    virtual unsigned firstToken() const;
    virtual unsigned lastToken() const;

    virtual TemplateIdAST* clone(MemoryPool *pool) const;

protected:
    virtual void accept0(ASTVisitor* visitor);
    virtual bool match0(AST* , ASTMatcher *);
};

class CFE_API NamespaceAST: public DeclarationAST
{
public:
    unsigned inline_token;
    unsigned namespace_token;
    unsigned identifier_token;
    SpecifierListAST* attribute_list;
    DeclarationAST* linkage_body;

public: // annotations
    Namespace *symbol;

public:
    NamespaceAST()
        : inline_token(0)
        , namespace_token(0)
        , identifier_token(0)
        , attribute_list(0)
        , linkage_body(0)
        , symbol(0)
    {}

    virtual NamespaceAST* asNamespace() { return this; }

    virtual unsigned firstToken() const;
    virtual unsigned lastToken() const;

    virtual NamespaceAST* clone(MemoryPool *pool) const;

protected:
    virtual void accept0(ASTVisitor* visitor);
    virtual bool match0(AST* , ASTMatcher *);
};

class CFE_API NamespaceAliasDefinitionAST: public DeclarationAST
{
public:
    unsigned namespace_token;
    unsigned namespace_name_token;
    unsigned equal_token;
    NameAST* name;
    unsigned semicolon_token;

public:
    NamespaceAliasDefinitionAST()
        : namespace_token(0)
        , namespace_name_token(0)
        , equal_token(0)
        , name(0)
        , semicolon_token(0)
    {}

    virtual NamespaceAliasDefinitionAST* asNamespaceAliasDefinition() { return this; }

    virtual unsigned firstToken() const;
    virtual unsigned lastToken() const;

    virtual NamespaceAliasDefinitionAST* clone(MemoryPool *pool) const;

protected:
    virtual void accept0(ASTVisitor* visitor);
    virtual bool match0(AST* , ASTMatcher *);
};

class CFE_API AliasDeclarationAST: public DeclarationAST
{
public:
    unsigned using_token;
    NameAST* name;
    unsigned equal_token;
    TypeIdAST* typeId;
    unsigned semicolon_token;

public: // annotations
    Declaration *symbol;

public:
    AliasDeclarationAST()
        : using_token(0)
        , name(0)
        , equal_token(0)
        , typeId(0)
        , semicolon_token(0)
        , symbol(0)
    {}

    virtual AliasDeclarationAST* asAliasDeclaration() { return this; }

    virtual unsigned firstToken() const;
    virtual unsigned lastToken() const;

    virtual AliasDeclarationAST* clone(MemoryPool *pool) const;

protected:
    virtual void accept0(ASTVisitor* visitor);
    virtual bool match0(AST* , ASTMatcher *);
};

class CFE_API ExpressionListParenAST: public ExpressionAST
{
public:
    unsigned lparen_token;
    ExpressionListAST* expression_list;
    unsigned rparen_token;

public:
    ExpressionListParenAST()
        : lparen_token(0)
        , expression_list(0)
        , rparen_token(0)
    {}

    virtual ExpressionListParenAST* asExpressionListParen() { return this; }

    virtual unsigned firstToken() const;
    virtual unsigned lastToken() const;

    virtual ExpressionListParenAST* clone(MemoryPool *pool) const;

protected:
    virtual void accept0(ASTVisitor* visitor);
    virtual bool match0(AST* , ASTMatcher *);
};

class CFE_API NewArrayDeclaratorAST: public AST
{
public:
    unsigned lbracket_token;
    ExpressionAST* expression;
    unsigned rbracket_token;

public:
    NewArrayDeclaratorAST()
        : lbracket_token(0)
        , expression(0)
        , rbracket_token(0)
    {}

    virtual NewArrayDeclaratorAST* asNewArrayDeclarator() { return this; }

    virtual unsigned firstToken() const;
    virtual unsigned lastToken() const;

    virtual NewArrayDeclaratorAST* clone(MemoryPool *pool) const;

protected:
    virtual void accept0(ASTVisitor* visitor);
    virtual bool match0(AST* , ASTMatcher *);
};

class CFE_API NewExpressionAST: public ExpressionAST
{
public:
    unsigned scope_token;
    unsigned new_token;
    ExpressionListParenAST* new_placement;

    unsigned lparen_token;
    ExpressionAST* type_id;
    unsigned rparen_token;

    NewTypeIdAST* new_type_id;

    ExpressionAST* new_initializer; // either ExpressionListParenAST or BracedInitializerAST

public:
    NewExpressionAST()
        : scope_token(0)
        , new_token(0)
        , new_placement(0)
        , lparen_token(0)
        , type_id(0)
        , rparen_token(0)
        , new_type_id(0)
        , new_initializer(0)
    {}

    virtual NewExpressionAST* asNewExpression() { return this; }

    virtual unsigned firstToken() const;
    virtual unsigned lastToken() const;

    virtual NewExpressionAST* clone(MemoryPool *pool) const;

protected:
    virtual void accept0(ASTVisitor* visitor);
    virtual bool match0(AST* , ASTMatcher *);
};

class CFE_API NewTypeIdAST: public AST
{
public:
    SpecifierListAST* type_specifier_list;
    PtrOperatorListAST* ptr_operator_list;
    NewArrayDeclaratorListAST* new_array_declarator_list;

public:
    NewTypeIdAST()
        : type_specifier_list(0)
        , ptr_operator_list(0)
        , new_array_declarator_list(0)
    {}

    virtual NewTypeIdAST* asNewTypeId() { return this; }

    virtual unsigned firstToken() const;
    virtual unsigned lastToken() const;

    virtual NewTypeIdAST* clone(MemoryPool *pool) const;

protected:
    virtual void accept0(ASTVisitor* visitor);
    virtual bool match0(AST* , ASTMatcher *);
};

class CFE_API OperatorAST: public AST
{
public:
    unsigned op_token;
    unsigned open_token;
    unsigned close_token;

public:
    OperatorAST()
        : op_token(0)
        , open_token(0)
        , close_token(0)
    {}

    virtual OperatorAST* asOperator() { return this; }

    virtual unsigned firstToken() const;
    virtual unsigned lastToken() const;

    virtual OperatorAST* clone(MemoryPool *pool) const;

protected:
    virtual void accept0(ASTVisitor* visitor);
    virtual bool match0(AST* , ASTMatcher *);
};

class CFE_API ParameterDeclarationAST: public DeclarationAST
{
public:
    SpecifierListAST* type_specifier_list;
    DeclaratorAST* declarator;
    unsigned equal_token;
    ExpressionAST* expression;

public: // annotations
    Argument *symbol;

public:
    ParameterDeclarationAST()
        : type_specifier_list(0)
        , declarator(0)
        , equal_token(0)
        , expression(0)
        , symbol(0)
    {}

    virtual ParameterDeclarationAST* asParameterDeclaration() { return this; }

    virtual unsigned firstToken() const;
    virtual unsigned lastToken() const;

    virtual ParameterDeclarationAST* clone(MemoryPool *pool) const;

protected:
    virtual void accept0(ASTVisitor* visitor);
    virtual bool match0(AST* , ASTMatcher *);
};

class CFE_API ParameterDeclarationClauseAST: public AST
{
public:
    ParameterDeclarationListAST* parameter_declaration_list;
    unsigned dot_dot_dot_token;

public:
    ParameterDeclarationClauseAST()
        : parameter_declaration_list(0)
        , dot_dot_dot_token(0)
    {}

    virtual ParameterDeclarationClauseAST* asParameterDeclarationClause() { return this; }

    virtual unsigned firstToken() const;
    virtual unsigned lastToken() const;

    virtual ParameterDeclarationClauseAST* clone(MemoryPool *pool) const;

protected:
    virtual void accept0(ASTVisitor* visitor);
    virtual bool match0(AST* , ASTMatcher *);
};

class CFE_API CallAST: public PostfixAST
{
public:
    ExpressionAST* base_expression;
    unsigned lparen_token;
    ExpressionListAST* expression_list;
    unsigned rparen_token;

public:
    CallAST()
        : base_expression(0)
        , lparen_token(0)
        , expression_list(0)
        , rparen_token(0)
    {}

    virtual CallAST* asCall() { return this; }

    virtual unsigned firstToken() const;
    virtual unsigned lastToken() const;

    virtual CallAST* clone(MemoryPool *pool) const;

protected:
    virtual void accept0(ASTVisitor* visitor);
    virtual bool match0(AST* , ASTMatcher *);
};

class CFE_API ArrayAccessAST: public PostfixAST
{
public:
    ExpressionAST* base_expression;
    unsigned lbracket_token;
    ExpressionAST* expression;
    unsigned rbracket_token;

public:
    ArrayAccessAST()
        : base_expression(0)
        , lbracket_token(0)
        , expression(0)
        , rbracket_token(0)
    {}

    virtual ArrayAccessAST* asArrayAccess() { return this; }

    virtual unsigned firstToken() const;
    virtual unsigned lastToken() const;

    virtual ArrayAccessAST* clone(MemoryPool *pool) const;

protected:
    virtual void accept0(ASTVisitor* visitor);
    virtual bool match0(AST* , ASTMatcher *);
};

class CFE_API PostIncrDecrAST: public PostfixAST
{
public:
    ExpressionAST* base_expression;
    unsigned incr_decr_token;

public:
    PostIncrDecrAST()
        : base_expression(0)
        , incr_decr_token(0)
    {}

    virtual PostIncrDecrAST* asPostIncrDecr() { return this; }

    virtual unsigned firstToken() const;
    virtual unsigned lastToken() const;

    virtual PostIncrDecrAST* clone(MemoryPool *pool) const;

protected:
    virtual void accept0(ASTVisitor* visitor);
    virtual bool match0(AST* , ASTMatcher *);
};

class CFE_API MemberAccessAST: public PostfixAST
{
public:
    ExpressionAST* base_expression;
    unsigned access_token;
    unsigned template_token;
    NameAST* member_name;

public:
    MemberAccessAST()
        : base_expression(0)
        , access_token(0)
        , template_token(0)
        , member_name(0)
    {}

    virtual MemberAccessAST* asMemberAccess() { return this; }

    virtual unsigned firstToken() const;
    virtual unsigned lastToken() const;

    virtual MemberAccessAST* clone(MemoryPool *pool) const;

protected:
    virtual void accept0(ASTVisitor* visitor);
    virtual bool match0(AST* , ASTMatcher *);
};

class CFE_API TypeidExpressionAST: public ExpressionAST
{
public:
    unsigned typeid_token;
    unsigned lparen_token;
    ExpressionAST* expression;
    unsigned rparen_token;

public:
    TypeidExpressionAST()
        : typeid_token(0)
        , lparen_token(0)
        , expression(0)
        , rparen_token(0)
    {}

    virtual TypeidExpressionAST* asTypeidExpression() { return this; }

    virtual unsigned firstToken() const;
    virtual unsigned lastToken() const;

    virtual TypeidExpressionAST* clone(MemoryPool *pool) const;

protected:
    virtual void accept0(ASTVisitor* visitor);
    virtual bool match0(AST* , ASTMatcher *);
};

class CFE_API TypenameCallExpressionAST: public ExpressionAST
{
public:
    unsigned typename_token;
    NameAST* name;
    ExpressionAST* expression; // either ExpressionListParenAST or BracedInitializerAST

public:
    TypenameCallExpressionAST()
        : typename_token(0)
        , name(0)
        , expression(0)
    {}

    virtual TypenameCallExpressionAST* asTypenameCallExpression() { return this; }

    virtual unsigned firstToken() const;
    virtual unsigned lastToken() const;

    virtual TypenameCallExpressionAST* clone(MemoryPool *pool) const;

protected:
    virtual void accept0(ASTVisitor* visitor);
    virtual bool match0(AST* , ASTMatcher *);
};

class CFE_API TypeConstructorCallAST: public ExpressionAST
{
public:
    SpecifierListAST* type_specifier_list;
    ExpressionAST* expression; // either ExpressionListParenAST or BracedInitializerAST

public:
    TypeConstructorCallAST()
        : type_specifier_list(0)
        , expression(0)
    {}

    virtual TypeConstructorCallAST* asTypeConstructorCall() { return this; }

    virtual unsigned firstToken() const;
    virtual unsigned lastToken() const;

    virtual TypeConstructorCallAST* clone(MemoryPool *pool) const;

protected:
    virtual void accept0(ASTVisitor* visitor);
    virtual bool match0(AST* , ASTMatcher *);
};

class CFE_API PointerToMemberAST: public PtrOperatorAST
{
public:
    unsigned global_scope_token;
    NestedNameSpecifierListAST* nested_name_specifier_list;
    unsigned star_token;
    SpecifierListAST* cv_qualifier_list;
    unsigned ref_qualifier_token;

public:
    PointerToMemberAST()
        : global_scope_token(0)
        , nested_name_specifier_list(0)
        , star_token(0)
        , cv_qualifier_list(0)
        , ref_qualifier_token(0)
    {}

    virtual PointerToMemberAST* asPointerToMember() { return this; }

    virtual unsigned firstToken() const;
    virtual unsigned lastToken() const;

    virtual PointerToMemberAST* clone(MemoryPool *pool) const;

protected:
    virtual void accept0(ASTVisitor* visitor);
    virtual bool match0(AST* , ASTMatcher *);
};

class CFE_API PointerAST: public PtrOperatorAST
{
public:
    unsigned star_token;
    SpecifierListAST* cv_qualifier_list;

public:
    PointerAST()
        : star_token(0)
        , cv_qualifier_list(0)
    {}

    virtual PointerAST* asPointer() { return this; }

    virtual unsigned firstToken() const;
    virtual unsigned lastToken() const;

    virtual PointerAST* clone(MemoryPool *pool) const;

protected:
    virtual void accept0(ASTVisitor* visitor);
    virtual bool match0(AST* , ASTMatcher *);
};

class CFE_API ReferenceAST: public PtrOperatorAST
{
public:
    unsigned reference_token;

public:
    ReferenceAST()
        : reference_token(0)
    {}

    virtual ReferenceAST* asReference() { return this; }

    virtual unsigned firstToken() const;
    virtual unsigned lastToken() const;

    virtual ReferenceAST* clone(MemoryPool *pool) const;

protected:
    virtual void accept0(ASTVisitor* visitor);
    virtual bool match0(AST* , ASTMatcher *);
};

class CFE_API BreakStatementAST: public StatementAST
{
public:
    unsigned break_token;
    unsigned semicolon_token;

public:
    BreakStatementAST()
        : break_token(0)
        , semicolon_token(0)
    {}

    virtual BreakStatementAST* asBreakStatement() { return this; }

    virtual unsigned firstToken() const;
    virtual unsigned lastToken() const;

    virtual BreakStatementAST* clone(MemoryPool *pool) const;

protected:
    virtual void accept0(ASTVisitor* visitor);
    virtual bool match0(AST* , ASTMatcher *);
};

class CFE_API ContinueStatementAST: public StatementAST
{
public:
    unsigned continue_token;
    unsigned semicolon_token;

public:
    ContinueStatementAST()
        : continue_token(0)
        , semicolon_token(0)
    {}

    virtual ContinueStatementAST* asContinueStatement() { return this; }

    virtual unsigned firstToken() const;
    virtual unsigned lastToken() const;

    virtual ContinueStatementAST* clone(MemoryPool *pool) const;

protected:
    virtual void accept0(ASTVisitor* visitor);
    virtual bool match0(AST* , ASTMatcher *);
};

class CFE_API GotoStatementAST: public StatementAST
{
public:
    unsigned goto_token;
    unsigned identifier_token;
    unsigned semicolon_token;

public:
    GotoStatementAST()
        : goto_token(0)
        , identifier_token(0)
        , semicolon_token(0)
    {}

    virtual GotoStatementAST* asGotoStatement() { return this; }

    virtual unsigned firstToken() const;
    virtual unsigned lastToken() const;

    virtual GotoStatementAST* clone(MemoryPool *pool) const;

protected:
    virtual void accept0(ASTVisitor* visitor);
    virtual bool match0(AST* , ASTMatcher *);
};

class CFE_API ReturnStatementAST: public StatementAST
{
public:
    unsigned return_token;
    ExpressionAST* expression;
    unsigned semicolon_token;

public:
    ReturnStatementAST()
        : return_token(0)
        , expression(0)
        , semicolon_token(0)
    {}

    virtual ReturnStatementAST* asReturnStatement() { return this; }

    virtual unsigned firstToken() const;
    virtual unsigned lastToken() const;

    virtual ReturnStatementAST* clone(MemoryPool *pool) const;

protected:
    virtual void accept0(ASTVisitor* visitor);
    virtual bool match0(AST* , ASTMatcher *);
};

class CFE_API SizeofExpressionAST: public ExpressionAST
{
public:
    unsigned sizeof_token;
    unsigned dot_dot_dot_token;
    unsigned lparen_token;
    ExpressionAST* expression;
    unsigned rparen_token;

public: // annotations
    FullySpecifiedType expression_type;

public:
    SizeofExpressionAST()
        : sizeof_token(0)
        , dot_dot_dot_token(0)
        , lparen_token(0)
        , expression(0)
        , rparen_token(0)
    {}

    virtual SizeofExpressionAST* asSizeofExpression() { return this; }

    virtual unsigned firstToken() const;
    virtual unsigned lastToken() const;

    virtual SizeofExpressionAST* clone(MemoryPool *pool) const;

protected:
    virtual void accept0(ASTVisitor* visitor);
    virtual bool match0(AST* , ASTMatcher *);
};

class CFE_API AlignofExpressionAST: public ExpressionAST
{
public:
    unsigned alignof_token;
    unsigned lparen_token;
    TypeIdAST* typeId;
    unsigned rparen_token;

public:
    AlignofExpressionAST()
        : alignof_token(0)
        , lparen_token(0)
        , typeId(0)
        , rparen_token(0)
    {}

    virtual AlignofExpressionAST* asAlignofExpression() { return this; }

    virtual unsigned firstToken() const;
    virtual unsigned lastToken() const;

    virtual AlignofExpressionAST* clone(MemoryPool *pool) const;

protected:
    virtual void accept0(ASTVisitor* visitor);
    virtual bool match0(AST* , ASTMatcher *);
};

class CFE_API PointerLiteralAST: public ExpressionAST
{
public:
    unsigned literal_token;

public:
    PointerLiteralAST()
        : literal_token(0)
    {}

    virtual PointerLiteralAST* asPointerLiteral() { return this; }

    virtual unsigned firstToken() const;
    virtual unsigned lastToken() const;

    virtual PointerLiteralAST* clone(MemoryPool *pool) const;

protected:
    virtual void accept0(ASTVisitor* visitor);
    virtual bool match0(AST* , ASTMatcher *);
};

class CFE_API NumericLiteralAST: public ExpressionAST
{
public:
    unsigned literal_token;

public:
    NumericLiteralAST()
        : literal_token(0)
    {}

    virtual NumericLiteralAST* asNumericLiteral() { return this; }

    virtual unsigned firstToken() const;
    virtual unsigned lastToken() const;

    virtual NumericLiteralAST* clone(MemoryPool *pool) const;

protected:
    virtual void accept0(ASTVisitor* visitor);
    virtual bool match0(AST* , ASTMatcher *);
};

class CFE_API BoolLiteralAST: public ExpressionAST
{
public:
    unsigned literal_token;

public:
    BoolLiteralAST()
        : literal_token(0)
    {}

    virtual BoolLiteralAST* asBoolLiteral() { return this; }

    virtual unsigned firstToken() const;
    virtual unsigned lastToken() const;

    virtual BoolLiteralAST* clone(MemoryPool *pool) const;

protected:
    virtual void accept0(ASTVisitor* visitor);
    virtual bool match0(AST* , ASTMatcher *);
};

class CFE_API ThisExpressionAST: public ExpressionAST
{
public:
    unsigned this_token;

public:
    ThisExpressionAST()
        : this_token(0)
    {}

    virtual ThisExpressionAST* asThisExpression() { return this; }

    virtual unsigned firstToken() const;
    virtual unsigned lastToken() const;

    virtual ThisExpressionAST* clone(MemoryPool *pool) const;

protected:
    virtual void accept0(ASTVisitor* visitor);
    virtual bool match0(AST* , ASTMatcher *);
};

class CFE_API NestedExpressionAST: public ExpressionAST
{
public:
    unsigned lparen_token;
    ExpressionAST* expression;
    unsigned rparen_token;

public:
    NestedExpressionAST()
        : lparen_token(0)
        , expression(0)
        , rparen_token(0)
    {}

    virtual NestedExpressionAST* asNestedExpression() { return this; }

    virtual unsigned firstToken() const;
    virtual unsigned lastToken() const;

    virtual NestedExpressionAST* clone(MemoryPool *pool) const;

protected:
    virtual void accept0(ASTVisitor* visitor);
    virtual bool match0(AST* , ASTMatcher *);
};

class CFE_API StaticAssertDeclarationAST: public DeclarationAST
{
public:
    unsigned static_assert_token;
    unsigned lparen_token;
    ExpressionAST* expression;
    unsigned comma_token;
    ExpressionAST* string_literal;
    unsigned rparen_token;
    unsigned semicolon_token;

public:
    StaticAssertDeclarationAST()
        : static_assert_token(0)
        , lparen_token(0)
        , expression(0)
        , comma_token(0)
        , string_literal(0)
        , rparen_token(0)
        , semicolon_token(0)
    {}

    virtual StaticAssertDeclarationAST* asStaticAssertDeclaration() { return this; }

    virtual unsigned firstToken() const;
    virtual unsigned lastToken() const;

    virtual StaticAssertDeclarationAST* clone(MemoryPool *pool) const;

protected:
    virtual void accept0(ASTVisitor* visitor);
    virtual bool match0(AST* , ASTMatcher *);
};

class CFE_API StringLiteralAST: public ExpressionAST
{
public:
    unsigned literal_token;
    StringLiteralAST* next;

public:
    StringLiteralAST()
        : literal_token(0)
        , next(0)
    {}

    virtual StringLiteralAST* asStringLiteral() { return this; }

    virtual unsigned firstToken() const;
    virtual unsigned lastToken() const;

    virtual StringLiteralAST* clone(MemoryPool *pool) const;

protected:
    virtual void accept0(ASTVisitor* visitor);
    virtual bool match0(AST* , ASTMatcher *);
};

class CFE_API SwitchStatementAST: public StatementAST
{
public:
    unsigned switch_token;
    unsigned lparen_token;
    ExpressionAST* condition;
    unsigned rparen_token;
    StatementAST* statement;

public: // annotations
    Block *symbol;

public:
    SwitchStatementAST()
        : switch_token(0)
        , lparen_token(0)
        , condition(0)
        , rparen_token(0)
        , statement(0)
        , symbol(0)
    {}

    virtual SwitchStatementAST* asSwitchStatement() { return this; }

    virtual unsigned firstToken() const;
    virtual unsigned lastToken() const;

    virtual SwitchStatementAST* clone(MemoryPool *pool) const;

protected:
    virtual void accept0(ASTVisitor* visitor);
    virtual bool match0(AST* , ASTMatcher *);
};

class CFE_API TemplateDeclarationAST: public DeclarationAST
{
public:
    unsigned export_token;
    unsigned template_token;
    unsigned less_token;
    DeclarationListAST* template_parameter_list;
    unsigned greater_token;
    DeclarationAST* declaration;

public: // annotations
    Template *symbol;

public:
    TemplateDeclarationAST()
        : export_token(0)
        , template_token(0)
        , less_token(0)
        , template_parameter_list(0)
        , greater_token(0)
        , declaration(0)
        , symbol(0)
    {}

    virtual TemplateDeclarationAST* asTemplateDeclaration() { return this; }

    virtual unsigned firstToken() const;
    virtual unsigned lastToken() const;

    virtual TemplateDeclarationAST* clone(MemoryPool *pool) const;

protected:
    virtual void accept0(ASTVisitor* visitor);
    virtual bool match0(AST* , ASTMatcher *);
};

class CFE_API ThrowExpressionAST: public ExpressionAST
{
public:
    unsigned throw_token;
    ExpressionAST* expression;

public:
    ThrowExpressionAST()
        : throw_token(0)
        , expression(0)
    {}

    virtual ThrowExpressionAST* asThrowExpression() { return this; }

    virtual unsigned firstToken() const;
    virtual unsigned lastToken() const;

    virtual ThrowExpressionAST* clone(MemoryPool *pool) const;

protected:
    virtual void accept0(ASTVisitor* visitor);
    virtual bool match0(AST* , ASTMatcher *);
};

class CFE_API NoExceptOperatorExpressionAST: public ExpressionAST
{
public:
    unsigned noexcept_token;
    ExpressionAST* expression;

public:
    NoExceptOperatorExpressionAST()
        : noexcept_token(0)
        , expression(0)
    {}

    virtual NoExceptOperatorExpressionAST* asNoExceptOperatorExpression() { return this; }

    virtual unsigned firstToken() const;
    virtual unsigned lastToken() const;

    virtual NoExceptOperatorExpressionAST* clone(MemoryPool *pool) const;

protected:
    virtual void accept0(ASTVisitor* visitor);
    virtual bool match0(AST* , ASTMatcher *);
};

class CFE_API TranslationUnitAST: public AST
{
public:
    DeclarationListAST* declaration_list;

public:
    TranslationUnitAST()
        : declaration_list(0)
    {}

    virtual TranslationUnitAST* asTranslationUnit() { return this; }

    virtual unsigned firstToken() const;
    virtual unsigned lastToken() const;

    virtual TranslationUnitAST* clone(MemoryPool *pool) const;

protected:
    virtual void accept0(ASTVisitor* visitor);
    virtual bool match0(AST* , ASTMatcher *);
};

class CFE_API TryBlockStatementAST: public StatementAST
{
public:
    unsigned try_token;
    StatementAST* statement;
    CatchClauseListAST* catch_clause_list;

public:
    TryBlockStatementAST()
        : try_token(0)
        , statement(0)
        , catch_clause_list(0)
    {}

    virtual TryBlockStatementAST* asTryBlockStatement() { return this; }

    virtual unsigned firstToken() const;
    virtual unsigned lastToken() const;

    virtual TryBlockStatementAST* clone(MemoryPool *pool) const;

protected:
    virtual void accept0(ASTVisitor* visitor);
    virtual bool match0(AST* , ASTMatcher *);
};

class CFE_API CatchClauseAST: public StatementAST
{
public:
    unsigned catch_token;
    unsigned lparen_token;
    ExceptionDeclarationAST* exception_declaration;
    unsigned rparen_token;
    StatementAST* statement;

public: // annotations
    Block *symbol;

public:
    CatchClauseAST()
        : catch_token(0)
        , lparen_token(0)
        , exception_declaration(0)
        , rparen_token(0)
        , statement(0)
        , symbol(0)
    {}

    virtual CatchClauseAST* asCatchClause() { return this; }

    virtual unsigned firstToken() const;
    virtual unsigned lastToken() const;

    virtual CatchClauseAST* clone(MemoryPool *pool) const;

protected:
    virtual void accept0(ASTVisitor* visitor);
    virtual bool match0(AST* , ASTMatcher *);
};

class CFE_API TypeIdAST: public ExpressionAST
{
public:
    SpecifierListAST* type_specifier_list;
    DeclaratorAST* declarator;

public:
    TypeIdAST()
        : type_specifier_list(0)
        , declarator(0)
    {}

    virtual TypeIdAST* asTypeId() { return this; }

    virtual unsigned firstToken() const;
    virtual unsigned lastToken() const;

    virtual TypeIdAST* clone(MemoryPool *pool) const;

protected:
    virtual void accept0(ASTVisitor* visitor);
    virtual bool match0(AST* , ASTMatcher *);
};

class CFE_API TypenameTypeParameterAST: public DeclarationAST
{
public:
    unsigned classkey_token;
    unsigned dot_dot_dot_token;
    NameAST* name;
    unsigned equal_token;
    ExpressionAST* type_id;

public: // annotations
    TypenameArgument *symbol;

public:
    TypenameTypeParameterAST()
        : classkey_token(0)
        , dot_dot_dot_token(0)
        , name(0)
        , equal_token(0)
        , type_id(0)
        , symbol(0)
    {}

    virtual TypenameTypeParameterAST* asTypenameTypeParameter() { return this; }

    virtual unsigned firstToken() const;
    virtual unsigned lastToken() const;

    virtual TypenameTypeParameterAST* clone(MemoryPool *pool) const;

protected:
    virtual void accept0(ASTVisitor* visitor);
    virtual bool match0(AST* , ASTMatcher *);
};

class CFE_API TemplateTypeParameterAST: public DeclarationAST
{
public:
    unsigned template_token;
    unsigned less_token;
    DeclarationListAST* template_parameter_list;
    unsigned greater_token;
    unsigned class_token;
    unsigned dot_dot_dot_token;
    NameAST* name;
    unsigned equal_token;
    ExpressionAST* type_id;

public:
    TypenameArgument *symbol;

public:
    TemplateTypeParameterAST()
        : template_token(0)
        , less_token(0)
        , template_parameter_list(0)
        , greater_token(0)
        , class_token(0)
        , dot_dot_dot_token(0)
        , name(0)
        , equal_token(0)
        , type_id(0)
        , symbol(0)
    {}

    virtual TemplateTypeParameterAST* asTemplateTypeParameter() { return this; }

    virtual unsigned firstToken() const;
    virtual unsigned lastToken() const;

    virtual TemplateTypeParameterAST* clone(MemoryPool *pool) const;

protected:
    virtual void accept0(ASTVisitor* visitor);
    virtual bool match0(AST* , ASTMatcher *);
};

class CFE_API UnaryExpressionAST: public ExpressionAST
{
public:
    unsigned unary_op_token;
    ExpressionAST* expression;

public:
    UnaryExpressionAST()
        : unary_op_token(0)
        , expression(0)
    {}

    virtual UnaryExpressionAST* asUnaryExpression() { return this; }

    virtual unsigned firstToken() const;
    virtual unsigned lastToken() const;

    virtual UnaryExpressionAST* clone(MemoryPool *pool) const;

protected:
    virtual void accept0(ASTVisitor* visitor);
    virtual bool match0(AST* , ASTMatcher *);
};

class CFE_API UsingAST: public DeclarationAST
{
public:
    unsigned using_token;
    unsigned typename_token;
    NameAST* name;
    unsigned semicolon_token;

public: // annotations
    UsingDeclaration *symbol;

public:
    UsingAST()
        : using_token(0)
        , typename_token(0)
        , name(0)
        , semicolon_token(0)
        , symbol(0)
    {}

    virtual UsingAST* asUsing() { return this; }

    virtual unsigned firstToken() const;
    virtual unsigned lastToken() const;

    virtual UsingAST* clone(MemoryPool *pool) const;

protected:
    virtual void accept0(ASTVisitor* visitor);
    virtual bool match0(AST* , ASTMatcher *);
};

class CFE_API UsingDirectiveAST: public DeclarationAST
{
public:
    unsigned using_token;
    unsigned namespace_token;
    NameAST* name;
    unsigned semicolon_token;

public:
    UsingNamespaceDirective *symbol;

public:
    UsingDirectiveAST()
        : using_token(0)
        , namespace_token(0)
        , name(0)
        , semicolon_token(0)
        , symbol(0)
    {}

    virtual UsingDirectiveAST* asUsingDirective() { return this; }

    virtual unsigned firstToken() const;
    virtual unsigned lastToken() const;

    virtual UsingDirectiveAST* clone(MemoryPool *pool) const;

protected:
    virtual void accept0(ASTVisitor* visitor);
    virtual bool match0(AST* , ASTMatcher *);
};

class CFE_API WhileStatementAST: public StatementAST
{
public:
    unsigned while_token;
    unsigned lparen_token;
    ExpressionAST* condition;
    unsigned rparen_token;
    StatementAST* statement;

public: // annotations
    Block *symbol;

public:
    WhileStatementAST()
        : while_token(0)
        , lparen_token(0)
        , condition(0)
        , rparen_token(0)
        , statement(0)
        , symbol(0)
    {}

    virtual WhileStatementAST* asWhileStatement() { return this; }

    virtual unsigned firstToken() const;
    virtual unsigned lastToken() const;

    virtual WhileStatementAST* clone(MemoryPool *pool) const;

protected:
    virtual void accept0(ASTVisitor* visitor);
    virtual bool match0(AST* , ASTMatcher *);
};

class LambdaExpressionAST: public ExpressionAST
{
public:
    LambdaIntroducerAST* lambda_introducer;
    LambdaDeclaratorAST* lambda_declarator;
    StatementAST* statement;

public:
    LambdaExpressionAST()
        : lambda_introducer(0)
        , lambda_declarator(0)
        , statement(0)
    {}

    virtual LambdaExpressionAST* asLambdaExpression() { return this; }

    virtual unsigned firstToken() const;
    virtual unsigned lastToken() const;
    virtual LambdaExpressionAST* clone(MemoryPool *pool) const;

protected:
    virtual void accept0(ASTVisitor* visitor);
    virtual bool match0(AST* , ASTMatcher *);
};

class LambdaIntroducerAST: public AST
{
public:
    unsigned lbracket_token;
    LambdaCaptureAST* lambda_capture;
    unsigned rbracket_token;

public:
    LambdaIntroducerAST()
        : lbracket_token(0)
        , lambda_capture(0)
        , rbracket_token(0)
    {}

    virtual LambdaIntroducerAST* asLambdaIntroducer() { return this; }
    virtual unsigned firstToken() const;
    virtual unsigned lastToken() const;

    virtual LambdaIntroducerAST* clone(MemoryPool *pool) const;

protected:
    virtual void accept0(ASTVisitor* visitor);
    virtual bool match0(AST* , ASTMatcher *);
};

class LambdaCaptureAST: public AST
{
public:
    unsigned default_capture_token;
    CaptureListAST* capture_list;

public:
    LambdaCaptureAST()
        : default_capture_token(0)
        , capture_list(0)
    {}

    virtual LambdaCaptureAST* asLambdaCapture() { return this; }
    virtual unsigned firstToken() const;
    virtual unsigned lastToken() const;

    virtual LambdaCaptureAST* clone(MemoryPool *pool) const;

protected:
    virtual void accept0(ASTVisitor* visitor);
    virtual bool match0(AST* , ASTMatcher *);
};

class CaptureAST: public AST
{
public:
    unsigned amper_token;
    NameAST* identifier;

public:
    CaptureAST()
        : amper_token(0)
        , identifier(0)
    {}

    virtual CaptureAST* asCapture() { return this; }
    virtual unsigned firstToken() const;
    virtual unsigned lastToken() const;

    virtual CaptureAST* clone(MemoryPool *pool) const;

protected:
    virtual void accept0(ASTVisitor* visitor);
    virtual bool match0(AST* , ASTMatcher *);
};

class LambdaDeclaratorAST: public AST
{
public:
    unsigned lparen_token;
    ParameterDeclarationClauseAST* parameter_declaration_clause;
    unsigned rparen_token;
    SpecifierListAST* attributes;
    unsigned mutable_token;
    ExceptionSpecificationAST* exception_specification;
    TrailingReturnTypeAST* trailing_return_type;

public: // annotations
    Function *symbol;

public:
    LambdaDeclaratorAST()
        : lparen_token(0)
        , parameter_declaration_clause(0)
        , rparen_token(0)
        , attributes(0)
        , mutable_token(0)
        , exception_specification(0)
        , trailing_return_type(0)
        , symbol(0)
    {}

    virtual LambdaDeclaratorAST* asLambdaDeclarator() { return this; }
    virtual unsigned firstToken() const;
    virtual unsigned lastToken() const;

    virtual LambdaDeclaratorAST* clone(MemoryPool *pool) const;

protected:
    virtual void accept0(ASTVisitor* visitor);
    virtual bool match0(AST* , ASTMatcher *);
};

class TrailingReturnTypeAST: public AST
{
public:
    unsigned arrow_token;
    SpecifierListAST* attributes;
    SpecifierListAST* type_specifier_list;
    DeclaratorAST* declarator;

public:
    TrailingReturnTypeAST()
        : arrow_token(0)
        , attributes(0)
        , type_specifier_list(0)
        , declarator(0)
    {}

    virtual TrailingReturnTypeAST* asTrailingReturnType() { return this; }
    virtual unsigned firstToken() const;
    virtual unsigned lastToken() const;

    virtual TrailingReturnTypeAST* clone(MemoryPool *pool) const;

protected:
    virtual void accept0(ASTVisitor* visitor);
    virtual bool match0(AST* , ASTMatcher *);
};

class BracedInitializerAST: public ExpressionAST
{
public:
    unsigned lbrace_token;
    ExpressionListAST* expression_list;
    unsigned comma_token;
    unsigned rbrace_token;

public:
    BracedInitializerAST()
        : lbrace_token(0)
        , expression_list(0)
        , comma_token(0)
        , rbrace_token(0)
    {}

    virtual BracedInitializerAST* asBracedInitializer() { return this; }
    virtual unsigned firstToken() const;
    virtual unsigned lastToken() const;

    virtual BracedInitializerAST* clone(MemoryPool *pool) const;

protected:
    virtual void accept0(ASTVisitor* visitor);
    virtual bool match0(AST* , ASTMatcher *);
};

class DesignatorAST: public AST
{
public:
    DesignatorAST()
    {}

    virtual DesignatorAST* asDesignator() { return this; }
    virtual DesignatorAST* clone(MemoryPool *pool) const = 0;
};

class DotDesignatorAST: public DesignatorAST
{
public:
    unsigned dot_token;
    unsigned identifier_token;
public:
    DotDesignatorAST()
        : dot_token(0)
        , identifier_token(0)
    {}

    virtual DotDesignatorAST* asDotDesignator() { return this; }
    virtual unsigned firstToken() const;
    virtual unsigned lastToken() const;

    virtual DotDesignatorAST* clone(MemoryPool *pool) const;

protected:
    virtual void accept0(ASTVisitor* visitor);
    virtual bool match0(AST* , ASTMatcher *);
};

class BracketDesignatorAST: public DesignatorAST
{
public:
    unsigned lbracket_token;
    ExpressionAST* expression;
    unsigned rbracket_token;
public:
    BracketDesignatorAST()
        : lbracket_token(0)
        , expression(0)
        , rbracket_token(0)
    {}

    virtual BracketDesignatorAST* asBracketDesignator() { return this; }
    virtual unsigned firstToken() const;
    virtual unsigned lastToken() const;

    virtual BracketDesignatorAST* clone(MemoryPool *pool) const;

protected:
    virtual void accept0(ASTVisitor* visitor);
    virtual bool match0(AST* , ASTMatcher *);
};

class DesignatedInitializerAST: public ExpressionAST
{
public:
    DesignatorListAST* designator_list;
    unsigned equal_token;
    ExpressionAST* initializer;

public:
    DesignatedInitializerAST()
        : designator_list(0)
        , equal_token(0)
        , initializer(0)
    {}

    virtual DesignatedInitializerAST* asDesignatedInitializer() { return this; }
    virtual unsigned firstToken() const;
    virtual unsigned lastToken() const;

    virtual DesignatedInitializerAST* clone(MemoryPool *pool) const;

protected:
    virtual void accept0(ASTVisitor* visitor);
    virtual bool match0(AST* , ASTMatcher *);
};

} // namespace psyche

#endif
