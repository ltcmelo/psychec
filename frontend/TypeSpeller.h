/******************************************************************************
 Copyright (c) 2016-17 Leandro T. C. Melo (ltcmelo@gmail.com)

 This library is free software; you can redistribute it and/or modify it under
 the terms of the GNU Lesser General Public License as published by the Free
 Software Foundation; either version 2.1 of the License, or (at your option)
 any later version.

 This library is distributed in the hope that it will be useful, but WITHOUT
 ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public License
 for more details.

 You should have received a copy of the GNU Lesser General Public License along
 with this library; if not, write to the Free Software Foundation, Inc., 51
 Franklin Street, Fifth Floor, Boston, MA  02110-1301 USA
 *****************************************************************************/

#ifndef PSYCHE_TYPENAMESPELLER_H__
#define PSYCHE_TYPENAMESPELLER_H__

#include "FrontendConfig.h"
#include "CSyntax.h"
#include "CoreTypes.h"
#include "Literals.h"
#include "Symbols.h"
#include "TypeVisitor.h"
#include <string>

namespace psyche {

/*!
 * \brief The TypeSpeller class template
 *
 * Spell the name of a type.
 */
template <class SyntaxT>
class CFE_API TypeSpeller : public psyche::TypeVisitor
{
public:
    TypeSpeller();

    /*!
     * \brief spellTypeName
     * \param fullType
     * \param scope
     * \return
     *
     * Spell the full type's name into an string.
     */
    std::string spell(const psyche::FullySpecifiedType& fullType,
                      const psyche::Scope* scope);

protected:
    void visitType(const psyche::FullySpecifiedType&);

    void visit(psyche::UndefinedType*) override;
    void visit(psyche::VoidType*) override;
    void visit(psyche::IntegerType*) override;
    void visit(psyche::FloatType*) override;
    void visit(psyche::PointerToMemberType*) override;
    void visit(psyche::PointerType*) override;
    void visit(psyche::ReferenceType*) override;
    void visit(psyche::ArrayType*) override;
    void visit(psyche::NamedType*) override;
    void visit(psyche::Function*) override;
    void visit(psyche::Class*) override;
    void visit(psyche::Enum*) override;
    void visit(psyche::ForwardClassDeclaration*) override;

    void func(psyche::Function*, RetParam);
    void func(psyche::Function*, ParamRet);
    virtual void funcParam(psyche::Function*);
    virtual void funcRet(psyche::Function*);

    std::string spelling_;
    const psyche::Scope* scope_;
};

} // namespace psyche

#endif
