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

#include "Reparser.h"

#include "syntax/SyntaxTree.h"

#include "reparser/NameCataloger.h"
#include "reparser/Disambiguator_GuidelineImposition.h"
#include "reparser/Disambiguator_SyntaxCorrelation.h"
#include "syntax/SyntaxNode.h"

#include "../common/infra/Assertions.h"

using namespace psy;
using namespace C;

Reparser::Reparser()
{}

void Reparser::addDisambiguationStrategy(DisambiguationStrategy strategy)
{
    strategies_.push_back(strategy);
}

bool Reparser::eliminatedAllAmbiguities() const
{
    return strategies_.empty()
            ? false
            : persistentAmbigs_.empty();
}

bool Reparser::ambiguityPersists(const SyntaxNode* node) const
{
    PSY_ASSERT_3(node->asAmbiguousCastOrBinaryExpression()
                        || node->asAmbiguousExpressionOrDeclarationStatement()
                        || node->asAmbiguousTypeNameOrExpressionAsTypeReference(),
                     return false,
                     "node isn't ambiguous node");

    if (strategies_.empty())
        return true;

    for (auto ambig : persistentAmbigs_) {
        if (node == ambig)
            return true;
    }
    return false;
}

void Reparser::reparse(SyntaxTree* tree)
{
    for (auto strategy : strategies_) {
        std::unique_ptr<Disambiguator> disambiguator;
        switch (strategy) {
            case Reparser::DisambiguationStrategy::SyntaxCorrelation: {
                NameCataloger cataloger(tree);
                auto catalog = cataloger.catalogNamesWithinNode(tree->root());
                disambiguator.reset(new SyntaxCorrelationDisambiguator(tree, std::move(catalog)));
                break;
            }
            case Reparser::DisambiguationStrategy::GuidelineImposition:
                disambiguator.reset(new GuidelineImpositionDisambiguator(tree));
                break;
            default:
                PSY_ASSERT_1(false);
                return;
        }
        auto ok = disambiguator->disambiguate();
        if (ok) {
            persistentAmbigs_.clear();
            return;
        }

        const auto& persistentAmbigs = disambiguator->persistentAmbiguities();
        for (const auto& ambig : persistentAmbigs)
            persistentAmbigs_.insert(ambig);
    }
}
