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
    : ParseOptions(LanguageDialect(),
                   LanguageExtensions())
{}

ParseOptions::ParseOptions(LanguageDialect dialect,
                           LanguageExtensions extensions)
    : dialect_(std::move(dialect))
    , extensions_(std::move(extensions))
    , bits_(0)
{
    setTreatmentOfIdentifiers(TreatmentOfIdentifiers::Classify);
    setTreatmentOfComments(TreatmentOfComments::None);
    setTreatmentOfAmbiguities(TreatmentOfAmbiguities::DisambiguateAlgorithmicallyOrHeuristically);
}

const LanguageDialect& ParseOptions::dialect() const
{
    return dialect_;
}

const LanguageExtensions& ParseOptions::extensions() const
{
    return extensions_;
}

ParseOptions& ParseOptions::setTreatmentOfIdentifiers(TreatmentOfIdentifiers treatOfIdent)
{
    BF_.treatmentOfIdentifiers_ = static_cast<int>(treatOfIdent);
    return *this;
}

ParseOptions::TreatmentOfIdentifiers ParseOptions::treatmentOfIdentifiers() const
{
    return static_cast<TreatmentOfIdentifiers>(BF_.treatmentOfIdentifiers_);
}

ParseOptions& ParseOptions::setTreatmentOfComments(TreatmentOfComments treatOfComments)
{
    BF_.treatmentOfComments_ = static_cast<int>(treatOfComments);
    return *this;
}

ParseOptions::TreatmentOfComments ParseOptions::treatmentOfComments() const
{
    return static_cast<TreatmentOfComments>(BF_.treatmentOfComments_);
}

ParseOptions &ParseOptions::setTreatmentOfAmbiguities(TreatmentOfAmbiguities treatOfAmbigs)
{
    BF_.treatmentOfAmbiguities_ = static_cast<int>(treatOfAmbigs);
    return *this;
}

ParseOptions::TreatmentOfAmbiguities ParseOptions::treatmentOfAmbiguities() const
{
    return static_cast<TreatmentOfAmbiguities>(BF_.treatmentOfAmbiguities_);
}
