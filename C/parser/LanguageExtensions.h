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

#include "../common/infra/InternalAccess.h"

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
     * Whether to enable GNU keywords:
     *
     * https://gcc.gnu.org/onlinedocs/gcc/Alternate-Keywords.html
     */
    LanguageExtensions& enable_ExtGNU_AlternateKeywords(bool enable);
    bool isEnabled_ExtGNU_AlternateKeywords() const;
    //!@}

    //!@{
    /**
     * Whether to enable GNU attributes:
     *
     * https://gcc.gnu.org/onlinedocs/gcc/Attribute-Syntax.html
     */
    LanguageExtensions& enable_ExtGNU_AttributeSpecifiers(bool enable);
    bool isEnabled_ExtGNU_AttributeSpecifiers() const;
    //!@}

    //!@{
    /**
     * Whether to enable GNU's alignment:
     *
     * https://gcc.gnu.org/onlinedocs/gcc/Alignment.html
     */
    LanguageExtensions& enable_ExtGNU_Alignment(bool enable);
    bool isEnabled_ExtGNU_Alignment() const;
    //!@}

    //!@{
    /**
     * Whether to enable GNU compound literals.
     *
     * https://gcc.gnu.org/onlinedocs/gcc/Compound-Literals.html
     */
    LanguageExtensions& enable_ExtGNU_CompoundLiterals(bool enable);
    bool isEnabled_ExtGNU_CompoundLiterals() const;
    //!@}

    //!@{
    /**
     * Whether to enable GNU conditionals.
     *
     * https://gcc.gnu.org/onlinedocs/gcc/Conditionals.html
     */
    LanguageExtensions& enable_ExtGNU_Conditionals(bool enable);
    bool isEnabled_ExtGNU_Conditionals() const;
    //!@}

    //!@{
    /**
     * Whether to enable GNU designated initializers:
     *
     * https://gcc.gnu.org/onlinedocs/gcc/Designated-Inits.html
     */
    LanguageExtensions& enable_ExtGNU_DesignatedInitializers(bool enable);
    bool isEnabled_ExtGNU_DesignatedInitializers() const;
    //!@}

    //!@{
    /**
     * Whether to enable GNU function names as strings:
     *
     * https://gcc.gnu.org/onlinedocs/gcc/Function-Names.html
     */
    LanguageExtensions& enable_ExtGNU_FunctionNames(bool enable);
    bool isEnabled_ExtGNU_FunctionNames() const;
    //!@}

    //!@{
    /**
     * Whether to enable GNU complex numbers extension:
     *
     * https://gcc.gnu.org/onlinedocs/gcc/Complex.html
     */
    LanguageExtensions& enable_ExtGNU_Complex(bool enable);
    bool isEnabled_ExtGNU_Complex() const;
    //!@}

    //!@{
    /**
     * Whether to enable GNU statement expressions.
     *
     * https://gcc.gnu.org/onlinedocs/gcc/Statement-Exprs.html
     */
    LanguageExtensions& enable_ExtGNU_StatementExpressions(bool enable);
    bool isEnabled_ExtGNU_StatementExpressions() const;
    //!@}

    //!@{
    /**
     * Whether to enable GNU assembly in C.
     *
     * https://gcc.gnu.org/onlinedocs/gcc/Using-Assembly-Language-with-C.html
     */
    LanguageExtensions& enable_ExtGNU_Asm(bool enable);
    bool isEnabled_ExtGNU_Asm() const;
    //!@}

    //!@{
    /**
     * Whether to enable GNU internal builtins (with the \c __builtin prefix).
     *
     * https://gcc.gnu.org/onlinedocs/gcc/Variadic-Macros.html
     * https://gcc.gnu.org/onlinedocs/gcc/Offsetof.html
     * https://gcc.gnu.org/onlinedocs/gcc/Other-Builtins.html
     */
    LanguageExtensions& enable_ExtGNU_InternalBuiltins(bool enable);
    bool isEnabled_ExtGNU_InternalBuiltins() const;
    //!@}

    //!@{
    /**
     * Whether to enable LLVM/Clang extensions of GNU attribute:
     *
     * https://clang.llvm.org/docs/AttributeReference.html#availability
     */
    LanguageExtensions& enable_ExtGNU_AttributeSpecifiersLLVM(bool enable);
    bool isEnabled_ExtGNU_AttributeSpecifiersLLVM() const;
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
    LanguageExtensions& enable_NativeBooleans(bool allow);
    bool isEnabled_NativeBooleans() const;
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
    LanguageExtensions& enable_ExtPSY_Generics(bool yes);
    bool isEnabled_ExtPSY_Generics() const;
    //!@}

private:
    MacroTranslations translations_;

    struct BitFields
    {
        /* GNU */
        std::uint64_t ExtGNU_AlternateKeywords_ : 1;
        std::uint64_t ExtGNU_Asm_: 1;
        std::uint64_t ExtGNU_AttributeSpecifiers_ : 1;
        std::uint64_t ExtGNU_AttributeSpecifiersLLVM_ : 1;
        std::uint64_t ExtGNU_Alignment_ : 1;
        std::uint64_t ExtGNU_CompoundLiterals_ : 1;
        std::uint64_t ExtGNU_Conditionals_ : 1;
        std::uint64_t ExtGNU_DesignatedInitializers_ : 1;
        std::uint64_t ExtGNU_StatementExpressions_ : 1;
        std::uint64_t ExtGNU_InternalBuiltins_ : 1;
        std::uint64_t ExtGNU_FunctionNames_ : 1;
        std::uint64_t ExtGNU_Complex_ : 1;

        /* Psyche */
        std::uint64_t ExtPSY_Generics_ : 1;

        /* C++ */
        std::uint64_t CPP_nullptr_ : 1;

        /* Custom */
        std::uint64_t NativeBooleans_ : 1;
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
