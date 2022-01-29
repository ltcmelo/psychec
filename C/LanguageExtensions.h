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

#ifndef PSYCHE_C_LANGUAGE_EXTENSIONS_H__
#define PSYCHE_C_LANGUAGE_EXTENSIONS_H__

#include <cstdint>

namespace psy {
namespace C {

/**
 * \brief The LanguageExtensions class.
 */
class LanguageExtensions final
{
public:
    LanguageExtensions();

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
     * Whether to automatically expand macro \c static_assert to \c _Static_assert.
     *
     * From header <assert.h>.
     *
     * \remark 7.2
     */
    LanguageExtensions& enable_Expand_static_assert_AsKeyword(bool expand);
    bool isEnabled_Expand_static_assert_AsKeyword() const;
    //!@}

    //!@{
    /**
     * Whether to automatically expand macro \c complex to \c _Complex.
     *
     * From header <complex.h>.
     *
     * \remark 7.3
     */
    LanguageExtensions& enable_Expand_complex_AsKeyword(bool expand);
    bool isEnabled_Expand_complex_AsKeyword() const;
    //!@}

    //!@{
    /**
     * Whether to automatically expand operators names.
     *
     * From header <iso646.h>.
     *
     * \remark 7.9
     */
    LanguageExtensions& enable_Expand_operatorNames(bool expand);
    bool isEnabled_Expand_operatorNames() const;
    //!@}

    //!@{
    /**
     * Whether to automatically expand macro \c alignas to \c _Alignas.
     *
     * From header <stdalign.h>.
     *
     * \remark 7.15
     */
    LanguageExtensions& enable_Expand_alignas_AsKeyword(bool expand);
    bool isEnabled_Expand_alignas_AsKeyword() const;
    //!@}

    //!@{
    /**
     * Whether to automatically expand macro \c alignof to \c _Alignof.
     *
     * From header <stdalign.h>.
     *
     * \remark 7.15
     */
    LanguageExtensions& enable_Expand_alignof_AsKeyword(bool expand);
    bool isEnabled_Expand_alignof_AsKeyword() const;
    //!@}

    //!@{
    /**
     * Whether to automatically expand macro \c bool to \c _Bool.
     *
     * From header <stdbool.h>.
     *
     * \remark 7.18
     */
    LanguageExtensions& enable_Expand_bool_AsKeyword(bool expand);
    bool isEnabled_Expand_bool_AsKeyword() const;
    //!@}

    //!@{
    /**
     * Whether to automatically expand macro \c thread_local to \c _Thread_local.
     *
     * From header <threads.h>.
     *
     * \remark 7.26
     */
    LanguageExtensions& enable_Expand_thread_local_AsKeyword(bool expand);
    bool isEnabled_Expand_thread_local_AsKeyword() const;
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

        /* Psyche */
        std::uint64_t ExtPSY_Generics_ : 1;

        /* Macros */
        std::uint64_t Expand_static_assert_AsKeyword_: 1;
        std::uint64_t Expand_complex_AsKeyword_: 1;
        std::uint64_t Expand_operatorNames_ : 1;
        std::uint64_t Expand_alignas_AsKeyword_ : 1;
        std::uint64_t Expand_alignof_AsKeyword_ : 1;
        std::uint64_t Expand_bool_AsKeyword_ : 1;
        std::uint64_t Expand_thread_local_AsKeyword_ : 1;

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

} // C
} // psy

#endif
