// Copyright (c) 2024 Leandro T. C. Melo <ltcmelo@gmail.com>
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

#include "TypeChecker.h"

#include "syntax/SyntaxTree.h"

using namespace psy;
using namespace C;

const std::string TypeChecker::DiagnosticsReporter::ID_InvalidOperator = "TypeChecker-001";
const std::string TypeChecker::DiagnosticsReporter::ID_ExpectedExpressionOfArithmeticType = "TypeChecker-001";
const std::string TypeChecker::DiagnosticsReporter::ID_ExpectedExpressionOfIntegerType = "TypeChecker-002";
const std::string TypeChecker::DiagnosticsReporter::ID_ExpectedExpressionOfRealType = "TypeChecker-003";
const std::string TypeChecker::DiagnosticsReporter::ID_ExpectedExpressionOfPointerType = "TypeChecker-004";
const std::string TypeChecker::DiagnosticsReporter::ID_ExpectedExpressionOfScalarType = "TypeChecker-005";
const std::string TypeChecker::DiagnosticsReporter::ID_ExpectedExpressionOfStructOrUnionType = "TypeChecker-006";
const std::string TypeChecker::DiagnosticsReporter::ID_ExpectedExpressionOfPointerToStructOrUnionType = "TypeChecker-007";
const std::string TypeChecker::DiagnosticsReporter::ID_ExpectedExpression = "TypeChecker-008";
const std::string TypeChecker::DiagnosticsReporter::ID_UnknownMemberOfTag = "TypeChecker-009";

void TypeChecker::DiagnosticsReporter::diagnose(DiagnosticDescriptor&& desc, SyntaxToken tk)
{
    tyChecker_->tree_->newDiagnostic(desc, tk);
};

void TypeChecker::DiagnosticsReporter::InvalidOperator(SyntaxToken tk)
{
    diagnose(DiagnosticDescriptor(
                 ID_InvalidOperator,
                 "[[invalid operator]]",
                 "invalid operator",
                 DiagnosticSeverity::Error,
                 DiagnosticCategory::TypeChecking),
             tk);
}

void TypeChecker::DiagnosticsReporter::ExpectedExpressionOfType_CORE(
        SyntaxToken tk,
        const std::string& diagnosticID,
        const std::string& category)
{
    std::string s = "expected expression of "
            + category
            + " type";
    diagnose(DiagnosticDescriptor(
                 diagnosticID,
                 "[[expected expression of type]]",
                 s,
                 DiagnosticSeverity::Error,
                 DiagnosticCategory::TypeChecking),
             tk);
}

void TypeChecker::DiagnosticsReporter::ExpectedExpressionOfArithmeticType(SyntaxToken tk)
{
    ExpectedExpressionOfType_CORE(
                tk,
                ID_ExpectedExpressionOfArithmeticType,
                "arithmetic");
}

void TypeChecker::DiagnosticsReporter::ExpectedExpressionOfIntegerType(SyntaxToken tk)
{
    ExpectedExpressionOfType_CORE(
                tk,
                ID_ExpectedExpressionOfIntegerType,
                "integer");
}

void TypeChecker::DiagnosticsReporter::ExpectedExpressionOfRealType(SyntaxToken tk)
{
    ExpectedExpressionOfType_CORE(
                tk,
                ID_ExpectedExpressionOfRealType,
                "real");
}

void TypeChecker::DiagnosticsReporter::ExpectedExpressionOfPointerType(SyntaxToken tk)
{
    ExpectedExpressionOfType_CORE(
                tk,
                ID_ExpectedExpressionOfPointerType,
                "pointer");
}

void TypeChecker::DiagnosticsReporter::ExpectedExpressionOfScalarType(SyntaxToken tk)
{
    ExpectedExpressionOfType_CORE(
                tk,
                ID_ExpectedExpressionOfScalarType,
                "scalar");
}

void TypeChecker::DiagnosticsReporter::ExpectedExpressionOfStructOrUnionType(SyntaxToken tk)
{
    ExpectedExpressionOfType_CORE(
                tk,
                ID_ExpectedExpressionOfStructOrUnionType,
                "structure or union");
}

void TypeChecker::DiagnosticsReporter::ExpectedExpressionOfPointerToStructOrUnionType(SyntaxToken tk)
{
    ExpectedExpressionOfType_CORE(
                tk,
                ID_ExpectedExpressionOfPointerToStructOrUnionType,
                "pointer to structure or union");
}

void TypeChecker::DiagnosticsReporter::ExpectedExpression(SyntaxToken tk)
{
    diagnose(DiagnosticDescriptor(
                 ID_InvalidOperator,
                 "[[expected expression]]",
                 "expected expression",
                 DiagnosticSeverity::Error,
                 DiagnosticCategory::TypeChecking),
             tk);
}

void TypeChecker::DiagnosticsReporter::UnknownMemberOfTag(SyntaxToken tk)
{
    diagnose(DiagnosticDescriptor(
                 ID_InvalidOperator,
                 "[[unknown member of tag]]",
                 "unknown member of tag",
                 DiagnosticSeverity::Error,
                 DiagnosticCategory::TypeChecking),
             tk);
}
