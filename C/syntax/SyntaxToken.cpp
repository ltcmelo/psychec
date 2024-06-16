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

#include "SyntaxToken.h"

#include "Lexeme_ALL.h"
#include "SyntaxTree.h"

#include <iostream>
#include <type_traits>

namespace psy {
namespace C {

const char* tokenNames[] =
{
    // ----------------------------------------------------------------- //
    // These must be ordered as according to the SyntaxKind enumerators. //
    // ----------------------------------------------------------------- //

    "EOF",
    "#error#",

    "<multiline comment>",
    "<documentation multiline comment>",
    "<comment>",
    "<documentation comment>",

    "<identifier>",

    "<integer constant>",
    "<floating constant>",
    "<imaginary integer constant>",
    "<imaginary floating constant>",

    "<character constant>",
    "<L character constant>",
    "<u character constant>",
    "<U character constant>",

    "<string literal>",
    "<L string literal>",
    "<u8 string literal>",
    "<u string literal>",
    "<U string literal>",

    "...",

    "{",
    "}",

    "[",
    "]",

    "(",
    ")",

    "#",
    "##",

     ";",

    "auto",
    "break",
    "case",
    "char",
    "const",
    "continue",
    "default",
    "do",
    "double",
    "else",
    "enum",
    "extern",
    "float",
    "for",
    "goto",
    "if",
    "inline",
    "int",
    "long",
    "register",
    "restrict",
    "return",
    "short",
    "signed",
    "static",
    "struct",
    "switch",
    "typedef",
    "union",
    "unsigned",
    "void",
    "volatile",
    "while",

    "->",
    ".",

    "++",
    "--",

    "*",
    "&",

    "+",
    "-",
    "~",
    "/",
    "%",
    "<<",
    ">>",
    "|",
    "^",

    "!",
    "&&",
    "||",

    "<",
    "<=",
    ">",
    ">=",
    "==",
    "!=",

    ":",
    "?",

    "=",
    "*=",
    "/=",
    "%=",
    "+=",
    "-=",
    "<<=",
    ">>=",
    "&=",
    "^=",
    "|=",

    ",",

    "sizeof",

    "wchar_t",

    "_Bool",
    "_Complex",
    "__func__",

    "_Alignas",
    "_Alignof",
    "_Atomic",
    "_Generic",
    "_Noreturn",
    "_Static_assert",
    "_Thread_local",

    "char16_t",
    "char32_t",

    "false",
    "true",

    "NULL",
    "nullptr",

    "<R string literal>",
    "<LR string literal>",
    "<u8R string literal>",
    "<uR string literal>",
    "<UR string literal>",

    "__attribute__",
    "__thread",
    "__typeof__",
    "__extension__",
    "asm",
    "__builtin_va_arg",
    "__builtin_offsetof",
    "__builtin_tgmath",
    "__builtin_choose_expr",
    "__FUNCTION__",
    "__PRETTY_FUNCTION__",
    "__complex__",
    "__real__",
    "__imag__",

    "va_arg",
    "offsetof",

    "_Template",
    "_Forall",
    "_Exists",
    "<psychec marker>"
};

} // C
} // psy

using namespace psy;
using namespace C;

SyntaxToken::SyntaxToken(SyntaxTree* tree)
    : tree_(tree)
    , syntaxK_(SyntaxKind::EndOfFile)
    , byteSize_(0)
    , charSize_(0)
    , byteOffset_(0)
    , charOffset_(0)
    , matchingBracket_(0)
    , BF_all_(0)
    , lineno_(0)
    , column_(0)
    , lexeme_(nullptr)
{
    if (!tree_)
        BF_.missing_ = true;
}

SyntaxToken::~SyntaxToken()
{}

void SyntaxToken::setup()
{
    syntaxK_ = SyntaxKind::UnknownSyntax;
    byteSize_ = 0;
    charSize_ = 0;
    byteOffset_ = 0;
    charOffset_ = 0;
    matchingBracket_ = 0;
    BF_all_ = 0;
    lexeme_ = nullptr;
}

bool SyntaxToken::isComment() const
{
    return syntaxK_ == SyntaxKind::MultiLineCommentTrivia
            || syntaxK_ == SyntaxKind::MultiLineDocumentationCommentTrivia
            || syntaxK_ == SyntaxKind::SingleLineCommentTrivia
            || syntaxK_ == SyntaxKind::SingleLineDocumentationCommentTrivia
            || syntaxK_ == SyntaxKind::Keyword_ExtPSY_omission;
}

Location SyntaxToken::location() const
{
    LinePosition lineStart(lineno_, column_);
    LinePosition lineEnd(lineno_, column_ + byteSize_ - 1); // TODO: Account for joined tokens.
    FileLinePositionSpan fileLineSpan(tree_->filePath(), lineStart, lineEnd);

    return Location::create(fileLineSpan);
}

SyntaxToken::Category SyntaxToken::category() const
{
    return category(SyntaxKind(syntaxK_));
}

SyntaxToken::Category SyntaxToken::category(SyntaxKind k)
{
    switch (k) {
        case SyntaxKind::IdentifierToken:
            return Category::Identifiers;

        case SyntaxKind::IntegerConstantToken:
        case SyntaxKind::FloatingConstantToken:
        case SyntaxKind::CharacterConstantToken:
        case SyntaxKind::CharacterConstant_L_Token:
        case SyntaxKind::CharacterConstant_u_Token:
        case SyntaxKind::CharacterConstant_U_Token:
        case SyntaxKind::ImaginaryIntegerConstantToken:
        case SyntaxKind::ImaginaryFloatingConstantToken:
            return Category::Constants;

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
            return Category::StringLiterals;

        case SyntaxKind::EllipsisToken:
        case SyntaxKind::OpenBraceToken:
        case SyntaxKind::CloseBraceToken:
        case SyntaxKind::OpenBracketToken:
        case SyntaxKind::CloseBracketToken:
        case SyntaxKind::OpenParenToken:
        case SyntaxKind::CloseParenToken:

        case SyntaxKind::HashToken:
        case SyntaxKind::HashHashToken:

        case SyntaxKind::SemicolonToken:

        case SyntaxKind::ArrowToken:
        case SyntaxKind::DotToken:

        case SyntaxKind::PlusPlusToken:
        case SyntaxKind::MinusMinusToken:

        case SyntaxKind::AsteriskToken:
        case SyntaxKind::AmpersandToken:

        case SyntaxKind::PlusToken:
        case SyntaxKind::MinusToken:
        case SyntaxKind::TildeToken:
        case SyntaxKind::SlashToken:
        case SyntaxKind::PercentToken:
        case SyntaxKind::LessThanLessThanToken:
        case SyntaxKind::GreaterThanGreaterThanToken:
        case SyntaxKind::BarToken:
        case SyntaxKind::CaretToken:

        case SyntaxKind::ExclamationToken:
        case SyntaxKind::AmpersandAmpersandToken:
        case SyntaxKind::BarBarToken:

        case SyntaxKind::LessThanToken:
        case SyntaxKind::LessThanEqualsToken:
        case SyntaxKind::GreaterThanToken:
        case SyntaxKind::GreaterThanEqualsToken:
        case SyntaxKind::EqualsEqualsToken:
        case SyntaxKind::ExclamationEqualsToken:

        case SyntaxKind::ColonToken:
        case SyntaxKind::QuestionToken:

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
        case SyntaxKind::CommaToken:
            return Category::Punctuators;

        default: {
            auto rawK = static_cast<std::underlying_type_t<SyntaxKind>>(k);
            if (rawK > static_cast<std::underlying_type_t<SyntaxKind>>(SyntaxKind::STARTof_KeywordOrPunctuatorToken)
                    && rawK <= static_cast<std::underlying_type_t<SyntaxKind>>(SyntaxKind::ENDof_KeywordOrPunctuatorToken)) {
                return Category::Keywords;
            }
            return Category::Unrecognized;
        }
    }
}

Lexeme* SyntaxToken::lexeme() const
{
    return lexeme_;
}

std::string SyntaxToken::valueText() const
{
    return valueText_c_str();
}

const char* SyntaxToken::valueText_c_str() const
{
    switch (syntaxK_) {
        case SyntaxKind::IdentifierToken:
        case SyntaxKind::IntegerConstantToken:
        case SyntaxKind::FloatingConstantToken:
        case SyntaxKind::CharacterConstantToken:
        case SyntaxKind::CharacterConstant_L_Token:
        case SyntaxKind::CharacterConstant_u_Token:
        case SyntaxKind::CharacterConstant_U_Token:
        case SyntaxKind::ImaginaryIntegerConstantToken:
        case SyntaxKind::ImaginaryFloatingConstantToken:
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
            return lexeme_->c_str();

        default:
            return tokenNames[static_cast<std::uint16_t>(syntaxK_)];
    }
}

bool SyntaxToken::isValid() const
{
    return tree_ != nullptr;
}

TextSpan SyntaxToken::span() const
{
    return TextSpan(charStart(), charEnd());
}

SyntaxToken SyntaxToken::invalid()
{
    return SyntaxToken(nullptr);
}

namespace psy {
namespace C {

bool operator==(const SyntaxToken& a, const SyntaxToken& b)
{
    return a.tree_ == b.tree_
            && a.syntaxK_ == b.syntaxK_
            && a.byteOffset_ == b.byteOffset_
            && a.byteSize_ == b.byteSize_;
}

bool operator!=(const SyntaxToken& a, const SyntaxToken& b)
{
    return !(a == b);
}

std::string to_string(SyntaxToken::Category category)
{
    switch (category) {
        case SyntaxToken::Category::Keywords:
            return "[keywords]";

        case SyntaxToken::Category::Identifiers:
            return "[identifiers]";

        case SyntaxToken::Category::Constants:
            return "[constants]";

        case SyntaxToken::Category::StringLiterals:
            return "[string literals]";

        case SyntaxToken::Category::Punctuators:
            return "[punctuators]";

        default:
            return "[unrecognized]";
    }
}

} // C
} // psy
