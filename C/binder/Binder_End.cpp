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

#include "Binder.h"
#include "Binder__MACROS__.inc"

#include "SyntaxTree.h"

#include "binder/Scope.h"
#include "compilation/Compilation.h"
#include "symbols/Symbol_ALL.h"
#include "syntax/SyntaxFacts.h"
#include "syntax/Lexeme_ALL.h"
#include "syntax/SyntaxNodes.h"
#include "syntax/SyntaxUtilities.h"
#include "types/Type_ALL.h"

#include "../common/infra/Assertions.h"

#include <iostream>

using namespace psy;
using namespace C;

SyntaxVisitor::Action Binder::visitStructOrUnionDeclaration_AtEnd(const StructOrUnionDeclarationSyntax* node)
{
    return visitDeclaration_AtEnd_COMMON(node);
}

SyntaxVisitor::Action Binder::visitEnumDeclaration_AtEnd(const EnumDeclarationSyntax* node)
{
    return visitDeclaration_AtEnd_COMMON(node);
}

SyntaxVisitor::Action Binder::visitTypedefDeclaration_AtEnd(const TypedefDeclarationSyntax* node)
{
    popType();
    return Action::Skip;
}

SyntaxVisitor::Action Binder::visitVariableAndOrFunctionDeclaration_AtEnd(
        const VariableAndOrFunctionDeclarationSyntax* node)
{
    popType();
    return Action::Skip;
}

SyntaxVisitor::Action Binder::visitFieldDeclaration_AtEnd(const FieldDeclarationSyntax* node)
{
    popType();
    return Action::Skip;
}

SyntaxVisitor::Action Binder::visitEnumeratorDeclaration_AtEnd(const EnumeratorDeclarationSyntax* node)
{
    popType();
    return visitDeclaration_AtEnd_COMMON(node);
}

SyntaxVisitor::Action Binder::visitParameterDeclaration_AtEnd(const ParameterDeclarationSyntax* node)
{
    return visitDeclaration_AtEnd_COMMON(node);
}

SyntaxVisitor::Action Binder::visitDeclaration_AtEnd_COMMON(const DeclarationSyntax* node)
{
    auto decl = popSymbolAsDeclaration();
    PSY_ASSERT_2(decl, return Action::Quit);
    DECL_TOP_SCOPE_retQ(scope);
    scope->addDeclaration(decl->asDeclaration());

    return Action::Skip;
}

SyntaxVisitor::Action Binder::visitFunctionDefinition_AtEnd(const FunctionDefinitionSyntax* node)
{
    return Action::Skip;
}
