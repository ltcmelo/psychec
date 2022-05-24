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

#include "Lexer.h"

#include "SyntaxTree.h"

#include "../common/diagnostics/Diagnostic.h"

using namespace psy;
using namespace C;

const std::string Lexer::DiagnosticsReporter::ID_of_IncompatibleLanguageDialect = "Lexer-001";
const std::string Lexer::DiagnosticsReporter::ID_of_IncompatibleLanguageExtension = "Lexer-002";

void Lexer::DiagnosticsReporter::IncompatibleLanguageDialect(
        const std::string& feature,
        LanguageDialect::Std expectedStd)
{
    DiagnosticDescriptor descriptor(ID_of_IncompatibleLanguageDialect,
                                    "Incompatible language dialect",
                                    feature + " is available in " + to_string(expectedStd),
                                    DiagnosticSeverity::Warning,
                                    DiagnosticCategory::Syntax);

    lexer_->tree_->newDiagnostic(descriptor, lexer_->tree_->freeTokenSlot());
}

void Lexer::DiagnosticsReporter::IncompatibleLanguageExtension(
        const std::string& feature,
        LanguageExtensions::Ext expectedExt)
{
    DiagnosticDescriptor descriptor(ID_of_IncompatibleLanguageExtension,
                                    "Incompatible language extension",
                                    feature + " is available in " + to_string(expectedExt),
                                    DiagnosticSeverity::Warning,
                                    DiagnosticCategory::Syntax);

    lexer_->tree_->newDiagnostic(descriptor, lexer_->tree_->freeTokenSlot());
}
