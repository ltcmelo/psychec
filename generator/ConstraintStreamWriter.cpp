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

#include "ConstraintStreamWriter.h"
#include "Assert.h"

using namespace psyche;

namespace {

const char* const kTypeDef = "$typedef$ ";
const char* const kDecl = "$def$ ";
const char* const kMember = "$has$ ";
const char* const kExistence = "$exists$ ";
const char* const kContainment = " $in$ ";
const char* const kDeclDelim = " : ";
const char* const kEquiv = " = ";
const char* const kAlias = " $as$ ";
const char* const kTypeOf = "$typeof$";
const char* const kReadOnly = "$read_only$";

} // anonymous

#define HONOR_BLOCKING_STATE \
    do { \
        if (blocked_) \
            return; \
    } while (false)


ConstraintStreamWriter::ConstraintStreamWriter(std::ostream &os)
    : os_(os)
{}

bool ConstraintStreamWriter::blockWriting(bool block)
{
    bool prev = blocked_;
    blocked_ = block;
    return prev;
}

void ConstraintStreamWriter::writeText(const std::string &text)
{
    HONOR_BLOCKING_STATE;

    os_ << text;
}

void ConstraintStreamWriter::writeTypedef(const std::string &ty1, const std::string &ty2)
{
    HONOR_BLOCKING_STATE;

    os_ << kTypeDef << ty1 << kAlias << ty2;
    ++cnt_;
}

void ConstraintStreamWriter::writeVarDecl(const std::string &name,
                                          const std::string &type)
{
    HONOR_BLOCKING_STATE;

    os_ << kDecl << name << kDeclDelim << type << kContainment << "\n";
    indent();
    ++cnt_;
}

void ConstraintStreamWriter::
writeFuncDecl(const std::string &name,
              const std::vector<ParamPair> &params,
              const std::string& ret)
{
    HONOR_BLOCKING_STATE;

    os_ << kDecl << name << kDeclDelim << "(";
    for (const auto& param : params)
        os_ << param.first << ", ";
    os_ << ret;
    os_ << ") " << kContainment << "\n";
    indent();
    if(!params.empty())
        writeFuncParams(params);
    ++cnt_;
}

void ConstraintStreamWriter::writeFuncParams(const std::vector<ParamPair> params)
{
    HONOR_BLOCKING_STATE;

    for (const auto& param : params)
        writeVarDecl(param.second, param.first);
}

void ConstraintStreamWriter::writeTypeof(const std::string &sym)
{
    HONOR_BLOCKING_STATE;

    os_ << kTypeOf << "(" << sym << ")";
    ++cnt_;
}

void ConstraintStreamWriter::writeNewTypeVar(const std::string &ty)
{
    HONOR_BLOCKING_STATE;

    os_ << kExistence << ty << ".\n";
    indent();
    ++cnt_;
}

void ConstraintStreamWriter::writeTypeEquiv()
{
    HONOR_BLOCKING_STATE;

    os_ << kEquiv;
}

void ConstraintStreamWriter::writeTypeName(const std::string& ty)
{
    HONOR_BLOCKING_STATE;

    os_ << ty;
}

void ConstraintStreamWriter::writeReadOnly(const std::string &ty)
{
    HONOR_BLOCKING_STATE;

    os_ << kReadOnly << "(" << ty << ")";
    ++cnt_;
}

void ConstraintStreamWriter::writeTypeNames(const std::vector<std::string> &tys)
{
    HONOR_BLOCKING_STATE;

    const auto size = tys.size();
    for (auto i = 0; i < size; ++i) {
        writeTypeName(tys[i]);
        if (i != tys.size() - 1)
            writeAnd();
    }
    ++cnt_;
}

void ConstraintStreamWriter::writeMemberRelation(const std::string &baseTy,
                                                 const std::string &sym,
                                                 const std::string &symTy)
{
    HONOR_BLOCKING_STATE;

    os_ << kMember << "(";
    writeTypeName(baseTy);
    writeAnd();
    os_ << sym;
    writeColon();
    writeTypeName(symTy);
    os_ << ")";
    ++cnt_;
}

void ConstraintStreamWriter::writePointerRelation(const std::string &ty1,
                                                  const std::string &ty2)
{
    HONOR_BLOCKING_STATE;

    writeTypeName(ty1);
    writeTypeEquiv();
    writeTypeName(ty2);
    os_ << "*";
    ++cnt_;
}

void ConstraintStreamWriter::writeEquivRelation(const std::string &ty1,
                                                const std::string &ty2)
{
    HONOR_BLOCKING_STATE;

    writeTypeName(ty1);
    writeTypeEquiv();
    writeTypeName(ty2);
    ++cnt_;
}

void ConstraintStreamWriter::writeTruth()
{
    writeEquivRelation("true", "true");
}

void ConstraintStreamWriter::enterGroup()
{
    HONOR_BLOCKING_STATE;

    os_ << "(";
}

void ConstraintStreamWriter::leaveGroup()
{
    HONOR_BLOCKING_STATE;

    os_ << ")";
}

void ConstraintStreamWriter::writeAnd(bool breakIt)
{
    HONOR_BLOCKING_STATE;

    os_ << ", ";
    if (breakIt)
        breakLine();
}

void ConstraintStreamWriter::breakLine()
{
    HONOR_BLOCKING_STATE;

    os_ << std::endl;
    indent();
}

void ConstraintStreamWriter::indent()
{
    HONOR_BLOCKING_STATE;

    int indent = ++indent_;
    while (indent > 0) {
        os_ << " ";
        --indent;
    }
}

void ConstraintStreamWriter::dedent()
{
    --indent_;
    PSYCHE_ASSERT(indent_ >= 0, return, "indent must be >= 0");
}

void ConstraintStreamWriter::clearIndent()
{
    indent_ = 0;
}

void ConstraintStreamWriter::writeColon()
{
    os_ << kDeclDelim;
}
