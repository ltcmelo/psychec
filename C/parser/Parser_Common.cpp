// Copyright (c) 2016/17/18/19/20/21 Leandro T. C. Melo <ltcmelo@gmail.com>
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
    if (!parseSpecifierQualifierList(decl, specList, false))
        return false;

    typeName = makeNode<TypeNameSyntax>();
    typeName->specs_ = specList;
    return parseAbstractDeclarator(typeName->decltor_);
}

bool Parser::parseParenthesizedTypeNameOrExpression(TypeReferenceSyntax*& typeRef)
{
    switch (peek().kind()) {
        case OpenParenToken: {
            Backtracker BT(this);
            ExpressionSyntax* expr = nullptr;
            if (parseExpression(expr)) {
                auto exprAsTyRef = makeNode<ExpressionAsTypeReferenceSyntax>();
                typeRef = exprAsTyRef;
                exprAsTyRef->expr_ = expr;
                if (expr->kind() == ParenthesizedExpression)
                    maybeAmbiguateTypeReference(typeRef);
                return true;
            }

            BT.backtrack();
            auto openParenTkIdx = consume();
            TypeNameSyntax* typeName = nullptr;
            if (!parseTypeName(typeName))
                return false;

            auto nameAsTyRef = makeNode<TypeNameAsTypeReferenceSyntax>();
            typeRef = nameAsTyRef;
            nameAsTyRef->openParenTkIdx_ = openParenTkIdx;
            nameAsTyRef->typeName_ = typeName;
            return matchOrSkipTo(CloseParenToken, &nameAsTyRef->closeParenTkIdx_);
        }

        default: {
            ExpressionSyntax* unaryExpr = nullptr;
            if (!parseExpressionWithPrecedenceUnary(unaryExpr))
                return false;
            auto exprAsTyRef = makeNode<ExpressionAsTypeReferenceSyntax>();
            typeRef = exprAsTyRef;
            exprAsTyRef->expr_ = unaryExpr;
            return true;
        }
    }


//    if (!match(OpenParenToken, &typeRef->openParenTkIdx_))
//        return false;

//    switch (peek().kind()) {
//        case EllipsisToken:
//            consume();
//            break;

//        case IdentifierToken: {
//            if (peek(2).kind() == CloseParenToken) {
//                auto ambiIdent = makeNode<AmbiguousTypedefNameOrIdentifierExpressionSyntax>();
//                typeRef->arg_ = ambiIdent;
//                Backtracker BT(this);
//                parseTypedefName_AtFirst(ambiIdent->typedefName_);
//                BT.backtrack();
//                parseIdentifierExpression_AtFirst(ambiIdent->identExpr_);
//                typeRef->closeParenTkIdx_ = consume();
//                return true;
//            }
//            [[fallthrough]];
//        }

//        default: {
//            Backtracker BT(this);
//            ExpressionSyntax* expr = nullptr;
//            if (parseExpression(expr)) {
//                typeRef->arg_ = expr;
//            }
//            else {
//                BT.backtrack();
//                TypeNameSyntax* typeName = nullptr;
//                if (!parseTypeName(typeName))
//                    return false;
//                typeRef->arg_ = typeName;
//            }
//            break;
//        }
//    }

//    return matchOrSkipTo(CloseParenToken, &typeRef->closeParenTkIdx_);
}

void Parser::maybeAmbiguateTypeReference(TypeReferenceSyntax*& typeRef)
{
    /*
    PSYCHE_ASSERT(typeRef->kind() == ExpressionAsTypeReference
                        && typeRef->arg_->kind() == ParenthesizedExpression,
                  return, "");

    auto exprAsRef = typeRef->arg_->asParenthesizedExpression();
    if (!exprAsRef->expr_->asIdentifierExpression())
        return;

    auto tyDefName = makeNode<TypedefNameSyntax>();
    tyDefName->identTkIdx_ = exprAsRef->expr_->asIdentifierExpression()->identTkIdx_;
    auto arg = makeNode<TypeNameSyntax>();
    arg->specs_ =  makeNode<SpecifierListSyntax>(tyDefName);
    auto tyNameAsRef = makeNode<TypeReferenceSyntax>(TypeNameAsTypeReference);
    tyNameAsRef->arg_ = arg;

    auto ambiTyRef = makeNode<AmbiguousTypedefNameOrIdentifierExpressionSyntax>();
    typeRef = ambiTyRef;
    ambiTyRef->identExpr_ = exprAsRef;
    ambiTyRef->typedefName_ = tyNameAsRef;
    */
}












































