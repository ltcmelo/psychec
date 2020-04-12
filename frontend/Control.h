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

#ifndef PSYCHE_CONTROL_H__
#define PSYCHE_CONTROL_H__

#include "FrontendConfig.h"

#include "ASTFwds.h"
#include "CoreTypes.h"
#include "Names.h"

namespace psyche {

class CFE_API TopLevelDeclarationProcessor
{
public:
    virtual ~TopLevelDeclarationProcessor() {}
    virtual bool processDeclaration(DeclarationAST* ast) = 0;
};

class CFE_API Control
{
public:
    Control();
    ~Control();

    Control(const Control&) = delete;
    Control& operator=(const Control&) = delete;

    TranslationUnit *translationUnit() const;
    TranslationUnit *switchTranslationUnit(TranslationUnit *unit);

    TopLevelDeclarationProcessor *topLevelDeclarationProcessor() const;
    void setTopLevelDeclarationProcessor(TopLevelDeclarationProcessor *processor);

    DiagnosticCollector *diagnosticCollector() const;
    void setDiagnosticCollector(DiagnosticCollector* collector);

    const EmptyName *emptyName();

    const Identifier* findIdentifier(const char *chars, unsigned size) const;
    const Identifier* identifier(const char *chars, unsigned size);
    const Identifier* identifier(const char *chars);

    const TemplateNameId *templateNameId(const Identifier* id,
                                         bool isSpecialization,
                                         const FullySpecifiedType* const args = 0,
                                         unsigned argc = 0);
    const DestructorNameId *destructorNameId(const Name* name);
    const TaggedNameId *taggedNameId(TaggedNameId::Tag, const Name* name);
    const OperatorNameId *operatorNameId(OperatorNameId::Kind operatorId);
    const ConversionNameId *conversionNameId(const FullySpecifiedType& type);
    const QualifiedNameId *qualifiedNameId(const Name* base, const Name* name);
    const SelectorNameId *selectorNameId(const Name* const *names,
                                         unsigned nameCount,
                                         bool hasArguments);

    VoidType* voidType();
    IntegerType* integerType(IntegerType::Kind kind);
    FloatType* floatType(FloatType::Kind floatId);
    PointerToMemberType* pointerToMemberType(const Name* memberName, const FullySpecifiedType& elementType);
    PointerType* pointerType(const FullySpecifiedType& elementType);
    ReferenceType* referenceType(const FullySpecifiedType& elementType);
    ArrayType* arrayType(const FullySpecifiedType& elementType, unsigned size = 0);
    NamedType* namedType(const Name* name);
    QuantifiedType* quantifiedType(const Name* name, QuantifiedType::Kind kind, int label);

    Declaration *newDeclaration(unsigned sourceLocation, const Name* name);
    EnumeratorDeclaration *newEnumeratorDeclaration(unsigned sourceLocation, const Name* name);
    Argument *newArgument(unsigned sourceLocation, const Name* name = 0);
    TypenameArgument *newTypenameArgument(unsigned sourceLocation, const Name* name = 0);
    Function *newFunction(unsigned sourceLocation, const Name* name = 0);
    Namespace *newNamespace(unsigned sourceLocation, const Name* name = 0);
    Template *newTemplate(unsigned sourceLocation, const Name* name = 0);
    NamespaceAlias *newNamespaceAlias(unsigned sourceLocation, const Name* name = 0);
    BaseClass *newBaseClass(unsigned sourceLocation, const Name* name = 0);
    Class *newClass(unsigned sourceLocation, const Name* name = 0);
    Enum *newEnum(unsigned sourceLocation, const Name* name = 0);
    Block *newBlock(unsigned sourceLocation);
    UsingNamespaceDirective *newUsingNamespaceDirective(unsigned sourceLocation, const Name* name = 0);
    UsingDeclaration *newUsingDeclaration(unsigned sourceLocation, const Name* name = 0);
    ForwardClassDeclaration *newForwardClassDeclaration(unsigned sourceLocation, const Name* name = 0);

    /// Symbol annulation
    void annulSymbol(const Symbol* sym);
    bool isSymbolAnulled(const Symbol* sym);

    const Identifier* deprecatedId() const;
    const Identifier* unavailableId() const;

    // C++11 contextual keywords
    const Identifier* cpp11Override() const;
    const Identifier* cpp11Final() const;

    // Distinguished attributes.
    const Identifier* attrAvailability() const;

    const OperatorNameId *findOperatorNameId(OperatorNameId::Kind operatorId) const;

    typedef const Identifier* const *IdentifierIterator;
    typedef const StringLiteral* const *StringLiteralIterator;
    typedef const NumericLiteral* const *NumericLiteralIterator;

    IdentifierIterator firstIdentifier() const;
    IdentifierIterator lastIdentifier() const;

    StringLiteralIterator firstStringLiteral() const;
    StringLiteralIterator lastStringLiteral() const;

    NumericLiteralIterator firstNumericLiteral() const;
    NumericLiteralIterator lastNumericLiteral() const;

    const StringLiteral* stringLiteral(const char *chars, unsigned size);
    const StringLiteral* stringLiteral(const char *chars);

    const NumericLiteral* numericLiteral(const char *chars, unsigned size);
    const NumericLiteral* numericLiteral(const char *chars);

    Symbol** firstSymbol() const;
    Symbol** lastSymbol() const;
    unsigned symbolCount() const;

    bool hasSymbol(Symbol* symbol) const;
    void addSymbol(Symbol* symbol);

    void squeeze();

    void reset();

private:
    void populateBuiltins();

    class Data;
    friend class Data;
    Data *d;
};

} // namespace psyche


#endif
