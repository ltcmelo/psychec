// Copyright (c) 2020/21/22 Leandro T. C. Melo <ltcmelo@gmail.com>
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

#include "ParseOptions.h"

using namespace psy;
using namespace C;

ParseOptions::ParseOptions()
    : ParseOptions(LanguageDialect(), LanguageExtensions())
{}

ParseOptions::ParseOptions(LanguageDialect langDialect)
    : ParseOptions(langDialect, LanguageExtensions())
{}

ParseOptions::ParseOptions(LanguageDialect langDialect,
                           LanguageExtensions langExts)
    : langDialect_(std::move(langDialect))
    , langExts_(std::move(langExts))
    , bits_(0)
{
    enable_keywordRecognition(true);
    setCommentMode(CommentMode::Discard);
    setAmbiguityMode(AmbiguityMode::DisambiguateAlgorithmicallyAndHeuristically);
}

ParseOptions& ParseOptions::withLanguageDialect(LanguageDialect langDialect)
{
    langDialect_ = langDialect;
    return *this;
}

const LanguageDialect& ParseOptions::languageDialect() const
{
    return langDialect_;
}

ParseOptions& ParseOptions::withLanguageExtensions(LanguageExtensions langExts)
{
    langExts_ = langExts;
    return *this;
}

const LanguageExtensions& ParseOptions::languageExtensions() const
{
    return langExts_;
}

ParseOptions& ParseOptions::setCommentMode(CommentMode commentMode)
{
    BF_.commentMode_ = static_cast<int>(commentMode);
    return *this;
}

ParseOptions::CommentMode ParseOptions::commentMode() const
{
    return static_cast<CommentMode>(BF_.commentMode_);
}

ParseOptions &ParseOptions::setAmbiguityMode(AmbiguityMode ambiguityMode)
{
    BF_.ambigMode_ = static_cast<int>(ambiguityMode);
    return *this;
}

ParseOptions::AmbiguityMode ParseOptions::ambiguityMode() const
{
    return static_cast<AmbiguityMode>(BF_.ambigMode_);
}

#define DEFINE_ENABLE_ISENABLED(FLAG) \
    ParseOptions& ParseOptions::enable_##FLAG(bool enable) \
        { BF_.FLAG##_ = enable; return *this; } \
    bool ParseOptions::isEnabled_##FLAG() const \
        { return BF_.FLAG##_; }

DEFINE_ENABLE_ISENABLED(keywordRecognition)

#undef DEFINE_ENABLE_ISENABLED
