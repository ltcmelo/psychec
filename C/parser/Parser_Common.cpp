// Copyright (c) 2016/17/18/19/20/21/22 Leandro T. C. Melo <ltcmelo@gmail.com>
// Copyright (c) 2008 Roberto Raggi <roberto.raggi@gmail.com>
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

#include "Parser__IMPL__.inc"

using namespace psy;
using namespace C;

//--------//
// Common //
//--------//

/**
 * Parse a \a type-name.
 *
 \verbatim
 type-name:
     specifier-qualifier-list abstract-decltor_opt
 \endverbatim
 *
 * \remark 6.7.7.
 */
bool Parser::parseTypeName(TypeNameSyntax*& typeName)
{
    DEBUG_THIS_RULE();

    DeclarationSyntax* decl = nullptr;
    SpecifierListSyntax* specList = nullptr;
    if (!parseSpecifierQualifierList(decl, specList))
        return false;

    typeName = makeNode<TypeNameSyntax>();
    typeName->specs_ = specList;
    return parseAbstractDeclarator(typeName->decltor_);
}

bool Parser::parseParenthesizedTypeNameOrExpression(TypeReferenceSyntax*& tyRef)
{
    switch (peek().kind()) {
        case OpenParenToken: {
            Backtracker BT(this);
            ExpressionSyntax* expr = nullptr;
            if (parseExpressionWithPrecedenceUnary(expr)) {
                auto exprAsTyRef = makeNode<ExpressionAsTypeReferenceSyntax>();
                tyRef = exprAsTyRef;
                exprAsTyRef->expr_ = expr;
                if (expr->kind() == ParenthesizedExpression)
                    maybeAmbiguateTypeReference(tyRef);
                return true;
            }

            BT.backtrack();
            auto openParenTkIdx = consume();
            TypeNameSyntax* typeName = nullptr;
            if (!parseTypeName(typeName))
                return false;

            auto nameAsTyRef = makeNode<TypeNameAsTypeReferenceSyntax>();
            tyRef = nameAsTyRef;
            nameAsTyRef->openParenTkIdx_ = openParenTkIdx;
            nameAsTyRef->typeName_ = typeName;
            return matchOrSkipTo(CloseParenToken, &nameAsTyRef->closeParenTkIdx_);
        }

        default: {
            ExpressionSyntax* unaryExpr = nullptr;
            if (!parseExpressionWithPrecedenceUnary(unaryExpr))
                return false;
            auto exprAsTyRef = makeNode<ExpressionAsTypeReferenceSyntax>();
            tyRef = exprAsTyRef;
            exprAsTyRef->expr_ = unaryExpr;
            return true;
        }
    }
}

void Parser::maybeAmbiguateTypeReference(TypeReferenceSyntax*& tyRef)
{
    PSY_ASSERT_W_MSG(tyRef->kind() == ExpressionAsTypeReference
                    && (tyRef->asExpressionAsTypeReference()->expr_->kind()
                            == ParenthesizedExpression),
                  return, "");

    auto exprAsTyRef = tyRef->asExpressionAsTypeReference();
    auto parenExpr = exprAsTyRef->expr_->asParenthesizedExpression();
    if (!parenExpr->expr_->asIdentifierName())
        return;

    auto tyDefName = makeNode<TypedefNameSyntax>();
    tyDefName->identTkIdx_ = parenExpr->expr_->asIdentifierName()->identTkIdx_;
    auto tyName = makeNode<TypeNameSyntax>();
    tyName->specs_ =  makeNode<SpecifierListSyntax>(tyDefName);
    auto tyNameAsTyRef = makeNode<TypeNameAsTypeReferenceSyntax>();
    tyNameAsTyRef->openParenTkIdx_ = parenExpr->openParenTkIdx_;
    tyNameAsTyRef->typeName_ = tyName;
    tyNameAsTyRef->closeParenTkIdx_ = parenExpr->closeParenTkIdx_;

    auto ambiTyRef = makeNode<AmbiguousTypeNameOrExpressionAsTypeReferenceSyntax>();
    tyRef = ambiTyRef;
    ambiTyRef->exprAsTyRef_ = exprAsTyRef;
    ambiTyRef->tyNameAsTyRef_ = tyNameAsTyRef;

    diagReporter_.AmbiguousTypeNameOrExpressionAsTypeReference();
}
