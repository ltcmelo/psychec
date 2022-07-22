// Copyright (c) 2022 Leandro T. C. Melo <ltcmelo@gmail.com>
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

#ifndef PSYCHE_C_DISAMBIGUATOR_H__
#define PSYCHE_C_DISAMBIGUATOR_H__

#include "API.h"
#include "Fwds.h"

#include "parser/ParseOptions.h"

#include "../common/infra/InternalAccess.h"

#include <cstdint>

namespace psy {
namespace C {

class PSY_C_NON_API Reparser
{
PSY_INTERNAL_AND_RESTRICTED:
    PSY_GRANT_ACCESS(SyntaxTree);
    PSY_GRANT_ACCESS(InternalsTestSuite);

    Reparser();

    enum class DisambiguationStrategy : std::uint8_t
    {
        UNSPECIFIED = 0,

        TypeSynonymsVerification,
        SyntaxCorrelation,
        GuidelineImposition
    };

    void setDisambiguationStrategy(DisambiguationStrategy strategy);

    void setPermitHeuristic(bool heuristic);

    void reparse(SyntaxTree* tree);

private:
    DisambiguationStrategy disambigStrategy_;
    bool permitHeuristic_;
};

} // C
} // psy

#endif
