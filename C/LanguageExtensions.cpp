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

#include "LanguageExtensions.h"

#define DEFINE_ENABLE_ISENABLED(FLAG) \
    LanguageExtensions& LanguageExtensions::enable_##FLAG(bool enable) \
        { BF_.FLAG##_ = enable; return *this; } \
    bool LanguageExtensions::isEnabled_##FLAG() const \
        { return BF_.FLAG##_; }

using namespace psy;
using namespace C;

LanguageExtensions::LanguageExtensions()
    : BF_all_(~0)
{
    /* Psyche */
    BF_.ExtPSY_Generics_ = false;

    /* Macros */
    BF_.Expand_static_assert_AsKeyword_ = false;
    BF_.Expand_complex_AsKeyword_ = false;
    BF_.Expand_operatorNames_ = false;
    BF_.Expand_alignas_AsKeyword_ = false;
    BF_.Expand_alignof_AsKeyword_ = false;
    BF_.Expand_bool_AsKeyword_ = false;
    BF_.Expand_thread_local_AsKeyword_ = false;

    /* C++ */
    BF_.CPP_nullptr_ = false;

    /* Custom */
    BF_.NativeBooleans_ = false;
}

DEFINE_ENABLE_ISENABLED(ExtGNU_AlternateKeywords)
DEFINE_ENABLE_ISENABLED(ExtGNU_Asm)
DEFINE_ENABLE_ISENABLED(ExtGNU_AttributeSpecifiers)
DEFINE_ENABLE_ISENABLED(ExtGNU_AttributeSpecifiersLLVM)
DEFINE_ENABLE_ISENABLED(ExtGNU_Alignment)
DEFINE_ENABLE_ISENABLED(ExtGNU_CompoundLiterals)
DEFINE_ENABLE_ISENABLED(ExtGNU_Conditionals)
DEFINE_ENABLE_ISENABLED(ExtGNU_DesignatedInitializers)
DEFINE_ENABLE_ISENABLED(ExtGNU_StatementExpressions)
DEFINE_ENABLE_ISENABLED(ExtGNU_InternalBuiltins)

DEFINE_ENABLE_ISENABLED(ExtPSY_Generics)

DEFINE_ENABLE_ISENABLED(Expand_static_assert_AsKeyword)
DEFINE_ENABLE_ISENABLED(Expand_complex_AsKeyword)
DEFINE_ENABLE_ISENABLED(Expand_operatorNames)
DEFINE_ENABLE_ISENABLED(Expand_alignas_AsKeyword)
DEFINE_ENABLE_ISENABLED(Expand_alignof_AsKeyword)
DEFINE_ENABLE_ISENABLED(Expand_bool_AsKeyword)
DEFINE_ENABLE_ISENABLED(Expand_thread_local_AsKeyword)

DEFINE_ENABLE_ISENABLED(CPP_nullptr)

DEFINE_ENABLE_ISENABLED(NativeBooleans)
DEFINE_ENABLE_ISENABLED(NULLAsBuiltin)

#undef DEFINE_ENABLE_ISENABLED
