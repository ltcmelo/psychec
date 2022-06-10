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

#ifndef PSYCHE_C_SYNTAX_KIND_H__
#define PSYCHE_C_SYNTAX_KIND_H__

#include "API.h"

#include <cstdint>

namespace psy {
namespace C {

/**
 * \brief The SyntaxKind enumeration.
 */
enum PSY_C_API SyntaxKind : std::uint16_t
{
    EndOfFile = 0,
    Error,

//=================================================================== Valued Tokens

    STARTof_ValuedToken,

    MultiLineCommentTrivia = STARTof_ValuedToken,
    MultiLineDocumentationCommentTrivia,
    SingleLineCommentTrivia,
    SingleLineDocumentationCommentTrivia,

    IdentifierToken,

    IntegerConstantToken,
    FloatingConstantToken,
    ImaginaryIntegerConstantToken,
    ImaginaryFloatingConstantToken,

    CharacterConstantToken,
    CharacterConstant_L_Token,
    CharacterConstant_u_Token,
    CharacterConstant_U_Token,

    StringLiteralToken,
    StringLiteral_L_Token,
    StringLiteral_u8_Token,
    StringLiteral_u_Token,
    StringLiteral_U_Token,

    ENDof_ValuedToken = StringLiteral_U_Token,

//=================================================================== Tokens

    STARTof_KeywordOrPunctuatorToken,

    EllipsisToken = STARTof_KeywordOrPunctuatorToken,

    OpenBraceToken,
    CloseBraceToken,

    OpenBracketToken,
    CloseBracketToken,

    OpenParenToken,
    CloseParenToken,

    HashToken,
    HashHashToken,

    SemicolonToken,

    // --------------- //
    // --- C89/C90 --- //
    // --------------- //

    /* Keywords: 6.4.1. */
    Keyword_auto,
    Keyword_break,
    Keyword_case,
    Keyword_char,
    Keyword_const,
    Keyword_continue,
    Keyword_default,
    Keyword_do,
    Keyword_double,
    Keyword_else,
    Keyword_enum,
    Keyword_extern,
    Keyword_float,
    Keyword_for,
    Keyword_goto,
    Keyword_if,
    Keyword_inline,
    Keyword_int,
    Keyword_long,
    Keyword_register,
    Keyword_restrict,
    Keyword_return,
    Keyword_short,
    Keyword_signed,
    Keyword_static,
    Keyword_struct,
    Keyword_switch,
    Keyword_typedef,
    Keyword_union,
    Keyword_unsigned,
    Keyword_void,
    Keyword_volatile,
    Keyword_while,

    /* Member access: 6.5.2.3. */
    ArrowToken,
    DotToken,

    /* Increment/decrement: 6.5.2.4 (postfix) and 6.5.3.3 (prefix). */
    PlusPlusToken,
    MinusMinusToken,

    /* Address, access, and indirection: 6.5.3.2.*/
    AsteriskToken,
    AmpersandToken,

    /* Arithmetic: 6.5.3.3 (unary), 6.5.5 (multiplicative), 6.5.6 (additive),
     * 6.5.7 (shift), 6.5.11 (bit or), and 6.5.12 (bit xor) -- 6.5.10 (bit AND)
     * and the star appeared earlier. */
    PlusToken,
    MinusToken,
    TildeToken,
    SlashToken,
    PercentToken,
    LessThanLessThanToken,
    GreaterThanGreaterThanToken,
    BarToken,
    CaretToken,

    /* Logical: 6.5.3.3 (unary), 6.5.13 (AND), and 6.5.14 (OR). */
    ExclamationToken,
    AmpersandAmpersandToken,
    BarBarToken,

    /* Comparisson: 6.5.8 (relational) and 6.5.9 (equality). */
    LessThanToken,
    LessThanEqualsToken,
    GreaterThanToken,
    GreaterThanEqualsToken,
    EqualsEqualsToken,
    ExclamationEqualsToken,

    /* Conditional: 6.5.15. */
    ColonToken,
    QuestionToken,

    /* Assignment: 6.5.16. */
    EqualsToken,
    AsteriskEqualsToken,
    SlashEqualsToken,
    PercentEqualsToken,
    PlusEqualsToken,
    MinusEqualsToken,
    LessThanLessThanEqualsToken,
    GreaterThanGreaterThanEqualsToken,
    AmpersandEqualsToken,
    CaretEqualsToken,
    BarEqualsToken,

    /* Comma: 6.5.17. */
    CommaToken,

    /* sizeof: 6.5.3.4. */
    Keyword_sizeof,

    // ----------- //
    // --- C95 --- //
    // ----------- //
    Keyword_Ext_wchar_t, /* Enabled/disabled by parse/language options. */

    // ----------- //
    // --- C99 --- //
    // ----------- //
    Keyword__Bool,
    Keyword__Complex,
    Keyword___func__,

    // ----------- //
    // --- C11 --- //
    // ----------- //
    Keyword__Alignas,
    Keyword__Alignof,
    Keyword__Atomic,
    Keyword__Generic,
    Keyword__Noreturn,
    Keyword__Static_assert,
    Keyword__Thread_local,

    /* The types char16_t and char32_t are from <uchar.h>.
       Enabled/disabled by parse/language options. */
    Keyword_Ext_char16_t,
    Keyword_Ext_char32_t,

    /* Enabled/disabled by parse/language options. */
    Keyword_Ext_false,
    Keyword_Ext_true,

    /* Enabled/disabled by parse/language options. */
    Keyword_Ext_NULL,
    Keyword_Ext_nullptr,

    /* Enabled/disabled by parse/language options. */
    StringLiteral_R_Token,
    StringLiteral_LR_Token,
    StringLiteral_u8R_Token,
    StringLiteral_uR_Token,
    StringLiteral_UR_Token,

    // -----------------------//
    // --- Extension: GNU --- //
    // -----------------------//
    Keyword_ExtGNU___attribute__,
    Keyword_ExtGNU___thread,
    Keyword_ExtGNU___typeof__,
    Keyword_ExtGNU___extension__,
    Keyword_ExtGNU___asm__, // Portability: J.5.10.
    Keyword_ExtGNU___builtin_va_arg,
    Keyword_ExtGNU___builtin_offsetof,
    Keyword_ExtGNU___builtin_tgmath,
    Keyword_ExtGNU___builtin_choose_expr,
    Keyword_ExtGNU___FUNCTION__,
    Keyword_ExtGNU___PRETTY_FUNCTION__,
    Keyword_ExtGNU___complex__,
    Keyword_ExtGNU___real__,
    Keyword_ExtGNU___imag__,

    // --------------------------//
    // --- Translated macros --- //
    // --------------------------//
    Keyword_MacroStd_va_arg,
    Keyword_MacroStd_offsetof,

    // --------------------------//
    // --- Extension: Psyche --- //
    // --------------------------//
    Keyword_ExtPSY__Template,
    Keyword_ExtPSY__Forall,
    Keyword_ExtPSY__Exists,
    Keyword_ExtPSY_omission,

    /* Operator names from <iso646.h> (7.9).
       Enable/disable this in parse/language options. */
    OperatorName_ORToken = BarBarToken,
    OperatorName_ANDToken = AmpersandAmpersandToken,
    OperatorName_NOTToken = ExclamationToken,
    OperatorName_XORToken = CaretToken,
    OperatorName_BITORToken = BarToken,
    OperatorName_COMPLToken = TildeToken,
    OperatorName_OREQToken = BarEqualsToken,
    OperatorName_ANDEQToken = AmpersandEqualsToken,
    OperatorName_BITANDToken = AmpersandToken,
    OperatorName_NOTEQToken = ExclamationEqualsToken,
    OperatorName_XOREQToken = CaretEqualsToken,

    /* Aliases
       TODO: Do NOT add more...
             We want to preserve original tokens to allow exact AST rewrite.
    */
    KeywordAlias_Bool = Keyword__Bool,
    KeywordAlias_asm = Keyword_ExtGNU___asm__,
    KeywordAlias_typeof = Keyword_ExtGNU___typeof__,

    KeywordAlias___alignas = Keyword__Alignas,
    KeywordAlias___alignof = Keyword__Alignof,
    KeywordAlias___asm = Keyword_ExtGNU___asm__,
    KeywordAlias___attribute = Keyword_ExtGNU___attribute__,
    KeywordAlias___const = Keyword_const,
    KeywordAlias___inline = Keyword_inline,
    KeywordAlias___restrict = Keyword_restrict,
    KeywordAlias___signed = Keyword_signed,
    KeywordAlias___typeof = Keyword_ExtGNU___typeof__,
    KeywordAlias___volatile = Keyword_volatile,

    KeywordAlias___alignas__ = Keyword__Alignas,
    KeywordAlias___alignof__ = Keyword__Alignof,
    KeywordAlias___const__ = Keyword_const,
    KeywordAlias___inline__ = Keyword_inline,
    KeywordAlias___restrict__ = Keyword_restrict,
    KeywordAlias___signed__ = Keyword_signed,
    KeywordAlias___volatile__ = Keyword_volatile,

    ENDof_KeywordOrPunctuatorToken = KeywordAlias___volatile__,

//=================================================================== Nodes

    STARTof_Node = 1000,

    //--------------//
    // Declarations //
    //--------------//
    TranslationUnit = STARTof_Node,
    IncompleteDeclaration,
    StructDeclaration,
    UnionDeclaration,
    EnumDeclaration,
    EnumeratorDeclaration,
    VariableAndOrFunctionDeclaration,
    FieldDeclaration,
    ParameterDeclaration,
    StaticAssertDeclaration,
    FunctionDefinition,
    ExtGNU_AsmStatementDeclaration,
    ExtPSY_TemplateDeclaration,
    ExtKR_ParameterDeclaration,

    /* Specifiers */
    TypedefStorageClass,
    ExternStorageClass,
    StaticStorageClass,
    AutoStorageClass,
    RegisterStorageClass,
    ThreadLocalStorageClass,
    ConstQualifier,
    RestrictQualifier,
    VolatileQualifier,
    AtomicQualifier,
    BuiltinTypeSpecifier,
    StructTypeSpecifier,
    UnionTypeSpecifier,
    EnumTypeSpecifier,
    AtomicTypeSpecifier,
    TypedefName,
    TypeDeclarationAsSpecifier,
    ExtGNU_Typeof,
    ExtPSY_QuantifiedTypeSpecifier,
    InlineSpecifier,
    NoReturnSpecifier,
    AlignmentSpecifier,
    ExtGNU_AttributeSpecifier,
    ExtGNU_Attribute,
    ExtGNU_AsmLabel,

    /* Declarators */
    PointerDeclarator,
    IdentifierDeclarator,
    AbstractDeclarator,
    ParenthesizedDeclarator,
    ArrayDeclarator,
    FunctionDeclarator,
    BitfieldDeclarator,
    SubscriptSuffix,
    ParameterSuffix,
    BitfieldDeclaratorSuffix,
    ExtKR_IdentifierDeclarator,

    /* Initializers */
    ExpressionInitializer,
    BraceEnclosedInitializer,
    DesignatedInitializer,
    FieldDesignator,
    ArrayDesignator,

    OffsetOfDesignator,

    //-------------//
    // Expressions //
    //-------------//
    IntegerConstantExpression,
    FloatingConstantExpression,
    ImaginaryIntegerConstantExpression,
    ImaginaryFloatingConstantExpression,
    CharacterConstantExpression,
    BooleanConstantExpression,
    NULL_ConstantExpression,
    StringLiteralExpression,
    ParenthesizedExpression,
    GenericSelectionExpression,
    TypedGenericAssociation,
    DefaultGenericAssociation,
    ExtGNU_EnclosedCompoundStatementExpression,
    ExtGNU_ComplexValuedExpression,
    ExtGNU_RealExpression,
    ExtGNU_ImagExpression,

    /* Names */
    IdentifierName,
    PredefinedName,

    /* Unary */
    PostIncrementExpression,
    PostDecrementExpression,
    PreIncrementExpression,
    PreDecrementExpression,
    UnaryPlusExpression,
    UnaryMinusExpression,
    BitwiseNotExpression,
    LogicalNotExpression,

    /* Type trait */
    SizeofExpression,
    AlignofExpression,

    /* Pointers */
    AddressOfExpression,
    PointerIndirectionExpression,

    /* Access */
    DirectMemberAccessExpression,
    IndirectMemberAccessExpression,
    ElementAccessExpression,

    /* Invocation-like */
    CallExpression,
    CompoundLiteralExpression,
    CastExpression,
    VAArgumentExpression,
    OffsetOfExpression,
    ExtGNU_ChooseExpression,

    /* Binary */
    MultiplyExpression,
    DivideExpression,
    ModuleExpression,
    AddExpression,
    SubstractExpression,
    LeftShiftExpression,
    RightShiftExpression,
    LessThanExpression,
    LessThanOrEqualExpression,
    GreaterThanExpression,
    GreaterThanOrEqualExpression,
    EqualsExpression,
    NotEqualsExpression,
    BitwiseANDExpression,
    BitwiseXORExpression,
    BitwiseORExpression,
    LogicalANDExpression,
    LogicalORExpression,

    /* Conditional */
    ConditionalExpression,

    /* Assignment */
    BasicAssignmentExpression,
    MultiplyAssignmentExpression,
    DivideAssignmentExpression,
    ModuloAssignmentExpression,
    AddAssignmentExpression,
    SubtractAssignmentExpression,
    LeftShiftAssignmentExpression,
    RightShiftAssignmentExpression,
    AndAssignmentExpression,
    ExclusiveOrAssignmentExpression,
    OrAssignmentExpression,

    /* Sequencing */
    SequencingExpression,

    //------------//
    // Statements //
    //------------//
    CompoundStatement,
    DeclarationStatement,
    ExpressionStatement,
    IdentifierLabelStatement,
    DefaultLabelStatement,
    CaseLabelStatement,
    DefaultStatement,
    IfStatement,
    SwitchStatement,
    WhileStatement,
    DoStatement,
    ForStatement,
    GotoStatement,
    ContinueStatement,
    BreakStatement,
    ReturnStatement,
    ExtGNU_AsmStatement,
    ExtGNU_AsmVolatileQualifier,
    ExtGNU_AsmInlineQualifier,
    ExtGNU_AsmGotoQualifier,
    ExtGNU_AsmInputOperand,
    ExtGNU_AsmOutputOperand,

    //--------//
    // Common //
    //--------//
    TypeName,
    ExpressionAsTypeReference,
    TypeNameAsTypeReference,

    //-------------//
    // Ambiguities //
    //-------------//
    AmbiguousCastOrBinaryExpression,
    AmbiguousTypeNameOrExpressionAsTypeReference,
    AmbiguousCallOrVariableDeclaration,
    AmbiguousMultiplicationOrPointerDeclaration,

    ENDof_Node = AmbiguousMultiplicationOrPointerDeclaration,

//=================================================================== Misc

    UnknownSyntax
};

} // C
} // psy

#endif
