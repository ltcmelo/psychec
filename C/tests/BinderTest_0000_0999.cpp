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

#include "BinderTest.h"

#include "ParserTest.h"

#include "syntax/SyntaxLexeme_ALL.h"
#include "parser/Unparser.h"

#include "syntax/SyntaxNodes.h"

#include <algorithm>
#include <cstring>
#include <iostream>
#include <string>
#include <sstream>

using namespace psy;
using namespace C;

void BinderTest::case0001()
{
    bind("void x ( ) ;",
         Expectation()
         .binding(DeclSummary()
                  .Function("x", ScopeKind::File)
                  .TySpec.deriv(TypeKind::Function)
                  .TySpec.basis("void", NamedTypeKind::Builtin, BuiltinTypeKind::Void)));
}

void BinderTest::case0002()
{
    bind("int x ( ) ;",
         Expectation()
         .binding(DeclSummary()
                  .Function("x", ScopeKind::File)
                  .TySpec.basis("int", NamedTypeKind::Builtin, BuiltinTypeKind::Int)
                  .TySpec.deriv(TypeKind::Function)));
}

void BinderTest::case0003()
{
    bind("void * x ( ) ;",
         Expectation()
         .binding(DeclSummary()
                  .Function("x", ScopeKind::File)
                  .TySpec.basis("void", NamedTypeKind::Builtin, BuiltinTypeKind::Void)
                  .TySpec.deriv(TypeKind::Pointer)
                  .TySpec.deriv(TypeKind::Function)));
}

void BinderTest::case0004()
{
    bind("int * x ( ) ;",
         Expectation()
         .binding(DeclSummary()
                  .Function("x", ScopeKind::File)
                  .TySpec.basis("int", NamedTypeKind::Builtin, BuiltinTypeKind::Int)
                  .TySpec.deriv(TypeKind::Pointer)
                  .TySpec.deriv(TypeKind::Function)));
}

void BinderTest::case0005()
{
    bind("x y ( ) ;",
         Expectation()
         .binding(DeclSummary()
                  .Function("y")
                  .TySpec.basis("x", NamedTypeKind::Synonym)
                  .TySpec.deriv(TypeKind::Function)));
}

void BinderTest::case0006()
{
    bind("int * ( x ) ( ) ;",
         Expectation()
         .binding(DeclSummary()
                  .Function("x", ScopeKind::File)
                  .TySpec.basis("int", NamedTypeKind::Builtin, BuiltinTypeKind::Int)
                  .TySpec.deriv(TypeKind::Pointer)
                  .TySpec.deriv(TypeKind::Function)));
}

void BinderTest::case0007()
{
    bind("int * ( ( x ) ) ( ) ;",
         Expectation()
         .binding(DeclSummary()
                  .Function("x", ScopeKind::File)
                  .TySpec.basis("int", NamedTypeKind::Builtin, BuiltinTypeKind::Int)
                  .TySpec.deriv(TypeKind::Pointer)
                  .TySpec.deriv(TypeKind::Function)));
}

void BinderTest::case0008()
{
    bind("int * * x ( ) ;",
         Expectation()
         .binding(DeclSummary().Function("x", ScopeKind::File)
                  .TySpec.basis("int", NamedTypeKind::Builtin, BuiltinTypeKind::Int)
                  .TySpec.deriv(TypeKind::Pointer)
                  .TySpec.deriv(TypeKind::Pointer)
                  .TySpec.deriv(TypeKind::Function)));
}

void BinderTest::case0009()
{
    bind("x * y ( ) ;",
         Expectation()
         .binding(DeclSummary().Function("y")
                  .TySpec.basis("x", NamedTypeKind::Synonym)
                  .TySpec.deriv(TypeKind::Pointer)
                  .TySpec.deriv(TypeKind::Function)));
}

void BinderTest::case0010()
{
}

void BinderTest::case0011()
{
}

void BinderTest::case0012()
{
}

void BinderTest::case0013()
{
}

void BinderTest::case0014()
{
}

void BinderTest::case0015() {}
void BinderTest::case0016() {}
void BinderTest::case0017() {}
void BinderTest::case0018() {}
void BinderTest::case0019() {}
void BinderTest::case0020() {}
void BinderTest::case0021() {}
void BinderTest::case0022() {}
void BinderTest::case0023() {}
void BinderTest::case0024() {}
void BinderTest::case0025() {}
void BinderTest::case0026() {}

void BinderTest::case0027() {}
void BinderTest::case0028() {}
void BinderTest::case0029() {}
void BinderTest::case0030() {}
void BinderTest::case0031() {}
void BinderTest::case0032() {}
void BinderTest::case0033() {}
void BinderTest::case0034() {}
void BinderTest::case0035() {}
void BinderTest::case0036() {}
void BinderTest::case0037() {}
void BinderTest::case0038() {}
void BinderTest::case0039() {}
void BinderTest::case0040() {}
void BinderTest::case0041() {}
void BinderTest::case0042() {}
void BinderTest::case0043() {}
void BinderTest::case0044() {}
void BinderTest::case0045() {}
void BinderTest::case0046() {}
void BinderTest::case0047() {}
void BinderTest::case0048() {}
void BinderTest::case0049() {}

void BinderTest::case0050()
{
    bind("void x ( int y ) ;",
         Expectation()
         .binding(DeclSummary()
                  .Function("x", ScopeKind::File)
                  .TySpec.basis("void", NamedTypeKind::Builtin, BuiltinTypeKind::Void)
                  .TySpec.deriv(TypeKind::Function)
                  .TySpec.Parameter().basis("int", NamedTypeKind::Builtin, BuiltinTypeKind::Int))
         .binding(DeclSummary()
                  .Value("y", ValueKind::Parameter, ScopeKind::FunctionPrototype)
                  .TySpec.basis("int", NamedTypeKind::Builtin, BuiltinTypeKind::Int)));
}

void BinderTest::case0051()
{
    bind("void x ( int y , double z ) ;",
         Expectation()
         .binding(DeclSummary()
                  .Function("x", ScopeKind::File)
                  .TySpec.basis("void", NamedTypeKind::Builtin, BuiltinTypeKind::Void)
                  .TySpec.deriv(TypeKind::Function)
                  .TySpec.Parameter().basis("int", NamedTypeKind::Builtin, BuiltinTypeKind::Int)
                  .TySpec.Parameter().basis("double", NamedTypeKind::Builtin, BuiltinTypeKind::Double))
         .binding(DeclSummary()
                  .Value("y", ValueKind::Parameter, ScopeKind::FunctionPrototype)
                  .TySpec.basis("int", NamedTypeKind::Builtin, BuiltinTypeKind::Int))
         .binding(DeclSummary()
                  .Value("z", ValueKind::Parameter, ScopeKind::FunctionPrototype)
                  .TySpec.basis("double", NamedTypeKind::Builtin, BuiltinTypeKind::Double)));
}

void BinderTest::case0052()
{
    bind("void x ( int * y ) ;",
         Expectation()
         .binding(DeclSummary()
                  .Function("x", ScopeKind::File)
                  .TySpec.basis("void", NamedTypeKind::Builtin, BuiltinTypeKind::Void)
                  .TySpec.deriv(TypeKind::Function)
                  .TySpec.Parameter().basis("int", NamedTypeKind::Builtin, BuiltinTypeKind::Int)
                  .TySpec._AtParam_().deriv(TypeKind::Pointer))
         .binding(DeclSummary()
                  .Value("y", ValueKind::Parameter, ScopeKind::FunctionPrototype)
                  .TySpec.basis("int", NamedTypeKind::Builtin, BuiltinTypeKind::Int)
                  .TySpec.deriv(TypeKind::Pointer)));
}

void BinderTest::case0053()
{
    bind("void x ( int * y , double * z) ;",
         Expectation()
         .binding(DeclSummary()
                  .Function("x", ScopeKind::File)
                  .TySpec.basis("void", NamedTypeKind::Builtin, BuiltinTypeKind::Void)
                  .TySpec.deriv(TypeKind::Function)
                  .TySpec.Parameter().basis("int", NamedTypeKind::Builtin, BuiltinTypeKind::Int)
                  .TySpec._AtParam_().deriv(TypeKind::Pointer)
                  .TySpec.Parameter().basis("double", NamedTypeKind::Builtin, BuiltinTypeKind::Double)
                  .TySpec._AtParam_().deriv(TypeKind::Pointer))
         .binding(DeclSummary()
                  .Value("y", ValueKind::Parameter, ScopeKind::FunctionPrototype)
                  .TySpec.basis("int", NamedTypeKind::Builtin, BuiltinTypeKind::Int)
                  .TySpec.deriv(TypeKind::Pointer))
         .binding(DeclSummary()
                  .Value("z", ValueKind::Parameter, ScopeKind::FunctionPrototype)
                  .TySpec.basis("double", NamedTypeKind::Builtin, BuiltinTypeKind::Double)
                  .TySpec.deriv(TypeKind::Pointer)));
}

void BinderTest::case0054()
{
    bind("void x ( int * * y , double * * z) ;",
         Expectation()
         .binding(DeclSummary()
                  .Function("x", ScopeKind::File)
                  .TySpec.basis("void", NamedTypeKind::Builtin, BuiltinTypeKind::Void)
                  .TySpec.deriv(TypeKind::Function)
                  .TySpec.Parameter().basis("int", NamedTypeKind::Builtin, BuiltinTypeKind::Int)
                  .TySpec._AtParam_().deriv(TypeKind::Pointer)
                  .TySpec._AtParam_().deriv(TypeKind::Pointer)
                  .TySpec.Parameter().basis("double", NamedTypeKind::Builtin, BuiltinTypeKind::Double)
                  .TySpec._AtParam_().deriv(TypeKind::Pointer)
                  .TySpec._AtParam_().deriv(TypeKind::Pointer))
         .binding(DeclSummary()
                  .Value("y", ValueKind::Parameter, ScopeKind::FunctionPrototype)
                  .TySpec.basis("int", NamedTypeKind::Builtin, BuiltinTypeKind::Int)
                  .TySpec.deriv(TypeKind::Pointer)
                  .TySpec.deriv(TypeKind::Pointer))
         .binding(DeclSummary()
                  .Value("z", ValueKind::Parameter, ScopeKind::FunctionPrototype)
                  .TySpec.basis("double", NamedTypeKind::Builtin, BuiltinTypeKind::Double)
                  .TySpec.deriv(TypeKind::Pointer)
                  .TySpec.deriv(TypeKind::Pointer)));
}

void BinderTest::case0055()
{
    bind("void x ( int * * y , double z) ;",
         Expectation()
         .binding(DeclSummary()
                  .Function("x", ScopeKind::File)
                  .TySpec.basis("void", NamedTypeKind::Builtin, BuiltinTypeKind::Void)
                  .TySpec.deriv(TypeKind::Function)
                  .TySpec.Parameter().basis("int", NamedTypeKind::Builtin, BuiltinTypeKind::Int)
                  .TySpec._AtParam_().deriv(TypeKind::Pointer)
                  .TySpec._AtParam_().deriv(TypeKind::Pointer)
                  .TySpec.Parameter().basis("double", NamedTypeKind::Builtin, BuiltinTypeKind::Double))
         .binding(DeclSummary()
                  .Value("y", ValueKind::Parameter, ScopeKind::FunctionPrototype)
                  .TySpec.basis("int", NamedTypeKind::Builtin, BuiltinTypeKind::Int)
                  .TySpec.deriv(TypeKind::Pointer)
                  .TySpec.deriv(TypeKind::Pointer))
         .binding(DeclSummary()
                  .Value("z", ValueKind::Parameter, ScopeKind::FunctionPrototype)
                  .TySpec.basis("double", NamedTypeKind::Builtin, BuiltinTypeKind::Double)));
}

void BinderTest::case0056()
{
    bind("void x ( int y , double * * z) ;",
         Expectation()
         .binding(DeclSummary()
                  .Function("x", ScopeKind::File)
                  .TySpec.basis("void", NamedTypeKind::Builtin, BuiltinTypeKind::Void)
                  .TySpec.deriv(TypeKind::Function)
                  .TySpec.Parameter().basis("int", NamedTypeKind::Builtin, BuiltinTypeKind::Int)
                  .TySpec.Parameter().basis("double", NamedTypeKind::Builtin, BuiltinTypeKind::Double)
                  .TySpec._AtParam_().deriv(TypeKind::Pointer)
                  .TySpec._AtParam_().deriv(TypeKind::Pointer))
         .binding(DeclSummary()
                  .Value("y", ValueKind::Parameter, ScopeKind::FunctionPrototype)
                  .TySpec.basis("int", NamedTypeKind::Builtin, BuiltinTypeKind::Int))
         .binding(DeclSummary()
                  .Value("z", ValueKind::Parameter, ScopeKind::FunctionPrototype)
                  .TySpec.basis("double", NamedTypeKind::Builtin, BuiltinTypeKind::Double)
                  .TySpec.deriv(TypeKind::Pointer)
                  .TySpec.deriv(TypeKind::Pointer)));
}

void BinderTest::case0057()
{
    bind("void x ( y z , w * * v ) ;",
         Expectation()
         .binding(DeclSummary()
                  .Function("x", ScopeKind::File)
                  .TySpec.basis("void", NamedTypeKind::Builtin, BuiltinTypeKind::Void)
                  .TySpec.deriv(TypeKind::Function)
                  .TySpec.Parameter().basis("y", NamedTypeKind::Synonym)
                  .TySpec.Parameter().basis("w", NamedTypeKind::Synonym)
                  .TySpec._AtParam_().deriv(TypeKind::Pointer)
                  .TySpec._AtParam_().deriv(TypeKind::Pointer))
         .binding(DeclSummary()
                  .Value("z", ValueKind::Parameter, ScopeKind::FunctionPrototype)
                  .TySpec.basis("y", NamedTypeKind::Synonym))
         .binding(DeclSummary()
                  .Value("v", ValueKind::Parameter, ScopeKind::FunctionPrototype)
                  .TySpec.basis("w", NamedTypeKind::Synonym)
                  .TySpec.deriv(TypeKind::Pointer)
                  .TySpec.deriv(TypeKind::Pointer)));
}

void BinderTest::case0058()
{
    bind("x y ( z const * w , u * v) ;",
         Expectation()
         .binding(DeclSummary()
                  .Function("y", ScopeKind::File)
                  .TySpec.basis("x", NamedTypeKind::Synonym)
                  .TySpec.deriv(TypeKind::Function)
                  .TySpec.Parameter().basis("z", NamedTypeKind::Synonym, BuiltinTypeKind::UNSPECIFIED, CVR::Const)
                  .TySpec._AtParam_().deriv(TypeKind::Pointer)
                  .TySpec.Parameter().basis("u", NamedTypeKind::Synonym)
                  .TySpec._AtParam_().deriv(TypeKind::Pointer))
         .binding(DeclSummary()
                  .Value("w", ValueKind::Parameter, ScopeKind::FunctionPrototype)
                  .TySpec.basis("z", NamedTypeKind::Synonym, BuiltinTypeKind::UNSPECIFIED, CVR::Const)
                  .TySpec.deriv(TypeKind::Pointer))
         .binding(DeclSummary()
                  .Value("v", ValueKind::Parameter, ScopeKind::FunctionPrototype)
                  .TySpec.basis("u", NamedTypeKind::Synonym)
                  .TySpec.deriv(TypeKind::Pointer)));
}

void BinderTest::case0059()
{
    bind("x y ( z * const w , u * v) ;",
         Expectation()
         .binding(DeclSummary()
                  .Function("y", ScopeKind::File)
                  .TySpec.basis("x", NamedTypeKind::Synonym)
                  .TySpec.deriv(TypeKind::Function)
                  .TySpec.Parameter().basis("z", NamedTypeKind::Synonym)
                  .TySpec._AtParam_().deriv(TypeKind::Pointer, CVR::Const)
                  .TySpec.Parameter().basis("u", NamedTypeKind::Synonym)
                  .TySpec._AtParam_().deriv(TypeKind::Pointer))
         .binding(DeclSummary()
                  .Value("w", ValueKind::Parameter, ScopeKind::FunctionPrototype)
                  .TySpec.basis("z", NamedTypeKind::Synonym)
                  .TySpec.deriv(TypeKind::Pointer, CVR::Const))
         .binding(DeclSummary()
                  .Value("v", ValueKind::Parameter, ScopeKind::FunctionPrototype)
                  .TySpec.basis("u", NamedTypeKind::Synonym)
                  .TySpec.deriv(TypeKind::Pointer)));
}

void BinderTest::case0060()
{
    bind("x * y ( z w) ;",
         Expectation()
         .binding(DeclSummary()
                  .Function("y", ScopeKind::File)
                  .TySpec.basis("x", NamedTypeKind::Synonym)
                  .TySpec.deriv(TypeKind::Pointer)
                  .TySpec.deriv(TypeKind::Function)
                  .TySpec.Parameter().basis("z", NamedTypeKind::Synonym))
         .binding(DeclSummary()
                  .Value("w", ValueKind::Parameter, ScopeKind::FunctionPrototype)
                  .TySpec.basis("z", NamedTypeKind::Synonym)));
}

void BinderTest::case0061() {}
void BinderTest::case0062() {}
void BinderTest::case0063() {}
void BinderTest::case0064() {}
void BinderTest::case0065() {}
void BinderTest::case0066() {}
void BinderTest::case0067() {}
void BinderTest::case0068() {}
void BinderTest::case0069() {}
void BinderTest::case0070() {}
void BinderTest::case0071() {}
void BinderTest::case0072() {}
void BinderTest::case0073() {}
void BinderTest::case0074() {}
void BinderTest::case0075() {}
void BinderTest::case0076() {}
void BinderTest::case0077() {}
void BinderTest::case0078() {}
void BinderTest::case0079() {}
void BinderTest::case0080() {}
void BinderTest::case0081() {}
void BinderTest::case0082() {}
void BinderTest::case0083() {}
void BinderTest::case0084() {}
void BinderTest::case0085() {}
void BinderTest::case0086() {}
void BinderTest::case0087() {}
void BinderTest::case0088() {}
void BinderTest::case0089() {}
void BinderTest::case0090() {}
void BinderTest::case0091() {}
void BinderTest::case0092() {}
void BinderTest::case0093() {}
void BinderTest::case0094() {}
void BinderTest::case0095() {}
void BinderTest::case0096() {}
void BinderTest::case0097() {}
void BinderTest::case0098() {}
void BinderTest::case0099() {}
void BinderTest::case0100() {}

void BinderTest::case0101()
{
    bind("void x ( ) { }",
         Expectation()
         .binding(DeclSummary()
                  .Function("x", ScopeKind::File)
                  .TySpec.basis("void", NamedTypeKind::Builtin, BuiltinTypeKind::Void)
                  .TySpec.deriv(TypeKind::Function)));
}

void BinderTest::case0102()
{
    bind("int x ( ) { return 1 ; }",
         Expectation()
         .binding(DeclSummary()
                  .Function("x", ScopeKind::File)
                  .TySpec.basis("int", NamedTypeKind::Builtin, BuiltinTypeKind::Int)
                  .TySpec.deriv(TypeKind::Function)));
}

void BinderTest::case0103()
{
    bind("void * x ( ) { return 1 ; }",
         Expectation()
         .binding(DeclSummary()
                  .Function("x", ScopeKind::File)
                  .TySpec.basis("void", NamedTypeKind::Builtin, BuiltinTypeKind::Void)
                  .TySpec.deriv(TypeKind::Pointer)
                  .TySpec.deriv(TypeKind::Function)));
}

void BinderTest::case0104()
{
    bind("int * x ( ) { return 1 ; }",
         Expectation()
         .binding(DeclSummary()
                  .Function("x", ScopeKind::File)
                  .TySpec.basis("int", NamedTypeKind::Builtin, BuiltinTypeKind::Int)
                  .TySpec.deriv(TypeKind::Pointer)
                  .TySpec.deriv(TypeKind::Function)));
}

void BinderTest::case0105()
{
    bind("x y ( ) { return 1 ; }",
         Expectation()
         .binding(DeclSummary()
                  .Function("y")
                  .TySpec.basis("x", NamedTypeKind::Synonym)
                  .TySpec.deriv(TypeKind::Function)));
}

void BinderTest::case0106()
{
    bind("int * ( x ) ( ) { return 1 ; }",
         Expectation()
         .binding(DeclSummary()
                  .Function("x", ScopeKind::File)
                  .TySpec.basis("int", NamedTypeKind::Builtin, BuiltinTypeKind::Int)
                  .TySpec.deriv(TypeKind::Pointer)
                  .TySpec.deriv(TypeKind::Function)));
}

void BinderTest::case0107()
{
    bind("int * ( ( x ) ) ( ) { return 1 ; }",
         Expectation()
         .binding(DeclSummary().Function("x", ScopeKind::File)
                  .TySpec.basis("int", NamedTypeKind::Builtin, BuiltinTypeKind::Int)
                  .TySpec.deriv(TypeKind::Pointer)
                  .TySpec.deriv(TypeKind::Function)));
}

void BinderTest::case0108()
{
    bind("int * * x ( ) { return 1 ; }",
         Expectation()
         .binding(DeclSummary().Function("x", ScopeKind::File)
                  .TySpec.basis("int", NamedTypeKind::Builtin, BuiltinTypeKind::Int)
                  .TySpec.deriv(TypeKind::Pointer)
                  .TySpec.deriv(TypeKind::Pointer)
                  .TySpec.deriv(TypeKind::Function)));
}

void BinderTest::case0109()
{
    bind("x * y ( ) { return 1 ; }",
         Expectation()
         .binding(DeclSummary().Function("y")
                  .TySpec.basis("x", NamedTypeKind::Synonym)
                  .TySpec.deriv(TypeKind::Pointer)
                  .TySpec.deriv(TypeKind::Function)));
}

void BinderTest::case0110()
{
}

void BinderTest::case0111()
{
}

void BinderTest::case0112()
{
}

void BinderTest::case0113()
{
}

void BinderTest::case0114()
{
}

void BinderTest::case0115() {}
void BinderTest::case0116() {}
void BinderTest::case0117() {}
void BinderTest::case0118() {}
void BinderTest::case0119() {}
void BinderTest::case0120() {}
void BinderTest::case0121() {}
void BinderTest::case0122() {}
void BinderTest::case0123() {}
void BinderTest::case0124() {}
void BinderTest::case0125() {}
void BinderTest::case0126() {}
void BinderTest::case0127() {}
void BinderTest::case0128() {}
void BinderTest::case0129() {}
void BinderTest::case0130() {}
void BinderTest::case0131() {}
void BinderTest::case0132() {}
void BinderTest::case0133() {}
void BinderTest::case0134() {}
void BinderTest::case0135() {}
void BinderTest::case0136() {}
void BinderTest::case0137() {}
void BinderTest::case0138() {}
void BinderTest::case0139() {}
void BinderTest::case0140() {}
void BinderTest::case0141() {}
void BinderTest::case0142() {}
void BinderTest::case0143() {}
void BinderTest::case0144() {}
void BinderTest::case0145() {}
void BinderTest::case0146() {}
void BinderTest::case0147() {}
void BinderTest::case0148() {}
void BinderTest::case0149() {}

void BinderTest::case0150()
{
    bind("void x ( int y ) { }",
         Expectation()
         .binding(DeclSummary()
                  .Function("x", ScopeKind::File)
                  .TySpec.basis("void", NamedTypeKind::Builtin, BuiltinTypeKind::Void)
                  .TySpec.deriv(TypeKind::Function)
                  .TySpec.Parameter().basis("int", NamedTypeKind::Builtin, BuiltinTypeKind::Int))
         .binding(DeclSummary()
                  .Value("y", ValueKind::Parameter, ScopeKind::Block)
                  .TySpec.basis("int", NamedTypeKind::Builtin, BuiltinTypeKind::Int)));
}

void BinderTest::case0151()
{
    bind("void x ( int y , double z ) { }",
         Expectation()
         .binding(DeclSummary()
                  .Function("x", ScopeKind::File)
                  .TySpec.basis("void", NamedTypeKind::Builtin, BuiltinTypeKind::Void)
                  .TySpec.deriv(TypeKind::Function)
                  .TySpec.Parameter().basis("int", NamedTypeKind::Builtin, BuiltinTypeKind::Int)
                  .TySpec.Parameter().basis("double", NamedTypeKind::Builtin, BuiltinTypeKind::Double))
         .binding(DeclSummary()
                  .Value("y", ValueKind::Parameter, ScopeKind::Block)
                  .TySpec.basis("int", NamedTypeKind::Builtin, BuiltinTypeKind::Int))
         .binding(DeclSummary()
                  .Value("z", ValueKind::Parameter, ScopeKind::Block)
                  .TySpec.basis("double", NamedTypeKind::Builtin, BuiltinTypeKind::Double)));

}

void BinderTest::case0152()
{
    bind("void x ( int * y ) { }",
         Expectation()
         .binding(DeclSummary()
                  .Function("x", ScopeKind::File)
                  .TySpec.basis("void", NamedTypeKind::Builtin, BuiltinTypeKind::Void)
                  .TySpec.deriv(TypeKind::Function)
                  .TySpec.Parameter().basis("int", NamedTypeKind::Builtin, BuiltinTypeKind::Int)
                  .TySpec._AtParam_().deriv(TypeKind::Pointer))
         .binding(DeclSummary()
                  .Value("y", ValueKind::Parameter, ScopeKind::Block)
                  .TySpec.basis("int", NamedTypeKind::Builtin, BuiltinTypeKind::Int)
                  .TySpec.deriv(TypeKind::Pointer)));
}

void BinderTest::case0153()
{
    bind("void x ( int * y , double * z) { }",
         Expectation()
         .binding(DeclSummary()
                  .Function("x", ScopeKind::File)
                  .TySpec.basis("void", NamedTypeKind::Builtin, BuiltinTypeKind::Void)
                  .TySpec.deriv(TypeKind::Function)
                  .TySpec.Parameter().basis("int", NamedTypeKind::Builtin, BuiltinTypeKind::Int)
                  .TySpec._AtParam_().deriv(TypeKind::Pointer)
                  .TySpec.Parameter().basis("double", NamedTypeKind::Builtin, BuiltinTypeKind::Double)
                  .TySpec._AtParam_().deriv(TypeKind::Pointer))
         .binding(DeclSummary()
                  .Value("y", ValueKind::Parameter, ScopeKind::Block)
                  .TySpec.basis("int", NamedTypeKind::Builtin, BuiltinTypeKind::Int)
                  .TySpec.deriv(TypeKind::Pointer))
         .binding(DeclSummary()
                  .Value("z", ValueKind::Parameter, ScopeKind::Block)
                  .TySpec.basis("double", NamedTypeKind::Builtin, BuiltinTypeKind::Double)
                  .TySpec.deriv(TypeKind::Pointer)));
}

void BinderTest::case0154()
{
    bind("void x ( int * * y , double * * z) { }",
         Expectation()
         .binding(DeclSummary()
                  .Function("x", ScopeKind::File)
                  .TySpec.basis("void", NamedTypeKind::Builtin, BuiltinTypeKind::Void)
                  .TySpec.deriv(TypeKind::Function)
                  .TySpec.Parameter().basis("int", NamedTypeKind::Builtin, BuiltinTypeKind::Int)
                  .TySpec._AtParam_().deriv(TypeKind::Pointer)
                  .TySpec._AtParam_().deriv(TypeKind::Pointer)
                  .TySpec.Parameter().basis("double", NamedTypeKind::Builtin, BuiltinTypeKind::Double)
                  .TySpec._AtParam_().deriv(TypeKind::Pointer)
                  .TySpec._AtParam_().deriv(TypeKind::Pointer))
         .binding(DeclSummary()
                  .Value("y", ValueKind::Parameter, ScopeKind::Block)
                  .TySpec.basis("int", NamedTypeKind::Builtin, BuiltinTypeKind::Int)
                  .TySpec.deriv(TypeKind::Pointer)
                  .TySpec.deriv(TypeKind::Pointer))
         .binding(DeclSummary()
                  .Value("z", ValueKind::Parameter, ScopeKind::Block)
                  .TySpec.basis("double", NamedTypeKind::Builtin, BuiltinTypeKind::Double)
                  .TySpec.deriv(TypeKind::Pointer)
                  .TySpec.deriv(TypeKind::Pointer)));
}

void BinderTest::case0155()
{
    bind("void x ( int * * y , double z) { }",
         Expectation()
         .binding(DeclSummary()
                  .Function("x", ScopeKind::File)
                  .TySpec.basis("void", NamedTypeKind::Builtin, BuiltinTypeKind::Void)
                  .TySpec.deriv(TypeKind::Function)
                  .TySpec.Parameter().basis("int", NamedTypeKind::Builtin, BuiltinTypeKind::Int)
                  .TySpec._AtParam_().deriv(TypeKind::Pointer)
                  .TySpec._AtParam_().deriv(TypeKind::Pointer)
                  .TySpec.Parameter().basis("double", NamedTypeKind::Builtin, BuiltinTypeKind::Double))
         .binding(DeclSummary()
                  .Value("y", ValueKind::Parameter, ScopeKind::Block)
                  .TySpec.basis("int", NamedTypeKind::Builtin, BuiltinTypeKind::Int)
                  .TySpec.deriv(TypeKind::Pointer)
                  .TySpec.deriv(TypeKind::Pointer))
         .binding(DeclSummary()
                  .Value("z", ValueKind::Parameter, ScopeKind::Block)
                  .TySpec.basis("double", NamedTypeKind::Builtin, BuiltinTypeKind::Double)));
}

void BinderTest::case0156()
{
    bind("void x ( int y , double * * z) { }",
         Expectation()
         .binding(DeclSummary()
                  .Function("x", ScopeKind::File)
                  .TySpec.basis("void", NamedTypeKind::Builtin, BuiltinTypeKind::Void)
                  .TySpec.deriv(TypeKind::Function)
                  .TySpec.Parameter().basis("int", NamedTypeKind::Builtin, BuiltinTypeKind::Int)
                  .TySpec.Parameter().basis("double", NamedTypeKind::Builtin, BuiltinTypeKind::Double)
                  .TySpec._AtParam_().deriv(TypeKind::Pointer)
                  .TySpec._AtParam_().deriv(TypeKind::Pointer))
         .binding(DeclSummary()
                  .Value("y", ValueKind::Parameter, ScopeKind::Block)
                  .TySpec.basis("int", NamedTypeKind::Builtin, BuiltinTypeKind::Int))
         .binding(DeclSummary()
                  .Value("z", ValueKind::Parameter, ScopeKind::Block)
                  .TySpec.basis("double", NamedTypeKind::Builtin, BuiltinTypeKind::Double)
                  .TySpec.deriv(TypeKind::Pointer)
                  .TySpec.deriv(TypeKind::Pointer)));
}

void BinderTest::case0157()
{
    bind("void x ( y z , w * * v ) { }",
         Expectation()
         .binding(DeclSummary()
                  .Function("x", ScopeKind::File)
                  .TySpec.basis("void", NamedTypeKind::Builtin, BuiltinTypeKind::Void)
                  .TySpec.deriv(TypeKind::Function)
                  .TySpec.Parameter().basis("y", NamedTypeKind::Synonym)
                  .TySpec.Parameter().basis("w", NamedTypeKind::Synonym)
                  .TySpec._AtParam_().deriv(TypeKind::Pointer)
                  .TySpec._AtParam_().deriv(TypeKind::Pointer))
         .binding(DeclSummary()
                  .Value("z", ValueKind::Parameter, ScopeKind::Block)
                  .TySpec.basis("y", NamedTypeKind::Synonym))
         .binding(DeclSummary()
                  .Value("v", ValueKind::Parameter, ScopeKind::Block)
                  .TySpec.basis("w", NamedTypeKind::Synonym)
                  .TySpec.deriv(TypeKind::Pointer)
                  .TySpec.deriv(TypeKind::Pointer)));
}

void BinderTest::case0158()
{
    bind("x y ( z const * w , u * v) { }",
         Expectation()
         .binding(DeclSummary()
                  .Function("y", ScopeKind::File)
                  .TySpec.basis("x", NamedTypeKind::Synonym)
                  .TySpec.deriv(TypeKind::Function)
                  .TySpec.Parameter().basis("z", NamedTypeKind::Synonym, BuiltinTypeKind::UNSPECIFIED, CVR::Const)
                  .TySpec._AtParam_().deriv(TypeKind::Pointer)
                  .TySpec.Parameter().basis("u", NamedTypeKind::Synonym)
                  .TySpec._AtParam_().deriv(TypeKind::Pointer))
         .binding(DeclSummary()
                  .Value("w", ValueKind::Parameter, ScopeKind::Block)
                  .TySpec.basis("z", NamedTypeKind::Synonym, BuiltinTypeKind::UNSPECIFIED, CVR::Const)
                  .TySpec.deriv(TypeKind::Pointer))
         .binding(DeclSummary()
                  .Value("v", ValueKind::Parameter, ScopeKind::Block)
                  .TySpec.basis("u", NamedTypeKind::Synonym)
                  .TySpec.deriv(TypeKind::Pointer)));
}

void BinderTest::case0159()
{
    bind("x y ( z * const w , u * v) { }",
         Expectation()
         .binding(DeclSummary()
                  .Function("y", ScopeKind::File)
                  .TySpec.basis("x", NamedTypeKind::Synonym)
                  .TySpec.deriv(TypeKind::Function)
                  .TySpec.Parameter().basis("z", NamedTypeKind::Synonym)
                  .TySpec._AtParam_().deriv(TypeKind::Pointer, CVR::Const)
                  .TySpec.Parameter().basis("u", NamedTypeKind::Synonym)
                  .TySpec._AtParam_().deriv(TypeKind::Pointer))
         .binding(DeclSummary()
                  .Value("w", ValueKind::Parameter, ScopeKind::Block)
                  .TySpec.basis("z", NamedTypeKind::Synonym)
                  .TySpec.deriv(TypeKind::Pointer, CVR::Const))
         .binding(DeclSummary()
                  .Value("v", ValueKind::Parameter, ScopeKind::Block)
                  .TySpec.basis("u", NamedTypeKind::Synonym)
                  .TySpec.deriv(TypeKind::Pointer)));
}

void BinderTest::case0160()
{
    bind("x * y ( z w) { }",
         Expectation()
         .binding(DeclSummary()
                  .Function("y", ScopeKind::File)
                  .TySpec.basis("x", NamedTypeKind::Synonym)
                  .TySpec.deriv(TypeKind::Pointer)
                  .TySpec.deriv(TypeKind::Function)
                  .TySpec.Parameter().basis("z", NamedTypeKind::Synonym))
         .binding(DeclSummary()
                  .Value("w", ValueKind::Parameter, ScopeKind::Block)
                  .TySpec.basis("z", NamedTypeKind::Synonym)));
}

void BinderTest::case0161() {}
void BinderTest::case0162() {}
void BinderTest::case0163() {}
void BinderTest::case0164() {}
void BinderTest::case0165() {}
void BinderTest::case0166() {}
void BinderTest::case0167() {}
void BinderTest::case0168() {}
void BinderTest::case0169() {}
void BinderTest::case0170() {}
void BinderTest::case0171() {}
void BinderTest::case0172() {}
void BinderTest::case0173() {}
void BinderTest::case0174() {}
void BinderTest::case0175() {}
void BinderTest::case0176() {}
void BinderTest::case0177() {}
void BinderTest::case0178() {}
void BinderTest::case0179() {}
void BinderTest::case0180() {}
void BinderTest::case0181() {}
void BinderTest::case0182() {}
void BinderTest::case0183() {}
void BinderTest::case0184() {}
void BinderTest::case0185() {}
void BinderTest::case0186() {}
void BinderTest::case0187() {}
void BinderTest::case0188() {}
void BinderTest::case0189() {}
void BinderTest::case0190() {}
void BinderTest::case0191() {}
void BinderTest::case0192() {}
void BinderTest::case0193() {}
void BinderTest::case0194() {}
void BinderTest::case0195() {}
void BinderTest::case0196() {}
void BinderTest::case0197() {}
void BinderTest::case0198() {}
void BinderTest::case0199() {}

void BinderTest::case0200()
{
    bind("void x ( int ) ;",
         Expectation()
         .binding(DeclSummary()
                  .Function("x", ScopeKind::File)
                  .TySpec.basis("void", NamedTypeKind::Builtin, BuiltinTypeKind::Void)
                  .TySpec.deriv(TypeKind::Function)
                  .TySpec.Parameter().basis("int", NamedTypeKind::Builtin, BuiltinTypeKind::Int))
         .binding(DeclSummary()
                  .Value("", ValueKind::Parameter, ScopeKind::FunctionPrototype)
                  .TySpec.basis("int", NamedTypeKind::Builtin, BuiltinTypeKind::Int)));
}

void BinderTest::case0201()
{
    bind("void x ( y ) ;",
         Expectation()
         .binding(DeclSummary()
                  .Function("x", ScopeKind::File)
                  .TySpec.basis("void", NamedTypeKind::Builtin, BuiltinTypeKind::Void)
                  .TySpec.deriv(TypeKind::Function)
                  .TySpec.Parameter().basis("y", NamedTypeKind::Synonym))
         .binding(DeclSummary()
                  .Value("", ValueKind::Parameter, ScopeKind::FunctionPrototype)
                  .TySpec.basis("y", NamedTypeKind::Synonym)));
}

void BinderTest::case0202()
{
    bind("void x ( int , y ) ;",
         Expectation()
         .binding(DeclSummary()
                  .Function("x", ScopeKind::File)
                  .TySpec.basis("void", NamedTypeKind::Builtin, BuiltinTypeKind::Void)
                  .TySpec.deriv(TypeKind::Function)
                  .TySpec.Parameter().basis("int", NamedTypeKind::Builtin, BuiltinTypeKind::Int)
                  .TySpec.Parameter().basis("y", NamedTypeKind::Synonym))
         .binding(DeclSummary()
                  .Value("", ValueKind::Parameter, ScopeKind::FunctionPrototype)
                  .TySpec.basis("int", NamedTypeKind::Builtin, BuiltinTypeKind::Int))
         .binding(DeclSummary()
                  .Value("", ValueKind::Parameter, ScopeKind::FunctionPrototype)
                  .TySpec.basis("y", NamedTypeKind::Synonym)));
}

void BinderTest::case0203()
{
    bind("void x ( y , int ) ;",
         Expectation()
         .binding(DeclSummary()
                  .Function("x", ScopeKind::File)
                  .TySpec.basis("void", NamedTypeKind::Builtin, BuiltinTypeKind::Void)
                  .TySpec.deriv(TypeKind::Function)
                  .TySpec.Parameter().basis("y", NamedTypeKind::Synonym)
                  .TySpec.Parameter().basis("int", NamedTypeKind::Builtin, BuiltinTypeKind::Int))
         .binding(DeclSummary()
                  .Value("", ValueKind::Parameter, ScopeKind::FunctionPrototype)
                  .TySpec.basis("int", NamedTypeKind::Builtin, BuiltinTypeKind::Int))
         .binding(DeclSummary()
                  .Value("", ValueKind::Parameter, ScopeKind::FunctionPrototype)
                  .TySpec.basis("y", NamedTypeKind::Synonym)));
}

void BinderTest::case0204()
{
    bind("void x ( y , z ) ;",
         Expectation()
         .binding(DeclSummary()
                  .Function("x", ScopeKind::File)
                  .TySpec.basis("void", NamedTypeKind::Builtin, BuiltinTypeKind::Void)
                  .TySpec.deriv(TypeKind::Function)
                  .TySpec.Parameter().basis("y", NamedTypeKind::Synonym)
                  .TySpec.Parameter().basis("z", NamedTypeKind::Synonym))
         .binding(DeclSummary()
                  .Value("", ValueKind::Parameter, ScopeKind::FunctionPrototype)
                  .TySpec.basis("z", NamedTypeKind::Synonym))
         .binding(DeclSummary()
                  .Value("", ValueKind::Parameter, ScopeKind::FunctionPrototype)
                  .TySpec.basis("y", NamedTypeKind::Synonym)));

}

void BinderTest::case0205()
{
    bind("void x ( int * ) ;",
         Expectation()
         .binding(DeclSummary()
                  .Function("x", ScopeKind::File)
                  .TySpec.basis("void", NamedTypeKind::Builtin, BuiltinTypeKind::Void)
                  .TySpec.deriv(TypeKind::Function)
                  .TySpec.Parameter().basis("int", NamedTypeKind::Builtin, BuiltinTypeKind::Int)
                  .TySpec._AtParam_().deriv(TypeKind::Pointer))
         .binding(DeclSummary()
                  .Value("", ValueKind::Parameter, ScopeKind::FunctionPrototype)
                  .TySpec.basis("int", NamedTypeKind::Builtin, BuiltinTypeKind::Int)
                  .TySpec.deriv(TypeKind::Pointer)));
}

void BinderTest::case0206()
{
    bind("void x ( int [ ] ) ;",
         Expectation()
         .binding(DeclSummary()
                  .Function("x", ScopeKind::File)
                  .TySpec.basis("void", NamedTypeKind::Builtin, BuiltinTypeKind::Void)
                  .TySpec.deriv(TypeKind::Function)
                  .TySpec.Parameter().basis("int", NamedTypeKind::Builtin, BuiltinTypeKind::Int)
                  .TySpec._AtParam_().deriv(TypeKind::Pointer, CVR::None, Decay::FromArrayToPointer))
         .binding(DeclSummary()
                  .Value("", ValueKind::Parameter, ScopeKind::FunctionPrototype)
                  .TySpec.basis("int", NamedTypeKind::Builtin, BuiltinTypeKind::Int)
                  .TySpec.deriv(TypeKind::Pointer, CVR::None, Decay::FromArrayToPointer)));
}

void BinderTest::case0207()
{
    bind("void x ( int [ 1 ] ) ;",
         Expectation()
         .binding(DeclSummary()
                  .Function("x", ScopeKind::File)
                  .TySpec.basis("void", NamedTypeKind::Builtin, BuiltinTypeKind::Void)
                  .TySpec.deriv(TypeKind::Function)
                  .TySpec.Parameter().basis("int", NamedTypeKind::Builtin, BuiltinTypeKind::Int)
                  .TySpec._AtParam_().deriv(TypeKind::Pointer, CVR::None, Decay::FromArrayToPointer))
         .binding(DeclSummary()
                  .Value("", ValueKind::Parameter, ScopeKind::FunctionPrototype)
                  .TySpec.basis("int", NamedTypeKind::Builtin, BuiltinTypeKind::Int)
                  .TySpec.deriv(TypeKind::Pointer, CVR::None, Decay::FromArrayToPointer)));
}

void BinderTest::case0208(){}
void BinderTest::case0209(){}
void BinderTest::case0210(){}
void BinderTest::case0211(){}
void BinderTest::case0212(){}
void BinderTest::case0213(){}
void BinderTest::case0214(){}
void BinderTest::case0215(){}
void BinderTest::case0216(){}
void BinderTest::case0217(){}
void BinderTest::case0218(){}
void BinderTest::case0219(){}
void BinderTest::case0220(){}
void BinderTest::case0221(){}
void BinderTest::case0222(){}
void BinderTest::case0223(){}
void BinderTest::case0224(){}
void BinderTest::case0225(){}
void BinderTest::case0226(){}
void BinderTest::case0227(){}
void BinderTest::case0228(){}
void BinderTest::case0229(){}
void BinderTest::case0230(){}
void BinderTest::case0231(){}
void BinderTest::case0232(){}
void BinderTest::case0233(){}
void BinderTest::case0234(){}
void BinderTest::case0235(){}
void BinderTest::case0236(){}
void BinderTest::case0237(){}
void BinderTest::case0238(){}
void BinderTest::case0239(){}
void BinderTest::case0240(){}

void BinderTest::case0241(){}
void BinderTest::case0242(){}
void BinderTest::case0243(){}
void BinderTest::case0244(){}
void BinderTest::case0245(){}
void BinderTest::case0246(){}
void BinderTest::case0247(){}
void BinderTest::case0248(){}
void BinderTest::case0249(){}
void BinderTest::case0250(){}
void BinderTest::case0251(){}
void BinderTest::case0252(){}
void BinderTest::case0253(){}
void BinderTest::case0254(){}
void BinderTest::case0255(){}
void BinderTest::case0256(){}
void BinderTest::case0257(){}
void BinderTest::case0258(){}
void BinderTest::case0259(){}
void BinderTest::case0260(){}
void BinderTest::case0261(){}
void BinderTest::case0262(){}
void BinderTest::case0263(){}
void BinderTest::case0264(){}
void BinderTest::case0265(){}
void BinderTest::case0266(){}
void BinderTest::case0267(){}
void BinderTest::case0268(){}
void BinderTest::case0269(){}
void BinderTest::case0270(){}
void BinderTest::case0271(){}
void BinderTest::case0272(){}
void BinderTest::case0273(){}
void BinderTest::case0274(){}
void BinderTest::case0275(){}
void BinderTest::case0276(){}
void BinderTest::case0277(){}
void BinderTest::case0278(){}
void BinderTest::case0279(){}
void BinderTest::case0280(){}
void BinderTest::case0281(){}
void BinderTest::case0282(){}
void BinderTest::case0283(){}
void BinderTest::case0284(){}
void BinderTest::case0285(){}
void BinderTest::case0286(){}
void BinderTest::case0287(){}
void BinderTest::case0288(){}
void BinderTest::case0289(){}
void BinderTest::case0290(){}
void BinderTest::case0291(){}
void BinderTest::case0292(){}
void BinderTest::case0293(){}
void BinderTest::case0294(){}
void BinderTest::case0295(){}
void BinderTest::case0296(){}
void BinderTest::case0297(){}
void BinderTest::case0298(){}
void BinderTest::case0299(){}

void BinderTest::case0300()
{
    bind("void x ( int ( * y ) ( double ) ) ;",
         Expectation()
         .binding(DeclSummary()
                  .Function("x", ScopeKind::File)
                  .TySpec.basis("void", NamedTypeKind::Builtin, BuiltinTypeKind::Void)
                  .TySpec.deriv(TypeKind::Function)
                  .TySpec.Parameter().basis("int", NamedTypeKind::Builtin, BuiltinTypeKind::Int)
                  .TySpec._AtParam_().deriv(TypeKind::Function)
                  .TySpec._AtParam_().Parameter().basis("double", NamedTypeKind::Builtin, BuiltinTypeKind::Double)
                  .TySpec._AtParam_().deriv(TypeKind::Pointer))
         .binding(DeclSummary()
                  .Value("y", ValueKind::Parameter, ScopeKind::FunctionPrototype)
                  .TySpec.basis("int", NamedTypeKind::Builtin, BuiltinTypeKind::Int)
                  .TySpec.deriv(TypeKind::Function)
                  .TySpec.Parameter().basis("double", NamedTypeKind::Builtin, BuiltinTypeKind::Double)
                  .TySpec.deriv(TypeKind::Pointer)));
}

void BinderTest::case0301()
{
    bind("void x ( int ( * y ) ( double ) , char ( * z ) ( float ) ) ;",
         Expectation()
         .binding(DeclSummary()
                  .Function("x", ScopeKind::File)
                  .TySpec.basis("void", NamedTypeKind::Builtin, BuiltinTypeKind::Void)
                  .TySpec.deriv(TypeKind::Function)
                  .TySpec.Parameter().basis("int", NamedTypeKind::Builtin, BuiltinTypeKind::Int)
                  .TySpec._AtParam_().deriv(TypeKind::Function)
                  .TySpec._AtParam_().Parameter().basis("double", NamedTypeKind::Builtin, BuiltinTypeKind::Double)
                  .TySpec._AtParam_().deriv(TypeKind::Pointer)
                  .TySpec.Parameter().basis("char", NamedTypeKind::Builtin, BuiltinTypeKind::Char)
                  .TySpec._AtParam_().deriv(TypeKind::Function)
                  .TySpec._AtParam_().Parameter().basis("float", NamedTypeKind::Builtin, BuiltinTypeKind::Float)
                  .TySpec._AtParam_().deriv(TypeKind::Pointer))
         .binding(DeclSummary()
                  .Value("y", ValueKind::Parameter, ScopeKind::FunctionPrototype)
                  .TySpec.basis("int", NamedTypeKind::Builtin, BuiltinTypeKind::Int)
                  .TySpec.deriv(TypeKind::Function)
                  .TySpec.Parameter().basis("double", NamedTypeKind::Builtin, BuiltinTypeKind::Double)
                  .TySpec.deriv(TypeKind::Pointer))
         .binding(DeclSummary()
                  .Value("z", ValueKind::Parameter, ScopeKind::FunctionPrototype)
                  .TySpec.basis("char", NamedTypeKind::Builtin, BuiltinTypeKind::Char)
                  .TySpec.deriv(TypeKind::Function)
                  .TySpec.Parameter().basis("float", NamedTypeKind::Builtin, BuiltinTypeKind::Float)
                  .TySpec.deriv(TypeKind::Pointer)));
}

void BinderTest::case0302()
{
    bind("void x ( int * ( * y ) ( double ) ) ;",
         Expectation()
         .binding(DeclSummary()
                  .Function("x", ScopeKind::File)
                  .TySpec.basis("void", NamedTypeKind::Builtin, BuiltinTypeKind::Void)
                  .TySpec.deriv(TypeKind::Function)
                  .TySpec.Parameter().basis("int", NamedTypeKind::Builtin, BuiltinTypeKind::Int)
                  .TySpec._AtParam_().deriv(TypeKind::Pointer)
                  .TySpec._AtParam_().deriv(TypeKind::Function)
                  .TySpec._AtParam_().Parameter().basis("double", NamedTypeKind::Builtin, BuiltinTypeKind::Double)
                  .TySpec._AtParam_().deriv(TypeKind::Pointer))
         .binding(DeclSummary()
                  .Value("y", ValueKind::Parameter, ScopeKind::FunctionPrototype)
                  .TySpec.basis("int", NamedTypeKind::Builtin, BuiltinTypeKind::Int)
                  .TySpec.deriv(TypeKind::Pointer)
                  .TySpec.deriv(TypeKind::Function)
                  .TySpec.Parameter().basis("double", NamedTypeKind::Builtin, BuiltinTypeKind::Double)
                  .TySpec.deriv(TypeKind::Pointer)));
}

void BinderTest::case0303()
{
    bind("void x ( int ( * * y ) ( double ) ) ;",
         Expectation()
         .binding(DeclSummary()
                  .Function("x", ScopeKind::File)
                  .TySpec.basis("void", NamedTypeKind::Builtin, BuiltinTypeKind::Void)
                  .TySpec.deriv(TypeKind::Function)
                  .TySpec.Parameter().basis("int", NamedTypeKind::Builtin, BuiltinTypeKind::Int)
                  .TySpec._AtParam_().deriv(TypeKind::Function)
                  .TySpec._AtParam_().Parameter().basis("double", NamedTypeKind::Builtin, BuiltinTypeKind::Double)
                  .TySpec._AtParam_().deriv(TypeKind::Pointer)
                  .TySpec._AtParam_().deriv(TypeKind::Pointer))
         .binding(DeclSummary()
                  .Value("y", ValueKind::Parameter, ScopeKind::FunctionPrototype)
                  .TySpec.basis("int", NamedTypeKind::Builtin, BuiltinTypeKind::Int)
                  .TySpec.deriv(TypeKind::Function)
                  .TySpec.Parameter().basis("double", NamedTypeKind::Builtin, BuiltinTypeKind::Double)
                  .TySpec.deriv(TypeKind::Pointer)
                  .TySpec.deriv(TypeKind::Pointer)));
}

void BinderTest::case0304()
{
    bind("void x ( y ( * z ) ( w ) ) ;",
         Expectation()
         .binding(DeclSummary()
                  .Function("x", ScopeKind::File)
                  .TySpec.basis("void", NamedTypeKind::Builtin, BuiltinTypeKind::Void)
                  .TySpec.deriv(TypeKind::Function)
                  .TySpec.Parameter().basis("y", NamedTypeKind::Synonym)
                  .TySpec._AtParam_().deriv(TypeKind::Function)
                  .TySpec._AtParam_().Parameter().basis("w", NamedTypeKind::Synonym)
                  .TySpec._AtParam_().deriv(TypeKind::Pointer))
         .binding(DeclSummary()
                  .Value("z", ValueKind::Parameter, ScopeKind::FunctionPrototype)
                  .TySpec.basis("y", NamedTypeKind::Synonym)
                  .TySpec.deriv(TypeKind::Function)
                  .TySpec.Parameter().basis("w", NamedTypeKind::Synonym)
                  .TySpec.deriv(TypeKind::Pointer)));
}

void BinderTest::case0305()
{
    bind("void x ( y * * ( * z ) ( w ) ) ;",
         Expectation()
         .binding(DeclSummary()
                  .Function("x", ScopeKind::File)
                  .TySpec.basis("void", NamedTypeKind::Builtin, BuiltinTypeKind::Void)
                  .TySpec.deriv(TypeKind::Function)
                  .TySpec.Parameter().basis("y", NamedTypeKind::Synonym)
                  .TySpec._AtParam_().deriv(TypeKind::Pointer)
                  .TySpec._AtParam_().deriv(TypeKind::Pointer)
                  .TySpec._AtParam_().deriv(TypeKind::Function)
                  .TySpec._AtParam_().Parameter().basis("w", NamedTypeKind::Synonym)
                  .TySpec._AtParam_().deriv(TypeKind::Pointer))
         .binding(DeclSummary()
                  .Value("z", ValueKind::Parameter, ScopeKind::FunctionPrototype)
                  .TySpec.basis("y", NamedTypeKind::Synonym)
                  .TySpec.deriv(TypeKind::Pointer)
                  .TySpec.deriv(TypeKind::Pointer)
                  .TySpec.deriv(TypeKind::Function)
                  .TySpec.Parameter().basis("w", NamedTypeKind::Synonym)
                  .TySpec.deriv(TypeKind::Pointer)));
}

void BinderTest::case0306()
{
    bind("void x ( y * * ( * z ) ( double ) , int * u ) ;",
         Expectation()
         .binding(DeclSummary()
                  .Function("x", ScopeKind::File)
                  .TySpec.basis("void", NamedTypeKind::Builtin, BuiltinTypeKind::Void)
                  .TySpec.deriv(TypeKind::Function)
                  .TySpec.Parameter().basis("y", NamedTypeKind::Synonym)
                  .TySpec._AtParam_().deriv(TypeKind::Pointer)
                  .TySpec._AtParam_().deriv(TypeKind::Pointer)
                  .TySpec._AtParam_().deriv(TypeKind::Function)
                  .TySpec._AtParam_().Parameter().basis("double", NamedTypeKind::Builtin, BuiltinTypeKind::Double)
                  .TySpec._AtParam_().deriv(TypeKind::Pointer)
                  .TySpec.Parameter().basis("int", NamedTypeKind::Builtin, BuiltinTypeKind::Int)
                  .TySpec._AtParam_().deriv(TypeKind::Pointer))
         .binding(DeclSummary()
                  .Value("z", ValueKind::Parameter, ScopeKind::FunctionPrototype)
                  .TySpec.basis("y", NamedTypeKind::Synonym)
                  .TySpec.deriv(TypeKind::Pointer)
                  .TySpec.deriv(TypeKind::Pointer)
                  .TySpec.deriv(TypeKind::Function)
                  .TySpec.Parameter().basis("double", NamedTypeKind::Builtin, BuiltinTypeKind::Double)
                  .TySpec.deriv(TypeKind::Pointer))
         .binding(DeclSummary()
                  .Value("u", ValueKind::Parameter, ScopeKind::FunctionPrototype)
                  .TySpec.basis("int", NamedTypeKind::Builtin, BuiltinTypeKind::Int)
                  .TySpec.deriv(TypeKind::Pointer)));
}

void BinderTest::case0307()
{
    bind("void x ( int ( * y ) ( z ) , int * w ) ;",
         Expectation()
         .binding(DeclSummary()
                  .Function("x", ScopeKind::File)
                  .TySpec.basis("void", NamedTypeKind::Builtin, BuiltinTypeKind::Void)
                  .TySpec.deriv(TypeKind::Function)
                  .TySpec.Parameter().basis("int", NamedTypeKind::Builtin, BuiltinTypeKind::Int)
                  .TySpec._AtParam_().deriv(TypeKind::Function)
                  .TySpec._AtParam_().Parameter().basis("z", NamedTypeKind::Synonym)
                  .TySpec._AtParam_().deriv(TypeKind::Pointer)
                  .TySpec.Parameter().basis("int", NamedTypeKind::Builtin, BuiltinTypeKind::Int)
                  .TySpec._AtParam_().deriv(TypeKind::Pointer))
         .binding(DeclSummary()
                  .Value("w", ValueKind::Parameter, ScopeKind::FunctionPrototype)
                  .TySpec.basis("int", NamedTypeKind::Builtin, BuiltinTypeKind::Int)
                  .TySpec.deriv(TypeKind::Pointer))
         .binding(DeclSummary()
                  .Value("y", ValueKind::Parameter, ScopeKind::FunctionPrototype)
                  .TySpec.basis("int", NamedTypeKind::Builtin, BuiltinTypeKind::Int)
                  .TySpec.deriv(TypeKind::Function)
                  .TySpec.Parameter().basis("z", NamedTypeKind::Synonym)
                  .TySpec.deriv(TypeKind::Pointer)));
}

void BinderTest::case0308()
{
    bind("void x ( int * y , int ( * z ) ( w ) ) ;",
         Expectation()
         .binding(DeclSummary()
                  .Function("x", ScopeKind::File)
                  .TySpec.basis("void", NamedTypeKind::Builtin, BuiltinTypeKind::Void)
                  .TySpec.deriv(TypeKind::Function)
                  .TySpec.Parameter().basis("int", NamedTypeKind::Builtin, BuiltinTypeKind::Int)
                  .TySpec._AtParam_().deriv(TypeKind::Pointer)
                  .TySpec.Parameter().basis("z", NamedTypeKind::Synonym)
                  .TySpec._AtParam_().deriv(TypeKind::Function)
                  .TySpec._AtParam_().basis("int", NamedTypeKind::Builtin, BuiltinTypeKind::Int)
                  .TySpec._AtParam_().Parameter().basis("w", NamedTypeKind::Synonym)
                  .TySpec._AtParam_().deriv(TypeKind::Pointer))
         .binding(DeclSummary()
                  .Value("z", ValueKind::Parameter, ScopeKind::FunctionPrototype)
                  .TySpec.basis("int", NamedTypeKind::Builtin, BuiltinTypeKind::Int)
                  .TySpec.deriv(TypeKind::Function)
                  .TySpec.Parameter().basis("w", NamedTypeKind::Synonym)
                  .TySpec.deriv(TypeKind::Pointer))
         .binding(DeclSummary()
                  .Value("y", ValueKind::Parameter, ScopeKind::FunctionPrototype)
                  .TySpec.basis("int", NamedTypeKind::Builtin, BuiltinTypeKind::Int)
                  .TySpec.deriv(TypeKind::Pointer)));
}

void BinderTest::case0309()
{
    bind("void x ( int ( * y ) ( z * ) , int * w ) ;",
         Expectation()
         .binding(DeclSummary()
                  .Function("x", ScopeKind::File)
                  .TySpec.basis("void", NamedTypeKind::Builtin, BuiltinTypeKind::Void)
                  .TySpec.deriv(TypeKind::Function)
                  .TySpec.Parameter().basis("int", NamedTypeKind::Builtin, BuiltinTypeKind::Int)
                  .TySpec._AtParam_().deriv(TypeKind::Function)
                  .TySpec._AtParam_().Parameter().basis("z", NamedTypeKind::Synonym)
                  .TySpec._AtParam_()._AtParam_().deriv(TypeKind::Pointer)
                  .TySpec._AtParam_().deriv(TypeKind::Pointer)
                  .TySpec.Parameter().basis("int", NamedTypeKind::Builtin, BuiltinTypeKind::Int)
                  .TySpec._AtParam_().deriv(TypeKind::Pointer))
         .binding(DeclSummary()
                  .Value("w", ValueKind::Parameter, ScopeKind::FunctionPrototype)
                  .TySpec.basis("int", NamedTypeKind::Builtin, BuiltinTypeKind::Int)
                  .TySpec.deriv(TypeKind::Pointer))
         .binding(DeclSummary()
                  .Value("y", ValueKind::Parameter, ScopeKind::FunctionPrototype)
                  .TySpec.basis("int", NamedTypeKind::Builtin, BuiltinTypeKind::Int)
                  .TySpec.deriv(TypeKind::Function)
                  .TySpec.Parameter().basis("z", NamedTypeKind::Synonym)
                  .TySpec._AtParam_().deriv(TypeKind::Pointer)
                  .TySpec.deriv(TypeKind::Pointer)));
}

void BinderTest::case0310(){}
void BinderTest::case0311(){}
void BinderTest::case0312(){}
void BinderTest::case0313(){}
void BinderTest::case0314(){}
void BinderTest::case0315(){}
void BinderTest::case0316(){}
void BinderTest::case0317(){}
void BinderTest::case0318(){}
void BinderTest::case0319(){}
void BinderTest::case0320(){}
void BinderTest::case0321(){}
void BinderTest::case0322(){}
void BinderTest::case0323(){}
void BinderTest::case0324(){}
void BinderTest::case0325(){}
void BinderTest::case0326(){}
void BinderTest::case0327(){}
void BinderTest::case0328(){}
void BinderTest::case0329(){}
void BinderTest::case0330(){}
void BinderTest::case0331(){}
void BinderTest::case0332(){}
void BinderTest::case0333(){}
void BinderTest::case0334(){}

void BinderTest::case0335()
{
    bind("void x ( int y ( double ) ) ;",
         Expectation()
         .binding(DeclSummary()
                  .Function("x", ScopeKind::File)
                  .TySpec.basis("void", NamedTypeKind::Builtin, BuiltinTypeKind::Void)
                  .TySpec.deriv(TypeKind::Function)
                  .TySpec.Parameter().basis("int", NamedTypeKind::Builtin, BuiltinTypeKind::Int)
                  .TySpec._AtParam_().deriv(TypeKind::Function)
                  .TySpec._AtParam_().Parameter().basis("double", NamedTypeKind::Builtin, BuiltinTypeKind::Double)
                  .TySpec._AtParam_().deriv(TypeKind::Pointer, CVR::None, Decay::FromFunctionToFunctionPointer))
         .binding(DeclSummary()
                  .Value("y", ValueKind::Parameter, ScopeKind::FunctionPrototype)
                  .TySpec.basis("int", NamedTypeKind::Builtin, BuiltinTypeKind::Int)
                  .TySpec.deriv(TypeKind::Function)
                  .TySpec.Parameter().basis("double", NamedTypeKind::Builtin, BuiltinTypeKind::Double)
                  .TySpec.deriv(TypeKind::Pointer, CVR::None, Decay::FromFunctionToFunctionPointer)));
}

void BinderTest::case0336(){}
void BinderTest::case0337(){}
void BinderTest::case0338(){}
void BinderTest::case0339(){}
void BinderTest::case0340(){}
void BinderTest::case0341(){}
void BinderTest::case0342(){}
void BinderTest::case0343(){}
void BinderTest::case0344(){}
void BinderTest::case0345(){}
void BinderTest::case0346(){}
void BinderTest::case0347(){}
void BinderTest::case0348(){}
void BinderTest::case0349(){}

void BinderTest::case0350()
{
    bind("void x ( int ( * y ) [ ] ) ;",
         Expectation()
         .binding(DeclSummary()
                  .Function("x", ScopeKind::File)
                  .TySpec.basis("void", NamedTypeKind::Builtin, BuiltinTypeKind::Void)
                  .TySpec.deriv(TypeKind::Function)
                  .TySpec.Parameter().basis("int", NamedTypeKind::Builtin, BuiltinTypeKind::Int)
                  .TySpec._AtParam_().deriv(TypeKind::Array)
                  .TySpec._AtParam_().deriv(TypeKind::Pointer))
         .binding(DeclSummary()
                  .Value("y", ValueKind::Parameter, ScopeKind::FunctionPrototype)
                  .TySpec.basis("int", NamedTypeKind::Builtin, BuiltinTypeKind::Int)
                  .TySpec.deriv(TypeKind::Array)
                  .TySpec.deriv(TypeKind::Pointer)));
}

void BinderTest::case0351()
{
    bind("void x ( int ( * y ) [ 1 ] ) ;",
         Expectation()
         .binding(DeclSummary()
                  .Function("x", ScopeKind::File)
                  .TySpec.basis("void", NamedTypeKind::Builtin, BuiltinTypeKind::Void)
                  .TySpec.deriv(TypeKind::Function)
                  .TySpec.Parameter().basis("int", NamedTypeKind::Builtin, BuiltinTypeKind::Int)
                  .TySpec._AtParam_().deriv(TypeKind::Array)
                  .TySpec._AtParam_().deriv(TypeKind::Pointer))
         .binding(DeclSummary()
                  .Value("y", ValueKind::Parameter, ScopeKind::FunctionPrototype)
                  .TySpec.basis("int", NamedTypeKind::Builtin, BuiltinTypeKind::Int)
                  .TySpec.deriv(TypeKind::Array)
                  .TySpec.deriv(TypeKind::Pointer)));
}

void BinderTest::case0352()
{
    bind("void x ( int * ( * y ) [ 1 ] ) ;",
         Expectation()
         .binding(DeclSummary()
                  .Function("x", ScopeKind::File)
                  .TySpec.basis("void", NamedTypeKind::Builtin, BuiltinTypeKind::Void)
                  .TySpec.deriv(TypeKind::Function)
                  .TySpec.Parameter().basis("int", NamedTypeKind::Builtin, BuiltinTypeKind::Int)
                  .TySpec._AtParam_().deriv(TypeKind::Pointer)
                  .TySpec._AtParam_().deriv(TypeKind::Array)
                  .TySpec._AtParam_().deriv(TypeKind::Pointer))
         .binding(DeclSummary()
                  .Value("y", ValueKind::Parameter, ScopeKind::FunctionPrototype)
                  .TySpec.basis("int", NamedTypeKind::Builtin, BuiltinTypeKind::Int)
                  .TySpec.deriv(TypeKind::Pointer)
                  .TySpec.deriv(TypeKind::Array)
                  .TySpec.deriv(TypeKind::Pointer)));
}

void BinderTest::case0353()
{
    bind("void x ( y ( * z ) [ ] ) ;",
         Expectation()
         .binding(DeclSummary()
                  .Function("x", ScopeKind::File)
                  .TySpec.basis("void", NamedTypeKind::Builtin, BuiltinTypeKind::Void)
                  .TySpec.deriv(TypeKind::Function)
                  .TySpec.Parameter().basis("y", NamedTypeKind::Synonym)
                  .TySpec._AtParam_().deriv(TypeKind::Array)
                  .TySpec._AtParam_().deriv(TypeKind::Pointer))
         .binding(DeclSummary()
                  .Value("z", ValueKind::Parameter, ScopeKind::FunctionPrototype)
                  .TySpec.basis("y", NamedTypeKind::Synonym)
                  .TySpec.deriv(TypeKind::Array)
                  .TySpec.deriv(TypeKind::Pointer)));
}

void BinderTest::case0354()
{
    bind("void x ( y * ( * z ) [ ] ) ;",
         Expectation()
         .binding(DeclSummary()
                  .Function("x", ScopeKind::File)
                  .TySpec.basis("void", NamedTypeKind::Builtin, BuiltinTypeKind::Void)
                  .TySpec.deriv(TypeKind::Function)
                  .TySpec.Parameter().basis("y", NamedTypeKind::Synonym)
                  .TySpec._AtParam_().deriv(TypeKind::Pointer)
                  .TySpec._AtParam_().deriv(TypeKind::Array)
                  .TySpec._AtParam_().deriv(TypeKind::Pointer))
         .binding(DeclSummary()
                  .Value("z", ValueKind::Parameter, ScopeKind::FunctionPrototype)
                  .TySpec.basis("y", NamedTypeKind::Synonym)
                  .TySpec.deriv(TypeKind::Pointer)
                  .TySpec.deriv(TypeKind::Array)
                  .TySpec.deriv(TypeKind::Pointer)));
}

void BinderTest::case0355()
{
    bind("void x ( int w , y * ( * z ) [ 1 ] ) ;",
         Expectation()
         .binding(DeclSummary()
                  .Function("x", ScopeKind::File)
                  .TySpec.basis("void", NamedTypeKind::Builtin, BuiltinTypeKind::Void)
                  .TySpec.deriv(TypeKind::Function)
                  .TySpec.Parameter().basis("int", NamedTypeKind::Builtin, BuiltinTypeKind::Int)
                  .TySpec.Parameter().basis("y", NamedTypeKind::Synonym)
                  .TySpec._AtParam_().deriv(TypeKind::Pointer)
                  .TySpec._AtParam_().deriv(TypeKind::Array)
                  .TySpec._AtParam_().deriv(TypeKind::Pointer))
         .binding(DeclSummary()
                  .Value("w", ValueKind::Parameter, ScopeKind::FunctionPrototype)
                  .TySpec.basis("int", NamedTypeKind::Builtin, BuiltinTypeKind::Int))
         .binding(DeclSummary()
                  .Value("z", ValueKind::Parameter, ScopeKind::FunctionPrototype)
                  .TySpec.basis("y", NamedTypeKind::Synonym)
                  .TySpec.deriv(TypeKind::Pointer)
                  .TySpec.deriv(TypeKind::Array)
                  .TySpec.deriv(TypeKind::Pointer)));
}

void BinderTest::case0356()
{
    bind("void x ( y * ( * z ) [ 1 ] , int w ) ;",
         Expectation()
         .binding(DeclSummary()
                  .Function("x", ScopeKind::File)
                  .TySpec.basis("void", NamedTypeKind::Builtin, BuiltinTypeKind::Void)
                  .TySpec.deriv(TypeKind::Function)
                  .TySpec.Parameter().basis("y", NamedTypeKind::Synonym)
                  .TySpec._AtParam_().deriv(TypeKind::Pointer)
                  .TySpec._AtParam_().deriv(TypeKind::Array)
                  .TySpec._AtParam_().deriv(TypeKind::Pointer)
                  .TySpec.Parameter().basis("int", NamedTypeKind::Builtin, BuiltinTypeKind::Int))
         .binding(DeclSummary()
                  .Value("w", ValueKind::Parameter, ScopeKind::FunctionPrototype)
                  .TySpec.basis("int", NamedTypeKind::Builtin, BuiltinTypeKind::Int))
         .binding(DeclSummary()
                  .Value("z", ValueKind::Parameter, ScopeKind::FunctionPrototype)
                  .TySpec.basis("y", NamedTypeKind::Synonym)
                  .TySpec.deriv(TypeKind::Pointer)
                  .TySpec.deriv(TypeKind::Array)
                  .TySpec.deriv(TypeKind::Pointer)));
}

void BinderTest::case0357()
{
    bind("void x ( v w , y * ( * z ) [ 1 ] ) ;",
          Expectation()
          .binding(DeclSummary()
                   .Function("x", ScopeKind::File)
                   .TySpec.basis("void", NamedTypeKind::Builtin, BuiltinTypeKind::Void)
                   .TySpec.deriv(TypeKind::Function)
                   .TySpec.Parameter().basis("v", NamedTypeKind::Synonym)
                   .TySpec.Parameter().basis("y", NamedTypeKind::Synonym)
                   .TySpec._AtParam_().deriv(TypeKind::Pointer)
                   .TySpec._AtParam_().deriv(TypeKind::Array)
                   .TySpec._AtParam_().deriv(TypeKind::Pointer))
          .binding(DeclSummary()
                   .Value("w", ValueKind::Parameter, ScopeKind::FunctionPrototype)
                   .TySpec.basis("v", NamedTypeKind::Synonym))
          .binding(DeclSummary()
                   .Value("z", ValueKind::Parameter, ScopeKind::FunctionPrototype)
                   .TySpec.basis("y", NamedTypeKind::Synonym)
                   .TySpec.deriv(TypeKind::Pointer)
                   .TySpec.deriv(TypeKind::Array)
                   .TySpec.deriv(TypeKind::Pointer)));
}

void BinderTest::case0358()
{
    bind("void x ( y * ( * z ) [ 1 ] , v w ) ;",
         Expectation()
         .binding(DeclSummary()
                  .Function("x", ScopeKind::File)
                  .TySpec.basis("void", NamedTypeKind::Builtin, BuiltinTypeKind::Void)
                  .TySpec.deriv(TypeKind::Function)
                  .TySpec.Parameter().basis("y", NamedTypeKind::Synonym)
                  .TySpec._AtParam_().deriv(TypeKind::Pointer)
                  .TySpec._AtParam_().deriv(TypeKind::Array)
                  .TySpec._AtParam_().deriv(TypeKind::Pointer)
                  .TySpec.Parameter().basis("v", NamedTypeKind::Synonym))
         .binding(DeclSummary()
                  .Value("w", ValueKind::Parameter, ScopeKind::FunctionPrototype)
                  .TySpec.basis("v", NamedTypeKind::Synonym))
         .binding(DeclSummary()
                  .Value("z", ValueKind::Parameter, ScopeKind::FunctionPrototype)
                  .TySpec.basis("y", NamedTypeKind::Synonym)
                  .TySpec.deriv(TypeKind::Pointer)
                  .TySpec.deriv(TypeKind::Array)
                  .TySpec.deriv(TypeKind::Pointer)));
}

void BinderTest::case0359(){}
void BinderTest::case0360(){}
void BinderTest::case0361(){}
void BinderTest::case0362(){}
void BinderTest::case0363(){}
void BinderTest::case0364(){}
void BinderTest::case0365(){}
void BinderTest::case0366(){}
void BinderTest::case0367(){}
void BinderTest::case0368(){}
void BinderTest::case0369(){}
void BinderTest::case0370(){}
void BinderTest::case0371(){}
void BinderTest::case0372(){}
void BinderTest::case0373(){}
void BinderTest::case0374(){}
void BinderTest::case0375(){}
void BinderTest::case0376(){}
void BinderTest::case0377(){}
void BinderTest::case0378(){}
void BinderTest::case0379(){}
void BinderTest::case0380(){}
void BinderTest::case0381(){}
void BinderTest::case0382(){}
void BinderTest::case0383(){}
void BinderTest::case0384(){}

void BinderTest::case0385()
{
    bind("void x ( int y [ ] ) ;",
         Expectation()
         .binding(DeclSummary()
                  .Function("x", ScopeKind::File)
                  .TySpec.basis("void", NamedTypeKind::Builtin, BuiltinTypeKind::Void)
                  .TySpec.deriv(TypeKind::Function)
                  .TySpec.Parameter().basis("int", NamedTypeKind::Builtin, BuiltinTypeKind::Int)
                  .TySpec._AtParam_().deriv(TypeKind::Pointer, CVR::None, Decay::FromArrayToPointer))
         .binding(DeclSummary()
                  .Value("y", ValueKind::Parameter, ScopeKind::FunctionPrototype)
                  .TySpec.basis("int", NamedTypeKind::Builtin, BuiltinTypeKind::Int)
                  .TySpec.deriv(TypeKind::Pointer, CVR::None, Decay::FromArrayToPointer)));
}

void BinderTest::case0386()
{
    bind("void x ( y z [ ] ) ;",
         Expectation()
         .binding(DeclSummary()
                  .Function("x", ScopeKind::File)
                  .TySpec.basis("void", NamedTypeKind::Builtin, BuiltinTypeKind::Void)
                  .TySpec.deriv(TypeKind::Function)
                  .TySpec.Parameter().basis("y", NamedTypeKind::Synonym)
                  .TySpec._AtParam_().deriv(TypeKind::Pointer, CVR::None, Decay::FromArrayToPointer))
         .binding(DeclSummary()
                  .Value("z", ValueKind::Parameter, ScopeKind::FunctionPrototype)
                  .TySpec.basis("y", NamedTypeKind::Synonym)
                  .TySpec.deriv(TypeKind::Pointer, CVR::None, Decay::FromArrayToPointer)));
}

void BinderTest::case0387()
{
    bind("void x ( int * y [ 1 ] ) ;",
         Expectation()
         .binding(DeclSummary()
                  .Function("x", ScopeKind::File)
                  .TySpec.basis("void", NamedTypeKind::Builtin, BuiltinTypeKind::Void)
                  .TySpec.deriv(TypeKind::Function)
                  .TySpec.Parameter().basis("int", NamedTypeKind::Builtin, BuiltinTypeKind::Int)
                  .TySpec._AtParam_().deriv(TypeKind::Pointer)
                  .TySpec._AtParam_().deriv(TypeKind::Pointer, CVR::None, Decay::FromArrayToPointer))
         .binding(DeclSummary()
                  .Value("y", ValueKind::Parameter, ScopeKind::FunctionPrototype)
                  .TySpec.basis("int", NamedTypeKind::Builtin, BuiltinTypeKind::Int)
                  .TySpec.deriv(TypeKind::Pointer)
                  .TySpec.deriv(TypeKind::Pointer, CVR::None, Decay::FromArrayToPointer)));
}

void BinderTest::case0388()
{
    bind("void x ( y * z [ 1 ] ) ;",
         Expectation()
         .binding(DeclSummary()
                  .Function("x", ScopeKind::File)
                  .TySpec.basis("void", NamedTypeKind::Builtin, BuiltinTypeKind::Void)
                  .TySpec.deriv(TypeKind::Function)
                  .TySpec.Parameter().basis("y", NamedTypeKind::Synonym)
                  .TySpec._AtParam_().deriv(TypeKind::Pointer)
                  .TySpec._AtParam_().deriv(TypeKind::Pointer, CVR::None, Decay::FromArrayToPointer))
         .binding(DeclSummary()
                  .Value("z", ValueKind::Parameter, ScopeKind::FunctionPrototype)
                  .TySpec.basis("y", NamedTypeKind::Synonym)
                  .TySpec.deriv(TypeKind::Pointer)
                  .TySpec.deriv(TypeKind::Pointer, CVR::None, Decay::FromArrayToPointer)));
}

void BinderTest::case0389()
{
    bind("void x ( int * * y [ 1 ] ) ;",
         Expectation()
         .binding(DeclSummary()
                  .Function("x", ScopeKind::File)
                  .TySpec.basis("void", NamedTypeKind::Builtin, BuiltinTypeKind::Void)
                  .TySpec.deriv(TypeKind::Function)
                  .TySpec.Parameter().basis("int", NamedTypeKind::Builtin, BuiltinTypeKind::Int)
                  .TySpec._AtParam_().deriv(TypeKind::Pointer)
                  .TySpec._AtParam_().deriv(TypeKind::Pointer)
                  .TySpec._AtParam_().deriv(TypeKind::Pointer, CVR::None, Decay::FromArrayToPointer))
         .binding(DeclSummary()
                  .Value("y", ValueKind::Parameter, ScopeKind::FunctionPrototype)
                  .TySpec.basis("int", NamedTypeKind::Builtin, BuiltinTypeKind::Int)
                  .TySpec.deriv(TypeKind::Pointer)
                  .TySpec.deriv(TypeKind::Pointer)
                  .TySpec.deriv(TypeKind::Pointer, CVR::None, Decay::FromArrayToPointer)));
}

void BinderTest::case0390()
{
    bind("void x ( y const * z [ 1 ] ) ;",
         Expectation()
         .binding(DeclSummary()
                  .Function("x", ScopeKind::File)
                  .TySpec.basis("void", NamedTypeKind::Builtin, BuiltinTypeKind::Void)
                  .TySpec.deriv(TypeKind::Function)
                  .TySpec.Parameter().basis("y", NamedTypeKind::Synonym, BuiltinTypeKind::UNSPECIFIED, CVR::Const)
                  .TySpec._AtParam_().deriv(TypeKind::Pointer)
                  .TySpec._AtParam_().deriv(TypeKind::Pointer, CVR::None, Decay::FromArrayToPointer))
         .binding(DeclSummary()
                  .Value("z", ValueKind::Parameter, ScopeKind::FunctionPrototype)
                  .TySpec.basis("y", NamedTypeKind::Synonym, BuiltinTypeKind::UNSPECIFIED, CVR::Const)
                  .TySpec.deriv(TypeKind::Pointer)
                  .TySpec.deriv(TypeKind::Pointer, CVR::None, Decay::FromArrayToPointer)));
}

void BinderTest::case0391()
{
    bind("void x ( y z [ ] , w z ) ;",
         Expectation()
         .binding(DeclSummary()
                  .Function("x", ScopeKind::File)
                  .TySpec.basis("void", NamedTypeKind::Builtin, BuiltinTypeKind::Void)
                  .TySpec.deriv(TypeKind::Function)
                  .TySpec.Parameter().basis("y", NamedTypeKind::Synonym)
                  .TySpec._AtParam_().deriv(TypeKind::Pointer, CVR::None, Decay::FromArrayToPointer)
                  .TySpec.Parameter().basis("w", NamedTypeKind::Synonym))
         .binding(DeclSummary()
                  .Value("z", ValueKind::Parameter, ScopeKind::FunctionPrototype)
                  .TySpec.basis("w", NamedTypeKind::Synonym))
         .binding(DeclSummary()
                  .Value("z", ValueKind::Parameter, ScopeKind::FunctionPrototype)
                  .TySpec.basis("y", NamedTypeKind::Synonym)
                  .TySpec.deriv(TypeKind::Pointer, CVR::None, Decay::FromArrayToPointer)));
}

void BinderTest::case0392()
{
    bind("void x ( w z , y z [ ] ) ;",
         Expectation()
         .binding(DeclSummary()
                  .Function("x", ScopeKind::File)
                  .TySpec.basis("void", NamedTypeKind::Builtin, BuiltinTypeKind::Void)
                  .TySpec.deriv(TypeKind::Function)
                  .TySpec.Parameter().basis("w", NamedTypeKind::Synonym)
                  .TySpec.Parameter().basis("y", NamedTypeKind::Synonym)
                  .TySpec._AtParam_().deriv(TypeKind::Pointer, CVR::None, Decay::FromArrayToPointer))
         .binding(DeclSummary()
                  .Value("z", ValueKind::Parameter, ScopeKind::FunctionPrototype)
                  .TySpec.basis("w", NamedTypeKind::Synonym))
         .binding(DeclSummary()
                  .Value("z", ValueKind::Parameter, ScopeKind::FunctionPrototype)
                  .TySpec.basis("y", NamedTypeKind::Synonym)
                  .TySpec.deriv(TypeKind::Pointer, CVR::None, Decay::FromArrayToPointer)));
}

void BinderTest::case0393(){}
void BinderTest::case0394(){}
void BinderTest::case0395(){}
void BinderTest::case0396(){}
void BinderTest::case0397(){}
void BinderTest::case0398(){}
void BinderTest::case0399(){}

void BinderTest::case0400() {}
void BinderTest::case0401() {}
void BinderTest::case0402() {}
void BinderTest::case0403() {}
void BinderTest::case0404() {}
void BinderTest::case0405() {}
void BinderTest::case0406() {}
void BinderTest::case0407() {}
void BinderTest::case0408() {}
void BinderTest::case0409() {}
void BinderTest::case0410() {}
void BinderTest::case0411() {}
void BinderTest::case0412() {}
void BinderTest::case0413() {}
void BinderTest::case0414() {}
void BinderTest::case0415() {}
void BinderTest::case0416() {}
void BinderTest::case0417() {}
void BinderTest::case0418() {}
void BinderTest::case0419() {}
void BinderTest::case0420() {}
void BinderTest::case0421() {}
void BinderTest::case0422() {}
void BinderTest::case0423() {}
void BinderTest::case0424() {}
void BinderTest::case0425() {}
void BinderTest::case0426() {}
void BinderTest::case0427() {}
void BinderTest::case0428() {}
void BinderTest::case0429() {}
void BinderTest::case0430() {}
void BinderTest::case0431() {}
void BinderTest::case0432() {}
void BinderTest::case0433() {}
void BinderTest::case0434() {}
void BinderTest::case0435() {}
void BinderTest::case0436() {}
void BinderTest::case0437() {}
void BinderTest::case0438() {}
void BinderTest::case0439() {}
void BinderTest::case0440() {}
void BinderTest::case0441() {}
void BinderTest::case0442() {}
void BinderTest::case0443() {}
void BinderTest::case0444() {}
void BinderTest::case0445() {}
void BinderTest::case0446() {}
void BinderTest::case0447() {}
void BinderTest::case0448() {}
void BinderTest::case0449() {}
void BinderTest::case0450() {}
void BinderTest::case0451() {}
void BinderTest::case0452() {}
void BinderTest::case0453() {}
void BinderTest::case0454() {}
void BinderTest::case0455() {}
void BinderTest::case0456() {}
void BinderTest::case0457() {}
void BinderTest::case0458() {}
void BinderTest::case0459() {}
void BinderTest::case0460() {}
void BinderTest::case0461() {}
void BinderTest::case0462() {}
void BinderTest::case0463() {}
void BinderTest::case0464() {}
void BinderTest::case0465() {}
void BinderTest::case0466() {}
void BinderTest::case0467() {}
void BinderTest::case0468() {}
void BinderTest::case0469() {}
void BinderTest::case0470() {}
void BinderTest::case0471() {}
void BinderTest::case0472() {}
void BinderTest::case0473() {}
void BinderTest::case0474() {}
void BinderTest::case0475() {}
void BinderTest::case0476() {}
void BinderTest::case0477() {}
void BinderTest::case0478() {}
void BinderTest::case0479() {}
void BinderTest::case0480() {}
void BinderTest::case0481() {}
void BinderTest::case0482() {}
void BinderTest::case0483() {}
void BinderTest::case0484() {}
void BinderTest::case0485() {}
void BinderTest::case0486() {}
void BinderTest::case0487() {}
void BinderTest::case0488() {}
void BinderTest::case0489() {}
void BinderTest::case0490() {}
void BinderTest::case0491() {}
void BinderTest::case0492() {}
void BinderTest::case0493() {}
void BinderTest::case0494() {}
void BinderTest::case0495() {}
void BinderTest::case0496() {}
void BinderTest::case0497() {}
void BinderTest::case0498() {}
void BinderTest::case0499() {}

void BinderTest::case0500() {}
void BinderTest::case0501() {}
void BinderTest::case0502() {}
void BinderTest::case0503() {}
void BinderTest::case0504() {}
void BinderTest::case0505() {}
void BinderTest::case0506() {}
void BinderTest::case0507() {}
void BinderTest::case0508() {}
void BinderTest::case0509() {}
void BinderTest::case0510() {}
void BinderTest::case0511() {}
void BinderTest::case0512() {}
void BinderTest::case0513() {}
void BinderTest::case0514() {}
void BinderTest::case0515() {}
void BinderTest::case0516() {}
void BinderTest::case0517() {}
void BinderTest::case0518() {}
void BinderTest::case0519() {}
void BinderTest::case0520() {}
void BinderTest::case0521() {}
void BinderTest::case0522() {}
void BinderTest::case0523() {}
void BinderTest::case0524() {}
void BinderTest::case0525() {}
void BinderTest::case0526() {}
void BinderTest::case0527() {}
void BinderTest::case0528() {}
void BinderTest::case0529() {}
void BinderTest::case0530() {}
void BinderTest::case0531() {}
void BinderTest::case0532() {}
void BinderTest::case0533() {}
void BinderTest::case0534() {}
void BinderTest::case0535() {}
void BinderTest::case0536() {}
void BinderTest::case0537() {}
void BinderTest::case0538() {}
void BinderTest::case0539() {}
void BinderTest::case0540() {}
void BinderTest::case0541() {}
void BinderTest::case0542() {}
void BinderTest::case0543() {}
void BinderTest::case0544() {}
void BinderTest::case0545() {}
void BinderTest::case0546() {}
void BinderTest::case0547() {}
void BinderTest::case0548() {}
void BinderTest::case0549() {}
void BinderTest::case0550() {}
void BinderTest::case0551() {}
void BinderTest::case0552() {}
void BinderTest::case0553() {}
void BinderTest::case0554() {}
void BinderTest::case0555() {}
void BinderTest::case0556() {}
void BinderTest::case0557() {}
void BinderTest::case0558() {}
void BinderTest::case0559() {}
void BinderTest::case0560() {}
void BinderTest::case0561() {}
void BinderTest::case0562() {}
void BinderTest::case0563() {}
void BinderTest::case0564() {}
void BinderTest::case0565() {}
void BinderTest::case0566() {}
void BinderTest::case0567() {}
void BinderTest::case0568() {}
void BinderTest::case0569() {}
void BinderTest::case0570() {}
void BinderTest::case0571() {}
void BinderTest::case0572() {}
void BinderTest::case0573() {}
void BinderTest::case0574() {}
void BinderTest::case0575() {}
void BinderTest::case0576() {}
void BinderTest::case0577() {}
void BinderTest::case0578() {}
void BinderTest::case0579() {}
void BinderTest::case0580() {}
void BinderTest::case0581() {}
void BinderTest::case0582() {}
void BinderTest::case0583() {}
void BinderTest::case0584() {}
void BinderTest::case0585() {}
void BinderTest::case0586() {}
void BinderTest::case0587() {}
void BinderTest::case0588() {}
void BinderTest::case0589() {}
void BinderTest::case0590() {}
void BinderTest::case0591() {}
void BinderTest::case0592() {}
void BinderTest::case0593() {}
void BinderTest::case0594() {}
void BinderTest::case0595() {}
void BinderTest::case0596() {}
void BinderTest::case0597() {}
void BinderTest::case0598() {}
void BinderTest::case0599() {}






void BinderTest::case0901()
{
    CROSS_REFERENCE_TEST(ParserTest::case0028);

    bind("const int ;");
}

void BinderTest::case0902()
{
    bind("int x ;");
}

void BinderTest::case0903()
{
    bind("void x ( ) ;");
}

void BinderTest::case0904()
{
    bind("int x ( ) ;");
}

void BinderTest::case0905()
{
    CROSS_REFERENCE_TEST(ParserTest::case0021);

    bind("int double x ;");
}

void BinderTest::case0906()
{
    CROSS_REFERENCE_TEST(ParserTest::case0416);

    bind("struct x { y ; } ;");
}

void BinderTest::case0907()
{
    CROSS_REFERENCE_TEST(ParserTest::case0422);

    bind("struct x { int ; } ;");
}

void BinderTest::case0908()
{
    CROSS_REFERENCE_TEST(ParserTest::case0204);

    // Function returning a function.

    bind("void x ( ) ( ) { }");
}

void BinderTest::case0909()
{
    CROSS_REFERENCE_TEST(ParserTest::case0205);

    // Function returning a function.

    bind("void x ( ) ( ) ;");
}

void BinderTest::case0910()
{
    CROSS_REFERENCE_TEST(ParserTest::case0206);

    // Function returning an array.

    bind("int x ( ) [ ] ;");
}

void BinderTest::case0911()
{
    CROSS_REFERENCE_TEST(ParserTest::case0207);

    bind("int ( x ) ( ) [ ] ;");
}

void BinderTest::case0912()
{
    CROSS_REFERENCE_TEST(ParserTest::case0212);

    // 6.9.1-5

    bind("void x ( int ) { }");
}

void BinderTest::case0913()
{
    CROSS_REFERENCE_TEST(ParserTest::case0040);

    bind("int ;");
}

void BinderTest::case0914()
{
    CROSS_REFERENCE_TEST(ParserTest::case0406);

    // Anonymous `struct' declaration without any variable declared
    // with such type.

    bind("struct { int x ; } ;");
}

void BinderTest::case0915()
{
    CROSS_REFERENCE_TEST(ParserTest::case0632);

    bind("void x ( ) { __thread x y ; }");
}

void BinderTest::case0916()
{
    CROSS_REFERENCE_TEST(ParserTest::case0050);

    bind("double _Complex int x ;");
}

void BinderTest::case0917()
{
    CROSS_REFERENCE_TEST(ParserTest::case0052);

    bind("int int x ;");
}

void BinderTest::case0918()
{
    CROSS_REFERENCE_TEST(ParserTest::case0053);

    bind("_Complex _Complex x ;");
}

void BinderTest::case0919()
{
    CROSS_REFERENCE_TEST(ParserTest::case0435);

    bind("int struct x { int y ; } z ;");
}

void BinderTest::case0920()
{
    CROSS_REFERENCE_TEST(ParserTest::case0436);

    bind("struct x struct { int y ; } z ;");
}

void BinderTest::case0921()
{
    CROSS_REFERENCE_TEST(ParserTest::case0437);

    parse("int struct x y ;");
}

void BinderTest::case0922()
{
    CROSS_REFERENCE_TEST(ParserTest::case0438);

    parse("struct x int y ;");
}

void BinderTest::case0923()
{
    CROSS_REFERENCE_TEST(ParserTest::case0737);

    bind("int __attribute__ ( ( ) ) double ;");
}

void BinderTest::case0924()
{
    CROSS_REFERENCE_TEST(ParserTest::case0738);

    bind("x __attribute__ ( ( ) ) int ;");
}

void BinderTest::case0925()
{
    CROSS_REFERENCE_TEST(ParserTest::case0297);

    bind("void x ( int y ) int y ; { }");
}

void BinderTest::case0926()
{
    CROSS_REFERENCE_TEST(ParserTest::case0298);

    bind("int x ( int y ) z y ; { }");
}

void BinderTest::case0927()
{
    bind("*1;");
}

void BinderTest::case0928()
{
    bind("&1;");
}


void BinderTest::case0929(){}
void BinderTest::case0930(){}
void BinderTest::case0931(){}
void BinderTest::case0932(){}
void BinderTest::case0933(){}
void BinderTest::case0934(){}
void BinderTest::case0935(){}
void BinderTest::case0936(){}
void BinderTest::case0937(){}
void BinderTest::case0938(){}
void BinderTest::case0939(){}
void BinderTest::case0940(){}
void BinderTest::case0941(){}
void BinderTest::case0942(){}
void BinderTest::case0943(){}
void BinderTest::case0944(){}
void BinderTest::case0945(){}
void BinderTest::case0946(){}
void BinderTest::case0947(){}
void BinderTest::case0948(){}
void BinderTest::case0949(){}
void BinderTest::case0950(){}
void BinderTest::case0951(){}
void BinderTest::case0952(){}
void BinderTest::case0953(){}
void BinderTest::case0954(){}
void BinderTest::case0955(){}
void BinderTest::case0956(){}
void BinderTest::case0957(){}
void BinderTest::case0958(){}
void BinderTest::case0959(){}
void BinderTest::case0960(){}
void BinderTest::case0961(){}
void BinderTest::case0962(){}
void BinderTest::case0963(){}
void BinderTest::case0964(){}
void BinderTest::case0965(){}
void BinderTest::case0966(){}
void BinderTest::case0967(){}
void BinderTest::case0968(){}
void BinderTest::case0969(){}
void BinderTest::case0970(){}
void BinderTest::case0971(){}
void BinderTest::case0972(){}
void BinderTest::case0973(){}
void BinderTest::case0974(){}
void BinderTest::case0975(){}
void BinderTest::case0976(){}
void BinderTest::case0977(){}
void BinderTest::case0978(){}
void BinderTest::case0979(){}
void BinderTest::case0980(){}
void BinderTest::case0981(){}
void BinderTest::case0982(){}
void BinderTest::case0983(){}
void BinderTest::case0984(){}
void BinderTest::case0985(){}
void BinderTest::case0986(){}
void BinderTest::case0987(){}
void BinderTest::case0988(){}
void BinderTest::case0989(){}
void BinderTest::case0990(){}
void BinderTest::case0991(){}
void BinderTest::case0992(){}
void BinderTest::case0993(){}
void BinderTest::case0994(){}
void BinderTest::case0995(){}
void BinderTest::case0996(){}
void BinderTest::case0997(){}
void BinderTest::case0998(){}
void BinderTest::case0999(){}
