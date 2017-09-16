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

#include "ConstraintWriter.h"
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
const char* const kSubtype = " > ";
const char* const kAlias = " $as$ ";
const char* const kTypeOf = "$typeof$";
const char* const kReadOnly = "$read_only$";
const char* const kStatic = "$static$";

} // anonymous

#define HONOR_BLOCKING_STATE \
    do { \
        if (blocked_) \
            return; \
    } while (false)


ConstraintWriter::ConstraintWriter(std::ostream &os)
    : os_(&os)
{}

ConstraintWriter::~ConstraintWriter()
{}

bool ConstraintWriter::block(bool b)
{
    bool prev = blocked_;
    blocked_ = b;
    return prev;
}

void ConstraintWriter::writeText(const std::string &text)
{
    HONOR_BLOCKING_STATE;

    beginSection();
    *os_ << text;
}

void ConstraintWriter::writeTypedef(const std::string &ty1, const std::string &ty2)
{
    HONOR_BLOCKING_STATE;

    beginSection();
    writeLineBreak();
    *os_ << kTypeDef << ty1 << kAlias << ty2;
    endSection();

    ++cnt_;
}

void ConstraintWriter::writeVarDecl(const std::string &name, const std::string &ty)
{
    HONOR_BLOCKING_STATE;

    beginSection();
    writeLineBreak();
    *os_ << kDecl << name << kDeclDelim << ty << kContainment << " ";

    ++cnt_;
}

void ConstraintWriter::writeFuncDecl(const std::string &name,
                                     const std::vector<std::string> &params,
                                     const std::string& ret)
{
    HONOR_BLOCKING_STATE;

    beginSection();
    writeLineBreak();
    *os_ << kDecl << name << kDeclDelim << "(";
    for (const auto& param : params)
        *os_ << param << ", ";
    *os_ << ret;
    *os_ << ") " << kContainment << " ";

    ++cnt_;
}

void ConstraintWriter::writeTypeof(const std::string &sym)
{
    HONOR_BLOCKING_STATE;

    *os_ << kTypeOf << "(" << sym << ")";

    ++cnt_;
}

void ConstraintWriter::writeExists(const std::string &ty)
{
    HONOR_BLOCKING_STATE;

    beginSection();
    writeLineBreak();
    *os_ << kExistence << ty << ". ";

    ++cnt_;
}

void ConstraintWriter::writeEquivMark()
{
    HONOR_BLOCKING_STATE;

    *os_ << kEquiv;
}

void ConstraintWriter::writeSubtypeMark()
{
    HONOR_BLOCKING_STATE;

    *os_ << kSubtype;
}

void ConstraintWriter::writeSubtypeRel(const std::string& ty, const std::string& subTy)
{
    HONOR_BLOCKING_STATE;

    beginSection();
    writeTypeSection(ty);
    writeSubtypeMark();
    writeTypeSection(subTy);
    endSection();
    ++cnt_;
}

void ConstraintWriter::writeTypeSection(const std::string& ty)
{
    HONOR_BLOCKING_STATE;

    *os_ << ty;
}

void ConstraintWriter::writeConstantExpression(const std::string &val)
{
    HONOR_BLOCKING_STATE;

    beginSection();
    *os_ << kReadOnly << "(" << val << ")";
    endSection();

    ++cnt_;
}

void ConstraintWriter::writeStatic(const std::string &val)
{
    HONOR_BLOCKING_STATE;

    beginSection();
    *os_ << kStatic << "(" << val << ")";
    endSection();

    ++cnt_;
}

void ConstraintWriter::writeTypesSection(const std::vector<std::string> &tys)
{
    HONOR_BLOCKING_STATE;

    const auto size = tys.size();
    for (auto i = 0u; i < size; ++i) {
        writeTypeSection(tys[i]);
        if (i != tys.size() - 1)
            writeAnd();
    }
    ++cnt_;
}

void ConstraintWriter::writeMemberRel(const std::string &baseTy,
                                      const std::string &memberName,
                                      const std::string &symTy)
{
    HONOR_BLOCKING_STATE;

    beginSection();
    *os_ << kMember << "(";
    writeTypeSection(baseTy);
    writeAnd();
    *os_ << memberName;
    writeColon();
    writeTypeSection(symTy);
    *os_ << ")";
    endSection();

    ++cnt_;
}

void ConstraintWriter::writePtrRel(const std::string &ty1, const std::string &ty2)
{
    HONOR_BLOCKING_STATE;

    beginSection();
    writeTypeSection(ty1);
    writeEquivMark();
    writeTypeSection(ty2);
    *os_ << "*";
    endSection();
    ++cnt_;
}

void ConstraintWriter::writeEquivRel(const std::string &ty1, const std::string &ty2)
{
    HONOR_BLOCKING_STATE;

    beginSection();
    writeTypeSection(ty1);
    writeEquivMark();
    writeTypeSection(ty2);
    endSection();
    ++cnt_;
}

void ConstraintWriter::enterGroup()
{
    HONOR_BLOCKING_STATE;

    *os_ << "(";
}

void ConstraintWriter::leaveGroup()
{
    HONOR_BLOCKING_STATE;

    *os_ << ")";
}

void ConstraintWriter::openScope()
{
    HONOR_BLOCKING_STATE;

    beginSection();
    writeLineBreak();
    *os_ << "[ ";
    ++indent_;
}

void ConstraintWriter::closeScope()
{
    HONOR_BLOCKING_STATE;

    --indent_;
    writeLineBreak();
    *os_ << "]";
    endSection();
}

void ConstraintWriter::writeAnd()
{
    HONOR_BLOCKING_STATE;

    *os_ << ", ";
}

void ConstraintWriter::writeLineBreak()
{
    HONOR_BLOCKING_STATE;

    *os_ << std::endl;
    indent();
}

void ConstraintWriter::indent()
{
    HONOR_BLOCKING_STATE;

    std::string space(indent_, ' ');
    *os_ << space;
}

void ConstraintWriter::dedent()
{
    --indent_;
    PSYCHE_ASSERT(indent_ >= 0, return, "indent must be >= 0");
}

void ConstraintWriter::beginSection()
{
    if (wantComma)
        writeAnd();
    wantComma = false;
}

void ConstraintWriter::endSection()
{
    wantComma = true;
}

void ConstraintWriter::writeColon()
{
    *os_ << kDeclDelim;
}
