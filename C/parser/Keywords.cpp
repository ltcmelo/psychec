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

#include "Lexer.h"

#include "syntax/SyntaxKind.h"
#include "parser/ParseOptions.h"

#include <iostream>

namespace psy {
namespace C {

static inline SyntaxKind classify2(const char* s, const ParseOptions& opts)
{
    if (s[0] == 'd') {
        if (s[1] == 'o') {
            return Keyword_do;
        }
    }
    else if (s[0] == 'i') {
        if (s[1] == 'f') {
            return Keyword_if;
        }
    }
    return IdentifierToken;
}

static inline SyntaxKind classify3(const char* s, const ParseOptions& opts)
{
    if (s[0] == 'a') {
        if (s[1] == 's') {
            if (s[2] == 'm') {
                return KeywordAlias_asm;
            }
        }
    }
    else if (s[0] == 'f') {
        if (s[1] == 'o') {
            if (s[2] == 'r') {
                return Keyword_for;
            }
        }
    }
    else if (s[0] == 'i') {
        if (s[1] == 'n') {
            if (s[2] == 't') {
                return Keyword_int;
            }
        }
    }
    return IdentifierToken;
}

static inline SyntaxKind classify4(const char* s, const ParseOptions& opts)
{
    if (s[0] == 'a') {
        if (s[1] == 'u') {
            if (s[2] == 't') {
                if (s[3] == 'o') {
                    return Keyword_auto;
                }
            }
        }
    }
    else if (s[0] == 'b'
             && opts.extensions().translations().isEnabled_Translate_bool_AsKeyword()) {
        if (s[1] == 'o') {
            if (s[2] == 'o') {
                if (s[3] == 'l') {
                    return KeywordAlias_Bool;
                }
            }
        }
    }
    else if (s[0] == 'c') {
        if (s[1] == 'a') {
            if (s[2] == 's') {
                if (s[3] == 'e') {
                    return Keyword_case;
                }
            }
        }
        else if (s[1] == 'h') {
            if (s[2] == 'a') {
                if (s[3] == 'r') {
                    return Keyword_char;
                }
            }
        }
    }
    else if (s[0] == 'e') {
        if (s[1] == 'l') {
            if (s[2] == 's') {
                if (s[3] == 'e') {
                    return Keyword_else;
                }
            }
        }
        else if (s[1] == 'n') {
            if (s[2] == 'u') {
                if (s[3] == 'm') {
                    return Keyword_enum;
                }
            }
        }
    }
    else if (s[0] == 'g') {
        if (s[1] == 'o') {
            if (s[2] == 't') {
                if (s[3] == 'o') {
                    return Keyword_goto;
                }
            }
        }
    }
    else if (s[0] == 'l') {
        if (s[1] == 'o') {
            if (s[2] == 'n') {
                if (s[3] == 'g') {
                    return Keyword_long;
                }
            }
        }
    }
    else if (s[0] == 'N'
             && opts.extensions().isEnabled_NULLAsBuiltin()) {
        if (s[1] == 'U') {
            if (s[2] == 'L') {
                if (s[3] == 'L') {
                    return Keyword_Ext_NULL;
                }
            }
        }
    }
    else if (s[0] == 't'
             && opts.extensions().isEnabled_NativeBooleans()) {
        if (s[1] == 'r') {
            if (s[2] == 'u') {
                if (s[3] == 'e') {
                    return Keyword_Ext_true;
                }
            }
        }
    }
    else if (s[0] == 'v') {
        if (s[1] == 'o') {
            if (s[2] == 'i') {
                if (s[3] == 'd') {
                    return Keyword_void;
                }
            }
        }
    }
    return IdentifierToken;
}

static inline SyntaxKind classify5(const char* s, const ParseOptions& opts)
{
    if (s[0] == '_') {
        if (s[1] == '_') {
            if (s[2] == 'a') {
                if (s[3] == 's') {
                    if (s[4] == 'm') {
                        return KeywordAlias___asm;
                    }
                }
            }
        }
        else if (s[1] == 'B'
                 && opts.extensions().translations().isEnabled_Translate_bool_AsKeyword()) {
            if (s[2] == 'o') {
                if (s[3] == 'o') {
                    if (s[4] == 'l') {
                        return Keyword__Bool;
                    }
                }
            }
        }
    }
    else if (s[0] == 'b') {
        if (s[1] == 'r') {
            if (s[2] == 'e') {
                if (s[3] == 'a') {
                    if (s[4] == 'k') {
                        return Keyword_break;
                    }
                }
            }
        }
    }
    else if (s[0] == 'c') {
        if (s[1] == 'o') {
            if (s[2] == 'n') {
                if (s[3] == 's') {
                    if (s[4] == 't') {
                        return Keyword_const;
                    }
                }
            }
        }
    }
    else if (s[0] == 'f') {
        if (s[1] == 'a'
                && opts.extensions().isEnabled_NativeBooleans()) {
            if (s[2] == 'l') {
                if (s[3] == 's') {
                    if (s[4] == 'e') {
                        return Keyword_Ext_false;
                    }
                }
            }
        }
        else if (s[1] == 'l') {
            if (s[2] == 'o') {
                if (s[3] == 'a') {
                    if (s[4] == 't') {
                        return Keyword_float;
                    }
                }
            }
        }
    }
    else if (s[0] == 's') {
        if (s[1] == 'h') {
            if (s[2] == 'o') {
                if (s[3] == 'r') {
                    if (s[4] == 't') {
                        return Keyword_short;
                    }
                }
            }
        }
    }
    else if (s[0] == 'u') {
        if (s[1] == 'n') {
            if (s[2] == 'i') {
                if (s[3] == 'o') {
                    if (s[4] == 'n') {
                        return Keyword_union;
                    }
                }
            }
        }
    }
    else if (s[0] == 'w') {
        if (s[1] == 'h') {
            if (s[2] == 'i') {
                if (s[3] == 'l') {
                    if (s[4] == 'e') {
                        return Keyword_while;
                    }
                }
            }
        }
    }
    return IdentifierToken;
}

static inline SyntaxKind classify6(const char* s, const ParseOptions& opts)
{
    if (s[0] == 'd') {
        if (s[1] == 'o') {
            if (s[2] == 'u') {
                if (s[3] == 'b') {
                    if (s[4] == 'l') {
                        if (s[5] == 'e') {
                            return Keyword_double;
                        }
                    }
                }
            }
        }
    }
    else if (s[0] == 'e') {
        if (s[1] == 'x') {
            if (s[2] == 't') {
                if (s[3] == 'e') {
                    if (s[4] == 'r') {
                        if (s[5] == 'n') {
                            return Keyword_extern;
                        }
                    }
                }
            }
        }
    }
    else if (s[0] == 'i'
             && opts.dialect().std() >= LanguageDialect::Std::C99) {
        if (s[1] == 'n') {
            if (s[2] == 'l') {
                if (s[3] == 'i') {
                    if (s[4] == 'n') {
                        if (s[5] == 'e') {
                            return Keyword_inline;
                        }
                    }
                }
            }
        }
    }
    else if (s[0] == 'r') {
        if (s[1] == 'e') {
            if (s[2] == 't') {
                if (s[3] == 'u') {
                    if (s[4] == 'r') {
                        if (s[5] == 'n') {
                            return Keyword_return;
                        }
                    }
                }
            }
        }
    }
    else if (s[0] == 's') {
        if (s[1] == 'i') {
            if (s[2] == 'g') {
                if (s[3] == 'n') {
                    if (s[4] == 'e') {
                        if (s[5] == 'd') {
                            return Keyword_signed;
                        }
                    }
                }
            }
            else if (s[2] == 'z') {
                if (s[3] == 'e') {
                    if (s[4] == 'o') {
                        if (s[5] == 'f') {
                            return Keyword_sizeof;
                        }
                    }
                }
            }
        }
        else if (s[1] == 't') {
            if (s[2] == 'a') {
                if (s[3] == 't') {
                    if (s[4] == 'i') {
                        if (s[5] == 'c') {
                            return Keyword_static;
                        }
                    }
                }
            }
            else if (s[2] == 'r') {
                if (s[3] == 'u') {
                    if (s[4] == 'c') {
                        if (s[5] == 't') {
                            return Keyword_struct;
                        }
                    }
                }
            }
        }
        else if (s[1] == 'w') {
            if (s[2] == 'i') {
                if (s[3] == 't') {
                    if (s[4] == 'c') {
                        if (s[5] == 'h') {
                            return Keyword_switch;
                        }
                    }
                }
            }
        }
    }
    else if (s[0] == 't') {
        if (s[1] == 'y') {
            if (s[2] == 'p') {
                if (s[3] == 'e') {
                    if (s[4] == 'o') {
                        if (s[5] == 'f') {
                            return KeywordAlias_typeof;
                        }
                    }
                }
            }
        }
    }
    else if (s[0] == 'v'
                && opts.extensions().translations().isEnabled_Translate_va_arg_AsKeyword()) {
        if (s[1] == 'a') {
            if (s[2] == '_') {
                if (s[3] == 'a') {
                    if (s[4] == 'r') {
                        if (s[5] == 'g') {
                            return Keyword_MacroStd_va_arg;
                        }
                    }
                }
            }
        }
    }
    return IdentifierToken;
}

static inline SyntaxKind classify7(const char* s, const ParseOptions& opts)
{
    if (s[0] == '_') {
        if (s[1] == '_') {
            if (s[2] == 'a') {
                if (s[3] == 's') {
                    if (s[4] == 'm') {
                        if (s[5] == '_') {
                            if (s[6] == '_') {
                                return Keyword_ExtGNU___asm__;
                            }
                        }
                    }
                }
            }
            else if (s[2] == 'c') {
                if (s[3] == 'o') {
                    if (s[4] == 'n') {
                        if (s[5] == 's') {
                            if (s[6] == 't') {
                                return KeywordAlias___const;
                            }
                        }
                    }
                }
            }
        }
        else if (s[1] == 'A'
                 && opts.dialect().std() >= LanguageDialect::Std::C11) {
            if (s[2] == 't') {
                if (s[3] == 'o') {
                    if (s[4] == 'm') {
                        if (s[5] == 'i') {
                            if (s[6] == 'c') {
                                return Keyword__Atomic;
                            }
                        }
                    }
                }
            }
        }
        else if (s[1] == 'F'
                 && opts.extensions().isEnabled_ExtPSY_Generics()) {
            if (s[2] == 'o') {
                if (s[3] == 'r') {
                    if (s[4] == 'a') {
                        if (s[5] == 'l') {
                            if (s[6] == 'l') {
                                return Keyword_ExtPSY__Forall;
                            }
                        }
                    }
                }
            }
        }
        else if (s[1] == 'E'
                 && opts.extensions().isEnabled_ExtPSY_Generics()) {
            if (s[2] == 'x') {
                if (s[3] == 'i') {
                    if (s[4] == 's') {
                        if (s[5] == 't') {
                            if (s[6] == 's') {
                                return Keyword_ExtPSY__Exists;
                            }
                        }
                    }
                }
            }
        }
    }
    else if (s[0] == 'a'
                && opts.dialect().std() >= LanguageDialect::Std::C11) {
        if (s[1] == 'l') {
            if (s[2] == 'i') {
                if (s[3] == 'g') {
                    if (s[4] == 'n') {
                        if (s[5] == 'a') {
                            if (s[6] == 's'
                                    && opts.extensions().translations().isEnabled_Translate_alignas_AsKeyword()) {
                                return Keyword__Alignas;
                            }
                        }
                        else if (s[5] == 'o') {
                            if (s[6] == 'f'
                                    && opts.extensions().translations().isEnabled_Translate_alignof_AsKeyword()) {
                                return Keyword__Alignof;
                            }
                        }
                    }
                }
            }
        }
    }
    else if (s[0] == 'd') {
        if (s[1] == 'e') {
            if (s[2] == 'f') {
                if (s[3] == 'a') {
                    if (s[4] == 'u') {
                        if (s[5] == 'l') {
                            if (s[6] == 't') {
                                return Keyword_default;
                            }
                        }
                    }
                }
            }
        }
    }
    else if (s[0] == 'n'
             && opts.extensions().isEnabled_CPP_nullptr()) {
        if (s[1] == 'u') {
            if (s[2] == 'l') {
                if (s[3] == 'l') {
                    if (s[4] == 'p') {
                        if (s[5] == 't') {
                            if (s[6] == 'r') {
                                return Keyword_Ext_nullptr;
                            }
                        }
                    }
                }
            }
        }
    }
    else if (s[0] == 't') {
        if (s[1] == 'y') {
            if (s[2] == 'p') {
                if (s[3] == 'e') {
                    if (s[4] == 'd') {
                        if (s[5] == 'e') {
                            if (s[6] == 'f') {
                                return Keyword_typedef;
                            }
                        }
                    }
                }
            }
        }
    }
    else if (s[0] == 'w') {
        if (s[1] == 'c') {
            if (s[2] == 'h') {
                if (s[3] == 'a') {
                    if (s[4] == 'r') {
                        if (s[5] == '_') {
                            if (s[6] == 't') {
                                return Keyword_Ext_wchar_t;
                            }
                        }
                    }
                }
            }
        }
    }
    return IdentifierToken;
}

static inline SyntaxKind classify8(const char* s, const ParseOptions& opts)
{
    if (s[0] == '_') {
        if (s[1] == '_'
                && opts.extensions().isEnabled_ExtGNU_AlternateKeywords()) {
            if (s[2] == 'i') {
                if (s[3] == 'n') {
                    if (s[4] == 'l') {
                        if (s[5] == 'i') {
                            if (s[6] == 'n') {
                                if (s[7] == 'e') {
                                    return KeywordAlias___inline;
                                }
                            }
                        }
                    }
                }
                else if (s[3] == 'm'
                         && opts.extensions().isEnabled_ExtGNU_Complex()) {
                    if (s[4] == 'a') {
                        if (s[5] == 'g') {
                            if (s[6] == '_') {
                                if (s[7] == '_') {
                                    return Keyword_ExtGNU___imag__;
                                }
                            }
                        }
                    }
                }
            }
            else if (s[2] == 'f'
                     && opts.dialect().std() >= LanguageDialect::Std::C99) {
                if (s[3] == 'u') {
                    if (s[4] == 'n') {
                        if (s[5] == 'c') {
                            if (s[6] == '_') {
                                if (s[7] == '_') {
                                    return Keyword___func__;
                                }
                            }
                        }
                    }
                }
            }
            else if (s[2] == 't') {
                if (s[3] == 'y') {
                    if (s[4] == 'p') {
                        if (s[5] == 'e') {
                            if (s[6] == 'o') {
                                if (s[7] == 'f') {
                                    return KeywordAlias___typeof;
                                }
                            }
                        }
                    }
                }
                else if (s[3] == 'h') {
                    if (s[4] == 'r') {
                        if (s[5] == 'e') {
                            if (s[6] == 'a') {
                                if (s[7] == 'd') {
                                    return Keyword_ExtGNU___thread;
                                }
                            }
                        }
                    }
                }
            }
            else if (s[2] == 'r'
                     && opts.extensions().isEnabled_ExtGNU_Complex()) {
                if (s[3] == 'e') {
                    if (s[4] == 'a') {
                        if (s[5] == 'l') {
                            if (s[6] == '_') {
                                if (s[7] == '_') {
                                    return Keyword_ExtGNU___real__;
                                }
                            }
                        }
                    }
                }
            }
            else if (s[2] == 's'
                     && opts.extensions().isEnabled_ExtGNU_AlternateKeywords()) {
                if (s[3] == 'i') {
                    if (s[4] == 'g') {
                        if (s[5] == 'n') {
                            if (s[6] == 'e') {
                                if (s[7] == 'd') {
                                    return KeywordAlias___signed;
                                }
                            }
                        }
                    }
                }
            }
        }
        else if (s[1] == 'A'
                 && opts.dialect().std() >= LanguageDialect::Std::C11) {
            if (s[2] == 'l') {
                if (s[3] == 'i') {
                    if (s[4] == 'g') {
                        if (s[5] == 'n') {
                            if (s[6] == 'a') {
                                if (s[7] == 's') {
                                    return Keyword__Alignas;
                                }
                            }
                            else if (s[6] == 'o') {
                                if (s[7] == 'f') {
                                    return Keyword__Alignof;
                                }
                            }
                        }
                    }
                }
            }
        }
        else if (s[1] == 'C'
                    && opts.dialect().std() >= LanguageDialect::Std::C99) {
            if (s[2] == 'o') {
                if (s[3] == 'm') {
                    if (s[4] == 'p') {
                        if (s[5] == 'l') {
                            if (s[6] == 'e') {
                                if (s[7] == 'x') {
                                    return Keyword__Complex;
                                }
                            }
                        }
                    }
                }
            }
        }
        else if (s[1] == 'G'
                    && opts.dialect().std() >= LanguageDialect::Std::C11) {
            if (s[2] == 'e') {
                if (s[3] == 'n') {
                    if (s[4] == 'e') {
                        if (s[5] == 'r') {
                            if (s[6] == 'i') {
                                if (s[7] == 'c') {
                                    return Keyword__Generic;
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    else if (s[0] == 'c') {
        if (s[1] == 'o') {
            if (s[2] == 'n') {
                if (s[3] == 't') {
                    if (s[4] == 'i') {
                        if (s[5] == 'n') {
                            if (s[6] == 'u') {
                                if (s[7] == 'e') {
                                    return Keyword_continue;
                                }
                            }
                        }
                    }
                }
            }
        }
        else if (s[1] == 'h') {
            if (s[2] == 'a') {
                if (s[3] == 'r') {
                    if (s[4] == '1') {
                        if (s[5] == '6') {
                            if (s[6] == '_') {
                                if (s[7] == 't') {
                                    return Keyword_Ext_char16_t;
                                }
                            }
                        }
                    } else if (s[4] == '3') {
                        if (s[5] == '2') {
                            if (s[6] == '_') {
                                if (s[7] == 't') {
                                    return Keyword_Ext_char32_t;
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    else if (s[0] == 'o'
             && opts.extensions().translations().isEnabled_Translate_offsetof_AsKeyword()) {
        if (s[1] == 'f') {
            if (s[2] == 'f') {
                if (s[3] == 's') {
                    if (s[4] == 'e') {
                        if (s[5] == 't') {
                            if (s[6] == 'o') {
                                if (s[7] == 'f') {
                                    return Keyword_MacroStd_offsetof;
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    else if (s[0] == 'r') {
        if (s[1] == 'e') {
            if (s[2] == 'g') {
                if (s[3] == 'i') {
                    if (s[4] == 's') {
                        if (s[5] == 't') {
                            if (s[6] == 'e') {
                                if (s[7] == 'r') {
                                    return Keyword_register;
                                }
                            }
                        }
                    }
                }
            } else if (s[2] == 's') {
                if (s[3] == 't') {
                    if (s[4] == 'r') {
                        if (s[5] == 'i') {
                            if (s[6] == 'c') {
                                if (s[7] == 't') {
                                    return Keyword_restrict;
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    else if (s[0] == 'u') {
        if (s[1] == 'n') {
            if (s[2] == 's') {
                if (s[3] == 'i') {
                    if (s[4] == 'g') {
                        if (s[5] == 'n') {
                            if (s[6] == 'e') {
                                if (s[7] == 'd') {
                                    return Keyword_unsigned;
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    else if (s[0] == 'v') {
        if (s[1] == 'o') {
            if (s[2] == 'l') {
                if (s[3] == 'a') {
                    if (s[4] == 't') {
                        if (s[5] == 'i') {
                            if (s[6] == 'l') {
                                if (s[7] == 'e') {
                                    return Keyword_volatile;
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    return IdentifierToken;
}

static inline SyntaxKind classify9(const char* s, const ParseOptions& opts)
{
    if (s[0] == '_') {
        if (s[1] == 'N'
                && opts.dialect().std() >= LanguageDialect::Std::C11) {
            if (s[2] == 'o') {
                if (s[3] == 'r') {
                    if (s[4] == 'e') {
                        if (s[5] == 't') {
                            if (s[6] == 'u') {
                                if (s[7] == 'r') {
                                    if (s[8] == 'n') {
                                        return Keyword__Noreturn;
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
        if (s[1] == '_') {
            if (s[2] == 'c') {
                if (s[3] == 'o') {
                    if (s[4] == 'n') {
                        if (s[5] == 's') {
                            if (s[6] == 't') {
                                if (s[7] == '_') {
                                    if (s[8] == '_') {
                                        return KeywordAlias___const__;
                                    }
                                }
                            }
                        }
                    }
                }
            }
            else if (s[2] == 'a'
                     && opts.extensions().isEnabled_ExtGNU_AlternateKeywords()) {
                if (s[3] == 'l') {
                    if (s[4] == 'i') {
                        if (s[5] == 'g') {
                            if (s[6] == 'n') {
                                if (s[7] == 'o') {
                                    if (s[8] == 'f') {
                                        return KeywordAlias___alignof;
                                    }
                                }
                                else if (s[7] == 'a') {
                                    if (s[8] == 's') {
                                        return KeywordAlias___alignas;
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
        else if(s[1] == 'T'
                    && opts.extensions().isEnabled_ExtPSY_Generics()) {
            if (s[2] == 'e') {
                if (s[3] == 'm') {
                    if (s[4] == 'p') {
                        if (s[5] == 'l') {
                            if (s[6] == 'a') {
                                if (s[7] == 't') {
                                    if (s[8] == 'e') {
                                        return Keyword_ExtPSY__Template;
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    return IdentifierToken;
}

static inline SyntaxKind classify10(const char* s, const ParseOptions& opts)
{
    if (s[0] == '_') {
        if (s[1] == '_') {
            if (s[2] == 'i') {
                if (s[3] == 'n') {
                    if (s[4] == 'l') {
                        if (s[5] == 'i') {
                            if (s[6] == 'n') {
                                if (s[7] == 'e') {
                                    if (s[8] == '_') {
                                        if (s[9] == '_') {
                                            return KeywordAlias___inline__;
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
            else if (s[2] == 'r') {
                if (s[3] == 'e') {
                    if (s[4] == 's') {
                        if (s[5] == 't') {
                            if (s[6] == 'r') {
                                if (s[7] == 'i') {
                                    if (s[8] == 'c') {
                                        if (s[9] == 't') {
                                            return KeywordAlias___restrict;
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
            else if (s[2] == 't') {
                if (s[3] == 'y') {
                    if (s[4] == 'p') {
                        if (s[5] == 'e') {
                            if (s[6] == 'o') {
                                if (s[7] == 'f') {
                                    if (s[8] == '_') {
                                        if (s[9] == '_') {
                                            return Keyword_ExtGNU___typeof__;
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
            else if (s[2] == 's'
                        && opts.extensions().isEnabled_ExtGNU_AlternateKeywords()) {
                if (s[3] == 'i') {
                    if (s[4] == 'g') {
                        if (s[5] == 'n') {
                            if (s[6] == 'e') {
                                if (s[7] == 'd') {
                                    if (s[8] == '_') {
                                        if (s[9] == '_') {
                                            return KeywordAlias___signed__;
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
            else if (s[2] == 'v') {
                if (s[3] == 'o') {
                    if (s[4] == 'l') {
                        if (s[5] == 'a') {
                            if (s[6] == 't') {
                                if (s[7] == 'i') {
                                    if (s[8] == 'l') {
                                        if (s[9] == 'e') {
                                            return KeywordAlias___volatile;
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    return IdentifierToken;
}

static inline SyntaxKind classify11(const char* s, const ParseOptions& opts)
{
    if (s[0] == '_') {
        if (s[1] == '_') {
            if (s[2] == 'a') {
                if (s[3] == 't') {
                    if (s[4] == 't') {
                        if (s[5] == 'r') {
                            if (s[6] == 'i') {
                                if (s[7] == 'b') {
                                    if (s[8] == 'u') {
                                        if (s[9] == 't') {
                                            if (s[10] == 'e') {
                                                return KeywordAlias___attribute;
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
                else if (s[3] == 'l') {
                    if (s[4] == 'i') {
                        if (s[5] == 'g') {
                            if (s[6] == 'n') {
                                if (s[7] == 'o') {
                                    if (s[8] == 'f') {
                                        if (s[9] == '_') {
                                            if (s[10] == '_') {
                                                return KeywordAlias___alignof__;
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
            else if (s[2] == 'c'
                     && opts.extensions().isEnabled_ExtGNU_Complex()) {
                if (s[3] == 'o') {
                    if (s[4] == 'm') {
                        if (s[5] == 'p') {
                            if (s[6] == 'l') {
                                if (s[7] == 'e') {
                                    if (s[8] == 'x') {
                                        if (s[9] == '_') {
                                            if (s[10] == '_') {
                                                return Keyword_ExtGNU___complex__;
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    return IdentifierToken;
}

static inline SyntaxKind classify12(const char* s, const ParseOptions& opts)
{
    if (s[0] == '_') {
        if (s[1] == '_'
                && opts.extensions().isEnabled_ExtGNU_AlternateKeywords()) {
            if (s[2] == 'v') {
                if (s[3] == 'o') {
                    if (s[4] == 'l') {
                        if (s[5] == 'a') {
                            if (s[6] == 't') {
                                if (s[7] == 'i') {
                                    if (s[8] == 'l') {
                                        if (s[9] == 'e') {
                                            if (s[10] == '_') {
                                                if (s[11] == '_') {
                                                    return KeywordAlias___volatile__;
                                                }
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
            else if (s[2] == 'r') {
                if (s[3] == 'e') {
                    if (s[4] == 's') {
                        if (s[5] == 't') {
                            if (s[6] == 'r') {
                                if (s[7] == 'i') {
                                    if (s[8] == 'c') {
                                        if (s[9] == 't') {
                                            if (s[10] == '_') {
                                                if (s[11] == '_') {
                                                    return KeywordAlias___restrict__;
                                                }
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
            else if (s[2] == 'F'
                     && opts.extensions().isEnabled_ExtGNU_FunctionNames()) {
                if (s[3] == 'U') {
                    if (s[4] == 'N') {
                        if (s[5] == 'C') {
                            if (s[6] == 'T') {
                                if (s[7] == 'I') {
                                    if (s[8] == 'O') {
                                        if (s[9] == 'N') {
                                            if (s[10] == '_') {
                                                if (s[11] == '_') {
                                                    return Keyword_ExtGNU___FUNCTION__;
                                                }
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
        else if (s[1] == 't'
                 && opts.extensions().translations().isEnabled_Translate_thread_local_AsKeyword()) {
            if (s[2] == 'h') {
                if (s[3] == 'r') {
                    if (s[4] == 'e') {
                        if (s[5] == 'a') {
                            if (s[6] == 'd') {
                                if (s[7] == '_') {
                                    if (s[8] == 'l') {
                                        if (s[9] == 'o') {
                                            if (s[10] == 'c') {
                                                if (s[11] == 'a') {
                                                    if (s[12] == 'l') {
                                                        return Keyword__Thread_local;
                                                    }
                                                }
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    return IdentifierToken;
}

static inline SyntaxKind classify13(const char* s, const ParseOptions& opts)
{
    if (s[0] == '_') {
        if (s[1] == '_'
                && opts.extensions().isEnabled_ExtGNU_AlternateKeywords()) {
            if (s[2] == 'a') {
                if (s[3] == 't') {
                    if (s[4] == 't') {
                        if (s[5] == 'r') {
                            if (s[6] == 'i') {
                                if (s[7] == 'b') {
                                    if (s[8] == 'u') {
                                        if (s[9] == 't') {
                                            if (s[10] == 'e') {
                                                if (s[11] == '_') {
                                                    if (s[12] == '_') {
                                                        return Keyword_ExtGNU___attribute__;
                                                    }
                                                }
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
            else if (s[2] == 'e'
                     && opts.extensions().isEnabled_ExtGNU_AlternateKeywords()) {
                if (s[3] == 'x') {
                    if (s[4] == 't') {
                        if (s[5] == 'e') {
                            if (s[6] == 'n') {
                                if (s[7] == 's') {
                                    if (s[8] == 'i') {
                                        if (s[9] == 'o') {
                                            if (s[10] == 'n') {
                                                if (s[11] == '_') {
                                                    if (s[12] == '_') {
                                                        return Keyword_ExtGNU___extension__;
                                                    }
                                                }
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
        else if (s[1] == 'T'
                 && opts.dialect().std() >= LanguageDialect::Std::C11) {
            if (s[2] == 'h') {
                if (s[3] == 'r') {
                    if (s[4] == 'e') {
                        if (s[5] == 'a') {
                            if (s[6] == 'd') {
                                if (s[7] == '_') {
                                    if (s[8] == 'l') {
                                        if (s[9] == 'o') {
                                            if (s[10] == 'c') {
                                                if (s[11] == 'a') {
                                                    if (s[12] == 'l') {
                                                        return Keyword__Thread_local;
                                                    }
                                                }
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    return IdentifierToken;
}

static inline SyntaxKind classify14(const char* s, const ParseOptions& opts)
{
    if (s[0] == '_'
            && opts.dialect().std() >= LanguageDialect::Std::C11) {
        if (s[1] == 'S') {
            if (s[2] == 't') {
                if (s[3] == 'a') {
                    if (s[4] == 't') {
                        if (s[5] == 'i') {
                            if (s[6] == 'c') {
                                if (s[7] == '_') {
                                    if (s[8] == 'a') {
                                        if (s[9] == 's') {
                                            if (s[10] == 's') {
                                                if (s[11] == 'e') {
                                                    if (s[12] == 'r') {
                                                        if (s[13] == 't') {
                                                            return Keyword__Static_assert;
                                                        }
                                                    }
                                                }
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }

                }
            }
        }
    }
    return IdentifierToken;
}

static inline SyntaxKind classify15(const char* s, const ParseOptions& opts)
{
    return IdentifierToken;
}

static inline SyntaxKind classify16(const char* s, const ParseOptions& opts)
{
    if (s[0] == '_'
            && opts.extensions().isEnabled_ExtGNU_InternalBuiltins()) {
        if (s[1] == '_') {
            if (s[2] == 'b') {
                if (s[3] == 'u') {
                    if (s[4] == 'i') {
                        if (s[5] == 'l') {
                            if (s[6] == 't') {
                                if (s[7] == 'i') {
                                    if (s[8] == 'n') {
                                        if (s[9] == '_') {
                                            if (s[10] == 'v') {
                                                if (s[11] == 'a') {
                                                    if (s[12] == '_') {
                                                        if (s[13] == 'a') {
                                                            if (s[14] == 'r') {
                                                                if (s[15] == 'g') {
                                                                    return Keyword_ExtGNU___builtin_va_arg;
                                                                }
                                                            }
                                                        }
                                                    }
                                                }
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
        else if (s[1] == '_') {
            if (s[2] == 'b') {
                if (s[3] == 'u') {
                    if (s[4] == 'i') {
                        if (s[5] == 'l') {
                            if (s[6] == 't') {
                                if (s[7] == 'i') {
                                    if (s[8] == 'n') {
                                        if (s[9] == '_') {
                                            if (s[10] == 't') {
                                                if (s[11] == 'g') {
                                                    if (s[12] == 'm') {
                                                        if (s[13] == 'a') {
                                                            if (s[14] == 't') {
                                                                if (s[15] == 'h') {
                                                                    return Keyword_ExtGNU___builtin_tgmath;
                                                                }
                                                            }
                                                        }
                                                    }
                                                }
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    return IdentifierToken;
}

static inline SyntaxKind classify17(const char* s, const ParseOptions& opts)
{
    return IdentifierToken;
}

static inline SyntaxKind classify18(const char* s, const ParseOptions& opts)
{
    if (s[0] == '_'
            && opts.extensions().isEnabled_ExtGNU_InternalBuiltins()) {
        if (s[1] == '_') {
            if (s[2] == 'b') {
                if (s[3] == 'u') {
                    if (s[4] == 'i') {
                        if (s[5] == 'l') {
                            if (s[6] == 't') {
                                if (s[7] == 'i') {
                                    if (s[8] == 'n') {
                                        if (s[9] == '_') {
                                            if (s[10] == 'o') {
                                                if (s[11] == 'f') {
                                                    if (s[12] == 'f') {
                                                        if (s[13] == 's') {
                                                            if (s[14] == 'e') {
                                                                if (s[15] == 't') {
                                                                    if (s[16] == 'o') {
                                                                        if (s[17] == 'f') {
                                                                            return Keyword_ExtGNU___builtin_offsetof;
                                                                        }
                                                                    }
                                                                }
                                                            }
                                                        }
                                                    }
                                                }
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    return IdentifierToken;
}

static inline SyntaxKind classify19(const char* s, const ParseOptions& opts)
{
    if (s[0] == '_') {
        if (s[1] == '_') {
            if (s[2] == 'P'
                && opts.extensions().isEnabled_ExtGNU_FunctionNames()) {
                if (s[3] == 'R') {
                    if (s[4] == 'E') {
                        if (s[5] == 'T') {
                            if (s[6] == 'T') {
                                if (s[7] == 'Y') {
                                    if (s[8] == '_') {
                                        if (s[9] == 'F') {
                                            if (s[10] == 'U') {
                                                if (s[11] == 'N') {
                                                    if (s[12] == 'C') {
                                                        if (s[13] == 'T') {
                                                            if (s[14] == 'I') {
                                                                if (s[15] == 'O') {
                                                                    if (s[16] == 'N') {
                                                                        if (s[17] == '_') {
                                                                            if (s[18] == '_') {
                                                                                return Keyword_ExtGNU___PRETTY_FUNCTION__;
                                                                            }
                                                                        }
                                                                    }
                                                                }
                                                            }
                                                        }
                                                    }
                                                }
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }

    return IdentifierToken;
}

static inline SyntaxKind classify21(const char* s, const ParseOptions& opts)
{
    if (s[0] == '_') {
        if (s[1] == '_') {
            if (s[2] == 'b'
                && opts.extensions().isEnabled_ExtGNU_InternalBuiltins()) {
                if (s[3] == 'u') {
                    if (s[4] == 'i') {
                        if (s[5] == 'l') {
                            if (s[6] == 't') {
                                if (s[7] == 'i') {
                                    if (s[8] == 'n') {
                                        if (s[9] == '_') {
                                            if (s[10] == 'c') {
                                                if (s[11] == 'h') {
                                                    if (s[12] == 'o') {
                                                        if (s[13] == 'o') {
                                                            if (s[14] == 's') {
                                                                if (s[15] == 'e') {
                                                                    if (s[16] == '_') {
                                                                        if (s[17] == 'e') {
                                                                            if (s[18] == 'x') {
                                                                                if (s[19] == 'p') {
                                                                                    if (s[20] == 'r') {
                                                                                        return Keyword_ExtGNU___builtin_choose_expr;
                                                                                    }
                                                                                }
                                                                            }
                                                                        }
                                                                    }
                                                                }
                                                            }
                                                        }
                                                    }
                                                }
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    return IdentifierToken;
}

SyntaxKind Lexer::classify(const char* s, int n, const ParseOptions& opts)
{
    switch (n) {
        case 2: return classify2(s, opts);
        case 3: return classify3(s, opts);
        case 4: return classify4(s, opts);
        case 5: return classify5(s, opts);
        case 6: return classify6(s, opts);
        case 7: return classify7(s, opts);
        case 8: return classify8(s, opts);
        case 9: return classify9(s, opts);
        case 10: return classify10(s, opts);
        case 11: return classify11(s, opts);
        case 12: return classify12(s, opts);
        case 13: return classify13(s, opts);
        case 14: return classify14(s, opts);
        case 15: return classify15(s, opts);
        case 16: return classify16(s, opts);
        case 17: return classify17(s, opts);
        case 18: return classify18(s, opts);
        case 19: return classify19(s, opts);
        case 21: return classify21(s, opts);
        default: return IdentifierToken;
    }
}

static inline SyntaxKind classifyOperator2(const char* s)
{
    if (s[0] == 'o') {
        if (s[1] == 'r') {
            return OperatorName_ORToken;
        }
    }
    return IdentifierToken;
}

static inline SyntaxKind classifyOperator3(const char* s)
{
    if (s[0] == 'a') {
        if (s[1] == 'n') {
            if (s[2] == 'd') {
                return OperatorName_ANDToken;
            }
        }
    }
    else if (s[0] == 'n') {
        if (s[1] == 'o') {
            if (s[2] == 't') {
                return OperatorName_NOTToken;
            }
        }
    }
    else if (s[0] == 'x') {
        if (s[1] == 'o') {
            if (s[2] == 'r') {
                return OperatorName_XORToken;
            }
        }
    }
    return IdentifierToken;
}

static inline SyntaxKind classifyOperator5(const char* s)
{
    if (s[0] == 'b') {
        if (s[1] == 'i') {
            if (s[2] == 't') {
                if (s[3] == 'o') {
                    if (s[4] == 'r') {
                        return OperatorName_BITORToken;
                    }
                }
            }
        }
    }
    else if (s[0] == 'c') {
        if (s[1] == 'o') {
            if (s[2] == 'm') {
                if (s[3] == 'p') {
                    if (s[4] == 'l') {
                        return OperatorName_COMPLToken;
                    }
                }
            }
        }
    }
    else if (s[0] == 'o') {
        if (s[1] == 'r') {
            if (s[2] == '_') {
                if (s[3] == 'e') {
                    if (s[4] == 'q') {
                        return OperatorName_OREQToken;
                    }
                }
            }
        }
    }
    return IdentifierToken;
}

static inline SyntaxKind classifyOperator6(const char* s)
{
    if (s[0] == 'a') {
        if (s[1] == 'n') {
            if (s[2] == 'd') {
                if (s[3] == '_') {
                    if (s[4] == 'e') {
                        if (s[5] == 'q') {
                            return OperatorName_ANDEQToken;
                        }
                    }
                }
            }
        }
    }
    else if (s[0] == 'b') {
        if (s[1] == 'i') {
            if (s[2] == 't') {
                if (s[3] == 'a') {
                    if (s[4] == 'n') {
                        if (s[5] == 'd') {
                            return OperatorName_BITANDToken;
                        }
                    }
                }
            }
        }
    }
    else if (s[0] == 'n') {
        if (s[1] == 'o') {
            if (s[2] == 't') {
                if (s[3] == '_') {
                    if (s[4] == 'e') {
                        if (s[5] == 'q') {
                            return OperatorName_NOTEQToken;
                        }
                    }
                }
            }
        }
    }
    else if (s[0] == 'x') {
        if (s[1] == 'o') {
            if (s[2] == 'r') {
                if (s[3] == '_') {
                    if (s[4] == 'e') {
                        if (s[5] == 'q') {
                            return OperatorName_XOREQToken;
                        }
                    }
                }
            }
        }
    }
    return IdentifierToken;
}

SyntaxKind Lexer::classifyOperator(const char* s, int n, const ParseOptions& opts)
{
    if (!opts.extensions().translations().isEnabled_Translate_operatorNames())
        return IdentifierToken;

    switch (n) {
        case 2: return classifyOperator2(s);
        case 3: return classifyOperator3(s);
        case 5: return classifyOperator5(s);
        case 6: return classifyOperator6(s);
        default: return IdentifierToken;
    }
}

} // C
} // psy
