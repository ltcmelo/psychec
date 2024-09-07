// Copyright (c) 2021/22 Leandro T. C. Melo <ltcmelo@gmail.com>
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

#include "Symbol__IMPL__.inc"
#include "Symbol_Program.h"

#include "types/Type_ALL.h"

#include <sstream>

using namespace psy;
using namespace C;

struct Program::ProgramImpl : SymbolImpl
{
    ProgramImpl()
        : SymbolImpl(SymbolKind::Program)
        , tyVoid_(new VoidType)
        , tyChar_(new BasicType(BasicTypeKind::Char))
        , tyCharS_(new BasicType(BasicTypeKind::Char_S))
        , tyCharU_(new BasicType(BasicTypeKind::Char_U))
        , tyShort_(new BasicType(BasicTypeKind::Short))
        , tyShortS_(new BasicType(BasicTypeKind::Short_S))
        , tyShortU_(new BasicType(BasicTypeKind::Short_U))
        , tyInt_(new BasicType(BasicTypeKind::Int))
        , tyIntS_(new BasicType(BasicTypeKind::Int_S))
        , tyIntU_(new BasicType(BasicTypeKind::Int_U))
        , tyLong_(new BasicType(BasicTypeKind::Long))
        , tyLongS_(new BasicType(BasicTypeKind::Long_S))
        , tyLongU_(new BasicType(BasicTypeKind::Long_U))
        , tyLongLong_(new BasicType(BasicTypeKind::LongLong))
        , tyLongLongS_(new BasicType(BasicTypeKind::LongLong_S))
        , tyLongLongU_(new BasicType(BasicTypeKind::LongLong_U))
        , tyFloat_(new BasicType(BasicTypeKind::Float))
        , tyDouble_(new BasicType(BasicTypeKind::Double))
        , tyLongDouble_(new BasicType(BasicTypeKind::LongDouble))
        , tyBool_(new BasicType(BasicTypeKind::Bool))
        , tyFloatComplex_(new BasicType(BasicTypeKind::FloatComplex))
        , tyDoubleComplex_(new BasicType(BasicTypeKind::DoubleComplex))
        , tyLongDoubleComplex_(new BasicType(BasicTypeKind::LongDoubleComplex))
    {
    }

    std::unique_ptr<VoidType> tyVoid_;
    std::unique_ptr<BasicType> tyChar_;
    std::unique_ptr<BasicType> tyCharS_;
    std::unique_ptr<BasicType> tyCharU_;
    std::unique_ptr<BasicType> tyShort_;
    std::unique_ptr<BasicType> tyShortS_;
    std::unique_ptr<BasicType> tyShortU_;
    std::unique_ptr<BasicType> tyInt_;
    std::unique_ptr<BasicType> tyIntS_;
    std::unique_ptr<BasicType> tyIntU_;
    std::unique_ptr<BasicType> tyLong_;
    std::unique_ptr<BasicType> tyLongS_;
    std::unique_ptr<BasicType> tyLongU_;
    std::unique_ptr<BasicType> tyLongLong_;
    std::unique_ptr<BasicType> tyLongLongS_;
    std::unique_ptr<BasicType> tyLongLongU_;
    std::unique_ptr<BasicType> tyFloat_;
    std::unique_ptr<BasicType> tyDouble_;
    std::unique_ptr<BasicType> tyLongDouble_;
    std::unique_ptr<BasicType> tyBool_;
    std::unique_ptr<BasicType> tyFloatComplex_;
    std::unique_ptr<BasicType> tyDoubleComplex_;
    std::unique_ptr<BasicType> tyLongDoubleComplex_;
};

Program::Program()
    : Symbol(new ProgramImpl)
{
}

std::string Program::toDisplayString() const
{
    return "";
}

const VoidType* Program::canonicalVoidType() const
{
    return P_CAST->tyVoid_.get();
}

const BasicType* Program::canonicalBasicType(BasicTypeKind basicTyK) const
{
    switch (basicTyK) {
        case BasicTypeKind::Char:
            return P_CAST->tyChar_.get();
        case BasicTypeKind::Char_S:
            return P_CAST->tyCharS_.get();
        case BasicTypeKind::Char_U:
            return P_CAST->tyCharU_.get();
        case BasicTypeKind::Short:
            return P_CAST->tyShort_.get();
        case BasicTypeKind::Short_S:
            return P_CAST->tyShortS_.get();
        case BasicTypeKind::Short_U:
            return P_CAST->tyShortU_.get();
        case BasicTypeKind::Int:
            return P_CAST->tyInt_.get();
        case BasicTypeKind::Int_S:
            return P_CAST->tyIntS_.get();
        case BasicTypeKind::Int_U:
            return P_CAST->tyIntU_.get();
        case BasicTypeKind::Long:
            return P_CAST->tyLong_.get();
        case BasicTypeKind::Long_S:
            return P_CAST->tyLongS_.get();
        case BasicTypeKind::Long_U:
            return P_CAST->tyLongU_.get();
        case BasicTypeKind::LongLong:
            return P_CAST->tyLongLong_.get();
        case BasicTypeKind::LongLong_S:
            return P_CAST->tyLongLongS_.get();
        case BasicTypeKind::LongLong_U:
            return P_CAST->tyLongLongU_.get();
        case BasicTypeKind::Float:
            return P_CAST->tyFloat_.get();
        case BasicTypeKind::Double:
            return P_CAST->tyDouble_.get();
        case BasicTypeKind::LongDouble:
            return P_CAST->tyLongDouble_.get();
        case BasicTypeKind::Bool:
            return P_CAST->tyBool_.get();
        case BasicTypeKind::FloatComplex:
            return P_CAST->tyFloatComplex_.get();
        case BasicTypeKind::DoubleComplex:
            return P_CAST->tyDoubleComplex_.get();
        case BasicTypeKind::LongDoubleComplex:
            return P_CAST->tyLongDoubleComplex_.get();
    }
}

namespace psy {
namespace C {

std::string to_string(const Program& prog)
{
    std::ostringstream oss;
    oss << "<Program>";
    return oss.str();
}

} // C
} // psi
