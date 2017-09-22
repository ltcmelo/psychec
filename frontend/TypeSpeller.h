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

#include "CPlusPlusForwardDeclarations.h"
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
class CPLUSPLUS_EXPORT TypeSpeller : public CPlusPlus::TypeVisitor
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
    std::string spell(const CPlusPlus::FullySpecifiedType& fullType,
                      const CPlusPlus::Scope* scope);

protected:
    void visitType(const CPlusPlus::FullySpecifiedType&);

    void visit(CPlusPlus::UndefinedType*) override;
    void visit(CPlusPlus::VoidType*) override;
    void visit(CPlusPlus::IntegerType*) override;
    void visit(CPlusPlus::FloatType*) override;
    void visit(CPlusPlus::PointerToMemberType*) override;
    void visit(CPlusPlus::PointerType*) override;
    void visit(CPlusPlus::ReferenceType*) override;
    void visit(CPlusPlus::ArrayType*) override;
    void visit(CPlusPlus::NamedType*) override;
    void visit(CPlusPlus::Function*) override;
    void visit(CPlusPlus::Class*) override;
    void visit(CPlusPlus::Enum*) override;
    void visit(CPlusPlus::ForwardClassDeclaration*) override;

    void func(CPlusPlus::Function*, RetParam);
    void func(CPlusPlus::Function*, ParamRet);
    virtual void funcParam(CPlusPlus::Function*);
    virtual void funcRet(CPlusPlus::Function*);

    std::string spelling_;
    const CPlusPlus::Scope* scope_;
};

} // namespace psyche

#endif
