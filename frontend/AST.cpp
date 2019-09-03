// Copyright (c) 2008 Roberto Raggi <roberto.raggi@gmail.com>
// Copyright (c) 2016 Leandro T. C. Melo <ltcmelo@gmail.com>
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

#include "AST.h"
#include "ASTVisitor.h"
#include "ASTMatcher.h"
#include "MemoryPool.h"
#include <algorithm>
#include <cstddef>

using namespace psyche;

AST::AST()
{}

AST::~AST()
{}

void AST::accept(ASTVisitor* visitor)
{
    if (visitor->preVisit(this))
        accept0(visitor);
    visitor->postVisit(this);
}

bool AST::match(AST* ast, AST* pattern, ASTMatcher* matcher)
{
    if (ast == pattern)
        return true;

    else if (! ast || ! pattern)
        return false;

    return ast->match(pattern, matcher);
}

bool AST::match(AST* pattern, ASTMatcher* matcher)
{
    return match0(pattern, matcher);
}

unsigned GnuAttributeSpecifierAST::firstToken() const
{
    return attribute_token;
}

unsigned BaseSpecifierAST::firstToken() const
{
    if (virtual_token && access_specifier_token)
        return std::min(virtual_token, access_specifier_token);
    if (virtual_token)
        return virtual_token;
    if (access_specifier_token)
        return access_specifier_token;
    if (name)
        return name->firstToken();
    // assert?
    return 0;
}

unsigned BaseSpecifierAST::lastToken() const
{
    if (ellipsis_token)
        return ellipsis_token;
    else if (name)
        return name->lastToken();
    else if (virtual_token && access_specifier_token)
        return std::max(virtual_token, access_specifier_token) + 1;
    else if (virtual_token)
        return virtual_token + 1;
    else if (access_specifier_token)
        return access_specifier_token + 1;
    // assert?
    return 0;
}


unsigned AccessDeclarationAST::firstToken() const
{
    if (access_specifier_token)
        return access_specifier_token;
    if (slots_token)
        return slots_token;
    if (colon_token)
        return colon_token;
    return 0;
}


unsigned AccessDeclarationAST::lastToken() const
{
    if (colon_token)
        return colon_token + 1;
    if (slots_token)
        return slots_token + 1;
    if (access_specifier_token)
        return access_specifier_token + 1;
    return 1;
}


unsigned ArrayAccessAST::firstToken() const
{
    if (base_expression)
        if (unsigned candidate = base_expression->firstToken())
            return candidate;
    if (lbracket_token)
        return lbracket_token;
    if (expression)
        if (unsigned candidate = expression->firstToken())
            return candidate;
    if (rbracket_token)
        return rbracket_token;
    return 0;
}


unsigned ArrayAccessAST::lastToken() const
{
    if (rbracket_token)
        return rbracket_token + 1;
    if (expression)
        if (unsigned candidate = expression->lastToken())
            return candidate;
    if (lbracket_token)
        return lbracket_token + 1;
    if (base_expression)
        if (unsigned candidate = base_expression->lastToken())
            return candidate;
    return 1;
}


unsigned ArrayDeclaratorAST::firstToken() const
{
    if (lbracket_token)
        return lbracket_token;
    if (expression)
        if (unsigned candidate = expression->firstToken())
            return candidate;
    if (rbracket_token)
        return rbracket_token;
    return 0;
}


unsigned ArrayDeclaratorAST::lastToken() const
{
    if (rbracket_token)
        return rbracket_token + 1;
    if (expression)
        if (unsigned candidate = expression->lastToken())
            return candidate;
    if (lbracket_token)
        return lbracket_token + 1;
    return 1;
}


unsigned ArrayInitializerAST::firstToken() const
{
    if (lbrace_token)
        return lbrace_token;
    if (expression_list)
        if (unsigned candidate = expression_list->firstToken())
            return candidate;
    if (rbrace_token)
        return rbrace_token;
    return 0;
}


unsigned ArrayInitializerAST::lastToken() const
{
    if (rbrace_token)
        return rbrace_token + 1;
    if (expression_list)
        if (unsigned candidate = expression_list->lastToken())
            return candidate;
    if (lbrace_token)
        return lbrace_token + 1;
    return 1;
}


unsigned AsmDefinitionAST::firstToken() const
{
    if (asm_token)
        return asm_token;
    if (volatile_token)
        return volatile_token;
    if (lparen_token)
        return lparen_token;
    if (rparen_token)
        return rparen_token;
    if (semicolon_token)
        return semicolon_token;
    return 0;
}


unsigned AsmDefinitionAST::lastToken() const
{
    if (semicolon_token)
        return semicolon_token + 1;
    if (rparen_token)
        return rparen_token + 1;
    if (lparen_token)
        return lparen_token + 1;
    if (volatile_token)
        return volatile_token + 1;
    if (asm_token)
        return asm_token + 1;
    return 1;
}

unsigned GenericsDeclarationAST::firstToken() const
{
    if (generics_token)
        return generics_token;
    if (declaration)
        if (unsigned candidate = declaration->firstToken())
            return candidate;
    return 0;
}

unsigned GenericsDeclarationAST::lastToken() const
{
    if (declaration)
        if (unsigned candidate = declaration->lastToken())
            return candidate;
    if (generics_token)
        return generics_token + 1;
    return 1;
}

unsigned GnuAttributeAST::firstToken() const
{
    if (identifier_token)
        return identifier_token;
    if (lparen_token)
        return lparen_token;
    if (tag_token)
        return tag_token;
    if (expression_list)
        if (unsigned candidate = expression_list->firstToken())
            return candidate;
    if (rparen_token)
        return rparen_token;
    return 0;
}


unsigned GnuAttributeAST::lastToken() const
{
    if (rparen_token)
        return rparen_token + 1;
    if (expression_list)
        if (unsigned candidate = expression_list->lastToken())
            return candidate;
    if (tag_token)
        return tag_token + 1;
    if (lparen_token)
        return lparen_token + 1;
    if (identifier_token)
        return identifier_token + 1;
    return 1;
}


unsigned BinaryExpressionAST::firstToken() const
{
    if (left_expression)
        if (unsigned candidate = left_expression->firstToken())
            return candidate;
    if (binary_op_token)
        return binary_op_token;
    if (right_expression)
        if (unsigned candidate = right_expression->firstToken())
            return candidate;
    return 0;
}


unsigned BinaryExpressionAST::lastToken() const
{
    if (right_expression)
        if (unsigned candidate = right_expression->lastToken())
            return candidate;
    if (binary_op_token)
        return binary_op_token + 1;
    if (left_expression)
        if (unsigned candidate = left_expression->lastToken())
            return candidate;
    return 1;
}


unsigned BoolLiteralAST::firstToken() const
{
    if (literal_token)
        return literal_token;
    return 0;
}


unsigned BoolLiteralAST::lastToken() const
{
    if (literal_token)
        return literal_token + 1;
    return 1;
}


unsigned BracedInitializerAST::firstToken() const
{
    if (lbrace_token)
        return lbrace_token;
    if (expression_list)
        if (unsigned candidate = expression_list->firstToken())
            return candidate;
    if (comma_token)
        return comma_token;
    if (rbrace_token)
        return rbrace_token;
    return 0;
}


unsigned BracedInitializerAST::lastToken() const
{
    if (rbrace_token)
        return rbrace_token + 1;
    if (comma_token)
        return comma_token + 1;
    if (expression_list)
        if (unsigned candidate = expression_list->lastToken())
            return candidate;
    if (lbrace_token)
        return lbrace_token + 1;
    return 1;
}


unsigned BreakStatementAST::firstToken() const
{
    if (break_token)
        return break_token;
    if (semicolon_token)
        return semicolon_token;
    return 0;
}


unsigned BreakStatementAST::lastToken() const
{
    if (semicolon_token)
        return semicolon_token + 1;
    if (break_token)
        return break_token + 1;
    return 1;
}


unsigned CallAST::firstToken() const
{
    if (base_expression)
        if (unsigned candidate = base_expression->firstToken())
            return candidate;
    if (lparen_token)
        return lparen_token;
    if (expression_list)
        if (unsigned candidate = expression_list->firstToken())
            return candidate;
    if (rparen_token)
        return rparen_token;
    return 0;
}


unsigned CallAST::lastToken() const
{
    if (rparen_token)
        return rparen_token + 1;
    if (expression_list)
        if (unsigned candidate = expression_list->lastToken())
            return candidate;
    if (lparen_token)
        return lparen_token + 1;
    if (base_expression)
        if (unsigned candidate = base_expression->lastToken())
            return candidate;
    return 1;
}


unsigned CaptureAST::firstToken() const
{
    if (amper_token)
        return amper_token;
    if (identifier)
        if (unsigned candidate = identifier->firstToken())
            return candidate;
    return 0;
}


unsigned CaptureAST::lastToken() const
{
    if (identifier)
        if (unsigned candidate = identifier->lastToken())
            return candidate;
    if (amper_token)
        return amper_token + 1;
    return 1;
}


unsigned CaseStatementAST::firstToken() const
{
    if (case_token)
        return case_token;
    if (expression)
        if (unsigned candidate = expression->firstToken())
            return candidate;
    if (colon_token)
        return colon_token;
    if (statement)
        if (unsigned candidate = statement->firstToken())
            return candidate;
    return 0;
}


unsigned CaseStatementAST::lastToken() const
{
    if (statement)
        if (unsigned candidate = statement->lastToken())
            return candidate;
    if (colon_token)
        return colon_token + 1;
    if (expression)
        if (unsigned candidate = expression->lastToken())
            return candidate;
    if (case_token)
        return case_token + 1;
    return 1;
}


unsigned CastExpressionAST::firstToken() const
{
    if (lparen_token)
        return lparen_token;
    if (type_id)
        if (unsigned candidate = type_id->firstToken())
            return candidate;
    if (rparen_token)
        return rparen_token;
    if (expression)
        if (unsigned candidate = expression->firstToken())
            return candidate;
    return 0;
}


unsigned CastExpressionAST::lastToken() const
{
    if (expression)
        if (unsigned candidate = expression->lastToken())
            return candidate;
    if (rparen_token)
        return rparen_token + 1;
    if (type_id)
        if (unsigned candidate = type_id->lastToken())
            return candidate;
    if (lparen_token)
        return lparen_token + 1;
    return 1;
}


unsigned CatchClauseAST::firstToken() const
{
    if (catch_token)
        return catch_token;
    if (lparen_token)
        return lparen_token;
    if (exception_declaration)
        if (unsigned candidate = exception_declaration->firstToken())
            return candidate;
    if (rparen_token)
        return rparen_token;
    if (statement)
        if (unsigned candidate = statement->firstToken())
            return candidate;
    return 0;
}


unsigned CatchClauseAST::lastToken() const
{
    if (statement)
        if (unsigned candidate = statement->lastToken())
            return candidate;
    if (rparen_token)
        return rparen_token + 1;
    if (exception_declaration)
        if (unsigned candidate = exception_declaration->lastToken())
            return candidate;
    if (lparen_token)
        return lparen_token + 1;
    if (catch_token)
        return catch_token + 1;
    return 1;
}


unsigned ClassSpecifierAST::firstToken() const
{
    if (classkey_token)
        return classkey_token;
    if (attribute_list)
        if (unsigned candidate = attribute_list->firstToken())
            return candidate;
    if (name)
        if (unsigned candidate = name->firstToken())
            return candidate;
    if (final_token)
        return final_token;
    if (colon_token)
        return colon_token;
    if (base_clause_list)
        if (unsigned candidate = base_clause_list->firstToken())
            return candidate;
    if (dot_dot_dot_token)
        return dot_dot_dot_token;
    if (lbrace_token)
        return lbrace_token;
    if (member_specifier_list)
        if (unsigned candidate = member_specifier_list->firstToken())
            return candidate;
    if (rbrace_token)
        return rbrace_token;
    return 0;
}


unsigned ClassSpecifierAST::lastToken() const
{
    if (rbrace_token)
        return rbrace_token + 1;
    if (member_specifier_list)
        if (unsigned candidate = member_specifier_list->lastToken())
            return candidate;
    if (lbrace_token)
        return lbrace_token + 1;
    if (dot_dot_dot_token)
        return dot_dot_dot_token + 1;
    if (base_clause_list)
        if (unsigned candidate = base_clause_list->lastToken())
            return candidate;
    if (colon_token)
        return colon_token + 1;
    if (final_token)
        return final_token + 1;
    if (name)
        if (unsigned candidate = name->lastToken())
            return candidate;
    if (attribute_list)
        if (unsigned candidate = attribute_list->lastToken())
            return candidate;
    if (classkey_token)
        return classkey_token + 1;
    return 1;
}


unsigned CompoundExpressionAST::firstToken() const
{
    if (lparen_token)
        return lparen_token;
    if (statement)
        if (unsigned candidate = statement->firstToken())
            return candidate;
    if (rparen_token)
        return rparen_token;
    return 0;
}


unsigned CompoundExpressionAST::lastToken() const
{
    if (rparen_token)
        return rparen_token + 1;
    if (statement)
        if (unsigned candidate = statement->lastToken())
            return candidate;
    if (lparen_token)
        return lparen_token + 1;
    return 1;
}


unsigned CompoundLiteralAST::firstToken() const
{
    if (lparen_token)
        return lparen_token;
    if (type_id)
        if (unsigned candidate = type_id->firstToken())
            return candidate;
    if (rparen_token)
        return rparen_token;
    if (initializer)
        if (unsigned candidate = initializer->firstToken())
            return candidate;
    return 0;
}


unsigned CompoundLiteralAST::lastToken() const
{
    if (initializer)
        if (unsigned candidate = initializer->lastToken())
            return candidate;
    if (rparen_token)
        return rparen_token + 1;
    if (type_id)
        if (unsigned candidate = type_id->lastToken())
            return candidate;
    if (lparen_token)
        return lparen_token + 1;
    return 1;
}


unsigned CompoundStatementAST::firstToken() const
{
    if (lbrace_token)
        return lbrace_token;
    if (statement_list)
        if (unsigned candidate = statement_list->firstToken())
            return candidate;
    if (rbrace_token)
        return rbrace_token;
    return 0;
}


unsigned CompoundStatementAST::lastToken() const
{
    if (rbrace_token)
        return rbrace_token + 1;
    if (statement_list)
        if (unsigned candidate = statement_list->lastToken())
            return candidate;
    if (lbrace_token)
        return lbrace_token + 1;
    return 1;
}


unsigned ConditionAST::firstToken() const
{
    if (type_specifier_list)
        if (unsigned candidate = type_specifier_list->firstToken())
            return candidate;
    if (declarator)
        if (unsigned candidate = declarator->firstToken())
            return candidate;
    return 0;
}


unsigned ConditionAST::lastToken() const
{
    if (declarator)
        if (unsigned candidate = declarator->lastToken())
            return candidate;
    if (type_specifier_list)
        if (unsigned candidate = type_specifier_list->lastToken())
            return candidate;
    return 1;
}


unsigned ConditionalExpressionAST::firstToken() const
{
    if (condition)
        if (unsigned candidate = condition->firstToken())
            return candidate;
    if (question_token)
        return question_token;
    if (left_expression)
        if (unsigned candidate = left_expression->firstToken())
            return candidate;
    if (colon_token)
        return colon_token;
    if (right_expression)
        if (unsigned candidate = right_expression->firstToken())
            return candidate;
    return 0;
}


unsigned ConditionalExpressionAST::lastToken() const
{
    if (right_expression)
        if (unsigned candidate = right_expression->lastToken())
            return candidate;
    if (colon_token)
        return colon_token + 1;
    if (left_expression)
        if (unsigned candidate = left_expression->lastToken())
            return candidate;
    if (question_token)
        return question_token + 1;
    if (condition)
        if (unsigned candidate = condition->lastToken())
            return candidate;
    return 1;
}


unsigned ContinueStatementAST::firstToken() const
{
    if (continue_token)
        return continue_token;
    if (semicolon_token)
        return semicolon_token;
    return 0;
}


unsigned ContinueStatementAST::lastToken() const
{
    if (semicolon_token)
        return semicolon_token + 1;
    if (continue_token)
        return continue_token + 1;
    return 1;
}


unsigned ConversionFunctionIdAST::firstToken() const
{
    if (operator_token)
        return operator_token;
    if (type_specifier_list)
        if (unsigned candidate = type_specifier_list->firstToken())
            return candidate;
    if (ptr_operator_list)
        if (unsigned candidate = ptr_operator_list->firstToken())
            return candidate;
    return 0;
}


unsigned ConversionFunctionIdAST::lastToken() const
{
    if (ptr_operator_list)
        if (unsigned candidate = ptr_operator_list->lastToken())
            return candidate;
    if (type_specifier_list)
        if (unsigned candidate = type_specifier_list->lastToken())
            return candidate;
    if (operator_token)
        return operator_token + 1;
    return 1;
}


unsigned CppCastExpressionAST::firstToken() const
{
    if (cast_token)
        return cast_token;
    if (less_token)
        return less_token;
    if (type_id)
        if (unsigned candidate = type_id->firstToken())
            return candidate;
    if (greater_token)
        return greater_token;
    if (lparen_token)
        return lparen_token;
    if (expression)
        if (unsigned candidate = expression->firstToken())
            return candidate;
    if (rparen_token)
        return rparen_token;
    return 0;
}


unsigned CppCastExpressionAST::lastToken() const
{
    if (rparen_token)
        return rparen_token + 1;
    if (expression)
        if (unsigned candidate = expression->lastToken())
            return candidate;
    if (lparen_token)
        return lparen_token + 1;
    if (greater_token)
        return greater_token + 1;
    if (type_id)
        if (unsigned candidate = type_id->lastToken())
            return candidate;
    if (less_token)
        return less_token + 1;
    if (cast_token)
        return cast_token + 1;
    return 1;
}


unsigned CtorInitializerAST::firstToken() const
{
    if (colon_token)
        return colon_token;
    if (member_initializer_list)
        if (unsigned candidate = member_initializer_list->firstToken())
            return candidate;
    if (dot_dot_dot_token)
        return dot_dot_dot_token;
    return 0;
}


unsigned CtorInitializerAST::lastToken() const
{
    if (dot_dot_dot_token)
        return dot_dot_dot_token + 1;
    if (member_initializer_list)
        if (unsigned candidate = member_initializer_list->lastToken())
            return candidate;
    if (colon_token)
        return colon_token + 1;
    return 1;
}


unsigned DeclarationStatementAST::firstToken() const
{
    if (declaration)
        if (unsigned candidate = declaration->firstToken())
            return candidate;
    return 0;
}


unsigned DeclarationStatementAST::lastToken() const
{
    if (declaration)
        if (unsigned candidate = declaration->lastToken())
            return candidate;
    return 1;
}


unsigned DeclaratorAST::firstToken() const
{
    if (attribute_list)
        if (unsigned candidate = attribute_list->firstToken())
            return candidate;
    if (ptr_operator_list)
        if (unsigned candidate = ptr_operator_list->firstToken())
            return candidate;
    if (core_declarator)
        if (unsigned candidate = core_declarator->firstToken())
            return candidate;
    if (postfix_declarator_list)
        if (unsigned candidate = postfix_declarator_list->firstToken())
            return candidate;
    if (post_attribute_list)
        if (unsigned candidate = post_attribute_list->firstToken())
            return candidate;
    if (equal_token)
        return equal_token;
    if (initializer)
        if (unsigned candidate = initializer->firstToken())
            return candidate;
    return 0;
}


unsigned DeclaratorAST::lastToken() const
{
    if (initializer)
        if (unsigned candidate = initializer->lastToken())
            return candidate;
    if (equal_token)
        return equal_token + 1;
    if (post_attribute_list)
        if (unsigned candidate = post_attribute_list->lastToken())
            return candidate;
    if (postfix_declarator_list)
        if (unsigned candidate = postfix_declarator_list->lastToken())
            return candidate;
    if (core_declarator)
        if (unsigned candidate = core_declarator->lastToken())
            return candidate;
    if (ptr_operator_list)
        if (unsigned candidate = ptr_operator_list->lastToken())
            return candidate;
    if (attribute_list)
        if (unsigned candidate = attribute_list->lastToken())
            return candidate;
    return 1;
}


unsigned DeclaratorIdAST::firstToken() const
{
    if (dot_dot_dot_token)
        return dot_dot_dot_token;
    if (name)
        if (unsigned candidate = name->firstToken())
            return candidate;
    return 0;
}


unsigned DeclaratorIdAST::lastToken() const
{
    if (name)
        if (unsigned candidate = name->lastToken())
            return candidate;
    if (dot_dot_dot_token)
        return dot_dot_dot_token + 1;
    return 1;
}


unsigned DeleteExpressionAST::firstToken() const
{
    if (scope_token)
        return scope_token;
    if (delete_token)
        return delete_token;
    if (lbracket_token)
        return lbracket_token;
    if (rbracket_token)
        return rbracket_token;
    if (expression)
        if (unsigned candidate = expression->firstToken())
            return candidate;
    return 0;
}


unsigned DeleteExpressionAST::lastToken() const
{
    if (expression)
        if (unsigned candidate = expression->lastToken())
            return candidate;
    if (rbracket_token)
        return rbracket_token + 1;
    if (lbracket_token)
        return lbracket_token + 1;
    if (delete_token)
        return delete_token + 1;
    if (scope_token)
        return scope_token + 1;
    return 1;
}


unsigned DestructorNameAST::firstToken() const
{
    if (tilde_token)
        return tilde_token;
    if (unqualified_name)
        if (unsigned candidate = unqualified_name->firstToken())
            return candidate;
    return 0;
}


unsigned DestructorNameAST::lastToken() const
{
    if (unqualified_name)
        if (unsigned candidate = unqualified_name->lastToken())
            return candidate;
    if (tilde_token)
        return tilde_token + 1;
    return 1;
}


unsigned DoStatementAST::firstToken() const
{
    if (do_token)
        return do_token;
    if (statement)
        if (unsigned candidate = statement->firstToken())
            return candidate;
    if (while_token)
        return while_token;
    if (lparen_token)
        return lparen_token;
    if (expression)
        if (unsigned candidate = expression->firstToken())
            return candidate;
    if (rparen_token)
        return rparen_token;
    if (semicolon_token)
        return semicolon_token;
    return 0;
}


unsigned DoStatementAST::lastToken() const
{
    if (semicolon_token)
        return semicolon_token + 1;
    if (rparen_token)
        return rparen_token + 1;
    if (expression)
        if (unsigned candidate = expression->lastToken())
            return candidate;
    if (lparen_token)
        return lparen_token + 1;
    if (while_token)
        return while_token + 1;
    if (statement)
        if (unsigned candidate = statement->lastToken())
            return candidate;
    if (do_token)
        return do_token + 1;
    return 1;
}


unsigned ElaboratedTypeSpecifierAST::firstToken() const
{
    if (attribute_list)
        if (unsigned candidate = attribute_list->firstToken())
            return candidate;
    if (name)
        if (unsigned candidate = name->firstToken())
            return candidate;
    return 0;
}


unsigned ElaboratedTypeSpecifierAST::lastToken() const
{
    if (name)
        if (unsigned candidate = name->lastToken())
            return candidate + 1;
    if (attribute_list)
        if (unsigned candidate = attribute_list->lastToken())
            return candidate + 1;
    return 1;
}


unsigned EmptyDeclarationAST::firstToken() const
{
    if (semicolon_token)
        return semicolon_token;
    return 0;
}


unsigned EmptyDeclarationAST::lastToken() const
{
    if (semicolon_token)
        return semicolon_token + 1;
    return 1;
}


unsigned EnumSpecifierAST::firstToken() const
{
    if (enum_token)
        return enum_token;
    if (key_token)
        return key_token;
    if (name)
        if (unsigned candidate = name->firstToken())
            return candidate;
    if (colon_token)
        return colon_token;
    if (type_specifier_list)
        if (unsigned candidate = type_specifier_list->firstToken())
            return candidate;
    if (lbrace_token)
        return lbrace_token;
    if (enumerator_list)
        if (unsigned candidate = enumerator_list->firstToken())
            return candidate;
    if (stray_comma_token)
        return stray_comma_token;
    if (rbrace_token)
        return rbrace_token;
    return 0;
}


unsigned EnumSpecifierAST::lastToken() const
{
    if (rbrace_token)
        return rbrace_token + 1;
    if (stray_comma_token)
        return stray_comma_token + 1;
    if (enumerator_list)
        if (unsigned candidate = enumerator_list->lastToken())
            return candidate;
    if (lbrace_token)
        return lbrace_token + 1;
    if (type_specifier_list)
        if (unsigned candidate = type_specifier_list->lastToken())
            return candidate;
    if (colon_token)
        return colon_token + 1;
    if (name)
        if (unsigned candidate = name->lastToken())
            return candidate;
    if (key_token)
        return key_token + 1;
    if (enum_token)
        return enum_token + 1;
    return 1;
}


unsigned EnumeratorAST::firstToken() const
{
    if (identifier_token)
        return identifier_token;
    if (attribute_list)
        if (unsigned candidate = attribute_list->firstToken())
            return candidate;
    if (equal_token)
        return equal_token;
    if (expression)
        if (unsigned candidate = expression->firstToken())
            return candidate;
    return 0;
}


unsigned EnumeratorAST::lastToken() const
{
    if (expression)
        if (unsigned candidate = expression->lastToken())
            return candidate;
    if (equal_token)
        return equal_token + 1;
    if (attribute_list)
        if (unsigned candidate = attribute_list->firstToken())
            return candidate;
    if (identifier_token)
        return identifier_token + 1;
    return 1;
}


unsigned ExceptionDeclarationAST::firstToken() const
{
    if (type_specifier_list)
        if (unsigned candidate = type_specifier_list->firstToken())
            return candidate;
    if (declarator)
        if (unsigned candidate = declarator->firstToken())
            return candidate;
    if (dot_dot_dot_token)
        return dot_dot_dot_token;
    return 0;
}


unsigned ExceptionDeclarationAST::lastToken() const
{
    if (dot_dot_dot_token)
        return dot_dot_dot_token + 1;
    if (declarator)
        if (unsigned candidate = declarator->lastToken())
            return candidate;
    if (type_specifier_list)
        if (unsigned candidate = type_specifier_list->lastToken())
            return candidate;
    return 1;
}


unsigned DynamicExceptionSpecificationAST::firstToken() const
{
    if (throw_token)
        return throw_token;
    if (lparen_token)
        return lparen_token;
    if (dot_dot_dot_token)
        return dot_dot_dot_token;
    if (type_id_list)
        if (unsigned candidate = type_id_list->firstToken())
            return candidate;
    if (rparen_token)
        return rparen_token;
    return 0;
}


unsigned DynamicExceptionSpecificationAST::lastToken() const
{
    if (rparen_token)
        return rparen_token + 1;
    if (type_id_list)
        if (unsigned candidate = type_id_list->lastToken())
            return candidate;
    if (dot_dot_dot_token)
        return dot_dot_dot_token + 1;
    if (lparen_token)
        return lparen_token + 1;
    if (throw_token)
        return throw_token + 1;
    return 1;
}


unsigned ExpressionOrDeclarationStatementAST::firstToken() const
{
    if (expression)
        if (unsigned candidate = expression->firstToken())
            return candidate;
    if (declaration)
        if (unsigned candidate = declaration->firstToken())
            return candidate;
    return 0;
}


unsigned ExpressionOrDeclarationStatementAST::lastToken() const
{
    if (declaration)
        if (unsigned candidate = declaration->lastToken())
            return candidate;
    if (expression)
        if (unsigned candidate = expression->lastToken())
            return candidate;
    return 1;
}


unsigned ExpressionStatementAST::firstToken() const
{
    if (expression)
        if (unsigned candidate = expression->firstToken())
            return candidate;
    if (semicolon_token)
        return semicolon_token;
    return 0;
}


unsigned ExpressionStatementAST::lastToken() const
{
    if (semicolon_token)
        return semicolon_token + 1;
    if (expression)
        if (unsigned candidate = expression->lastToken())
            return candidate;
    return 1;
}


unsigned ForStatementAST::firstToken() const
{
    if (for_token)
        return for_token;
    if (lparen_token)
        return lparen_token;
    if (initializer)
        if (unsigned candidate = initializer->firstToken())
            return candidate;
    if (condition)
        if (unsigned candidate = condition->firstToken())
            return candidate;
    if (semicolon_token)
        return semicolon_token;
    if (expression)
        if (unsigned candidate = expression->firstToken())
            return candidate;
    if (rparen_token)
        return rparen_token;
    if (statement)
        if (unsigned candidate = statement->firstToken())
            return candidate;
    return 0;
}


unsigned ForStatementAST::lastToken() const
{
    if (statement)
        if (unsigned candidate = statement->lastToken())
            return candidate;
    if (rparen_token)
        return rparen_token + 1;
    if (expression)
        if (unsigned candidate = expression->lastToken())
            return candidate;
    if (semicolon_token)
        return semicolon_token + 1;
    if (condition)
        if (unsigned candidate = condition->lastToken())
            return candidate;
    if (initializer)
        if (unsigned candidate = initializer->lastToken())
            return candidate;
    if (lparen_token)
        return lparen_token + 1;
    if (for_token)
        return for_token + 1;
    return 1;
}


unsigned ForeachStatementAST::firstToken() const
{
    if (foreach_token)
        return foreach_token;
    if (lparen_token)
        return lparen_token;
    if (type_specifier_list)
        if (unsigned candidate = type_specifier_list->firstToken())
            return candidate;
    if (declarator)
        if (unsigned candidate = declarator->firstToken())
            return candidate;
    if (initializer)
        if (unsigned candidate = initializer->firstToken())
            return candidate;
    if (comma_token)
        return comma_token;
    if (expression)
        if (unsigned candidate = expression->firstToken())
            return candidate;
    if (rparen_token)
        return rparen_token;
    if (statement)
        if (unsigned candidate = statement->firstToken())
            return candidate;
    return 0;
}


unsigned ForeachStatementAST::lastToken() const
{
    if (statement)
        if (unsigned candidate = statement->lastToken())
            return candidate;
    if (rparen_token)
        return rparen_token + 1;
    if (expression)
        if (unsigned candidate = expression->lastToken())
            return candidate;
    if (comma_token)
        return comma_token + 1;
    if (initializer)
        if (unsigned candidate = initializer->lastToken())
            return candidate;
    if (declarator)
        if (unsigned candidate = declarator->lastToken())
            return candidate;
    if (type_specifier_list)
        if (unsigned candidate = type_specifier_list->lastToken())
            return candidate;
    if (lparen_token)
        return lparen_token + 1;
    if (foreach_token)
        return foreach_token + 1;
    return 1;
}


unsigned BitfieldDeclaratorAST::firstToken() const
{
    if (colon_token)
        return colon_token;
    return 0;
}


unsigned BitfieldDeclaratorAST::lastToken() const
{
    if (colon_token)
        return colon_token + 1;
    return 1;
}


unsigned FunctionDeclaratorAST::firstToken() const
{
    if (lparen_token)
        return lparen_token;
    if (parameter_declaration_clause)
        if (unsigned candidate = parameter_declaration_clause->firstToken())
            return candidate;
    if (rparen_token)
        return rparen_token;
    if (cv_qualifier_list)
        if (unsigned candidate = cv_qualifier_list->firstToken())
            return candidate;
    if (ref_qualifier_token)
        return ref_qualifier_token;
    if (exception_specification)
        if (unsigned candidate = exception_specification->firstToken())
            return candidate;
    if (trailing_return_type)
        if (unsigned candidate = trailing_return_type->firstToken())
            return candidate;
    if (as_cpp_initializer)
        if (unsigned candidate = as_cpp_initializer->firstToken())
            return candidate;
    return 0;
}


unsigned FunctionDeclaratorAST::lastToken() const
{
    if (as_cpp_initializer)
        if (unsigned candidate = as_cpp_initializer->lastToken())
            return candidate;
    if (trailing_return_type)
        if (unsigned candidate = trailing_return_type->lastToken())
            return candidate;
    if (exception_specification)
        if (unsigned candidate = exception_specification->lastToken())
            return candidate;
    if (ref_qualifier_token)
        return ref_qualifier_token + 1;
    if (cv_qualifier_list)
        if (unsigned candidate = cv_qualifier_list->lastToken())
            return candidate;
    if (rparen_token)
        return rparen_token + 1;
    if (parameter_declaration_clause)
        if (unsigned candidate = parameter_declaration_clause->lastToken())
            return candidate;
    if (lparen_token)
        return lparen_token + 1;
    return 1;
}


unsigned FunctionDefinitionAST::firstToken() const
{
    if (decl_specifier_list)
        if (unsigned candidate = decl_specifier_list->firstToken())
            return candidate;
    if (declarator)
        if (unsigned candidate = declarator->firstToken())
            return candidate;
    if (ctor_initializer)
        if (unsigned candidate = ctor_initializer->firstToken())
            return candidate;
    if (function_body)
        if (unsigned candidate = function_body->firstToken())
            return candidate;
    return 0;
}


unsigned FunctionDefinitionAST::lastToken() const
{
    if (function_body)
        if (unsigned candidate = function_body->lastToken())
            return candidate;
    if (ctor_initializer)
        if (unsigned candidate = ctor_initializer->lastToken())
            return candidate;
    if (declarator)
        if (unsigned candidate = declarator->lastToken())
            return candidate;
    if (decl_specifier_list)
        if (unsigned candidate = decl_specifier_list->lastToken())
            return candidate;
    return 1;
}


unsigned GotoStatementAST::firstToken() const
{
    if (goto_token)
        return goto_token;
    if (identifier_token)
        return identifier_token;
    if (semicolon_token)
        return semicolon_token;
    return 0;
}


unsigned GotoStatementAST::lastToken() const
{
    if (semicolon_token)
        return semicolon_token + 1;
    if (identifier_token)
        return identifier_token + 1;
    if (goto_token)
        return goto_token + 1;
    return 1;
}


unsigned IdExpressionAST::firstToken() const
{
    if (name)
        if (unsigned candidate = name->firstToken())
            return candidate;
    return 0;
}


unsigned IdExpressionAST::lastToken() const
{
    if (name)
        if (unsigned candidate = name->lastToken())
            return candidate;
    return 1;
}


unsigned IfStatementAST::firstToken() const
{
    if (if_token)
        return if_token;
    if (lparen_token)
        return lparen_token;
    if (condition)
        if (unsigned candidate = condition->firstToken())
            return candidate;
    if (rparen_token)
        return rparen_token;
    if (statement)
        if (unsigned candidate = statement->firstToken())
            return candidate;
    if (else_token)
        return else_token;
    if (else_statement)
        if (unsigned candidate = else_statement->firstToken())
            return candidate;
    return 0;
}


unsigned IfStatementAST::lastToken() const
{
    if (else_statement)
        if (unsigned candidate = else_statement->lastToken())
            return candidate;
    if (else_token)
        return else_token + 1;
    if (statement)
        if (unsigned candidate = statement->lastToken())
            return candidate;
    if (rparen_token)
        return rparen_token + 1;
    if (condition)
        if (unsigned candidate = condition->lastToken())
            return candidate;
    if (lparen_token)
        return lparen_token + 1;
    if (if_token)
        return if_token + 1;
    return 1;
}


unsigned LabeledStatementAST::firstToken() const
{
    if (label_token)
        return label_token;
    if (colon_token)
        return colon_token;
    if (statement)
        if (unsigned candidate = statement->firstToken())
            return candidate;
    return 0;
}


unsigned LabeledStatementAST::lastToken() const
{
    if (statement)
        if (unsigned candidate = statement->lastToken())
            return candidate;
    if (colon_token)
        return colon_token + 1;
    if (label_token)
        return label_token + 1;
    return 1;
}


unsigned LambdaCaptureAST::firstToken() const
{
    if (default_capture_token)
        return default_capture_token;
    if (capture_list)
        if (unsigned candidate = capture_list->firstToken())
            return candidate;
    return 0;
}


unsigned LambdaCaptureAST::lastToken() const
{
    if (capture_list)
        if (unsigned candidate = capture_list->lastToken())
            return candidate;
    if (default_capture_token)
        return default_capture_token + 1;
    return 1;
}


unsigned LambdaDeclaratorAST::firstToken() const
{
    if (lparen_token)
        return lparen_token;
    if (parameter_declaration_clause)
        if (unsigned candidate = parameter_declaration_clause->firstToken())
            return candidate;
    if (rparen_token)
        return rparen_token;
    if (attributes)
        if (unsigned candidate = attributes->firstToken())
            return candidate;
    if (mutable_token)
        return mutable_token;
    if (exception_specification)
        if (unsigned candidate = exception_specification->firstToken())
            return candidate;
    if (trailing_return_type)
        if (unsigned candidate = trailing_return_type->firstToken())
            return candidate;
    return 0;
}


unsigned LambdaDeclaratorAST::lastToken() const
{
    if (trailing_return_type)
        if (unsigned candidate = trailing_return_type->lastToken())
            return candidate;
    if (exception_specification)
        if (unsigned candidate = exception_specification->lastToken())
            return candidate;
    if (mutable_token)
        return mutable_token + 1;
    if (attributes)
        if (unsigned candidate = attributes->lastToken())
            return candidate;
    if (rparen_token)
        return rparen_token + 1;
    if (parameter_declaration_clause)
        if (unsigned candidate = parameter_declaration_clause->lastToken())
            return candidate;
    if (lparen_token)
        return lparen_token + 1;
    return 1;
}


unsigned LambdaExpressionAST::firstToken() const
{
    if (lambda_introducer)
        if (unsigned candidate = lambda_introducer->firstToken())
            return candidate;
    if (lambda_declarator)
        if (unsigned candidate = lambda_declarator->firstToken())
            return candidate;
    if (statement)
        if (unsigned candidate = statement->firstToken())
            return candidate;
    return 0;
}


unsigned LambdaExpressionAST::lastToken() const
{
    if (statement)
        if (unsigned candidate = statement->lastToken())
            return candidate;
    if (lambda_declarator)
        if (unsigned candidate = lambda_declarator->lastToken())
            return candidate;
    if (lambda_introducer)
        if (unsigned candidate = lambda_introducer->lastToken())
            return candidate;
    return 1;
}


unsigned LambdaIntroducerAST::firstToken() const
{
    if (lbracket_token)
        return lbracket_token;
    if (lambda_capture)
        if (unsigned candidate = lambda_capture->firstToken())
            return candidate;
    if (rbracket_token)
        return rbracket_token;
    return 0;
}


unsigned LambdaIntroducerAST::lastToken() const
{
    if (rbracket_token)
        return rbracket_token + 1;
    if (lambda_capture)
        if (unsigned candidate = lambda_capture->lastToken())
            return candidate;
    if (lbracket_token)
        return lbracket_token + 1;
    return 1;
}


unsigned LinkageBodyAST::firstToken() const
{
    if (lbrace_token)
        return lbrace_token;
    if (declaration_list)
        if (unsigned candidate = declaration_list->firstToken())
            return candidate;
    if (rbrace_token)
        return rbrace_token;
    return 0;
}


unsigned LinkageBodyAST::lastToken() const
{
    if (rbrace_token)
        return rbrace_token + 1;
    if (declaration_list)
        if (unsigned candidate = declaration_list->lastToken())
            return candidate;
    if (lbrace_token)
        return lbrace_token + 1;
    return 1;
}


unsigned LinkageSpecificationAST::firstToken() const
{
    if (extern_token)
        return extern_token;
    if (extern_type_token)
        return extern_type_token;
    if (declaration)
        if (unsigned candidate = declaration->firstToken())
            return candidate;
    return 0;
}


unsigned LinkageSpecificationAST::lastToken() const
{
    if (declaration)
        if (unsigned candidate = declaration->lastToken())
            return candidate;
    if (extern_type_token)
        return extern_type_token + 1;
    if (extern_token)
        return extern_token + 1;
    return 1;
}


unsigned MemInitializerAST::firstToken() const
{
    if (name)
        if (unsigned candidate = name->firstToken())
            return candidate;
    if (expression)
        if (unsigned candidate = expression->firstToken())
            return candidate;
    return 0;
}


unsigned MemInitializerAST::lastToken() const
{
    if (expression)
        if (unsigned candidate = expression->lastToken())
            return candidate;
    if (name)
        if (unsigned candidate = name->lastToken())
            return candidate;
    return 1;
}


unsigned MemberAccessAST::firstToken() const
{
    if (base_expression)
        if (unsigned candidate = base_expression->firstToken())
            return candidate;
    if (access_token)
        return access_token;
    if (template_token)
        return template_token;
    if (member_name)
        if (unsigned candidate = member_name->firstToken())
            return candidate;
    return 0;
}


unsigned MemberAccessAST::lastToken() const
{
    if (member_name)
        if (unsigned candidate = member_name->lastToken())
            return candidate;
    if (template_token)
        return template_token + 1;
    if (access_token)
        return access_token + 1;
    if (base_expression)
        if (unsigned candidate = base_expression->lastToken())
            return candidate;
    return 1;
}


unsigned NamedTypeSpecifierAST::firstToken() const
{
    if (name)
        if (unsigned candidate = name->firstToken())
            return candidate;
    return 0;
}


unsigned NamedTypeSpecifierAST::lastToken() const
{
    if (name)
        if (unsigned candidate = name->lastToken())
            return candidate;
    return 1;
}


unsigned NamespaceAST::firstToken() const
{
    if (inline_token)
        return inline_token;
    if (namespace_token)
        return namespace_token;
    if (identifier_token)
        return identifier_token;
    if (attribute_list)
        if (unsigned candidate = attribute_list->firstToken())
            return candidate;
    if (linkage_body)
        if (unsigned candidate = linkage_body->firstToken())
            return candidate;
    return 0;
}


unsigned NamespaceAST::lastToken() const
{
    if (linkage_body)
        if (unsigned candidate = linkage_body->lastToken())
            return candidate;
    if (attribute_list)
        if (unsigned candidate = attribute_list->lastToken())
            return candidate;
    if (identifier_token)
        return identifier_token + 1;
    if (namespace_token)
        return namespace_token + 1;
    if (inline_token)
        return inline_token + 1;
    return 1;
}


unsigned NamespaceAliasDefinitionAST::firstToken() const
{
    if (namespace_token)
        return namespace_token;
    if (namespace_name_token)
        return namespace_name_token;
    if (equal_token)
        return equal_token;
    if (name)
        if (unsigned candidate = name->firstToken())
            return candidate;
    if (semicolon_token)
        return semicolon_token;
    return 0;
}


unsigned NamespaceAliasDefinitionAST::lastToken() const
{
    if (semicolon_token)
        return semicolon_token + 1;
    if (name)
        if (unsigned candidate = name->lastToken())
            return candidate;
    if (equal_token)
        return equal_token + 1;
    if (namespace_name_token)
        return namespace_name_token + 1;
    if (namespace_token)
        return namespace_token + 1;
    return 1;
}


unsigned NestedDeclaratorAST::firstToken() const
{
    if (lparen_token)
        return lparen_token;
    if (declarator)
        if (unsigned candidate = declarator->firstToken())
            return candidate;
    if (rparen_token)
        return rparen_token;
    return 0;
}


unsigned NestedDeclaratorAST::lastToken() const
{
    if (rparen_token)
        return rparen_token + 1;
    if (declarator)
        if (unsigned candidate = declarator->lastToken())
            return candidate;
    if (lparen_token)
        return lparen_token + 1;
    return 1;
}


unsigned NestedExpressionAST::firstToken() const
{
    if (lparen_token)
        return lparen_token;
    if (expression)
        if (unsigned candidate = expression->firstToken())
            return candidate;
    if (rparen_token)
        return rparen_token;
    return 0;
}


unsigned NestedExpressionAST::lastToken() const
{
    if (rparen_token)
        return rparen_token + 1;
    if (expression)
        if (unsigned candidate = expression->lastToken())
            return candidate;
    if (lparen_token)
        return lparen_token + 1;
    return 1;
}


unsigned NestedNameSpecifierAST::firstToken() const
{
    if (class_or_namespace_name)
        if (unsigned candidate = class_or_namespace_name->firstToken())
            return candidate;
    if (scope_token)
        return scope_token;
    return 0;
}


unsigned NestedNameSpecifierAST::lastToken() const
{
    if (scope_token)
        return scope_token + 1;
    if (class_or_namespace_name)
        if (unsigned candidate = class_or_namespace_name->lastToken())
            return candidate;
    return 1;
}


unsigned NewArrayDeclaratorAST::firstToken() const
{
    if (lbracket_token)
        return lbracket_token;
    if (expression)
        if (unsigned candidate = expression->firstToken())
            return candidate;
    if (rbracket_token)
        return rbracket_token;
    return 0;
}


unsigned NewArrayDeclaratorAST::lastToken() const
{
    if (rbracket_token)
        return rbracket_token + 1;
    if (expression)
        if (unsigned candidate = expression->lastToken())
            return candidate;
    if (lbracket_token)
        return lbracket_token + 1;
    return 1;
}


unsigned NewExpressionAST::firstToken() const
{
    if (scope_token)
        return scope_token;
    if (new_token)
        return new_token;
    if (new_placement)
        if (unsigned candidate = new_placement->firstToken())
            return candidate;
    if (lparen_token)
        return lparen_token;
    if (type_id)
        if (unsigned candidate = type_id->firstToken())
            return candidate;
    if (rparen_token)
        return rparen_token;
    if (new_type_id)
        if (unsigned candidate = new_type_id->firstToken())
            return candidate;
    if (new_initializer)
        if (unsigned candidate = new_initializer->firstToken())
            return candidate;
    return 0;
}


unsigned NewExpressionAST::lastToken() const
{
    if (new_initializer)
        if (unsigned candidate = new_initializer->lastToken())
            return candidate;
    if (new_type_id)
        if (unsigned candidate = new_type_id->lastToken())
            return candidate;
    if (rparen_token)
        return rparen_token + 1;
    if (type_id)
        if (unsigned candidate = type_id->lastToken())
            return candidate;
    if (lparen_token)
        return lparen_token + 1;
    if (new_placement)
        if (unsigned candidate = new_placement->lastToken())
            return candidate;
    if (new_token)
        return new_token + 1;
    if (scope_token)
        return scope_token + 1;
    return 1;
}


unsigned ExpressionListParenAST::firstToken() const
{
    if (lparen_token)
        return lparen_token;
    if (expression_list)
        if (unsigned candidate = expression_list->firstToken())
            return candidate;
    if (rparen_token)
        return rparen_token;
    return 0;
}


unsigned ExpressionListParenAST::lastToken() const
{
    if (rparen_token)
        return rparen_token + 1;
    if (expression_list)
        if (unsigned candidate = expression_list->lastToken())
            return candidate;
    if (lparen_token)
        return lparen_token + 1;
    return 1;
}


unsigned NewTypeIdAST::firstToken() const
{
    if (type_specifier_list)
        if (unsigned candidate = type_specifier_list->firstToken())
            return candidate;
    if (ptr_operator_list)
        if (unsigned candidate = ptr_operator_list->firstToken())
            return candidate;
    if (new_array_declarator_list)
        if (unsigned candidate = new_array_declarator_list->firstToken())
            return candidate;
    return 0;
}


unsigned NewTypeIdAST::lastToken() const
{
    if (new_array_declarator_list)
        if (unsigned candidate = new_array_declarator_list->lastToken())
            return candidate;
    if (ptr_operator_list)
        if (unsigned candidate = ptr_operator_list->lastToken())
            return candidate;
    if (type_specifier_list)
        if (unsigned candidate = type_specifier_list->lastToken())
            return candidate;
    return 1;
}


unsigned NumericLiteralAST::firstToken() const
{
    if (literal_token)
        return literal_token;
    return 0;
}


unsigned NumericLiteralAST::lastToken() const
{
    if (literal_token)
        return literal_token + 1;
    return 1;
}


unsigned OperatorAST::firstToken() const
{
    if (op_token)
        return op_token;
    if (open_token)
        return open_token;
    if (close_token)
        return close_token;
    return 0;
}


unsigned OperatorAST::lastToken() const
{
    if (close_token)
        return close_token + 1;
    if (open_token)
        return open_token + 1;
    if (op_token)
        return op_token + 1;
    return 1;
}


unsigned OperatorFunctionIdAST::firstToken() const
{
    if (operator_token)
        return operator_token;
    if (op)
        if (unsigned candidate = op->firstToken())
            return candidate;
    return 0;
}


unsigned OperatorFunctionIdAST::lastToken() const
{
    if (op)
        if (unsigned candidate = op->lastToken())
            return candidate;
    if (operator_token)
        return operator_token + 1;
    return 1;
}


unsigned ParameterDeclarationAST::firstToken() const
{
    if (type_specifier_list)
        if (unsigned candidate = type_specifier_list->firstToken())
            return candidate;
    if (declarator)
        if (unsigned candidate = declarator->firstToken())
            return candidate;
    if (equal_token)
        return equal_token;
    if (expression)
        if (unsigned candidate = expression->firstToken())
            return candidate;
    return 0;
}


unsigned ParameterDeclarationAST::lastToken() const
{
    if (expression)
        if (unsigned candidate = expression->lastToken())
            return candidate;
    if (equal_token)
        return equal_token + 1;
    if (declarator)
        if (unsigned candidate = declarator->lastToken())
            return candidate;
    if (type_specifier_list)
        if (unsigned candidate = type_specifier_list->lastToken())
            return candidate;
    return 1;
}


unsigned ParameterDeclarationClauseAST::firstToken() const
{
    if (parameter_declaration_list)
        if (unsigned candidate = parameter_declaration_list->firstToken())
            return candidate;
    if (dot_dot_dot_token)
        return dot_dot_dot_token;
    return 0;
}


unsigned ParameterDeclarationClauseAST::lastToken() const
{
    if (dot_dot_dot_token)
        return dot_dot_dot_token + 1;
    if (parameter_declaration_list)
        if (unsigned candidate = parameter_declaration_list->lastToken())
            return candidate;
    return 1;
}


unsigned PointerAST::firstToken() const
{
    if (star_token)
        return star_token;
    if (cv_qualifier_list)
        if (unsigned candidate = cv_qualifier_list->firstToken())
            return candidate;
    return 0;
}


unsigned PointerAST::lastToken() const
{
    if (cv_qualifier_list)
        if (unsigned candidate = cv_qualifier_list->lastToken())
            return candidate;
    if (star_token)
        return star_token + 1;
    return 1;
}


unsigned PointerToMemberAST::firstToken() const
{
    if (global_scope_token)
        return global_scope_token;
    if (nested_name_specifier_list)
        if (unsigned candidate = nested_name_specifier_list->firstToken())
            return candidate;
    if (star_token)
        return star_token;
    if (cv_qualifier_list)
        if (unsigned candidate = cv_qualifier_list->firstToken())
            return candidate;
    if (ref_qualifier_token)
        return ref_qualifier_token;
    return 0;
}


unsigned PointerToMemberAST::lastToken() const
{
    if (ref_qualifier_token)
        return ref_qualifier_token + 1;
    if (cv_qualifier_list)
        if (unsigned candidate = cv_qualifier_list->lastToken())
            return candidate;
    if (star_token)
        return star_token + 1;
    if (nested_name_specifier_list)
        if (unsigned candidate = nested_name_specifier_list->lastToken())
            return candidate;
    if (global_scope_token)
        return global_scope_token + 1;
    return 1;
}


unsigned PostIncrDecrAST::firstToken() const
{
    if (base_expression)
        if (unsigned candidate = base_expression->firstToken())
            return candidate;
    if (incr_decr_token)
        return incr_decr_token;
    return 0;
}


unsigned PostIncrDecrAST::lastToken() const
{
    if (incr_decr_token)
        return incr_decr_token + 1;
    if (base_expression)
        if (unsigned candidate = base_expression->lastToken())
            return candidate;
    return 1;
}


unsigned QualifiedNameAST::firstToken() const
{
    if (global_scope_token)
        return global_scope_token;
    if (nested_name_specifier_list)
        if (unsigned candidate = nested_name_specifier_list->firstToken())
            return candidate;
    if (unqualified_name)
        if (unsigned candidate = unqualified_name->firstToken())
            return candidate;
    return 0;
}


unsigned QualifiedNameAST::lastToken() const
{
    if (unqualified_name)
        if (unsigned candidate = unqualified_name->lastToken())
            return candidate;
    if (nested_name_specifier_list)
        if (unsigned candidate = nested_name_specifier_list->lastToken())
            return candidate;
    if (global_scope_token)
        return global_scope_token + 1;
    return 1;
}

unsigned QuantifiedTypeSpecifierAST::firstToken() const
{
    if (quantifier_token)
        return quantifier_token;
    if (lparen_token)
        return lparen_token;
    if (name)
        if (unsigned candidate = name->firstToken())
            return candidate;
    if (rparen_token)
        return rparen_token;
    return 0;
}


unsigned QuantifiedTypeSpecifierAST::lastToken() const
{
    if (rparen_token)
        return rparen_token;
    if (name)
        if (unsigned candidate = name->firstToken())
            return candidate;
    if (lparen_token)
        return lparen_token;
    if (quantifier_token)
        return quantifier_token + 1;
    return 1;
}

unsigned TaggedNameAST::firstToken() const
{
    return tag_token;
}

unsigned TaggedNameAST::lastToken() const
{
    if (core_name)
        return core_name->lastToken();
    return 1;
}

unsigned ReferenceAST::firstToken() const
{
    if (reference_token)
        return reference_token;
    return 0;
}


unsigned ReferenceAST::lastToken() const
{
    if (reference_token)
        return reference_token + 1;
    return 1;
}


unsigned ReturnStatementAST::firstToken() const
{
    if (return_token)
        return return_token;
    if (expression)
        if (unsigned candidate = expression->firstToken())
            return candidate;
    if (semicolon_token)
        return semicolon_token;
    return 0;
}


unsigned ReturnStatementAST::lastToken() const
{
    if (semicolon_token)
        return semicolon_token + 1;
    if (expression)
        if (unsigned candidate = expression->lastToken())
            return candidate;
    if (return_token)
        return return_token + 1;
    return 1;
}


unsigned SimpleDeclarationAST::firstToken() const
{
    if (decl_specifier_list)
        if (unsigned candidate = decl_specifier_list->firstToken())
            return candidate;
    if (declarator_list)
        if (unsigned candidate = declarator_list->firstToken())
            return candidate;
    if (semicolon_token)
        return semicolon_token;
    return 0;
}


unsigned SimpleDeclarationAST::lastToken() const
{
    if (semicolon_token)
        return semicolon_token + 1;
    if (declarator_list)
        if (unsigned candidate = declarator_list->lastToken())
            return candidate;
    if (decl_specifier_list)
        if (unsigned candidate = decl_specifier_list->lastToken())
            return candidate;
    return 1;
}


unsigned SimpleNameAST::firstToken() const
{
    if (identifier_token)
        return identifier_token;
    return 0;
}


unsigned SimpleNameAST::lastToken() const
{
    if (identifier_token)
        return identifier_token + 1;
    return 1;
}


unsigned SimpleSpecifierAST::firstToken() const
{
    if (specifier_token)
        return specifier_token;
    return 0;
}


unsigned SimpleSpecifierAST::lastToken() const
{
    if (specifier_token)
        return specifier_token + 1;
    return 1;
}


unsigned SizeofExpressionAST::firstToken() const
{
    if (sizeof_token)
        return sizeof_token;
    if (dot_dot_dot_token)
        return dot_dot_dot_token;
    if (lparen_token)
        return lparen_token;
    if (expression)
        if (unsigned candidate = expression->firstToken())
            return candidate;
    if (rparen_token)
        return rparen_token;
    return 0;
}


unsigned SizeofExpressionAST::lastToken() const
{
    if (rparen_token)
        return rparen_token + 1;
    if (expression)
        if (unsigned candidate = expression->lastToken())
            return candidate;
    if (lparen_token)
        return lparen_token + 1;
    if (dot_dot_dot_token)
        return dot_dot_dot_token + 1;
    if (sizeof_token)
        return sizeof_token + 1;
    return 1;
}


unsigned StringLiteralAST::firstToken() const
{
    if (literal_token)
        return literal_token;
    if (next)
        if (unsigned candidate = next->firstToken())
            return candidate;
    return 0;
}


unsigned StringLiteralAST::lastToken() const
{
    if (next)
        if (unsigned candidate = next->lastToken())
            return candidate;
    if (literal_token)
        return literal_token + 1;
    return 1;
}


unsigned SwitchStatementAST::firstToken() const
{
    if (switch_token)
        return switch_token;
    if (lparen_token)
        return lparen_token;
    if (condition)
        if (unsigned candidate = condition->firstToken())
            return candidate;
    if (rparen_token)
        return rparen_token;
    if (statement)
        if (unsigned candidate = statement->firstToken())
            return candidate;
    return 0;
}


unsigned SwitchStatementAST::lastToken() const
{
    if (statement)
        if (unsigned candidate = statement->lastToken())
            return candidate;
    if (rparen_token)
        return rparen_token + 1;
    if (condition)
        if (unsigned candidate = condition->lastToken())
            return candidate;
    if (lparen_token)
        return lparen_token + 1;
    if (switch_token)
        return switch_token + 1;
    return 1;
}


unsigned TemplateDeclarationAST::firstToken() const
{
    if (export_token)
        return export_token;
    if (template_token)
        return template_token;
    if (less_token)
        return less_token;
    if (template_parameter_list)
        if (unsigned candidate = template_parameter_list->firstToken())
            return candidate;
    if (greater_token)
        return greater_token;
    if (declaration)
        if (unsigned candidate = declaration->firstToken())
            return candidate;
    return 0;
}


unsigned TemplateDeclarationAST::lastToken() const
{
    if (declaration)
        if (unsigned candidate = declaration->lastToken())
            return candidate;
    if (greater_token)
        return greater_token + 1;
    if (template_parameter_list)
        if (unsigned candidate = template_parameter_list->lastToken())
            return candidate;
    if (less_token)
        return less_token + 1;
    if (template_token)
        return template_token + 1;
    if (export_token)
        return export_token + 1;
    return 1;
}


unsigned TemplateIdAST::firstToken() const
{
    if (template_token)
        return template_token;
    if (identifier_token)
        return identifier_token;
    if (less_token)
        return less_token;
    if (template_argument_list)
        if (unsigned candidate = template_argument_list->firstToken())
            return candidate;
    if (greater_token)
        return greater_token;
    return 0;
}


unsigned TemplateIdAST::lastToken() const
{
    if (greater_token)
        return greater_token + 1;
    if (template_argument_list)
        if (unsigned candidate = template_argument_list->lastToken())
            return candidate;
    if (less_token)
        return less_token + 1;
    if (identifier_token)
        return identifier_token + 1;
    if (template_token)
        return template_token + 1;
    return 1;
}


unsigned TemplateTypeParameterAST::firstToken() const
{
    if (template_token)
        return template_token;
    if (less_token)
        return less_token;
    if (template_parameter_list)
        if (unsigned candidate = template_parameter_list->firstToken())
            return candidate;
    if (greater_token)
        return greater_token;
    if (class_token)
        return class_token;
    if (dot_dot_dot_token)
        return dot_dot_dot_token;
    if (name)
        if (unsigned candidate = name->firstToken())
            return candidate;
    if (equal_token)
        return equal_token;
    if (type_id)
        if (unsigned candidate = type_id->firstToken())
            return candidate;
    return 0;
}


unsigned TemplateTypeParameterAST::lastToken() const
{
    if (type_id)
        if (unsigned candidate = type_id->lastToken())
            return candidate;
    if (equal_token)
        return equal_token + 1;
    if (name)
        if (unsigned candidate = name->lastToken())
            return candidate;
    if (dot_dot_dot_token)
        return dot_dot_dot_token + 1;
    if (class_token)
        return class_token + 1;
    if (greater_token)
        return greater_token + 1;
    if (template_parameter_list)
        if (unsigned candidate = template_parameter_list->lastToken())
            return candidate;
    if (less_token)
        return less_token + 1;
    if (template_token)
        return template_token + 1;
    return 1;
}


unsigned ThisExpressionAST::firstToken() const
{
    if (this_token)
        return this_token;
    return 0;
}


unsigned ThisExpressionAST::lastToken() const
{
    if (this_token)
        return this_token + 1;
    return 1;
}


unsigned ThrowExpressionAST::firstToken() const
{
    if (throw_token)
        return throw_token;
    if (expression)
        if (unsigned candidate = expression->firstToken())
            return candidate;
    return 0;
}


unsigned ThrowExpressionAST::lastToken() const
{
    if (expression)
        if (unsigned candidate = expression->lastToken())
            return candidate;
    if (throw_token)
        return throw_token + 1;
    return 1;
}


unsigned TrailingReturnTypeAST::firstToken() const
{
    if (arrow_token)
        return arrow_token;
    if (attributes)
        if (unsigned candidate = attributes->firstToken())
            return candidate;
    if (type_specifier_list)
        if (unsigned candidate = type_specifier_list->firstToken())
            return candidate;
    if (declarator)
        if (unsigned candidate = declarator->firstToken())
            return candidate;
    return 0;
}


unsigned TrailingReturnTypeAST::lastToken() const
{
    if (declarator)
        if (unsigned candidate = declarator->lastToken())
            return candidate;
    if (type_specifier_list)
        if (unsigned candidate = type_specifier_list->lastToken())
            return candidate;
    if (attributes)
        if (unsigned candidate = attributes->lastToken())
            return candidate;
    if (arrow_token)
        return arrow_token + 1;
    return 1;
}


unsigned TranslationUnitAST::firstToken() const
{
    if (declaration_list)
        if (unsigned candidate = declaration_list->firstToken())
            return candidate;
    return 0;
}


unsigned TranslationUnitAST::lastToken() const
{
    if (declaration_list)
        if (unsigned candidate = declaration_list->lastToken())
            return candidate;
    return 1;
}


unsigned TryBlockStatementAST::firstToken() const
{
    if (try_token)
        return try_token;
    if (statement)
        if (unsigned candidate = statement->firstToken())
            return candidate;
    if (catch_clause_list)
        if (unsigned candidate = catch_clause_list->firstToken())
            return candidate;
    return 0;
}


unsigned TryBlockStatementAST::lastToken() const
{
    if (catch_clause_list)
        if (unsigned candidate = catch_clause_list->lastToken())
            return candidate;
    if (statement)
        if (unsigned candidate = statement->lastToken())
            return candidate;
    if (try_token)
        return try_token + 1;
    return 1;
}


unsigned TypeConstructorCallAST::firstToken() const
{
    if (type_specifier_list)
        if (unsigned candidate = type_specifier_list->firstToken())
            return candidate;
    if (expression)
        if (unsigned candidate = expression->firstToken())
            return candidate;
    return 0;
}


unsigned TypeConstructorCallAST::lastToken() const
{
    if (expression)
        if (unsigned candidate = expression->lastToken())
            return candidate;
    if (type_specifier_list)
        if (unsigned candidate = type_specifier_list->lastToken())
            return candidate;
    return 1;
}


unsigned TypeIdAST::firstToken() const
{
    if (type_specifier_list)
        if (unsigned candidate = type_specifier_list->firstToken())
            return candidate;
    if (declarator)
        if (unsigned candidate = declarator->firstToken())
            return candidate;
    return 0;
}


unsigned TypeIdAST::lastToken() const
{
    if (declarator)
        if (unsigned candidate = declarator->lastToken())
            return candidate;
    if (type_specifier_list)
        if (unsigned candidate = type_specifier_list->lastToken())
            return candidate;
    return 1;
}


unsigned TypeidExpressionAST::firstToken() const
{
    if (typeid_token)
        return typeid_token;
    if (lparen_token)
        return lparen_token;
    if (expression)
        if (unsigned candidate = expression->firstToken())
            return candidate;
    if (rparen_token)
        return rparen_token;
    return 0;
}


unsigned TypeidExpressionAST::lastToken() const
{
    if (rparen_token)
        return rparen_token + 1;
    if (expression)
        if (unsigned candidate = expression->lastToken())
            return candidate;
    if (lparen_token)
        return lparen_token + 1;
    if (typeid_token)
        return typeid_token + 1;
    return 1;
}


unsigned TypenameCallExpressionAST::firstToken() const
{
    if (typename_token)
        return typename_token;
    if (name)
        if (unsigned candidate = name->firstToken())
            return candidate;
    if (expression)
        if (unsigned candidate = expression->firstToken())
            return candidate;
    return 0;
}


unsigned TypenameCallExpressionAST::lastToken() const
{
    if (expression)
        if (unsigned candidate = expression->lastToken())
            return candidate;
    if (name)
        if (unsigned candidate = name->lastToken())
            return candidate;
    if (typename_token)
        return typename_token + 1;
    return 1;
}


unsigned TypenameTypeParameterAST::firstToken() const
{
    if (classkey_token)
        return classkey_token;
    if (dot_dot_dot_token)
        return dot_dot_dot_token;
    if (name)
        if (unsigned candidate = name->firstToken())
            return candidate;
    if (equal_token)
        return equal_token;
    if (type_id)
        if (unsigned candidate = type_id->firstToken())
            return candidate;
    return 0;
}


unsigned TypenameTypeParameterAST::lastToken() const
{
    if (type_id)
        if (unsigned candidate = type_id->lastToken())
            return candidate;
    if (equal_token)
        return equal_token + 1;
    if (name)
        if (unsigned candidate = name->lastToken())
            return candidate;
    if (dot_dot_dot_token)
        return dot_dot_dot_token + 1;
    if (classkey_token)
        return classkey_token + 1;
    return 1;
}


unsigned TypeofSpecifierAST::firstToken() const
{
    if (typeof_token)
        return typeof_token;
    if (lparen_token)
        return lparen_token;
    if (expression)
        if (unsigned candidate = expression->firstToken())
            return candidate;
    if (rparen_token)
        return rparen_token;
    return 0;
}


unsigned TypeofSpecifierAST::lastToken() const
{
    if (rparen_token)
        return rparen_token + 1;
    if (expression)
        if (unsigned candidate = expression->lastToken())
            return candidate;
    if (lparen_token)
        return lparen_token + 1;
    if (typeof_token)
        return typeof_token + 1;
    return 1;
}


unsigned UnaryExpressionAST::firstToken() const
{
    if (unary_op_token)
        return unary_op_token;
    if (expression)
        if (unsigned candidate = expression->firstToken())
            return candidate;
    return 0;
}


unsigned UnaryExpressionAST::lastToken() const
{
    if (expression)
        if (unsigned candidate = expression->lastToken())
            return candidate;
    if (unary_op_token)
        return unary_op_token + 1;
    return 1;
}


unsigned UsingAST::firstToken() const
{
    if (using_token)
        return using_token;
    if (typename_token)
        return typename_token;
    if (name)
        if (unsigned candidate = name->firstToken())
            return candidate;
    if (semicolon_token)
        return semicolon_token;
    return 0;
}


unsigned UsingAST::lastToken() const
{
    if (semicolon_token)
        return semicolon_token + 1;
    if (name)
        if (unsigned candidate = name->lastToken())
            return candidate;
    if (typename_token)
        return typename_token + 1;
    if (using_token)
        return using_token + 1;
    return 1;
}


unsigned UsingDirectiveAST::firstToken() const
{
    if (using_token)
        return using_token;
    if (namespace_token)
        return namespace_token;
    if (name)
        if (unsigned candidate = name->firstToken())
            return candidate;
    if (semicolon_token)
        return semicolon_token;
    return 0;
}


unsigned UsingDirectiveAST::lastToken() const
{
    if (semicolon_token)
        return semicolon_token + 1;
    if (name)
        if (unsigned candidate = name->lastToken())
            return candidate;
    if (namespace_token)
        return namespace_token + 1;
    if (using_token)
        return using_token + 1;
    return 1;
}


unsigned WhileStatementAST::firstToken() const
{
    if (while_token)
        return while_token;
    if (lparen_token)
        return lparen_token;
    if (condition)
        if (unsigned candidate = condition->firstToken())
            return candidate;
    if (rparen_token)
        return rparen_token;
    if (statement)
        if (unsigned candidate = statement->firstToken())
            return candidate;
    return 0;
}


unsigned WhileStatementAST::lastToken() const
{
    if (statement)
        if (unsigned candidate = statement->lastToken())
            return candidate;
    if (rparen_token)
        return rparen_token + 1;
    if (condition)
        if (unsigned candidate = condition->lastToken())
            return candidate;
    if (lparen_token)
        return lparen_token + 1;
    if (while_token)
        return while_token + 1;
    return 1;
}


unsigned GnuAttributeSpecifierAST::lastToken() const
{
    if (second_rparen_token)
        return second_rparen_token + 1;
    if (first_rparen_token)
        return first_rparen_token + 1;
    if (attribute_list)
        if (unsigned candidate = attribute_list->lastToken())
            return candidate;
    if (second_lparen_token)
        return second_lparen_token + 1;
    if (first_lparen_token)
        return first_lparen_token + 1;
    if (attribute_token)
        return attribute_token + 1;
    return 1;
}


unsigned PointerLiteralAST::firstToken() const
{
    if (literal_token)
        return literal_token;
    return 0;
}


unsigned PointerLiteralAST::lastToken() const
{
    if (literal_token)
        return literal_token + 1;
    return 1;
}


unsigned NoExceptSpecificationAST::firstToken() const
{
    if (noexcept_token)
        return noexcept_token;
    if (lparen_token)
        return lparen_token;
    if (expression)
        if (unsigned candidate = expression->firstToken())
            return candidate;
    if (rparen_token)
        return rparen_token;
    return 0;
}


unsigned NoExceptSpecificationAST::lastToken() const
{
    if (rparen_token)
        return rparen_token + 1;
    if (expression)
        if (unsigned candidate = expression->lastToken())
            return candidate;
    if (lparen_token)
        return lparen_token + 1;
    if (noexcept_token)
        return noexcept_token + 1;
    return 1;
}


unsigned StaticAssertDeclarationAST::firstToken() const
{
    if (static_assert_token)
        return static_assert_token;
    if (lparen_token)
        return lparen_token;
    if (expression)
        if (unsigned candidate = expression->firstToken())
            return candidate;
    if (comma_token)
        return comma_token;
    if (string_literal)
        if (unsigned candidate = string_literal->firstToken())
            return candidate;
    if (rparen_token)
        return rparen_token;
    if (semicolon_token)
        return semicolon_token;
    return 0;
}


unsigned StaticAssertDeclarationAST::lastToken() const
{
    if (semicolon_token)
        return semicolon_token + 1;
    if (rparen_token)
        return rparen_token + 1;
    if (string_literal)
        if (unsigned candidate = string_literal->lastToken())
            return candidate;
    if (comma_token)
        return comma_token + 1;
    if (expression)
        if (unsigned candidate = expression->lastToken())
            return candidate;
    if (lparen_token)
        return lparen_token + 1;
    if (static_assert_token)
        return static_assert_token + 1;
    return 1;
}


unsigned DecltypeSpecifierAST::firstToken() const
{
    if (decltype_token)
        return decltype_token;
    if (lparen_token)
        return lparen_token;
    if (expression)
        if (unsigned candidate = expression->firstToken())
            return candidate;
    if (rparen_token)
        return rparen_token;
    return 0;
}


unsigned DecltypeSpecifierAST::lastToken() const
{
    if (rparen_token)
        return rparen_token + 1;
    if (expression)
        if (unsigned candidate = expression->lastToken())
            return candidate;
    if (lparen_token)
        return lparen_token + 1;
    if (decltype_token)
        return decltype_token + 1;
    return 1;
}


unsigned RangeBasedForStatementAST::firstToken() const
{
    if (for_token)
        return for_token;
    if (lparen_token)
        return lparen_token;
    if (type_specifier_list)
        if (unsigned candidate = type_specifier_list->firstToken())
            return candidate;
    if (declarator)
        if (unsigned candidate = declarator->firstToken())
            return candidate;
    if (colon_token)
        return colon_token;
    if (expression)
        if (unsigned candidate = expression->firstToken())
            return candidate;
    if (rparen_token)
        return rparen_token;
    if (statement)
        if (unsigned candidate = statement->firstToken())
            return candidate;
    return 0;
}


unsigned RangeBasedForStatementAST::lastToken() const
{
    if (statement)
        if (unsigned candidate = statement->lastToken())
            return candidate;
    if (rparen_token)
        return rparen_token + 1;
    if (expression)
        if (unsigned candidate = expression->lastToken())
            return candidate;
    if (colon_token)
        return colon_token + 1;
    if (declarator)
        if (unsigned candidate = declarator->lastToken())
            return candidate;
    if (type_specifier_list)
        if (unsigned candidate = type_specifier_list->lastToken())
            return candidate;
    if (lparen_token)
        return lparen_token + 1;
    if (for_token)
        return for_token + 1;
    return 1;
}


unsigned AlignofExpressionAST::firstToken() const
{
    if (alignof_token)
        return alignof_token;
    if (lparen_token)
        return lparen_token;
    if (typeId)
        if (unsigned candidate = typeId->firstToken())
            return candidate;
    if (rparen_token)
        return rparen_token;
    return 0;
}


unsigned AlignofExpressionAST::lastToken() const
{
    if (rparen_token)
        return rparen_token + 1;
    if (typeId)
        if (unsigned candidate = typeId->lastToken())
            return candidate;
    if (lparen_token)
        return lparen_token + 1;
    if (alignof_token)
        return alignof_token + 1;
    return 1;
}


unsigned AliasDeclarationAST::firstToken() const
{
    if (using_token)
        return using_token;
    if (name)
        if (unsigned candidate = name->firstToken())
            return candidate;
    if (equal_token)
        return equal_token;
    if (typeId)
        if (unsigned candidate = typeId->firstToken())
            return candidate;
    if (semicolon_token)
        return semicolon_token;
    return 0;
}


unsigned AliasDeclarationAST::lastToken() const
{
    if (semicolon_token)
        return semicolon_token + 1;
    if (typeId)
        if (unsigned candidate = typeId->lastToken())
            return candidate;
    if (equal_token)
        return equal_token + 1;
    if (name)
        if (unsigned candidate = name->lastToken())
            return candidate;
    if (using_token)
        return using_token + 1;
    return 1;
}


unsigned DesignatedInitializerAST::firstToken() const
{
    if (designator_list)
        if (unsigned candidate = designator_list->firstToken())
            return candidate;
    if (equal_token)
        return equal_token;
    if (initializer)
        if (unsigned candidate = initializer->firstToken())
            return candidate;
    return 0;
}


unsigned DesignatedInitializerAST::lastToken() const
{
    if (initializer)
        if (unsigned candidate = initializer->lastToken())
            return candidate;
    if (equal_token)
        return equal_token + 1;
    if (designator_list)
        if (unsigned candidate = designator_list->lastToken())
            return candidate;
    return 1;
}


unsigned BracketDesignatorAST::firstToken() const
{
    if (lbracket_token)
        return lbracket_token;
    if (expression)
        if (unsigned candidate = expression->firstToken())
            return candidate;
    if (rbracket_token)
        return rbracket_token;
    return 0;
}


unsigned BracketDesignatorAST::lastToken() const
{
    if (rbracket_token)
        return rbracket_token + 1;
    if (expression)
        if (unsigned candidate = expression->lastToken())
            return candidate;
    if (lbracket_token)
        return lbracket_token + 1;
    return 1;
}


unsigned DotDesignatorAST::firstToken() const
{
    if (dot_token)
        return dot_token;
    if (identifier_token)
        return identifier_token;
    return 0;
}


unsigned DotDesignatorAST::lastToken() const
{
    if (identifier_token)
        return identifier_token + 1;
    if (dot_token)
        return dot_token + 1;
    return 1;
}


unsigned AlignmentSpecifierAST::firstToken() const
{
    if (align_token)
        return align_token;
    if (lparen_token)
        return lparen_token;
    if (typeIdExprOrAlignmentExpr)
        if (unsigned candidate = typeIdExprOrAlignmentExpr->firstToken())
            return candidate;
    if (ellipses_token)
        return ellipses_token;
    if (rparen_token)
        return rparen_token;
    return 0;
}


unsigned AlignmentSpecifierAST::lastToken() const
{
    if (rparen_token)
        return rparen_token + 1;
    if (ellipses_token)
        return ellipses_token + 1;
    if (typeIdExprOrAlignmentExpr)
        if (unsigned candidate = typeIdExprOrAlignmentExpr->lastToken())
            return candidate;
    if (lparen_token)
        return lparen_token + 1;
    if (align_token)
        return align_token + 1;
    return 1;
}


unsigned NoExceptOperatorExpressionAST::firstToken() const
{
    if (noexcept_token)
        return noexcept_token;
    if (expression)
        if (unsigned candidate = expression->firstToken())
            return candidate;
    return 0;
}


unsigned NoExceptOperatorExpressionAST::lastToken() const
{
    if (expression)
        if (unsigned candidate = expression->lastToken())
            return candidate;
    if (noexcept_token)
        return noexcept_token + 1;
    return 1;
}

unsigned AmbiguousStatementAST::firstToken() const
{
    if (declarationStmt)
        return declarationStmt->firstToken();
    if (expressionStmt)
        return expressionStmt->firstToken();
    return 0;
}


unsigned AmbiguousStatementAST::lastToken() const
{
    if (expressionStmt)
        return expressionStmt->lastToken();
    if (declarationStmt)
        return declarationStmt->lastToken();
    return 1;
}
