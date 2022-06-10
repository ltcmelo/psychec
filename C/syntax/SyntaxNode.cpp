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
        case TranslationUnit:
            return "TranslationUnit";
        case IncompleteDeclaration:
            return "IncompleteDeclaration";
        case StructDeclaration:
            return "StructDeclaration";
        case UnionDeclaration:
            return "UnionDeclaration";
        case EnumDeclaration:
            return "EnumDeclaration";
        case EnumeratorDeclaration:
            return "EnumeratorDeclaration";
        case VariableAndOrFunctionDeclaration:
            return "VariableAndOrFunctionDeclaration";
        case ParameterDeclaration:
            return "ParameterDeclaration";
        case FieldDeclaration:
            return "FieldDeclaration";
        case StaticAssertDeclaration:
            return "StaticAssertDeclaration";
        case FunctionDefinition:
            return "FunctionDefinition";
        case ExtGNU_AsmStatementDeclaration:
            return "ExtGNU_AsmStatementDeclaration";
        case ExtPSY_TemplateDeclaration:
            return "ExtPSY_TemplateDeclaration";
        case ExtKR_ParameterDeclaration:
            return "ExtKR_ParameterDeclaration";

        /* Specifiers */
        case TypedefStorageClass:
            return "TypedefStorageClass";
        case ExternStorageClass:
            return "ExternStorageClass";
        case StaticStorageClass:
            return "StaticStorageClass";
        case AutoStorageClass:
            return "AutoStorageClass";
        case RegisterStorageClass:
            return "RegisterStorageClass";
        case ThreadLocalStorageClass:
            return "ThreadLocalStorageClass";
        case ConstQualifier:
            return "ConstQualifier";
        case RestrictQualifier:
            return "RestrictQualifier";
        case VolatileQualifier:
            return "VolatileQualifier";
        case AtomicQualifier:
            return "AtomicQualifier";
        case BuiltinTypeSpecifier:
            return "BuiltinTypeSpecifier";
        case StructTypeSpecifier:
            return "StructTypeSpecifier";
        case UnionTypeSpecifier:
            return "UnionTypeSpecifier";
        case EnumTypeSpecifier:
            return "EnumTypeSpecifier";
        case AtomicTypeSpecifier:
            return "AtomicTypeSpecifier";
        case TypeDeclarationAsSpecifier:
            return "TypeDeclarationAsSpecifier";
        case InlineSpecifier:
            return "InlineSpecifier";
        case NoReturnSpecifier:
            return "NoReturnSpecifier";
        case AlignmentSpecifier:
            return "AlignmentSpecifier";
        case TypedefName:
            return "TypedefName";
        case ExtGNU_AttributeSpecifier:
            return "ExtGNU_AttributeSpecifier";
        case ExtGNU_Attribute:
            return "ExtGNU_Attribute";
        case ExtGNU_AsmLabel:
            return "ExtGNU_AsmLabel";
        case ExtGNU_Typeof:
            return "ExtGNU_Typeof";

        /* Declarators */
        case PointerDeclarator:
            return "PointerDeclarator";
        case IdentifierDeclarator:
            return "IdentifierDeclarator";
        case AbstractDeclarator:
            return "AbstractDeclarator";
        case ParenthesizedDeclarator:
            return "ParenthesizedDeclarator";
        case ArrayDeclarator:
            return "ArrayDeclarator";
        case FunctionDeclarator:
            return "FunctionDeclarator";
        case BitfieldDeclarator:
            return "BitfieldDeclarator";
        case SubscriptSuffix:
            return "SubscriptSuffix";
        case ParameterSuffix:
            return "ParameterSuffix";
        case BitfieldDeclaratorSuffix:
            return "BitfieldDeclaratorSuffix";

        /* Initializers */
        case ExpressionInitializer:
            return "ExpressionInitializer";
        case BraceEnclosedInitializer:
            return "BraceEnclosedInitializer";
        case DesignatedInitializer:
            return "DesignatedInitializer";
        case FieldDesignator:
            return "FieldDesignator";
        case ArrayDesignator:
            return "ArrayDesignator";

        case OffsetOfDesignator:
            return "OffsetOfDesignator";

        //-------------//
        // Expressions //
        //-------------//
        case IntegerConstantExpression:
            return "IntegerConstantExpression";
        case FloatingConstantExpression:
            return "FloatingConstantExpression";
        case CharacterConstantExpression:
            return "CharacterConstantExpression";
        case ImaginaryIntegerConstantExpression:
            return "ImaginaryIntegerConstantExpression";
        case ImaginaryFloatingConstantExpression:
            return "ImaginaryFloatingConstantExpression";
        case BooleanConstantExpression:
            return "BooleanLiteralExpression";
        case NULL_ConstantExpression:
            return "NULL_ConstantExpression";
        case StringLiteralExpression:
            return "StringLiteralExpression";
        case ParenthesizedExpression:
            return "ParenthesizedExpression";
        case GenericSelectionExpression:
            return "GenericSelectionExpression";
        case TypedGenericAssociation:
            return "GenericTypenameAssociation";
        case DefaultGenericAssociation:
            return "GenericDefaultAssociation";
        case ExtGNU_EnclosedCompoundStatementExpression:
            return "ExtGNU_EnclosedCompoundStatementExpression";
        case ExtGNU_ComplexValuedExpression:
            return "ExtGNU_ComplexValuedExpression";
        case ExtGNU_RealExpression:
            return "ExtGNU_RealExpression";
        case ExtGNU_ImagExpression:
            return "ExtGNU_ImagExpression";

        /* Names */
        case IdentifierName:
            return "IdentifierName";
        case PredefinedName:
            return "PredefinedName";

        /* Unary */
        case PreIncrementExpression:
            return "PreIncrementExpression";
        case PreDecrementExpression:
            return "PreDecrementExpression";
        case PostIncrementExpression:
            return "PostIncrementExpression";
        case PostDecrementExpression:
            return "PostDecrementExpression";
        case UnaryPlusExpression:
            return "UnaryPlusExpression";
        case UnaryMinusExpression:
            return "UnaryMinusExpression";
        case BitwiseNotExpression:
            return "BitwiseNotExpression";
        case LogicalNotExpression:
            return "LogicalNotExpression";

        /* Type trait */
        case SizeofExpression:
            return "SizeofExpression";
        case AlignofExpression:
            return "AlignofExpression";

        /* Pointers */
        case AddressOfExpression:
            return "AddressOfExpression";
        case PointerIndirectionExpression:
            return "PointerIndirectionExpression";

        /* Access */
        case DirectMemberAccessExpression:
            return "DirectMemberAccessExpression";
        case IndirectMemberAccessExpression:
            return "IndirectMemberAccessExpression";
        case ElementAccessExpression:
            return "ElementAccessExpression";

        /* Invocation-like */
        case CallExpression:
            return "CallExpression";
        case CompoundLiteralExpression:
            return "CompoundLiteralExpression";
        case CastExpression:
            return "CastExpression";
        case VAArgumentExpression:
            return "VAArgumentExpression";
        case OffsetOfExpression:
            return "OffsetOfExpression";
        case ExtGNU_ChooseExpression:
            return "ExtGNU_ChooseExpression";

        /* Binary */
        case MultiplyExpression:
            return "MultipliyExpression";
        case DivideExpression:
            return "DivideExpression";
        case ModuleExpression:
            return "ModuleExpression";
        case AddExpression:
            return "AddExpression";
        case SubstractExpression:
            return "SubstractExpression";
        case LeftShiftExpression:
            return "LeftShiftExpression";
        case RightShiftExpression:
            return "RightShiftExpression";
        case LessThanExpression:
            return "LessThanExpression";
        case LessThanOrEqualExpression:
            return "LessThanOrEqualExpression";
        case GreaterThanExpression:
            return "GreaterThanExpression";
        case GreaterThanOrEqualExpression:
            return "GreaterThanOrEqualExpression";
        case EqualsExpression:
            return "EqualsExpression";
        case NotEqualsExpression:
            return "NotEqualsExpression";
        case BitwiseANDExpression:
            return "BitwiseExpression";
        case BitwiseXORExpression:
            return "ExclusiveOrExpression";
        case BitwiseORExpression:
            return "BitwiseOrExpression";
        case LogicalANDExpression:
            return "LogicalAndExpression";
        case LogicalORExpression:
            return "LogicalOrExpression";
        case ConditionalExpression:
            return "ConditionalExpression";

        /* Assignment */
        case BasicAssignmentExpression:
            return "BasicAssignmentExpression";
        case MultiplyAssignmentExpression:
            return "MultiplyAssignmentExpression";
        case DivideAssignmentExpression:
            return "DivideAssignmentExpression";
        case ModuloAssignmentExpression:
            return "ModuloAssignmentExpression";
        case AddAssignmentExpression:
            return "AddAssignmentExpression";
        case SubtractAssignmentExpression:
            return "SubtractAssignmentExpression";
        case LeftShiftAssignmentExpression:
            return "LeftShiftAssignmentExpression";
        case RightShiftAssignmentExpression:
            return "RightShiftAssignmentExpression";
        case AndAssignmentExpression:
            return "AndAssignmentExpression";
        case ExclusiveOrAssignmentExpression:
            return "ExclusiveOrAssignmentExpression";
        case OrAssignmentExpression:
            return "OrAssignmentExpression";

        /* Sequencing */
        case SequencingExpression:
            return "SequencingExpression";

        //------------//
        // Statements //
        //------------//
        case CompoundStatement:
            return "CompoundStatement";
        case DeclarationStatement:
            return "DeclarationStatement";
        case ExpressionStatement:
            return "ExpressionStatement";
        case IdentifierLabelStatement:
            return "IdentifierLabelStatement";
        case DefaultLabelStatement:
            return "DefaultLabelStatement";
        case CaseLabelStatement:
            return "CaseLabelStatement";
        case DefaultStatement:
            return "DefaultStatement";
        case IfStatement:
            return "IfStatement";
        case SwitchStatement:
            return "SwitchStatement";
        case WhileStatement:
            return "WhileStatement";
        case DoStatement:
            return "DoStatement";
        case ForStatement:
            return "ForStatement";
        case GotoStatement:
            return "GotoStatement";
        case ContinueStatement:
            return "ContinueStatement";
        case BreakStatement:
            return "BreakStatement";
        case ReturnStatement:
            return "ReturnStatement";
        case ExtGNU_AsmStatement:
            return "ExtGNU_AsmStatement";
        case ExtGNU_AsmVolatileQualifier:
            return "ExtGNU_AsmVolatileQualifier";
        case ExtGNU_AsmInlineQualifier:
            return "ExtGNU_AsmInlineQualifier";
        case ExtGNU_AsmGotoQualifier:
            return "ExtGNU_AsmGotoQualifier";
        case ExtGNU_AsmInputOperand:
            return "ExtGNU_AsmInputOperand";
        case ExtGNU_AsmOutputOperand:
            return "ExtGNU_AsmOutputOperand";

        //--------//
        // Common //
        //--------//
        case TypeName:
            return "TypeName";
        case ExpressionAsTypeReference:
            return "ExpressionAsTypeReference";
        case TypeNameAsTypeReference:
            return "TypeNameAsTypeReference";

        //-------------//
        // Ambiguities //
        //-------------//
        case AmbiguousTypeNameOrExpressionAsTypeReference:
            return "AmbiguousTypeNameOrExpressionAsTypeReference";
        case AmbiguousCastOrBinaryExpression:
            return "AmbiguousCastOrBinaryExpression";
        case AmbiguousCallOrVariableDeclaration:
            return "AmbiguousCallOrVariableDeclaration";
        case AmbiguousMultiplicationOrPointerDeclaration:
            return "AmbiguousCallOrVariableDeclaration";

//=================================================================== Misc

        case UnknownSyntax:
            return "<UNKNOWN>";
        default:
            return tokenNames[kind];
    }
}

} // C
} // psy
