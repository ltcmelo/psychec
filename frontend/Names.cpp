// Copyright (c) 2008 Roberto Raggi <roberto.raggi@gmail.com>
//
// Modifications:
// Copyright (c) 2016,17 Leandro T. C. Melo (ltcmelo@gmail.com)
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

#include "Names.h"
#include "NameVisitor.h"
#include "Literals.h"
#include <algorithm>
#include <cstring>

using namespace psyche;

QualifiedNameId::~QualifiedNameId()
{ }

void QualifiedNameId::accept0(NameVisitor *visitor) const
{ visitor->visit(this); }

const Identifier *QualifiedNameId::identifier() const
{
    if (const Name *u = name())
        return u->identifier();

    return 0;
}

const Name *QualifiedNameId::base() const
{ return _base; }

const Name *QualifiedNameId::name() const
{ return _name; }

DestructorNameId::DestructorNameId(const Name *name)
    : _name(name)
{ }

DestructorNameId::~DestructorNameId()
{ }

void DestructorNameId::accept0(NameVisitor *visitor) const
{ visitor->visit(this); }

const Name *DestructorNameId::name() const
{ return _name; }

const Identifier *DestructorNameId::identifier() const
{ return _name->identifier(); }

TemplateNameId::~TemplateNameId()
{ }

void TemplateNameId::accept0(NameVisitor *visitor) const
{ visitor->visit(this); }

const Identifier *TemplateNameId::identifier() const
{ return _identifier; }

unsigned TemplateNameId::templateArgumentCount() const
{ return unsigned(_templateArguments.size()); }

const FullySpecifiedType &TemplateNameId::templateArgumentAt(unsigned index) const
{ return _templateArguments[index]; }

bool TemplateNameId::Compare::operator()(const TemplateNameId *name,
                                         const TemplateNameId *other) const
{
    if (name == 0)
        return other != 0;
    if (other == 0)
        return false;
    if (name == other)
        return false;

    const Identifier *id = name->identifier();
    const Identifier *otherId = other->identifier();

    if (id == 0)
        return otherId != 0;
    if (otherId == 0)
        return false;

    const int c = std::strcmp(id->chars(), otherId->chars());
    if (c == 0) {
        // we have to differentiate TemplateNameId with respect to specialization or instantiation
        if (name->isSpecialization() == other->isSpecialization()) {
            return std::lexicographical_compare(name->firstTemplateArgument(),
                                                name->lastTemplateArgument(),
                                                other->firstTemplateArgument(),
                                                other->lastTemplateArgument());
        } else {
            return name->isSpecialization();
        }
    }

    return c < 0;
}

OperatorNameId::OperatorNameId(Kind kind)
    : _kind(kind)
{ }

OperatorNameId::~OperatorNameId()
{ }

void OperatorNameId::accept0(NameVisitor *visitor) const
{ visitor->visit(this); }

OperatorNameId::Kind OperatorNameId::kind() const
{ return _kind; }

const Identifier *OperatorNameId::identifier() const
{ return 0; }

TaggedNameId::TaggedNameId(Tag tag, const Name *name)
    : _tag(tag)
    , _name(name)
{ }

TaggedNameId::~TaggedNameId()
{ }

void TaggedNameId::accept0(NameVisitor *visitor) const
{ visitor->visit(this); }

TaggedNameId::Tag TaggedNameId::tag() const
{ return _tag; }

const Name *TaggedNameId::name() const
{ return _name; }

const Identifier *TaggedNameId::identifier() const
{ return _name->identifier(); }

ConversionNameId::ConversionNameId(const FullySpecifiedType &type)
    : _type(type)
{ }

ConversionNameId::~ConversionNameId()
{ }

void ConversionNameId::accept0(NameVisitor *visitor) const
{ visitor->visit(this); }

FullySpecifiedType ConversionNameId::type() const
{ return _type; }

const Identifier *ConversionNameId::identifier() const
{ return 0; }

SelectorNameId::~SelectorNameId()
{ }

void SelectorNameId::accept0(NameVisitor *visitor) const
{ visitor->visit(this); }

const Identifier *SelectorNameId::identifier() const
{
    if (_names.empty())
        return 0;

    return nameAt(0)->identifier();
}

unsigned SelectorNameId::nameCount() const
{ return unsigned(_names.size()); }

const Name *SelectorNameId::nameAt(unsigned index) const
{ return _names[index]; }

bool SelectorNameId::hasArguments() const
{ return _hasArguments; }

AnonymousNameId::AnonymousNameId(unsigned classTokenIndex)
    : _classTokenIndex(classTokenIndex)
{ }

AnonymousNameId::~AnonymousNameId()
{ }

unsigned AnonymousNameId::classTokenIndex() const
{
    return _classTokenIndex;
}

void AnonymousNameId::accept0(NameVisitor *visitor) const
{ visitor->visit(this); }

const Identifier *AnonymousNameId::identifier() const
{ return 0; }
