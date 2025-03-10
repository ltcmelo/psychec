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

#include "DiagnosticCategory.h"

#include "../infra/Assertions.h"

namespace psy {

std::ostream& operator<<(std::ostream& os, DiagnosticCategory category)
{
    switch (category) {
        case DiagnosticCategory::Syntax:
            os << "syntax";
            break;
        case DiagnosticCategory::Binding:
            os << "declaration";
            break;
        case DiagnosticCategory::TypeChecking:
            os << "type checking";
            break;
        case DiagnosticCategory::TypeResolution:
            os << "type resolution";
            break;
        case DiagnosticCategory::UNSPECIFIED:
            os << "generic";
            break;
    }
    PSY_ASSERT_1(false);
    return os;
}

} // psy
