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

#include "Disambiguator.h"

#include "SyntaxTree.h"

#include "reparser/DisambiguationCataloger.h"
#include "reparser/Reparser_GuidelineImposition.h"
#include "reparser/Reparser_SyntaxCorrelation.h"
#include "reparser/Reparser_TypeSynonymsVerification.h"
#include "syntax/SyntaxNode.h"

#include "../common/infra/Escape.h"

using namespace psy;
using namespace C;

Reparser::Reparser()
    : strategy_(DisambiguationStrategy::SyntaxCorrelation)
    , heuristic_(false)
{}

void Reparser::chooseStrategy(DisambiguationStrategy strategy)
{
    strategy_ = strategy;
}

void Reparser::permitHeuristic(bool heuristic)
{
    heuristic_ = heuristic;
}

void Reparser::disambiguate(SyntaxTree* tree)
{
    if (strategy_ == Reparser::DisambiguationStrategy::GuidelineImposition) {
        // TODO
        return;
    }

    DisambiguationCataloger cataloger(tree);
    auto catalog = cataloger.catalogFor(tree->root());

    switch (strategy_) {
        case Reparser::DisambiguationStrategy::SyntaxCorrelation: {
            SyntaxCorrelationDisambiguator reparser(tree);
            reparser.acquireCatalog(std::move(catalog));
            reparser.reparse();
            break;
        }

        case Reparser::DisambiguationStrategy::TypeSynonymsVerification:
            break;

        default:
            PSY_ESCAPE();
    }
}
