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

#include "Binder.h"

#include "AST.h"
#include "Control.h"
#include "CoreTypes.h"
#include "Literals.h"
#include "Lookup.h"
#include "Names.h"
#include "Scope.h"
#include "Symbols.h"
#include "TranslationUnit.h"
#include <algorithm>
#include <vector>
#include <string>
#include <memory>
#include <sstream>
#include <string.h>
#include <stack>

using namespace psyche;
using namespace psyche;

const int Binder::kMaxDepth(100);

Binder::Binder(TranslationUnit *unit)
    : ASTVisitor(unit)
    , scope_(nullptr)
    , depth_(0)
    , exprTy_(nullptr)
    , name_(nullptr)
    , decltorIdent_(nullptr)
    , visibility_(Symbol::Public)
    , methodKey_(Function::NormalMethod)
    , skipFunctionBodies_(false)
    , quantTyLabel_(0)
    , parentAmbiguousStmt_(nullptr)
{}

bool Binder::skipFunctionBodies() const
{
    return skipFunctionBodies_;
}

void Binder::setSkipFunctionBodies(bool skipFunctionBodies)
{
    skipFunctionBodies_ = skipFunctionBodies;
}

unsigned Binder::location(DeclaratorAST* ast, unsigned defaultLocation) const
{
    if (! ast)
        return defaultLocation;

    else if (ast->core_declarator)
        return location(ast->core_declarator, defaultLocation);

    return ast->firstToken();
}

unsigned Binder::location(CoreDeclaratorAST* ast, unsigned defaultLocation) const
{
    if (! ast)
        return defaultLocation;

    else if (NestedDeclaratorAST* nested = ast->asNestedDeclarator())
        return location(nested->declarator, defaultLocation);

    else if (DeclaratorIdAST* id = ast->asDeclaratorId())
        return location(id->name, defaultLocation);

    return ast->firstToken();
}

unsigned Binder::location(NameAST* name, unsigned defaultLocation) const
{
    if (! name)
        return defaultLocation;

    else if (DestructorNameAST* dtor = name->asDestructorName())
        return location(dtor->unqualified_name, defaultLocation);

    else if (TemplateIdAST* templId = name->asTemplateId())
        return templId->identifier_token;

    else if (QualifiedNameAST* q = name->asQualifiedName()) {
        if (q->unqualified_name)
            return location(q->unqualified_name, defaultLocation);
    }

    return name->firstToken();
}

void Binder::setDeclSpecifiers(Symbol *symbol, const FullySpecifiedType &declSpecifiers)
{
    if (! symbol)
        return;

    int storage = Symbol::NoStorage;

    if (declSpecifiers.isFriend())
        storage = Symbol::Friend;
    else if (declSpecifiers.isAuto())
        storage = Symbol::Auto;
    else if (declSpecifiers.isRegister())
        storage = Symbol::Register;
    else if (declSpecifiers.isStatic())
        storage = Symbol::Static;
    else if (declSpecifiers.isExtern())
        storage = Symbol::Extern;
    else if (declSpecifiers.isMutable())
        storage = Symbol::Mutable;
    else if (declSpecifiers.isTypedef())
        storage = Symbol::Typedef;

    symbol->setStorage(storage);

    if (Function *funTy = symbol->asFunction()) {
        if (declSpecifiers.isVirtual())
            funTy->setVirtual(true);
    }

    if (declSpecifiers.isDeprecated())
        symbol->setDeprecated(true);

    if (declSpecifiers.isUnavailable())
        symbol->setUnavailable(true);
}

Scope *Binder::switchScope(Scope *scope)
{
    if (! scope)
        return scope_;

    std::swap(scope_, scope);
    return scope;
}

int Binder::switchVisibility(int visibility)
{
    std::swap(visibility_, visibility);
    return visibility;
}

int Binder::switchMethodKey(int methodKey)
{
    std::swap(methodKey_, methodKey);
    return methodKey;
}

void Binder::operator()(TranslationUnitAST* ast, Namespace *globalNamespace)
{
    Scope *previousScope = switchScope(globalNamespace);
    translationUnit(ast);
    (void) switchScope(previousScope);
}

void Binder::operator()(DeclarationAST* ast, Scope *scope)
{
    Scope *previousScope = switchScope(scope);
    declaration(ast);
    (void) switchScope(previousScope);
}

void Binder::operator()(StatementAST* ast, Scope *scope)
{
    Scope *previousScope = switchScope(scope);
    statement(ast);
    (void) switchScope(previousScope);
}

FullySpecifiedType Binder::operator()(ExpressionAST* ast, Scope *scope)
{
    Scope *previousScope = switchScope(scope);
    FullySpecifiedType ty = expression(ast);
    (void) switchScope(previousScope);
    return ty;
}

FullySpecifiedType Binder::operator()(NewTypeIdAST* ast, Scope *scope)
{
    Scope *previousScope = switchScope(scope);
    FullySpecifiedType ty = newTypeId(ast);
    (void) switchScope(previousScope);
    return ty;
}

void Binder::statement(StatementAST* ast)
{
    accept(ast);
}

Binder::ExpressionTy Binder::expression(ExpressionAST* ast)
{
    ExpressionTy value = ExpressionTy();
    std::swap(exprTy_, value);
    accept(ast);
    std::swap(exprTy_, value);
    return value;
}

void Binder::declaration(DeclarationAST* ast)
{
    accept(ast);
}

const Name *Binder::name(NameAST* ast)
{
    const Name *value = 0;
    std::swap(name_, value);
    accept(ast);
    std::swap(name_, value);
    return value;
}

FullySpecifiedType Binder::specifier(SpecifierAST* ast, const FullySpecifiedType &init)
{
    FullySpecifiedType value = init;
    std::swap(type_, value);
    accept(ast);
    std::swap(type_, value);
    return value;
}

FullySpecifiedType Binder::ptrOperator(PtrOperatorAST* ast, const FullySpecifiedType &init)
{
    FullySpecifiedType value = init;
    std::swap(type_, value);
    accept(ast);
    std::swap(type_, value);
    return value;
}

FullySpecifiedType Binder::coreDeclarator(CoreDeclaratorAST* ast, const FullySpecifiedType &init)
{
    FullySpecifiedType value = init;
    std::swap(type_, value);
    accept(ast);
    std::swap(type_, value);
    return value;
}

FullySpecifiedType Binder::postfixDeclarator(PostfixDeclaratorAST* ast, const FullySpecifiedType &init)
{
    FullySpecifiedType value = init;
    std::swap(type_, value);
    accept(ast);
    std::swap(type_, value);
    return value;
}

bool Binder::preVisit(AST* )
{
    ++depth_;
    if (depth_ > kMaxDepth)
        return false;
    return true;
}

void Binder::postVisit(AST* )
{
    --depth_;
}

bool Binder::visit(GnuAttributeAST* )
{
    return false;
}

void Binder::attribute(GnuAttributeAST* ast)
{
    if (! ast)
        return;

    // unsigned identifier_token = ast->identifier_token;
    if (const Identifier *id = identifier(ast->identifier_token)) {
        if (id == control()->deprecatedId())
            type_.setDeprecated(true);
        else if (id == control()->unavailableId())
            type_.setUnavailable(true);
    }

    // unsigned lparen_token = ast->lparen_token;
    // unsigned tag_token = ast->tag_token;
    for (ExpressionListAST* it = ast->expression_list; it; it = it->next) {
        ExpressionTy value = this->expression(it->value);
    }
    // unsigned rparen_token = ast->rparen_token;
}

bool Binder::visit(DeclaratorAST* )
{
    return false;
}

FullySpecifiedType Binder::declarator(DeclaratorAST* ast, const FullySpecifiedType &init, DeclaratorIdAST* *declaratorId)
{
    FullySpecifiedType type = init;

    if (! ast)
        return type;

    std::swap(decltorIdent_, declaratorId);
    bool isAuto = false;
    const bool cxx11Enabled = translationUnit()->dialect().cpp11;
    if (cxx11Enabled)
        isAuto = type.isAuto();

    for (SpecifierListAST* it = ast->attribute_list; it; it = it->next) {
        type = this->specifier(it->value, type);
        if (type.isAuto())
            isAuto = true;
    }
    for (PtrOperatorListAST* it = ast->ptr_operator_list; it; it = it->next) {
        type = this->ptrOperator(it->value, type);
    }
    for (PostfixDeclaratorListAST* it = ast->postfix_declarator_list; it; it = it->next) {
        type = this->postfixDeclarator(it->value, type);
    }
    type = this->coreDeclarator(ast->core_declarator, type);
    for (SpecifierListAST* it = ast->post_attribute_list; it; it = it->next) {
        type = this->specifier(it->value, type);
        if (type.isAuto())
            isAuto = true;
    }
    if (!type->isFunctionType()) {
        ExpressionTy initializer = this->expression(ast->initializer);
        if (cxx11Enabled && isAuto) {
            type = initializer;
            type.setAuto(true);
        }
    }

    std::swap(decltorIdent_, declaratorId);
    return type;
}

bool Binder::visit(BaseSpecifierAST* )
{
    return false;
}

void Binder::baseSpecifier(BaseSpecifierAST* ast, unsigned colon_token, Class *klass)
{
    if (! ast)
        return;

    unsigned sourceLocation = location(ast->name, ast->firstToken());
    if (! sourceLocation)
        sourceLocation = std::max(colon_token, klass->sourceLocation());

    const Name *baseClassName = this->name(ast->name);
    BaseClass *baseClass = control()->newBaseClass(sourceLocation, baseClassName);
    if (ast->virtual_token)
        baseClass->setVirtual(true);
    if (ast->access_specifier_token) {
        const int visibility = visibilityForAccessSpecifier(tokenKind(ast->access_specifier_token));
        baseClass->setVisibility(visibility); // ### well, not exactly.
    }
    if (ast->ellipsis_token)
        baseClass->setVariadic(true);
    klass->addBaseClass(baseClass);
    ast->symbol = baseClass;
}

bool Binder::visit(CtorInitializerAST* )
{
    return false;
}

void Binder::ctorInitializer(CtorInitializerAST* ast, Function *fun)
{
    if (! ast)
        return;

    // unsigned colon_token = ast->colon_token;
    for (MemInitializerListAST* it = ast->member_initializer_list; it; it = it->next) {
        this->memInitializer(it->value, fun);
    }
    // unsigned dot_dot_dot_token = ast->dot_dot_dot_token;
}

bool Binder::visit(EnumeratorAST* )
{
    return false;
}

namespace {

bool isInteger(const StringLiteral *stringLiteral)
{
    const int size = stringLiteral->size();
    const char *chars = stringLiteral->chars();
    int i = 0;
    if (chars[i] == '-')
        ++i;
    for (; i < size; ++i) {
        if (!isdigit(chars[i]))
            return false;
    }
    return true;
}

bool stringLiteralToInt(const StringLiteral *stringLiteral, int *output)
{
    if (!output)
        return false;

    if (!isInteger(stringLiteral)) {
        *output = 0;
        return false;
    }

    std::stringstream ss(std::string(stringLiteral->chars(), stringLiteral->size()));
    const bool ok = !(ss >> *output).fail();
    if (!ok)
        *output = 0;

    return ok;
}

void calculateConstantValue(const Symbol *symbol, EnumeratorDeclaration *e, Control *control)
{
    if (symbol) {
        if (const Declaration *decl = symbol->asDeclaration()) {
            if (const EnumeratorDeclaration *previousEnumDecl = decl->asEnumeratorDeclarator()) {
                if (const StringLiteral *constantValue = previousEnumDecl->constantValue()) {
                    int constantValueAsInt = 0;
                    if (stringLiteralToInt(constantValue, &constantValueAsInt)) {
                        ++constantValueAsInt;
                        const std::string buffer
                                = std::to_string(static_cast<long long>(constantValueAsInt));
                        e->setConstantValue(control->stringLiteral(buffer.c_str(),
                                                                   unsigned(buffer.size())));
                    }
                }
            }
        }
    }
}

const StringLiteral *valueOfEnumerator(const Enum *e, const Identifier *value) {
    const int enumMemberCount = e->memberCount();
    for (int i = 0; i < enumMemberCount; ++i) {
        const Symbol *member = e->memberAt(i);
        if (const Declaration *decl = member->asDeclaration()) {
            if (const EnumeratorDeclaration *enumDecl = decl->asEnumeratorDeclarator()) {
                if (const Name *enumDeclName = enumDecl->name()) {
                    if (const Identifier *enumDeclIdentifier = enumDeclName->identifier()) {
                        if (enumDeclIdentifier->equalTo(value))
                            return enumDecl->constantValue();
                    }
                }
            }
        }
    }
    return 0;
}

} // anonymous namespace

void Binder::enumerator(EnumeratorAST* ast, Enum *symbol)
{
    (void) symbol;

    if (! ast)
        return;

    // unsigned identifier_token = ast->identifier_token;
    // unsigned equal_token = ast->equal_token;
    /*ExpressionTy expression =*/ this->expression(ast->expression);

    if (ast->identifier_token) {
        const Name *name = identifier(ast->identifier_token);
        EnumeratorDeclaration *e = control()->newEnumeratorDeclaration(ast->identifier_token, name);
        e->setType(control()->integerType(IntegerType::Int)); // ### introduce IntegerType::Enumerator

        if (ExpressionAST* expr = ast->expression) {
            const int firstToken = expr->firstToken();
            const int lastToken = expr->lastToken();
            const StringLiteral *constantValue = asStringLiteral(expr);
            const StringLiteral *resolvedValue = 0;
            if (lastToken - firstToken == 1) {
                if (const Identifier *constantId = identifier(firstToken))
                    resolvedValue = valueOfEnumerator(symbol, constantId);
            }
            e->setConstantValue(resolvedValue ? resolvedValue : constantValue);
        } else if (!symbol->isEmpty()) {
            calculateConstantValue(*(symbol->memberEnd()-1), e, control());
        } else {
            e->setConstantValue(control()->stringLiteral("0", 1));
        }

        symbol->addMember(e);
    }
}

bool Binder::visit(DynamicExceptionSpecificationAST* )
{
    return false;
}

FullySpecifiedType Binder::exceptionSpecification(ExceptionSpecificationAST* ast, const FullySpecifiedType &init)
{
    FullySpecifiedType type = init;

    if (! ast)
        return type;

    if (DynamicExceptionSpecificationAST* dyn = ast->asDynamicExceptionSpecification()) {
        // unsigned throw_token = ast->throw_token;
        // unsigned lparen_token = ast->lparen_token;
        // unsigned dot_dot_dot_token = ast->dot_dot_dot_token;
        for (ExpressionListAST* it = dyn->type_id_list; it; it = it->next) {
            /*ExpressionTy value =*/ this->expression(it->value);
        }
    } else if (NoExceptSpecificationAST* no = ast->asNoExceptSpecification()) {
        /*ExpressionTy value =*/ this->expression(no->expression);
    }
    // unsigned rparen_token = ast->rparen_token;
    return type;
}

bool Binder::visit(MemInitializerAST* )
{
    return false;
}

void Binder::memInitializer(MemInitializerAST* ast, Function *fun)
{
    if (! ast)
        return;

    /*const Name *name =*/ this->name(ast->name);

    Scope *previousScope = switchScope(fun);
    this->expression(ast->expression);
    (void) switchScope(previousScope);
}

bool Binder::visit(NestedNameSpecifierAST* )
{
    return false;
}

const Name *Binder::nestedNameSpecifier(NestedNameSpecifierAST* ast)
{
    if (! ast)
        return 0;

    const Name *class_or_namespace_name = this->name(ast->class_or_namespace_name);
    return class_or_namespace_name;
}

bool Binder::visit(ExpressionListParenAST* ast)
{
    // unsigned lparen_token = ast->lparen_token;
    for (ExpressionListAST* it = ast->expression_list; it; it = it->next) {
        /*ExpressionTy value =*/ this->expression(it->value);
    }
    // unsigned rparen_token = ast->rparen_token;

    return false;
}

void Binder::newPlacement(ExpressionListParenAST* ast)
{
    if (! ast)
        return;

    // unsigned lparen_token = ast->lparen_token;
    for (ExpressionListAST* it = ast->expression_list; it; it = it->next) {
        ExpressionTy value = this->expression(it->value);
    }
    // unsigned rparen_token = ast->rparen_token;
}

bool Binder::visit(NewArrayDeclaratorAST* )
{
    return false;
}

FullySpecifiedType Binder::newArrayDeclarator(NewArrayDeclaratorAST* ast, const FullySpecifiedType &init)
{
    FullySpecifiedType type = init;

    if (! ast)
        return type;

    // unsigned lbracket_token = ast->lbracket_token;
    ExpressionTy expression = this->expression(ast->expression);
    // unsigned rbracket_token = ast->rbracket_token;
    return type;
}

bool Binder::visit(NewTypeIdAST* )
{
    return false;
}

FullySpecifiedType Binder::newTypeId(NewTypeIdAST* ast)
{
    FullySpecifiedType type;

    if (! ast)
        return type;


    for (SpecifierListAST* it = ast->type_specifier_list; it; it = it->next) {
        type = this->specifier(it->value, type);
    }
    for (PtrOperatorListAST* it = ast->ptr_operator_list; it; it = it->next) {
        type = this->ptrOperator(it->value, type);
    }
    for (NewArrayDeclaratorListAST* it = ast->new_array_declarator_list; it; it = it->next) {
        type = this->newArrayDeclarator(it->value, type);
    }
    return type;
}

bool Binder::visit(OperatorAST* ast)
{
    return false;
}

OperatorNameId::Kind Binder::cppOperator(OperatorAST* ast)
{
    OperatorNameId::Kind kind = OperatorNameId::InvalidOp;

    if (! ast)
        return kind;

    // unsigned op_token = ast->op_token;
    // unsigned open_token = ast->open_token;
    // unsigned close_token = ast->close_token;

    switch (tokenKind(ast->op_token)) {
    case T_NEW:
        if (ast->open_token)
            kind = OperatorNameId::NewArrayOp;
        else
            kind = OperatorNameId::NewOp;
        break;

    case T_DELETE:
        if (ast->open_token)
            kind = OperatorNameId::DeleteArrayOp;
        else
            kind = OperatorNameId::DeleteOp;
        break;

    case T_PLUS:
        kind = OperatorNameId::PlusOp;
        break;

    case T_MINUS:
        kind = OperatorNameId::MinusOp;
        break;

    case T_STAR:
        kind = OperatorNameId::StarOp;
        break;

    case T_SLASH:
        kind = OperatorNameId::SlashOp;
        break;

    case T_PERCENT:
        kind = OperatorNameId::PercentOp;
        break;

    case T_CARET:
        kind = OperatorNameId::CaretOp;
        break;

    case T_AMPER:
        kind = OperatorNameId::AmpOp;
        break;

    case T_PIPE:
        kind = OperatorNameId::PipeOp;
        break;

    case T_TILDE:
        kind = OperatorNameId::TildeOp;
        break;

    case T_EXCLAIM:
        kind = OperatorNameId::ExclaimOp;
        break;

    case T_EQUAL:
        kind = OperatorNameId::EqualOp;
        break;

    case T_LESS:
        kind = OperatorNameId::LessOp;
        break;

    case T_GREATER:
        kind = OperatorNameId::GreaterOp;
        break;

    case T_PLUS_EQUAL:
        kind = OperatorNameId::PlusEqualOp;
        break;

    case T_MINUS_EQUAL:
        kind = OperatorNameId::MinusEqualOp;
        break;

    case T_STAR_EQUAL:
        kind = OperatorNameId::StarEqualOp;
        break;

    case T_SLASH_EQUAL:
        kind = OperatorNameId::SlashEqualOp;
        break;

    case T_PERCENT_EQUAL:
        kind = OperatorNameId::PercentEqualOp;
        break;

    case T_CARET_EQUAL:
        kind = OperatorNameId::CaretEqualOp;
        break;

    case T_AMPER_EQUAL:
        kind = OperatorNameId::AmpEqualOp;
        break;

    case T_PIPE_EQUAL:
        kind = OperatorNameId::PipeEqualOp;
        break;

    case T_LESS_LESS:
        kind = OperatorNameId::LessLessOp;
        break;

    case T_GREATER_GREATER:
        kind = OperatorNameId::GreaterGreaterOp;
        break;

    case T_LESS_LESS_EQUAL:
        kind = OperatorNameId::LessLessEqualOp;
        break;

    case T_GREATER_GREATER_EQUAL:
        kind = OperatorNameId::GreaterGreaterEqualOp;
        break;

    case T_EQUAL_EQUAL:
        kind = OperatorNameId::EqualEqualOp;
        break;

    case T_EXCLAIM_EQUAL:
        kind = OperatorNameId::ExclaimEqualOp;
        break;

    case T_LESS_EQUAL:
        kind = OperatorNameId::LessEqualOp;
        break;

    case T_GREATER_EQUAL:
        kind = OperatorNameId::GreaterEqualOp;
        break;

    case T_AMPER_AMPER:
        kind = OperatorNameId::AmpAmpOp;
        break;

    case T_PIPE_PIPE:
        kind = OperatorNameId::PipePipeOp;
        break;

    case T_PLUS_PLUS:
        kind = OperatorNameId::PlusPlusOp;
        break;

    case T_MINUS_MINUS:
        kind = OperatorNameId::MinusMinusOp;
        break;

    case T_COMMA:
        kind = OperatorNameId::CommaOp;
        break;

    case T_ARROW_STAR:
        kind = OperatorNameId::ArrowStarOp;
        break;

    case T_ARROW:
        kind = OperatorNameId::ArrowOp;
        break;

    case T_LPAREN:
        kind = OperatorNameId::FunctionCallOp;
        break;

    case T_LBRACKET:
        kind = OperatorNameId::ArrayAccessOp;
        break;

    default:
        kind = OperatorNameId::InvalidOp;
    } // switch

    return kind;
}

bool Binder::visit(ParameterDeclarationClauseAST* )
{
    return false;
}

void Binder::parameterDeclarationClause(ParameterDeclarationClauseAST* ast, unsigned lparen_token, Function *fun)
{
    if (! ast)
        return;

    if (! fun) {
        translationUnit()->warning(lparen_token, "undefined function");
        return;
    }

    Scope *previousScope = switchScope(fun);

    for (ParameterDeclarationListAST* it = ast->parameter_declaration_list; it; it = it->next) {
        this->declaration(it->value);
    }

    if (ast->dot_dot_dot_token)
        fun->setVariadic(true);

    (void) switchScope(previousScope);
}

bool Binder::visit(TranslationUnitAST* )
{
    return false;
}

void Binder::translationUnit(TranslationUnitAST* ast)
{
    if (! ast)
        return;

    for (DeclarationListAST* it = ast->declaration_list; it; it = it->next) {
        this->declaration(it->value);
    }
}

bool Binder::visit(LambdaIntroducerAST* )
{
    return false;
}

void Binder::lambdaIntroducer(LambdaIntroducerAST* ast)
{
    if (! ast)
        return;

    // unsigned lbracket_token = ast->lbracket_token;
    this->lambdaCapture(ast->lambda_capture);
    // unsigned rbracket_token = ast->rbracket_token;
}

bool Binder::visit(LambdaCaptureAST* )
{
    return false;
}

void Binder::lambdaCapture(LambdaCaptureAST* ast)
{
    if (! ast)
        return;

    // unsigned default_capture_token = ast->default_capture_token;
    for (CaptureListAST* it = ast->capture_list; it; it = it->next) {
        this->capture(it->value);
    }
}

bool Binder::visit(CaptureAST* )
{
    return false;
}

void Binder::capture(CaptureAST* ast)
{
    if (! ast)
        return;

    name(ast->identifier);
}

bool Binder::visit(LambdaDeclaratorAST* )
{
    return false;
}

Function *Binder::lambdaDeclarator(LambdaDeclaratorAST* ast)
{
    if (! ast)
        return 0;

    Function *fun = control()->newFunction(0, 0);
    fun->setStartOffset(tokenAt(ast->firstToken()).utf16charsBegin());
    fun->setEndOffset(tokenAt(ast->lastToken() - 1).utf16charsEnd());

    FullySpecifiedType type;
    if (ast->trailing_return_type)
        type = this->trailingReturnType(ast->trailing_return_type, type);
    ast->symbol = fun;

    // unsigned lparen_token = ast->lparen_token;
    this->parameterDeclarationClause(ast->parameter_declaration_clause, ast->lparen_token, fun);
    // unsigned rparen_token = ast->rparen_token;
    for (SpecifierListAST* it = ast->attributes; it; it = it->next) {
        type = this->specifier(it->value, type);
    }
    // unsigned mutable_token = ast->mutable_token;
    type = this->exceptionSpecification(ast->exception_specification, type);

    if (!type.isValid())
        type.setType(control()->voidType());
    fun->setReturnType(type);
    return fun;
}

bool Binder::visit(TrailingReturnTypeAST* )
{
    return false;
}

bool Binder::visit(AmbiguousStatementAST* ambiStmt)
{
    // It's possible that the parser created an AST ambiguity node but was later capable
    // of disambiguating it on its own.
    StatementAST* pickedStmt = nullptr;
    if (ambiStmt->ambiguity->resolution() == SyntaxAmbiguity::Resolution::IsDecl)
        pickedStmt = ambiStmt->declarationStmt;
    else if (ambiStmt->ambiguity->resolution() == SyntaxAmbiguity::Resolution::IsExpr)
        pickedStmt = ambiStmt->expressionStmt;

    if (pickedStmt) {
        this->statement(pickedStmt);
        return false;
    }

    parentAmbiguousStmt_ = ambiStmt;
    this->statement(ambiStmt->expressionStmt);
    this->statement(ambiStmt->declarationStmt);
    parentAmbiguousStmt_ = nullptr;

    // If the ambiguity still remains unresolved, store it for later.
    if (ambiStmt->ambiguity->resolution() == SyntaxAmbiguity::Resolution::Unknown)
        ambiguousStmts_.insert(std::make_pair(scope_, ambiStmt));

    return false;
}

FullySpecifiedType Binder::trailingReturnType(TrailingReturnTypeAST* ast, const FullySpecifiedType &init)
{
    FullySpecifiedType type = init;

    if (! ast)
        return type;

    // unsigned arrow_token = ast->arrow_token;
    for (SpecifierListAST* it = ast->attributes; it; it = it->next) {
        type = this->specifier(it->value, type);
    }
    for (SpecifierListAST* it = ast->type_specifier_list; it; it = it->next) {
        type = this->specifier(it->value, type);
    }
    DeclaratorIdAST* declaratorId = 0;
    type = this->declarator(ast->declarator, type, &declaratorId);
    return type;
}

const StringLiteral *Binder::asStringLiteral(const ExpressionAST* ast)
{
    const unsigned firstToken = ast->firstToken();
    const unsigned lastToken = ast->lastToken();
    std::string buffer;
    for (unsigned index = firstToken; index != lastToken; ++index) {
        const Token &tk = tokenAt(index);
        if (index != firstToken && (tk.whitespace() || tk.newline()))
            buffer += ' ';
        buffer += tk.spell();
    }
    return control()->stringLiteral(buffer.c_str(), unsigned(buffer.size()));
}

bool Binder::visit(CaseStatementAST* ast)
{
    ExpressionTy expression = this->expression(ast->expression);
    this->statement(ast->statement);
    return false;
}

bool Binder::visit(CompoundStatementAST* ast)
{
    Block *block = control()->newBlock(ast->firstToken());
    unsigned startScopeToken = ast->lbrace_token ? ast->lbrace_token : ast->firstToken();
    block->setStartOffset(tokenAt(startScopeToken).utf16charsEnd());
    block->setEndOffset(tokenAt(ast->lastToken() - 1).utf16charsEnd());
    ast->symbol = block;
    scope_->addMember(block);
    Scope *previousScope = switchScope(block);
    for (StatementListAST* it = ast->statement_list; it; it = it->next) {
        this->statement(it->value);
    }
    (void) switchScope(previousScope);
    return false;
}

bool Binder::visit(DeclarationStatementAST* ast)
{
    this->declaration(ast->declaration);
    return false;
}

bool Binder::visit(DoStatementAST* ast)
{
    this->statement(ast->statement);
    ExpressionTy expression = this->expression(ast->expression);
    return false;
}

bool Binder::visit(ExpressionOrDeclarationStatementAST* ast)
{
    this->statement(ast->expression);
    this->statement(ast->declaration);
    return false;
}

bool Binder::visit(ExpressionStatementAST* ast)
{
    ExpressionTy expression = this->expression(ast->expression);
    // unsigned semicolon_token = ast->semicolon_token;
    return false;
}

bool Binder::visit(ForeachStatementAST* ast)
{
    Block *block = control()->newBlock(ast->firstToken());
    const unsigned startScopeToken = ast->lparen_token ? ast->lparen_token : ast->firstToken();
    block->setStartOffset(tokenAt(startScopeToken).utf16charsEnd());
    block->setEndOffset(tokenAt(ast->lastToken()).utf16charsBegin());
    scope_->addMember(block);
    ast->symbol = block;

    Scope *previousScope = switchScope(block);

    FullySpecifiedType type;
    for (SpecifierListAST* it = ast->type_specifier_list; it; it = it->next) {
        type = this->specifier(it->value, type);
    }
    DeclaratorIdAST* declaratorId = 0;
    type = this->declarator(ast->declarator, type, &declaratorId);
    const StringLiteral *initializer = 0;
    if (type.isAuto() && translationUnit()->dialect().cpp11) {
        ExpressionTy exprType = this->expression(ast->expression);

        ArrayType* arrayType = 0;
        arrayType = exprType->asArrayType();

        if (arrayType != 0)
            type = arrayType->elementType();
        else if (ast->expression != 0) {
            const StringLiteral *sl = asStringLiteral(ast->expression);
            const std::string buff = std::string("*") + sl->chars() + ".begin()";
            initializer = control()->stringLiteral(buff.c_str(), unsigned(buff.size()));
        }
    }

    if (declaratorId && declaratorId->name) {
        unsigned sourceLocation = location(declaratorId->name, ast->firstToken());
        Declaration *decl = control()->newDeclaration(sourceLocation, declaratorId->name->name);
        decl->setType(type);
        decl->setInitializer(initializer);
        block->addMember(decl);
    }

    /*ExpressionTy initializer =*/ this->expression(ast->initializer);
    /*ExpressionTy expression =*/ this->expression(ast->expression);
    this->statement(ast->statement);
    (void) switchScope(previousScope);
    return false;
}

bool Binder::visit(RangeBasedForStatementAST* ast)
{
    Block *block = control()->newBlock(ast->firstToken());
    const unsigned startScopeToken = ast->lparen_token ? ast->lparen_token : ast->firstToken();
    block->setStartOffset(tokenAt(startScopeToken).utf16charsEnd());
    block->setEndOffset(tokenAt(ast->lastToken()).utf16charsBegin());
    scope_->addMember(block);
    ast->symbol = block;

    Scope *previousScope = switchScope(block);

    FullySpecifiedType type;
    for (SpecifierListAST* it = ast->type_specifier_list; it; it = it->next) {
        type = this->specifier(it->value, type);
    }
    DeclaratorIdAST* declaratorId = 0;
    type = this->declarator(ast->declarator, type, &declaratorId);
    const StringLiteral *initializer = 0;
    if (type.isAuto() && translationUnit()->dialect().cpp11) {
        ExpressionTy exprType = this->expression(ast->expression);

        ArrayType* arrayType = 0;
        arrayType = exprType->asArrayType();

        if (arrayType != 0)
            type = arrayType->elementType();
        else if (ast->expression != 0) {
            const StringLiteral *sl = asStringLiteral(ast->expression);
            const std::string buff = std::string("*") + sl->chars() + ".begin()";
            initializer = control()->stringLiteral(buff.c_str(), unsigned(buff.size()));
        }
    }

    if (declaratorId && declaratorId->name) {
        unsigned sourceLocation = location(declaratorId->name, ast->firstToken());
        Declaration *decl = control()->newDeclaration(sourceLocation, declaratorId->name->name);
        decl->setType(type);
        decl->setInitializer(initializer);
        block->addMember(decl);
    }

    /*ExpressionTy expression =*/ this->expression(ast->expression);
    this->statement(ast->statement);
    (void) switchScope(previousScope);
    return false;
}

bool Binder::visit(ForStatementAST* ast)
{
    Block *block = control()->newBlock(ast->firstToken());
    const unsigned startScopeToken = ast->lparen_token ? ast->lparen_token : ast->firstToken();
    block->setStartOffset(tokenAt(startScopeToken).utf16charsEnd());
    block->setEndOffset(tokenAt(ast->lastToken()).utf16charsBegin());
    scope_->addMember(block);
    ast->symbol = block;

    Scope *previousScope = switchScope(block);
    this->statement(ast->initializer);
    /*ExpressionTy condition =*/ this->expression(ast->condition);
    // unsigned semicolon_token = ast->semicolon_token;
    /*ExpressionTy expression =*/ this->expression(ast->expression);
    // unsigned rparen_token = ast->rparen_token;
    this->statement(ast->statement);
    (void) switchScope(previousScope);
    return false;
}

bool Binder::visit(IfStatementAST* ast)
{
    Block *block = control()->newBlock(ast->firstToken());
    const unsigned startScopeToken = ast->lparen_token ? ast->lparen_token : ast->firstToken();
    block->setStartOffset(tokenAt(startScopeToken).utf16charsEnd());
    block->setEndOffset(tokenAt(ast->lastToken()).utf16charsBegin());
    scope_->addMember(block);
    ast->symbol = block;

    Scope *previousScope = switchScope(block);
    /*ExpressionTy condition =*/ this->expression(ast->condition);
    this->statement(ast->statement);
    this->statement(ast->else_statement);
    (void) switchScope(previousScope);
    return false;
}

bool Binder::visit(LabeledStatementAST* ast)
{
    // unsigned label_token = ast->label_token;
    // unsigned colon_token = ast->colon_token;
    this->statement(ast->statement);
    return false;
}

bool Binder::visit(BreakStatementAST* ast)
{
    (void) ast;
    // unsigned break_token = ast->break_token;
    // unsigned semicolon_token = ast->semicolon_token;
    return false;
}

bool Binder::visit(ContinueStatementAST* ast)
{
    (void) ast;
    // unsigned continue_token = ast->continue_token;
    // unsigned semicolon_token = ast->semicolon_token;
    return false;
}

bool Binder::visit(GotoStatementAST* ast)
{
    (void) ast;
    // unsigned goto_token = ast->goto_token;
    // unsigned identifier_token = ast->identifier_token;
    // unsigned semicolon_token = ast->semicolon_token;
    return false;
}

bool Binder::visit(ReturnStatementAST* ast)
{
    ExpressionTy expression = this->expression(ast->expression);
    return false;
}

bool Binder::visit(SwitchStatementAST* ast)
{
    Block *block = control()->newBlock(ast->firstToken());
    const unsigned startScopeToken = ast->lparen_token ? ast->lparen_token : ast->firstToken();
    block->setStartOffset(tokenAt(startScopeToken).utf16charsEnd());
    block->setEndOffset(tokenAt(ast->lastToken()).utf16charsBegin());
    scope_->addMember(block);
    ast->symbol = block;

    Scope *previousScope = switchScope(block);
    /*ExpressionTy condition =*/ this->expression(ast->condition);
    this->statement(ast->statement);
    (void) switchScope(previousScope);
    return false;
}

bool Binder::visit(TryBlockStatementAST* ast)
{
    // unsigned try_token = ast->try_token;
    this->statement(ast->statement);
    for (CatchClauseListAST* it = ast->catch_clause_list; it; it = it->next) {
        this->statement(it->value);
    }
    return false;
}

bool Binder::visit(CatchClauseAST* ast)
{
    Block *block = control()->newBlock(ast->firstToken());
    const unsigned startScopeToken = ast->lparen_token ? ast->lparen_token : ast->firstToken();
    block->setStartOffset(tokenAt(startScopeToken).utf16charsEnd());
    block->setEndOffset(tokenAt(ast->lastToken()).utf16charsBegin());
    scope_->addMember(block);
    ast->symbol = block;

    Scope *previousScope = switchScope(block);
    this->declaration(ast->exception_declaration);
    // unsigned rparen_token = ast->rparen_token;
    this->statement(ast->statement);
    (void) switchScope(previousScope);
    return false;
}

bool Binder::visit(WhileStatementAST* ast)
{
    Block *block = control()->newBlock(ast->firstToken());
    const unsigned startScopeToken = ast->lparen_token ? ast->lparen_token : ast->firstToken();
    block->setStartOffset(tokenAt(startScopeToken).utf16charsEnd());
    block->setEndOffset(tokenAt(ast->lastToken()).utf16charsBegin());
    scope_->addMember(block);
    ast->symbol = block;

    Scope *previousScope = switchScope(block);
    /*ExpressionTy condition =*/ this->expression(ast->condition);
    this->statement(ast->statement);
    (void) switchScope(previousScope);
    return false;
}

bool Binder::visit(IdExpressionAST* ast)
{
    const Name *name = this->name(ast->name);

    // If we're within an ambiguous statement, we can't tell this identifier is really
    // an expression, so it isn't possible to use for disambiguating ambiguities.
    if (withinAmbiguousStmt())
        return false;

    if (name->asNameId())
        maybeResolveAmbiguity(name, &SyntaxAmbiguity::lhs, SyntaxAmbiguity::Resolution::IsExpr);

    return false;
}

bool Binder::visit(CompoundExpressionAST* ast)
{
    // unsigned lparen_token = ast->lparen_token;
    this->statement(ast->statement);
    // unsigned rparen_token = ast->rparen_token;
    return false;
}

bool Binder::visit(CompoundLiteralAST* ast)
{
    // unsigned lparen_token = ast->lparen_token;
    ExpressionTy type_id = this->expression(ast->type_id);
    // unsigned rparen_token = ast->rparen_token;
    ExpressionTy initializer = this->expression(ast->initializer);
    return false;
}

bool Binder::visit(BinaryExpressionAST* ast)
{
    this->expression(ast->left_expression);
    this->expression(ast->right_expression);

    if (!withinAmbiguousStmt()
            || parentAmbiguousStmt_->ambiguity->resolution() != SyntaxAmbiguity::Resolution::Unknown) {
        return false;
    }

    auto resolve = [this] (AST* astSide, void (SyntaxAmbiguity::*set) (const Name*)) {
        if (astSide->asIdExpression()) {
            const Name* name = astSide->asIdExpression()->name->name;
            if (lookupValueSymbol(name, scope_)) {
                parentAmbiguousStmt_->ambiguity->applyResolution(SyntaxAmbiguity::Resolution::IsExpr);
                return true;
            }
            (parentAmbiguousStmt_->ambiguity.get()->*(set))(name);
        }
        return false;
    };

    // Try to resolve ambiguity through symbol lookup.
    if (resolve(ast->left_expression, &SyntaxAmbiguity::setLhs)
            || resolve(ast->right_expression, &SyntaxAmbiguity::setRhs))
        return false;

    // Undeclared names won't produce a symbol, but maybe this name has been seen as a
    // type specifier.
    if (ast->left_expression->asIdExpression()) {
        const Name* name = ast->left_expression->asIdExpression()->name->name;
        auto it = typeNames_.find(scope_);
        if (it != typeNames_.end() && (it->second.find(name) != it->second.end()))
            parentAmbiguousStmt_->ambiguity->applyResolution(SyntaxAmbiguity::Resolution::IsDecl);
    }

    return false;
}

bool Binder::visit(CastExpressionAST* ast)
{
    // unsigned lparen_token = ast->lparen_token;
    ast->expression_type = this->expression(ast->type_id);
    // unsigned rparen_token = ast->rparen_token;
    ExpressionTy expression = this->expression(ast->expression);
    return false;
}

bool Binder::visit(ConditionAST* ast)
{
    FullySpecifiedType type;
    for (SpecifierListAST* it = ast->type_specifier_list; it; it = it->next) {
        type = this->specifier(it->value, type);
    }
    DeclaratorIdAST* declaratorId = 0;
    type = this->declarator(ast->declarator, type, &declaratorId);

    if (declaratorId && declaratorId->name) {
        unsigned sourceLocation = location(declaratorId->name, ast->firstToken());
        Declaration *decl = control()->newDeclaration(sourceLocation, declaratorId->name->name);
        decl->setType(type);

        if (type.isAuto() && translationUnit()->dialect().cpp11)
            decl->setInitializer(asStringLiteral(ast->declarator->initializer));

        scope_->addMember(decl);
    }

    return false;
}

bool Binder::visit(ConditionalExpressionAST* ast)
{
    ExpressionTy condition = this->expression(ast->condition);
    // unsigned question_token = ast->question_token;
    ExpressionTy left_expression = this->expression(ast->left_expression);
    // unsigned colon_token = ast->colon_token;
    ExpressionTy right_expression = this->expression(ast->right_expression);
    return false;
}

bool Binder::visit(CppCastExpressionAST* ast)
{
    // unsigned cast_token = ast->cast_token;
    // unsigned less_token = ast->less_token;
    ExpressionTy type_id = this->expression(ast->type_id);
    // unsigned greater_token = ast->greater_token;
    // unsigned lparen_token = ast->lparen_token;
    ExpressionTy expression = this->expression(ast->expression);
    // unsigned rparen_token = ast->rparen_token;
    return false;
}

bool Binder::visit(DeleteExpressionAST* ast)
{
    // unsigned scope_token = ast->scope_token;
    // unsigned delete_token = ast->delete_token;
    // unsigned lbracket_token = ast->lbracket_token;
    // unsigned rbracket_token = ast->rbracket_token;
    ExpressionTy expression = this->expression(ast->expression);
    return false;
}

bool Binder::visit(ArrayInitializerAST* ast)
{
    // unsigned lbrace_token = ast->lbrace_token;
    for (ExpressionListAST* it = ast->expression_list; it; it = it->next) {
        ExpressionTy value = this->expression(it->value);
    }
    // unsigned rbrace_token = ast->rbrace_token;
    return false;
}

bool Binder::visit(NewExpressionAST* ast)
{
    // unsigned scope_token = ast->scope_token;
    // unsigned new_token = ast->new_token;
    this->newPlacement(ast->new_placement);
    // unsigned lparen_token = ast->lparen_token;
    ExpressionTy type_id = this->expression(ast->type_id);
    // unsigned rparen_token = ast->rparen_token;
    this->newTypeId(ast->new_type_id);
    this->expression(ast->new_initializer);
    return false;
}

bool Binder::visit(TypeidExpressionAST* ast)
{
    // unsigned typeid_token = ast->typeid_token;
    // unsigned lparen_token = ast->lparen_token;
    ExpressionTy expression = this->expression(ast->expression);
    // unsigned rparen_token = ast->rparen_token;
    return false;
}

bool Binder::visit(TypenameCallExpressionAST* ast)
{
    // unsigned typename_token = ast->typename_token;
    /*const Name *name =*/ this->name(ast->name);
    this->expression(ast->expression);
    return false;
}

bool Binder::visit(TypeConstructorCallAST* ast)
{
    FullySpecifiedType type;
    for (SpecifierListAST* it = ast->type_specifier_list; it; it = it->next) {
        type = this->specifier(it->value, type);
    }
    this->expression(ast->expression);
    return false;
}

bool Binder::visit(SizeofExpressionAST* ast)
{
    // unsigned sizeof_token = ast->sizeof_token;
    // unsigned dot_dot_dot_token = ast->dot_dot_dot_token;
    // unsigned lparen_token = ast->lparen_token;
    ast->expression_type = this->expression(ast->expression);
    // unsigned rparen_token = ast->rparen_token;
    return false;
}

bool Binder::visit(PointerLiteralAST* ast)
{
    (void) ast;
    // unsigned literal_token = ast->literal_token;
    return false;
}

bool Binder::visit(NumericLiteralAST* ast)
{
    (void) ast;
    // unsigned literal_token = ast->literal_token;
    return false;
}

bool Binder::visit(BoolLiteralAST* ast)
{
    (void) ast;
    // unsigned literal_token = ast->literal_token;
    return false;
}

bool Binder::visit(ThisExpressionAST* ast)
{
    (void) ast;
    // unsigned this_token = ast->this_token;
    return false;
}

bool Binder::visit(NestedExpressionAST* ast)
{
    // unsigned lparen_token = ast->lparen_token;
    ExpressionTy expression = this->expression(ast->expression);
    // unsigned rparen_token = ast->rparen_token;
    return false;
}

bool Binder::visit(StringLiteralAST* ast)
{
    // unsigned literal_token = ast->literal_token;
    ExpressionTy next = this->expression(ast->next);
    return false;
}

bool Binder::visit(ThrowExpressionAST* ast)
{
    // unsigned throw_token = ast->throw_token;
    ExpressionTy expression = this->expression(ast->expression);
    return false;
}

bool Binder::visit(TypeIdAST* ast)
{
    FullySpecifiedType type;
    for (SpecifierListAST* it = ast->type_specifier_list; it; it = it->next) {
        type = this->specifier(it->value, type);
    }
    DeclaratorIdAST* declaratorId = 0;
    type = this->declarator(ast->declarator, type, &declaratorId);
    exprTy_ = type;
    return false;
}

bool Binder::visit(UnaryExpressionAST* ast)
{
    // unsigned unary_op_token = ast->unary_op_token;
    this->expression(ast->expression);

    if (ast->expression
            && ast->expression->asIdExpression()
            && tokenKind(ast->unary_op_token) == T_STAR) {
        maybeResolveAmbiguity(ast->expression->asIdExpression()->name->name,
                              &SyntaxAmbiguity::rhs,
                              SyntaxAmbiguity::Resolution::IsDecl);
    }

    return false;
}

bool Binder::visit(LambdaExpressionAST* ast)
{
    this->lambdaIntroducer(ast->lambda_introducer);
    if (Function *function = this->lambdaDeclarator(ast->lambda_declarator)) {
        scope_->addMember(function);
        Scope *previousScope = switchScope(function);
        this->statement(ast->statement);
        (void) switchScope(previousScope);
    } else {
        this->statement(ast->statement);
    }

    return false;
}

bool Binder::visit(BracedInitializerAST* ast)
{
    // unsigned lbrace_token = ast->lbrace_token;
    for (ExpressionListAST* it = ast->expression_list; it; it = it->next) {
        /*ExpressionTy value =*/ this->expression(it->value);
    }
    // unsigned comma_token = ast->comma_token;
    // unsigned rbrace_token = ast->rbrace_token;
    return false;
}

bool Binder::visit(SimpleDeclarationAST* ast)
{
    int methodKey = methodKey_;

    FullySpecifiedType type;
    for (SpecifierListAST* it = ast->decl_specifier_list; it; it = it->next) {
        type = this->specifier(it->value, type);
    }

    // Make undefined unsigned type an int type.
    if (type.isUnsigned() && type->isUndefinedType())
        type.setType(control()->integerType(IntegerType::Int));

    List<Symbol *> **symbolTail = &ast->symbols;

    if (! ast->declarator_list) {
        ElaboratedTypeSpecifierAST* elabTypeSpec = 0;
        for (SpecifierListAST* it = ast->decl_specifier_list; ! elabTypeSpec && it; it = it->next)
            elabTypeSpec = it->value->asElaboratedTypeSpecifier();

        if (elabTypeSpec && tokenKind(elabTypeSpec->name->tag_token) != T_TYPENAME) {
            unsigned sourceLocation = elabTypeSpec->firstToken();
            const Name *name = 0;
            if (elabTypeSpec->name) {
                sourceLocation = location(elabTypeSpec->name, sourceLocation);
                name = elabTypeSpec->name->name;
            }

            ensureValidClassName(&name, sourceLocation);

            ForwardClassDeclaration *decl = control()->newForwardClassDeclaration(sourceLocation, name);
            setDeclSpecifiers(decl, type);
            scope_->addMember(decl);

            *symbolTail = new (translationUnit()->memoryPool()) List<Symbol *>(decl);
            symbolTail = &(*symbolTail)->next;
        }
    }

    for (DeclaratorListAST* it = ast->declarator_list; it; it = it->next) {
        DeclaratorIdAST* declaratorId = 0;
        FullySpecifiedType declTy = this->declarator(it->value, type, &declaratorId);

        const Name *declName = 0;
        unsigned sourceLocation = location(it->value, ast->firstToken());
        if (declaratorId && declaratorId->name)
            declName = declaratorId->name->name;

        Declaration *decl = control()->newDeclaration(sourceLocation, declName);
        decl->setType(declTy);
        setDeclSpecifiers(decl, type);

        // Keep track of named types not accompanined by a pointer declarator, so we can
        // use them for AST disambiguation.
        if (type->isNamedType() && !declTy->isPointerType())
            typeNames_[scope_].insert(type->asNamedType()->name());

        if (withinAmbiguousStmt()
                && parentAmbiguousStmt_->ambiguity->resolution() == SyntaxAmbiguity::Resolution::Unknown
                && type->isNamedType()) {
            if (lookupTypeSymbol(type->asNamedType()->name(), scope_)) {
                parentAmbiguousStmt_->ambiguity->applyResolution(SyntaxAmbiguity::Resolution::IsDecl);
            } else {
                parentAmbiguousStmt_->ambiguity->setLhs(type->asNamedType()->name());

                // It might be the case this is not a declaration. But because the symbol
                // is always created nevertheless, we keep track such "declarations" so
                // it's possible to invalidate them later.
                parentAmbiguousStmt_->suspiciousDecls.push_back(decl);
            }
        } else if (declTy->asNamedType()) {
            maybeResolveAmbiguity(declTy->asNamedType()->name(),
                                  &SyntaxAmbiguity::lhs,
                                  SyntaxAmbiguity::Resolution::IsDecl);
        }

        if (Function *fun = decl->type()->asFunctionType()) {
            fun->setEnclosingScope(scope_);
            fun->setSourceLocation(sourceLocation, translationUnit());

            setDeclSpecifiers(fun, type);
            if (declaratorId && declaratorId->name)
                fun->setName(declaratorId->name->name); // update the function name
        }
        else if (declTy.isAuto()) {
            const ExpressionAST* initializer = it->value->initializer;
            if (!initializer && declaratorId)
                translationUnit()->error(location(declaratorId->name, ast->firstToken()), "auto-initialized variable must have an initializer");
            else if (initializer)
                decl->setInitializer(asStringLiteral(initializer));
        }

        if (scope_->isClass()) {
            decl->setVisibility(visibility_);

            if (Function *funTy = decl->type()->asFunctionType()) {
                funTy->setMethodKey(methodKey);

                bool pureVirtualInit = it->value->equal_token
                        && it->value->initializer
                        && it->value->initializer->asNumericLiteral();
                if (funTy->isVirtual() && pureVirtualInit)
                    funTy->setPureVirtual(true);
            }
        }

        scope_->addMember(decl);

        *symbolTail = new (translationUnit()->memoryPool()) List<Symbol *>(decl);
        symbolTail = &(*symbolTail)->next;
    }

    return false;
}

bool Binder::visit(EmptyDeclarationAST* ast)
{
    (void) ast;
    unsigned semicolon_token = ast->semicolon_token;
    if (scope_ && (scope_->isClass() || scope_->isNamespace())) {
        const Token &tk = tokenAt(semicolon_token);

        if (! tk.generated())
            translationUnit()->warning(semicolon_token, "extra `;'");
    }
    return false;
}

bool Binder::visit(AccessDeclarationAST* ast)
{
    const int accessSpecifier = tokenKind(ast->access_specifier_token);
    visibility_ = visibilityForAccessSpecifier(accessSpecifier);
    methodKey_ = Function::NormalMethod;

    return false;
}

bool Binder::visit(AliasDeclarationAST* ast)
{
    if (!ast->name)
        return false;

    const Name *name = this->name(ast->name);

    FullySpecifiedType ty = expression(ast->typeId);
    ty.setTypedef(true);

    Declaration *decl = control()->newDeclaration(ast->name->firstToken(), name);
    decl->setType(ty);
    decl->setStorage(Symbol::Typedef);
    ast->symbol = decl;
    if (scope_->isClass())
        decl->setVisibility(visibility_);
    scope_->addMember(decl);

    return false;
}

bool Binder::visit(AsmDefinitionAST* ast)
{
    (void) ast;
    // unsigned asm_token = ast->asm_token;
    // unsigned volatile_token = ast->volatile_token;
    // unsigned lparen_token = ast->lparen_token;
    // unsigned rparen_token = ast->rparen_token;
    // unsigned semicolon_token = ast->semicolon_token;
    return false;
}

bool Binder::visit(ExceptionDeclarationAST* ast)
{
    FullySpecifiedType type;
    for (SpecifierListAST* it = ast->type_specifier_list; it; it = it->next) {
        type = this->specifier(it->value, type);
    }
    DeclaratorIdAST* declaratorId = 0;
    type = this->declarator(ast->declarator, type, &declaratorId);

    const Name *argName = 0;
    if (declaratorId && declaratorId->name)
        argName = declaratorId->name->name;
    Argument *arg = control()->newArgument(location(declaratorId, ast->firstToken()), argName);
    arg->setType(type);
    scope_->addMember(arg);

    // unsigned dot_dot_dot_token = ast->dot_dot_dot_token;

    return false;
}

bool Binder::visit(FunctionDefinitionAST* ast)
{
    int methodKey = methodKey_;

    FullySpecifiedType declSpecifiers;
    for (SpecifierListAST* it = ast->decl_specifier_list; it; it = it->next) {
        declSpecifiers = this->specifier(it->value, declSpecifiers);
    }
    DeclaratorIdAST* declaratorId = 0;
    FullySpecifiedType type = this->declarator(ast->declarator, declSpecifiers.strippedType(), &declaratorId);

    Function *fun = type->asFunctionType();
    ast->symbol = fun;

    if (fun) {
        setDeclSpecifiers(fun, declSpecifiers);
        fun->setEndOffset(tokenAt(ast->lastToken() - 1).utf16charsEnd());

        if (scope_->isClass()) {
            fun->setVisibility(visibility_);
            fun->setMethodKey(methodKey);
        }

        if (declaratorId && declaratorId->name) {
            fun->setSourceLocation(location(declaratorId, ast->firstToken()), translationUnit());
            fun->setName(declaratorId->name->name);
        }

        scope_->addMember(fun);
    } else
        translationUnit()->warning(ast->firstToken(), "expected a function declarator");

    this->ctorInitializer(ast->ctor_initializer, fun);

    if (fun && ! skipFunctionBodies_ && ast->function_body) {
        Scope *previousScope = switchScope(fun);
        this->statement(ast->function_body);
        (void) switchScope(previousScope);
    }

    return false;
}

bool Binder::visit(GenericsDeclarationAST* ast)
{
    this->declaration(ast->declaration);

    if (ast->declaration
            && ast->declaration->asFunctionDefinition()
            && ast->declaration->asFunctionDefinition()->symbol) {
        Function* func = ast->declaration->asFunctionDefinition()->symbol;
        func->setIsGeneric(true);
    }

    return false;
}

bool Binder::visit(LinkageBodyAST* ast)
{
    // unsigned lbrace_token = ast->lbrace_token;
    for (DeclarationListAST* it = ast->declaration_list; it; it = it->next) {
        this->declaration(it->value);
    }
    // unsigned rbrace_token = ast->rbrace_token;
    return false;
}

bool Binder::visit(LinkageSpecificationAST* ast)
{
    // unsigned extern_token = ast->extern_token;
    // unsigned extern_type_token = ast->extern_type_token;
    this->declaration(ast->declaration);
    return false;
}

bool Binder::visit(NamespaceAST* ast)
{
    // unsigned namespace_token = ast->namespace_token;
    // unsigned identifier_token = ast->identifier_token;
    FullySpecifiedType type;
    for (SpecifierListAST* it = ast->attribute_list; it; it = it->next) {
        type = this->specifier(it->value, type);
    }

    unsigned sourceLocation = ast->firstToken();
    const Name *namespaceName = 0;
    if (ast->identifier_token) {
        sourceLocation = ast->identifier_token;
        namespaceName = identifier(ast->identifier_token);
    }

    Namespace *ns = control()->newNamespace(sourceLocation, namespaceName);
    ns->setStartOffset(tokenAt(sourceLocation).utf16charsEnd()); // the scope starts after the namespace or the identifier token.
    ns->setEndOffset(tokenAt(ast->lastToken() - 1).utf16charsEnd());
    ns->setInline(ast->inline_token != 0);
    ast->symbol = ns;
    scope_->addMember(ns);

    Scope *previousScope = switchScope(ns);
    this->declaration(ast->linkage_body);
    (void) switchScope(previousScope);
    return false;
}

bool Binder::visit(NamespaceAliasDefinitionAST* ast)
{
    unsigned sourceLocation = ast->firstToken();
    const Name *name = 0;
    if (ast->namespace_name_token) {
        sourceLocation = ast->namespace_name_token;
        name = identifier(ast->namespace_name_token);
    }

    NamespaceAlias *namespaceAlias = control()->newNamespaceAlias(sourceLocation, name);
    namespaceAlias->setNamespaceName(this->name(ast->name));
    scope_->addMember(namespaceAlias);
    return false;
}

bool Binder::visit(ParameterDeclarationAST* ast)
{
    FullySpecifiedType type;
    for (SpecifierListAST* it = ast->type_specifier_list; it; it = it->next) {
        type = this->specifier(it->value, type);
    }
    DeclaratorIdAST* declaratorId = 0;
    type = this->declarator(ast->declarator, type, &declaratorId);
    // unsigned equal_token = ast->equal_token;
    ExpressionTy expression = this->expression(ast->expression);

    const Name *argName = 0;
    if (declaratorId && declaratorId->name)
        argName = declaratorId->name->name;

    Argument *arg = control()->newArgument(location(declaratorId, ast->firstToken()), argName);
    arg->setType(type);

    if (ast->expression)
        arg->setInitializer(asStringLiteral(ast->expression));

    scope_->addMember(arg);

    ast->symbol = arg;
    return false;
}

bool Binder::visit(TemplateDeclarationAST* ast)
{
    Template *templ = control()->newTemplate(ast->firstToken(), 0);
    templ->setStartOffset(tokenAt(ast->firstToken()).utf16charsBegin());
    templ->setEndOffset(tokenAt(ast->lastToken() - 1).utf16charsEnd());
    ast->symbol = templ;
    Scope *previousScope = switchScope(templ);

    for (DeclarationListAST* it = ast->template_parameter_list; it; it = it->next) {
        this->declaration(it->value);
    }
    // unsigned greater_token = ast->greater_token;
    this->declaration(ast->declaration);
    (void) switchScope(previousScope);

    if (Symbol *decl = templ->declaration()) {
        templ->setSourceLocation(decl->sourceLocation(), translationUnit());
        templ->setName(decl->name());
    }

    scope_->addMember(templ);
    return false;
}

bool Binder::visit(TypenameTypeParameterAST* ast)
{
    unsigned sourceLocation = location(ast->name, ast->firstToken());
    // unsigned classkey_token = ast->classkey_token;
    // unsigned dot_dot_dot_token = ast->dot_dot_dot_token;
    const Name *name = this->name(ast->name);
    ExpressionTy type_id = this->expression(ast->type_id);

    TypenameArgument *arg = control()->newTypenameArgument(sourceLocation, name);
    arg->setType(type_id);
    ast->symbol = arg;
    scope_->addMember(arg);
    return false;
}

bool Binder::visit(TemplateTypeParameterAST* ast)
{
    unsigned sourceLocation = location(ast->name, ast->firstToken());

    // unsigned template_token = ast->template_token;
    // unsigned less_token = ast->less_token;
    // ### process the template prototype
#if 0
    for (DeclarationListAST* it = ast->template_parameter_list; it; it = it->next) {
        this->declaration(it->value);
    }
#endif
    // unsigned greater_token = ast->greater_token;
    // unsigned class_token = ast->class_token;
    // unsigned dot_dot_dot_token = ast->dot_dot_dot_token;

    const Name *name = this->name(ast->name);
    ExpressionTy type_id = this->expression(ast->type_id);

    // ### introduce TemplateTypeArgument
    TypenameArgument *arg = control()->newTypenameArgument(sourceLocation, name);
    arg->setType(type_id);
    ast->symbol = arg;
    scope_->addMember(arg);

    return false;
}

bool Binder::visit(UsingAST* ast)
{
    unsigned sourceLocation = location(ast->name, ast->firstToken());
    const Name *name = this->name(ast->name);

    UsingDeclaration *symbol = control()->newUsingDeclaration(sourceLocation, name);
    ast->symbol = symbol;
    scope_->addMember(symbol);
    return false;
}

bool Binder::visit(UsingDirectiveAST* ast)
{
    unsigned sourceLocation = location(ast->name, ast->firstToken());
    const Name *name = this->name(ast->name);
    UsingNamespaceDirective *symbol = control()->newUsingNamespaceDirective(sourceLocation, name);
    ast->symbol = symbol;
    scope_->addMember(symbol);
    return false;
}

bool Binder::visit(QualifiedNameAST* ast)
{
    for (NestedNameSpecifierListAST* it = ast->nested_name_specifier_list; it; it = it->next) {
        const Name *class_or_namespace_name = this->nestedNameSpecifier(it->value);
        if (name_ || ast->global_scope_token)
            name_ = control()->qualifiedNameId(name_, class_or_namespace_name);
        else
            name_ = class_or_namespace_name;
    }

    const Name *unqualified_name = this->name(ast->unqualified_name);
    if (name_ || ast->global_scope_token)
        name_ = control()->qualifiedNameId(name_, unqualified_name);
    else
        name_ = unqualified_name;

    ast->name = name_;
    return false;
}

bool Binder::visit(OperatorFunctionIdAST* ast)
{
    const OperatorNameId::Kind op = this->cppOperator(ast->op);
    ast->name = name_ = control()->operatorNameId(op);
    return false;
}

bool Binder::visit(ConversionFunctionIdAST* ast)
{
    FullySpecifiedType type;
    for (SpecifierListAST* it = ast->type_specifier_list; it; it = it->next) {
        type = this->specifier(it->value, type);
    }
    for (PtrOperatorListAST* it = ast->ptr_operator_list; it; it = it->next) {
        type = this->ptrOperator(it->value, type);
    }
    ast->name = name_ = control()->conversionNameId(type);
    return false;
}

bool Binder::visit(EmptyNameAST* ast)
{
    ast->name = name_ = control()->emptyName();
    return false;
}

bool Binder::visit(SimpleNameAST* ast)
{
    const Identifier *id = identifier(ast->identifier_token);
    name_ = id;
    ast->name = name_;
    return false;
}

bool Binder::visit(TaggedNameAST* ast)
{
    unsigned classKey = tokenKind(ast->tag_token);
    TaggedNameId::Tag tag = TaggedNameId::Struct;
    if (classKey == T_UNION)
        tag = TaggedNameId::Union;
    else if (classKey == T_ENUM)
        tag = TaggedNameId::Enum;

    const Name *core_name = this->name(ast->core_name);
    name_ = control()->taggedNameId(tag, core_name);
    ast->name = name_;

    return false;
}

bool Binder::visit(DestructorNameAST* ast)
{
    name_ = control()->destructorNameId(name(ast->unqualified_name));
    ast->name = name_;
    return false;
}

bool Binder::visit(TemplateIdAST* ast)
{
    // collect the template parameters
    std::vector<FullySpecifiedType> templateArguments;
    for (ExpressionListAST* it = ast->template_argument_list; it; it = it->next) {
        ExpressionTy value = this->expression(it->value);
        templateArguments.push_back(value);
    }

    const Identifier *id = identifier(ast->identifier_token);
    const int tokenKindBeforeIdentifier(translationUnit()->tokenKind(ast->identifier_token - 1));
    const bool isSpecialization = (tokenKindBeforeIdentifier == T_CLASS ||
                                   tokenKindBeforeIdentifier == T_STRUCT);
    if (templateArguments.empty())
        name_ = control()->templateNameId(id, isSpecialization);
    else
        name_ = control()->templateNameId(id, isSpecialization, &templateArguments[0],
                unsigned(templateArguments.size()));

    ast->name = name_;
    return false;
}


// SpecifierAST
bool Binder::visit(SimpleSpecifierAST* ast)
{
    switch (tokenKind(ast->specifier_token)) {
        case T_IDENTIFIER: {
                // TODO: Check for contextual keywords override and final and set them.
                // const Identifier *id = tokenAt(ast->specifier_token).identifier;
            }
            break;
        case T_CONST:
            if (type_.isConst())
                translationUnit()->error(ast->specifier_token, "duplicate `%s'", spell(ast->specifier_token));
            type_.setConst(true);
            break;

        case T_VOLATILE:
            if (type_.isVolatile())
                translationUnit()->error(ast->specifier_token, "duplicate `%s'", spell(ast->specifier_token));
            type_.setVolatile(true);
            break;

        case T_FRIEND:
            if (type_.isFriend())
                translationUnit()->error(ast->specifier_token, "duplicate `%s'", spell(ast->specifier_token));
            type_.setFriend(true);
            break;

        case T_AUTO:
            if (!translationUnit()->dialect().cpp11) {
                if (type_.isAuto())
                    translationUnit()->error(ast->specifier_token, "duplicate `%s'", spell(ast->specifier_token));
            }
            type_.setAuto(true);
            break;

        case T_REGISTER:
            if (type_.isRegister())
                translationUnit()->error(ast->specifier_token, "duplicate `%s'", spell(ast->specifier_token));
            type_.setRegister(true);
            break;

        case T_STATIC:
            if (type_.isStatic())
                translationUnit()->error(ast->specifier_token, "duplicate `%s'", spell(ast->specifier_token));
            type_.setStatic(true);
            break;

        case T_EXTERN:
            if (type_.isExtern())
                translationUnit()->error(ast->specifier_token, "duplicate `%s'", spell(ast->specifier_token));
            type_.setExtern(true);
            break;

        case T_MUTABLE:
            if (type_.isMutable())
                translationUnit()->error(ast->specifier_token, "duplicate `%s'", spell(ast->specifier_token));
            type_.setMutable(true);
            break;

        case T_TYPEDEF:
            if (type_.isTypedef())
                translationUnit()->error(ast->specifier_token, "duplicate `%s'", spell(ast->specifier_token));
            type_.setTypedef(true);
            break;

        case T_INLINE:
            if (type_.isInline())
                translationUnit()->error(ast->specifier_token, "duplicate `%s'", spell(ast->specifier_token));
            type_.setInline(true);
            break;

        case T_VIRTUAL:
            if (type_.isVirtual())
                translationUnit()->error(ast->specifier_token, "duplicate `%s'", spell(ast->specifier_token));
            type_.setVirtual(true);
            break;

        case T_EXPLICIT:
            if (type_.isExplicit())
                translationUnit()->error(ast->specifier_token, "duplicate `%s'", spell(ast->specifier_token));
            type_.setExplicit(true);
            break;

        case T_SIGNED:
            if (type_.isSigned())
                translationUnit()->error(ast->specifier_token, "duplicate `%s'", spell(ast->specifier_token));
            type_.setSigned(true);
            break;

        case T_UNSIGNED:
            if (type_.isUnsigned())
                translationUnit()->error(ast->specifier_token, "duplicate `%s'", spell(ast->specifier_token));
            type_.setUnsigned(true);
            break;

        case T_CHAR:
            if (type_)
                translationUnit()->error(ast->specifier_token, "duplicate data type in declaration");
            type_.setType(control()->integerType(IntegerType::Char));
            break;

        case T_CHAR16_T:
            if (type_)
                translationUnit()->error(ast->specifier_token, "duplicate data type in declaration");
            type_.setType(control()->integerType(IntegerType::Char16));
            break;

        case T_CHAR32_T:
            if (type_)
                translationUnit()->error(ast->specifier_token, "duplicate data type in declaration");
            type_.setType(control()->integerType(IntegerType::Char32));
            break;

        case T_WCHAR_T:
            if (type_)
                translationUnit()->error(ast->specifier_token, "duplicate data type in declaration");
            type_.setType(control()->integerType(IntegerType::WideChar));
            break;

        case T_BOOL:
            if (type_)
                translationUnit()->error(ast->specifier_token, "duplicate data type in declaration");
            type_.setType(control()->integerType(IntegerType::Bool));
            break;

        case T_SHORT:
            if (type_) {
                IntegerType *intType = control()->integerType(IntegerType::Int);
                if (type_.type() != intType)
                    translationUnit()->error(ast->specifier_token, "duplicate data type in declaration");
            }
            type_.setType(control()->integerType(IntegerType::Short));
            break;

        case T_INT:
            if (type_) {
                Type *tp = type_.type();
                IntegerType *shortType = control()->integerType(IntegerType::Short);
                IntegerType *longType = control()->integerType(IntegerType::Long);
                IntegerType *longLongType = control()->integerType(IntegerType::LongLong);
                if (tp == shortType || tp == longType || tp == longLongType)
                    break;
                translationUnit()->error(ast->specifier_token, "duplicate data type in declaration");
            }
            type_.setType(control()->integerType(IntegerType::Int));
            break;

        case T_LONG:
            if (type_) {
                Type *tp = type_.type();
                IntegerType *intType = control()->integerType(IntegerType::Int);
                IntegerType *longType = control()->integerType(IntegerType::Long);
                FloatType *doubleType = control()->floatType(FloatType::Double);
                if (tp == longType) {
                    type_.setType(control()->integerType(IntegerType::LongLong));
                    break;
                } else if (tp == doubleType) {
                    type_.setType(control()->floatType(FloatType::LongDouble));
                    break;
                } else if (tp != intType) {
                    translationUnit()->error(ast->specifier_token, "duplicate data type in declaration");
                }
            }
            type_.setType(control()->integerType(IntegerType::Long));
            break;

        case T_FLOAT:
            if (type_)
                translationUnit()->error(ast->specifier_token, "duplicate data type in declaration");
            type_.setType(control()->floatType(FloatType::Float));
            break;

        case T_DOUBLE:
            if (type_) {
                IntegerType *longType = control()->integerType(IntegerType::Long);
                if (type_.type() == longType) {
                    type_.setType(control()->floatType(FloatType::LongDouble));
                    break;
                }
                translationUnit()->error(ast->specifier_token, "duplicate data type in declaration");
            }
            type_.setType(control()->floatType(FloatType::Double));
            break;

        case T_VOID:
            if (type_)
                translationUnit()->error(ast->specifier_token, "duplicate data type in declaration");
            type_.setType(control()->voidType());
            break;

        default:
            break;
    } // switch
    return false;
}

bool Binder::visit(AlignmentSpecifierAST* ast)
{
    // Prevent visiting the type-id or alignment expression from changing the currently
    // calculated type:
    expression(ast->typeIdExprOrAlignmentExpr);
    return false;
}

bool Binder::visit(GnuAttributeSpecifierAST* ast)
{
    // unsigned attribute_token = ast->attribute_token;
    // unsigned first_lparen_token = ast->first_lparen_token;
    // unsigned second_lparen_token = ast->second_lparen_token;
    for (GnuAttributeListAST* it = ast->attribute_list; it; it = it->next) {
        this->attribute(it->value);
    }
    // unsigned first_rparen_token = ast->first_rparen_token;
    // unsigned second_rparen_token = ast->second_rparen_token;
    return false;
}

bool Binder::visit(TypeofSpecifierAST* ast)
{
    ExpressionTy expression = this->expression(ast->expression);
    type_ = expression;
    return false;
}

bool Binder::visit(DecltypeSpecifierAST* ast)
{
    type_ = this->expression(ast->expression);
    return false;
}

bool Binder::visit(ClassSpecifierAST* ast)
{
    // unsigned classkey_token = ast->classkey_token;
    unsigned sourceLocation = ast->firstToken();
    unsigned startScopeOffset = tokenAt(sourceLocation).utf16charsEnd(); // at the end of the class key

    for (SpecifierListAST* it = ast->attribute_list; it; it = it->next) {
        type_ = this->specifier(it->value, type_);
    }

    const Name *className = this->name(ast->name);

    if (ast->name && ! ast->name->asEmptyName()) {
        sourceLocation = location(ast->name, sourceLocation);
        startScopeOffset = tokenAt(sourceLocation).utf16charsEnd(); // at the end of the class name

        if (QualifiedNameAST* q = ast->name->asQualifiedName()) {
            if (q->unqualified_name) {
                sourceLocation = q->unqualified_name->firstToken();
                startScopeOffset = tokenAt(q->unqualified_name->lastToken() - 1).utf16charsEnd(); // at the end of the unqualified name
            }
        }

        ensureValidClassName(&className, sourceLocation);
    }

    Class *klass = control()->newClass(sourceLocation, className);
    klass->setStartOffset(startScopeOffset);
    klass->setEndOffset(tokenAt(ast->lastToken() - 1).utf16charsEnd());
    scope_->addMember(klass);

    if (scope_->isClass())
        klass->setVisibility(visibility_);

    // set the class key
    unsigned classKey = tokenKind(ast->classkey_token);
    if (classKey == T_CLASS)
        klass->setClassKey(Class::ClassKey);
    else if (classKey == T_STRUCT)
        klass->setClassKey(Class::StructKey);
    else if (classKey == T_UNION)
        klass->setClassKey(Class::UnionKey);

    type_.setType(klass);

    Scope *previousScope = switchScope(klass);
    const int previousVisibility = switchVisibility(visibilityForClassKey(classKey));
    const int previousMethodKey = switchMethodKey(Function::NormalMethod);

    for (BaseSpecifierListAST* it = ast->base_clause_list; it; it = it->next) {
        this->baseSpecifier(it->value, ast->colon_token, klass);
    }
    // unsigned dot_dot_dot_token = ast->dot_dot_dot_token;
    for (DeclarationListAST* it = ast->member_specifier_list; it; it = it->next) {
        this->declaration(it->value);
    }

    (void) switchMethodKey(previousMethodKey);
    (void) switchVisibility(previousVisibility);
    (void) switchScope(previousScope);

    ast->symbol = klass;

    return false;
}

bool Binder::visit(NamedTypeSpecifierAST* ast)
{
    NamedType* ty = control()->namedType(this->name(ast->name));
    type_.setType(ty);

    return false;
}

bool Binder::visit(QuantifiedTypeSpecifierAST* ast)
{
    QuantifiedType::Kind kind;
    switch (tokenKind(ast->quantifier_token)) {
    case T__EXISTS:
        kind = QuantifiedType::Existential;
        break;

    case T__FORALL:
        kind = QuantifiedType::Universal;
        break;
    }

    QuantifiedType* ty = control()->quantifiedType(this->name(ast->name), kind, ++quantTyLabel_);
    type_.setType(ty);

    return false;
}

bool Binder::visit(ElaboratedTypeSpecifierAST* ast)
{
    for (SpecifierListAST* it = ast->attribute_list; it; it = it->next)
        type_ = this->specifier(it->value, type_);

    NamedType* ty = control()->namedType(this->name(ast->name));
    type_.setType(ty);

    return false;
}

bool Binder::visit(EnumSpecifierAST* ast)
{
    unsigned sourceLocation = location(ast->name, ast->firstToken());
    const Name *enumName = this->name(ast->name);

    Enum *e = control()->newEnum(sourceLocation, enumName);
    e->setStartOffset(tokenAt(sourceLocation).utf16charsEnd());
    e->setEndOffset(tokenAt(ast->lastToken() - 1).utf16charsEnd());
    if (ast->key_token)
        e->setScoped(true);
    ast->symbol = e;
    scope_->addMember(e);

    if (scope_->isClass())
        e->setVisibility(visibility_);

    Scope *previousScope = switchScope(e);
    for (EnumeratorListAST* it = ast->enumerator_list; it; it = it->next) {
        this->enumerator(it->value, e);
    }
    switchScope(previousScope);

    return false;
}

// PtrOperatorAST
bool Binder::visit(PointerToMemberAST* ast)
{
    const Name *memberName = 0;

    for (NestedNameSpecifierListAST* it = ast->nested_name_specifier_list; it; it = it->next) {
        const Name *class_or_namespace_name = this->nestedNameSpecifier(it->value);
        if (memberName || ast->global_scope_token)
            memberName = control()->qualifiedNameId(memberName, class_or_namespace_name);
        else
            memberName = class_or_namespace_name;
    }

    FullySpecifiedType type(control()->pointerToMemberType(memberName, type_));
    for (SpecifierListAST* it = ast->cv_qualifier_list; it; it = it->next) {
        type = this->specifier(it->value, type);
    }
    type_ = type;
    return false;
}

bool Binder::visit(PointerAST* ast)
{
    if (type_->isReferenceType())
        translationUnit()->error(ast->firstToken(), "cannot declare pointer to a reference");

    FullySpecifiedType type(control()->pointerType(type_));
    for (SpecifierListAST* it = ast->cv_qualifier_list; it; it = it->next) {
        type = this->specifier(it->value, type);
    }
    type_ = type;
    return false;
}

bool Binder::visit(ReferenceAST* ast)
{
    if (type_->isReferenceType())
        translationUnit()->error(ast->firstToken(), "cannot declare reference to a reference");

    FullySpecifiedType type(control()->referenceType(type_));
    type_ = type;
    return false;
}


// PostfixAST
bool Binder::visit(CallAST* ast)
{
    this->expression(ast->base_expression);
    for (ExpressionListAST* it = ast->expression_list; it; it = it->next) {
        this->expression(it->value);
    }

    if (!withinAmbiguousStmt()
            || parentAmbiguousStmt_->ambiguity->resolution() != SyntaxAmbiguity::Resolution::Unknown
            || !ast->base_expression->asIdExpression()) {
        return false;
    }

    // We have a call x decl ambiguity, such as `a(b)`. We can disambiguate it
    // as a call if either we have already bound a function symbol `a` or if we
    // have already bound a variable declaration under name `b`.
    const Name* name = ast->base_expression->asIdExpression()->name->name;
    Symbol *sym = lookupValueSymbol(name, scope_);
    if (sym) {
        if (sym->asFunction()
                || (sym->asDeclaration()
                    && sym->asDeclaration()->type()->asFunctionType())) {
            parentAmbiguousStmt_->ambiguity->applyResolution(SyntaxAmbiguity::Resolution::IsExpr);
            return false;
        }
    } else if (ast->expression_list
               && ast->expression_list->value
               && ast->expression_list->value->asIdExpression()) {
        name = ast->expression_list->value->asIdExpression()->name->name;
        if (lookupValueSymbol(name, scope_)) {
            parentAmbiguousStmt_->ambiguity->applyResolution(SyntaxAmbiguity::Resolution::IsExpr);
            return false;
        }
    }

    return false;
}

bool Binder::visit(ArrayAccessAST* ast)
{
    /*ExpressionTy base_expression =*/ this->expression(ast->base_expression);
    // unsigned lbracket_token = ast->lbracket_token;
    /*ExpressionTy expression =*/ this->expression(ast->expression);
    // unsigned rbracket_token = ast->rbracket_token;
    return false;
}

bool Binder::visit(PostIncrDecrAST* ast)
{
    ExpressionTy base_expression = this->expression(ast->base_expression);
    // unsigned incr_decr_token = ast->incr_decr_token;
    return false;
}

bool Binder::visit(MemberAccessAST* ast)
{
    /*ExpressionTy base_expression =*/ this->expression(ast->base_expression);

    /*const Name *member_name =*/ this->name(ast->member_name);

    if (ast->base_expression->asIdExpression()) {
        maybeResolveAmbiguity(ast->base_expression->asIdExpression()->name->name,
                              &SyntaxAmbiguity::rhs,
                              SyntaxAmbiguity::Resolution::IsDecl);
    }

    return false;
}

// CoreDeclaratorAST
bool Binder::visit(DeclaratorIdAST* ast)
{
    /*const Name *name =*/ this->name(ast->name);
    *decltorIdent_ = ast;
    return false;
}

bool Binder::visit(NestedDeclaratorAST* ast)
{
    type_ = this->declarator(ast->declarator, type_, decltorIdent_);
    return false;
}


// PostfixDeclaratorAST
bool Binder::visit(FunctionDeclaratorAST* ast)
{
    Function *fun = control()->newFunction(0, 0);
    fun->setStartOffset(tokenAt(ast->firstToken()).utf16charsBegin());
    fun->setEndOffset(tokenAt(ast->lastToken() - 1).utf16charsEnd());
    if (ast->trailing_return_type)
        type_ = this->trailingReturnType(ast->trailing_return_type, type_);
    fun->setReturnType(type_);

    // unsigned lparen_token = ast->lparen_token;
    this->parameterDeclarationClause(ast->parameter_declaration_clause, ast->lparen_token, fun);
    // unsigned rparen_token = ast->rparen_token;
    FullySpecifiedType type(fun);
    for (SpecifierListAST* it = ast->cv_qualifier_list; it; it = it->next) {
        type = this->specifier(it->value, type);
    }

    // propagate the cv-qualifiers
    fun->setConst(type.isConst());
    fun->setVolatile(type.isVolatile());
    fun->setOverride(type.isOverride());
    fun->setFinal(type.isFinal());

    this->exceptionSpecification(ast->exception_specification, type);
    if (ast->as_cpp_initializer != 0) {
        fun->setAmbiguous(true);
        /*ExpressionTy as_cpp_initializer =*/ this->expression(ast->as_cpp_initializer);
    }
    ast->symbol = fun;
    type_ = type;
    return false;
}

bool Binder::visit(ArrayDeclaratorAST* ast)
{
    ExpressionTy expression = this->expression(ast->expression);
    FullySpecifiedType type(control()->arrayType(type_));
    type_ = type;
    return false;
}

void Binder::ensureValidClassName(const Name **name, unsigned sourceLocation)
{
    if (!*name)
        return;

    const Name *uqName = nullptr;
    const QualifiedNameId *qName = (*name)->asQualifiedNameId();
    if (qName)
        uqName = qName->name();
    else {
        const TaggedNameId* eName = (*name)->asTaggedNameId();
        if (eName)
            uqName = eName->name();
        else
            uqName = *name;
    }

    if (!uqName->isNameId()
            && !uqName->isTaggedNameId()
            && !uqName->isTemplateNameId()) {
        translationUnit()->error(sourceLocation, "expected a class-name");

        *name = uqName->identifier();
        if (qName)
            *name = control()->qualifiedNameId(qName->base(), *name);
    }
}

std::vector<AmbiguousStatementAST*> Binder::findAmbiguousStmts(const Scope *scope) const
{
    std::vector<AmbiguousStatementAST*> ambigs;
    while (scope) {
        auto it = ambiguousStmts_.find(scope);
        if (it != ambiguousStmts_.end()
                && it->second->ambiguity->resolution() == SyntaxAmbiguity::Resolution::Unknown) {
            ambigs.push_back(it->second);
        }
        scope = scope->enclosingScope();
    }
    return ambigs;
}

void Binder::maybeResolveAmbiguity(const Name* name,
                                 const Name* (SyntaxAmbiguity::*getSide)() const,
                                 SyntaxAmbiguity::Resolution resolution) const
{
    const auto& stmts = findAmbiguousStmts(scope_);
    for (auto ambiStmt : stmts) {
        if (((ambiStmt->ambiguity.get())->*(getSide))()
                && !strcmp(name->identifier()->chars(),
                           (ambiStmt->ambiguity.get()->*(getSide))()->identifier()->chars())) {
            ambiStmt->ambiguity->applyResolution(resolution);
        }
    }
}

int Binder::visibilityForAccessSpecifier(int tokenKind)
{
    switch (tokenKind) {
    case T_PUBLIC:
        return Symbol::Public;
    case T_PROTECTED:
        return Symbol::Protected;
    case T_PRIVATE:
        return Symbol::Private;
    default:
        return Symbol::Public;
    }
}

int Binder::visibilityForClassKey(int tokenKind)
{
    switch (tokenKind) {
    case T_CLASS:
        return Symbol::Private;
    case T_STRUCT:
    case T_UNION:
        return Symbol::Public;
    default:
        return Symbol::Public;
    }
}
