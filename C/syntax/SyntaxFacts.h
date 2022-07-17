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

#ifndef PSYCHE_C_SYNTAX_FACTS_H__
#define PSYCHE_C_SYNTAX_FACTS_H__

#include "API.h"
#include "SyntaxKind.h"

namespace psy {
namespace C {

/**
 * \brief The SyntaxFacts class.
 *
 * \note
 * This API is inspired by that of \c Microsoft.CodeAnalysis.CSharp.SyntaxFacts
 * from Roslyn, the .NET Compiler Platform.
 */
class PSY_C_API SyntaxFacts
{
public:

    //--------------//
    // Declarations //
    //--------------//

    static bool isStorageClassToken(SyntaxKind tkK)
    {
        switch (tkK) {
            case Keyword_typedef:
            case Keyword_extern:
            case Keyword_static:
            case Keyword__Thread_local:
            case Keyword_ExtGNU___thread:
            case Keyword_auto:
            case Keyword_register:
                return true;
            default:
                return false;
        }
    }

    static bool isBuiltinTypeSpecifierToken(SyntaxKind tkK)
    {
        switch (tkK) {
            case Keyword_void:
            case Keyword_char:
            case Keyword_short:
            case Keyword_int:
            case Keyword_long:
            case Keyword_float:
            case Keyword_double:
            case Keyword__Bool:
            case Keyword__Complex:
            case Keyword_signed:
            case Keyword_unsigned:
            case Keyword_Ext_char16_t:
            case Keyword_Ext_char32_t:
            case Keyword_Ext_wchar_t:
            case Keyword_ExtGNU___complex__:
                return true;
            default:
                return false;
        }
    }

    static bool isTypeQualifierToken(SyntaxKind tkK)
    {
        switch (tkK) {
            case Keyword_const:
            case Keyword_volatile:
            case Keyword_restrict:
            case Keyword__Atomic:
                return true;
            default:
                return false;
        }
    }

    static bool isFunctionSpecifierToken(SyntaxKind tkK)
    {
        switch (tkK) {
            case Keyword_inline:
            case Keyword__Noreturn:
                return true;
            default:
                return false;
        }
    }

    //-------------//
    // Expressions //
    //-------------//

    static bool isConstantToken(SyntaxKind tkK)
    {
        switch (tkK) {
            case IntegerConstantToken:
            case FloatingConstantToken:
            case CharacterConstantToken:
            case CharacterConstant_L_Token:
            case CharacterConstant_u_Token:
            case CharacterConstant_U_Token:
            case ImaginaryIntegerConstantToken:
            case ImaginaryFloatingConstantToken:
            case Keyword_Ext_true:
            case Keyword_Ext_false:
            case Keyword_Ext_NULL:
            case Keyword_Ext_nullptr:
                return true;
            default:
                return false;
        }
    }

    static bool isStringLiteralToken(SyntaxKind tkK)
    {
        switch (tkK) {
            case StringLiteralToken:
            case StringLiteral_L_Token:
            case StringLiteral_u8_Token:
            case StringLiteral_u_Token:
            case StringLiteral_U_Token:
            case StringLiteral_R_Token:
            case StringLiteral_LR_Token:
            case StringLiteral_u8R_Token:
            case StringLiteral_uR_Token:
            case StringLiteral_UR_Token:
                return true;
            default:
                return false;
        }
    }

    static bool isNAryOperatorToken(SyntaxKind tkK)
    {
        switch (tkK) {
            /* Binary */
            case BarBarToken:
            case AmpersandAmpersandToken:
            case BarToken:
            case CaretToken:
            case AmpersandToken:
            case EqualsEqualsToken:
            case ExclamationEqualsToken:
            case GreaterThanToken:
            case LessThanToken:
            case GreaterThanEqualsToken:
            case LessThanEqualsToken:
            case LessThanLessThanToken:
            case GreaterThanGreaterThanToken:
            case PlusToken:
            case MinusToken:
            case AsteriskToken:
            case SlashToken:
            case PercentToken:

            /* Conditional */
            case QuestionToken:

            /* Assignment */
            case EqualsToken:
            case AsteriskEqualsToken:
            case SlashEqualsToken:
            case PercentEqualsToken:
            case PlusEqualsToken:
            case MinusEqualsToken:
            case LessThanLessThanEqualsToken:
            case GreaterThanGreaterThanEqualsToken:
            case AmpersandEqualsToken:
            case CaretEqualsToken:
            case BarEqualsToken:

            /* Sequencing */
            case CommaToken:
                return true;

            default:
                return false;
        }
    }

    static SyntaxKind NAryExpressionKind(SyntaxKind NAryTkK)
    {
        switch (NAryTkK) {
            /* Binary */
            case BarBarToken:
                return LogicalORExpression;
            case AmpersandAmpersandToken:
                return LogicalANDExpression;
            case BarToken:
                return BitwiseORExpression;
            case CaretToken:
                return BitwiseXORExpression;
            case AmpersandToken:
                return BitwiseANDExpression;
            case EqualsEqualsToken:
                 return EqualsExpression;
            case ExclamationEqualsToken:
                return NotEqualsExpression;
            case GreaterThanToken:
                return GreaterThanExpression;
            case LessThanToken:
                return LessThanExpression;
            case GreaterThanEqualsToken:
                return GreaterThanOrEqualExpression;
            case LessThanEqualsToken:
                return LessThanOrEqualExpression;
            case LessThanLessThanToken:
                return LeftShiftExpression;
            case GreaterThanGreaterThanToken:
                return RightShiftExpression;
            case PlusToken:
                return AddExpression;
            case MinusToken:
                return SubstractExpression;
            case AsteriskToken:
                return MultiplyExpression;
            case SlashToken:
                return DivideExpression;
            case PercentToken:
                return ModuleExpression;

            /* Conditional */
            case QuestionToken:
                return ConditionalExpression;

            /* Assignment */
            case EqualsToken:
                return BasicAssignmentExpression;
            case AsteriskEqualsToken:
                return MultiplyAssignmentExpression;
            case SlashEqualsToken:
                return DivideAssignmentExpression;
            case PercentEqualsToken:
                return ModuloAssignmentExpression;
            case PlusEqualsToken:
                return AddAssignmentExpression;
            case MinusEqualsToken:
                return SubtractAssignmentExpression;
            case LessThanLessThanEqualsToken:
                return LeftShiftAssignmentExpression;
            case GreaterThanGreaterThanEqualsToken:
                return RightShiftAssignmentExpression;
            case AmpersandEqualsToken:
                return AndAssignmentExpression;
            case CaretEqualsToken:
                return ExclusiveOrAssignmentExpression;
            case BarEqualsToken:
                return OrAssignmentExpression;

            /* Sequencing */
            case CommaToken:
                return SequencingExpression;

            default:
                return Error;
        }
    }

    static bool isAssignmentExpression(SyntaxKind exprK)
    {
        switch (exprK) {
            case BasicAssignmentExpression:
            case MultiplyAssignmentExpression:
            case DivideAssignmentExpression:
            case ModuloAssignmentExpression:
            case AddAssignmentExpression:
            case SubtractAssignmentExpression:
            case LeftShiftAssignmentExpression:
            case RightShiftAssignmentExpression:
            case AndAssignmentExpression:
            case ExclusiveOrAssignmentExpression:
            case OrAssignmentExpression:
                return true;

            default:
                return false;
        }
    }

    static bool isAssignmentExpressionOperatorToken(SyntaxKind tkK)
    {
        switch (tkK) {
            case EqualsToken:
            case PlusEqualsToken:
            case MinusEqualsToken:
            case AsteriskEqualsToken:
            case SlashEqualsToken:
            case PercentEqualsToken:
            case LessThanLessThanEqualsToken:
            case GreaterThanGreaterThanEqualsToken:
            case AmpersandEqualsToken:
            case CaretEqualsToken:
            case BarEqualsToken:
                return true;

            default:
                return false;
        }
    }

    static bool isBinaryExpression(SyntaxKind exprK)
    {
        switch (exprK) {
            case MultiplyExpression:
            case DivideExpression:
            case ModuleExpression:
            case AddExpression:
            case SubstractExpression:
            case LeftShiftExpression:
            case RightShiftExpression:
            case LessThanExpression:
            case LessThanOrEqualExpression:
            case GreaterThanExpression:
            case GreaterThanOrEqualExpression:
            case EqualsExpression:
            case NotEqualsExpression:
            case BitwiseANDExpression:
            case BitwiseXORExpression:
            case BitwiseORExpression:
            case LogicalANDExpression:
            case LogicalORExpression:
                return true;

            default:
                return false;
        }
    }

    static bool isBinaryExpressionOperatorToken(SyntaxKind tkK)
    {
        switch (tkK) {
            case AsteriskToken:
            case SlashToken:
            case PercentToken:
            case PlusToken:
            case MinusToken:
            case LessThanLessThanToken:
            case GreaterThanGreaterThanToken:
            case LessThanToken:
            case LessThanEqualsToken:
            case GreaterThanToken:
            case GreaterThanEqualsToken:
            case EqualsEqualsToken:
            case ExclamationEqualsToken:
            case AmpersandToken:
            case CaretToken:
            case BarToken:
            case AmpersandAmpersandToken:
            case BarBarToken:
                return true;

            default:
                return false;
        }
    }

    static bool isPredefinedToken(SyntaxKind tkK)
    {
        switch(tkK) {
            case Keyword___func__:
            case Keyword_ExtGNU___FUNCTION__:
            case Keyword_ExtGNU___PRETTY_FUNCTION__:
                return true;

            default:
                return false;
        }
    }

    //------------//
    // Statements //
    //------------//

    static bool isExtGNU_AsmQualifierToken(SyntaxKind tkK)
    {
        switch (tkK) {
            case Keyword_volatile:
            case Keyword_inline:
            case Keyword_goto:
                return true;
            default:
                return false;
        }
    }
};

} // C
} // psy

#endif
