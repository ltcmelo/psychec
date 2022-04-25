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

#include "Diagnostic.h"

#include <regex>

using namespace psy;

Diagnostic::Diagnostic(DiagnosticDescriptor descriptor,
                       Location location,
                       std::string snippet)
    : descriptor_(std::move(descriptor))
    , location_(std::move(location))
    , snippet_(std::move(snippet))
    , outputIndent_(0)
{}

DiagnosticSeverity Diagnostic::severity() const
{
    // TODO: Implement "warning as error".

    return descriptor_.defaultSeverity();
}

namespace psy {

std::ostream& operator<<(std::ostream& os, const Diagnostic& diagnostic)
{
    if (diagnostic.outputIndent_)
        os << std::string(diagnostic.outputIndent_, '\t');

    os << diagnostic.location().lineSpan().path() << ":"
       << diagnostic.location().lineSpan().span().start() << " "
       << diagnostic.severity() << ": "
       << diagnostic.descriptor().description();

    if (!diagnostic.snippet().empty()) {
        os << "\n";

        auto s = diagnostic.snippet();
        if (diagnostic.outputIndent_) {
            std::string indent(diagnostic.outputIndent_, '\t');
            os << indent;
            s = std::regex_replace(s, std::regex("\n"), "\n" + indent);
        }
        os << s;
    }

    return os;
}

} // psy
