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

#ifndef PSYCHE_C_SYNTAX_WRITER_DOT_FORMAT_H__
#define PSYCHE_C_SYNTAX_WRITER_DOT_FORMAT_H__

#include "API.h"

#include "SyntaxDumper.h"

#include <fstream>
#include <stack>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>

namespace psy {
namespace C {

class PSY_C_API SyntaxWriterDOTFormat final : public SyntaxDumper
{
public:
    using SyntaxDumper::SyntaxDumper;

    void write(const SyntaxNode* node, const std::string& fileSuffix);
    void write(const SyntaxNode* node, const std::string& fileSuffix, std::ostream& os);

private:
    static std::string name(const SyntaxNode* node);

    void terminal(const SyntaxToken& tk, const SyntaxNode* node) override;
    std::string terminalId(unsigned token);
    std::string terminalId(const SyntaxToken& tk);

    void generateTokens();
    void alignTerminals();
    void nodeLabel(const SyntaxNode* node);

    bool preVisit(const SyntaxNode* node) override;
    void postVisit(const SyntaxNode*) override;

    std::unordered_map<const SyntaxNode*, std::string> id_;
    std::vector<std::pair<std::string, std::string>> connections_;
    std::stack<const SyntaxNode*> nodes_;
    std::vector<std::string> terminalShapes_;
    std::ostream* os_;
    int count_;
};

} // C
} // psy

#endif
