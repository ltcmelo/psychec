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

const std::string TypeChecker::DiagnosticsReporter::ID_of_InvalidOperator = "TypeChecker-000";
const std::string TypeChecker::DiagnosticsReporter::ID_of_ExpectedExpressionOfArithmeticType = "TypeChecker-001";
const std::string TypeChecker::DiagnosticsReporter::ID_of_ExpectedExpressionOfIntegerType = "TypeChecker-002";
const std::string TypeChecker::DiagnosticsReporter::ID_of_ExpectedExpressionOfRealType = "TypeChecker-003";
const std::string TypeChecker::DiagnosticsReporter::ID_of_ExpectedExpressionOfPointerType = "TypeChecker-004";
const std::string TypeChecker::DiagnosticsReporter::ID_of_ExpectedExpressionOfScalarType = "TypeChecker-005";
const std::string TypeChecker::DiagnosticsReporter::ID_of_ExpectedExpressionOfStructOrUnionType = "TypeChecker-006";
const std::string TypeChecker::DiagnosticsReporter::ID_of_ExpectedExpressionOfPointerToStructOrUnionType = "TypeChecker-007";
const std::string TypeChecker::DiagnosticsReporter::ID_of_ExpectedExpressionOfPointerOrArrayType = "TypeChecker-008";
const std::string TypeChecker::DiagnosticsReporter::ID_of_ExpectedExpressionOfFunctionOrFunctionPointerType = "TypeChecker-009";
const std::string TypeChecker::DiagnosticsReporter::ID_of_ExpectedExpression = "TypeChecker-010";
const std::string TypeChecker::DiagnosticsReporter::ID_of_UnknownMemberOfTag = "TypeChecker-011";
const std::string TypeChecker::DiagnosticsReporter::ID_of_IncompatibleTypesInAssignment = "TypeChecker-012";
const std::string TypeChecker::DiagnosticsReporter::ID_of_CannotAssignToExpressionOfConstQualifiedType = "TypeChecker-013";
const std::string TypeChecker::DiagnosticsReporter::ID_of_CannotAssignToExpressionOfArrayType = "TypeChecker-014";
const std::string TypeChecker::DiagnosticsReporter::ID_of_ConversionBetweenIntegerAndPointerTypesInAssignment = "TypeChecker-015";
const std::string TypeChecker::DiagnosticsReporter::ID_of_TooFewArgumentsToFunctionCall = "TypeChecker-016";

void TypeChecker::DiagnosticsReporter::diagnose(DiagnosticDescriptor&& desc, SyntaxToken tk)
{
    tyChecker_->tree_->newDiagnostic(desc, tk);
};

void TypeChecker::DiagnosticsReporter::InvalidOperator(SyntaxToken tk)
{
    diagnose(DiagnosticDescriptor(
                 ID_of_InvalidOperator,
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
                ID_of_ExpectedExpressionOfArithmeticType,
                "arithmetic");
}

void TypeChecker::DiagnosticsReporter::ExpectedExpressionOfIntegerType(SyntaxToken tk)
{
    ExpectedExpressionOfType_CORE(
                tk,
                ID_of_ExpectedExpressionOfIntegerType,
                "integer");
}

void TypeChecker::DiagnosticsReporter::ExpectedExpressionOfRealType(SyntaxToken tk)
{
    ExpectedExpressionOfType_CORE(
                tk,
                ID_of_ExpectedExpressionOfRealType,
                "real");
}

void TypeChecker::DiagnosticsReporter::ExpectedExpressionOfPointerType(SyntaxToken tk)
{
    ExpectedExpressionOfType_CORE(
                tk,
                ID_of_ExpectedExpressionOfPointerType,
                "pointer");
}

void TypeChecker::DiagnosticsReporter::ExpectedExpressionOfScalarType(SyntaxToken tk)
{
    ExpectedExpressionOfType_CORE(
                tk,
                ID_of_ExpectedExpressionOfScalarType,
                "scalar");
}

void TypeChecker::DiagnosticsReporter::ExpectedExpressionOfStructOrUnionType(SyntaxToken tk)
{
    ExpectedExpressionOfType_CORE(
                tk,
                ID_of_ExpectedExpressionOfStructOrUnionType,
                "structure or union");
}

void TypeChecker::DiagnosticsReporter::ExpectedExpressionOfPointerToStructOrUnionType(SyntaxToken tk)
{
    ExpectedExpressionOfType_CORE(
                tk,
                ID_of_ExpectedExpressionOfPointerToStructOrUnionType,
                "pointer to structure or union");
}

void TypeChecker::DiagnosticsReporter::ExpectedExpressionOfPointerOrArrayType(SyntaxToken tk)
{
    ExpectedExpressionOfType_CORE(
                tk,
                ID_of_ExpectedExpressionOfPointerOrArrayType,
                "pointer or array");
}

void TypeChecker::DiagnosticsReporter::ExpectedExpressionOfFunctionOrFunctionPointerType(SyntaxToken tk)
{
    ExpectedExpressionOfType_CORE(
                tk,
                ID_of_ExpectedExpressionOfFunctionOrFunctionPointerType,
                "function or function pointer");
}

void TypeChecker::DiagnosticsReporter::ExpectedExpression(SyntaxToken tk)
{
    diagnose(DiagnosticDescriptor(
                 ID_of_ExpectedExpression,
                 "[[expected expression]]",
                 "expected expression",
                 DiagnosticSeverity::Error,
                 DiagnosticCategory::TypeChecking),
             tk);
}

void TypeChecker::DiagnosticsReporter::UnknownMemberOfTag(SyntaxToken tk)
{
    diagnose(DiagnosticDescriptor(
                 ID_of_UnknownMemberOfTag,
                 "[[unknown member of tag]]",
                 "unknown member of tag",
                 DiagnosticSeverity::Error,
                 DiagnosticCategory::TypeChecking),
             tk);
}

void TypeChecker::DiagnosticsReporter::IncompatibleTypesInAssignment(SyntaxToken tk)
{
    diagnose(DiagnosticDescriptor(
                 ID_of_IncompatibleTypesInAssignment,
                 "[[incompatible types in assignment]]",
                 "incompatible types in assignment",
                 DiagnosticSeverity::Error,
                 DiagnosticCategory::TypeChecking),
             tk);
}

void TypeChecker::DiagnosticsReporter::CannotAssignToExpressionOfConstQualifiedType(SyntaxToken tk)
{
    diagnose(DiagnosticDescriptor(
                 ID_of_CannotAssignToExpressionOfConstQualifiedType,
                 "[[assignment to expression of const qualified type]]",
                 "assignment to expression of const qualified type",
                 DiagnosticSeverity::Error,
                 DiagnosticCategory::TypeChecking),
             tk);
}

void TypeChecker::DiagnosticsReporter::CannotAssignToExpressionOfArrayType(SyntaxToken tk)
{
    diagnose(DiagnosticDescriptor(
                 ID_of_CannotAssignToExpressionOfArrayType,
                 "[[assignment to expression of array type]]",
                 "assignment to expression of array type",
                 DiagnosticSeverity::Error,
                 DiagnosticCategory::TypeChecking),
             tk);
}

void TypeChecker::DiagnosticsReporter::ConversionBetweenIntegerAndPointerTypesInAssignment(SyntaxToken tk)
{
    diagnose(DiagnosticDescriptor(
                 ID_of_ConversionBetweenIntegerAndPointerTypesInAssignment,
                 "[[conversion between integer and pointer types in assignment]]",
                 "conversion between integer and pointer types in assignment",
                 DiagnosticSeverity::Warning,
                 DiagnosticCategory::TypeChecking),
             tk);
}

void TypeChecker::DiagnosticsReporter::TooFewArgumentsToFunctionCall(SyntaxToken tk)
{
    diagnose(DiagnosticDescriptor(
                 ID_of_TooFewArgumentsToFunctionCall,
                 "[[too few arguments to function call]]",
                 "too few arguments to function call",
                 DiagnosticSeverity::Error,
                 DiagnosticCategory::TypeChecking),
             tk);
}
