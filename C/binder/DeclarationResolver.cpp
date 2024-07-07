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

#include "DeclarationResolver.h"

#include "binder/Scope.h"
#include "compilation/SemanticModel.h"
#include "symbols/Symbol_ALL.h"
#include "syntax/Lexeme_Identifier.h"
#include "syntax/SyntaxNodes.h"
#include "types/Type_ALL.h"

#include <iostream>

using namespace psy;
using namespace C;

DeclarationResolver::DeclarationResolver(SemanticModel* semaModel, const SyntaxTree* tree)
    : SyntaxVisitor(tree)
    , semaModel_(semaModel)
{}

void DeclarationResolver::resolveDeclarations()
{
    visit(tree_->root());
}

SyntaxVisitor::Action DeclarationResolver::visitTranslationUnit(const TranslationUnitSyntax* node)
{
    for (auto declIt = node->declarations(); declIt; declIt = declIt->next)
        visit(declIt->value);

    return Action::Skip;
}

SyntaxVisitor::Action DeclarationResolver::visitVariableAndOrFunctionDeclaration(const VariableAndOrFunctionDeclarationSyntax* node)
{
    for (auto decltorIt = node->declarators(); decltorIt; decltorIt = decltorIt->next) {
        visit(decltorIt->value);
    }

    return Action::Skip;
}

SyntaxVisitor::Action DeclarationResolver::visitIdentifierDeclarator(const IdentifierDeclaratorSyntax* node)
{
    auto decl = semaModel_->declarationOf(node);
    if (decl) {
        switch (decl->kind()) {
            case DeclarationSymbolKind::Function:
                break;

            case DeclarationSymbolKind::Object: {
                auto objDecl = decl->asObjectDeclarationSymbol();
                auto ty = objDecl->type();
                switch (ty->kind()) {
                    case TypeKind::Typedef: {
                        auto tydefName = ty->asTypedefType()->typedefName();
                        auto tyDecl = decl->enclosingScope()->searchForDeclaration(
                                    tydefName,
                                    NameSpace::OrdinaryIdentifiers);
                        if (tyDecl) {
                            // TODO
                            std::cout << "got it " << std::endl;
                        }
                        break;
                    }

                    default:
                        break;
                }
                break;
            }

            case DeclarationSymbolKind::Type:
                break;
        }
    }

    return Action::Skip;
}
