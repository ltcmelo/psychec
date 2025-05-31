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

#include "LanguageExtensions.h"

#include "common/infra/Assertions.h"

#include <utility>

using namespace psy;
using namespace C;

LanguageExtensions::LanguageExtensions()
    : LanguageExtensions(MacroTranslations())
{}

LanguageExtensions::LanguageExtensions(MacroTranslations translations)
    : translations_(std::move(translations))
    , BD_(~0)
{
    /* C */
    F_.extC_KandRStyle_ = false;
    F_.extC_wchar_t_Keyword_ = false;
    F_.extC_char8_t_Keyword_ = false;
    F_.extC_char16_t_Keyword_ = false;
    F_.extC_char32_t_Keyword_ = false;

    /* C++ */
    F_.CPP_nullptr_ = false;

    /* Custom */
    F_.nativeBooleans_ = false;
}

const MacroTranslations& LanguageExtensions::translations() const
{
    return translations_;
}

#define DEFINE_ENABLE_ISENABLED(FLAG) \
    LanguageExtensions& LanguageExtensions::enable_##FLAG(bool enable) \
        { F_.FLAG##_ = enable; return *this; } \
    bool LanguageExtensions::isEnabled_##FLAG() const \
        { return F_.FLAG##_; }

DEFINE_ENABLE_ISENABLED(extC_KandRStyle)
DEFINE_ENABLE_ISENABLED(extC_wchar_t_Keyword)
DEFINE_ENABLE_ISENABLED(extC_char8_t_Keyword)
DEFINE_ENABLE_ISENABLED(extC_char16_t_Keyword)
DEFINE_ENABLE_ISENABLED(extC_char32_t_Keyword)
DEFINE_ENABLE_ISENABLED(extGNU_AlternateKeywords)
DEFINE_ENABLE_ISENABLED(extGNU_Asm)
DEFINE_ENABLE_ISENABLED(extGNU_AttributeSpecifiers)
DEFINE_ENABLE_ISENABLED(extGNU_AttributeSpecifiersLLVM)
DEFINE_ENABLE_ISENABLED(extGNU_Alignment)
DEFINE_ENABLE_ISENABLED(extGNU_CompoundLiterals)
DEFINE_ENABLE_ISENABLED(extGNU_Conditionals)
DEFINE_ENABLE_ISENABLED(extGNU_DesignatedInitializers)
DEFINE_ENABLE_ISENABLED(extGNU_FunctionNames)
DEFINE_ENABLE_ISENABLED(extGNU_Complex)
DEFINE_ENABLE_ISENABLED(extGNU_StatementExpressions)
DEFINE_ENABLE_ISENABLED(extGNU_InternalBuiltins)
DEFINE_ENABLE_ISENABLED(extGNU_LabelsAsValues)

DEFINE_ENABLE_ISENABLED(extPSY_Generics)

DEFINE_ENABLE_ISENABLED(CPP_nullptr)

DEFINE_ENABLE_ISENABLED(nativeBooleans)
DEFINE_ENABLE_ISENABLED(NULLAsBuiltin)

#undef DEFINE_ENABLE_ISENABLED

namespace psy {
namespace C {

#define OSTREAM_FLAG(FLAG) \
    os << #FLAG << ":" << langExts.F_.FLAG << " "

std::ostream& operator<<(std::ostream& os, const LanguageExtensions& langExts)
{
    OSTREAM_FLAG(extC_KandRStyle_);
    OSTREAM_FLAG(extC_wchar_t_Keyword_);
    OSTREAM_FLAG(extC_char8_t_Keyword_);
    OSTREAM_FLAG(extC_char16_t_Keyword_);
    OSTREAM_FLAG(extC_char32_t_Keyword_);
    OSTREAM_FLAG(extGNU_AlternateKeywords_);
    OSTREAM_FLAG(extGNU_Asm_);
    OSTREAM_FLAG(extGNU_AttributeSpecifiers_);
    OSTREAM_FLAG(extGNU_AttributeSpecifiersLLVM_);
    OSTREAM_FLAG(extGNU_Alignment_);
    OSTREAM_FLAG(extGNU_CompoundLiterals_);
    OSTREAM_FLAG(extGNU_Conditionals_);
    OSTREAM_FLAG(extGNU_DesignatedInitializers_);
    OSTREAM_FLAG(extGNU_StatementExpressions_);
    OSTREAM_FLAG(extGNU_InternalBuiltins_);
    OSTREAM_FLAG(extGNU_LabelsAsValues_);
    OSTREAM_FLAG(extGNU_FunctionNames_);
    OSTREAM_FLAG(extGNU_Complex_);
    OSTREAM_FLAG(CPP_nullptr_);
    OSTREAM_FLAG(nativeBooleans_);
    OSTREAM_FLAG(NULLAsBuiltin_);
    return os;
}

PSY_C_API std::string to_string(LanguageExtensions::Ext ext)
{
    switch (ext) {
        /* C */
    case LanguageExtensions::Ext::C_KandRStyle:
        return "C K&R style";
    case LanguageExtensions::Ext::C_wchar_t_Keyword:
        return "wchar_t as keyword";
    case LanguageExtensions::Ext::C_char8_t_Keyword:
        return "char8_t as keyword";
    case LanguageExtensions::Ext::C_char16_t_Keyword:
        return "char16_t as keyword";
    case LanguageExtensions::Ext::C_char32_t_Keyword:
        return "char32_t as keyword";

        /* GNU */
    case LanguageExtensions::Ext::GNU_AlternateKeywords:
        return "GNU Alternate Keywords";
    case LanguageExtensions::Ext::GNU_Asm:
        return "GNU Asm";
    case LanguageExtensions::Ext::GNU_AttributeSpecifiers:
        return "GNU Attribute Specifiers";
    case LanguageExtensions::Ext::GNU_AttributeSpecifiersLLVM:
        return "GNU Attribute Specifiers LLVM";
    case LanguageExtensions::Ext::GNU_Alignment:
        return "GNU Alignment";
    case LanguageExtensions::Ext::GNU_CompoundLiterals:
        return "GNU Compound Literals";
    case LanguageExtensions::Ext::GNU_Conditionals:
        return "GNU Conditionals";
    case LanguageExtensions::Ext::GNU_DesignatedInitializers:
        return "GNU Designated Initializers";
    case LanguageExtensions::Ext::GNU_StatementExpressions:
        return "GNU Statement Expressions";
    case LanguageExtensions::Ext::GNU_InternalBuiltins:
        return "GNU Internal Builtins";
    case LanguageExtensions::Ext::GNU_LabelsAsValues:
        return "GNU Label as Values";
    case LanguageExtensions::Ext::GNU_FunctionNames:
        return "GNU Function Names";
    case LanguageExtensions::Ext::GNU_Complex:
        return "GNU Complex";

        /* Psyche */
    case LanguageExtensions::Ext::PSY_Generics:
        return "PSY Generics";

        /* C++ */
    case LanguageExtensions::Ext::CPP_nullptr:
        return "CPP nullptr";

        /* Custom */
    case LanguageExtensions::Ext::NativeBooleans:
        return "Native Booleans";
    case LanguageExtensions::Ext::NULLAsBuiltin:
        return "NULL As Builtin";
    }
    PSY_ASSERT_1(false);
    return "<invalid LanguageExtensions::Ext >";
}

} // C
} // psy
