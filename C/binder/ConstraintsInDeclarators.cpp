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

#include "ConstraintsInDeclarators.h"

using namespace psy;
using namespace C;

const std::string ConstraintsInDeclarators::ID_FunctionReturningFunction = "Binder-200-6.7.6.3-1-A";
const std::string ConstraintsInDeclarators::ID_FunctionReturningArray = "Binder-200-6.7.6.3-1-B";

void ConstraintsInDeclarators::FunctionReturningFunction(
        SyntaxToken decltorTk,
        Binder::DiagnosticsReporter* diagReporter)
{
    auto s = "`"
            + decltorTk.valueText()
            + "' declared as function returning a function";

    diagReporter->diagnose(DiagnosticDescriptor(
                               ID_FunctionReturningFunction,
                               "[[function returning function]]",
                               s,
                               DiagnosticSeverity::Error,
                               DiagnosticCategory::Binding),
                           decltorTk);
}

void ConstraintsInDeclarators::FunctionReturningArray(
        SyntaxToken decltorTk,
        Binder::DiagnosticsReporter* diagReporter)
{
    auto s = "`"
            + decltorTk.valueText()
            + "' declared as function returning an array";

    diagReporter->diagnose(DiagnosticDescriptor(
                               ID_FunctionReturningArray,
                               "[[function returning array]]",
                               s,
                               DiagnosticSeverity::Error,
                               DiagnosticCategory::Binding),
                           decltorTk);
}
