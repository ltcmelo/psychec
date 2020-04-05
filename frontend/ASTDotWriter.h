/****************************************************************************
**
** Copyright (C) 2015 The Qt Company Ltd.
** Contact: https://www.qt.io/licensing/
**
** Modifications (apply on the LGPL usage):
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
///// Except for this file, which is copyrighed by NOKIA/Digia/ /////
///// Qt Company and licensed under LGPL, all of PsycheC front- /////
///// end is licensed under BSD. This version has been modified /////
///// by Leandro T. C. Melo.                                    /////
/////////////////////////////////////////////////////////////////////

#ifndef PSYCHE_AST_DOT_WRITER_H__
#define PSYCHE_AST_DOT_WRITER_H__

#include "FrontendConfig.h"
#include "ASTDumper.h"
#include <fstream>
#include <stack>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>

namespace psyche {

/*!
 * \brief The Dumper class
 *
 * This is an adaptation of Qt Creator's ASTDump class.
 */
class CFE_API ASTDotWriter final : public ASTDumper
{
public:
    using ASTDumper::ASTDumper;

    void write(psyche::AST *ast, const std::string& fileSuffix);
    void write(psyche::AST *ast, const std::string& fileSuffix, std::ostream& os);

private:
    static std::string name(AST *ast);

    void terminal(unsigned token, AST *node);
    std::string terminalId(unsigned token);
    void alignTerminals();
    void generateTokens();
    void node(AST *ast);

    bool preVisit(AST *ast) override;
    void postVisit(AST *) override;

    std::unordered_map<psyche::AST *, std::string> id_;
    std::vector<std::pair<std::string, std::string>> connections_;
    std::stack<psyche::AST *> nodes_;
    std::vector<std::string> terminalShapes_;
    std::ostream* os_;
    int count_;
};

}

#endif
