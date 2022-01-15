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

#include "Semantics_TypeQualifiers.h"

#include "SyntaxTree.h"

#include "../common/infra/PsycheAssert.h"

#include <iostream>

using namespace psy;
using namespace C;

const std::string Semantics_TypeQualifiers::ID_Temp =
        "Binder-100-6.7.2-2";


void Semantics_TypeQualifiers::temp(
        Binder::DiagnosticsReporter* diagReporter)
{
//    diagReporter->diagnose(DiagnosticDescriptor(
//                               ID_Temp,
//                               "[[two or more data types in declaration specifiers]]",
//                               "two or more data types in declaration specifiers",
//                               DiagnosticSeverity::Error,
//                               DiagnosticCategory::Binding),
//                           builtTySpecTk);
}

void Semantics_TypeQualifiers::qualify(SyntaxToken tyQualTk,
                                       TypeSymbol* tySym,
                                       Binder::DiagnosticsReporter* diagReporter)
{
    const auto tkK = tyQualTk.kind();
    switch (tkK) {
        case Keyword_const:
            std::cout << "qualifying with const\n";
            tySym->qualifyWithConst();
            break;

        default:
            PSYCHE_FAIL_0(break);
            break;
    }
}
