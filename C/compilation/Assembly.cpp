// Copyright (c) 2021/22 Leandro T. C. Melo <ltcmelo@gmail.com>
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

#include "Assembly.h"

#include <algorithm>

using namespace psy;
using namespace C;

std::vector<const Symbol*> Assembly::symbols() const
{
    std::vector<const Symbol*> syms;
    std::transform(symDEFs_.begin(),
                   symDEFs_.end(),
                   std::back_inserter(syms),
                   [] (auto& sym) { return sym.get();});
    return syms;
}

Symbol* Assembly::findSymDEF(std::function<bool (const std::unique_ptr<Symbol>&)> pred) const
{
    auto it = std::find_if(symDEFs_.begin(), symDEFs_.end(), pred);
    return it == symDEFs_.end() ? nullptr
                                : it->get();
}
