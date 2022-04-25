// Copyright (c) 2020/21/22 Leandro T. C. Melo <ltcmelo@gmail.com>
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

#include "SyntaxNamePrinter.h"

#include "SyntaxNode.h"

#include <algorithm>
#include <iostream>
#include <unordered_map>

using namespace psy;
using namespace C;

namespace {

int CUR_LEVEL;

std::string formatSnippet(std::string& snippet)
{
    std::replace_if(snippet.begin(), snippet.end(),
                 [] (char c) { return c == '\n' || c == '\t'; },
                 ' ');

    while (true) {
        auto pos = snippet.find("  ");
        if (pos == std::string::npos)
            break;
        snippet = snippet.replace(pos, 2, " ");
    }

    static const auto MAX_LEN = 30;
    if (snippet.length() > MAX_LEN) {
        snippet = snippet.substr(0, MAX_LEN);
        snippet += "...";
    }

    return snippet;
}

} // anonymous

void SyntaxNamePrinter::print(const SyntaxNode* node, Style style)
{
    print(node, style, std::cout);
}

void SyntaxNamePrinter::print(const SyntaxNode* node, Style style, std::ostream& os)
{
    CUR_LEVEL = 0;

    nonterminal(node);

    auto source = node->syntaxTree()->text().rawText();

    os << std::endl;
    for (auto i = 0U; i < dump_.size(); ++i) {
        auto node = std::get<0>(dump_[i]);
        auto nodeLevel = std::get<1>(dump_[i]);

        if (style == Style::Plain) {
            os << std::string(nodeLevel * 4, ' ');
            os << to_string(node->kind()) << std::endl;
            continue;
        }

        auto levelCnt = 0;
        while (nodeLevel > levelCnt) {
            if (nodeLevel == levelCnt + 1) {
                os << '|';
                os << std::string(2, '-');
            }
            else {
                int nextLevelBelow;
                for (auto j = i + 1; j < dump_.size(); ++j) {
                    nextLevelBelow = std::get<1>(dump_[j]);
                    if (nextLevelBelow <= levelCnt + 1)
                        break;
                }
                if (nextLevelBelow == levelCnt + 1)
                    os << '|';
                else
                    os << ' ';
                os << std::string(2, ' ');
            }
            ++levelCnt;
        }

        os << to_string(node->kind()) << " ";

        if (node->kind() == TranslationUnit) {
            os << std::endl;
            continue;
        }

        os << " <";
        auto firstTk = node->firstToken();
        auto lastTk = node->lastToken();
        if (firstTk.isValid())
            os << firstTk.location().lineSpan().span().start();
        os << "..";
        if (lastTk.isValid())
            os << lastTk.location().lineSpan().span().end();
        os << "> ";

        if (firstTk.isValid() && lastTk.isValid()) {
            auto firstTkStart = source.c_str() + firstTk.span().start();
            auto lastTkEnd = source.c_str() + lastTk.span().end();
            std::string snippet(firstTkStart, lastTkEnd - firstTkStart);
            os << " `" << formatSnippet(snippet) << "`";
        }

        os << std::endl;
    }
}

void SyntaxNamePrinter::nonterminal(const SyntaxNode* node)
{
    if (!node)
        return;

    dump_.push_back(std::make_tuple(node, CUR_LEVEL));

    ++CUR_LEVEL;
    visit(node);
    --CUR_LEVEL;
}

