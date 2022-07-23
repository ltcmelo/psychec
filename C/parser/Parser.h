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

#ifndef PSYCHE_C_PARSER_H__
#define PSYCHE_C_PARSER_H__

#include "API.h"
#include "Fwds.h"

#include "SyntaxTree.h"
#include "LexedTokens.h"

#include "infra/MemoryPool.h"
#include "syntax/SyntaxToken.h"

#include "../common/infra/InternalAccess.h"

#include <cstdint>
#include <functional>
#include <stack>
#include <vector>
#include <unordered_set>
#include <utility>

namespace psy {
namespace C {

class Lexer;

/**
 * \brief The C Parser class.
 */
class PSY_C_NON_API Parser
{
    friend class ParserTester;

public:
    ~Parser();

PSY_INTERNAL_AND_RESTRICTED:
    PSY_GRANT_ACCESS(SyntaxTree);
    PSY_GRANT_ACCESS(DebugRule);

    Parser(SyntaxTree* tree);

    TranslationUnitSyntax* parse();

    bool detectedAnyAmbiguity() const;

private:
    // Unavailable
    Parser(const Parser&) = delete;
    void operator=(const Parser&) = delete;

    MemoryPool* pool_;
    SyntaxTree* tree_;

    // While the parser is in backtracking mode, diagnostics are disabled.
    // To avoid unintended omission of syntax errors, the backtracker
    // should be discarded immediately after use, either explicitly or
    // implicitly; the latter happens either upon the object destruction
    // or after a single backtracking operation.
    struct Backtracker
    {
        Backtracker(Parser* parser, LexedTokens::IndexType tkIdx = 0);
        ~Backtracker();
        void discard();
        void backtrack();

        Parser* parser_;
        LexedTokens::IndexType refTkIdx_;
        bool done_;
        std::stack<const Backtracker*> chained_;
    };
    friend struct Backtracker;
    const Backtracker* backtracker_;
    bool mightBacktrack() const;

    struct DiagnosticsReporter
    {
        DiagnosticsReporter(Parser* parser)
            : parser_(parser)
            , IDsForDelay_(false)
        {}
        Parser* parser_;

        static std::string joinTokenNames(const std::vector<SyntaxKind>& validTkKinds);

        std::unordered_set<std::string> IDsForDelay_;
        std::vector<std::pair<DiagnosticDescriptor, LexedTokens::IndexType>> delayedDiags_;
        std::vector<std::pair<DiagnosticDescriptor, LexedTokens::IndexType>> retainedAmbiguityDiags_;

        void diagnose(DiagnosticDescriptor&& desc);
        void diagnoseDelayed();
        void diagnoseAmbiguityButRetainIt(DiagnosticDescriptor&& desc);

        /* General */
        void ExpectedFeature(const std::string& name);

        static const std::string ID_of_ExpectedFeature;

        /* Terminal */
        void ExpectedToken(SyntaxKind syntaxK);
        void ExpectedTokenWithin(const std::vector<SyntaxKind>& validTokens);
        void ExpectedTokenOfCategory(SyntaxToken::Category category, const std::string& id);
        void ExpectedTokenOfCategoryIdentifier();
        void ExpectedTokenOfCategoryConstant();
        void ExpectedTokenOfCategoryStringLiteral();

        static const std::string ID_of_ExpectedToken;
        static const std::string ID_of_ExpectedTokenWithin;
        static const std::string ID_of_ExpectedTokenOfCategoryIdentifier;
        static const std::string ID_of_ExpectedTokenOfCategoryConstant;
        static const std::string ID_of_ExpectedTokenOfCategoryStringLiteral;

        /* Non-terminal */
        void ExpectedFIRSTof(const std::string& rule, const std::string& id);
        void ExpectedFIRSTofExpression();
        void ExpectedFIRSTofEnumerationConstant();
        void ExpectedFIRSTofDirectDeclarator();
        void ExpectedFIRSTofParameterDeclaration();
        void ExpectedFIRSTofSpecifierQualifier();
        void ExpectedFOLLOWofDesignatedInitializer();
        void ExpectedFOLLOWofDeclarator();
        void ExpectedFOLLOWofStructDeclarator();
        void ExpectedFOLLOWofDeclaratorAndInitializer();
        void ExpectedFOLLOWofStructOrUnionOrEnum();
        void ExpectedFOLLOWofEnum();

        static const std::string ID_of_ExpectedFIRSTofExpression;
        static const std::string ID_of_ExpectedFIRSTofEnumerationConstant;
        static const std::string ID_of_ExpectedFIRSTofDirectDeclarator;
        static const std::string ID_of_ExpectedFIRSTofParameterDeclaration;
        static const std::string ID_of_ExpectedFIRSTofSpecifierQualifier;
        static const std::string ID_of_ExpectedFOLLOWofDesignatedInitializer;
        static const std::string ID_of_ExpectedFOLLOWofDeclarator;
        static const std::string ID_of_ExpectedFOLLOWofStructDeclarator;
        static const std::string ID_of_ExpectedFOLLOWofDeclaratorAndInitializer;
        static const std::string ID_of_ExpectedFOLLOWofStructOrUnionOrEnum;
        static const std::string ID_of_ExpectedFOLLOWofEnum;

        /* Detailed */
        void ExpectedFieldName();
        void ExpectedBraceEnclosedInitializerList();
        void ExpectedFieldDesignator();
        void UnexpectedInitializerOfDeclarator();
        void UnexpectedStaticOrTypeQualifiersInArrayDeclarator();
        void UnexpectedPointerInArrayDeclarator();
        void ExpectedNamedParameterBeforeEllipsis();
        void ExpectedTypeSpecifier();
        void UnexpectedCaseLabelOutsideSwitch();
        void UnexpectedDefaultLabelOutsideSwitch();
        void UnexpectedContinueOutsideLoop();
        void UnexpectedBreakOutsideSwitchOrLoop();
        void UnexpectedGNUExtensionFlag();

        static const std::string ID_of_ExpectedFieldName;
        static const std::string ID_of_ExpectedBraceEnclosedInitializerList;
        static const std::string ID_of_ExpectedFieldDesignator;
        static const std::string ID_of_UnexpectedInitializerOfDeclarator;
        static const std::string ID_of_UnexpectedStaticOrTypeQualifierInArrayDeclarator;
        static const std::string ID_of_UnexpectedPointerInArrayDeclarator;
        static const std::string ID_of_ExpectedNamedParameterBeforeEllipsis;
        static const std::string ID_of_ExpectedTypeSpecifier;
        static const std::string ID_of_UnexpectedCaseLabelOutsideSwitch;
        static const std::string ID_of_UnexpectedDefaultLabelOutsideSwitch;
        static const std::string ID_of_UnexpectedContinueOutsideLoop;
        static const std::string ID_of_UnexpectedBreakOutsideSwitchOrLoop;
        static const std::string ID_of_UnexpectedGNUExtensionFlag;

        /* Ambiguities */
        void AmbiguousTypeNameOrExpressionAsTypeReference();
        void AmbiguousCastOrBinaryExpression();
        void AmbiguousExpressionOrDeclarationStatement();

        static const std::string ID_of_AmbiguousTypeNameOrExpressionAsTypeReference;
        static const std::string ID_of_AmbiguousCastOrBinaryExpression;
        static const std::string ID_of_AmbiguousExpressionOrDeclarationStatement;
    };
    friend struct DiagnosticsReporter;

    DiagnosticsReporter diagReporter_;

    std::vector<
        std::pair<DiagnosticDescriptor,
                  LexedTokens::IndexType>> releaseRetainedAmbiguityDiags() const;

    struct DiagnosticsReporterDelayer
    {
        DiagnosticsReporterDelayer(DiagnosticsReporter* diagReporter,
                                   const std::string& diagID)
            : diagReporter_(diagReporter)
            , diagID_(diagID)
        {
            diagReporter_->IDsForDelay_.insert(diagID);
        }

        ~DiagnosticsReporterDelayer()
        {
            diagReporter_->IDsForDelay_.erase(diagID_);
        }

        DiagnosticsReporter* diagReporter_;
        std::string diagID_;
    };

    const SyntaxToken& peek(unsigned int LA = 1) const;
    LexedTokens::IndexType consume();
    bool match(SyntaxKind expectedTkK, LexedTokens::IndexType* tkIdx);
    bool matchOrSkipTo(SyntaxKind expectedTkK, LexedTokens::IndexType* tkIdx);
    void skipTo(SyntaxKind tkK);
    unsigned int curTkIdx_;

    int DEPTH_OF_EXPRS_;
    int DEPTH_OF_STMTS_;

    struct DepthControl
    {
        DepthControl(int& depth);
        ~DepthControl();
        int depth_;
    };
    friend struct DepthControl;

    /**
     * \brief The scope of a declaration.
     *
     * \remark 6.2.1-4
     */
    enum class DeclarationScope : uint8_t
    {
        File,
        Block,
        FunctionPrototype
    };

    enum class DeclaratorVariety : uint8_t
    {
        Named,
        Abstract
    };

    enum class IdentifierRole : uint8_t
    {
        AsDeclarator,
        AsTypedefName
    };

    enum class StatementContext : uint8_t
    {
        None,
        Switch,
        Loop,
        SwitchAndLoop
    };

    friend StatementContext operator+(StatementContext a, StatementContext b);

    template <class NodeT, class... Args> NodeT* makeNode(Args&&... args) const;

    //--------------//
    // Declarations //
    //--------------//
    void parseTranslationUnit(TranslationUnitSyntax*& unit);
    bool parseExternalDeclaration(DeclarationSyntax*& decl);
    void parseIncompleteDeclaration_AtFirst(DeclarationSyntax*& decl,
                                            const SpecifierListSyntax* specList = nullptr);
    bool parseStaticAssertDeclaration_AtFirst(DeclarationSyntax*& decl);
    bool parseExtGNU_AsmStatementDeclaration_AtFirst(DeclarationSyntax*& decl);
    bool parseDeclaration(
            DeclarationSyntax*& decl,
            bool (Parser::*parseSpecifiers)(DeclarationSyntax*&, SpecifierListSyntax*&),
            bool (Parser::*parse_AtFollowOfSpecifiers)(DeclarationSyntax*&, const SpecifierListSyntax*),
            DeclarationScope declScope);
    bool parseDeclarationOrFunctionDefinition(DeclarationSyntax*& decl);
    bool parseDeclarationOrFunctionDefinition_AtFollowOfSpecifiers(
            DeclarationSyntax*& decl,
            const SpecifierListSyntax* specList);
    bool parseFunctionDefinition_AtOpenBrace(
            DeclarationSyntax*& decl,
            const SpecifierListSyntax* specList,
            DeclaratorSyntax*& decltor,
            ExtKR_ParameterDeclarationListSyntax* paramKRList);
    bool parseStructDeclaration(DeclarationSyntax*& decl);
    bool parseStructDeclaration_AtFollowOfSpecifierQualifierList(
            DeclarationSyntax*& decl,
            const SpecifierListSyntax* specList);
    bool parseEnumerator(DeclarationSyntax*& decl);
    bool parseParameterDeclarationListAndOrEllipsis(ParameterSuffixSyntax*& paramDecltorSfx);
    bool parseParameterDeclarationList(ParameterDeclarationListSyntax*& paramList);
    bool parseParameterDeclaration(ParameterDeclarationSyntax*& paramDecl);
    bool parseExtPSY_TemplateDeclaration_AtFirst(DeclarationSyntax*& decl);
    bool parseExtKR_ParameterDeclarationList(ExtKR_ParameterDeclarationListSyntax*& paramList);
    bool parseExtKR_ParameterDeclaration(ExtKR_ParameterDeclarationSyntax*& paramDecl);

    bool ignoreDeclarator();
    bool ignoreDeclarationOrDefinition();
    bool ignoreMemberDeclaration();
    bool ignoreStatement();

    /* Specifiers */
    bool parseDeclarationSpecifiers(DeclarationSyntax*& decl, SpecifierListSyntax*& specList);
    bool parseSpecifierQualifierList(DeclarationSyntax*& decl, SpecifierListSyntax*& specList);
    template <class SpecT> void parseTrivialSpecifier_AtFirst(
            SpecifierSyntax*& spec,
            SyntaxKind specK);
    bool parseAlignmentSpecifier_AtFirst(SpecifierSyntax*& spec);
    bool parseExtGNU_Typeof_AtFirst(SpecifierSyntax*& spec);
    void parseTypedefName_AtFirst(SpecifierSyntax*& spec);
    bool parseAtomiceTypeSpecifier_AtFirst(SpecifierSyntax*& spec);
    template <class TypeDeclT> bool parseTagTypeSpecifier_AtFirst(
            DeclarationSyntax*& decl,
            SpecifierSyntax*& spec,
            SyntaxKind declK,
            SyntaxKind specK,
            bool (Parser::*parseMember)(DeclarationSyntax*&));
    bool parseExtGNU_AttributeSpecifierList_AtFirst(SpecifierListSyntax*& specList);
    bool parseExtGNU_AttributeSpecifier_AtFirst(SpecifierSyntax*& spec);
    bool parseExtGNU_AttributeList(ExtGNU_AttributeListSyntax*& attrList);
    bool parseExtGNU_Attribute(ExtGNU_AttributeSyntax*& attr);
    bool parseExtGNU_AttributeArguments(ExpressionListSyntax*& exprList);
    bool parseExtGNU_AttributeArgumentsLLVM(ExpressionListSyntax*& exprList);
    bool parseExtGNU_AsmLabel_AtFirst(SpecifierSyntax*& attr);
    bool parseExtPSY_QuantifiedTypeSpecifier_AtFirst(SpecifierSyntax*& spec);

    IdentifierRole determineIdentifierRole(bool seenType) const;

    /* Declarators */
    bool parseAbstractDeclarator(DeclaratorSyntax*& decltor);
    bool parseDeclarator(DeclaratorSyntax*& decltor, DeclarationScope declScope);
    bool parseDeclarator(DeclaratorSyntax*& decltor,
                         DeclarationScope declScope,
                         DeclaratorVariety decltorVariety);
    bool parseDirectDeclarator(DeclaratorSyntax*& decltor,
                               DeclarationScope declScope,
                               DeclaratorVariety decltorVariety,
                               SpecifierListSyntax* attrList);
    bool parseDirectDeclaratorSuffix(DeclaratorSyntax*& decltor,
                                     DeclarationScope declScope,
                                     DeclaratorVariety decltorVariety,
                                     SpecifierListSyntax* attrList,
                                     DeclaratorSyntax* innerDecltor);
    bool parseTypeQualifiersAndAttributes(SpecifierListSyntax*& specList);

    /* Initializers */
    bool parseInitializer(InitializerSyntax*& init);
    bool parseExpressionInitializer(InitializerSyntax*& init);
    bool parseBraceEnclosedInitializer_AtFirst(InitializerSyntax*& init);
    bool parseInitializerList(InitializerListSyntax*& initList);
    bool parseInitializerListItem(InitializerSyntax*& init, InitializerListSyntax*& initList);
    bool parseDesignatedInitializer_AtFirst(
            InitializerSyntax*& init,
            bool (Parser::*parseDesig)(DesignatorSyntax*& desig));
    bool parseDesignatorList_AtFirst(
            DesignatorListSyntax*& desigList,
            bool (Parser::*parseDesig)(DesignatorSyntax*& desig));
    bool parseFieldDesignator_AtFirst(DesignatorSyntax*& desig);
    bool parseArrayDesignator_AtFirst(DesignatorSyntax*& desig);

    bool parseOffsetOfDesignator(DesignatorSyntax*& desig);

    //-------------//
    // Expressions //
    //-------------//
    bool parseExpression(ExpressionSyntax*& expr);
    bool parseIdentifierName(ExpressionSyntax*& expr);
    void parseIdentifierName_AtFirst(ExpressionSyntax*& expr);
    void parsePredefinedName_AtFirst(ExpressionSyntax*& expr);
    template <class ExprT> bool parseConstant(ExpressionSyntax*& expr, SyntaxKind exprK);
    template <class ExprT> void parseConstant_AtFirst(ExpressionSyntax*& expr, SyntaxKind exprK);
    bool parseStringLiteral(ExpressionSyntax*& expr);
    void parseStringLiteral_AtFirst(ExpressionSyntax*& expr);
    bool parseParenthesizedExpression_AtFirst(ExpressionSyntax*& expr);
    bool parseGenericSelectionExpression_AtFirst(ExpressionSyntax*& expr);
    bool parseGenericAssociationList(GenericAssociationListSyntax*& assocList);
    bool parseGenericAssociation(GenericAssociationSyntax*& assoc,
                                 GenericAssociationListSyntax*& assocList);
    bool parseExtGNU_StatementExpression_AtFirst(ExpressionSyntax*& expr);
    bool parseExtGNU_ComplexValuedExpression_AtFirst(ExpressionSyntax*& expr, SyntaxKind exprK);

    /* Postfix */
    bool parseExpressionWithPrecedencePostfix(ExpressionSyntax*& expr);
    bool parsePostfixExpression_AtFollowOfPrimary(ExpressionSyntax*& expr);
    template <class ExprT> bool parsePostfixExpression_AtFollowOfPrimary(
            ExpressionSyntax*& expr,
            SyntaxKind exprK,
            std::function<bool(ExprT*&)> parsePostfix);
    bool parseCallArguments(ExpressionListSyntax*& exprList);
    bool parseCallArgument(ExpressionSyntax*& expr, ExpressionListSyntax*& exprList);
    bool parseCompoundLiteral_AtOpenParen(ExpressionSyntax*& expr);
    bool parseCompoundLiteral_AtOpenBrace(
            ExpressionSyntax*& expr,
            LexedTokens::IndexType openParenTkIdx,
            TypeNameSyntax* typeName,
            LexedTokens::IndexType closeParenTkIdx);
    bool parseVAArgumentExpression_AtFirst(ExpressionSyntax*& expr);
    bool parseOffsetOfExpression_AtFirst(ExpressionSyntax*& expr);
    bool parseExtGNU_ChooseExpression_AtFirst(ExpressionSyntax*& expr);

    /* Unary */
    bool parseExpressionWithPrecedenceUnary(ExpressionSyntax*& expr);
    bool parsePrefixUnaryExpression_AtFirst(
            ExpressionSyntax*& expr,
            SyntaxKind exprK,
            bool (Parser::*parseOperand)(ExpressionSyntax*&));
    bool parseTypeTraitExpression_AtFirst(ExpressionSyntax*& expr, SyntaxKind exprK);

    /* Cast */
    bool parseExpressionWithPrecedenceCast(ExpressionSyntax*& expr);
    bool parseCompoundLiteralOrCastExpression_AtFirst(ExpressionSyntax*& expr);
    void maybeAmbiguateCastExpression(ExpressionSyntax*& expr);

    /* N-ary */
    bool parseExpressionWithPrecedenceMultiplicative(ExpressionSyntax*& expr);
    bool parseExpressionWithPrecedenceAdditive(ExpressionSyntax*& expr);
    bool parseExpressionWithPrecedenceShift(ExpressionSyntax*& expr);
    bool parseExpressionWithPrecedenceRelational(ExpressionSyntax*& expr);
    bool parseExpressionWithPrecedenceEquality(ExpressionSyntax*& expr);
    bool parseExpressionWithPrecedenceBitwiseAND(ExpressionSyntax*& expr);
    bool parseExpressionWithPrecedenceBitwiseXOR(ExpressionSyntax*& expr);
    bool parseExpressionWithPrecedenceBitwiseOR(ExpressionSyntax*& expr);
    bool parseExpressionWithPrecedenceLogicalAND(ExpressionSyntax*& expr);
    bool parseExpressionWithPrecedenceLogicalOR(ExpressionSyntax*& expr);
    bool parseExpressionWithPrecedenceConditional(ExpressionSyntax*& expr);
    bool parseExpressionWithPrecedenceAssignment(ExpressionSyntax*& expr);
    bool parseExpressionWithPrecedenceComma(ExpressionSyntax*& expr);
    bool parseNAryExpression(ExpressionSyntax*& expr, std::uint8_t cutoffPrecedence);
    bool parseNAryExpression_AtOperator(ExpressionSyntax*& baseExpr,
                                        std::uint8_t cutoffPrecedence);

    template <class NodeT> NodeT* fill_LeftOperandInfixOperatorRightOperand_MIXIN(
            NodeT* expr,
            ExpressionSyntax* left,
            LexedTokens::IndexType opTkIdx,
            ExpressionSyntax* right);

    //------------//
    // Statements //
    //------------//
    bool parseStatement(StatementSyntax*& stmt, StatementContext stmtCtx);
    bool parseCompoundStatement_AtFirst(
            StatementSyntax*& stmt,
            StatementContext stmtCtx);
    bool parseDeclarationStatement(
            StatementSyntax*& stmt,
            bool (Parser::*parseDecl)(DeclarationSyntax*&));
    bool parseExpressionStatement(StatementSyntax*& stmt);
    bool parseLabeledStatement_AtFirst(StatementSyntax*& stmt, StatementContext stmtCtx);
    bool parseIfStatement_AtFirst(StatementSyntax*& stmt, StatementContext stmtCtx);
    bool parseSwitchStatement_AtFirst(StatementSyntax*& stmt, StatementContext stmtCtx);
    bool parseWhileStatement_AtFirst(StatementSyntax*& stmt, StatementContext stmtCtx);
    bool parseDoStatement_AtFirst(StatementSyntax*& stmt, StatementContext stmtCtx);
    bool parseForStatement_AtFirst(StatementSyntax*& stmt, StatementContext stmtCtx);
    bool parseGotoStatement_AtFirst(StatementSyntax*& stmt);
    bool parseContinueStatement_AtFirst(StatementSyntax*& stmt, StatementContext stmtCtx);
    bool parseBreakStatement_AtFirst(StatementSyntax*& stmt, StatementContext stmtCtx);
    bool parseReturnStatement_AtFirst(StatementSyntax*& stmt);
    bool parseExtGNU_AsmStatement(StatementSyntax*& stmt);
    void parseExtGNU_AsmQualifiers(SpecifierListSyntax*& specList);
    bool parseExtGNU_AsmOperands(ExtGNU_AsmOperandListSyntax*& asmOprdsList);
    bool parseExtGNU_AsmOutputOperand_AtFirst(
            ExtGNU_AsmOperandSyntax*& asmOprd,
            ExtGNU_AsmOperandListSyntax*&);
    bool parseExtGNU_AsmInputOperand_AtFirst(
            ExtGNU_AsmOperandSyntax*& asmOprd,
            ExtGNU_AsmOperandListSyntax*&);
    bool parseExtGNU_AsmOperand_AtFirst(
            ExtGNU_AsmOperandSyntax*& asmOprd,
            SyntaxKind oprdK);
    bool parseExtGNU_AsmClobbers(ExpressionListSyntax*& clobList);
    bool parseExtGNU_AsmClobber_AtFirst(
            ExpressionSyntax*& clob,
            ExpressionListSyntax*& clobList);
    bool parseExtGNU_AsmGotoLabels(ExpressionListSyntax*& labelList);
    bool parseExtGNU_AsmGotoLabel_AtFirst(
            ExpressionSyntax*& label,
            ExpressionListSyntax*&);
    void maybeAmbiguateStatement(StatementSyntax*& stmt);
    bool checkStatementParse(bool stmtParsed);

    //--------//
    // Common //
    //--------//
    template <class NodeT, class NodeListT> bool parseCommaSeparatedItems(
            NodeListT*& nodeList,
            bool (Parser::*parseItem)(NodeT*& node, NodeListT*& nodeList));
    bool parseTypeName(TypeNameSyntax*& typeName);
    bool parseParenthesizedTypeNameOrExpression(TypeReferenceSyntax*& tyRef);
    void maybeAmbiguateTypeReference(TypeReferenceSyntax*& tyRef);
};

} // C
} // psy

#endif
