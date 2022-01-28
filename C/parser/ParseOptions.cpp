// Copyright (c) 2020/21 Leandro T. C. Melo <ltcmelo@gmail.com>
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
    : bits_(0)
{
    BF_.keywordIdentifiersClassified_ = true;
}

ParseOptions::ParseOptions(LanguageDialect dialect)
    : dialect_(std::move(dialect))
    , bits_(0)
{
    BF_.keywordIdentifiersClassified_ = true;
}

ParseOptions::ParseOptions(LanguageDialect dialect, LanguageExtensions extensions)
    : dialect_(std::move(dialect))
    , extensions_(std::move(extensions))
    , bits_(0)
{
    BF_.keywordIdentifiersClassified_ = true;
}

const LanguageDialect& ParseOptions::dialect() const
{
    return dialect_;
}

const LanguageExtensions& ParseOptions::extensions() const
{
    return extensions_;
}

ParseOptions& ParseOptions::setCommentMode(CommentMode mode)
{
    BF_.commentMode_ =  static_cast<int>(mode);
    return *this;
}

ParseOptions& ParseOptions::classifyKeywordIdentifiers(bool yes)
{
    BF_.keywordIdentifiersClassified_ = yes;
    return *this;
}
