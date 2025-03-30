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

#include "DeclarationBinder.h"
#include "DeclarationBinder__MACROS__.inc"

#include "sema/Scope.h"
#include "sema/Compilation.h"
#include "symbols/Symbol_ALL.h"
#include "syntax/SyntaxFacts.h"
#include "syntax/Lexeme_ALL.h"
#include "syntax/SyntaxNodes.h"
#include "syntax/SyntaxUtilities.h"
#include "syntax/SyntaxTree.h"
#include "syntax/SyntaxVisitor__MACROS__.inc"
#include "types/Type_ALL.h"
#include "../common/infra/Assertions.h"

#include <iostream>

using namespace psy;
using namespace C;

SyntaxVisitor::Action DeclarationBinder::visitStructOrUnionDeclaration_AtEnd(
        const StructOrUnionDeclarationSyntax* node)
{
    finishDeclaration();

    return Action::Skip;
}

SyntaxVisitor::Action DeclarationBinder::visitEnumDeclaration_AtEnd(
        const EnumDeclarationSyntax* node)
{
    finishDeclaration();

    return Action::Skip;
}

SyntaxVisitor::Action DeclarationBinder::visitTypedefDeclaration_AtEnd(
        const TypedefDeclarationSyntax* node)
{
    popType();

    return Action::Skip;
}

SyntaxVisitor::Action DeclarationBinder::visitVariableAndOrFunctionDeclaration_AtEnd(
        const VariableAndOrFunctionDeclarationSyntax* node)
{
    popType();

    return Action::Skip;
}

SyntaxVisitor::Action DeclarationBinder::visitFieldDeclaration_AtEnd(
        const FieldDeclarationSyntax* node)
{
    popType();

    return Action::Skip;
}

SyntaxVisitor::Action DeclarationBinder::visitEnumeratorDeclaration_AtEnd(
        const EnumeratorDeclarationSyntax* node)
{
    finishDeclaration();
    popType();

    return Action::Skip;
}

SyntaxVisitor::Action DeclarationBinder::visitParameterDeclaration_AtEnd(
        const ParameterDeclarationSyntax* node)
{
    finishDeclaration();
    popType();

    return Action::Skip;
}

SyntaxVisitor::Action DeclarationBinder::visitFunctionDefinition_AtEnd(
        const FunctionDefinitionSyntax* node)
{
    finishDeclaration();
    popType();

    /*
     * 6.9.2-10 (C23)
     * The parameter type list, the attribute specifier sequence of the
     * declarator that follows the parameter type list, and the compound
     * statement of the function body form a single block.
     */
    pushStashedScope();
    VALID_TOP(scopes_, return Action::Quit);
    auto scope = scopes_.top();
    scope->morphFrom_FunctionPrototype_to_Block();
    VISIT(node->body()->statements());
    popScope();

    return Action::Skip;
}

SyntaxVisitor::Action DeclarationBinder::visitTypeName_AtEnd(const TypeNameSyntax* node)
{
    finishDeclaration();
    popType();

    return Action::Skip;
}
