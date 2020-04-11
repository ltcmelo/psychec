// Copyright (c) 2008 Roberto Raggi <roberto.raggi@gmail.com>
// Copyright (c) 2016-20 Leandro T. C. Melo <ltcmelo@gmail.com>
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
#include "ASTMatcher.h"

using namespace psyche;

ASTMatcher::ASTMatcher()
{}

ASTMatcher::~ASTMatcher()
{}

bool ASTMatcher::match(SimpleSpecifierAST* node, SimpleSpecifierAST* pattern)
{
    (void) node;
    (void) pattern;

    pattern->specifier_token = node->specifier_token;

    return true;
}

bool ASTMatcher::match(AlignmentSpecifierAST* node, AlignmentSpecifierAST* pattern)
{
    (void) node;
    (void) pattern;

    pattern->align_token = node->align_token;

    pattern->lparen_token = node->lparen_token;

    if (! pattern->typeIdExprOrAlignmentExpr)
        pattern->typeIdExprOrAlignmentExpr = node->typeIdExprOrAlignmentExpr;
    else if (! AST::match(node->typeIdExprOrAlignmentExpr, pattern->typeIdExprOrAlignmentExpr, this))
        return false;

    pattern->ellipses_token = node->ellipses_token;

    pattern->rparen_token = node->rparen_token;

    return true;
}

bool ASTMatcher::match(GenericsDeclarationAST* node, GenericsDeclarationAST* pattern)
{
    (void) node;
    (void) pattern;

    pattern->generics_token = node->generics_token;

    if (! pattern->declaration)
        pattern->declaration = node->declaration;
    else if (! AST::match(node->declaration, pattern->declaration, this))
        return false;

    return true;
}

bool ASTMatcher::match(GnuAttributeSpecifierAST* node, GnuAttributeSpecifierAST* pattern)
{
    (void) node;
    (void) pattern;

    pattern->attribute_token = node->attribute_token;

    pattern->first_lparen_token = node->first_lparen_token;

    pattern->second_lparen_token = node->second_lparen_token;

    if (! pattern->attribute_list)
        pattern->attribute_list = node->attribute_list;
    else if (! AST::match(node->attribute_list, pattern->attribute_list, this))
        return false;

    pattern->first_rparen_token = node->first_rparen_token;

    pattern->second_rparen_token = node->second_rparen_token;

    return true;
}

bool ASTMatcher::match(GnuAttributeAST* node, GnuAttributeAST* pattern)
{
    (void) node;
    (void) pattern;

    pattern->identifier_token = node->identifier_token;

    pattern->lparen_token = node->lparen_token;

    pattern->tag_token = node->tag_token;

    if (! pattern->expression_list)
        pattern->expression_list = node->expression_list;
    else if (! AST::match(node->expression_list, pattern->expression_list, this))
        return false;

    pattern->rparen_token = node->rparen_token;

    return true;
}

bool ASTMatcher::match(TypeofSpecifierAST* node, TypeofSpecifierAST* pattern)
{
    (void) node;
    (void) pattern;

    pattern->typeof_token = node->typeof_token;

    pattern->lparen_token = node->lparen_token;

    if (! pattern->expression)
        pattern->expression = node->expression;
    else if (! AST::match(node->expression, pattern->expression, this))
        return false;

    pattern->rparen_token = node->rparen_token;

    return true;
}

bool ASTMatcher::match(DecltypeSpecifierAST* node, DecltypeSpecifierAST* pattern)
{
    (void) node;
    (void) pattern;

    pattern->decltype_token = node->decltype_token;

    pattern->lparen_token = node->lparen_token;

    if (! pattern->expression)
        pattern->expression = node->expression;
    else if (! AST::match(node->expression, pattern->expression, this))
        return false;

    pattern->rparen_token = node->rparen_token;

    return true;
}

bool ASTMatcher::match(DeclaratorAST* node, DeclaratorAST* pattern)
{
    (void) node;
    (void) pattern;

    if (! pattern->attribute_list)
        pattern->attribute_list = node->attribute_list;
    else if (! AST::match(node->attribute_list, pattern->attribute_list, this))
        return false;

    if (! pattern->ptr_operator_list)
        pattern->ptr_operator_list = node->ptr_operator_list;
    else if (! AST::match(node->ptr_operator_list, pattern->ptr_operator_list, this))
        return false;

    if (! pattern->core_declarator)
        pattern->core_declarator = node->core_declarator;
    else if (! AST::match(node->core_declarator, pattern->core_declarator, this))
        return false;

    if (! pattern->postfix_declarator_list)
        pattern->postfix_declarator_list = node->postfix_declarator_list;
    else if (! AST::match(node->postfix_declarator_list, pattern->postfix_declarator_list, this))
        return false;

    if (! pattern->post_attribute_list)
        pattern->post_attribute_list = node->post_attribute_list;
    else if (! AST::match(node->post_attribute_list, pattern->post_attribute_list, this))
        return false;

    pattern->equal_token = node->equal_token;

    if (! pattern->initializer)
        pattern->initializer = node->initializer;
    else if (! AST::match(node->initializer, pattern->initializer, this))
        return false;

    return true;
}

bool ASTMatcher::match(SimpleDeclarationAST* node, SimpleDeclarationAST* pattern)
{
    (void) node;
    (void) pattern;

    if (! pattern->decl_specifier_list)
        pattern->decl_specifier_list = node->decl_specifier_list;
    else if (! AST::match(node->decl_specifier_list, pattern->decl_specifier_list, this))
        return false;

    if (! pattern->declarator_list)
        pattern->declarator_list = node->declarator_list;
    else if (! AST::match(node->declarator_list, pattern->declarator_list, this))
        return false;

    pattern->semicolon_token = node->semicolon_token;

    return true;
}

bool ASTMatcher::match(EmptyDeclarationAST* node, EmptyDeclarationAST* pattern)
{
    (void) node;
    (void) pattern;

    pattern->semicolon_token = node->semicolon_token;

    return true;
}

bool ASTMatcher::match(AccessDeclarationAST* node, AccessDeclarationAST* pattern)
{
    (void) node;
    (void) pattern;

    pattern->access_specifier_token = node->access_specifier_token;

    pattern->slots_token = node->slots_token;

    pattern->colon_token = node->colon_token;

    return true;
}

bool ASTMatcher::match(AsmDefinitionAST* node, AsmDefinitionAST* pattern)
{
    (void) node;
    (void) pattern;

    pattern->asm_token = node->asm_token;

    pattern->qualifier_token = node->qualifier_token;

    pattern->lparen_token = node->lparen_token;

    pattern->rparen_token = node->rparen_token;

    if (! pattern->string_literal)
        pattern->string_literal = node->string_literal;
    else if (! AST::match(node->string_literal, pattern->string_literal, this))
        return false;

    pattern->semicolon_token = node->semicolon_token;

    return true;
}

bool ASTMatcher::match(BaseSpecifierAST* node, BaseSpecifierAST* pattern)
{
    (void) node;
    (void) pattern;

    pattern->virtual_token = node->virtual_token;

    pattern->access_specifier_token = node->access_specifier_token;

    if (! pattern->name)
        pattern->name = node->name;
    else if (! AST::match(node->name, pattern->name, this))
        return false;

    pattern->ellipsis_token = node->ellipsis_token;

    return true;
}

bool ASTMatcher::match(IdExpressionAST* node, IdExpressionAST* pattern)
{
    (void) node;
    (void) pattern;

    if (! pattern->name)
        pattern->name = node->name;
    else if (! AST::match(node->name, pattern->name, this))
        return false;

    return true;
}

bool ASTMatcher::match(CompoundExpressionAST* node, CompoundExpressionAST* pattern)
{
    (void) node;
    (void) pattern;

    pattern->lparen_token = node->lparen_token;

    if (! pattern->statement)
        pattern->statement = node->statement;
    else if (! AST::match(node->statement, pattern->statement, this))
        return false;

    pattern->rparen_token = node->rparen_token;

    return true;
}

bool ASTMatcher::match(CompoundLiteralAST* node, CompoundLiteralAST* pattern)
{
    (void) node;
    (void) pattern;

    pattern->lparen_token = node->lparen_token;

    if (! pattern->type_id)
        pattern->type_id = node->type_id;
    else if (! AST::match(node->type_id, pattern->type_id, this))
        return false;

    pattern->rparen_token = node->rparen_token;

    if (! pattern->initializer)
        pattern->initializer = node->initializer;
    else if (! AST::match(node->initializer, pattern->initializer, this))
        return false;

    return true;
}

bool ASTMatcher::match(BinaryExpressionAST* node, BinaryExpressionAST* pattern)
{
    (void) node;
    (void) pattern;

    if (! pattern->left_expression)
        pattern->left_expression = node->left_expression;
    else if (! AST::match(node->left_expression, pattern->left_expression, this))
        return false;

    pattern->binary_op_token = node->binary_op_token;

    if (! pattern->right_expression)
        pattern->right_expression = node->right_expression;
    else if (! AST::match(node->right_expression, pattern->right_expression, this))
        return false;

    return true;
}

bool ASTMatcher::match(CastExpressionAST* node, CastExpressionAST* pattern)
{
    (void) node;
    (void) pattern;

    pattern->lparen_token = node->lparen_token;

    if (! pattern->type_id)
        pattern->type_id = node->type_id;
    else if (! AST::match(node->type_id, pattern->type_id, this))
        return false;

    pattern->rparen_token = node->rparen_token;

    if (! pattern->expression)
        pattern->expression = node->expression;
    else if (! AST::match(node->expression, pattern->expression, this))
        return false;

    return true;
}

bool ASTMatcher::match(ClassSpecifierAST* node, ClassSpecifierAST* pattern)
{
    (void) node;
    (void) pattern;

    pattern->classkey_token = node->classkey_token;

    if (! pattern->attribute_list)
        pattern->attribute_list = node->attribute_list;
    else if (! AST::match(node->attribute_list, pattern->attribute_list, this))
        return false;

    if (! pattern->name)
        pattern->name = node->name;
    else if (! AST::match(node->name, pattern->name, this))
        return false;

    pattern->final_token = node->final_token;

    pattern->colon_token = node->colon_token;

    if (! pattern->base_clause_list)
        pattern->base_clause_list = node->base_clause_list;
    else if (! AST::match(node->base_clause_list, pattern->base_clause_list, this))
        return false;

    pattern->dot_dot_dot_token = node->dot_dot_dot_token;

    pattern->lbrace_token = node->lbrace_token;

    if (! pattern->member_specifier_list)
        pattern->member_specifier_list = node->member_specifier_list;
    else if (! AST::match(node->member_specifier_list, pattern->member_specifier_list, this))
        return false;

    pattern->rbrace_token = node->rbrace_token;

    return true;
}

bool ASTMatcher::match(CaseStatementAST* node, CaseStatementAST* pattern)
{
    (void) node;
    (void) pattern;

    pattern->case_token = node->case_token;

    if (! pattern->expression)
        pattern->expression = node->expression;
    else if (! AST::match(node->expression, pattern->expression, this))
        return false;

    pattern->colon_token = node->colon_token;

    if (! pattern->statement)
        pattern->statement = node->statement;
    else if (! AST::match(node->statement, pattern->statement, this))
        return false;

    return true;
}

bool ASTMatcher::match(CompoundStatementAST* node, CompoundStatementAST* pattern)
{
    (void) node;
    (void) pattern;

    pattern->lbrace_token = node->lbrace_token;

    if (! pattern->statement_list)
        pattern->statement_list = node->statement_list;
    else if (! AST::match(node->statement_list, pattern->statement_list, this))
        return false;

    pattern->rbrace_token = node->rbrace_token;

    return true;
}

bool ASTMatcher::match(ConditionAST* node, ConditionAST* pattern)
{
    (void) node;
    (void) pattern;

    if (! pattern->type_specifier_list)
        pattern->type_specifier_list = node->type_specifier_list;
    else if (! AST::match(node->type_specifier_list, pattern->type_specifier_list, this))
        return false;

    if (! pattern->declarator)
        pattern->declarator = node->declarator;
    else if (! AST::match(node->declarator, pattern->declarator, this))
        return false;

    return true;
}

bool ASTMatcher::match(ConditionalExpressionAST* node, ConditionalExpressionAST* pattern)
{
    (void) node;
    (void) pattern;

    if (! pattern->condition)
        pattern->condition = node->condition;
    else if (! AST::match(node->condition, pattern->condition, this))
        return false;

    pattern->question_token = node->question_token;

    if (! pattern->left_expression)
        pattern->left_expression = node->left_expression;
    else if (! AST::match(node->left_expression, pattern->left_expression, this))
        return false;

    pattern->colon_token = node->colon_token;

    if (! pattern->right_expression)
        pattern->right_expression = node->right_expression;
    else if (! AST::match(node->right_expression, pattern->right_expression, this))
        return false;

    return true;
}

bool ASTMatcher::match(CppCastExpressionAST* node, CppCastExpressionAST* pattern)
{
    (void) node;
    (void) pattern;

    pattern->cast_token = node->cast_token;

    pattern->less_token = node->less_token;

    if (! pattern->type_id)
        pattern->type_id = node->type_id;
    else if (! AST::match(node->type_id, pattern->type_id, this))
        return false;

    pattern->greater_token = node->greater_token;

    pattern->lparen_token = node->lparen_token;

    if (! pattern->expression)
        pattern->expression = node->expression;
    else if (! AST::match(node->expression, pattern->expression, this))
        return false;

    pattern->rparen_token = node->rparen_token;

    return true;
}

bool ASTMatcher::match(CtorInitializerAST* node, CtorInitializerAST* pattern)
{
    (void) node;
    (void) pattern;

    pattern->colon_token = node->colon_token;

    if (! pattern->member_initializer_list)
        pattern->member_initializer_list = node->member_initializer_list;
    else if (! AST::match(node->member_initializer_list, pattern->member_initializer_list, this))
        return false;

    pattern->dot_dot_dot_token = node->dot_dot_dot_token;

    return true;
}

bool ASTMatcher::match(DeclarationStatementAST* node, DeclarationStatementAST* pattern)
{
    (void) node;
    (void) pattern;

    if (! pattern->declaration)
        pattern->declaration = node->declaration;
    else if (! AST::match(node->declaration, pattern->declaration, this))
        return false;

    return true;
}

bool ASTMatcher::match(DeclaratorIdAST* node, DeclaratorIdAST* pattern)
{
    (void) node;
    (void) pattern;

    pattern->dot_dot_dot_token = node->dot_dot_dot_token;

    if (! pattern->name)
        pattern->name = node->name;
    else if (! AST::match(node->name, pattern->name, this))
        return false;

    return true;
}

bool ASTMatcher::match(NestedDeclaratorAST* node, NestedDeclaratorAST* pattern)
{
    (void) node;
    (void) pattern;

    pattern->lparen_token = node->lparen_token;

    if (! pattern->declarator)
        pattern->declarator = node->declarator;
    else if (! AST::match(node->declarator, pattern->declarator, this))
        return false;

    pattern->rparen_token = node->rparen_token;

    return true;
}

bool ASTMatcher::match(BitfieldDeclaratorAST* node, BitfieldDeclaratorAST* pattern)
{
    (void) node;
    (void) pattern;

    pattern->colon_token = node->colon_token;

    return true;
}

bool ASTMatcher::match(FunctionDeclaratorAST* node, FunctionDeclaratorAST* pattern)
{
    (void) node;
    (void) pattern;

    pattern->lparen_token = node->lparen_token;

    if (! pattern->parameter_declaration_clause)
        pattern->parameter_declaration_clause = node->parameter_declaration_clause;
    else if (! AST::match(node->parameter_declaration_clause, pattern->parameter_declaration_clause, this))
        return false;

    pattern->rparen_token = node->rparen_token;

    if (! pattern->cv_qualifier_list)
        pattern->cv_qualifier_list = node->cv_qualifier_list;
    else if (! AST::match(node->cv_qualifier_list, pattern->cv_qualifier_list, this))
        return false;

    pattern->ref_qualifier_token = node->ref_qualifier_token;

    if (! pattern->exception_specification)
        pattern->exception_specification = node->exception_specification;
    else if (! AST::match(node->exception_specification, pattern->exception_specification, this))
        return false;

    if (! pattern->trailing_return_type)
        pattern->trailing_return_type = node->trailing_return_type;
    else if (! AST::match(node->trailing_return_type, pattern->trailing_return_type, this))
        return false;

    if (! pattern->as_cpp_initializer)
        pattern->as_cpp_initializer = node->as_cpp_initializer;
    else if (! AST::match(node->as_cpp_initializer, pattern->as_cpp_initializer, this))
        return false;

    return true;
}

bool ASTMatcher::match(ArrayDeclaratorAST* node, ArrayDeclaratorAST* pattern)
{
    (void) node;
    (void) pattern;

    pattern->lbracket_token = node->lbracket_token;

    if (! pattern->expression)
        pattern->expression = node->expression;
    else if (! AST::match(node->expression, pattern->expression, this))
        return false;

    pattern->rbracket_token = node->rbracket_token;

    return true;
}

bool ASTMatcher::match(DeleteExpressionAST* node, DeleteExpressionAST* pattern)
{
    (void) node;
    (void) pattern;

    pattern->scope_token = node->scope_token;

    pattern->delete_token = node->delete_token;

    pattern->lbracket_token = node->lbracket_token;

    pattern->rbracket_token = node->rbracket_token;

    if (! pattern->expression)
        pattern->expression = node->expression;
    else if (! AST::match(node->expression, pattern->expression, this))
        return false;

    return true;
}

bool ASTMatcher::match(DoStatementAST* node, DoStatementAST* pattern)
{
    (void) node;
    (void) pattern;

    pattern->do_token = node->do_token;

    if (! pattern->statement)
        pattern->statement = node->statement;
    else if (! AST::match(node->statement, pattern->statement, this))
        return false;

    pattern->while_token = node->while_token;

    pattern->lparen_token = node->lparen_token;

    if (! pattern->expression)
        pattern->expression = node->expression;
    else if (! AST::match(node->expression, pattern->expression, this))
        return false;

    pattern->rparen_token = node->rparen_token;

    pattern->semicolon_token = node->semicolon_token;

    return true;
}

bool ASTMatcher::match(NamedTypeSpecifierAST* node, NamedTypeSpecifierAST* pattern)
{
    (void) node;
    (void) pattern;

    if (! pattern->name)
        pattern->name = node->name;
    else if (! AST::match(node->name, pattern->name, this))
        return false;

    return true;
}

bool ASTMatcher::match(ElaboratedTypeSpecifierAST* node, ElaboratedTypeSpecifierAST* pattern)
{
    (void) node;
    (void) pattern;

    if (! pattern->attribute_list)
        pattern->attribute_list = node->attribute_list;
    else if (! AST::match(node->attribute_list, pattern->attribute_list, this))
        return false;

    if (! pattern->name)
        pattern->name = node->name;
    else if (! AST::match(node->name, pattern->name, this))
        return false;

    return true;
}

bool ASTMatcher::match(EnumSpecifierAST* node, EnumSpecifierAST* pattern)
{
    (void) node;
    (void) pattern;

    pattern->enum_token = node->enum_token;

    pattern->key_token = node->key_token;

    if (! pattern->name)
        pattern->name = node->name;
    else if (! AST::match(node->name, pattern->name, this))
        return false;

    pattern->colon_token = node->colon_token;

    if (! pattern->type_specifier_list)
        pattern->type_specifier_list = node->type_specifier_list;
    else if (! AST::match(node->type_specifier_list, pattern->type_specifier_list, this))
        return false;

    pattern->lbrace_token = node->lbrace_token;

    if (! pattern->enumerator_list)
        pattern->enumerator_list = node->enumerator_list;
    else if (! AST::match(node->enumerator_list, pattern->enumerator_list, this))
        return false;

    pattern->stray_comma_token = node->stray_comma_token;

    pattern->rbrace_token = node->rbrace_token;

    return true;
}

bool ASTMatcher::match(EnumeratorAST* node, EnumeratorAST* pattern)
{
    (void) node;
    (void) pattern;

    pattern->identifier_token = node->identifier_token;

    pattern->equal_token = node->equal_token;

    if (! pattern->expression)
        pattern->expression = node->expression;
    else if (! AST::match(node->expression, pattern->expression, this))
        return false;

    return true;
}

bool ASTMatcher::match(ExceptionDeclarationAST* node, ExceptionDeclarationAST* pattern)
{
    (void) node;
    (void) pattern;

    if (! pattern->type_specifier_list)
        pattern->type_specifier_list = node->type_specifier_list;
    else if (! AST::match(node->type_specifier_list, pattern->type_specifier_list, this))
        return false;

    if (! pattern->declarator)
        pattern->declarator = node->declarator;
    else if (! AST::match(node->declarator, pattern->declarator, this))
        return false;

    pattern->dot_dot_dot_token = node->dot_dot_dot_token;

    return true;
}

bool ASTMatcher::match(DynamicExceptionSpecificationAST* node, DynamicExceptionSpecificationAST* pattern)
{
    (void) node;
    (void) pattern;

    pattern->throw_token = node->throw_token;

    pattern->lparen_token = node->lparen_token;

    pattern->dot_dot_dot_token = node->dot_dot_dot_token;

    if (! pattern->type_id_list)
        pattern->type_id_list = node->type_id_list;
    else if (! AST::match(node->type_id_list, pattern->type_id_list, this))
        return false;

    pattern->rparen_token = node->rparen_token;

    return true;
}

bool ASTMatcher::match(TaggedNameAST* node, TaggedNameAST* pattern)
{
    (void) node;
    (void) pattern;

    pattern->tag_token = node->tag_token;

    if (! pattern->core_name)
        pattern->core_name = node->core_name;
    else if (! AST::match(node->core_name, pattern->core_name, this))
        return false;

    return true;
}

bool ASTMatcher::match(NoExceptSpecificationAST* node, NoExceptSpecificationAST* pattern)
{
    (void) node;
    (void) pattern;

    pattern->noexcept_token = node->noexcept_token;

    pattern->lparen_token = node->lparen_token;

    if (! pattern->expression)
        pattern->expression = node->expression;
    else if (! AST::match(node->expression, pattern->expression, this))
        return false;

    pattern->rparen_token = node->rparen_token;

    return true;
}

bool ASTMatcher::match(ExpressionOrDeclarationStatementAST* node, ExpressionOrDeclarationStatementAST* pattern)
{
    (void) node;
    (void) pattern;

    if (! pattern->expression)
        pattern->expression = node->expression;
    else if (! AST::match(node->expression, pattern->expression, this))
        return false;

    if (! pattern->declaration)
        pattern->declaration = node->declaration;
    else if (! AST::match(node->declaration, pattern->declaration, this))
        return false;

    return true;
}

bool ASTMatcher::match(ExpressionStatementAST* node, ExpressionStatementAST* pattern)
{
    (void) node;
    (void) pattern;

    if (! pattern->expression)
        pattern->expression = node->expression;
    else if (! AST::match(node->expression, pattern->expression, this))
        return false;

    pattern->semicolon_token = node->semicolon_token;

    return true;
}

bool ASTMatcher::match(FunctionDefinitionAST* node, FunctionDefinitionAST* pattern)
{
    (void) node;
    (void) pattern;

    if (! pattern->decl_specifier_list)
        pattern->decl_specifier_list = node->decl_specifier_list;
    else if (! AST::match(node->decl_specifier_list, pattern->decl_specifier_list, this))
        return false;

    if (! pattern->declarator)
        pattern->declarator = node->declarator;
    else if (! AST::match(node->declarator, pattern->declarator, this))
        return false;

    if (! pattern->ctor_initializer)
        pattern->ctor_initializer = node->ctor_initializer;
    else if (! AST::match(node->ctor_initializer, pattern->ctor_initializer, this))
        return false;

    if (! pattern->function_body)
        pattern->function_body = node->function_body;
    else if (! AST::match(node->function_body, pattern->function_body, this))
        return false;

    return true;
}

bool ASTMatcher::match(ForeachStatementAST* node, ForeachStatementAST* pattern)
{
    (void) node;
    (void) pattern;

    pattern->foreach_token = node->foreach_token;

    pattern->lparen_token = node->lparen_token;

    if (! pattern->type_specifier_list)
        pattern->type_specifier_list = node->type_specifier_list;
    else if (! AST::match(node->type_specifier_list, pattern->type_specifier_list, this))
        return false;

    if (! pattern->declarator)
        pattern->declarator = node->declarator;
    else if (! AST::match(node->declarator, pattern->declarator, this))
        return false;

    if (! pattern->initializer)
        pattern->initializer = node->initializer;
    else if (! AST::match(node->initializer, pattern->initializer, this))
        return false;

    pattern->comma_token = node->comma_token;

    if (! pattern->expression)
        pattern->expression = node->expression;
    else if (! AST::match(node->expression, pattern->expression, this))
        return false;

    pattern->rparen_token = node->rparen_token;

    if (! pattern->statement)
        pattern->statement = node->statement;
    else if (! AST::match(node->statement, pattern->statement, this))
        return false;

    return true;
}

bool ASTMatcher::match(RangeBasedForStatementAST* node, RangeBasedForStatementAST* pattern)
{
    (void) node;
    (void) pattern;

    pattern->for_token = node->for_token;

    pattern->lparen_token = node->lparen_token;

    if (! pattern->type_specifier_list)
        pattern->type_specifier_list = node->type_specifier_list;
    else if (! AST::match(node->type_specifier_list, pattern->type_specifier_list, this))
        return false;

    if (! pattern->declarator)
        pattern->declarator = node->declarator;
    else if (! AST::match(node->declarator, pattern->declarator, this))
        return false;

    pattern->colon_token = node->colon_token;

    if (! pattern->expression)
        pattern->expression = node->expression;
    else if (! AST::match(node->expression, pattern->expression, this))
        return false;

    pattern->rparen_token = node->rparen_token;

    if (! pattern->statement)
        pattern->statement = node->statement;
    else if (! AST::match(node->statement, pattern->statement, this))
        return false;

    return true;
}

bool ASTMatcher::match(ForStatementAST* node, ForStatementAST* pattern)
{
    (void) node;
    (void) pattern;

    pattern->for_token = node->for_token;

    pattern->lparen_token = node->lparen_token;

    if (! pattern->initializer)
        pattern->initializer = node->initializer;
    else if (! AST::match(node->initializer, pattern->initializer, this))
        return false;

    if (! pattern->condition)
        pattern->condition = node->condition;
    else if (! AST::match(node->condition, pattern->condition, this))
        return false;

    pattern->semicolon_token = node->semicolon_token;

    if (! pattern->expression)
        pattern->expression = node->expression;
    else if (! AST::match(node->expression, pattern->expression, this))
        return false;

    pattern->rparen_token = node->rparen_token;

    if (! pattern->statement)
        pattern->statement = node->statement;
    else if (! AST::match(node->statement, pattern->statement, this))
        return false;

    return true;
}

bool ASTMatcher::match(IfStatementAST* node, IfStatementAST* pattern)
{
    (void) node;
    (void) pattern;

    pattern->if_token = node->if_token;

    pattern->lparen_token = node->lparen_token;

    if (! pattern->condition)
        pattern->condition = node->condition;
    else if (! AST::match(node->condition, pattern->condition, this))
        return false;

    pattern->rparen_token = node->rparen_token;

    if (! pattern->statement)
        pattern->statement = node->statement;
    else if (! AST::match(node->statement, pattern->statement, this))
        return false;

    pattern->else_token = node->else_token;

    if (! pattern->else_statement)
        pattern->else_statement = node->else_statement;
    else if (! AST::match(node->else_statement, pattern->else_statement, this))
        return false;

    return true;
}

bool ASTMatcher::match(ArrayInitializerAST* node, ArrayInitializerAST* pattern)
{
    (void) node;
    (void) pattern;

    pattern->lbrace_token = node->lbrace_token;

    if (! pattern->expression_list)
        pattern->expression_list = node->expression_list;
    else if (! AST::match(node->expression_list, pattern->expression_list, this))
        return false;

    pattern->rbrace_token = node->rbrace_token;

    return true;
}

bool ASTMatcher::match(LabeledStatementAST* node, LabeledStatementAST* pattern)
{
    (void) node;
    (void) pattern;

    pattern->label_token = node->label_token;

    pattern->colon_token = node->colon_token;

    if (! pattern->statement)
        pattern->statement = node->statement;
    else if (! AST::match(node->statement, pattern->statement, this))
        return false;

    return true;
}

bool ASTMatcher::match(LinkageBodyAST* node, LinkageBodyAST* pattern)
{
    (void) node;
    (void) pattern;

    pattern->lbrace_token = node->lbrace_token;

    if (! pattern->declaration_list)
        pattern->declaration_list = node->declaration_list;
    else if (! AST::match(node->declaration_list, pattern->declaration_list, this))
        return false;

    pattern->rbrace_token = node->rbrace_token;

    return true;
}

bool ASTMatcher::match(LinkageSpecificationAST* node, LinkageSpecificationAST* pattern)
{
    (void) node;
    (void) pattern;

    pattern->extern_token = node->extern_token;

    pattern->extern_type_token = node->extern_type_token;

    if (! pattern->declaration)
        pattern->declaration = node->declaration;
    else if (! AST::match(node->declaration, pattern->declaration, this))
        return false;

    return true;
}

bool ASTMatcher::match(MemInitializerAST* node, MemInitializerAST* pattern)
{
    (void) node;
    (void) pattern;

    if (! pattern->name)
        pattern->name = node->name;
    else if (! AST::match(node->name, pattern->name, this))
        return false;

    if (! pattern->expression)
        pattern->expression = node->expression;
    else if (! AST::match(node->expression, pattern->expression, this))
        return false;

    return true;
}

bool ASTMatcher::match(NestedNameSpecifierAST* node, NestedNameSpecifierAST* pattern)
{
    (void) node;
    (void) pattern;

    if (! pattern->class_or_namespace_name)
        pattern->class_or_namespace_name = node->class_or_namespace_name;
    else if (! AST::match(node->class_or_namespace_name, pattern->class_or_namespace_name, this))
        return false;

    pattern->scope_token = node->scope_token;

    return true;
}

bool ASTMatcher::match(QualifiedNameAST* node, QualifiedNameAST* pattern)
{
    (void) node;
    (void) pattern;

    pattern->global_scope_token = node->global_scope_token;

    if (! pattern->nested_name_specifier_list)
        pattern->nested_name_specifier_list = node->nested_name_specifier_list;
    else if (! AST::match(node->nested_name_specifier_list, pattern->nested_name_specifier_list, this))
        return false;

    if (! pattern->unqualified_name)
        pattern->unqualified_name = node->unqualified_name;
    else if (! AST::match(node->unqualified_name, pattern->unqualified_name, this))
        return false;

    return true;
}

bool ASTMatcher::match(QuantifiedTypeSpecifierAST* node, QuantifiedTypeSpecifierAST* pattern)
{
    (void) node;
    (void) pattern;

    pattern->quantifier_token = node->quantifier_token;

    pattern->lparen_token = node->lparen_token;

    if (! pattern->name)
        pattern->name = node->name;
    else if (! AST::match(node->name, pattern->name, this))
        return false;

    pattern->rparen_token = node->rparen_token;

    return true;
}

bool ASTMatcher::match(OperatorFunctionIdAST* node, OperatorFunctionIdAST* pattern)
{
    (void) node;
    (void) pattern;

    pattern->operator_token = node->operator_token;

    if (! pattern->op)
        pattern->op = node->op;
    else if (! AST::match(node->op, pattern->op, this))
        return false;

    return true;
}

bool ASTMatcher::match(ConversionFunctionIdAST* node, ConversionFunctionIdAST* pattern)
{
    (void) node;
    (void) pattern;

    pattern->operator_token = node->operator_token;

    if (! pattern->type_specifier_list)
        pattern->type_specifier_list = node->type_specifier_list;
    else if (! AST::match(node->type_specifier_list, pattern->type_specifier_list, this))
        return false;

    if (! pattern->ptr_operator_list)
        pattern->ptr_operator_list = node->ptr_operator_list;
    else if (! AST::match(node->ptr_operator_list, pattern->ptr_operator_list, this))
        return false;

    return true;
}

bool ASTMatcher::match(EmptyNameAST* node, EmptyNameAST* pattern)
{
    (void) node;
    (void) pattern;

    return true;
}

bool ASTMatcher::match(SimpleNameAST* node, SimpleNameAST* pattern)
{
    (void) node;
    (void) pattern;

    pattern->identifier_token = node->identifier_token;

    return true;
}

bool ASTMatcher::match(DestructorNameAST* node, DestructorNameAST* pattern)
{
    (void) node;
    (void) pattern;

    pattern->tilde_token = node->tilde_token;

    if (! pattern->unqualified_name)
        pattern->unqualified_name = node->unqualified_name;
    else if (! AST::match(node->unqualified_name, pattern->unqualified_name, this))
        return false;

    return true;
}

bool ASTMatcher::match(TemplateIdAST* node, TemplateIdAST* pattern)
{
    (void) node;
    (void) pattern;

    pattern->template_token = node->template_token;

    pattern->identifier_token = node->identifier_token;

    pattern->less_token = node->less_token;

    if (! pattern->template_argument_list)
        pattern->template_argument_list = node->template_argument_list;
    else if (! AST::match(node->template_argument_list, pattern->template_argument_list, this))
        return false;

    pattern->greater_token = node->greater_token;

    return true;
}

bool ASTMatcher::match(NamespaceAST* node, NamespaceAST* pattern)
{
    (void) node;
    (void) pattern;

    pattern->inline_token = node->inline_token;

    pattern->namespace_token = node->namespace_token;

    pattern->identifier_token = node->identifier_token;

    if (! pattern->attribute_list)
        pattern->attribute_list = node->attribute_list;
    else if (! AST::match(node->attribute_list, pattern->attribute_list, this))
        return false;

    if (! pattern->linkage_body)
        pattern->linkage_body = node->linkage_body;
    else if (! AST::match(node->linkage_body, pattern->linkage_body, this))
        return false;

    return true;
}

bool ASTMatcher::match(NamespaceAliasDefinitionAST* node, NamespaceAliasDefinitionAST* pattern)
{
    (void) node;
    (void) pattern;

    pattern->namespace_token = node->namespace_token;

    pattern->namespace_name_token = node->namespace_name_token;

    pattern->equal_token = node->equal_token;

    if (! pattern->name)
        pattern->name = node->name;
    else if (! AST::match(node->name, pattern->name, this))
        return false;

    pattern->semicolon_token = node->semicolon_token;

    return true;
}

bool ASTMatcher::match(AliasDeclarationAST* node, AliasDeclarationAST* pattern)
{
    (void) node;
    (void) pattern;

    pattern->using_token = node->using_token;

    if (! pattern->name)
        pattern->name = node->name;
    else if (! AST::match(node->name, pattern->name, this))
        return false;

    pattern->equal_token = node->equal_token;

    if (! pattern->typeId)
        pattern->typeId = node->typeId;
    else if (! AST::match(node->typeId, pattern->typeId, this))
        return false;

    pattern->semicolon_token = node->semicolon_token;

    return true;
}

bool ASTMatcher::match(ExpressionListParenAST* node, ExpressionListParenAST* pattern)
{
    (void) node;
    (void) pattern;

    pattern->lparen_token = node->lparen_token;

    if (! pattern->expression_list)
        pattern->expression_list = node->expression_list;
    else if (! AST::match(node->expression_list, pattern->expression_list, this))
        return false;

    pattern->rparen_token = node->rparen_token;

    return true;
}

bool ASTMatcher::match(NewArrayDeclaratorAST* node, NewArrayDeclaratorAST* pattern)
{
    (void) node;
    (void) pattern;

    pattern->lbracket_token = node->lbracket_token;

    if (! pattern->expression)
        pattern->expression = node->expression;
    else if (! AST::match(node->expression, pattern->expression, this))
        return false;

    pattern->rbracket_token = node->rbracket_token;

    return true;
}

bool ASTMatcher::match(NewExpressionAST* node, NewExpressionAST* pattern)
{
    (void) node;
    (void) pattern;

    pattern->scope_token = node->scope_token;

    pattern->new_token = node->new_token;

    if (! pattern->new_placement)
        pattern->new_placement = node->new_placement;
    else if (! AST::match(node->new_placement, pattern->new_placement, this))
        return false;

    pattern->lparen_token = node->lparen_token;

    if (! pattern->type_id)
        pattern->type_id = node->type_id;
    else if (! AST::match(node->type_id, pattern->type_id, this))
        return false;

    pattern->rparen_token = node->rparen_token;

    if (! pattern->new_type_id)
        pattern->new_type_id = node->new_type_id;
    else if (! AST::match(node->new_type_id, pattern->new_type_id, this))
        return false;

    if (! pattern->new_initializer)
        pattern->new_initializer = node->new_initializer;
    else if (! AST::match(node->new_initializer, pattern->new_initializer, this))
        return false;

    return true;
}

bool ASTMatcher::match(NewTypeIdAST* node, NewTypeIdAST* pattern)
{
    (void) node;
    (void) pattern;

    if (! pattern->type_specifier_list)
        pattern->type_specifier_list = node->type_specifier_list;
    else if (! AST::match(node->type_specifier_list, pattern->type_specifier_list, this))
        return false;

    if (! pattern->ptr_operator_list)
        pattern->ptr_operator_list = node->ptr_operator_list;
    else if (! AST::match(node->ptr_operator_list, pattern->ptr_operator_list, this))
        return false;

    if (! pattern->new_array_declarator_list)
        pattern->new_array_declarator_list = node->new_array_declarator_list;
    else if (! AST::match(node->new_array_declarator_list, pattern->new_array_declarator_list, this))
        return false;

    return true;
}

bool ASTMatcher::match(OperatorAST* node, OperatorAST* pattern)
{
    (void) node;
    (void) pattern;

    pattern->op_token = node->op_token;

    pattern->open_token = node->open_token;

    pattern->close_token = node->close_token;

    return true;
}

bool ASTMatcher::match(ParameterDeclarationAST* node, ParameterDeclarationAST* pattern)
{
    (void) node;
    (void) pattern;

    if (! pattern->type_specifier_list)
        pattern->type_specifier_list = node->type_specifier_list;
    else if (! AST::match(node->type_specifier_list, pattern->type_specifier_list, this))
        return false;

    if (! pattern->declarator)
        pattern->declarator = node->declarator;
    else if (! AST::match(node->declarator, pattern->declarator, this))
        return false;

    pattern->equal_token = node->equal_token;

    if (! pattern->expression)
        pattern->expression = node->expression;
    else if (! AST::match(node->expression, pattern->expression, this))
        return false;

    return true;
}

bool ASTMatcher::match(ParameterDeclarationClauseAST* node, ParameterDeclarationClauseAST* pattern)
{
    (void) node;
    (void) pattern;

    if (! pattern->parameter_declaration_list)
        pattern->parameter_declaration_list = node->parameter_declaration_list;
    else if (! AST::match(node->parameter_declaration_list, pattern->parameter_declaration_list, this))
        return false;

    pattern->dot_dot_dot_token = node->dot_dot_dot_token;

    return true;
}

bool ASTMatcher::match(CallAST* node, CallAST* pattern)
{
    (void) node;
    (void) pattern;

    if (! pattern->base_expression)
        pattern->base_expression = node->base_expression;
    else if (! AST::match(node->base_expression, pattern->base_expression, this))
        return false;

    pattern->lparen_token = node->lparen_token;

    if (! pattern->expression_list)
        pattern->expression_list = node->expression_list;
    else if (! AST::match(node->expression_list, pattern->expression_list, this))
        return false;

    pattern->rparen_token = node->rparen_token;

    return true;
}

bool ASTMatcher::match(ArrayAccessAST* node, ArrayAccessAST* pattern)
{
    (void) node;
    (void) pattern;

    if (! pattern->base_expression)
        pattern->base_expression = node->base_expression;
    else if (! AST::match(node->base_expression, pattern->base_expression, this))
        return false;

    pattern->lbracket_token = node->lbracket_token;

    if (! pattern->expression)
        pattern->expression = node->expression;
    else if (! AST::match(node->expression, pattern->expression, this))
        return false;

    pattern->rbracket_token = node->rbracket_token;

    return true;
}

bool ASTMatcher::match(PostIncrDecrAST* node, PostIncrDecrAST* pattern)
{
    (void) node;
    (void) pattern;

    if (! pattern->base_expression)
        pattern->base_expression = node->base_expression;
    else if (! AST::match(node->base_expression, pattern->base_expression, this))
        return false;

    pattern->incr_decr_token = node->incr_decr_token;

    return true;
}

bool ASTMatcher::match(MemberAccessAST* node, MemberAccessAST* pattern)
{
    (void) node;
    (void) pattern;

    if (! pattern->base_expression)
        pattern->base_expression = node->base_expression;
    else if (! AST::match(node->base_expression, pattern->base_expression, this))
        return false;

    pattern->access_token = node->access_token;

    pattern->template_token = node->template_token;

    if (! pattern->member_name)
        pattern->member_name = node->member_name;
    else if (! AST::match(node->member_name, pattern->member_name, this))
        return false;

    return true;
}

bool ASTMatcher::match(TypeidExpressionAST* node, TypeidExpressionAST* pattern)
{
    (void) node;
    (void) pattern;

    pattern->typeid_token = node->typeid_token;

    pattern->lparen_token = node->lparen_token;

    if (! pattern->expression)
        pattern->expression = node->expression;
    else if (! AST::match(node->expression, pattern->expression, this))
        return false;

    pattern->rparen_token = node->rparen_token;

    return true;
}

bool ASTMatcher::match(TypenameCallExpressionAST* node, TypenameCallExpressionAST* pattern)
{
    (void) node;
    (void) pattern;

    pattern->typename_token = node->typename_token;

    if (! pattern->name)
        pattern->name = node->name;
    else if (! AST::match(node->name, pattern->name, this))
        return false;

    if (! pattern->expression)
        pattern->expression = node->expression;
    else if (! AST::match(node->expression, pattern->expression, this))
        return false;

    return true;
}

bool ASTMatcher::match(TypeConstructorCallAST* node, TypeConstructorCallAST* pattern)
{
    (void) node;
    (void) pattern;

    if (! pattern->type_specifier_list)
        pattern->type_specifier_list = node->type_specifier_list;
    else if (! AST::match(node->type_specifier_list, pattern->type_specifier_list, this))
        return false;

    if (! pattern->expression)
        pattern->expression = node->expression;
    else if (! AST::match(node->expression, pattern->expression, this))
        return false;

    return true;
}

bool ASTMatcher::match(PointerToMemberAST* node, PointerToMemberAST* pattern)
{
    (void) node;
    (void) pattern;

    pattern->global_scope_token = node->global_scope_token;

    if (! pattern->nested_name_specifier_list)
        pattern->nested_name_specifier_list = node->nested_name_specifier_list;
    else if (! AST::match(node->nested_name_specifier_list, pattern->nested_name_specifier_list, this))
        return false;

    pattern->star_token = node->star_token;

    if (! pattern->cv_qualifier_list)
        pattern->cv_qualifier_list = node->cv_qualifier_list;
    else if (! AST::match(node->cv_qualifier_list, pattern->cv_qualifier_list, this))
        return false;

    pattern->ref_qualifier_token = node->ref_qualifier_token;

    return true;
}

bool ASTMatcher::match(PointerAST* node, PointerAST* pattern)
{
    (void) node;
    (void) pattern;

    pattern->star_token = node->star_token;

    if (! pattern->cv_qualifier_list)
        pattern->cv_qualifier_list = node->cv_qualifier_list;
    else if (! AST::match(node->cv_qualifier_list, pattern->cv_qualifier_list, this))
        return false;

    return true;
}

bool ASTMatcher::match(ReferenceAST* node, ReferenceAST* pattern)
{
    (void) node;
    (void) pattern;

    pattern->reference_token = node->reference_token;

    return true;
}

bool ASTMatcher::match(BreakStatementAST* node, BreakStatementAST* pattern)
{
    (void) node;
    (void) pattern;

    pattern->break_token = node->break_token;

    pattern->semicolon_token = node->semicolon_token;

    return true;
}

bool ASTMatcher::match(ContinueStatementAST* node, ContinueStatementAST* pattern)
{
    (void) node;
    (void) pattern;

    pattern->continue_token = node->continue_token;

    pattern->semicolon_token = node->semicolon_token;

    return true;
}

bool ASTMatcher::match(GotoStatementAST* node, GotoStatementAST* pattern)
{
    (void) node;
    (void) pattern;

    pattern->goto_token = node->goto_token;

    pattern->identifier_token = node->identifier_token;

    pattern->semicolon_token = node->semicolon_token;

    return true;
}

bool ASTMatcher::match(ReturnStatementAST* node, ReturnStatementAST* pattern)
{
    (void) node;
    (void) pattern;

    pattern->return_token = node->return_token;

    if (! pattern->expression)
        pattern->expression = node->expression;
    else if (! AST::match(node->expression, pattern->expression, this))
        return false;

    pattern->semicolon_token = node->semicolon_token;

    return true;
}

bool ASTMatcher::match(SizeofExpressionAST* node, SizeofExpressionAST* pattern)
{
    (void) node;
    (void) pattern;

    pattern->sizeof_token = node->sizeof_token;

    pattern->dot_dot_dot_token = node->dot_dot_dot_token;

    pattern->lparen_token = node->lparen_token;

    if (! pattern->expression)
        pattern->expression = node->expression;
    else if (! AST::match(node->expression, pattern->expression, this))
        return false;

    pattern->rparen_token = node->rparen_token;

    return true;
}

bool ASTMatcher::match(AlignofExpressionAST* node, AlignofExpressionAST* pattern)
{
    (void) node;
    (void) pattern;

    pattern->alignof_token = node->alignof_token;

    pattern->lparen_token = node->lparen_token;

    if (! pattern->typeId)
        pattern->typeId = node->typeId;
    else if (! AST::match(node->typeId, pattern->typeId, this))
        return false;

    pattern->rparen_token = node->rparen_token;

    return true;
}

bool ASTMatcher::match(AmbiguousStatementAST* node, AmbiguousStatementAST* pattern)
{
    (void) node;
    (void) pattern;

    if (! pattern->declarationStmt)
        pattern->declarationStmt = node->declarationStmt;
    else if (! AST::match(node->declarationStmt, pattern->declarationStmt, this))
        return false;

    if (! pattern->expressionStmt)
        pattern->expressionStmt = node->expressionStmt;
    else if (! AST::match(node->expressionStmt, pattern->expressionStmt, this))
        return false;

    return true;
}

bool ASTMatcher::match(PointerLiteralAST* node, PointerLiteralAST* pattern)
{
    (void) node;
    (void) pattern;

    pattern->literal_token = node->literal_token;

    return true;
}

bool ASTMatcher::match(NumericLiteralAST* node, NumericLiteralAST* pattern)
{
    (void) node;
    (void) pattern;

    pattern->literal_token = node->literal_token;

    return true;
}

bool ASTMatcher::match(BoolLiteralAST* node, BoolLiteralAST* pattern)
{
    (void) node;
    (void) pattern;

    pattern->literal_token = node->literal_token;

    return true;
}

bool ASTMatcher::match(ThisExpressionAST* node, ThisExpressionAST* pattern)
{
    (void) node;
    (void) pattern;

    pattern->this_token = node->this_token;

    return true;
}

bool ASTMatcher::match(NestedExpressionAST* node, NestedExpressionAST* pattern)
{
    (void) node;
    (void) pattern;

    pattern->lparen_token = node->lparen_token;

    if (! pattern->expression)
        pattern->expression = node->expression;
    else if (! AST::match(node->expression, pattern->expression, this))
        return false;

    pattern->rparen_token = node->rparen_token;

    return true;
}

bool ASTMatcher::match(StaticAssertDeclarationAST* node, StaticAssertDeclarationAST* pattern)
{
    (void) node;
    (void) pattern;

    pattern->static_assert_token = node->static_assert_token;

    pattern->lparen_token = node->lparen_token;

    if (! pattern->expression)
        pattern->expression = node->expression;
    else if (! AST::match(node->expression, pattern->expression, this))
        return false;

    pattern->comma_token = node->comma_token;

    if (! pattern->string_literal)
        pattern->string_literal = node->string_literal;
    else if (! AST::match(node->string_literal, pattern->string_literal, this))
        return false;

    pattern->rparen_token = node->rparen_token;

    pattern->semicolon_token = node->semicolon_token;

    return true;
}

bool ASTMatcher::match(StringLiteralAST* node, StringLiteralAST* pattern)
{
    (void) node;
    (void) pattern;

    pattern->literal_token = node->literal_token;

    if (! pattern->next)
        pattern->next = node->next;
    else if (! AST::match(node->next, pattern->next, this))
        return false;

    return true;
}

bool ASTMatcher::match(SwitchStatementAST* node, SwitchStatementAST* pattern)
{
    (void) node;
    (void) pattern;

    pattern->switch_token = node->switch_token;

    pattern->lparen_token = node->lparen_token;

    if (! pattern->condition)
        pattern->condition = node->condition;
    else if (! AST::match(node->condition, pattern->condition, this))
        return false;

    pattern->rparen_token = node->rparen_token;

    if (! pattern->statement)
        pattern->statement = node->statement;
    else if (! AST::match(node->statement, pattern->statement, this))
        return false;

    return true;
}

bool ASTMatcher::match(TemplateDeclarationAST* node, TemplateDeclarationAST* pattern)
{
    (void) node;
    (void) pattern;

    pattern->export_token = node->export_token;

    pattern->template_token = node->template_token;

    pattern->less_token = node->less_token;

    if (! pattern->template_parameter_list)
        pattern->template_parameter_list = node->template_parameter_list;
    else if (! AST::match(node->template_parameter_list, pattern->template_parameter_list, this))
        return false;

    pattern->greater_token = node->greater_token;

    if (! pattern->declaration)
        pattern->declaration = node->declaration;
    else if (! AST::match(node->declaration, pattern->declaration, this))
        return false;

    return true;
}

bool ASTMatcher::match(ThrowExpressionAST* node, ThrowExpressionAST* pattern)
{
    (void) node;
    (void) pattern;

    pattern->throw_token = node->throw_token;

    if (! pattern->expression)
        pattern->expression = node->expression;
    else if (! AST::match(node->expression, pattern->expression, this))
        return false;

    return true;
}

bool ASTMatcher::match(NoExceptOperatorExpressionAST* node, NoExceptOperatorExpressionAST* pattern)
{
    (void) node;
    (void) pattern;

    pattern->noexcept_token = node->noexcept_token;

    if (! pattern->expression)
        pattern->expression = node->expression;
    else if (! AST::match(node->expression, pattern->expression, this))
        return false;

    return true;
}

bool ASTMatcher::match(TranslationUnitAST* node, TranslationUnitAST* pattern)
{
    (void) node;
    (void) pattern;

    if (! pattern->declaration_list)
        pattern->declaration_list = node->declaration_list;
    else if (! AST::match(node->declaration_list, pattern->declaration_list, this))
        return false;

    return true;
}

bool ASTMatcher::match(TryBlockStatementAST* node, TryBlockStatementAST* pattern)
{
    (void) node;
    (void) pattern;

    pattern->try_token = node->try_token;

    if (! pattern->statement)
        pattern->statement = node->statement;
    else if (! AST::match(node->statement, pattern->statement, this))
        return false;

    if (! pattern->catch_clause_list)
        pattern->catch_clause_list = node->catch_clause_list;
    else if (! AST::match(node->catch_clause_list, pattern->catch_clause_list, this))
        return false;

    return true;
}

bool ASTMatcher::match(CatchClauseAST* node, CatchClauseAST* pattern)
{
    (void) node;
    (void) pattern;

    pattern->catch_token = node->catch_token;

    pattern->lparen_token = node->lparen_token;

    if (! pattern->exception_declaration)
        pattern->exception_declaration = node->exception_declaration;
    else if (! AST::match(node->exception_declaration, pattern->exception_declaration, this))
        return false;

    pattern->rparen_token = node->rparen_token;

    if (! pattern->statement)
        pattern->statement = node->statement;
    else if (! AST::match(node->statement, pattern->statement, this))
        return false;

    return true;
}

bool ASTMatcher::match(TypeIdAST* node, TypeIdAST* pattern)
{
    (void) node;
    (void) pattern;

    if (! pattern->type_specifier_list)
        pattern->type_specifier_list = node->type_specifier_list;
    else if (! AST::match(node->type_specifier_list, pattern->type_specifier_list, this))
        return false;

    if (! pattern->declarator)
        pattern->declarator = node->declarator;
    else if (! AST::match(node->declarator, pattern->declarator, this))
        return false;

    return true;
}

bool ASTMatcher::match(TypenameTypeParameterAST* node, TypenameTypeParameterAST* pattern)
{
    (void) node;
    (void) pattern;

    pattern->classkey_token = node->classkey_token;

    pattern->dot_dot_dot_token = node->dot_dot_dot_token;

    if (! pattern->name)
        pattern->name = node->name;
    else if (! AST::match(node->name, pattern->name, this))
        return false;

    pattern->equal_token = node->equal_token;

    if (! pattern->type_id)
        pattern->type_id = node->type_id;
    else if (! AST::match(node->type_id, pattern->type_id, this))
        return false;

    return true;
}

bool ASTMatcher::match(TemplateTypeParameterAST* node, TemplateTypeParameterAST* pattern)
{
    (void) node;
    (void) pattern;

    pattern->template_token = node->template_token;

    pattern->less_token = node->less_token;

    if (! pattern->template_parameter_list)
        pattern->template_parameter_list = node->template_parameter_list;
    else if (! AST::match(node->template_parameter_list, pattern->template_parameter_list, this))
        return false;

    pattern->greater_token = node->greater_token;

    pattern->class_token = node->class_token;

    pattern->dot_dot_dot_token = node->dot_dot_dot_token;

    if (! pattern->name)
        pattern->name = node->name;
    else if (! AST::match(node->name, pattern->name, this))
        return false;

    pattern->equal_token = node->equal_token;

    if (! pattern->type_id)
        pattern->type_id = node->type_id;
    else if (! AST::match(node->type_id, pattern->type_id, this))
        return false;

    return true;
}

bool ASTMatcher::match(UnaryExpressionAST* node, UnaryExpressionAST* pattern)
{
    (void) node;
    (void) pattern;

    pattern->unary_op_token = node->unary_op_token;

    if (! pattern->expression)
        pattern->expression = node->expression;
    else if (! AST::match(node->expression, pattern->expression, this))
        return false;

    return true;
}

bool ASTMatcher::match(UsingAST* node, UsingAST* pattern)
{
    (void) node;
    (void) pattern;

    pattern->using_token = node->using_token;

    pattern->typename_token = node->typename_token;

    if (! pattern->name)
        pattern->name = node->name;
    else if (! AST::match(node->name, pattern->name, this))
        return false;

    pattern->semicolon_token = node->semicolon_token;

    return true;
}

bool ASTMatcher::match(UsingDirectiveAST* node, UsingDirectiveAST* pattern)
{
    (void) node;
    (void) pattern;

    pattern->using_token = node->using_token;

    pattern->namespace_token = node->namespace_token;

    if (! pattern->name)
        pattern->name = node->name;
    else if (! AST::match(node->name, pattern->name, this))
        return false;

    pattern->semicolon_token = node->semicolon_token;

    return true;
}

bool ASTMatcher::match(WhileStatementAST* node, WhileStatementAST* pattern)
{
    (void) node;
    (void) pattern;

    pattern->while_token = node->while_token;

    pattern->lparen_token = node->lparen_token;

    if (! pattern->condition)
        pattern->condition = node->condition;
    else if (! AST::match(node->condition, pattern->condition, this))
        return false;

    pattern->rparen_token = node->rparen_token;

    if (! pattern->statement)
        pattern->statement = node->statement;
    else if (! AST::match(node->statement, pattern->statement, this))
        return false;

    return true;
}

bool ASTMatcher::match(LambdaExpressionAST* node, LambdaExpressionAST* pattern)
{
    (void) node;
    (void) pattern;

    if (! pattern->lambda_introducer)
        pattern->lambda_introducer = node->lambda_introducer;
    else if (! AST::match(node->lambda_introducer, pattern->lambda_introducer, this))
        return false;

    if (! pattern->lambda_declarator)
        pattern->lambda_declarator = node->lambda_declarator;
    else if (! AST::match(node->lambda_declarator, pattern->lambda_declarator, this))
        return false;

    if (! pattern->statement)
        pattern->statement = node->statement;
    else if (! AST::match(node->statement, pattern->statement, this))
        return false;

    return true;
}

bool ASTMatcher::match(LambdaIntroducerAST* node, LambdaIntroducerAST* pattern)
{
    (void) node;
    (void) pattern;

    pattern->lbracket_token = node->lbracket_token;

    if (! pattern->lambda_capture)
        pattern->lambda_capture = node->lambda_capture;
    else if (! AST::match(node->lambda_capture, pattern->lambda_capture, this))
        return false;

    pattern->rbracket_token = node->rbracket_token;

    return true;
}

bool ASTMatcher::match(LambdaCaptureAST* node, LambdaCaptureAST* pattern)
{
    (void) node;
    (void) pattern;

    pattern->default_capture_token = node->default_capture_token;

    if (! pattern->capture_list)
        pattern->capture_list = node->capture_list;
    else if (! AST::match(node->capture_list, pattern->capture_list, this))
        return false;

    return true;
}

bool ASTMatcher::match(CaptureAST* node, CaptureAST* pattern)
{
    (void) node;
    (void) pattern;

    pattern->amper_token = node->amper_token;

    if (! pattern->identifier)
        pattern->identifier = node->identifier;
    else if (! AST::match(node->identifier, pattern->identifier, this))
        return false;

    return true;
}

bool ASTMatcher::match(LambdaDeclaratorAST* node, LambdaDeclaratorAST* pattern)
{
    (void) node;
    (void) pattern;

    pattern->lparen_token = node->lparen_token;

    if (! pattern->parameter_declaration_clause)
        pattern->parameter_declaration_clause = node->parameter_declaration_clause;
    else if (! AST::match(node->parameter_declaration_clause, pattern->parameter_declaration_clause, this))
        return false;

    pattern->rparen_token = node->rparen_token;

    if (! pattern->attributes)
        pattern->attributes = node->attributes;
    else if (! AST::match(node->attributes, pattern->attributes, this))
        return false;

    pattern->mutable_token = node->mutable_token;

    if (! pattern->exception_specification)
        pattern->exception_specification = node->exception_specification;
    else if (! AST::match(node->exception_specification, pattern->exception_specification, this))
        return false;

    if (! pattern->trailing_return_type)
        pattern->trailing_return_type = node->trailing_return_type;
    else if (! AST::match(node->trailing_return_type, pattern->trailing_return_type, this))
        return false;

    return true;
}

bool ASTMatcher::match(TrailingReturnTypeAST* node, TrailingReturnTypeAST* pattern)
{
    (void) node;
    (void) pattern;

    pattern->arrow_token = node->arrow_token;

    if (! pattern->attributes)
        pattern->attributes = node->attributes;
    else if (! AST::match(node->attributes, pattern->attributes, this))
        return false;

    if (! pattern->type_specifier_list)
        pattern->type_specifier_list = node->type_specifier_list;
    else if (! AST::match(node->type_specifier_list, pattern->type_specifier_list, this))
        return false;

    if (! pattern->declarator)
        pattern->declarator = node->declarator;
    else if (! AST::match(node->declarator, pattern->declarator, this))
        return false;

    return true;
}

bool ASTMatcher::match(BracedInitializerAST* node, BracedInitializerAST* pattern)
{
    (void) node;
    (void) pattern;

    pattern->lbrace_token = node->lbrace_token;

    if (! pattern->expression_list)
        pattern->expression_list = node->expression_list;
    else if (! AST::match(node->expression_list, pattern->expression_list, this))
        return false;

    pattern->comma_token = node->comma_token;

    pattern->rbrace_token = node->rbrace_token;

    return true;
}

bool ASTMatcher::match(DotDesignatorAST* node, DotDesignatorAST* pattern)
{
    (void) node;
    (void) pattern;

    pattern->dot_token = node->dot_token;

    pattern->identifier_token = node->identifier_token;

    return true;
}

bool ASTMatcher::match(BracketDesignatorAST* node, BracketDesignatorAST* pattern)
{
    (void) node;
    (void) pattern;

    pattern->lbracket_token = node->lbracket_token;

    if (! pattern->expression)
        pattern->expression = node->expression;
    else if (! AST::match(node->expression, pattern->expression, this))
        return false;

    pattern->rbracket_token = node->rbracket_token;

    return true;
}

bool ASTMatcher::match(DesignatedInitializerAST* node, DesignatedInitializerAST* pattern)
{
    (void) node;
    (void) pattern;

    if (! pattern->designator_list)
        pattern->designator_list = node->designator_list;
    else if (! AST::match(node->designator_list, pattern->designator_list, this))
        return false;

    pattern->equal_token = node->equal_token;

    if (! pattern->initializer)
        pattern->initializer = node->initializer;
    else if (! AST::match(node->initializer, pattern->initializer, this))
        return false;

    return true;
}

