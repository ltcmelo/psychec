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

#include "TypeCanonicalizer.h"

#include "syntax/SyntaxTree.h"

using namespace psy;
using namespace C;

const std::string TypeCanonicalizer::DiagnosticsReporter::ID_of_TypeDeclarationNotFound = "Resolver - decl not found";
const std::string TypeCanonicalizer::DiagnosticsReporter::ID_of_ExpectedTypedefDeclaration = "Resolver - expected type decl";
const std::string TypeCanonicalizer::DiagnosticsReporter::ID_of_TagTypeDoesNotMatchTagDeclaration = "Resolver - unmatched tag";

void TypeCanonicalizer::DiagnosticsReporter::diagnose(DiagnosticDescriptor&& desc, SyntaxToken tk)
{
    canonicalizer_->tree_->newDiagnostic(desc, tk);
};

void TypeCanonicalizer::DiagnosticsReporter::TypeDeclarationNotFound(SyntaxToken tk)
{
    diagnose(DiagnosticDescriptor(
                 ID_of_TypeDeclarationNotFound,
                 "[[type declaration not found]]",
                 "type declaration not found",
                 DiagnosticSeverity::Error,
                 DiagnosticCategory::TypeResolution),
             tk);
}

void TypeCanonicalizer::DiagnosticsReporter::ExpectedTypedefDeclaration(SyntaxToken tk)
{
    diagnose(DiagnosticDescriptor(
                 ID_of_ExpectedTypedefDeclaration,
                 "[[expected typedef declaration]]",
                 "expected `typedef' declaration",
                 DiagnosticSeverity::Error,
                 DiagnosticCategory::TypeResolution),
             tk);
}

void TypeCanonicalizer::DiagnosticsReporter::TagTypeDoesNotMatchTagDeclaration(SyntaxToken tk)
{
    diagnose(DiagnosticDescriptor(
                 ID_of_TagTypeDoesNotMatchTagDeclaration,
                 "[[tag does not match declaration]]",
                 "tag does not match declaration",
                 DiagnosticSeverity::Error,
                 DiagnosticCategory::TypeResolution),
             tk);
}
