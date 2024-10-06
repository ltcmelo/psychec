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

#ifndef PSYCHE_C_LANGUAGE_EXTENSIONS_H__
#define PSYCHE_C_LANGUAGE_EXTENSIONS_H__

#include "API.h"

#include "MacroTranslations.h"

#include "../common/infra/AccessSpecifiers.h"

#include <cstdint>
#include <string>

namespace psy {
namespace C {

/**
 * \brief The LanguageExtensions class.
 */
class PSY_C_API LanguageExtensions final
{
public:
    //!@{
    /**
     * Create LanguageExtensions.
     */
    LanguageExtensions();
    LanguageExtensions(MacroTranslations translations);
    //!@}

    /**
     * The MacroTranslations of \c this LanguageExtensions.
     */
    const MacroTranslations& translations() const;

    /**
     * \brief The C language extensions enumeration.
     */
    enum class Ext : std::uint8_t
    {
        /* C */
        C_wchar_t_Keyword,
        C_char8_t_Keyword,
        C_char16_t_Keyword,
        C_char32_t_Keyword,

        /* GNU */
        GNU_AlternateKeywords,
        GNU_AttributeSpecifiers,
        GNU_Alignment,
        GNU_CompoundLiterals,
        GNU_Conditionals,
        GNU_DesignatedInitializers,
        GNU_FunctionNames,
        GNU_Complex,
        GNU_StatementExpressions,
        GNU_Asm,
        GNU_InternalBuiltins,
        GNU_AttributeSpecifiersLLVM,

        /* Psyche */
        PSY_Generics,

        /* C++ */
        CPP_nullptr,

        /* Custom */
        NativeBooleans,
        NULLAsBuiltin,
    };

    //!@{
    /**
     * Whether to enable \c wchar_t as a keyword.
     */
    LanguageExtensions& enable_extC_wchar_t_Keyword(bool enable);
    bool isEnabled_extC_wchar_t_Keyword() const;
    //!@}

    //!@{
    /**
     * Whether to enable \c char8_t as a keyword.
     */
    LanguageExtensions& enable_extC_char8_t_Keyword(bool enable);
    bool isEnabled_extC_char8_t_Keyword() const;
    //!@}

    //!@{
    /**
     * Whether to enable \c char16_t as a keyword.
     */
    LanguageExtensions& enable_extC_char16_t_Keyword(bool enable);
    bool isEnabled_extC_char16_t_Keyword() const;
    //!@}

    //!@{
    /**
     * Whether to enable \c char32_t as a keyword.
     */
    LanguageExtensions& enable_extC_char32_t_Keyword(bool enable);
    bool isEnabled_extC_char32_t_Keyword() const;
    //!@}

    //!@{
    /**
     * Whether to enable GNU keywords:
     *
     * https://gcc.gnu.org/onlinedocs/gcc/Alternate-Keywords.html
     */
    LanguageExtensions& enable_extGNU_AlternateKeywords(bool enable);
    bool isEnabled_extGNU_AlternateKeywords() const;
    //!@}

    //!@{
    /**
     * Whether to enable GNU attributes:
     *
     * https://gcc.gnu.org/onlinedocs/gcc/Attribute-Syntax.html
     */
    LanguageExtensions& enable_extGNU_AttributeSpecifiers(bool enable);
    bool isEnabled_extGNU_AttributeSpecifiers() const;
    //!@}

    //!@{
    /**
     * Whether to enable GNU's alignment:
     *
     * https://gcc.gnu.org/onlinedocs/gcc/Alignment.html
     */
    LanguageExtensions& enable_extGNU_Alignment(bool enable);
    bool isEnabled_extGNU_Alignment() const;
    //!@}

    //!@{
    /**
     * Whether to enable GNU compound literals.
     *
     * https://gcc.gnu.org/onlinedocs/gcc/Compound-Literals.html
     */
    LanguageExtensions& enable_extGNU_CompoundLiterals(bool enable);
    bool isEnabled_extGNU_CompoundLiterals() const;
    //!@}

    //!@{
    /**
     * Whether to enable GNU conditionals.
     *
     * https://gcc.gnu.org/onlinedocs/gcc/Conditionals.html
     */
    LanguageExtensions& enable_extGNU_Conditionals(bool enable);
    bool isEnabled_extGNU_Conditionals() const;
    //!@}

    //!@{
    /**
     * Whether to enable GNU designated initializers:
     *
     * https://gcc.gnu.org/onlinedocs/gcc/Designated-Inits.html
     */
    LanguageExtensions& enable_extGNU_DesignatedInitializers(bool enable);
    bool isEnabled_extGNU_DesignatedInitializers() const;
    //!@}

    //!@{
    /**
     * Whether to enable GNU function names as strings:
     *
     * https://gcc.gnu.org/onlinedocs/gcc/Function-Names.html
     */
    LanguageExtensions& enable_extGNU_FunctionNames(bool enable);
    bool isEnabled_extGNU_FunctionNames() const;
    //!@}

    //!@{
    /**
     * Whether to enable GNU complex numbers extension:
     *
     * https://gcc.gnu.org/onlinedocs/gcc/Complex.html
     */
    LanguageExtensions& enable_extGNU_Complex(bool enable);
    bool isEnabled_extGNU_Complex() const;
    //!@}

    //!@{
    /**
     * Whether to enable GNU statement expressions.
     *
     * https://gcc.gnu.org/onlinedocs/gcc/Statement-Exprs.html
     */
    LanguageExtensions& enable_extGNU_StatementExpressions(bool enable);
    bool isEnabled_extGNU_StatementExpressions() const;
    //!@}

    //!@{
    /**
     * Whether to enable GNU assembly in C.
     *
     * https://gcc.gnu.org/onlinedocs/gcc/Using-Program-Language-with-C.html
     */
    LanguageExtensions& enable_extGNU_Asm(bool enable);
    bool isEnabled_extGNU_Asm() const;
    //!@}

    //!@{
    /**
     * Whether to enable GNU internal builtins (with the \c __builtin prefix).
     *
     * https://gcc.gnu.org/onlinedocs/gcc/Variadic-Macros.html
     * https://gcc.gnu.org/onlinedocs/gcc/Offsetof.html
     * https://gcc.gnu.org/onlinedocs/gcc/Other-Builtins.html
     */
    LanguageExtensions& enable_extGNU_InternalBuiltins(bool enable);
    bool isEnabled_extGNU_InternalBuiltins() const;
    //!@}

    //!@{
    /**
     * Whether to enable LLVM/Clang extensions of GNU attribute:
     *
     * https://clang.llvm.org/docs/AttributeReference.html#availability
     */
    LanguageExtensions& enable_extGNU_AttributeSpecifiersLLVM(bool enable);
    bool isEnabled_extGNU_AttributeSpecifiersLLVM() const;
    //!@}

    //!@{
    /**
     * Whether to recognize the \c nullptr pointer literal from C++.
     */
    LanguageExtensions& enable_CPP_nullptr(bool recognize);
    bool isEnabled_CPP_nullptr() const;
    //!@}

    //!@{
    /**
     * Whether to allow boolean types natively (even in non-C99 standard),
     * including keywords \c bool, \c false, and \c true,
     */
    LanguageExtensions& enable_nativeBooleans(bool allow);
    bool isEnabled_nativeBooleans() const;
    //!@}

    //!@{
    /**
     * Whether to threat \c NULL as a builtin.
     */
    LanguageExtensions& enable_NULLAsBuiltin(bool treat);
    bool isEnabled_NULLAsBuiltin() const;
    //!@}

    //!@{
    /**
     * Whether to set experimental generics from Psyche.
     */
    LanguageExtensions& enable_extPSY_Generics(bool yes);
    bool isEnabled_extPSY_Generics() const;
    //!@}

private:
    MacroTranslations translations_;

    struct BitFields
    {
        /* C */
        std::uint64_t extC_wchar_t_Keyword_ : 1;
        std::uint64_t extC_char8_t_Keyword_ : 1;
        std::uint64_t extC_char16_t_Keyword_ : 1;
        std::uint64_t extC_char32_t_Keyword_ : 1;

        /* GNU */
        std::uint64_t extGNU_AlternateKeywords_ : 1;
        std::uint64_t extGNU_Asm_: 1;
        std::uint64_t extGNU_AttributeSpecifiers_ : 1;
        std::uint64_t extGNU_AttributeSpecifiersLLVM_ : 1;
        std::uint64_t extGNU_Alignment_ : 1;
        std::uint64_t extGNU_CompoundLiterals_ : 1;
        std::uint64_t extGNU_Conditionals_ : 1;
        std::uint64_t extGNU_DesignatedInitializers_ : 1;
        std::uint64_t extGNU_StatementExpressions_ : 1;
        std::uint64_t extGNU_InternalBuiltins_ : 1;
        std::uint64_t extGNU_FunctionNames_ : 1;
        std::uint64_t extGNU_Complex_ : 1;

        /* Psyche */
        std::uint64_t extPSY_Generics_ : 1;

        /* C++ */
        std::uint64_t CPP_nullptr_ : 1;

        /* Custom */
        std::uint64_t nativeBooleans_ : 1;
        std::uint64_t NULLAsBuiltin_ : 1;
    };
    union
    {
        std::uint64_t BF_all_;
        BitFields BF_;
    };
};

std::string PSY_C_API to_string(LanguageExtensions::Ext ext);

} // C
} // psy

#endif
