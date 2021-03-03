// Copyright (c) 2020/21 Leandro T. C. Melo <ltcmelo@gmail.com>
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

#ifndef PSYCHE_C_SYNTAX_NAME_PRINTER_H__
#define PSYCHE_C_SYNTAX_NAME_PRINTER_H__

#include "API.h"

#include "SyntaxDumper.h"

#include <ostream>
#include <tuple>
#include <vector>

namespace psy {
namespace C {

class PSY_C_API SyntaxNamePrinter final : public SyntaxDumper
{
public:
    using SyntaxDumper::SyntaxDumper;

    enum class Mode : char
    {
        Basic,
        Elaborate
    };

    void print(const SyntaxNode* node, Mode mode);
    void print(const SyntaxNode* node, Mode mode, std::ostream& os);

private:
    virtual void nonterminal(const SyntaxNode* node) override;

    std::vector<std::tuple<const SyntaxNode*, int>> dump_;
};

} // C
} // psy

#endif
