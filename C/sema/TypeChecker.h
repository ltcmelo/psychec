// Copyright (c) 2024-2025 Leandro T. C. Melo <ltcmelo@gmail.com>
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

#ifndef PSYCHE_C_TYPE_CHECKER_H__
#define PSYCHE_C_TYPE_CHECKER_H__

#include "API.h"

#include "syntax/Lexeme_StringLiteral.h"
#include "syntax/SyntaxVisitor.h"
#include "types/TypeKind_Basic.h"
#include "../common/diagnostics/DiagnosticDescriptor.h"
#include "../common/infra/AccessSpecifiers.h"

#include <memory>

namespace psy {
namespace C {

class PSY_C_INTERNAL_API TypeChecker final : protected SyntaxVisitor
{
public:
    ~TypeChecker();

PSY_INTERNAL:
    PSY_GRANT_INTERNAL_ACCESS(Compilation);
    PSY_GRANT_INTERNAL_ACCESS(TypeCheckerTester);

    TypeChecker(SemanticModel* semaModel, const SyntaxTree* tree);
    TypeChecker(const TypeChecker&) = delete;
    void operator=(const TypeChecker&) = delete;

    void checkTypes();

private:
    SemanticModel* semaModel_;
    const Type* ty_;
    const Type* ptrdiffTy_;
    const Type* sizeTy_;
    const Type* maxAlignTy_;
    const Type* wcharTy_;
    const Type* char16Ty_;
    const Type* char32Ty_;
    const Type* strLitTy_;
    const Type* u8StrLitTy_;
    const Type* uStrLitTy_;
    const Type* UStrLitTy_;
    const Type* LStrLitTy_;

    struct DiagnosticsReporter
    {
        DiagnosticsReporter(TypeChecker* tyChecker)
            : tyChecker_(tyChecker)
        {}
        TypeChecker* tyChecker_;

        void diagnose(DiagnosticDescriptor&& desc, SyntaxToken tk);

        void InvalidOperator(SyntaxToken optrTk);
        void ExpectedExpressionOfType_CORE(
                SyntaxToken exprTk,
                const std::string& diagnosticID,
                const std::string& category);
        void ExpectedExpressionOfArithmeticType(SyntaxToken exprTk);
        void ExpectedExpressionOfIntegerType(SyntaxToken exprTk);
        void ExpectedExpressionOfRealType(SyntaxToken exprTk);
        void ExpectedExpressionOfPointerType(SyntaxToken exprTk);
        void ExpectedExpressionOfScalarType(SyntaxToken exprTk);
        void ExpectedExpressionOfStructOrUnionType(SyntaxToken exprTk);
        void ExpectedExpressionOfPointerToStructOrUnionType(SyntaxToken exprTk);
        void ExpectedExpressionOfPointerOrArrayType(SyntaxToken exprTk);
        void ExpectedExpressionOfFunctionOrFunctionPointerType(SyntaxToken tk);
        void ExpectedExpression(SyntaxToken tk);
        void UnknownMemberOfTag(SyntaxToken tk);
        void IncompatibleTypesInAssignment(SyntaxToken tk);
        void CannotAssignToExpressionOfConstQualifiedType(SyntaxToken tk);
        void CannotAssignToExpressionOfArrayType(SyntaxToken tk);
        void ConversionBetweenIntegerAndPointerTypesInAssignment(SyntaxToken tk);
        void TooFewArgumentsToFunctionCall(SyntaxToken tk);

        static const std::string ID_of_InvalidOperator;
        static const std::string ID_of_ExpectedExpressionOfArithmeticType;
        static const std::string ID_of_ExpectedExpressionOfIntegerType;
        static const std::string ID_of_ExpectedExpressionOfRealType;
        static const std::string ID_of_ExpectedExpressionOfPointerType;
        static const std::string ID_of_ExpectedExpressionOfScalarType;
        static const std::string ID_of_ExpectedExpressionOfStructOrUnionType;
        static const std::string ID_of_ExpectedExpressionOfPointerToStructOrUnionType;
        static const std::string ID_of_ExpectedExpressionOfPointerOrArrayType;
        static const std::string ID_of_ExpectedExpressionOfFunctionOrFunctionPointerType;
        static const std::string ID_of_ExpectedExpression;
        static const std::string ID_of_UnknownMemberOfTag;
        static const std::string ID_of_IncompatibleTypesInAssignment;
        static const std::string ID_of_CannotAssignToExpressionOfConstQualifiedType;
        static const std::string ID_of_CannotAssignToExpressionOfArrayType;
        static const std::string ID_of_ConversionBetweenIntegerAndPointerTypesInAssignment;
        static const std::string ID_of_TooFewArgumentsToFunctionCall;
    };
    DiagnosticsReporter diagReporter_;

    //--------------//
    // Declarations //
    //--------------//

    /* Specifiers */
    virtual Action visitExtGNU_Attribute(const ExtGNU_AttributeSyntax*) override;

    //-------------//
    // Expressions //
    //-------------//

    Action visitExpression(const SyntaxNode* node);

    virtual Action visitIdentifierName(const IdentifierNameSyntax*) override;
    virtual Action visitPredefinedName(const PredefinedNameSyntax*) override;
    virtual Action visitConstantExpression(const ConstantExpressionSyntax*) override;
    virtual Action visitStringLiteralExpression(const StringLiteralExpressionSyntax*) override;
    virtual Action visitParenthesizedExpression(const ParenthesizedExpressionSyntax*) override;
    virtual Action visitGenericSelectionExpression(const GenericSelectionExpressionSyntax*) override;
    virtual Action visitGenericAssociation(const GenericAssociationSyntax*) override;
    virtual Action visitExtGNU_EnclosedCompoundStatementExpression(const ExtGNU_EnclosedCompoundStatementExpressionSyntax*) override;
    virtual Action visitExtGNU_ComplexValuedExpression(const ExtGNU_ComplexValuedExpressionSyntax*) override;

    /* Operations */
    virtual Action visitPrefixUnaryExpression(const PrefixUnaryExpressionSyntax*) override;
    virtual Action visitPostfixUnaryExpression(const PostfixUnaryExpressionSyntax*) override;
    virtual Action visitMemberAccessExpression(const MemberAccessExpressionSyntax*) override;
    virtual Action visitArraySubscriptExpression(const ArraySubscriptExpressionSyntax*) override;
    virtual Action visitTypeTraitExpression(const TypeTraitExpressionSyntax*) override;
    virtual Action visitCastExpression(const CastExpressionSyntax*) override;
    virtual Action visitCallExpression(const CallExpressionSyntax*) override;
    virtual Action visitVAArgumentExpression(const VAArgumentExpressionSyntax*) override;
    virtual Action visitOffsetOfExpression(const OffsetOfExpressionSyntax*) override;
    virtual Action visitCompoundLiteralExpression(const CompoundLiteralExpressionSyntax*) override;
    virtual Action visitBinaryExpression(const BinaryExpressionSyntax*) override;
    virtual Action visitConditionalExpression(const ConditionalExpressionSyntax*) override;
    virtual Action visitAssignmentExpression(const AssignmentExpressionSyntax*) override;
    virtual Action visitSequencingExpression(const SequencingExpressionSyntax*) override;
    virtual Action visitExtGNU_ChooseExpression(const ExtGNU_ChooseExpressionSyntax*) override;

    /* Binary-like expressions */
    template <class BinaryLikeExprNodeT>
    Action visitBinaryExpression_MultiplicationOrDivision(
            const BinaryLikeExprNodeT* node,
            const Type* leftTy,
            const Type* rightTy);
    template <class BinaryLikeExprNodeT>
    Action visitBinaryExpression_Remainder(
            const BinaryLikeExprNodeT* node,
            const Type* leftTy,
            const Type* rightTy);
    template <class BinaryLikeExprNodeT>
    Action visitBinaryExpression_Addition(
            const BinaryLikeExprNodeT* node,
            const Type* leftTy,
            const Type* rightTy);
    template <class BinaryLikeExprNodeT>
    Action visitBinaryExpression_Subtraction(
            const BinaryLikeExprNodeT* node,
            const Type* leftTy,
            const Type* rightTy);
    template <class BinaryLikeExprNodeT>
    Action visitBinaryExpression_BitwiseShift(
            const BinaryLikeExprNodeT* node,
            const Type* leftTy,
            const Type* rightTy);
    template <class BinaryLikeExprNodeT>
    Action visitBinaryExpression_Relational(
            const BinaryLikeExprNodeT* node,
            const Type* leftTy,
            const Type* rightTy);
    template <class BinaryLikeExprNodeT>
    Action visitBinaryExpression_Equality(
            const BinaryLikeExprNodeT* node,
            const Type* leftTy,
            const Type* rightTy);
    template <class BinaryLikeExprNodeT>
    Action visitBinaryExpression_Bitwise(
            const BinaryLikeExprNodeT* node,
            const Type* leftTy,
            const Type* rightTy);
    template <class BinaryLikeExprNodeT>
    Action visitBinaryExpression_Logical(
            const BinaryLikeExprNodeT* node,
            const Type* leftTy,
            const Type* rightTy);

    /* Assignments */
    Action visitAssignmentExpression_Simple(
            const AssignmentExpressionSyntax* node,
            const Type* leftTy,
            const Type* rightTy);

    //------------//
    // Statements //
    //------------//

    Action visitStatement(const StatementSyntax*);
    virtual Action visitDeclarationStatement(const DeclarationStatementSyntax*) override;
    virtual Action visitExpressionStatement(const ExpressionStatementSyntax*) override;
    virtual Action visitReturnStatement(const ReturnStatementSyntax*) override;

    //--------//
    // Common //
    //--------//
    virtual Action visitTypeName(const TypeNameSyntax*) override;

    static const Type* unqualifiedAndResolved(const Type* ty);
    bool isAssignable(const SyntaxNode* node, const Type* ty);
    const Type* typeOfStringLiteral(StringLiteral::EncodingPrefix encodingSuffix);
    bool isNULLPointerConstant(const SyntaxNode* node);
    bool typesAreCompatible(const Type* oneTy,
                            const Type* otherTy,
                            bool treatVoidAsAny,
                            bool ignoreQualifier);
    bool satisfyArithmeticTypeConstraint(const Type* ty, const SyntaxNode* node);
    bool satisfyIntegerTypeConstraint(const Type* ty, const SyntaxNode* node);
    bool satisfyRealTypeConstraint(const Type* ty, const SyntaxNode* node);
    const BasicType* determineCommonRealType(
            const BasicType* leftTy,
            const BasicType* rightTy) const;
    static BasicTypeKind performArithmeticConversions(
            BasicTypeKind leftTyK,
            BasicTypeKind rightTyK);
    static BasicTypeKind performIntegerPromotion(BasicTypeKind basicTyK);
    static BasicTypeKind performSignBasedIntegerConversion(
            BasicTypeKind signedTyK,
            BasicTypeKind unsignedTyK);
};

} // C
} // psy

#endif
