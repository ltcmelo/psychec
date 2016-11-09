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

#ifndef PSYCHE_CONSTRAINTSTREAMWRITER_H__
#define PSYCHE_CONSTRAINTSTREAMWRITER_H__

#include <ostream>
#include <string>
#include <tuple>
#include <vector>

namespace psyche {

/*!
 * \brief The ConstraintStreamWriter class
 */
class ConstraintStreamWriter final
{
public:
    /*!
     * \brief ConstraintStreamWriter
     * \param os
     */
    ConstraintStreamWriter(std::ostream& os);

    ConstraintStreamWriter(const ConstraintStreamWriter&) = delete;
    ConstraintStreamWriter& operator=(const ConstraintStreamWriter&) = delete;

    /*!
     * \brief blockWriting
     * \param block
     * \return
     *
     * Block (or unblock) actuall writing on the stream.
     */
    bool blockWriting(bool block);

    /*!
     * \brief writeText
     * \param s
     *
     * A "plain" version of write for whatever content.
     */
    void writeText(const std::string& text);

    /*!
     * \brief writeTypedef
     * \param ty1
     * \param ty2
     */
    void writeTypedef(const std::string& ty1, const std::string& ty2);

    /*!
     * \brief writeVarDecl
     * \param value
     * \param type
     */
    void writeVarDecl(const std::string& name, const std::string& type);

    using ParamPair = std::pair<std::string, std::string>;

    /*!
     * \brief writeFuncDecl
     * \param name
     * \param params
     * \param ret
     *
     *
     */
    void writeFuncDecl(const std::string& name,
                                const std::vector<ParamPair>& params,
                                const std::string& ret);

    /*!
     * \brief writeFunctionParameters
     * \param params
     */
    void writeFuncParams(const std::vector<ParamPair> params);

    /*!
     * \brief writeTypeofExpr
     * \param sym
     */
    void writeTypeof(const std::string& sym);

    /*!
     * \brief writeNewTypeVar
     * \param ty
     */
    void writeNewTypeVar(const std::string& ty);

    /*!
     * \brief writeTypeEquiv
     *
     * Write an equivalence. If both types surrounding the equivalence are
     * known in advance, the equivalence relation can be written at once
     * through writeEquivRelation.
     *
     * \sa writeEquivRelation
     */
    void writeTypeEquiv();

    /*!
     * \brief writeTypeName
     * \param ty
     *
     * Write a type name. If there's a sequence of names to be written, they
     * can all be written through writeTypeNames.
     *
     * \sa writeTypeNames
     */
    void writeTypeName(const std::string& ty);

    /*!
     * \brief writeIsConst
     * \param ty
     */
    void writeReadOnly(const std::string& ty);

    /*!
     * \brief writeTypeNames
     * \param tys
     *
     * Write a sequence of type names.
     */
    void writeTypeNames(const std::vector<std::string>& tys);

    /*!
     * \brief writerMemberRelation
     * \param baseTy
     * \param sym
     * \param symTy
     */
    void writeMemberRelation(const std::string& baseTy,
                             const std::string& sym,
                             const std::string& symTy);

    /*!
     * \brief writePointerRelation
     * \param ty1
     * \param ty2
     */
    void writePointerRelation(const std::string& ty1, const std::string& ty2);

    /*!
     * \brief writeEquivRelation
     * \param ty1
     * \param ty2
     *
     * Essentially the same of sequentially writing a type, a type equivalence,
     * and another type.
     */
    void writeEquivRelation(const std::string& ty1, const std::string& ty2);

    /*!
     * \brief writeTruth
     *
     * Write a filling true=true constraint.
     */
    void writeTruth();

    /*!
     * \brief writeAnd
     */
    void writeAnd(bool breakLine = false);

    /*!
     * \brief write a colon
     */
    void writeColon();

    /*!
     * \brief breakLine
     */
    void breakLine();

    //!@{
    /*!
     * Sub-grouping
     */
    void enterGroup();
    void leaveGroup();
    //!@}

    void clearIndent();

    /*!
     * \brief totalConstraints
     * \return
     */
    size_t totalConstraints() const { return cnt_; }

private:
    void indent();
    void dedent();

    std::ostream& os_;
    int indent_ { 0 };
    bool blocked_ { false };
    size_t cnt_ { 0 };
};

} // namespace psyche

#endif
