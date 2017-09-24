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

#ifndef PSYCHE_CONSTRAINTWRITER_H__
#define PSYCHE_CONSTRAINTWRITER_H__

#include <ostream>
#include <string>
#include <tuple>
#include <vector>

namespace psyche {

/*!
 * \brief The ConstraintWriter class
 */
class ConstraintWriter
{
public:
    /*!
     * \brief ConstraintWriter
     * \param os
     */
    ConstraintWriter(std::ostream& os);
    ConstraintWriter(const ConstraintWriter&) = delete;
    ConstraintWriter& operator=(const ConstraintWriter&) = delete;
    virtual ~ConstraintWriter();

    /*!
     * \brief blockWriting
     * \param block
     * \return
     *
     * Block (or unblock) actuall writing on the stream.
     */
    virtual bool block(bool b);

    /*!
     * \brief writeText
     * \param s
     *
     * A "plain" version of write for whatever content.
     */
    virtual void writeText(const std::string& text);

    /*!
     * \brief writeTypedef
     * \param ty1
     * \param ty2
     */
    virtual void writeTypedef(const std::string& ty1, const std::string& ty2);

    /*!
     * \brief writeVarDecl
     * \param value
     * \param type
     */
    virtual void writeVarDecl(const std::string& name, const std::string& type);

    /*!
     * \brief writeFuncDecl
     * \param name
     * \param params
     * \param ret
     *
     *
     */
    virtual void writeFuncDecl(const std::string& name,
                               const std::vector<std::string>& params,
                               const std::string& ret);

    /*!
     * \brief writeTypeofExpr
     * \param sym
     */
    virtual void writeTypeof(const std::string& sym);

    /*!
     * \brief writeNewTypeVar
     * \param ty
     */
    virtual void writeExists(const std::string& ty);

    /*!
     * \brief writeTypeName
     * \param ty
     *
     * Write a type name. If there's a sequence of names to be written, they can all be
     * written through writeTypeNames.
     *
     * \sa writeTypeNames
     */
    virtual void writeTypeSection(const std::string& ty);

    /*!
     * \brief writeTypeNames
     * \param tys
     *
     * Write a sequence of type names.
     */
    virtual void writeTypesSection(const std::vector<std::string>& tys);

    /*!
     * \brief writeReadOnly
     * \param ty
     */
    virtual void writeConstantExpression(const std::string& val);

    /*!
     * \brief writeStatic
     * \param val
     */
    virtual void writeStatic(const std::string& val);

    /*!
     * \brief writerMemberRel
     * \param baseTy
     * \param sym
     * \param symTy
     */
    virtual void writeMemberRel(const std::string& baseTy,
                                const std::string& sym,
                                const std::string& symTy);

    /*!
     * \brief writePtrRel
     * \param ty1
     * \param ty2
     */
    virtual void writePtrRel(const std::string& ty1, const std::string& ty2);

    /*!
     * \brief writeEquivRel
     * \param ty1
     * \param ty2
     *
     * Essentially the same of sequentially writing a type, a type equivalence,
     * and another type.
     */
    virtual void writeEquivRel(const std::string& ty1, const std::string& ty2);

    /*!
     * \brief writeSubtypeRelation
     * \param ty
     * \param subTy
     */
    virtual void writeSubtypeRel(const std::string &ty, const std::string &subTy);

    /*!
     * \brief writeTypeMark
     *
     * Write an equivalence. If both types are known in advance, the equivalence relation
     * can be written at once through writeEquivRelation.
     *
     * \sa writeEquivRelation
     */
    virtual void writeEquivMark();

    /*!
     * \brief writeSubtypeMark
     */
    virtual void writeSubtypeMark();

    //!@{
    /*!
     * Sub-grouping
     */
    virtual void enterGroup();
    virtual void leaveGroup();
    //!@}

    virtual void openScope();
    virtual void closeScope();

    /*!
     * \brief totalConstraints
     * \return
     */
    size_t totalConstraints() const { return cnt_; }

    void endSection();
    void beginSection();

protected:
    void writeAnd();
    void writeColon();
    void writeLineBreak();

    void indent();
    void dedent();

    std::ostream* os_;
    int indent_ { 0 };
    bool blocked_ { false };
    size_t cnt_ { 0 };
    bool wantComma { false };

};

} // namespace psyche

#endif
