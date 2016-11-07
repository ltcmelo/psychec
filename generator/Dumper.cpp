/******************************************************************************
 * Copyright (c) 2016 Leandro T. C. Melo (ltcmelo@gmail.com)
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301
 * USA
 *****************************************************************************/

#include "Dumper.h"
#include "ASTMatcher.h"
#include "ASTPatternBuilder.h"
#include "ASTVisitor.h"
#include "Control.h"
#include "CoreTypes.h"
#include "Literals.h"
#include "Names.h"
#include "Scope.h"
#include "Symbols.h"
#include "TranslationUnit.h"

#ifdef __GNUC__
#  include <cxxabi.h>
#endif

using namespace CPlusPlus;

void Dumper::dump(AST *ast, const std::string& fileSuffix)
{
    std::string basename = translationUnit()->fileName();
    basename.append(fileSuffix);
    std::ofstream ofs(basename);
    dump(ast, fileSuffix, ofs);
    ofs.close();
}

void Dumper::dump(AST *ast, const std::string &fileSuffix, std::ostream &os)
{
    os_ = &os;
    count_ = 1;

    *os_ << "digraph AST { ordering=out;" << std::endl;
    // std::cout << "rankdir = \"LR\";" << std::endl;

    generateTokens();
    accept(ast);

    for (const auto& conn : connections_)
        *os_ << conn.first << " -> " << conn.second << std::endl;

    alignTerminals();

    *os_ << "}" << std::endl;
}

void Dumper::alignTerminals()
{
    *os_ <<"{ rank=same;" << std::endl;
    for (const auto& terminalShape : terminalShapes_)
        *os_ << "  " << terminalShape << ";" << std::endl;
    *os_ <<"}"<<std::endl;
}

std::string Dumper::name(AST *ast) {
#ifdef __GNUC__
    std::string name = abi::__cxa_demangle(typeid(*ast).name(), 0, 0, 0) + 11;
#else
    std::string name = typeid(*ast).name();
#endif
    return name;
}

std::string Dumper::terminalId(unsigned token)
{
    return 't' + std::to_string(token);
}

void Dumper::terminal(unsigned token, AST *node)
{
    connections_.push_back(std::make_pair(id_[node], terminalId(token)));
}

void Dumper::generateTokens() {
    for (unsigned token = 1; token < translationUnit()->tokenCount(); ++token) {
        if (translationUnit()->tokenKind(token) == T_EOF_SYMBOL)
            break;

        std::string t;
        t.append(terminalId(token));
        t.append(" [shape=rect label = \"");
        t.append(spell(token));
        t.append("\"]");

        if (token > 1) {
            t.append("; ");
            t.append(terminalId(token - 1));
            t.append(" -> ");
            t.append(terminalId(token));
            t.append(" [arrowhead=\"vee\" color=\"transparent\"]");
        }

        terminalShapes_.push_back(t);
    }
}

void Dumper::nonterminal(AST *ast)
{
    accept(ast);
}

void Dumper::node(AST *ast)
{
    *os_ << id_[ast] << " [label=\"" << name(ast) << "\"];" << std::endl;
}

bool Dumper::preVisit(AST *ast)
{
    const std::string id = 'n' + std::to_string(count_++);
    id_[ast] = id;

    if (!nodes_.empty())
        connections_.push_back(std::make_pair(id_[nodes_.top()], id));

    nodes_.push(ast);

    node(ast);

    return true;
}

void Dumper::postVisit(AST *)
{
    nodes_.pop();
}
