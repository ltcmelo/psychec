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

#include "ConstraintsInTypeSpecifiers.h"

#include "SyntaxTree.h"

#include "../common/infra/Assertions.h"

#include <iostream>

using namespace psy;
using namespace C;

const std::string ConstraintsInTypeSpecifiers::ID_TypeSpecifierMissingDefaultsToInt =
        "Binder-100-6.7.2-2-A";
const std::string ConstraintsInTypeSpecifiers::ID_TwoOrMoreDataTypesInDeclarationSpecifiers =
        "Binder-100-6.7.2-2-B";

void ConstraintsInTypeSpecifiers::TypeSpecifierMissingDefaultsToInt(
        SyntaxToken declTk,
        Binder::DiagnosticsReporter* diagReporter)
{
    diagReporter->diagnose(DiagnosticDescriptor(
                               ID_TypeSpecifierMissingDefaultsToInt,
                               "[[type specifier missing]]",
                               "type specifier missing, defaults to `int'",
                               DiagnosticSeverity::Error,
                               DiagnosticCategory::Binding),
                           declTk);
}

void ConstraintsInTypeSpecifiers::TwoOrMoreDataTypesInDeclarationSpecifiers(
        SyntaxToken builtTySpecTk,
        Binder::DiagnosticsReporter* diagReporter)
{
    diagReporter->diagnose(DiagnosticDescriptor(
                               ID_TwoOrMoreDataTypesInDeclarationSpecifiers,
                               "[[two or more data types in declaration specifiers]]",
                               "two or more data types in declaration specifiers",
                               DiagnosticSeverity::Error,
                               DiagnosticCategory::Binding),
                           builtTySpecTk);
}

void ConstraintsInTypeSpecifiers::specify(SyntaxToken builtTySpecTk,
                                       NamedTypeSymbol* namedTySym,
                                       Binder::DiagnosticsReporter* diagReporter)
{
    auto builtTyKind = combine(builtTySpecTk,
                               namedTySym->builtinTypeKind(),
                               diagReporter);
    if (builtTyKind == namedTySym->builtinTypeKind())
        return;

    namedTySym->patchBuiltinTypeKind(builtTyKind);
}

BuiltinTypeKind ConstraintsInTypeSpecifiers::combine(SyntaxToken builtTySpecTk,
                                                  BuiltinTypeKind builtTyKind,
                                                  Binder::DiagnosticsReporter* diagReporter)
{
    const auto tkK = builtTySpecTk.kind();
    switch (builtTyKind) {
        case BuiltinTypeKind::UNSPECIFIED:
            switch (tkK) {
                case Keyword_void:
                    return BuiltinTypeKind::Void;
                case Keyword_char:
                    return BuiltinTypeKind::Char;
                case Keyword_short:
                    return BuiltinTypeKind::Short;
                case Keyword_int:
                    return BuiltinTypeKind::Int;
                case Keyword_long:
                    return BuiltinTypeKind::Long;
                case Keyword_float:
                    return BuiltinTypeKind::Float;
                case Keyword_double:
                    return BuiltinTypeKind::Double;
                case Keyword__Bool:
                    return BuiltinTypeKind::Bool;
                case Keyword__Complex:
                    return BuiltinTypeKind::DoubleComplex;
                case Keyword_signed:
                    return BuiltinTypeKind::Int_S;
                case Keyword_unsigned:
                    return BuiltinTypeKind::Int_U;
                default:
                    PSY_ESCAPE_VIA_RETURN(builtTyKind);
            }

        case BuiltinTypeKind::Void:
            // report
            return builtTyKind;

        case BuiltinTypeKind::Char:
            switch (tkK) {
                case Keyword_signed:
                    return BuiltinTypeKind::Char_S;
                case Keyword_unsigned:
                    return BuiltinTypeKind::Char_U;
                default:
                    // report
                    return builtTyKind;
            }
        case BuiltinTypeKind::Char_S:
        case BuiltinTypeKind::Char_U:
            // report
            return builtTyKind;

        case BuiltinTypeKind::Short:
            switch (tkK) {
                case Keyword_signed:
                    return BuiltinTypeKind::Short_S;
                case Keyword_unsigned:
                    return BuiltinTypeKind::Short_U;
                default:
                    // report
                    return builtTyKind;
            }
        case BuiltinTypeKind::Short_S:
        case BuiltinTypeKind::Short_U:
            // report
            return builtTyKind;

        case BuiltinTypeKind::Int:
            switch (tkK) {
                case Keyword_long:
                    return BuiltinTypeKind::Long;
                case Keyword_signed:
                    return BuiltinTypeKind::Int_S;
                case Keyword_unsigned:
                    return BuiltinTypeKind::Int_U;
                default:
                    TwoOrMoreDataTypesInDeclarationSpecifiers(builtTySpecTk, diagReporter);
                    return builtTyKind;
            }
        case BuiltinTypeKind::Int_S:
        case BuiltinTypeKind::Int_U:
            // report
            return builtTyKind;

        case BuiltinTypeKind::Long:
            switch (tkK) {
                case Keyword_int:
                    return builtTyKind;
                case Keyword_signed:
                    return BuiltinTypeKind::Long_S;
                case Keyword_unsigned:
                    return BuiltinTypeKind::Long_U;
                default:
                    TwoOrMoreDataTypesInDeclarationSpecifiers(builtTySpecTk, diagReporter);
                    return builtTyKind;
            }
        case BuiltinTypeKind::Long_S:
        case BuiltinTypeKind::Long_U:
            // report
            return builtTyKind;


        default:
            return BuiltinTypeKind::UNSPECIFIED;
    }
}
