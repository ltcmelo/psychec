/****************************************************************************
**
** Copyright (C) 2015 The Qt Company Ltd.
** Contact: https://www.qt.io/licensing/
**
** Modifications:
** Copyright (c) 2016-20 Leandro T. C. Melo (ltcmelo@gmail.com)
**
** This file is part of Qt Creator.
**
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and The Qt Company.  For licensing terms and
** conditions see http://www.qt.io/terms-conditions.  For further information
** use the contact form at http://www.qt.io/contact-us.
**
** GNU Lesser General Public License Usage
** Alternatively, this file may be used under the terms of the GNU Lesser
** General Public License version 2.1 or version 3 as published by the Free
** Software Foundation and appearing in the file LICENSE.LGPLv21 and
** LICENSE.LGPLv3 included in the packaging of this file.  Please review the
** following information to ensure the GNU Lesser General Public License
** requirements will be met: https://www.gnu.org/licenses/lgpl.html and
** http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
**
** In addition, as a special exception, The Qt Company gives you certain additional
** rights.  These rights are described in The Qt Company LGPL Exception
** version 1.1, included in the file LGPL_EXCEPTION.txt in this package.
**
****************************************************************************/

/////////////////////////////////////////////////////////////////////
/////              Note about copyright/licesing                /////
/////                                                           /////
///// This file, which is copyrighed by NOKIA/Digia/Qt Company, /////
///// is licensed under LGPL, as opposed to most of the files   /////
///// of the Psyche project, which are licensed under BSD. This /////
///// version has been modified by Leandro T. C. Melo.          /////
/////////////////////////////////////////////////////////////////////

#include "SyntaxWriterDOTFormat.h"

#include "SyntaxLexeme_ALL.h"
#include "SyntaxVisitor.h"
#include "SyntaxTree.h"

#ifdef __GNUC__
#  include <cxxabi.h>
#endif

using namespace psy;
using namespace C;

void SyntaxWriterDOTFormat::write(const SyntaxNode* node,
                                  const std::string& fileSuffix)
{
    std::string basename = tree_->filePath();
    basename.append(fileSuffix);
    std::ofstream ofs(basename);
    write(node, fileSuffix, ofs);
    ofs.close();
}

void SyntaxWriterDOTFormat::write(const SyntaxNode* node,
                                  const std::string& fileSuffix,
                                  std::ostream &os)
{
    os_ = &os;
    count_ = 1;

    *os_ << "digraph Syntax { ordering=out;" << std::endl;
    // std::cout << "rankdir = \"LR\";" << std::endl;

    generateTokens();
    visit(node);

    for (const auto& conn : connections_)
        *os_ << conn.first << " -> " << conn.second << std::endl;

    alignTerminals();

    *os_ << "}" << std::endl;
}

void SyntaxWriterDOTFormat::alignTerminals()
{
    *os_ <<"{ rank=same;" << std::endl;
    for (const auto& terminalShape : terminalShapes_)
        *os_ << "  " << terminalShape << ";" << std::endl;
    *os_ <<"}"<<std::endl;
}

std::string SyntaxWriterDOTFormat::name(const SyntaxNode* node) {
#ifdef __GNUC__
    std::string name = abi::__cxa_demangle(typeid(*node).name(), 0, 0, 0) + 8;
#else
    std::string name = typeid(*node).name();
#endif
    return name;
}

std::string SyntaxWriterDOTFormat::terminalId(unsigned token)
{
    return 't' + std::to_string(token);
}

std::string SyntaxWriterDOTFormat::terminalId(const SyntaxToken& tk)
{
    return 't' + tk.valueText();
}

void SyntaxWriterDOTFormat::terminal(const SyntaxToken& tk, const SyntaxNode *node)
{
    connections_.push_back(std::make_pair(id_[node], terminalId(tk)));
}

void SyntaxWriterDOTFormat::generateTokens() {
    for (unsigned token = 1; token < tree_->tokenCount(); ++token) {
        if (tree_->tokenAt(token).kind() == EndOfFile)
            break;

        std::string t;
        t.append(terminalId(token));
        t.append(" [shape=rect label = \"");
        t.append(tree_->tokenAt(token).valueText());
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

void SyntaxWriterDOTFormat::nodeLabel(const SyntaxNode* node)
{
    *os_ << id_[node] << " [label=\"" << name(node) << "\"];" << std::endl;
}

bool SyntaxWriterDOTFormat::preVisit(const SyntaxNode* node)
{
    const std::string id = 'n' + std::to_string(count_++);
    id_[node] = id;

    if (!nodes_.empty())
        connections_.push_back(std::make_pair(id_[nodes_.top()], id));

    nodes_.push(node);

    nodeLabel(node);

    return true;
}

void SyntaxWriterDOTFormat::postVisit(const SyntaxNode *)
{
    nodes_.pop();
}

