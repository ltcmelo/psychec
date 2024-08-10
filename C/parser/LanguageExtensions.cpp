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
    , BF_all_(~0)
{
    /* Psyche */
    BF_.extPSY_Generics_ = false;

    /* C++ */
    BF_.CPP_nullptr_ = false;

    /* Custom */
    BF_.nativeBooleans_ = false;
}

const MacroTranslations& LanguageExtensions::translations() const
{
    return translations_;
}

#define DEFINE_ENABLE_ISENABLED(FLAG) \
    LanguageExtensions& LanguageExtensions::enable_##FLAG(bool enable) \
        { BF_.FLAG##_ = enable; return *this; } \
    bool LanguageExtensions::isEnabled_##FLAG() const \
        { return BF_.FLAG##_; }

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

DEFINE_ENABLE_ISENABLED(extPSY_Generics)

DEFINE_ENABLE_ISENABLED(CPP_nullptr)

DEFINE_ENABLE_ISENABLED(nativeBooleans)
DEFINE_ENABLE_ISENABLED(NULLAsBuiltin)

#undef DEFINE_ENABLE_ISENABLED

namespace psy {
namespace C {

std::string PSY_C_API to_string(LanguageExtensions::Ext ext)
{
    switch (ext) {
        /* GNU */
    case LanguageExtensions::Ext::GNU_AlternateKeywords:
        return "GNU Alternate Keywords";

    case LanguageExtensions::Ext::GNU_AttributeSpecifiers:
        return "GNU Attribute Specifiers";

    case LanguageExtensions::Ext::GNU_Alignment:
        return "GNU Alignment";

    case LanguageExtensions::Ext::GNU_CompoundLiterals:
        return "GNU Compound Literals";

    case LanguageExtensions::Ext::GNU_Conditionals:
        return "GNU Conditionals";

    case LanguageExtensions::Ext::GNU_DesignatedInitializers:
        return "GNU Designated Initializers";

    case LanguageExtensions::Ext::GNU_FunctionNames:
        return "GNU Function Names";

    case LanguageExtensions::Ext::GNU_Complex:
        return "GNU Complex";

    case LanguageExtensions::Ext::GNU_StatementExpressions:
        return "GNU Statement Expressions";

    case LanguageExtensions::Ext::GNU_Asm:
        return "GNU Asm";

    case LanguageExtensions::Ext::GNU_InternalBuiltins:
        return "GNU Internal Builtins";

    case LanguageExtensions::Ext::GNU_AttributeSpecifiersLLVM:
        return "GNU Attribute Specifiers LLVM";

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

    default:
        PSY_ASSERT_3(false, return "", "");
    }
}

} // C
} // psy
