// Copyright (c) 2016/17/18/19/20/21/22 Leandro T. C. Melo <ltcmelo@gmail.com>
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

#ifndef PSYCHE_C_MACRO_TRANSLATIONS_H__
#define PSYCHE_C_MACRO_TRANSLATIONS_H__

#include "API.h"

#include "../common/infra/InternalAccess.h"

#include <cstdint>

namespace psy {
namespace C {

/**
 * \brief The MacroTranslations class.
 */
class PSY_C_API MacroTranslations final
{
public:
    //!@{
    /**
     * Create MacroTranslations.
     */
    MacroTranslations();
    //!@}

    //!@{
    /**
     * Whether to translate macro \c static_assert as \c _Static_assert.
     * From header <assert.h>.
     *
     * \remark 7.2
     */
    MacroTranslations& enable_Translate_static_assert_AsKeyword(bool expand);
    bool isEnabled_Translate_static_assert_AsKeyword() const;
    //!@}

    //!@{
    /**
     * Whether to translate macro \c complex as \c _Complex.
     * From header <complex.h>.
     *
     * \remark 7.3
     */
    MacroTranslations& enable_Translate_complex_AsKeyword(bool expand);
    bool isEnabled_Translate_complex_AsKeyword() const;
    //!@}

    //!@{
    /**
     * Whether to translate operators names.
     * From header <iso646.h>.
     *
     * \remark 7.9
     */
    MacroTranslations& enable_Translate_operatorNames(bool expand);
    bool isEnabled_Translate_operatorNames() const;
    //!@}

    //!@{
    /**
     * Whether to translate macro \c alignas as \c _Alignas.
     * From header <stdalign.h>.
     *
     * \remark 7.15
     */
    MacroTranslations& enable_Translate_alignas_AsKeyword(bool expand);
    bool isEnabled_Translate_alignas_AsKeyword() const;
    //!@}

    //!@{
    /**
     * Whether to translate macro \c alignof as \c _Alignof.
     * From header <stdalign.h>.
     *
     * \remark 7.15
     */
    MacroTranslations& enable_Translate_alignof_AsKeyword(bool expand);
    bool isEnabled_Translate_alignof_AsKeyword() const;
    //!@}

    //!@{
    /**
     * Whether to translate macro \c va_arg as an internal builtin.
     * From header <stdarg.h>.
     *
     * \remark 7.16
     */
    MacroTranslations& enable_Translate_va_arg_AsKeyword(bool expand);
    bool isEnabled_Translate_va_arg_AsKeyword() const;
    //!@}

    //!@{
    /**
     * Whether to translate macro \c offsetof as an internal builtin.
     * From header <stddef.h>
     *
     * \remark 7.19
     */
    MacroTranslations& enable_Translate_offsetof_AsKeyword(bool expand);
    bool isEnabled_Translate_offsetof_AsKeyword() const;
    //!@}

    //!@{
    /**
     * Whether to translate macro \c bool as \c _Bool.
     * From header <stdbool.h>.
     *
     * \remark 7.18
     */
    MacroTranslations& enable_Translate_bool_AsKeyword(bool expand);
    bool isEnabled_Translate_bool_AsKeyword() const;
    //!@}

    //!@{
    /**
     * Whether to translate macro \c thread_local as \c _Thread_local.
     * From header <threads.h>.
     *
     * \remark 7.26
     */
    MacroTranslations& enable_Translate_thread_local_AsKeyword(bool expand);
    bool isEnabled_Translate_thread_local_AsKeyword() const;
    //!@}

private:
    struct BitFields
    {
        std::uint64_t Translate_static_assert_AsKeyword_: 1;
        std::uint64_t Translate_complex_AsKeyword_: 1;
        std::uint64_t Translate_operatorNames_ : 1;
        std::uint64_t Translate_alignas_AsKeyword_ : 1;
        std::uint64_t Translate_alignof_AsKeyword_ : 1;
        std::uint64_t Translate_va_arg_AsKeyword_ : 1;
        std::uint64_t Translate_offsetof_AsKeyword_ : 1;
        std::uint64_t Translate_bool_AsKeyword_ : 1;
        std::uint64_t Translate_thread_local_AsKeyword_ : 1;
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
