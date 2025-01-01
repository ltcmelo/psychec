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
#include "Fwds.h"
#include "SyntaxKind.h"
#include "SyntaxNodes.h"

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

    static bool isStorageClassSyntax(SyntaxToken tk)
    {
        switch (tk.kind()) {
            case SyntaxKind::Keyword_typedef:
            case SyntaxKind::Keyword_extern:
            case SyntaxKind::Keyword_static:
            case SyntaxKind::Keyword__Thread_local:
            case SyntaxKind::Keyword_ExtGNU___thread:
            case SyntaxKind::Keyword_auto:
            case SyntaxKind::Keyword_register:
                return true;
            default:
                return false;
        }
    }

    static bool isBasicTypeSpecifierSyntax(SyntaxToken tk)
    {
        switch (tk.kind()) {
            case SyntaxKind::Keyword_char:
            case SyntaxKind::Keyword_short:
            case SyntaxKind::Keyword_int:
            case SyntaxKind::Keyword_long:
            case SyntaxKind::Keyword_float:
            case SyntaxKind::Keyword_double:
            case SyntaxKind::Keyword_signed:
            case SyntaxKind::Keyword_unsigned:
            case SyntaxKind::Keyword__Bool:
            case SyntaxKind::Keyword__Complex:
            case SyntaxKind::Keyword_Ext_char16_t:
            case SyntaxKind::Keyword_Ext_char32_t:
            case SyntaxKind::Keyword_Ext_wchar_t:
            case SyntaxKind::Keyword_ExtGNU___complex__:
                return true;
            default:
                return false;
        }
    }

    static bool isTypeQualifierSyntax(SyntaxToken tk)
    {
        switch (tk.kind()) {
            case SyntaxKind::Keyword_const:
            case SyntaxKind::Keyword_volatile:
            case SyntaxKind::Keyword_restrict:
            case SyntaxKind::Keyword__Atomic:
                return true;
            default:
                return false;
        }
    }

    static bool isFunctionSpecifierSyntax(SyntaxToken tk)
    {
        switch (tk.kind()) {
            case SyntaxKind::Keyword_inline:
            case SyntaxKind::Keyword__Noreturn:
                return true;
            default:
                return false;
        }
    }

    static bool isPredefinedSyntax(SyntaxToken tk)
    {
        switch(tk.kind()) {
            case SyntaxKind::Keyword___func__:
            case SyntaxKind::Keyword_ExtGNU___FUNCTION__:
            case SyntaxKind::Keyword_ExtGNU___PRETTY_FUNCTION__:
            case SyntaxKind::Keyword_ExtGNU___printf__:
            case SyntaxKind::Keyword_ExtGNU___scanf__:
            case SyntaxKind::Keyword_ExtGNU___strftime__:
            case SyntaxKind::Keyword_ExtGNU___strfmon__:
                return true;

            default:
                return false;
        }
    }

    static bool isTypeSpecifierSyntax(const SpecifierSyntax* spec)
    {
        switch (spec->kind()) {
            case SyntaxKind::VoidTypeSpecifier:
            case SyntaxKind::BasicTypeSpecifier:
            case SyntaxKind::AtomicTypeSpecifier:
            case SyntaxKind::StructTypeSpecifier:
            case SyntaxKind::UnionTypeSpecifier:
            case SyntaxKind::EnumTypeSpecifier:
            case SyntaxKind::TypedefName:
                return true;
            default:
                return false;
        }
    }

    //-------------//
    // Expressions //
    //-------------//

    static bool isConstantSyntax(SyntaxToken tk)
    {
        switch (tk.kind()) {
            case SyntaxKind::IntegerConstantToken:
            case SyntaxKind::FloatingConstantToken:
            case SyntaxKind::CharacterConstantToken:
            case SyntaxKind::CharacterConstant_L_Token:
            case SyntaxKind::CharacterConstant_u_Token:
            case SyntaxKind::CharacterConstant_U_Token:
            case SyntaxKind::ImaginaryIntegerConstantToken:
            case SyntaxKind::ImaginaryFloatingConstantToken:
            case SyntaxKind::Keyword_Ext_true:
            case SyntaxKind::Keyword_Ext_false:
            case SyntaxKind::Keyword_Ext_NULL:
            case SyntaxKind::Keyword_Ext_nullptr:
                return true;
            default:
                return false;
        }
    }

    static bool isStringLiteralSyntax(SyntaxToken tk)
    {
        switch (tk.kind()) {
            case SyntaxKind::StringLiteralToken:
            case SyntaxKind::StringLiteral_L_Token:
            case SyntaxKind::StringLiteral_u8_Token:
            case SyntaxKind::StringLiteral_u_Token:
            case SyntaxKind::StringLiteral_U_Token:
            case SyntaxKind::StringLiteral_R_Token:
            case SyntaxKind::StringLiteral_LR_Token:
            case SyntaxKind::StringLiteral_u8R_Token:
            case SyntaxKind::StringLiteral_uR_Token:
            case SyntaxKind::StringLiteral_UR_Token:
                return true;
            default:
                return false;
        }
    }

    static bool isNAryOperatorSyntax(SyntaxToken tk)
    {
        switch (tk.kind()) {
            /* Binary */
            case SyntaxKind::BarBarToken:
            case SyntaxKind::AmpersandAmpersandToken:
            case SyntaxKind::BarToken:
            case SyntaxKind::CaretToken:
            case SyntaxKind::AmpersandToken:
            case SyntaxKind::EqualsEqualsToken:
            case SyntaxKind::ExclamationEqualsToken:
            case SyntaxKind::GreaterThanToken:
            case SyntaxKind::LessThanToken:
            case SyntaxKind::GreaterThanEqualsToken:
            case SyntaxKind::LessThanEqualsToken:
            case SyntaxKind::LessThanLessThanToken:
            case SyntaxKind::GreaterThanGreaterThanToken:
            case SyntaxKind::PlusToken:
            case SyntaxKind::MinusToken:
            case SyntaxKind::AsteriskToken:
            case SyntaxKind::SlashToken:
            case SyntaxKind::PercentToken:

            /* Conditional */
            case SyntaxKind::QuestionToken:

            /* Assignment */
            case SyntaxKind::EqualsToken:
            case SyntaxKind::AsteriskEqualsToken:
            case SyntaxKind::SlashEqualsToken:
            case SyntaxKind::PercentEqualsToken:
            case SyntaxKind::PlusEqualsToken:
            case SyntaxKind::MinusEqualsToken:
            case SyntaxKind::LessThanLessThanEqualsToken:
            case SyntaxKind::GreaterThanGreaterThanEqualsToken:
            case SyntaxKind::AmpersandEqualsToken:
            case SyntaxKind::CaretEqualsToken:
            case SyntaxKind::BarEqualsToken:

            /* Sequencing */
            case SyntaxKind::CommaToken:
                return true;

            default:
                return false;
        }
    }

    static SyntaxKind kindOfNAryOperatorSyntax(SyntaxToken tk)
    {
        switch (tk.kind()) {
            /* Binary */
            case SyntaxKind::BarBarToken:
                return SyntaxKind::LogicalORExpression;
            case SyntaxKind::AmpersandAmpersandToken:
                return SyntaxKind::LogicalANDExpression;
            case SyntaxKind::BarToken:
                return SyntaxKind::BitwiseORExpression;
            case SyntaxKind::CaretToken:
                return SyntaxKind::BitwiseXORExpression;
            case SyntaxKind::AmpersandToken:
                return SyntaxKind::BitwiseANDExpression;
            case SyntaxKind::EqualsEqualsToken:
                 return SyntaxKind::EqualsExpression;
            case SyntaxKind::ExclamationEqualsToken:
                return SyntaxKind::NotEqualsExpression;
            case SyntaxKind::GreaterThanToken:
                return SyntaxKind::GreaterThanExpression;
            case SyntaxKind::LessThanToken:
                return SyntaxKind::LessThanExpression;
            case SyntaxKind::GreaterThanEqualsToken:
                return SyntaxKind::GreaterThanOrEqualExpression;
            case SyntaxKind::LessThanEqualsToken:
                return SyntaxKind::LessThanOrEqualExpression;
            case SyntaxKind::LessThanLessThanToken:
                return SyntaxKind::LeftShiftExpression;
            case SyntaxKind::GreaterThanGreaterThanToken:
                return SyntaxKind::RightShiftExpression;
            case SyntaxKind::PlusToken:
                return SyntaxKind::AddExpression;
            case SyntaxKind::MinusToken:
                return SyntaxKind::SubstractExpression;
            case SyntaxKind::AsteriskToken:
                return SyntaxKind::MultiplyExpression;
            case SyntaxKind::SlashToken:
                return SyntaxKind::DivideExpression;
            case SyntaxKind::PercentToken:
                return SyntaxKind::ModuleExpression;

            /* Conditional */
            case SyntaxKind::QuestionToken:
                return SyntaxKind::ConditionalExpression;

            /* Assignment */
            case SyntaxKind::EqualsToken:
                return SyntaxKind::BasicAssignmentExpression;
            case SyntaxKind::AsteriskEqualsToken:
                return SyntaxKind::MultiplyAssignmentExpression;
            case SyntaxKind::SlashEqualsToken:
                return SyntaxKind::DivideAssignmentExpression;
            case SyntaxKind::PercentEqualsToken:
                return SyntaxKind::ModuloAssignmentExpression;
            case SyntaxKind::PlusEqualsToken:
                return SyntaxKind::AddAssignmentExpression;
            case SyntaxKind::MinusEqualsToken:
                return SyntaxKind::SubtractAssignmentExpression;
            case SyntaxKind::LessThanLessThanEqualsToken:
                return SyntaxKind::LeftShiftAssignmentExpression;
            case SyntaxKind::GreaterThanGreaterThanEqualsToken:
                return SyntaxKind::RightShiftAssignmentExpression;
            case SyntaxKind::AmpersandEqualsToken:
                return SyntaxKind::AndAssignmentExpression;
            case SyntaxKind::CaretEqualsToken:
                return SyntaxKind::ExclusiveOrAssignmentExpression;
            case SyntaxKind::BarEqualsToken:
                return SyntaxKind::OrAssignmentExpression;

            /* Sequencing */
            case SyntaxKind::CommaToken:
                return SyntaxKind::SequencingExpression;

            default:
                return SyntaxKind::Error;
        }
    }

    static bool isKindOfAssignmentExpression(SyntaxKind exprK)
    {
        switch (exprK) {
            case SyntaxKind::BasicAssignmentExpression:
            case SyntaxKind::MultiplyAssignmentExpression:
            case SyntaxKind::DivideAssignmentExpression:
            case SyntaxKind::ModuloAssignmentExpression:
            case SyntaxKind::AddAssignmentExpression:
            case SyntaxKind::SubtractAssignmentExpression:
            case SyntaxKind::LeftShiftAssignmentExpression:
            case SyntaxKind::RightShiftAssignmentExpression:
            case SyntaxKind::AndAssignmentExpression:
            case SyntaxKind::ExclusiveOrAssignmentExpression:
            case SyntaxKind::OrAssignmentExpression:
                return true;

            default:
                return false;
        }
    }

    static bool isKindOfBinaryExpression(SyntaxKind exprK)
    {
        switch (exprK) {
            case SyntaxKind::MultiplyExpression:
            case SyntaxKind::DivideExpression:
            case SyntaxKind::ModuleExpression:
            case SyntaxKind::AddExpression:
            case SyntaxKind::SubstractExpression:
            case SyntaxKind::LeftShiftExpression:
            case SyntaxKind::RightShiftExpression:
            case SyntaxKind::LessThanExpression:
            case SyntaxKind::LessThanOrEqualExpression:
            case SyntaxKind::GreaterThanExpression:
            case SyntaxKind::GreaterThanOrEqualExpression:
            case SyntaxKind::EqualsExpression:
            case SyntaxKind::NotEqualsExpression:
            case SyntaxKind::BitwiseANDExpression:
            case SyntaxKind::BitwiseXORExpression:
            case SyntaxKind::BitwiseORExpression:
            case SyntaxKind::LogicalANDExpression:
            case SyntaxKind::LogicalORExpression:
                return true;

            default:
                return false;
        }
    }

    //------------//
    // Statements //
    //------------//

    static bool isExtGNU_AsmQualifierSyntax(SyntaxToken tk)
    {
        switch (tk.kind()) {
            case SyntaxKind::Keyword_volatile:
            case SyntaxKind::Keyword_inline:
            case SyntaxKind::Keyword_goto:
                return true;
            default:
                return false;
        }
    }
};

} // C
} // psy

#endif
