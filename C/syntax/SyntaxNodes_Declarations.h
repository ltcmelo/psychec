// Copyright (c) 2021/22 Leandro T. C. Melo <ltcmelo@gmail.com>
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

#ifndef PSYCHE_C_SYNTAX_NODES_DECLARATIONS_H__
#define PSYCHE_C_SYNTAX_NODES_DECLARATIONS_H__

#include "SyntaxNode.h"
#include "SyntaxToken.h"
#include "SyntaxTree.h"
#include "SyntaxVisitor.h"

#include "parser/LexedTokens.h"

namespace psy {
namespace C {

/* Specifiers */
/**
 * \brief The TrivialSpecifierSyntax class.
 *
 * The base class of every \c specifier that is:
 *
 *  - a \a storage-class-specifier,
 *  - a \a type-specifer of a builtin,
 *  - a \a type-qualifier, or
 *  - a \a function-specifier.
 */
class PSY_C_API TrivialSpecifierSyntax : public SpecifierSyntax
{
    AST_NODE(TrivialSpecifier, Specifier)

public:
    SyntaxToken specifierToken() const { return tokenAtIndex(specTkIdx_); }

protected:
    LexedTokens::IndexType specTkIdx_ = LexedTokens::invalidIndex();
    AST_CHILD_LST1(specTkIdx_);
};

/**
 * \brief The StorageClassSyntax class.
 *
 * \remark 6.7.1
 */
class PSY_C_API StorageClassSyntax final : public TrivialSpecifierSyntax
{
    AST_NODE_NK(StorageClass, TrivialSpecifier)

public:
    SyntaxToken storageClassKeyword() const { return tokenAtIndex(specTkIdx_); }
};

/**
 * \brief The BuiltinTypeSpecifierSyntax class.
 *
 * \remark 6.7.2
 */
class PSY_C_API BuiltinTypeSpecifierSyntax final : public TrivialSpecifierSyntax
{
    AST_NODE_NK(BuiltinTypeSpecifier, TrivialSpecifier)

public:
    SyntaxToken builtinTypeKeyword() const { return tokenAtIndex(specTkIdx_); }
};

/**
 * \brief The TagTypeSpecifierSyntax class.
 *
 * \remark 6.7.2.1, 6.7.2.2, and 6.7.2.3
 */
class PSY_C_API TagTypeSpecifierSyntax final : public SpecifierSyntax
{
    AST_NODE_NK(TagTypeSpecifier, Specifier)

public:
    SyntaxToken keyword() const { return tokenAtIndex(kwTkIdx_); }
    const SpecifierListSyntax* attributes() const { return attrs1_; }
    SyntaxToken tagToken() const { return tokenAtIndex(tagTkIdx_); }
    SyntaxToken openBraceToken() const { return tokenAtIndex(openBraceTkIdx_); }
    const DeclarationListSyntax* declarations() const { return decls_; }
    SyntaxToken closeBraceToken() const { return tokenAtIndex(closeBraceTkIdx_); }
    const SpecifierListSyntax* attributes_PostCloseBrace() const { return attrs2_; }

private:
    LexedTokens::IndexType kwTkIdx_ = LexedTokens::invalidIndex();
    SpecifierListSyntax* attrs1_ = nullptr;
    LexedTokens::IndexType tagTkIdx_ = LexedTokens::invalidIndex();
    LexedTokens::IndexType openBraceTkIdx_ = LexedTokens::invalidIndex();
    DeclarationListSyntax* decls_ = nullptr;
    LexedTokens::IndexType closeBraceTkIdx_ = LexedTokens::invalidIndex();;
    SpecifierListSyntax* attrs2_ = nullptr;
    AST_CHILD_LST7(kwTkIdx_,
                   attrs1_,
                   tagTkIdx_,
                   openBraceTkIdx_,
                   decls_,
                   closeBraceTkIdx_,
                   attrs2_);

    mutable Symbol* sym_ = nullptr;
};

/**
 * \brief The AtomicTypeSpecifierSyntax class.
 *
 * \remark 6.7.2.4
 */
class PSY_C_API AtomicTypeSpecifierSyntax final : public SpecifierSyntax
{
    AST_NODE_1K(AtomicTypeSpecifier, Specifier)

public:
    SyntaxToken atomicKeyword() const { return tokenAtIndex(atomicKwTkIdx_); }
    SyntaxToken openParenthesisToken() const { return tokenAtIndex(openParenTkIdx_); }
    const TypeNameSyntax* typeName() const { return typeName_; }
    SyntaxToken closeParenthesisToken() const { return tokenAtIndex(closeParenTkIdx_); }

private:
    LexedTokens::IndexType atomicKwTkIdx_ = LexedTokens::invalidIndex();
    LexedTokens::IndexType openParenTkIdx_ = LexedTokens::invalidIndex();
    TypeNameSyntax* typeName_ = nullptr;
    LexedTokens::IndexType closeParenTkIdx_ = LexedTokens::invalidIndex();
    AST_CHILD_LST4(atomicKwTkIdx_,
                   openParenTkIdx_,
                   typeName_,
                   closeParenTkIdx_);
};

/**
 * \brief The TypeQualifierSyntax class.
 *
 * \remark 6.7.3
 */
class PSY_C_API TypeQualifierSyntax final : public TrivialSpecifierSyntax
{
    AST_NODE_NK(TypeQualifier, TrivialSpecifier)

public:
    SyntaxToken qualifierKeyword() const { return tokenAtIndex(specTkIdx_); }
};

/**
 * \brief The FunctionSpecifierSyntax class.
 *
 * \remark 6.7.4
 */
class PSY_C_API FunctionSpecifierSyntax final : public TrivialSpecifierSyntax
{
    AST_NODE_NK(FunctionSpecifier, TrivialSpecifier)

public:
    SyntaxToken specifierKeyword() const { return tokenAtIndex(specTkIdx_); }
};

/**
 * \brief The AlignmentSpecifierSyntax class.
 *
 * The specifier's argument is:
 *   - a TypeNameSyntax,
 *   - an ExpressionSyntax, or
 *   - an AmbiguousTypeNameOrExpressionAsTypeReferenceSyntax.
 *
 * The latter case happens if the argument is an \a identifier, making
 * it impossible to determine whether it names a type or variable.
 *
 * \remark 6.7.5
 */
class PSY_C_API AlignmentSpecifierSyntax final : public SpecifierSyntax
{
    AST_NODE_1K(AlignmentSpecifier, Specifier)

public:
    SyntaxToken alignasKeyword() const { return tokenAtIndex(alignasKwTkIdx_); }
    const TypeReferenceSyntax* tyReference() const { return tyRef_; }

private:
    LexedTokens::IndexType alignasKwTkIdx_ = LexedTokens::invalidIndex();
    TypeReferenceSyntax* tyRef_ = nullptr;
    AST_CHILD_LST2(alignasKwTkIdx_, tyRef_);
};

/**
 * \brief The ExtGNU_TypeofSyntax class.
 *
 * The specifier's argument is:
 *   - a TypeNameSyntax,
 *   - an ExpressionSyntax, or
 *   - an AmbiguousTypeNameOrExpressionAsTypeReferenceSyntax.
 *
 * The latter case happens if the argument is a sole \a identifier, making
 * it impossible to determine whether it names a type or variable.
 *
 * \attention Thisis a GNU extension:
 * https://gcc.gnu.org/onlinedocs/gcc/Typeof.html#Typeof
 */
class PSY_C_API ExtGNU_TypeofSyntax final : public SpecifierSyntax
{
    AST_NODE_1K(ExtGNU_Typeof, Specifier);

public:
    SyntaxToken typeofKeyword() const { return tokenAtIndex(typeofKwTkIdx_); }
    const TypeReferenceSyntax* tyReference() const { return tyRef_; }

private:
    LexedTokens::IndexType typeofKwTkIdx_ = LexedTokens::invalidIndex();
    TypeReferenceSyntax* tyRef_ = nullptr;
    AST_CHILD_LST2(typeofKwTkIdx_, tyRef_);
};

/**
 * \brief The ExtGNU_AttributeSpecifierSyntax class.
 *
 * \attention This is a GNU extension:
 *  https://gcc.gnu.org/onlinedocs/gcc/Attribute-Syntax.html
 */
class PSY_C_API ExtGNU_AttributeSpecifierSyntax final : public SpecifierSyntax
{
    AST_NODE_1K(ExtGNU_AttributeSpecifier, Specifier)

public:
    SyntaxToken attributeKeyword() const { return tokenAtIndex(attrKwTkIdx_); }
    SyntaxToken openOuterParenthesisToken() const { return tokenAtIndex(openOuterParenTkIdx_); }
    SyntaxToken openInnerParenthesisToken() const { return tokenAtIndex(openInnerParenTkIdx_); }
    const ExtGNU_AttributeListSyntax* attributes() const { return attrs_; }
    SyntaxToken closeInnerParenthesisToken() const { return tokenAtIndex(closeInnerParenTkIdx_); }
    SyntaxToken closeOuterParenthesisToken() const { return tokenAtIndex(closeOuterParenTkIdx_); }

private:
    LexedTokens::IndexType attrKwTkIdx_ = LexedTokens::invalidIndex();
    LexedTokens::IndexType openOuterParenTkIdx_ = LexedTokens::invalidIndex();
    LexedTokens::IndexType openInnerParenTkIdx_ = LexedTokens::invalidIndex();
    ExtGNU_AttributeListSyntax* attrs_ = nullptr;
    LexedTokens::IndexType closeInnerParenTkIdx_ = LexedTokens::invalidIndex();
    LexedTokens::IndexType closeOuterParenTkIdx_ = LexedTokens::invalidIndex();
    AST_CHILD_LST6(attrKwTkIdx_,
                   openOuterParenTkIdx_,
                   openInnerParenTkIdx_,
                   attrs_,
                   closeInnerParenTkIdx_,
                   closeOuterParenTkIdx_);
};

/**
 * \brief The ExtGNU_AttributeSyntax class.
 *
 * \attention This is a GNU extension:
 * https://gcc.gnu.org/onlinedocs/gcc/Attribute-Syntax.html
 */
class PSY_C_API ExtGNU_AttributeSyntax final : public SyntaxNode
{
    AST_G_NODE_1K(ExtGNU_Attribute)

public:
    SyntaxToken keywordOrIdentifierToken() const { return tokenAtIndex(kwOrIdentTkIdx_); }
    SyntaxToken openParenthesisToken() const { return tokenAtIndex(openParenTkIdx_); }
    const ExpressionListSyntax* expressions() const { return exprs_; }
    SyntaxToken closeParenthesisToken() const { return tokenAtIndex(closeParenTkIdx_); }

private:
    LexedTokens::IndexType kwOrIdentTkIdx_ = LexedTokens::invalidIndex();
    LexedTokens::IndexType openParenTkIdx_ = LexedTokens::invalidIndex();
    ExpressionListSyntax* exprs_ = nullptr;
    LexedTokens::IndexType closeParenTkIdx_  = LexedTokens::invalidIndex();
    AST_CHILD_LST4(kwOrIdentTkIdx_, openParenTkIdx_, exprs_, closeParenTkIdx_);
};

/**
 * \brief The ExtGNU_AttributeSpecifierSyntax class.
 *
 * \attention This is a GNU extension:
 *  https://gcc.gnu.org/onlinedocs/gcc/Attribute-Syntax.html
 *
 * \note Similar to:
 * - \c clang::AsmLabelAttrExpr of LLMV/Clang.
 */
class PSY_C_API ExtGNU_AsmLabelSyntax final : public SpecifierSyntax
{
    AST_NODE_1K(ExtGNU_AsmLabel, Specifier)

public:
    SyntaxToken asmKeyword() const { return tokenAtIndex(asmKwTkIdx_); }
    SyntaxToken openParenthesisToken() const { return tokenAtIndex(openParenTkIdx_); }
    const ExpressionSyntax* stringLiteral() const { return strLit_; }
    SyntaxToken closeParenthesisToken() const { return tokenAtIndex(closeParenTkIdx_); }

private:
    LexedTokens::IndexType asmKwTkIdx_ = LexedTokens::invalidIndex();
    LexedTokens::IndexType openParenTkIdx_ = LexedTokens::invalidIndex();
    ExpressionSyntax* strLit_ = nullptr;
    LexedTokens::IndexType closeParenTkIdx_ = LexedTokens::invalidIndex();
    AST_CHILD_LST4(asmKwTkIdx_,
                   openParenTkIdx_,
                   strLit_,
                   closeParenTkIdx_);
};

/**
 * \brief The ExtPSY_QuantifiedTypeSpecifierSyntax class.
 *
 * \attention This is a Psyche-C extension.
 */
class PSY_C_API ExtPSY_QuantifiedTypeSpecifierSyntax : public SpecifierSyntax
{
    AST_NODE_1K(ExtPSY_QuantifiedTypeSpecifier, Specifier)

private:
    LexedTokens::IndexType quantifierTkIdx_ = LexedTokens::invalidIndex();
    LexedTokens::IndexType openParenTkIdx_ = LexedTokens::invalidIndex();
    LexedTokens::IndexType identTkIdx_ = LexedTokens::invalidIndex();
    LexedTokens::IndexType closeParenTkIdx_ = LexedTokens::invalidIndex();
    AST_CHILD_LST4(quantifierTkIdx_,
                   openParenTkIdx_,
                   identTkIdx_,
                   closeParenTkIdx_);
};

/**
 * \brief The TypedefNameSyntax class.
 *
 * \remark 6.7.8
 */
class PSY_C_API TypedefNameSyntax final : public SpecifierSyntax
{
    AST_NODE_1K(TypedefName, Specifier)

public:
    SyntaxToken identifierToken() const { return tokenAtIndex(identTkIdx_); }

private:
    LexedTokens::IndexType identTkIdx_ = LexedTokens::invalidIndex();
    AST_CHILD_LST1(identTkIdx_);
};

/* Declarators */
/**
 * \brief The IdentifierDeclaratorSyntax class.
 *
 * \remark 6.7.6-4
 */
class PSY_C_API IdentifierDeclaratorSyntax final : public DeclaratorSyntax
{
    AST_NODE_1K(IdentifierDeclarator, Declarator)

public:
    const SpecifierListSyntax* attributes() const { return attrs1_; }
    SyntaxToken identifierToken() const { return tokenAtIndex(identTkIdx_); }
    const SpecifierListSyntax* attributes_PostIdentifier() const { return attrs2_; }
    SyntaxToken equalsToken() const { return tokenAtIndex(equalsTkIdx_); }
    const InitializerSyntax* initializer() const { return init_; }

private:
    SpecifierListSyntax* attrs1_ = nullptr;
    LexedTokens::IndexType identTkIdx_ = LexedTokens::invalidIndex();
    SpecifierListSyntax* attrs2_ = nullptr;
    LexedTokens::IndexType equalsTkIdx_ = LexedTokens::invalidIndex();
    InitializerSyntax* init_ = nullptr;
    AST_CHILD_LST5(attrs1_,
                   identTkIdx_,
                   attrs2_,
                   equalsTkIdx_,
                   init_)
};

/**
 * The ParenthesizedDeclaratorSyntax class.
 *
 * \remark 6.7.6-6
 */
class PSY_C_API ParenthesizedDeclaratorSyntax final : public DeclaratorSyntax
{
    AST_NODE_1K(ParenthesizedDeclarator, Declarator);

public:
    SyntaxToken openParenthesisToken() const { return tokenAtIndex(openParenTkIdx_); }
    const DeclaratorSyntax* innerDeclarator() const { return innerDecltor_; }
    SyntaxToken closeParenthesisToken() const { return tokenAtIndex(closeParenTkIdx_); }

private:
    LexedTokens::IndexType openParenTkIdx_ = LexedTokens::invalidIndex();
    DeclaratorSyntax* innerDecltor_ = nullptr;
    LexedTokens::IndexType closeParenTkIdx_ = LexedTokens::invalidIndex();
    AST_CHILD_LST3(openParenTkIdx_, innerDecltor_, closeParenTkIdx_)
};

/**
 * The AbstractDeclaratorSyntax class.
 *
 * Represents the type name of the entity (an object or function)
 * referenced by a \a declarator.
 *
 * \remark 6.7.7-2
 */
class PSY_C_API AbstractDeclaratorSyntax final : public DeclaratorSyntax
{
    AST_NODE_1K(AbstractDeclarator, Declarator)

public:
    const SpecifierListSyntax* attributes() const { return attrs_; }

private:
    SpecifierListSyntax* attrs_ = nullptr;
    AST_CHILD_LST1(attrs_);
};

/**
 * \brief The PointerDeclaratorSyntax class.
 *
 * \remark 6.7.6.1
 */
class PSY_C_API PointerDeclaratorSyntax final : public DeclaratorSyntax
{
    AST_NODE_1K(PointerDeclarator, Declarator)

public:
    const SpecifierListSyntax* attributes() const { return attrs_; }
    SyntaxToken asteriskToken() const { return tokenAtIndex(asteriskTkIdx_); }
    const SpecifierListSyntax* qualifiersAndAttributes() const { return qualsAndAttrs_; }
    const DeclaratorSyntax* innerDeclarator() const { return innerDecltor_; }
    SyntaxToken equalsToken() const { return tokenAtIndex(equalsTkIdx_); }
    const InitializerSyntax* initializer() const { return init_; }

private:
    SpecifierListSyntax* attrs_ = nullptr;
    LexedTokens::IndexType asteriskTkIdx_ = LexedTokens::invalidIndex();
    SpecifierListSyntax* qualsAndAttrs_ = nullptr;
    DeclaratorSyntax* innerDecltor_ = nullptr;
    LexedTokens::IndexType equalsTkIdx_ = LexedTokens::invalidIndex();
    InitializerSyntax* init_ = nullptr;
    AST_CHILD_LST6(attrs_,
                   asteriskTkIdx_,
                   qualsAndAttrs_,
                   innerDecltor_,
                   equalsTkIdx_,
                   init_)
};

/**
 * \brief The ArrayOrFunctionDeclaratorSyntax class.
 *
 * The declarator of a function or of an array (object).
 *
 * \remark 6.7.6.2 and 6.7.6.3
 */
class PSY_C_API ArrayOrFunctionDeclaratorSyntax final : public DeclaratorSyntax
{
    AST_NODE_NK(ArrayOrFunctionDeclarator, Declarator)

public:
    const SpecifierListSyntax* attributes() const { return attrs1_; }
    const DeclaratorSyntax* innerDeclarator() const { return innerDecltor_; }
    const DeclaratorSuffixSyntax* suffix() const { return suffix_; }
    const SpecifierListSyntax* attributes_PostDeclarator() const { return attrs2_; }
    SyntaxToken equalsToken() const { return tokenAtIndex(equalsTkIdx_); }
    const InitializerSyntax* initializer() const { return init_; }

public:
    SpecifierListSyntax* attrs1_ = nullptr;
    DeclaratorSyntax* innerDecltor_ = nullptr;
    DeclaratorSuffixSyntax* suffix_ = nullptr;
    SpecifierListSyntax* attrs2_ = nullptr;
    LexedTokens::IndexType equalsTkIdx_ = LexedTokens::invalidIndex();
    InitializerSyntax* init_ = nullptr;
    AST_CHILD_LST6(attrs1_,
                   innerDecltor_,
                   suffix_,
                   attrs2_,
                   equalsTkIdx_,
                   init_)

    mutable Symbol* sym_;
};

/**
 * \brief The SubscriptSuffixSyntax class.
 *
 * The \a declarator suffix of an array \a declarator.
 *
 * \remark 6.7.6.2
 */
class PSY_C_API SubscriptSuffixSyntax final : public DeclaratorSuffixSyntax
{
    AST_NODE_1K(SubscriptSuffix, DeclaratorSuffix)

public:
    SyntaxToken openBracketToken() const { return tokenAtIndex(openBracketTkIdx_); }
    const SpecifierListSyntax* qualifiersAndAttributes() const { return qualsAndAttrs1_; }
    SyntaxToken staticKeyword() const { return tokenAtIndex(staticKwTkIdx_); }
    SpecifierListSyntax* qualifiersAndAttributes_PostStatic() const { return qualsAndAttrs2_; }
    const ExpressionSyntax* expression() const { return expr_; }
    SyntaxToken asteriskToken() const { return tokenAtIndex(asteriskTkIdx_); }
    SyntaxToken closeBracketToken() const { return tokenAtIndex(closeBracketTkIdx_); }

private:
    LexedTokens::IndexType openBracketTkIdx_ = LexedTokens::invalidIndex();
    SpecifierListSyntax* qualsAndAttrs1_ = nullptr;
    LexedTokens::IndexType staticKwTkIdx_ = LexedTokens::invalidIndex();
    SpecifierListSyntax* qualsAndAttrs2_ = nullptr;
    ExpressionSyntax* expr_ = nullptr;
    LexedTokens::IndexType asteriskTkIdx_ = LexedTokens::invalidIndex();
    LexedTokens::IndexType closeBracketTkIdx_ = LexedTokens::invalidIndex();
    AST_CHILD_LST7(openBracketTkIdx_,
                   qualsAndAttrs1_,
                   staticKwTkIdx_,
                   qualsAndAttrs2_,
                   expr_,
                   asteriskTkIdx_,
                   closeBracketTkIdx_)
};

/**
 * \brief The ParameterSuffixSyntax class.
 *
 * The \a declarator suffix of a function \a declarator.
 *
 * \remark 6.7.6.3
 */
class PSY_C_API ParameterSuffixSyntax final : public DeclaratorSuffixSyntax
{
    AST_NODE_1K(ParameterSuffix, DeclaratorSuffix)

public:
    SyntaxToken openParenthesisToken() const { return tokenAtIndex(openParenTkIdx_); }
    const ParameterDeclarationListSyntax* parameters() const { return decls_; }
    SyntaxToken ellipsisToken() const { return tokenAtIndex(ellipsisTkIdx_); }
    SyntaxToken closeParenthesisToken() const { return tokenAtIndex(closeParenTkIdx_); }

private:
    LexedTokens::IndexType openParenTkIdx_ = LexedTokens::invalidIndex();
    ParameterDeclarationListSyntax* decls_ = nullptr;
    LexedTokens::IndexType ellipsisTkIdx_ = LexedTokens::invalidIndex();
    LexedTokens::IndexType closeParenTkIdx_ = LexedTokens::invalidIndex();
    LexedTokens::IndexType psyOmitTkIdx_ = LexedTokens::invalidIndex();
    AST_CHILD_LST4(openParenTkIdx_,
                   decls_,
                   ellipsisTkIdx_,
                   closeParenTkIdx_)
};

/**
 * \brief The BitfieldDeclaratorSyntax class.
 *
 * \remark 6.7.2.1-9
 */
class PSY_C_API BitfieldDeclaratorSyntax final : public DeclaratorSyntax
{
    AST_NODE_1K(BitfieldDeclarator, Declarator)

public:
    const DeclaratorSyntax* innerDeclarator() const { return innerDecltor_; }
    SyntaxToken colonToken() const { return tokenAtIndex(colonTkIdx_); }
    const ExpressionSyntax* expression() const { return expr_; }
    const SpecifierListSyntax* attributes() const { return attrs_; }

private:
    DeclaratorSyntax* innerDecltor_ = nullptr;
    LexedTokens::IndexType colonTkIdx_ = LexedTokens::invalidIndex();
    ExpressionSyntax* expr_ = nullptr;
    SpecifierListSyntax* attrs_ = nullptr;
    AST_CHILD_LST4(innerDecltor_,
                   colonTkIdx_,
                   expr_,
                   expr_);
};

//--------------//
// Declarations //
//--------------//
/**
 *
 * \brief The TranslationUnitSyntax class.
 *
 * The \a translation-unit.
 *
 * \remark 6.9
 *
 * \note Similar to:
 * - \c clang::TranslationUnitDecl of LLVM/Clang.
 * - \c clang::syntax::TranslationUnit of Clang's Libtooling.
 * - \c Microsoft.CodeAnalysis.CSharp.Syntax.CompilationUnitSyntax of Roslyn.
 */
class PSY_C_API TranslationUnitSyntax final : public SyntaxNode
{
    AST_G_NODE_1K(TranslationUnit)

public:
    const DeclarationListSyntax* declarations() const { return decls_; }

private:
    DeclarationListSyntax* decls_ = nullptr;
    AST_CHILD_LST1(decls_)
};

/**
 * \brief The IncompleteDeclarationSyntax class.
 */
class PSY_C_API IncompleteDeclarationSyntax final : public DeclarationSyntax
{
    AST_NODE_1K(IncompleteDeclaration, Declaration)

public:
    const SpecifierListSyntax* specifiers() const { return specs_; }
    SyntaxToken semicolonToken() const { return tokenAtIndex(semicolonTkIdx_); }

private:
    const SpecifierListSyntax* specs_ = nullptr;
    LexedTokens::IndexType semicolonTkIdx_ = LexedTokens::invalidIndex();
    AST_CHILD_LST2(specs_, semicolonTkIdx_)
};

/**
 * \brief The NamedDeclarationSyntax class.
 *
 * A declaration that is identified by a name.
 *
 * \note Similar to:
 * - \c clang::NamedDecl of LLVM/Clang.
 * - \c clang::syntax::SimpleDeclaration of Clang's Libtooling.
 */
class PSY_C_API NamedDeclarationSyntax : public DeclarationSyntax
{
    AST_NODE(NamedDeclaration, Declaration)
};

/**
 * \brief The TypeDeclarationSyntax class.
 *
 * The base class of every \a declaration that declares a type.
 *
 * \remark 6.7.2.1
 *
 * \note Similar to:
 * - \c clang::TypeDecl of LLVM/Clang.
 * - \c Microsoft.CodeAnalysis.CSharp.Syntax.TypeDeclarationSyntax of Roslyn.
 */
class PSY_C_API TypeDeclarationSyntax : public NamedDeclarationSyntax
{
    AST_NODE(TypeDeclaration, NamedDeclaration)

    const TagTypeSpecifierSyntax* typeSpecifier() const { return typeSpec_; }
    SyntaxToken semicolonToken() const { return tokenAtIndex(semicolonTkIdx_); }

private:
    TagTypeSpecifierSyntax* typeSpec_ = nullptr;
    LexedTokens::IndexType semicolonTkIdx_ = LexedTokens::invalidIndex();
    AST_CHILD_LST2(typeSpec_, semicolonTkIdx_)
};

/**
 * \brief The TagDeclarationSyntax class.
 *
 * The base class of every \a declaration that declares a \a tag,
 * such that of an \c struct, \c union, or \c enum.
 *
 * \remark 6.7-2, 6.7.2.1, 6.7.2.2, and 6.7.2.3
 *
 * \note Similar to:
 * - \c clang::TagDecl of LLVM/Clang.
 */
class PSY_C_API TagDeclarationSyntax : public TypeDeclarationSyntax
{
    AST_NODE(TagDeclaration, TypeDeclaration)
};

/**
 * \brief The StructOrUnionDeclarationSyntax class.
 *
 * The \a declaration of a \c struct or \c union.
 *
 * \remark 6.7.2.1
 *
 * \note Similar to:
 * - \c clang::RecordDecl of LLVM/Clang.
 * - \c Microsoft.CodeAnalysis.CSharp.StructDeclarationSyntax and
 * - \c Microsoft.CodeAnalysis.CSharp.ClassDeclarationSyntax of Roslyn.
 */
class PSY_C_API StructOrUnionDeclarationSyntax final : public TagDeclarationSyntax
{
    AST_NODE_NK(StructOrUnionDeclaration, TagDeclaration)
};

/**
 * The EnumDeclarationSyntax class.
 *
 * \remark 6.7.2.2
 *
 * \note Similar to:
 * - \c clang::EnumDecl of LLVM/Clang.
 * - \c Microsoft.CodeAnalysis.CSharp.Syntax.EnumDeclarationSyntax of Roslyn.
 */
class PSY_C_API EnumDeclarationSyntax final : public TagDeclarationSyntax
{
    AST_NODE_NK(EnumDeclaration, TagDeclaration)
};

/**
 * \brief The TypeDeclarationAsSpecifierSyntax class.
 *
 * A \a type-specifier that consists of a \a declaration. Consider the
 * snippet below:
 *
 * \code
 * struct x { int y; } z;
 * \endcode
 *
 * Here, \c z is an object \a declarator whose \a type-specifier
 * is <tt>struct x { int y; }<\tt>, which, in turn, is also the
 * \a declaration of said type (6.7.2.1-8). To account for this
 * situation, while preserving the AST consistent and rewritable,
 * the \a declaration is embedded as a \a type-specifier.
 *
 * \remark 6.7.2.1-8
 */
class PSY_C_API TypeDeclarationAsSpecifierSyntax final : public SpecifierSyntax
{
    AST_NODE_1K(TypeDeclarationAsSpecifier, Specifier)

public:
    const TypeDeclarationSyntax* typeDeclaration() const { return typeDecl_; }

private:
    TypeDeclarationSyntax* typeDecl_ = nullptr;
    AST_CHILD_LST1(typeDecl_);
};


/**
 * \brief The ValueDeclarationSyntax class.
 *
 * The base class of a \a declaration whose declared entity is a value.
 *
 * \note Similar to:
 * - \c clang::ValueDecl of LLVM/Clang.
 */
class PSY_C_API ValueDeclarationSyntax : public DeclarationSyntax
{
    AST_NODE(ValueDeclaration, Declaration)
};

/**
 * \brief The EnumeratorDeclarationSyntax class.
 *
 * \remark 6.7.2.2-3
 *
 * \note Similar to:
 * - \c clang::EnumConstantDecl of LLVM/Clang.
 * - \c Microsoft.CodeAnalysis.CSharp.Syntax.EnumeratorDeclarationSyntax of Roslyn.
 */
class PSY_C_API EnumeratorDeclarationSyntax final : public ValueDeclarationSyntax
{
    AST_NODE_1K(EnumeratorDeclaration, ValueDeclaration)

public:
    SyntaxToken identifierToken() const { return tokenAtIndex(identTkIdx_); }
    const SpecifierListSyntax* attributes() const { return attrs_; }
    SyntaxToken equalsToken() const { return tokenAtIndex(equalsTkIdx_); }
    const ExpressionSyntax* expression() const { return expr_; }
    SyntaxToken commaToken() const { return tokenAtIndex(commaTkIdx_); }

private:
    LexedTokens::IndexType identTkIdx_ = LexedTokens::invalidIndex();
    SpecifierListSyntax* attrs_ = nullptr;
    LexedTokens::IndexType equalsTkIdx_ = LexedTokens::invalidIndex();
    ExpressionSyntax* expr_ = nullptr;
    LexedTokens::IndexType commaTkIdx_ = LexedTokens::invalidIndex();
    AST_CHILD_LST5(identTkIdx_,
                   attrs_,
                   equalsTkIdx_,
                   expr_,
                   commaTkIdx_)
};

/**
 * \brief The DeclaratorDeclarationSyntax class.
 *
 * The base class of a \a declaration that declares a \a declarator.
 *
 * \remark 6.7-2
 *
 * \note Similar to:
 * - \c clang::DeclaratorDecl of LLVM/Clang.
 * - \c Microsoft.CodeAnalysis.CSharp.Syntax.VariableDeclaratorSyntax of Roslyn.
 */
class PSY_C_API DeclaratorDeclarationSyntax : public ValueDeclarationSyntax
{
    AST_NODE(DeclaratorDeclaration, ValueDeclaration)
};

/**
 * \brief The VariableAndOrFunctionDeclarationSyntax class.
 *
 * A \a declaration that declares one ore more of the following:
 *   - an object (possibly an array), or
 *   - a \b function.
 *
 * It isn't distinguished, by means of a further inherited type, the exact
 * variety of the \a declaration, i.e., there are no \c "VariableDeclarationSyntax"
 * or \c "FunctionDeclarationSyntax" class. Yet, the \a declarator within
 * this \a declaration is refined enough: identifying either an object or
 * a function. The reason for this design is explained through the snippet below:
 *
 * \code
 * double x, y();
 * \endcode
 *
 * According to the grammar of C, there's a single \a declaration in the above
 * snippet, one that starts with \c double and ends with the semicolon \c ;.
 * This \a declaration declares a variable named \c x, and a function named
 * \c y. Specifically, the type specifier \c double is shared both declarators
 * (in the latter case, it states the return type of \c y). Therefore, by
 * providing only a "variable and/or function" syntax, we keep the AST consistent
 * and rewritable, without introducing artificial artifacts.
 *
 * \remark 6.7.6-2
 *
 * \note Similar to:
 * - \c clang::VarDecl and \c clang::FunctionDecl of LLVM/Clang.
 * - \c Microsoft.CodeAnalysis.CSharp.Syntax.VariableDeclarationSyntax and
 *   \c Microsoft.CodeAnalysis.CSharp.Syntax.BaseMethodDeclarationSyntax of Roslyn.
 */
class PSY_C_API VariableAndOrFunctionDeclarationSyntax : public DeclaratorDeclarationSyntax
{
    AST_NODE_1K(VariableAndOrFunctionDeclaration, DeclaratorDeclaration)

public:
    const SpecifierListSyntax* specifiers() const { return specs_; }
    const DeclaratorListSyntax* declarators() const { return decltors_; }
    SyntaxToken semicolonToken() const { return tokenAtIndex(semicolonTkIdx_); }

private:
    SpecifierListSyntax* specs_ = nullptr;
    DeclaratorListSyntax* decltors_ = nullptr;
    LexedTokens::IndexType semicolonTkIdx_ = LexedTokens::invalidIndex();
    AST_CHILD_LST3(specs_, decltors_, semicolonTkIdx_)

    mutable SymbolList<Symbol*>* syms_ = nullptr;
};

/**
 * \brief The FieldDeclarationSyntax class.
 *
 * A \a declaration that declares an \b object (possibly an array)
 * that is a member of an \c struct or \c union.
 *
 * \remark 6.2.5-20, 6.7-2, and 6.7.2.1-6
 *
 * \note Similar to:
 * - \c clang::FieldDecl of LLVM/Clang.
 */
class PSY_C_API FieldDeclarationSyntax final : public DeclaratorDeclarationSyntax
{
    AST_NODE_1K(FieldDeclaration, DeclaratorDeclaration)

public:
    const SpecifierListSyntax* specifiers() const { return specs_; }
    const DeclaratorListSyntax* declarators() const { return decltors_; }
    SyntaxToken semicolonToken() const { return tokenAtIndex(semicolonTkIdx_); }

private:
    SpecifierListSyntax* specs_ = nullptr;
    DeclaratorListSyntax* decltors_ = nullptr;
    LexedTokens::IndexType semicolonTkIdx_ = LexedTokens::invalidIndex();
    AST_CHILD_LST3(specs_,
                   decltors_,
                   semicolonTkIdx_)

    mutable SymbolList<Symbol*>* syms_ = nullptr;
};

/**
 * \brief The ParameterDeclarationSyntax class.
 *
 * \remark 6.7.6 and 6.7.6.3
 *
 * \note Similar to:
 * - \c clang::ParmVarDecl of LLVM/Clang.
 * - \c Microsoft.CodeAnalysis.CSharp.Syntax.ParameterSyntax of Roslyn.
 */
class PSY_C_API ParameterDeclarationSyntax final : public DeclaratorDeclarationSyntax
{
    AST_NODE_1K(ParameterDeclaration, DeclaratorDeclaration)

public:
    const SpecifierListSyntax* specifiers() const { return specs_; }
    const DeclaratorSyntax* declarator() const { return decltor_; }

private:
    SpecifierListSyntax* specs_ = nullptr;
    DeclaratorSyntax* decltor_ = nullptr;
    AST_CHILD_LST2(specs_, decltor_)

    mutable ParameterSymbol* sym_ = nullptr;
};

/**
 * \brief The StaticAssertDeclarationSyntax class
 *
 * \remark 6.7.10
 */
class PSY_C_API StaticAssertDeclarationSyntax final : public DeclarationSyntax
{
    AST_NODE_1K(StaticAssertDeclaration, Declaration)

public:
    SyntaxToken staticAssertKeyword() const { return tokenAtIndex(staticAssertKwTkIdx_); }
    SyntaxToken openParenthesisToken() const { return tokenAtIndex(openParenTkIdx_); }
    const ExpressionSyntax* expression() const { return expr_; }
    SyntaxToken commaToken() const { return tokenAtIndex(commaTkIdx_); }
    const ExpressionSyntax* stringLiteral() const { return strLit_; }
    SyntaxToken closeParenthesisToken() const { return tokenAtIndex(closeParenTkIdx_); }
    SyntaxToken semicolonToken() const { return tokenAtIndex(semicolonTkIdx_); }

private:
    LexedTokens::IndexType staticAssertKwTkIdx_ = LexedTokens::invalidIndex();
    LexedTokens::IndexType openParenTkIdx_ = LexedTokens::invalidIndex();
    ExpressionSyntax* expr_ = nullptr;
    LexedTokens::IndexType commaTkIdx_ = LexedTokens::invalidIndex();
    ExpressionSyntax* strLit_ = nullptr;
    LexedTokens::IndexType closeParenTkIdx_ = LexedTokens::invalidIndex();
    LexedTokens::IndexType semicolonTkIdx_ = LexedTokens::invalidIndex();
    AST_CHILD_LST7(staticAssertKwTkIdx_,
                   openParenTkIdx_,
                   expr_,
                   commaTkIdx_,
                   strLit_,
                   closeParenTkIdx_,
                   semicolonTkIdx_);
};

/**
 * \brief The FunctionDefinitionSyntax class.
 *
 * The \a declaration and \a definition of a function.
 *
 * \remark 6.9.1
 */
class PSY_C_API FunctionDefinitionSyntax final : public DeclarationSyntax
{
    AST_NODE_1K(FunctionDefinition, Declaration)

public:
    const SpecifierListSyntax* specifiers() const { return specs_; }
    const DeclaratorSyntax* declarator() const { return decltor_; }
    const ExtKR_ParameterDeclarationListSyntax* extKR_params() const { return extKR_params_; }
    const StatementSyntax* body() const { return body_; }

private:
    SpecifierListSyntax* specs_ = nullptr;
    DeclaratorSyntax* decltor_ = nullptr;
    ExtKR_ParameterDeclarationListSyntax* extKR_params_ = nullptr;
    StatementSyntax* body_ = nullptr;
    AST_CHILD_LST4(specs_, decltor_, extKR_params_, body_);

    mutable FunctionSymbol* sym_;
};

/**
 * \brief The ExtGNU_AsmStatementDeclarationSyntax class.
 *
 * \remark J.5.10
 *
 * \see ExtGNU_AsmStatementSyntax
 *
 * \attention Thisis a GNU extension:
 * https://gcc.gnu.org/onlinedocs/gcc/Basic-Asm.html#Basic-Asm
 *
 * \note Similar to:
 * - \c clang::FileScopeAsmDecl of LLVM/Clang.
 */
class PSY_C_API ExtGNU_AsmStatementDeclarationSyntax final : public DeclarationSyntax
{
    AST_NODE_1K(ExtGNU_AsmStatementDeclaration, Declaration)

public:
    SyntaxToken asmKeyword() const { return tokenAtIndex(asmTkIdx_); }
    SyntaxToken openParenthesisToken() const { return tokenAtIndex(openParenTkIdx_); }
    const SyntaxNode* stringLiteral() const { return strLit_; }
    SyntaxToken closeParenthesisToken() const { return tokenAtIndex(closeParenTkIdx_); }

private:
    LexedTokens::IndexType asmTkIdx_ = LexedTokens::invalidIndex();
    LexedTokens::IndexType openParenTkIdx_ = LexedTokens::invalidIndex();
    ExpressionSyntax* strLit_ = nullptr;
    LexedTokens::IndexType closeParenTkIdx_ = LexedTokens::invalidIndex();
    AST_CHILD_LST4(asmTkIdx_, openParenTkIdx_, strLit_, closeParenTkIdx_);
};

/**
 * The ExtPSY_TemplateDeclarationSyntax class
 *
 * A Psyche-C \c _Template extension \a declaration.
 */
class PSY_C_API ExtPSY_TemplateDeclarationSyntax final : public DeclarationSyntax
{
    AST_NODE_1K(ExtPSY_TemplateDeclaration, Declaration)

public:
    SyntaxToken templateToken() const { return tokenAtIndex(templateTkIdx_); }
    const DeclarationSyntax* declaration() const { return decl_; }

private:
    LexedTokens::IndexType templateTkIdx_ = LexedTokens::invalidIndex();
    DeclarationSyntax* decl_ = nullptr;
    AST_CHILD_LST2(templateTkIdx_, decl_)
};

/**
 * \brief The ExtKR_ParameterDeclarationSyntax class.
 *
 * Parameter declaration in
 * <a href="https://jameshfisher.com/2016/11/27/c-k-and-r/">K&R style</a>.
 *
 * \code{.c}
 * int foo(x, y)
 *     int x;
 *     float y;
 * {
 *     return x + y;
 * }
 * \endcode
 */
class PSY_C_API ExtKR_ParameterDeclarationSyntax final : public DeclaratorDeclarationSyntax
{
    AST_NODE_1K(ExtKR_ParameterDeclaration, DeclaratorDeclaration)

public:
    const SpecifierListSyntax* specifiers() const { return specs_; }
    const DeclaratorListSyntax* declarators() const { return decltors_; }
    SyntaxToken semicolonToken() const { return tokenAtIndex(semicolonTkIdx_); }

private:
    SpecifierListSyntax* specs_ = nullptr;
    DeclaratorListSyntax* decltors_ = nullptr;
    LexedTokens::IndexType semicolonTkIdx_ = LexedTokens::invalidIndex();
    AST_CHILD_LST3(specs_, decltors_, semicolonTkIdx_)
};

/* Initializers */
class PSY_C_API ExpressionInitializerSyntax final : public InitializerSyntax
{
    AST_NODE_1K(ExpressionInitializer, Initializer)

public:
    const ExpressionSyntax* expression() const { return expr_; }

private:
    ExpressionSyntax* expr_ = nullptr;
    AST_CHILD_LST1(expr_);
};

/**
 * \brief The BraceEnclosedInitializerSyntax class.
 *
 * \code
 * x = { 1, 2 };
 * \endcode
 *
 * \remark 6.7.9
 *
 * \note Similar to:
 * - \c clang::InitListExpr of LLMV/Clang.
 */
class BraceEnclosedInitializerSyntax final : public InitializerSyntax
{
    AST_NODE_1K(BraceEnclosedInitializer, Initializer)

public:
    SyntaxToken openBraceToken() const { return tokenAtIndex(openBraceTkIdx_); }
    const InitializerListSyntax* initializerList() const { return initList_; }
    SyntaxToken closeBraceToken() const { return tokenAtIndex(closeBraceTkIdx_); }

private:
    LexedTokens::IndexType openBraceTkIdx_ = LexedTokens::invalidIndex();
    InitializerListSyntax* initList_ = nullptr;
    LexedTokens::IndexType closeBraceTkIdx_ = LexedTokens::invalidIndex();
    AST_CHILD_LST3(openBraceTkIdx_, initList_, closeBraceTkIdx_)
};

/**
 * \brief The DesignatedInitializerSyntax class.
 *
 * \code
 * x y = { .z = 1 };
 * x y = { [0].z = 1 };
 * \endcode
 *
 * \remark 6.7.9
 *
 * \note Similar to:
 * - \c clang::DesignatedInitExpr of LLMV/Clang.
 * - \c Microsoft.CodeAnalysis.CSharp.Syntax.InitializerExpressionSyntax of Roslyn.
 */
class DesignatedInitializerSyntax final : public InitializerSyntax
{
    AST_NODE_1K(DesignatedInitializer, Initializer)

public:
    const DesignatorListSyntax* designators() const { return desigs_; }
    SyntaxToken equalsToken() const { return tokenAtIndex(equalsTkIdx_); }
    const InitializerSyntax* initializer() const { return init_; }

private:
    DesignatorListSyntax* desigs_ = nullptr;
    LexedTokens::IndexType equalsTkIdx_ = LexedTokens::invalidIndex();
    InitializerSyntax* init_ = nullptr;
    AST_CHILD_LST3(desigs_,
                   equalsTkIdx_,
                   init_);
};

/**
 * \brief The DesignatorSyntax class.
 *
 * \remark 6.7.9
 *
 * \note Similar to:
 * - \c clang::DesignatedInitExpr::Designator of LLMV/Clang.
 */
class DesignatorSyntax : public SyntaxNode
{
    AST_G_NODE(Designator)
};

/**
 * \brief The FieldDesignatorSyntax class.
 * \code
 * .z = 1
 * \endcode
 *
 * \remark 6.7.9
 *
 * \note Similar to:
 * - \c clang::DesignatedInitExpr::FieldDesignator of LLMV/Clang.
 */
class FieldDesignatorSyntax final : public DesignatorSyntax
{
    AST_NODE_1K(FieldDesignator, Designator)

public:
    SyntaxToken dotToken() const { return tokenAtIndex(dotTkIdx_); }
    SyntaxToken identifierToken() const { return tokenAtIndex(identTkIdx_); }

private:
    LexedTokens::IndexType dotTkIdx_ = LexedTokens::invalidIndex();
    LexedTokens::IndexType identTkIdx_ = LexedTokens::invalidIndex();
    AST_CHILD_LST2(dotTkIdx_,
                   identTkIdx_);
};

/**
 * \brief The ArrayDesignatorSyntax class.
 *
 * \code
 * [0].z = 1
 * \endcode
 *
 * \remark 6.7.9
 *
 * \note Similar to:
 * - \c clang::DesignatedInitExpr::ArrayOrRangeDesignator of LLMV/Clang.
 */
class ArrayDesignatorSyntax final : public DesignatorSyntax
{
    AST_NODE_1K(ArrayDesignator, Designator)

public:
    SyntaxToken openBracketToken() const { return tokenAtIndex(openBracketTkIdx_); }
    const ExpressionSyntax* expression() const { return expr_; }
    SyntaxToken closeBracketToken() const { return tokenAtIndex(closeBracketTkIdx_); }

public:
    LexedTokens::IndexType openBracketTkIdx_ = LexedTokens::invalidIndex();
    ExpressionSyntax* expr_ = nullptr;
    LexedTokens::IndexType closeBracketTkIdx_ = LexedTokens::invalidIndex();
    AST_CHILD_LST3(openBracketTkIdx_,
                   expr_,
                   closeBracketTkIdx_);
};

/**
 * \brief The OffsetOfDesignatorSyntax class.
 *
 */
class OffsetOfDesignatorSyntax final : public DesignatorSyntax
{
    AST_NODE_1K(OffsetOfDesignator, Designator)

public:
    SyntaxToken identifierToken() const { return tokenAtIndex(identTkIdx_); }
    const DesignatorListSyntax* designators() const { return desigs_; }

private:
    LexedTokens::IndexType identTkIdx_ = LexedTokens::invalidIndex();
    DesignatorListSyntax* desigs_ = nullptr;

    AST_CHILD_LST2(identTkIdx_, desigs_)
};


} // C
} // psy

#endif
