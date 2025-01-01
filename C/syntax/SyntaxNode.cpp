// Copyright (c) 2016/17/18/19/20/21/22 Leandro T. C. Melo <ltcmelo@gmail.com>
// Copyright (c) 2008 Roberto Raggi <roberto.raggi@gmail.com>
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
// THE SOFTWARE.

#include "SyntaxKind.h"
#include "SyntaxNodes.h"
#include "SyntaxVisitor.h"

#include <algorithm>
#include <cstddef>

using namespace psy;
using namespace C;

SyntaxNode::SyntaxNode(SyntaxTree* tree, SyntaxKind kind)
    : tree_(tree)
    , kind_(kind)
{}

SyntaxNode::~SyntaxNode()
{}

const SyntaxTree* SyntaxNode::syntaxTree() const
{
    return tree_;
}

SyntaxKind SyntaxNode::kind() const
{
    return kind_;
}

SyntaxToken SyntaxNode::firstToken() const
{
    return findValidToken(childNodesAndTokens());
}

SyntaxToken SyntaxNode::lastToken() const
{
    auto reversed = childNodesAndTokens();
    std::reverse(reversed.begin(), reversed.end());
    return findValidToken(reversed);
}

SyntaxToken SyntaxNode::findValidToken(const std::vector<SyntaxHolder>& syntaxHolders) const
{
    for (const auto& synH : syntaxHolders) {
        switch (synH.variant()) {
            case SyntaxHolder::Variant::Token:
                if (synH.tokenIndex() != LexedTokens::invalidIndex())
                    return tokenAtIndex(synH.tokenIndex());
                break;

            case SyntaxHolder::Variant::Node:
                if (synH.node()) {
                    auto tk = synH.node()->firstToken();
                    if (tk != SyntaxToken::invalid())
                        return tk;
                }
                break;

            case SyntaxHolder::Variant::NodeList:
                if (synH.nodeList()) {
                    auto tk = synH.nodeList()->firstToken();
                    if (tk != SyntaxToken::invalid())
                        return tk;
                }
                break;
        }
    }
    return 0;
}

SyntaxToken SyntaxNode::tokenAtIndex(LexedTokens::IndexType tkIdx) const
{
    if (tkIdx == 0)
        return SyntaxToken::invalid();
    return tree_->tokenAt(tkIdx);
}

void SyntaxNode::visitChildren(SyntaxVisitor* visitor) const
{
    for (auto synH : childNodesAndTokens()) {
        switch (synH.variant()) {
            case SyntaxHolder::Variant::Node: {
                if (!synH.node())
                    continue;
                auto node = const_cast<SyntaxNode*>(synH.node());
                node->acceptVisitor(visitor);
                break;
            }
            case SyntaxHolder::Variant::NodeList: {
                if (!synH.nodeList())
                    continue;
                auto nodeL = const_cast<SyntaxNodeList*>(synH.nodeList());
                nodeL->acceptVisitor(visitor);
                break;
            }
            default:
                break;
        }
    }
}

void SyntaxNode::acceptVisitor(SyntaxVisitor* visitor) const
{
    if (visitor->preVisit(this)) {
        auto action = dispatchVisit(visitor);
        if (action == SyntaxVisitor::Action::Visit)
            visitChildren(visitor);
    }
    visitor->postVisit(this);
}

namespace psy {
namespace C {

extern const char* tokenNames[];

std::string PSY_C_API to_string(SyntaxKind kind)
{
    switch (kind) {

        //--------------//
        // Declarations //
        //--------------//
        case SyntaxKind::TranslationUnit:
            return "TranslationUnit";
        case SyntaxKind::IncompleteDeclaration:
            return "IncompleteDeclaration";
        case SyntaxKind::StructDeclaration:
            return "StructDeclaration";
        case SyntaxKind::UnionDeclaration:
            return "UnionDeclaration";
        case SyntaxKind::EnumDeclaration:
            return "EnumDeclaration";
        case SyntaxKind::EnumeratorDeclaration:
            return "EnumeratorDeclaration";
        case SyntaxKind::VariableAndOrFunctionDeclaration:
            return "VariableAndOrFunctionDeclaration";
        case SyntaxKind::ParameterDeclaration:
            return "ParameterDeclaration";
        case SyntaxKind::FieldDeclaration:
            return "FieldDeclaration";
        case SyntaxKind::TypedefDeclaration:
            return "TypedefDeclaration";
        case SyntaxKind::StaticAssertDeclaration:
            return "StaticAssertDeclaration";
        case SyntaxKind::FunctionDefinition:
            return "FunctionDefinition";
        case SyntaxKind::ExtGNU_AsmStatementDeclaration:
            return "ExtGNU_AsmStatementDeclaration";
        case SyntaxKind::ExtPSY_TemplateDeclaration:
            return "ExtPSY_TemplateDeclaration";
        case SyntaxKind::ExtKR_ParameterDeclaration:
            return "ExtKR_ParameterDeclaration";

        /* Specifiers */
        case SyntaxKind::TypedefStorageClass:
            return "TypedefStorageClass";
        case SyntaxKind::ExternStorageClass:
            return "ExternStorageClass";
        case SyntaxKind::StaticStorageClass:
            return "StaticStorageClass";
        case SyntaxKind::AutoStorageClass:
            return "AutoStorageClass";
        case SyntaxKind::RegisterStorageClass:
            return "RegisterStorageClass";
        case SyntaxKind::ThreadLocalStorageClass:
            return "ThreadLocalStorageClass";
        case SyntaxKind::ConstQualifier:
            return "ConstQualifier";
        case SyntaxKind::RestrictQualifier:
            return "RestrictQualifier";
        case SyntaxKind::VolatileQualifier:
            return "VolatileQualifier";
        case SyntaxKind::AtomicQualifier:
            return "AtomicQualifier";
        case SyntaxKind::BasicTypeSpecifier:
            return "BasicTypeSpecifier";
        case SyntaxKind::VoidTypeSpecifier:
            return "VoidTypeSpecifier";
        case SyntaxKind::StructTypeSpecifier:
            return "StructTypeSpecifier";
        case SyntaxKind::UnionTypeSpecifier:
            return "UnionTypeSpecifier";
        case SyntaxKind::EnumTypeSpecifier:
            return "EnumTypeSpecifier";
        case SyntaxKind::AtomicTypeSpecifier:
            return "AtomicTypeSpecifier";
        case SyntaxKind::TagDeclarationAsSpecifier:
            return "TagDeclarationAsSpecifier";
        case SyntaxKind::InlineSpecifier:
            return "InlineSpecifier";
        case SyntaxKind::NoReturnSpecifier:
            return "NoReturnSpecifier";
        case SyntaxKind::AlignmentSpecifier:
            return "AlignmentSpecifier";
        case SyntaxKind::TypedefName:
            return "TypedefName";
        case SyntaxKind::ExtGNU_AttributeSpecifier:
            return "ExtGNU_AttributeSpecifier";
        case SyntaxKind::ExtGNU_Attribute:
            return "ExtGNU_Attribute";
        case SyntaxKind::ExtGNU_AsmLabel:
            return "ExtGNU_AsmLabel";
        case SyntaxKind::ExtGNU_Typeof:
            return "ExtGNU_Typeof";

        /* Declarators */
        case SyntaxKind::PointerDeclarator:
            return "PointerDeclarator";
        case SyntaxKind::IdentifierDeclarator:
            return "IdentifierDeclarator";
        case SyntaxKind::AbstractDeclarator:
            return "AbstractDeclarator";
        case SyntaxKind::ParenthesizedDeclarator:
            return "ParenthesizedDeclarator";
        case SyntaxKind::ArrayDeclarator:
            return "ArrayDeclarator";
        case SyntaxKind::FunctionDeclarator:
            return "FunctionDeclarator";
        case SyntaxKind::BitfieldDeclarator:
            return "BitfieldDeclarator";
        case SyntaxKind::SubscriptSuffix:
            return "SubscriptSuffix";
        case SyntaxKind::ParameterSuffix:
            return "ParameterSuffix";
        case SyntaxKind::BitfieldDeclaratorSuffix:
            return "BitfieldDeclaratorSuffix";

        /* Initializers */
        case SyntaxKind::ExpressionInitializer:
            return "ExpressionInitializer";
        case SyntaxKind::BraceEnclosedInitializer:
            return "BraceEnclosedInitializer";
        case SyntaxKind::DesignatedInitializer:
            return "DesignatedInitializer";
        case SyntaxKind::FieldDesignator:
            return "FieldDesignator";
        case SyntaxKind::ArrayDesignator:
            return "ArrayDesignator";

        case SyntaxKind::OffsetOfDesignator:
            return "OffsetOfDesignator";

        //-------------//
        // Expressions //
        //-------------//
        case SyntaxKind::IntegerConstantExpression:
            return "IntegerConstantExpression";
        case SyntaxKind::FloatingConstantExpression:
            return "FloatingConstantExpression";
        case SyntaxKind::CharacterConstantExpression:
            return "CharacterConstantExpression";
        case SyntaxKind::ImaginaryIntegerConstantExpression:
            return "ImaginaryIntegerConstantExpression";
        case SyntaxKind::ImaginaryFloatingConstantExpression:
            return "ImaginaryFloatingConstantExpression";
        case SyntaxKind::BooleanConstantExpression:
            return "BooleanLiteralExpression";
        case SyntaxKind::NULL_ConstantExpression:
            return "NULL_ConstantExpression";
        case SyntaxKind::StringLiteralExpression:
            return "StringLiteralExpression";
        case SyntaxKind::ParenthesizedExpression:
            return "ParenthesizedExpression";
        case SyntaxKind::GenericSelectionExpression:
            return "GenericSelectionExpression";
        case SyntaxKind::TypedGenericAssociation:
            return "GenericTypenameAssociation";
        case SyntaxKind::DefaultGenericAssociation:
            return "GenericDefaultAssociation";
        case SyntaxKind::ExtGNU_EnclosedCompoundStatementExpression:
            return "ExtGNU_EnclosedCompoundStatementExpression";
        case SyntaxKind::ExtGNU_ComplexValuedExpression:
            return "ExtGNU_ComplexValuedExpression";
        case SyntaxKind::ExtGNU_RealExpression:
            return "ExtGNU_RealExpression";
        case SyntaxKind::ExtGNU_ImagExpression:
            return "ExtGNU_ImagExpression";

        /* Names */
        case SyntaxKind::IdentifierName:
            return "IdentifierName";
        case SyntaxKind::PredefinedName:
            return "PredefinedName";

        /* Unary */
        case SyntaxKind::PreIncrementExpression:
            return "PreIncrementExpression";
        case SyntaxKind::PreDecrementExpression:
            return "PreDecrementExpression";
        case SyntaxKind::PostIncrementExpression:
            return "PostIncrementExpression";
        case SyntaxKind::PostDecrementExpression:
            return "PostDecrementExpression";
        case SyntaxKind::UnaryPlusExpression:
            return "UnaryPlusExpression";
        case SyntaxKind::UnaryMinusExpression:
            return "UnaryMinusExpression";
        case SyntaxKind::BitwiseNotExpression:
            return "BitwiseNotExpression";
        case SyntaxKind::LogicalNotExpression:
            return "LogicalNotExpression";
        case SyntaxKind::ExtGNU_LabelAddress:
            return "ExtGNU_LabelAddress";

        /* Type trait */
        case SyntaxKind::SizeofExpression:
            return "SizeofExpression";
        case SyntaxKind::AlignofExpression:
            return "AlignofExpression";

        /* Pointers */
        case SyntaxKind::AddressOfExpression:
            return "AddressOfExpression";
        case SyntaxKind::PointerIndirectionExpression:
            return "PointerIndirectionExpression";

        /* Access */
        case SyntaxKind::DirectMemberAccessExpression:
            return "DirectMemberAccessExpression";
        case SyntaxKind::IndirectMemberAccessExpression:
            return "IndirectMemberAccessExpression";
        case SyntaxKind::ElementAccessExpression:
            return "ElementAccessExpression";

        /* Invocation-like */
        case SyntaxKind::CallExpression:
            return "CallExpression";
        case SyntaxKind::CompoundLiteralExpression:
            return "CompoundLiteralExpression";
        case SyntaxKind::CastExpression:
            return "CastExpression";
        case SyntaxKind::VAArgumentExpression:
            return "VAArgumentExpression";
        case SyntaxKind::OffsetOfExpression:
            return "OffsetOfExpression";
        case SyntaxKind::ExtGNU_ChooseExpression:
            return "ExtGNU_ChooseExpression";

        /* Binary */
        case SyntaxKind::MultiplyExpression:
            return "MultipliyExpression";
        case SyntaxKind::DivideExpression:
            return "DivideExpression";
        case SyntaxKind::ModuleExpression:
            return "ModuleExpression";
        case SyntaxKind::AddExpression:
            return "AddExpression";
        case SyntaxKind::SubstractExpression:
            return "SubstractExpression";
        case SyntaxKind::LeftShiftExpression:
            return "LeftShiftExpression";
        case SyntaxKind::RightShiftExpression:
            return "RightShiftExpression";
        case SyntaxKind::LessThanExpression:
            return "LessThanExpression";
        case SyntaxKind::LessThanOrEqualExpression:
            return "LessThanOrEqualExpression";
        case SyntaxKind::GreaterThanExpression:
            return "GreaterThanExpression";
        case SyntaxKind::GreaterThanOrEqualExpression:
            return "GreaterThanOrEqualExpression";
        case SyntaxKind::EqualsExpression:
            return "EqualsExpression";
        case SyntaxKind::NotEqualsExpression:
            return "NotEqualsExpression";
        case SyntaxKind::BitwiseANDExpression:
            return "BitwiseANDExpression";
        case SyntaxKind::BitwiseXORExpression:
            return "ExclusiveXORExpression";
        case SyntaxKind::BitwiseORExpression:
            return "BitwiseORExpression";
        case SyntaxKind::LogicalANDExpression:
            return "LogicalANDExpression";
        case SyntaxKind::LogicalORExpression:
            return "LogicalORExpression";
        case SyntaxKind::ConditionalExpression:
            return "ConditionalExpression";

        /* Assignment */
        case SyntaxKind::BasicAssignmentExpression:
            return "BasicAssignmentExpression";
        case SyntaxKind::MultiplyAssignmentExpression:
            return "MultiplyAssignmentExpression";
        case SyntaxKind::DivideAssignmentExpression:
            return "DivideAssignmentExpression";
        case SyntaxKind::ModuloAssignmentExpression:
            return "ModuloAssignmentExpression";
        case SyntaxKind::AddAssignmentExpression:
            return "AddAssignmentExpression";
        case SyntaxKind::SubtractAssignmentExpression:
            return "SubtractAssignmentExpression";
        case SyntaxKind::LeftShiftAssignmentExpression:
            return "LeftShiftAssignmentExpression";
        case SyntaxKind::RightShiftAssignmentExpression:
            return "RightShiftAssignmentExpression";
        case SyntaxKind::AndAssignmentExpression:
            return "AndAssignmentExpression";
        case SyntaxKind::ExclusiveOrAssignmentExpression:
            return "ExclusiveOrAssignmentExpression";
        case SyntaxKind::OrAssignmentExpression:
            return "OrAssignmentExpression";

        /* Sequencing */
        case SyntaxKind::SequencingExpression:
            return "SequencingExpression";

        //------------//
        // Statements //
        //------------//
        case SyntaxKind::CompoundStatement:
            return "CompoundStatement";
        case SyntaxKind::DeclarationStatement:
            return "DeclarationStatement";
        case SyntaxKind::ExpressionStatement:
            return "ExpressionStatement";
        case SyntaxKind::IdentifierLabelStatement:
            return "IdentifierLabelStatement";
        case SyntaxKind::DefaultLabelStatement:
            return "DefaultLabelStatement";
        case SyntaxKind::CaseLabelStatement:
            return "CaseLabelStatement";
        case SyntaxKind::DefaultStatement:
            return "DefaultStatement";
        case SyntaxKind::IfStatement:
            return "IfStatement";
        case SyntaxKind::SwitchStatement:
            return "SwitchStatement";
        case SyntaxKind::WhileStatement:
            return "WhileStatement";
        case SyntaxKind::DoStatement:
            return "DoStatement";
        case SyntaxKind::ForStatement:
            return "ForStatement";
        case SyntaxKind::GotoStatement:
            return "GotoStatement";
        case SyntaxKind::ContinueStatement:
            return "ContinueStatement";
        case SyntaxKind::BreakStatement:
            return "BreakStatement";
        case SyntaxKind::ReturnStatement:
            return "ReturnStatement";
        case SyntaxKind::ExtGNU_AsmStatement:
            return "ExtGNU_AsmStatement";
        case SyntaxKind::ExtGNU_AsmVolatileQualifier:
            return "ExtGNU_AsmVolatileQualifier";
        case SyntaxKind::ExtGNU_AsmInlineQualifier:
            return "ExtGNU_AsmInlineQualifier";
        case SyntaxKind::ExtGNU_AsmGotoQualifier:
            return "ExtGNU_AsmGotoQualifier";
        case SyntaxKind::ExtGNU_AsmInputOperand:
            return "ExtGNU_AsmInputOperand";
        case SyntaxKind::ExtGNU_AsmOutputOperand:
            return "ExtGNU_AsmOutputOperand";

        //--------//
        // Common //
        //--------//
        case SyntaxKind::TypeName:
            return "TypeName";
        case SyntaxKind::ExpressionAsTypeReference:
            return "ExpressionAsTypeReference";
        case SyntaxKind::TypeNameAsTypeReference:
            return "TypeNameAsTypeReference";

        //-------------//
        // Ambiguities //
        //-------------//
        case SyntaxKind::AmbiguousTypeNameOrExpressionAsTypeReference:
            return "AmbiguousTypeNameOrExpressionAsTypeReference";
        case SyntaxKind::AmbiguousCastOrBinaryExpression:
            return "AmbiguousCastOrBinaryExpression";
        case SyntaxKind::AmbiguousCallOrVariableDeclaration:
            return "AmbiguousCallOrVariableDeclaration";
        case SyntaxKind::AmbiguousMultiplicationOrPointerDeclaration:
            return "AmbiguousMultiplicationOrPointerDeclaration";

//=================================================================== Misc

        case SyntaxKind::UnknownSyntax:
            return "<UNKNOWN>";
        default:
            return tokenNames[static_cast<std::uint16_t>(kind)];
    }
}

} // C
} // psy
