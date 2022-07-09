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

#include "BinderTester.h"
#include "ParserTester.h"

#include "binder/ConstraintsInDeclarators.h"
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

void BinderTester::case0001()
{
    bind("void x ( ) ;",
         Expectation()
         .binding(DeclSummary()
                  .Function("x", ScopeKind::File)
                  .TySpec.deriv(TypeKind::Function)
                  .TySpec.basis("void", NamedTypeKind::Builtin, BuiltinTypeKind::Void)));
}

void BinderTester::case0002()
{
    bind("int x ( ) ;",
         Expectation()
         .binding(DeclSummary()
                  .Function("x", ScopeKind::File)
                  .TySpec.basis("int", NamedTypeKind::Builtin, BuiltinTypeKind::Int)
                  .TySpec.deriv(TypeKind::Function)));
}

void BinderTester::case0003()
{
    bind("void * x ( ) ;",
         Expectation()
         .binding(DeclSummary()
                  .Function("x", ScopeKind::File)
                  .TySpec.basis("void", NamedTypeKind::Builtin, BuiltinTypeKind::Void)
                  .TySpec.deriv(TypeKind::Pointer)
                  .TySpec.deriv(TypeKind::Function)));
}

void BinderTester::case0004()
{
    bind("int * x ( ) ;",
         Expectation()
         .binding(DeclSummary()
                  .Function("x", ScopeKind::File)
                  .TySpec.basis("int", NamedTypeKind::Builtin, BuiltinTypeKind::Int)
                  .TySpec.deriv(TypeKind::Pointer)
                  .TySpec.deriv(TypeKind::Function)));
}

void BinderTester::case0005()
{
    bind("x y ( ) ;",
         Expectation()
         .binding(DeclSummary()
                  .Function("y")
                  .TySpec.basis("x", NamedTypeKind::Synonym)
                  .TySpec.deriv(TypeKind::Function)));
}

void BinderTester::case0006()
{
    bind("int * ( x ) ( ) ;",
         Expectation()
         .binding(DeclSummary()
                  .Function("x", ScopeKind::File)
                  .TySpec.basis("int", NamedTypeKind::Builtin, BuiltinTypeKind::Int)
                  .TySpec.deriv(TypeKind::Pointer)
                  .TySpec.deriv(TypeKind::Function)));
}

void BinderTester::case0007()
{
    bind("int * ( ( x ) ) ( ) ;",
         Expectation()
         .binding(DeclSummary()
                  .Function("x", ScopeKind::File)
                  .TySpec.basis("int", NamedTypeKind::Builtin, BuiltinTypeKind::Int)
                  .TySpec.deriv(TypeKind::Pointer)
                  .TySpec.deriv(TypeKind::Function)));
}

void BinderTester::case0008()
{
    bind("int * * x ( ) ;",
         Expectation()
         .binding(DeclSummary().Function("x", ScopeKind::File)
                  .TySpec.basis("int", NamedTypeKind::Builtin, BuiltinTypeKind::Int)
                  .TySpec.deriv(TypeKind::Pointer)
                  .TySpec.deriv(TypeKind::Pointer)
                  .TySpec.deriv(TypeKind::Function)));
}

void BinderTester::case0009()
{
    bind("x * y ( ) ;",
         Expectation()
         .binding(DeclSummary().Function("y")
                  .TySpec.basis("x", NamedTypeKind::Synonym)
                  .TySpec.deriv(TypeKind::Pointer)
                  .TySpec.deriv(TypeKind::Function)));
}

void BinderTester::case0010()
{
}

void BinderTester::case0011()
{
}

void BinderTester::case0012()
{
}

void BinderTester::case0013()
{
}

void BinderTester::case0014()
{
}

void BinderTester::case0015(){}
void BinderTester::case0016(){}
void BinderTester::case0017(){}
void BinderTester::case0018(){}
void BinderTester::case0019(){}
void BinderTester::case0020(){}
void BinderTester::case0021(){}
void BinderTester::case0022(){}
void BinderTester::case0023(){}
void BinderTester::case0024(){}
void BinderTester::case0025(){}
void BinderTester::case0026(){}

void BinderTester::case0027(){}
void BinderTester::case0028(){}
void BinderTester::case0029(){}
void BinderTester::case0030(){}
void BinderTester::case0031(){}
void BinderTester::case0032(){}
void BinderTester::case0033(){}
void BinderTester::case0034(){}
void BinderTester::case0035(){}
void BinderTester::case0036(){}
void BinderTester::case0037(){}
void BinderTester::case0038(){}
void BinderTester::case0039(){}
void BinderTester::case0040(){}
void BinderTester::case0041(){}
void BinderTester::case0042(){}
void BinderTester::case0043(){}
void BinderTester::case0044(){}
void BinderTester::case0045(){}
void BinderTester::case0046(){}
void BinderTester::case0047(){}
void BinderTester::case0048(){}
void BinderTester::case0049(){}

void BinderTester::case0050()
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

void BinderTester::case0051()
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

void BinderTester::case0052()
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

void BinderTester::case0053()
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

void BinderTester::case0054()
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

void BinderTester::case0055()
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

void BinderTester::case0056()
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

void BinderTester::case0057()
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

void BinderTester::case0058()
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

void BinderTester::case0059()
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

void BinderTester::case0060()
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

void BinderTester::case0061(){}
void BinderTester::case0062(){}
void BinderTester::case0063(){}
void BinderTester::case0064(){}
void BinderTester::case0065(){}
void BinderTester::case0066(){}
void BinderTester::case0067(){}
void BinderTester::case0068(){}
void BinderTester::case0069(){}
void BinderTester::case0070(){}
void BinderTester::case0071(){}
void BinderTester::case0072(){}
void BinderTester::case0073(){}
void BinderTester::case0074(){}
void BinderTester::case0075(){}
void BinderTester::case0076(){}
void BinderTester::case0077(){}
void BinderTester::case0078(){}
void BinderTester::case0079(){}
void BinderTester::case0080(){}
void BinderTester::case0081(){}
void BinderTester::case0082(){}
void BinderTester::case0083(){}
void BinderTester::case0084(){}
void BinderTester::case0085(){}
void BinderTester::case0086(){}
void BinderTester::case0087(){}
void BinderTester::case0088(){}
void BinderTester::case0089(){}
void BinderTester::case0090(){}
void BinderTester::case0091(){}
void BinderTester::case0092(){}
void BinderTester::case0093(){}
void BinderTester::case0094(){}
void BinderTester::case0095(){}
void BinderTester::case0096(){}
void BinderTester::case0097(){}
void BinderTester::case0098(){}
void BinderTester::case0099(){}
void BinderTester::case0100(){}

void BinderTester::case0101()
{
    bind("void x ( ) { }",
         Expectation()
         .binding(DeclSummary()
                  .Function("x", ScopeKind::File)
                  .TySpec.basis("void", NamedTypeKind::Builtin, BuiltinTypeKind::Void)
                  .TySpec.deriv(TypeKind::Function)));
}

void BinderTester::case0102()
{
    bind("int x ( ) { return 1 ; }",
         Expectation()
         .binding(DeclSummary()
                  .Function("x", ScopeKind::File)
                  .TySpec.basis("int", NamedTypeKind::Builtin, BuiltinTypeKind::Int)
                  .TySpec.deriv(TypeKind::Function)));
}

void BinderTester::case0103()
{
    bind("void * x ( ) { return 1 ; }",
         Expectation()
         .binding(DeclSummary()
                  .Function("x", ScopeKind::File)
                  .TySpec.basis("void", NamedTypeKind::Builtin, BuiltinTypeKind::Void)
                  .TySpec.deriv(TypeKind::Pointer)
                  .TySpec.deriv(TypeKind::Function)));
}

void BinderTester::case0104()
{
    bind("int * x ( ) { return 1 ; }",
         Expectation()
         .binding(DeclSummary()
                  .Function("x", ScopeKind::File)
                  .TySpec.basis("int", NamedTypeKind::Builtin, BuiltinTypeKind::Int)
                  .TySpec.deriv(TypeKind::Pointer)
                  .TySpec.deriv(TypeKind::Function)));
}

void BinderTester::case0105()
{
    bind("x y ( ) { return 1 ; }",
         Expectation()
         .binding(DeclSummary()
                  .Function("y")
                  .TySpec.basis("x", NamedTypeKind::Synonym)
                  .TySpec.deriv(TypeKind::Function)));
}

void BinderTester::case0106()
{
    bind("int * ( x ) ( ) { return 1 ; }",
         Expectation()
         .binding(DeclSummary()
                  .Function("x", ScopeKind::File)
                  .TySpec.basis("int", NamedTypeKind::Builtin, BuiltinTypeKind::Int)
                  .TySpec.deriv(TypeKind::Pointer)
                  .TySpec.deriv(TypeKind::Function)));
}

void BinderTester::case0107()
{
    bind("int * ( ( x ) ) ( ) { return 1 ; }",
         Expectation()
         .binding(DeclSummary().Function("x", ScopeKind::File)
                  .TySpec.basis("int", NamedTypeKind::Builtin, BuiltinTypeKind::Int)
                  .TySpec.deriv(TypeKind::Pointer)
                  .TySpec.deriv(TypeKind::Function)));
}

void BinderTester::case0108()
{
    bind("int * * x ( ) { return 1 ; }",
         Expectation()
         .binding(DeclSummary().Function("x", ScopeKind::File)
                  .TySpec.basis("int", NamedTypeKind::Builtin, BuiltinTypeKind::Int)
                  .TySpec.deriv(TypeKind::Pointer)
                  .TySpec.deriv(TypeKind::Pointer)
                  .TySpec.deriv(TypeKind::Function)));
}

void BinderTester::case0109()
{
    bind("x * y ( ) { return 1 ; }",
         Expectation()
         .binding(DeclSummary().Function("y")
                  .TySpec.basis("x", NamedTypeKind::Synonym)
                  .TySpec.deriv(TypeKind::Pointer)
                  .TySpec.deriv(TypeKind::Function)));
}

void BinderTester::case0110()
{
}

void BinderTester::case0111()
{
}

void BinderTester::case0112()
{
}

void BinderTester::case0113()
{
}

void BinderTester::case0114()
{
}

void BinderTester::case0115(){}
void BinderTester::case0116(){}
void BinderTester::case0117(){}
void BinderTester::case0118(){}
void BinderTester::case0119(){}
void BinderTester::case0120(){}
void BinderTester::case0121(){}
void BinderTester::case0122(){}
void BinderTester::case0123(){}
void BinderTester::case0124(){}
void BinderTester::case0125(){}
void BinderTester::case0126(){}
void BinderTester::case0127(){}
void BinderTester::case0128(){}
void BinderTester::case0129(){}
void BinderTester::case0130(){}
void BinderTester::case0131(){}
void BinderTester::case0132(){}
void BinderTester::case0133(){}
void BinderTester::case0134(){}
void BinderTester::case0135(){}
void BinderTester::case0136(){}
void BinderTester::case0137(){}
void BinderTester::case0138(){}
void BinderTester::case0139(){}
void BinderTester::case0140(){}
void BinderTester::case0141(){}
void BinderTester::case0142(){}
void BinderTester::case0143(){}
void BinderTester::case0144(){}
void BinderTester::case0145(){}
void BinderTester::case0146(){}
void BinderTester::case0147(){}
void BinderTester::case0148(){}
void BinderTester::case0149(){}

void BinderTester::case0150()
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

void BinderTester::case0151()
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

void BinderTester::case0152()
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

void BinderTester::case0153()
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

void BinderTester::case0154()
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

void BinderTester::case0155()
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

void BinderTester::case0156()
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

void BinderTester::case0157()
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

void BinderTester::case0158()
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

void BinderTester::case0159()
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

void BinderTester::case0160()
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

void BinderTester::case0161(){}
void BinderTester::case0162(){}
void BinderTester::case0163(){}
void BinderTester::case0164(){}
void BinderTester::case0165(){}
void BinderTester::case0166(){}
void BinderTester::case0167(){}
void BinderTester::case0168(){}
void BinderTester::case0169(){}
void BinderTester::case0170(){}
void BinderTester::case0171(){}
void BinderTester::case0172(){}
void BinderTester::case0173(){}
void BinderTester::case0174(){}
void BinderTester::case0175(){}
void BinderTester::case0176(){}
void BinderTester::case0177(){}
void BinderTester::case0178(){}
void BinderTester::case0179(){}
void BinderTester::case0180(){}
void BinderTester::case0181(){}
void BinderTester::case0182(){}
void BinderTester::case0183(){}
void BinderTester::case0184(){}
void BinderTester::case0185(){}
void BinderTester::case0186(){}
void BinderTester::case0187(){}
void BinderTester::case0188(){}
void BinderTester::case0189(){}
void BinderTester::case0190(){}
void BinderTester::case0191(){}
void BinderTester::case0192(){}
void BinderTester::case0193(){}
void BinderTester::case0194(){}
void BinderTester::case0195(){}
void BinderTester::case0196(){}
void BinderTester::case0197(){}
void BinderTester::case0198(){}
void BinderTester::case0199(){}

void BinderTester::case0200()
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

void BinderTester::case0201()
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

void BinderTester::case0202()
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

void BinderTester::case0203()
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

void BinderTester::case0204()
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

void BinderTester::case0205()
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

void BinderTester::case0206()
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

void BinderTester::case0207()
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

void BinderTester::case0208(){}
void BinderTester::case0209(){}
void BinderTester::case0210(){}
void BinderTester::case0211(){}
void BinderTester::case0212(){}
void BinderTester::case0213(){}
void BinderTester::case0214(){}
void BinderTester::case0215(){}
void BinderTester::case0216(){}
void BinderTester::case0217(){}
void BinderTester::case0218(){}
void BinderTester::case0219(){}
void BinderTester::case0220(){}
void BinderTester::case0221(){}
void BinderTester::case0222(){}
void BinderTester::case0223(){}
void BinderTester::case0224(){}
void BinderTester::case0225(){}
void BinderTester::case0226(){}
void BinderTester::case0227(){}
void BinderTester::case0228(){}
void BinderTester::case0229(){}
void BinderTester::case0230(){}
void BinderTester::case0231(){}
void BinderTester::case0232(){}
void BinderTester::case0233(){}
void BinderTester::case0234(){}
void BinderTester::case0235(){}
void BinderTester::case0236(){}
void BinderTester::case0237(){}
void BinderTester::case0238(){}
void BinderTester::case0239(){}
void BinderTester::case0240(){}

void BinderTester::case0241(){}
void BinderTester::case0242(){}
void BinderTester::case0243(){}
void BinderTester::case0244(){}
void BinderTester::case0245(){}
void BinderTester::case0246(){}
void BinderTester::case0247(){}
void BinderTester::case0248(){}
void BinderTester::case0249(){}
void BinderTester::case0250(){}
void BinderTester::case0251(){}
void BinderTester::case0252(){}
void BinderTester::case0253(){}
void BinderTester::case0254(){}
void BinderTester::case0255(){}
void BinderTester::case0256(){}
void BinderTester::case0257(){}
void BinderTester::case0258(){}
void BinderTester::case0259(){}
void BinderTester::case0260(){}
void BinderTester::case0261(){}
void BinderTester::case0262(){}
void BinderTester::case0263(){}
void BinderTester::case0264(){}
void BinderTester::case0265(){}
void BinderTester::case0266(){}
void BinderTester::case0267(){}
void BinderTester::case0268(){}
void BinderTester::case0269(){}
void BinderTester::case0270(){}
void BinderTester::case0271(){}
void BinderTester::case0272(){}
void BinderTester::case0273(){}
void BinderTester::case0274(){}
void BinderTester::case0275(){}
void BinderTester::case0276(){}
void BinderTester::case0277(){}
void BinderTester::case0278(){}
void BinderTester::case0279(){}
void BinderTester::case0280(){}
void BinderTester::case0281(){}
void BinderTester::case0282(){}
void BinderTester::case0283(){}
void BinderTester::case0284(){}
void BinderTester::case0285(){}
void BinderTester::case0286(){}
void BinderTester::case0287(){}
void BinderTester::case0288(){}
void BinderTester::case0289(){}
void BinderTester::case0290(){}
void BinderTester::case0291(){}
void BinderTester::case0292(){}
void BinderTester::case0293(){}
void BinderTester::case0294(){}
void BinderTester::case0295(){}
void BinderTester::case0296(){}
void BinderTester::case0297(){}
void BinderTester::case0298(){}
void BinderTester::case0299(){}

void BinderTester::case0300()
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

void BinderTester::case0301()
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

void BinderTester::case0302()
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

void BinderTester::case0303()
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

void BinderTester::case0304()
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

void BinderTester::case0305()
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

void BinderTester::case0306()
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

void BinderTester::case0307()
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

void BinderTester::case0308()
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

void BinderTester::case0309()
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

void BinderTester::case0310(){}
void BinderTester::case0311(){}
void BinderTester::case0312(){}
void BinderTester::case0313(){}
void BinderTester::case0314(){}
void BinderTester::case0315(){}
void BinderTester::case0316(){}
void BinderTester::case0317(){}
void BinderTester::case0318(){}
void BinderTester::case0319(){}
void BinderTester::case0320(){}
void BinderTester::case0321(){}
void BinderTester::case0322(){}
void BinderTester::case0323(){}
void BinderTester::case0324(){}
void BinderTester::case0325(){}
void BinderTester::case0326(){}
void BinderTester::case0327(){}
void BinderTester::case0328(){}
void BinderTester::case0329(){}
void BinderTester::case0330(){}
void BinderTester::case0331(){}
void BinderTester::case0332(){}
void BinderTester::case0333(){}
void BinderTester::case0334(){}

void BinderTester::case0335()
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

void BinderTester::case0336(){}
void BinderTester::case0337(){}
void BinderTester::case0338(){}
void BinderTester::case0339(){}
void BinderTester::case0340(){}
void BinderTester::case0341(){}
void BinderTester::case0342(){}
void BinderTester::case0343(){}
void BinderTester::case0344(){}
void BinderTester::case0345(){}
void BinderTester::case0346(){}
void BinderTester::case0347(){}
void BinderTester::case0348(){}
void BinderTester::case0349(){}

void BinderTester::case0350()
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

void BinderTester::case0351()
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

void BinderTester::case0352()
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

void BinderTester::case0353()
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

void BinderTester::case0354()
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

void BinderTester::case0355()
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

void BinderTester::case0356()
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

void BinderTester::case0357()
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

void BinderTester::case0358()
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

void BinderTester::case0359(){}
void BinderTester::case0360(){}
void BinderTester::case0361(){}
void BinderTester::case0362(){}
void BinderTester::case0363(){}
void BinderTester::case0364(){}
void BinderTester::case0365(){}
void BinderTester::case0366(){}
void BinderTester::case0367(){}
void BinderTester::case0368(){}
void BinderTester::case0369(){}
void BinderTester::case0370(){}
void BinderTester::case0371(){}
void BinderTester::case0372(){}
void BinderTester::case0373(){}
void BinderTester::case0374(){}
void BinderTester::case0375(){}
void BinderTester::case0376(){}
void BinderTester::case0377(){}
void BinderTester::case0378(){}
void BinderTester::case0379(){}
void BinderTester::case0380(){}
void BinderTester::case0381(){}
void BinderTester::case0382(){}
void BinderTester::case0383(){}
void BinderTester::case0384(){}

void BinderTester::case0385()
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

void BinderTester::case0386()
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

void BinderTester::case0387()
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

void BinderTester::case0388()
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

void BinderTester::case0389()
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

void BinderTester::case0390()
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

void BinderTester::case0391()
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

void BinderTester::case0392()
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

void BinderTester::case0393(){}
void BinderTester::case0394(){}
void BinderTester::case0395(){}
void BinderTester::case0396(){}
void BinderTester::case0397(){}
void BinderTester::case0398(){}
void BinderTester::case0399(){}

void BinderTester::case0400()
{
    bind("void ( * x ( ) ) ( ) ;",
         Expectation()
         .binding(DeclSummary()
                  .Function("x", ScopeKind::File)
                  .TySpec.basis("void", NamedTypeKind::Builtin, BuiltinTypeKind::Void)
                  .TySpec.deriv(TypeKind::Function)
                  .TySpec.deriv(TypeKind::Pointer)
                  .TySpec.NestAsReturn()
                  .TySpec.deriv(TypeKind::Function)));
}

void BinderTester::case0401()
{
    bind("void ( * x ( ) ) ( int ) ;",
         Expectation()
         .binding(DeclSummary()
                  .Function("x", ScopeKind::File)
                  .TySpec.basis("void", NamedTypeKind::Builtin, BuiltinTypeKind::Void)
                  .TySpec.deriv(TypeKind::Function)
                  .TySpec.Parameter().basis("int", NamedTypeKind::Builtin, BuiltinTypeKind::Int)
                  .TySpec.deriv(TypeKind::Pointer)
                  .TySpec.NestAsReturn()
                  .TySpec.deriv(TypeKind::Function)));
}

void BinderTester::case0402()
{
    bind("void ( * x ( int ) ) ( ) ;",
         Expectation()
         .binding(DeclSummary()
                  .Function("x", ScopeKind::File)
                  .TySpec.basis("void", NamedTypeKind::Builtin, BuiltinTypeKind::Void)
                  .TySpec.deriv(TypeKind::Function)
                  .TySpec.deriv(TypeKind::Pointer)
                  .TySpec.NestAsReturn()
                  .TySpec.deriv(TypeKind::Function)
                  .TySpec.Parameter().basis("int", NamedTypeKind::Builtin, BuiltinTypeKind::Int)));
}

void BinderTester::case0403()
{
    bind("void ( * x ( double ) ) ( int ) ;",
         Expectation()
         .binding(DeclSummary()
                  .Function("x", ScopeKind::File)
                  .TySpec.basis("void", NamedTypeKind::Builtin, BuiltinTypeKind::Void)
                  .TySpec.deriv(TypeKind::Function)
                  .TySpec.Parameter().basis("int", NamedTypeKind::Builtin, BuiltinTypeKind::Int)
                  .TySpec.deriv(TypeKind::Pointer)
                  .TySpec.NestAsReturn()
                  .TySpec.deriv(TypeKind::Function)
                  .TySpec.Parameter().basis("double", NamedTypeKind::Builtin, BuiltinTypeKind::Double)));
}

void BinderTester::case0404()
{
    bind("void * ( * x ( double ) ) ( int ) ;",
         Expectation()
         .binding(DeclSummary()
                  .Function("x", ScopeKind::File)
                  .TySpec.basis("void", NamedTypeKind::Builtin, BuiltinTypeKind::Void)
                  .TySpec.deriv(TypeKind::Pointer)
                  .TySpec.deriv(TypeKind::Function)
                  .TySpec.Parameter().basis("int", NamedTypeKind::Builtin, BuiltinTypeKind::Int)
                  .TySpec.deriv(TypeKind::Pointer)
                  .TySpec.NestAsReturn()
                  .TySpec.deriv(TypeKind::Function)
                  .TySpec.Parameter().basis("double", NamedTypeKind::Builtin, BuiltinTypeKind::Double)));
}

void BinderTester::case0405()
{
    bind("void * ( * x ( double , char ) ) ( int , long ) ;",
         Expectation()
         .binding(DeclSummary()
                  .Function("x", ScopeKind::File)
                  .TySpec.basis("void", NamedTypeKind::Builtin, BuiltinTypeKind::Void)
                  .TySpec.deriv(TypeKind::Pointer)
                  .TySpec.deriv(TypeKind::Function)
                  .TySpec.Parameter().basis("int", NamedTypeKind::Builtin, BuiltinTypeKind::Int)
                  .TySpec.Parameter().basis("long", NamedTypeKind::Builtin, BuiltinTypeKind::Long)
                  .TySpec.deriv(TypeKind::Pointer)
                  .TySpec.NestAsReturn()
                  .TySpec.deriv(TypeKind::Function)
                  .TySpec.Parameter().basis("double", NamedTypeKind::Builtin, BuiltinTypeKind::Double)
                  .TySpec.Parameter().basis("char", NamedTypeKind::Builtin, BuiltinTypeKind::Char)));
}

void BinderTester::case0406()
{
    bind("void * ( * x ( double * , char ) ) ( void * ) ;",
         Expectation()
         .binding(DeclSummary()
                  .Function("x", ScopeKind::File)
                  .TySpec.basis("void", NamedTypeKind::Builtin, BuiltinTypeKind::Void)
                  .TySpec.deriv(TypeKind::Pointer)
                  .TySpec.deriv(TypeKind::Function)
                  .TySpec.Parameter().basis("void", NamedTypeKind::Builtin, BuiltinTypeKind::Void)
                  .TySpec._AtParam_().deriv(TypeKind::Pointer)
                  .TySpec.deriv(TypeKind::Pointer)
                  .TySpec.NestAsReturn()
                  .TySpec.deriv(TypeKind::Function)
                  .TySpec.Parameter().basis("double", NamedTypeKind::Builtin, BuiltinTypeKind::Double)
                  .TySpec._AtParam_().deriv(TypeKind::Pointer)
                  .TySpec.Parameter().basis("char", NamedTypeKind::Builtin, BuiltinTypeKind::Char)));
}

void BinderTester::case0407()
{
    bind("void ( * x ( ) ) ( y ) ;",
         Expectation()
         .binding(DeclSummary()
                  .Function("x", ScopeKind::File)
                  .TySpec.basis("void", NamedTypeKind::Builtin, BuiltinTypeKind::Void)
                  .TySpec.deriv(TypeKind::Function)
                  .TySpec.Parameter().basis("y", NamedTypeKind::Synonym)
                  .TySpec.deriv(TypeKind::Pointer)
                  .TySpec.NestAsReturn()
                  .TySpec.deriv(TypeKind::Function)));
}

void BinderTester::case0408()
{
    bind("void ( * x ( y ) ) ( ) ;",
         Expectation()
         .binding(DeclSummary()
                  .Function("x", ScopeKind::File)
                  .TySpec.basis("void", NamedTypeKind::Builtin, BuiltinTypeKind::Void)
                  .TySpec.deriv(TypeKind::Function)
                  .TySpec.deriv(TypeKind::Pointer)
                  .TySpec.NestAsReturn()
                  .TySpec.deriv(TypeKind::Function)
                  .TySpec.Parameter().basis("y", NamedTypeKind::Synonym)));
}

void BinderTester::case0409()
{
    bind("void ( * x ( y ) ) ( z ) ;",
         Expectation()
         .binding(DeclSummary()
                  .Function("x", ScopeKind::File)
                  .TySpec.basis("void", NamedTypeKind::Builtin, BuiltinTypeKind::Void)
                  .TySpec.deriv(TypeKind::Function)
                  .TySpec.Parameter().basis("z", NamedTypeKind::Synonym)
                  .TySpec.deriv(TypeKind::Pointer)
                  .TySpec.NestAsReturn()
                  .TySpec.deriv(TypeKind::Function)
                  .TySpec.Parameter().basis("y", NamedTypeKind::Synonym)));
}

void BinderTester::case0410()
{
    bind("void ( * x ( y * ) ) ( z ) ;",
         Expectation()
         .binding(DeclSummary()
                  .Function("x", ScopeKind::File)
                  .TySpec.basis("void", NamedTypeKind::Builtin, BuiltinTypeKind::Void)
                  .TySpec.deriv(TypeKind::Function)
                  .TySpec.Parameter().basis("z", NamedTypeKind::Synonym)
                  .TySpec.deriv(TypeKind::Pointer)
                  .TySpec.NestAsReturn()
                  .TySpec.deriv(TypeKind::Function)
                  .TySpec.Parameter().basis("y", NamedTypeKind::Synonym)
                  .TySpec._AtParam_().deriv(TypeKind::Pointer)));
}

void BinderTester::case0411()
{
    // The `signal' function: http://c-faq.com/decl/spiral.anderson.html

    bind(" void ( * x ( int , void ( * y ) ( int ) ) ) ( int ) ;",
         Expectation()
         .binding(DeclSummary()
                  .Function("x", ScopeKind::File)
                  .TySpec.basis("void", NamedTypeKind::Builtin, BuiltinTypeKind::Void)
                  .TySpec.deriv(TypeKind::Function)
                  .TySpec.Parameter().basis("int", NamedTypeKind::Builtin, BuiltinTypeKind::Int)
                  .TySpec.deriv(TypeKind::Pointer)
                  .TySpec.NestAsReturn()
                  .TySpec.deriv(TypeKind::Function)
                  .TySpec.Parameter().basis("int", NamedTypeKind::Builtin, BuiltinTypeKind::Int)
                  .TySpec.Parameter().basis("void", NamedTypeKind::Builtin, BuiltinTypeKind::Void)
                  .TySpec._AtParam_().deriv(TypeKind::Function)
                  .TySpec._AtParam_().Parameter().basis("int", NamedTypeKind::Builtin, BuiltinTypeKind::Int)
                  .TySpec._AtParam_().deriv(TypeKind::Pointer)));
}

void BinderTester::case0412()
{
    bind("x ( * y ( ) ) ( ) ;",
         Expectation()
         .binding(DeclSummary()
                  .Function("y", ScopeKind::File)
                  .TySpec.basis("x", NamedTypeKind::Synonym)
                  .TySpec.deriv(TypeKind::Function)
                  .TySpec.deriv(TypeKind::Pointer)
                  .TySpec.NestAsReturn()
                  .TySpec.deriv(TypeKind::Function)));
}
void BinderTester::case0413()
{
    bind("void x ( ) ( ) ;",
         Expectation()
         .diagnostic(Expectation::ErrorOrWarn::Error,
                     ConstraintsInDeclarators::ID_FunctionReturningFunction)
         .ContinueTestDespiteOfErrors()
         .binding(DeclSummary()
                  .Function("x", ScopeKind::File)
                  .TySpec.basis("void", NamedTypeKind::Builtin, BuiltinTypeKind::Void)
                  .TySpec.deriv(TypeKind::Function)
                  .TySpec.NestAsReturn()
                  .TySpec.deriv(TypeKind::Function)));
}

void BinderTester::case0414()
{
    bind("void ( x ( ) ) ( ) ;",
         Expectation()
         .diagnostic(Expectation::ErrorOrWarn::Error,
                     ConstraintsInDeclarators::ID_FunctionReturningFunction)
         .ContinueTestDespiteOfErrors()
         .binding(DeclSummary()
                  .Function("x", ScopeKind::File)
                  .TySpec.basis("void", NamedTypeKind::Builtin, BuiltinTypeKind::Void)
                  .TySpec.deriv(TypeKind::Function)
                  .TySpec.NestAsReturn()
                  .TySpec.deriv(TypeKind::Function)));
}
void BinderTester::case0415(){}
void BinderTester::case0416(){}
void BinderTester::case0417(){}
void BinderTester::case0418(){}
void BinderTester::case0419(){}
void BinderTester::case0420(){}
void BinderTester::case0421(){}
void BinderTester::case0422(){}
void BinderTester::case0423(){}
void BinderTester::case0424(){}

void BinderTester::case0425()
{
    bind("int ( * x ( ) ) [] ;",
         Expectation()
         .binding(DeclSummary()
                  .Function("x", ScopeKind::File)
                  .TySpec.basis("int", NamedTypeKind::Builtin, BuiltinTypeKind::Int)
                  .TySpec.deriv(TypeKind::Array)
                  .TySpec.deriv(TypeKind::Pointer)
                  .TySpec.NestAsReturn()
                  .TySpec.deriv(TypeKind::Function)));
}

void BinderTester::case0426()
{
    bind("x ( * y ( ) ) [] ;",
         Expectation()
         .binding(DeclSummary()
                  .Function("y", ScopeKind::File)
                  .TySpec.basis("x", NamedTypeKind::Synonym)
                  .TySpec.deriv(TypeKind::Array)
                  .TySpec.deriv(TypeKind::Pointer)
                  .TySpec.NestAsReturn()
                  .TySpec.deriv(TypeKind::Function)));
}

void BinderTester::case0427()
{
    bind("int x ( ) [] ;",
         Expectation()
         .diagnostic(Expectation::ErrorOrWarn::Error,
                     ConstraintsInDeclarators::ID_FunctionReturningArray)
         .ContinueTestDespiteOfErrors()
         .binding(DeclSummary()
                  .Function("x", ScopeKind::File)
                  .TySpec.basis("int", NamedTypeKind::Builtin, BuiltinTypeKind::Int)
                  .TySpec.deriv(TypeKind::Array)
                  .TySpec.NestAsReturn()
                  .TySpec.deriv(TypeKind::Function)));
}

void BinderTester::case0428()
{
    bind("int ( x ) ( ) [] ;",
         Expectation()
         .diagnostic(Expectation::ErrorOrWarn::Error,
                     ConstraintsInDeclarators::ID_FunctionReturningArray)
         .ContinueTestDespiteOfErrors()
         .binding(DeclSummary()
                  .Function("x", ScopeKind::File)
                  .TySpec.basis("int", NamedTypeKind::Builtin, BuiltinTypeKind::Int)
                  .TySpec.deriv(TypeKind::Array)
                  .TySpec.NestAsReturn()
                  .TySpec.deriv(TypeKind::Function)));
}

void BinderTester::case0429(){}
void BinderTester::case0430(){}
void BinderTester::case0431(){}
void BinderTester::case0432(){}
void BinderTester::case0433(){}
void BinderTester::case0434(){}
void BinderTester::case0435(){}
void BinderTester::case0436(){}
void BinderTester::case0437(){}
void BinderTester::case0438(){}
void BinderTester::case0439(){}
void BinderTester::case0440()
{
}

void BinderTester::case0441(){}
void BinderTester::case0442(){}
void BinderTester::case0443(){}
void BinderTester::case0444(){}
void BinderTester::case0445(){}
void BinderTester::case0446(){}
void BinderTester::case0447(){}
void BinderTester::case0448(){}
void BinderTester::case0449(){}

void BinderTester::case0450()
{
}

void BinderTester::case0451()
{
}

void BinderTester::case0452(){}
void BinderTester::case0453(){}
void BinderTester::case0454(){}
void BinderTester::case0455(){}
void BinderTester::case0456(){}
void BinderTester::case0457(){}
void BinderTester::case0458(){}
void BinderTester::case0459(){}
void BinderTester::case0460(){}
void BinderTester::case0461(){}
void BinderTester::case0462(){}
void BinderTester::case0463(){}
void BinderTester::case0464(){}
void BinderTester::case0465(){}
void BinderTester::case0466(){}
void BinderTester::case0467(){}
void BinderTester::case0468(){}
void BinderTester::case0469(){}
void BinderTester::case0470(){}
void BinderTester::case0471(){}
void BinderTester::case0472(){}
void BinderTester::case0473(){}
void BinderTester::case0474(){}
void BinderTester::case0475(){}
void BinderTester::case0476(){}
void BinderTester::case0477(){}
void BinderTester::case0478(){}
void BinderTester::case0479(){}
void BinderTester::case0480(){}
void BinderTester::case0481(){}
void BinderTester::case0482(){}
void BinderTester::case0483(){}
void BinderTester::case0484(){}
void BinderTester::case0485(){}
void BinderTester::case0486(){}
void BinderTester::case0487(){}
void BinderTester::case0488(){}
void BinderTester::case0489(){}
void BinderTester::case0490(){}
void BinderTester::case0491(){}
void BinderTester::case0492(){}
void BinderTester::case0493(){}
void BinderTester::case0494(){}
void BinderTester::case0495(){}
void BinderTester::case0496(){}
void BinderTester::case0497(){}
void BinderTester::case0498(){}
void BinderTester::case0499(){}

void BinderTester::case0500(){}
void BinderTester::case0501(){}
void BinderTester::case0502(){}
void BinderTester::case0503(){}
void BinderTester::case0504(){}
void BinderTester::case0505(){}
void BinderTester::case0506(){}
void BinderTester::case0507(){}
void BinderTester::case0508(){}
void BinderTester::case0509(){}
void BinderTester::case0510(){}
void BinderTester::case0511(){}
void BinderTester::case0512(){}
void BinderTester::case0513(){}
void BinderTester::case0514(){}
void BinderTester::case0515(){}
void BinderTester::case0516(){}
void BinderTester::case0517(){}
void BinderTester::case0518(){}
void BinderTester::case0519(){}
void BinderTester::case0520(){}
void BinderTester::case0521(){}
void BinderTester::case0522(){}
void BinderTester::case0523(){}
void BinderTester::case0524(){}
void BinderTester::case0525(){}
void BinderTester::case0526(){}
void BinderTester::case0527(){}
void BinderTester::case0528(){}
void BinderTester::case0529(){}
void BinderTester::case0530(){}
void BinderTester::case0531(){}
void BinderTester::case0532(){}
void BinderTester::case0533(){}
void BinderTester::case0534(){}
void BinderTester::case0535(){}
void BinderTester::case0536(){}
void BinderTester::case0537(){}
void BinderTester::case0538(){}
void BinderTester::case0539(){}
void BinderTester::case0540(){}
void BinderTester::case0541(){}
void BinderTester::case0542(){}
void BinderTester::case0543(){}
void BinderTester::case0544(){}
void BinderTester::case0545(){}
void BinderTester::case0546(){}
void BinderTester::case0547(){}
void BinderTester::case0548(){}
void BinderTester::case0549(){}
void BinderTester::case0550(){}
void BinderTester::case0551(){}
void BinderTester::case0552(){}
void BinderTester::case0553(){}
void BinderTester::case0554(){}
void BinderTester::case0555(){}
void BinderTester::case0556(){}
void BinderTester::case0557(){}
void BinderTester::case0558(){}
void BinderTester::case0559(){}
void BinderTester::case0560(){}
void BinderTester::case0561(){}
void BinderTester::case0562(){}
void BinderTester::case0563(){}
void BinderTester::case0564(){}
void BinderTester::case0565(){}
void BinderTester::case0566(){}
void BinderTester::case0567(){}
void BinderTester::case0568(){}
void BinderTester::case0569(){}
void BinderTester::case0570(){}
void BinderTester::case0571(){}
void BinderTester::case0572(){}
void BinderTester::case0573(){}
void BinderTester::case0574(){}
void BinderTester::case0575(){}
void BinderTester::case0576(){}
void BinderTester::case0577(){}
void BinderTester::case0578(){}
void BinderTester::case0579(){}
void BinderTester::case0580(){}
void BinderTester::case0581(){}
void BinderTester::case0582(){}
void BinderTester::case0583(){}
void BinderTester::case0584(){}
void BinderTester::case0585(){}
void BinderTester::case0586(){}
void BinderTester::case0587(){}
void BinderTester::case0588(){}
void BinderTester::case0589(){}
void BinderTester::case0590(){}
void BinderTester::case0591(){}
void BinderTester::case0592(){}
void BinderTester::case0593(){}
void BinderTester::case0594(){}
void BinderTester::case0595(){}
void BinderTester::case0596(){}
void BinderTester::case0597(){}
void BinderTester::case0598(){}
void BinderTester::case0599(){}






void BinderTester::case0901()
{
    CROSS_REFERENCE_TEST(ParserTester::case0028);

    bind("const int ;");
}

void BinderTester::case0902()
{
    bind("int x ;");
}

void BinderTester::case0903()
{
    bind("void x ( ) ;");
}

void BinderTester::case0904()
{
    bind("int x ( ) ;");
}

void BinderTester::case0905()
{
    CROSS_REFERENCE_TEST(ParserTester::case0021);

    bind("int double x ;");
}

void BinderTester::case0906()
{
    CROSS_REFERENCE_TEST(ParserTester::case0416);

    bind("struct x { y ; } ;");
}

void BinderTester::case0907()
{
    CROSS_REFERENCE_TEST(ParserTester::case0422);

    bind("struct x { int ; } ;");
}

void BinderTester::case0908()
{
    CROSS_REFERENCE_TEST(ParserTester::case0204);

    // Function returning a function.

    bind("void x ( ) ( ) { }");
}

void BinderTester::case0909()
{
    CROSS_REFERENCE_TEST(ParserTester::case0205);

    // Function returning a function.

    bind("void x ( ) ( ) ;");
}

void BinderTester::case0910()
{
    CROSS_REFERENCE_TEST(ParserTester::case0206);

    // Function returning an array.

    bind("int x ( ) [ ] ;");
}

void BinderTester::case0911()
{
    CROSS_REFERENCE_TEST(ParserTester::case0207);

    bind("int ( x ) ( ) [ ] ;");
}

void BinderTester::case0912()
{
    CROSS_REFERENCE_TEST(ParserTester::case0212);

    // 6.9.1-5

    bind("void x ( int ) { }");
}

void BinderTester::case0913()
{
    CROSS_REFERENCE_TEST(ParserTester::case0040);

    bind("int ;");
}

void BinderTester::case0914()
{
    CROSS_REFERENCE_TEST(ParserTester::case0406);

    // Anonymous `struct' declaration without any variable declared
    // with such type.

    bind("struct { int x ; } ;");
}

void BinderTester::case0915()
{
    CROSS_REFERENCE_TEST(ParserTester::case0632);

    bind("void x ( ) { __thread x y ; }");
}

void BinderTester::case0916()
{
    CROSS_REFERENCE_TEST(ParserTester::case0050);

    bind("double _Complex int x ;");
}

void BinderTester::case0917()
{
    CROSS_REFERENCE_TEST(ParserTester::case0052);

    bind("int int x ;");
}

void BinderTester::case0918()
{
    CROSS_REFERENCE_TEST(ParserTester::case0053);

    bind("_Complex _Complex x ;");
}

void BinderTester::case0919()
{
    CROSS_REFERENCE_TEST(ParserTester::case0435);

    bind("int struct x { int y ; } z ;");
}

void BinderTester::case0920()
{
    CROSS_REFERENCE_TEST(ParserTester::case0436);

    bind("struct x struct { int y ; } z ;");
}

void BinderTester::case0921()
{
    CROSS_REFERENCE_TEST(ParserTester::case0437);

    bind("int struct x y ;");
}

void BinderTester::case0922()
{
    CROSS_REFERENCE_TEST(ParserTester::case0438);

    bind("struct x int y ;");
}

void BinderTester::case0923()
{
    CROSS_REFERENCE_TEST(ParserTester::case0737);

    bind("int __attribute__ ( ( ) ) double ;");
}

void BinderTester::case0924()
{
    CROSS_REFERENCE_TEST(ParserTester::case0738);

    bind("x __attribute__ ( ( ) ) int ;");
}

void BinderTester::case0925()
{
    CROSS_REFERENCE_TEST(ParserTester::case0297);

    bind("void x ( int y ) int y ; { }");
}

void BinderTester::case0926()
{
    CROSS_REFERENCE_TEST(ParserTester::case0298);

    bind("int x ( int y ) z y ; { }");
}

void BinderTester::case0927()
{
    bind("*1;");
}

void BinderTester::case0928()
{
    bind("&1;");
}

void BinderTester::case0929()
{
    CROSS_REFERENCE_TEST(ParserTester::case0446);

    bind("struct x { struct y { int z ; } ; } ;");
}

void BinderTester::case0930(){}
void BinderTester::case0931(){}
void BinderTester::case0932(){}
void BinderTester::case0933(){}
void BinderTester::case0934(){}
void BinderTester::case0935(){}
void BinderTester::case0936(){}
void BinderTester::case0937(){}
void BinderTester::case0938(){}
void BinderTester::case0939(){}
void BinderTester::case0940(){}
void BinderTester::case0941(){}
void BinderTester::case0942(){}
void BinderTester::case0943(){}
void BinderTester::case0944(){}
void BinderTester::case0945(){}
void BinderTester::case0946(){}
void BinderTester::case0947(){}
void BinderTester::case0948(){}
void BinderTester::case0949(){}
void BinderTester::case0950(){}
void BinderTester::case0951(){}
void BinderTester::case0952(){}
void BinderTester::case0953(){}
void BinderTester::case0954(){}
void BinderTester::case0955(){}
void BinderTester::case0956(){}
void BinderTester::case0957(){}
void BinderTester::case0958(){}
void BinderTester::case0959(){}
void BinderTester::case0960(){}
void BinderTester::case0961(){}
void BinderTester::case0962(){}
void BinderTester::case0963(){}
void BinderTester::case0964(){}
void BinderTester::case0965(){}
void BinderTester::case0966(){}
void BinderTester::case0967(){}
void BinderTester::case0968(){}
void BinderTester::case0969(){}
void BinderTester::case0970(){}
void BinderTester::case0971(){}
void BinderTester::case0972(){}
void BinderTester::case0973(){}
void BinderTester::case0974(){}
void BinderTester::case0975(){}
void BinderTester::case0976(){}
void BinderTester::case0977(){}
void BinderTester::case0978(){}
void BinderTester::case0979(){}
void BinderTester::case0980(){}
void BinderTester::case0981(){}
void BinderTester::case0982(){}
void BinderTester::case0983(){}
void BinderTester::case0984(){}
void BinderTester::case0985(){}
void BinderTester::case0986(){}
void BinderTester::case0987(){}
void BinderTester::case0988(){}
void BinderTester::case0989(){}
void BinderTester::case0990(){}
void BinderTester::case0991(){}
void BinderTester::case0992(){}
void BinderTester::case0993(){}
void BinderTester::case0994(){}
void BinderTester::case0995(){}
void BinderTester::case0996(){}
void BinderTester::case0997(){}
void BinderTester::case0998(){}
void BinderTester::case0999(){}
