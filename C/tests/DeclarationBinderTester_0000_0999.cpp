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

#include "DeclarationBinderTester.h"
#include "ParserTester.h"

#include "sema/DeclarationBinder.h"
#include "parser/Unparser.h"
#include "syntax/Lexeme_ALL.h"
#include "syntax/SyntaxNodes.h"
#include "types/Type_ALL.h"

#include <algorithm>
#include <cstring>
#include <iostream>
#include <string>
#include <sstream>

using namespace psy;
using namespace C;

void DeclarationBinderTester::case0001()
{
    bind("void x ( ) ;",
         Expectation()
         .declaration(Decl()
                  .Function("x", ScopeKind::File)
                  .ty_.Void()
                  .ty_.Derived(TypeKind::Function)));
}

void DeclarationBinderTester::case0002()
{
    bind("int x ( ) ;",
         Expectation()
         .declaration(Decl()
                  .Function("x", ScopeKind::File)
                  .ty_.Basic(BasicTypeKind::Int_S)
                  .ty_.Derived(TypeKind::Function)));
}

void DeclarationBinderTester::case0003()
{
    bind("void * x ( ) ;",
         Expectation()
         .declaration(Decl()
                  .Function("x", ScopeKind::File)
                  .ty_.Void()
                  .ty_.Derived(TypeKind::Pointer)
                  .ty_.Derived(TypeKind::Function)));
}

void DeclarationBinderTester::case0004()
{
    bind("int * x ( ) ;",
         Expectation()
         .declaration(Decl()
                  .Function("x", ScopeKind::File)
                  .ty_.Basic(BasicTypeKind::Int_S)
                  .ty_.Derived(TypeKind::Pointer)
                  .ty_.Derived(TypeKind::Function)));
}

void DeclarationBinderTester::case0005()
{
    bind("x y ( ) ;",
         Expectation()
         .declaration(Decl()
                  .Function("y")
                  .ty_.Typedef("x")
                  .ty_.Derived(TypeKind::Function)));
}

void DeclarationBinderTester::case0006()
{
    bind("int * ( x ) ( ) ;",
         Expectation()
         .declaration(Decl()
                  .Function("x", ScopeKind::File)
                  .ty_.Basic(BasicTypeKind::Int_S)
                  .ty_.Derived(TypeKind::Pointer)
                  .ty_.Derived(TypeKind::Function)));
}

void DeclarationBinderTester::case0007()
{
    bind("int * ( ( x ) ) ( ) ;",
         Expectation()
         .declaration(Decl()
                  .Function("x", ScopeKind::File)
                  .ty_.Basic(BasicTypeKind::Int_S)
                  .ty_.Derived(TypeKind::Pointer)
                  .ty_.Derived(TypeKind::Function)));
}

void DeclarationBinderTester::case0008()
{
    bind("int * * x ( ) ;",
         Expectation()
         .declaration(Decl().Function("x", ScopeKind::File)
                  .ty_.Basic(BasicTypeKind::Int_S)
                  .ty_.Derived(TypeKind::Pointer)
                  .ty_.Derived(TypeKind::Pointer)
                  .ty_.Derived(TypeKind::Function)));
}

void DeclarationBinderTester::case0009()
{
    bind("x * y ( ) ;",
         Expectation()
         .declaration(Decl().Function("y")
                  .ty_.Typedef("x")
                  .ty_.Derived(TypeKind::Pointer)
                  .ty_.Derived(TypeKind::Function)));
}

void DeclarationBinderTester::case0010()
{
}

void DeclarationBinderTester::case0011()
{
}

void DeclarationBinderTester::case0012()
{
}

void DeclarationBinderTester::case0013()
{
}

void DeclarationBinderTester::case0014()
{
}

void DeclarationBinderTester::case0015(){}
void DeclarationBinderTester::case0016(){}
void DeclarationBinderTester::case0017(){}
void DeclarationBinderTester::case0018(){}
void DeclarationBinderTester::case0019(){}
void DeclarationBinderTester::case0020(){}
void DeclarationBinderTester::case0021(){}
void DeclarationBinderTester::case0022(){}
void DeclarationBinderTester::case0023(){}
void DeclarationBinderTester::case0024(){}
void DeclarationBinderTester::case0025(){}
void DeclarationBinderTester::case0026(){}

void DeclarationBinderTester::case0027(){}
void DeclarationBinderTester::case0028(){}
void DeclarationBinderTester::case0029(){}
void DeclarationBinderTester::case0030(){}
void DeclarationBinderTester::case0031(){}
void DeclarationBinderTester::case0032(){}
void DeclarationBinderTester::case0033(){}
void DeclarationBinderTester::case0034(){}
void DeclarationBinderTester::case0035(){}
void DeclarationBinderTester::case0036(){}
void DeclarationBinderTester::case0037(){}
void DeclarationBinderTester::case0038(){}
void DeclarationBinderTester::case0039(){}
void DeclarationBinderTester::case0040(){}
void DeclarationBinderTester::case0041(){}
void DeclarationBinderTester::case0042(){}
void DeclarationBinderTester::case0043(){}
void DeclarationBinderTester::case0044(){}
void DeclarationBinderTester::case0045(){}
void DeclarationBinderTester::case0046(){}
void DeclarationBinderTester::case0047(){}
void DeclarationBinderTester::case0048(){}
void DeclarationBinderTester::case0049(){}

void DeclarationBinderTester::case0050()
{
    bind("void x ( int y ) ;",
         Expectation()
         .declaration(Decl()
                  .Function("x", ScopeKind::File)
                  .ty_.Void()
                  .ty_.Derived(TypeKind::Function)
                  .ty_.addParam().Basic(BasicTypeKind::Int_S))
         .declaration(Decl()
                  .Object("y", ObjectDeclarationCategory::Parameter, ScopeKind::FunctionPrototype)
                  .ty_.Basic(BasicTypeKind::Int_S)));
}

void DeclarationBinderTester::case0051()
{
    bind("void x ( int y , double z ) ;",
         Expectation()
         .declaration(Decl()
                  .Function("x", ScopeKind::File)
                  .ty_.Void()
                  .ty_.Derived(TypeKind::Function)
                  .ty_.addParam().Basic(BasicTypeKind::Int_S)
                  .ty_.addParam().Basic(BasicTypeKind::Double))
         .declaration(Decl()
                  .Object("y", ObjectDeclarationCategory::Parameter, ScopeKind::FunctionPrototype)
                  .ty_.Basic(BasicTypeKind::Int_S))
         .declaration(Decl()
                  .Object("z", ObjectDeclarationCategory::Parameter, ScopeKind::FunctionPrototype)
                  .ty_.Basic(BasicTypeKind::Double)));
}

void DeclarationBinderTester::case0052()
{
    bind("void x ( int * y ) ;",
         Expectation()
         .declaration(Decl()
                  .Function("x", ScopeKind::File)
                  .ty_.Void()
                  .ty_.Derived(TypeKind::Function)
                  .ty_.addParam().Basic(BasicTypeKind::Int_S)
                  .ty_.atParam().Derived(TypeKind::Pointer))
         .declaration(Decl()
                  .Object("y", ObjectDeclarationCategory::Parameter, ScopeKind::FunctionPrototype)
                  .ty_.Basic(BasicTypeKind::Int_S)
                  .ty_.Derived(TypeKind::Pointer)));
}

void DeclarationBinderTester::case0053()
{
    bind("void x ( int * y , double * z) ;",
         Expectation()
         .declaration(Decl()
                  .Function("x", ScopeKind::File)
                  .ty_.Void()
                  .ty_.Derived(TypeKind::Function)
                  .ty_.addParam().Basic(BasicTypeKind::Int_S)
                  .ty_.atParam().Derived(TypeKind::Pointer)
                  .ty_.addParam().Basic(BasicTypeKind::Double)
                  .ty_.atParam().Derived(TypeKind::Pointer))
         .declaration(Decl()
                  .Object("y", ObjectDeclarationCategory::Parameter, ScopeKind::FunctionPrototype)
                  .ty_.Basic(BasicTypeKind::Int_S)
                  .ty_.Derived(TypeKind::Pointer))
         .declaration(Decl()
                  .Object("z", ObjectDeclarationCategory::Parameter, ScopeKind::FunctionPrototype)
                  .ty_.Basic(BasicTypeKind::Double)
                  .ty_.Derived(TypeKind::Pointer)));
}

void DeclarationBinderTester::case0054()
{
    bind("void x ( int * * y , double * * z) ;",
         Expectation()
         .declaration(Decl()
                  .Function("x", ScopeKind::File)
                  .ty_.Void()
                  .ty_.Derived(TypeKind::Function)
                  .ty_.addParam().Basic(BasicTypeKind::Int_S)
                  .ty_.atParam().Derived(TypeKind::Pointer)
                  .ty_.atParam().Derived(TypeKind::Pointer)
                  .ty_.addParam().Basic(BasicTypeKind::Double)
                  .ty_.atParam().Derived(TypeKind::Pointer)
                  .ty_.atParam().Derived(TypeKind::Pointer))
         .declaration(Decl()
                  .Object("y", ObjectDeclarationCategory::Parameter, ScopeKind::FunctionPrototype)
                  .ty_.Basic(BasicTypeKind::Int_S)
                  .ty_.Derived(TypeKind::Pointer)
                  .ty_.Derived(TypeKind::Pointer))
         .declaration(Decl()
                  .Object("z", ObjectDeclarationCategory::Parameter, ScopeKind::FunctionPrototype)
                  .ty_.Basic(BasicTypeKind::Double)
                  .ty_.Derived(TypeKind::Pointer)
                  .ty_.Derived(TypeKind::Pointer)));
}

void DeclarationBinderTester::case0055()
{
    bind("void x ( int * * y , double z) ;",
         Expectation()
         .declaration(Decl()
                  .Function("x", ScopeKind::File)
                  .ty_.Void()
                  .ty_.Derived(TypeKind::Function)
                  .ty_.addParam().Basic(BasicTypeKind::Int_S)
                  .ty_.atParam().Derived(TypeKind::Pointer)
                  .ty_.atParam().Derived(TypeKind::Pointer)
                  .ty_.addParam().Basic(BasicTypeKind::Double))
         .declaration(Decl()
                  .Object("y", ObjectDeclarationCategory::Parameter, ScopeKind::FunctionPrototype)
                  .ty_.Basic(BasicTypeKind::Int_S)
                  .ty_.Derived(TypeKind::Pointer)
                  .ty_.Derived(TypeKind::Pointer))
         .declaration(Decl()
                  .Object("z", ObjectDeclarationCategory::Parameter, ScopeKind::FunctionPrototype)
                  .ty_.Basic(BasicTypeKind::Double)));
}

void DeclarationBinderTester::case0056()
{
    bind("void x ( int y , double * * z) ;",
         Expectation()
         .declaration(Decl()
                  .Function("x", ScopeKind::File)
                  .ty_.Void()
                  .ty_.Derived(TypeKind::Function)
                  .ty_.addParam().Basic(BasicTypeKind::Int_S)
                  .ty_.addParam().Basic(BasicTypeKind::Double)
                  .ty_.atParam().Derived(TypeKind::Pointer)
                  .ty_.atParam().Derived(TypeKind::Pointer))
         .declaration(Decl()
                  .Object("y", ObjectDeclarationCategory::Parameter, ScopeKind::FunctionPrototype)
                  .ty_.Basic(BasicTypeKind::Int_S))
         .declaration(Decl()
                  .Object("z", ObjectDeclarationCategory::Parameter, ScopeKind::FunctionPrototype)
                  .ty_.Basic(BasicTypeKind::Double)
                  .ty_.Derived(TypeKind::Pointer)
                  .ty_.Derived(TypeKind::Pointer)));
}

void DeclarationBinderTester::case0057()
{
    bind("void x ( y z , w * * v ) ;",
         Expectation()
         .declaration(Decl()
                  .Function("x", ScopeKind::File)
                  .ty_.Void()
                  .ty_.Derived(TypeKind::Function)
                  .ty_.addParam().Typedef("y")
                  .ty_.addParam().Typedef("w")
                  .ty_.atParam().Derived(TypeKind::Pointer)
                  .ty_.atParam().Derived(TypeKind::Pointer))
         .declaration(Decl()
                  .Object("z", ObjectDeclarationCategory::Parameter, ScopeKind::FunctionPrototype)
                  .ty_.Typedef("y"))
         .declaration(Decl()
                  .Object("v", ObjectDeclarationCategory::Parameter, ScopeKind::FunctionPrototype)
                  .ty_.Typedef("w")
                  .ty_.Derived(TypeKind::Pointer)
                  .ty_.Derived(TypeKind::Pointer)));
}

void DeclarationBinderTester::case0058()
{
    bind("x y ( z const * w , u * v) ;",
         Expectation()
         .declaration(Decl()
                  .Function("y", ScopeKind::File)
                  .ty_.Typedef("x")
                  .ty_.Derived(TypeKind::Function)
                  .ty_.addParam().Typedef("z",  CVR::Const)
                  .ty_.atParam().Derived(TypeKind::Pointer)
                  .ty_.addParam().Typedef("u")
                  .ty_.atParam().Derived(TypeKind::Pointer))
         .declaration(Decl()
                  .Object("w", ObjectDeclarationCategory::Parameter, ScopeKind::FunctionPrototype)
                  .ty_.Typedef("z",  CVR::Const)
                  .ty_.Derived(TypeKind::Pointer))
         .declaration(Decl()
                  .Object("v", ObjectDeclarationCategory::Parameter, ScopeKind::FunctionPrototype)
                  .ty_.Typedef("u")
                  .ty_.Derived(TypeKind::Pointer)));
}

void DeclarationBinderTester::case0059()
{
    bind("x y ( z * const w , u * v) ;",
         Expectation()
         .declaration(Decl()
                  .Function("y", ScopeKind::File)
                  .ty_.Typedef("x")
                  .ty_.Derived(TypeKind::Function)
                  .ty_.addParam().Typedef("z")
                  .ty_.atParam().Derived(TypeKind::Pointer, CVR::Const)
                  .ty_.addParam().Typedef("u")
                  .ty_.atParam().Derived(TypeKind::Pointer))
         .declaration(Decl()
                  .Object("w", ObjectDeclarationCategory::Parameter, ScopeKind::FunctionPrototype)
                  .ty_.Typedef("z")
                  .ty_.Derived(TypeKind::Pointer, CVR::Const))
         .declaration(Decl()
                  .Object("v", ObjectDeclarationCategory::Parameter, ScopeKind::FunctionPrototype)
                  .ty_.Typedef("u")
                  .ty_.Derived(TypeKind::Pointer)));
}

void DeclarationBinderTester::case0060()
{
    bind("x * y ( z w) ;",
         Expectation()
         .declaration(Decl()
                  .Function("y", ScopeKind::File)
                  .ty_.Typedef("x")
                  .ty_.Derived(TypeKind::Pointer)
                  .ty_.Derived(TypeKind::Function)
                  .ty_.addParam().Typedef("z"))
         .declaration(Decl()
                  .Object("w", ObjectDeclarationCategory::Parameter, ScopeKind::FunctionPrototype)
                  .ty_.Typedef("z")));
}

void DeclarationBinderTester::case0061(){}
void DeclarationBinderTester::case0062(){}
void DeclarationBinderTester::case0063(){}
void DeclarationBinderTester::case0064(){}
void DeclarationBinderTester::case0065(){}
void DeclarationBinderTester::case0066(){}
void DeclarationBinderTester::case0067(){}
void DeclarationBinderTester::case0068(){}
void DeclarationBinderTester::case0069(){}
void DeclarationBinderTester::case0070(){}
void DeclarationBinderTester::case0071(){}
void DeclarationBinderTester::case0072(){}
void DeclarationBinderTester::case0073(){}
void DeclarationBinderTester::case0074(){}
void DeclarationBinderTester::case0075(){}
void DeclarationBinderTester::case0076(){}
void DeclarationBinderTester::case0077(){}
void DeclarationBinderTester::case0078(){}
void DeclarationBinderTester::case0079(){}
void DeclarationBinderTester::case0080(){}
void DeclarationBinderTester::case0081(){}
void DeclarationBinderTester::case0082(){}
void DeclarationBinderTester::case0083(){}
void DeclarationBinderTester::case0084(){}
void DeclarationBinderTester::case0085(){}
void DeclarationBinderTester::case0086(){}
void DeclarationBinderTester::case0087(){}
void DeclarationBinderTester::case0088(){}
void DeclarationBinderTester::case0089(){}
void DeclarationBinderTester::case0090(){}
void DeclarationBinderTester::case0091(){}
void DeclarationBinderTester::case0092(){}
void DeclarationBinderTester::case0093(){}
void DeclarationBinderTester::case0094(){}
void DeclarationBinderTester::case0095(){}
void DeclarationBinderTester::case0096(){}
void DeclarationBinderTester::case0097(){}
void DeclarationBinderTester::case0098(){}
void DeclarationBinderTester::case0099(){}
void DeclarationBinderTester::case0100(){}

void DeclarationBinderTester::case0101()
{
    bind("void x ( ) { }",
         Expectation()
         .declaration(Decl()
                  .Function("x", ScopeKind::File)
                  .ty_.Void()
                  .ty_.Derived(TypeKind::Function)));
}

void DeclarationBinderTester::case0102()
{
    bind("int x ( ) { return 1 ; }",
         Expectation()
         .declaration(Decl()
                  .Function("x", ScopeKind::File)
                  .ty_.Basic(BasicTypeKind::Int_S)
                  .ty_.Derived(TypeKind::Function)));
}

void DeclarationBinderTester::case0103()
{
    bind("void * x ( ) { return 1 ; }",
         Expectation()
         .declaration(Decl()
                  .Function("x", ScopeKind::File)
                  .ty_.Void()
                  .ty_.Derived(TypeKind::Pointer)
                  .ty_.Derived(TypeKind::Function)));
}

void DeclarationBinderTester::case0104()
{
    bind("int * x ( ) { return 1 ; }",
         Expectation()
         .declaration(Decl()
                  .Function("x", ScopeKind::File)
                  .ty_.Basic(BasicTypeKind::Int_S)
                  .ty_.Derived(TypeKind::Pointer)
                  .ty_.Derived(TypeKind::Function)));
}

void DeclarationBinderTester::case0105()
{
    bind("x y ( ) { return 1 ; }",
         Expectation()
         .declaration(Decl()
                  .Function("y")
                  .ty_.Typedef("x")
                  .ty_.Derived(TypeKind::Function)));
}

void DeclarationBinderTester::case0106()
{
    bind("int * ( x ) ( ) { return 1 ; }",
         Expectation()
         .declaration(Decl()
                  .Function("x", ScopeKind::File)
                  .ty_.Basic(BasicTypeKind::Int_S)
                  .ty_.Derived(TypeKind::Pointer)
                  .ty_.Derived(TypeKind::Function)));
}

void DeclarationBinderTester::case0107()
{
    bind("int * ( ( x ) ) ( ) { return 1 ; }",
         Expectation()
         .declaration(Decl().Function("x", ScopeKind::File)
                  .ty_.Basic(BasicTypeKind::Int_S)
                  .ty_.Derived(TypeKind::Pointer)
                  .ty_.Derived(TypeKind::Function)));
}

void DeclarationBinderTester::case0108()
{
    bind("int * * x ( ) { return 1 ; }",
         Expectation()
         .declaration(Decl().Function("x", ScopeKind::File)
                  .ty_.Basic(BasicTypeKind::Int_S)
                  .ty_.Derived(TypeKind::Pointer)
                  .ty_.Derived(TypeKind::Pointer)
                  .ty_.Derived(TypeKind::Function)));
}

void DeclarationBinderTester::case0109()
{
    bind("x * y ( ) { return 1 ; }",
         Expectation()
         .declaration(Decl().Function("y")
                  .ty_.Typedef("x")
                  .ty_.Derived(TypeKind::Pointer)
                  .ty_.Derived(TypeKind::Function)));
}

void DeclarationBinderTester::case0110()
{
}

void DeclarationBinderTester::case0111()
{
}

void DeclarationBinderTester::case0112()
{
}

void DeclarationBinderTester::case0113()
{
}

void DeclarationBinderTester::case0114()
{
}

void DeclarationBinderTester::case0115(){}
void DeclarationBinderTester::case0116(){}
void DeclarationBinderTester::case0117(){}
void DeclarationBinderTester::case0118(){}
void DeclarationBinderTester::case0119(){}
void DeclarationBinderTester::case0120(){}
void DeclarationBinderTester::case0121(){}
void DeclarationBinderTester::case0122(){}
void DeclarationBinderTester::case0123(){}
void DeclarationBinderTester::case0124(){}
void DeclarationBinderTester::case0125(){}
void DeclarationBinderTester::case0126(){}
void DeclarationBinderTester::case0127(){}
void DeclarationBinderTester::case0128(){}
void DeclarationBinderTester::case0129(){}
void DeclarationBinderTester::case0130(){}
void DeclarationBinderTester::case0131(){}
void DeclarationBinderTester::case0132(){}
void DeclarationBinderTester::case0133(){}
void DeclarationBinderTester::case0134(){}
void DeclarationBinderTester::case0135(){}
void DeclarationBinderTester::case0136(){}
void DeclarationBinderTester::case0137(){}
void DeclarationBinderTester::case0138(){}
void DeclarationBinderTester::case0139(){}
void DeclarationBinderTester::case0140(){}
void DeclarationBinderTester::case0141(){}
void DeclarationBinderTester::case0142(){}
void DeclarationBinderTester::case0143(){}
void DeclarationBinderTester::case0144(){}
void DeclarationBinderTester::case0145(){}
void DeclarationBinderTester::case0146(){}
void DeclarationBinderTester::case0147(){}
void DeclarationBinderTester::case0148(){}
void DeclarationBinderTester::case0149(){}

void DeclarationBinderTester::case0150()
{
    bind("void x ( int y ) { }",
         Expectation()
         .declaration(Decl()
                  .Function("x", ScopeKind::File)
                  .ty_.Void()
                  .ty_.Derived(TypeKind::Function)
                  .ty_.addParam().Basic(BasicTypeKind::Int_S))
         .declaration(Decl()
                  .Object("y", ObjectDeclarationCategory::Parameter, ScopeKind::Block)
                  .ty_.Basic(BasicTypeKind::Int_S)));
}

void DeclarationBinderTester::case0151()
{
    bind("void x ( int y , double z ) { }",
         Expectation()
         .declaration(Decl()
                  .Function("x", ScopeKind::File)
                  .ty_.Void()
                  .ty_.Derived(TypeKind::Function)
                  .ty_.addParam().Basic(BasicTypeKind::Int_S)
                  .ty_.addParam().Basic(BasicTypeKind::Double))
         .declaration(Decl()
                  .Object("y", ObjectDeclarationCategory::Parameter, ScopeKind::Block)
                  .ty_.Basic(BasicTypeKind::Int_S))
         .declaration(Decl()
                  .Object("z", ObjectDeclarationCategory::Parameter, ScopeKind::Block)
                  .ty_.Basic(BasicTypeKind::Double)));

}

void DeclarationBinderTester::case0152()
{
    bind("void x ( int * y ) { }",
         Expectation()
         .declaration(Decl()
                  .Function("x", ScopeKind::File)
                  .ty_.Void()
                  .ty_.Derived(TypeKind::Function)
                  .ty_.addParam().Basic(BasicTypeKind::Int_S)
                  .ty_.atParam().Derived(TypeKind::Pointer))
         .declaration(Decl()
                  .Object("y", ObjectDeclarationCategory::Parameter, ScopeKind::Block)
                  .ty_.Basic(BasicTypeKind::Int_S)
                  .ty_.Derived(TypeKind::Pointer)));
}

void DeclarationBinderTester::case0153()
{
    bind("void x ( int * y , double * z) { }",
         Expectation()
         .declaration(Decl()
                  .Function("x", ScopeKind::File)
                  .ty_.Void()
                  .ty_.Derived(TypeKind::Function)
                  .ty_.addParam().Basic(BasicTypeKind::Int_S)
                  .ty_.atParam().Derived(TypeKind::Pointer)
                  .ty_.addParam().Basic(BasicTypeKind::Double)
                  .ty_.atParam().Derived(TypeKind::Pointer))
         .declaration(Decl()
                  .Object("y", ObjectDeclarationCategory::Parameter, ScopeKind::Block)
                  .ty_.Basic(BasicTypeKind::Int_S)
                  .ty_.Derived(TypeKind::Pointer))
         .declaration(Decl()
                  .Object("z", ObjectDeclarationCategory::Parameter, ScopeKind::Block)
                  .ty_.Basic(BasicTypeKind::Double)
                  .ty_.Derived(TypeKind::Pointer)));
}

void DeclarationBinderTester::case0154()
{
    bind("void x ( int * * y , double * * z) { }",
         Expectation()
         .declaration(Decl()
                  .Function("x", ScopeKind::File)
                  .ty_.Void()
                  .ty_.Derived(TypeKind::Function)
                  .ty_.addParam().Basic(BasicTypeKind::Int_S)
                  .ty_.atParam().Derived(TypeKind::Pointer)
                  .ty_.atParam().Derived(TypeKind::Pointer)
                  .ty_.addParam().Basic(BasicTypeKind::Double)
                  .ty_.atParam().Derived(TypeKind::Pointer)
                  .ty_.atParam().Derived(TypeKind::Pointer))
         .declaration(Decl()
                  .Object("y", ObjectDeclarationCategory::Parameter, ScopeKind::Block)
                  .ty_.Basic(BasicTypeKind::Int_S)
                  .ty_.Derived(TypeKind::Pointer)
                  .ty_.Derived(TypeKind::Pointer))
         .declaration(Decl()
                  .Object("z", ObjectDeclarationCategory::Parameter, ScopeKind::Block)
                  .ty_.Basic(BasicTypeKind::Double)
                  .ty_.Derived(TypeKind::Pointer)
                  .ty_.Derived(TypeKind::Pointer)));
}

void DeclarationBinderTester::case0155()
{
    bind("void x ( int * * y , double z) { }",
         Expectation()
         .declaration(Decl()
                  .Function("x", ScopeKind::File)
                  .ty_.Void()
                  .ty_.Derived(TypeKind::Function)
                  .ty_.addParam().Basic(BasicTypeKind::Int_S)
                  .ty_.atParam().Derived(TypeKind::Pointer)
                  .ty_.atParam().Derived(TypeKind::Pointer)
                  .ty_.addParam().Basic(BasicTypeKind::Double))
         .declaration(Decl()
                  .Object("y", ObjectDeclarationCategory::Parameter, ScopeKind::Block)
                  .ty_.Basic(BasicTypeKind::Int_S)
                  .ty_.Derived(TypeKind::Pointer)
                  .ty_.Derived(TypeKind::Pointer))
         .declaration(Decl()
                  .Object("z", ObjectDeclarationCategory::Parameter, ScopeKind::Block)
                  .ty_.Basic(BasicTypeKind::Double)));
}

void DeclarationBinderTester::case0156()
{
    bind("void x ( int y , double * * z) { }",
         Expectation()
         .declaration(Decl()
                  .Function("x", ScopeKind::File)
                  .ty_.Void()
                  .ty_.Derived(TypeKind::Function)
                  .ty_.addParam().Basic(BasicTypeKind::Int_S)
                  .ty_.addParam().Basic(BasicTypeKind::Double)
                  .ty_.atParam().Derived(TypeKind::Pointer)
                  .ty_.atParam().Derived(TypeKind::Pointer))
         .declaration(Decl()
                  .Object("y", ObjectDeclarationCategory::Parameter, ScopeKind::Block)
                  .ty_.Basic(BasicTypeKind::Int_S))
         .declaration(Decl()
                  .Object("z", ObjectDeclarationCategory::Parameter, ScopeKind::Block)
                  .ty_.Basic(BasicTypeKind::Double)
                  .ty_.Derived(TypeKind::Pointer)
                  .ty_.Derived(TypeKind::Pointer)));
}

void DeclarationBinderTester::case0157()
{
    bind("void x ( y z , w * * v ) { }",
         Expectation()
         .declaration(Decl()
                  .Function("x", ScopeKind::File)
                  .ty_.Void()
                  .ty_.Derived(TypeKind::Function)
                  .ty_.addParam().Typedef("y")
                  .ty_.addParam().Typedef("w")
                  .ty_.atParam().Derived(TypeKind::Pointer)
                  .ty_.atParam().Derived(TypeKind::Pointer))
         .declaration(Decl()
                  .Object("z", ObjectDeclarationCategory::Parameter, ScopeKind::Block)
                  .ty_.Typedef("y"))
         .declaration(Decl()
                  .Object("v", ObjectDeclarationCategory::Parameter, ScopeKind::Block)
                  .ty_.Typedef("w")
                  .ty_.Derived(TypeKind::Pointer)
                  .ty_.Derived(TypeKind::Pointer)));
}

void DeclarationBinderTester::case0158()
{
    bind("x y ( z const * w , u * v) { }",
         Expectation()
         .declaration(Decl()
                  .Function("y", ScopeKind::File)
                  .ty_.Typedef("x")
                  .ty_.Derived(TypeKind::Function)
                  .ty_.addParam().Typedef("z",  CVR::Const)
                  .ty_.atParam().Derived(TypeKind::Pointer)
                  .ty_.addParam().Typedef("u")
                  .ty_.atParam().Derived(TypeKind::Pointer))
         .declaration(Decl()
                  .Object("w", ObjectDeclarationCategory::Parameter, ScopeKind::Block)
                  .ty_.Typedef("z",  CVR::Const)
                  .ty_.Derived(TypeKind::Pointer))
         .declaration(Decl()
                  .Object("v", ObjectDeclarationCategory::Parameter, ScopeKind::Block)
                  .ty_.Typedef("u")
                  .ty_.Derived(TypeKind::Pointer)));
}

void DeclarationBinderTester::case0159()
{
    bind("x y ( z * const w , u * v) { }",
         Expectation()
         .declaration(Decl()
                  .Function("y", ScopeKind::File)
                  .ty_.Typedef("x")
                  .ty_.Derived(TypeKind::Function)
                  .ty_.addParam().Typedef("z")
                  .ty_.atParam().Derived(TypeKind::Pointer, CVR::Const)
                  .ty_.addParam().Typedef("u")
                  .ty_.atParam().Derived(TypeKind::Pointer))
         .declaration(Decl()
                  .Object("w", ObjectDeclarationCategory::Parameter, ScopeKind::Block)
                  .ty_.Typedef("z")
                  .ty_.Derived(TypeKind::Pointer, CVR::Const))
         .declaration(Decl()
                  .Object("v", ObjectDeclarationCategory::Parameter, ScopeKind::Block)
                  .ty_.Typedef("u")
                  .ty_.Derived(TypeKind::Pointer)));
}

void DeclarationBinderTester::case0160()
{
    bind("x * y ( z w) { }",
         Expectation()
         .declaration(Decl()
                  .Function("y", ScopeKind::File)
                  .ty_.Typedef("x")
                  .ty_.Derived(TypeKind::Pointer)
                  .ty_.Derived(TypeKind::Function)
                  .ty_.addParam().Typedef("z"))
         .declaration(Decl()
                  .Object("w", ObjectDeclarationCategory::Parameter, ScopeKind::Block)
                  .ty_.Typedef("z")));
}

void DeclarationBinderTester::case0161()
{
    bind("void x ( y ) { }",
         Expectation()
             .diagnostic(Expectation::ErrorOrWarn::Error,
                         DeclarationBinder::DiagnosticsReporter::ID_of_TypeSpecifierMissingDefaultsToInt)
             .declaration(Decl()
                      .Function("x", ScopeKind::File)
                      .ty_.Void()
                      .ty_.Derived(TypeKind::Function)
                      .ty_.addParam().Basic(BasicTypeKind::Int_S))
             .declaration(Decl()
                      .Object("y", ObjectDeclarationCategory::Parameter, ScopeKind::Block)
                      .ty_.Basic(BasicTypeKind::Int_S)));
}

void DeclarationBinderTester::case0162()
{
    bind("x ( int y ) { }",
         Expectation()
             .diagnostic(Expectation::ErrorOrWarn::Error,
                         DeclarationBinder::DiagnosticsReporter::ID_of_TypeSpecifierMissingDefaultsToInt)
             .declaration(Decl()
                      .Function("x", ScopeKind::File)
                      .ty_.Basic(BasicTypeKind::Int_S)
                      .ty_.Derived(TypeKind::Function)
                      .ty_.addParam().Basic(BasicTypeKind::Int_S))
             .declaration(Decl()
                      .Object("y", ObjectDeclarationCategory::Parameter, ScopeKind::Block)
                      .ty_.Basic(BasicTypeKind::Int_S)));
}

void DeclarationBinderTester::case0163()
{
    bind("x ( int y ) ;",
         Expectation()
             .diagnostic(Expectation::ErrorOrWarn::Error,
                         DeclarationBinder::DiagnosticsReporter::ID_of_TypeSpecifierMissingDefaultsToInt)
             .declaration(Decl()
                      .Function("x", ScopeKind::File)
                      .ty_.Basic(BasicTypeKind::Int_S)
                      .ty_.Derived(TypeKind::Function)
                      .ty_.addParam().Basic(BasicTypeKind::Int_S))
             .declaration(Decl()
                      .Object("y", ObjectDeclarationCategory::Parameter, ScopeKind::FunctionPrototype)
                      .ty_.Basic(BasicTypeKind::Int_S)));
}

void DeclarationBinderTester::case0164()
{
    bind("x ( y z ) { }",
         Expectation()
             .diagnostic(Expectation::ErrorOrWarn::Error,
                         DeclarationBinder::DiagnosticsReporter::ID_of_TypeSpecifierMissingDefaultsToInt)
             .declaration(Decl()
                      .Function("x", ScopeKind::File)
                      .ty_.Basic(BasicTypeKind::Int_S)
                      .ty_.Derived(TypeKind::Function)
                      .ty_.addParam().Typedef("y"))
             .declaration(Decl()
                      .Object("z", ObjectDeclarationCategory::Parameter, ScopeKind::Block)
                      .ty_.Typedef("y")));
}

void DeclarationBinderTester::case0165()
{
    bind("x ( y z ) ;",
         Expectation()
             .diagnostic(Expectation::ErrorOrWarn::Error,
                         DeclarationBinder::DiagnosticsReporter::ID_of_TypeSpecifierMissingDefaultsToInt)
             .declaration(Decl()
                      .Function("x", ScopeKind::File)
                      .ty_.Basic(BasicTypeKind::Int_S)
                      .ty_.Derived(TypeKind::Function)
                      .ty_.addParam().Typedef("y"))
             .declaration(Decl()
                      .Object("z", ObjectDeclarationCategory::Parameter, ScopeKind::FunctionPrototype)
                      .ty_.Typedef("y")));
}

void DeclarationBinderTester::case0166(){}
void DeclarationBinderTester::case0167(){}
void DeclarationBinderTester::case0168(){}
void DeclarationBinderTester::case0169(){}
void DeclarationBinderTester::case0170(){}
void DeclarationBinderTester::case0171(){}
void DeclarationBinderTester::case0172(){}
void DeclarationBinderTester::case0173(){}
void DeclarationBinderTester::case0174(){}
void DeclarationBinderTester::case0175(){}
void DeclarationBinderTester::case0176(){}
void DeclarationBinderTester::case0177(){}
void DeclarationBinderTester::case0178(){}
void DeclarationBinderTester::case0179(){}
void DeclarationBinderTester::case0180(){}
void DeclarationBinderTester::case0181(){}
void DeclarationBinderTester::case0182(){}
void DeclarationBinderTester::case0183(){}
void DeclarationBinderTester::case0184(){}
void DeclarationBinderTester::case0185(){}
void DeclarationBinderTester::case0186(){}
void DeclarationBinderTester::case0187(){}
void DeclarationBinderTester::case0188(){}
void DeclarationBinderTester::case0189(){}
void DeclarationBinderTester::case0190(){}
void DeclarationBinderTester::case0191(){}
void DeclarationBinderTester::case0192(){}
void DeclarationBinderTester::case0193(){}
void DeclarationBinderTester::case0194(){}
void DeclarationBinderTester::case0195(){}
void DeclarationBinderTester::case0196(){}
void DeclarationBinderTester::case0197(){}
void DeclarationBinderTester::case0198(){}
void DeclarationBinderTester::case0199(){}

void DeclarationBinderTester::case0200()
{
    bind("void x ( int ) ;",
         Expectation()
         .declaration(Decl()
                  .Function("x", ScopeKind::File)
                  .ty_.Void()
                  .ty_.Derived(TypeKind::Function)
                  .ty_.addParam().Basic(BasicTypeKind::Int_S))
         .declaration(Decl()
                  .Object("", ObjectDeclarationCategory::Parameter, ScopeKind::FunctionPrototype)
                  .ty_.Basic(BasicTypeKind::Int_S)));
}

void DeclarationBinderTester::case0201()
{
    bind("void x ( y ) ;",
         Expectation()
         .declaration(Decl()
                  .Function("x", ScopeKind::File)
                  .ty_.Void()
                  .ty_.Derived(TypeKind::Function)
                  .ty_.addParam().Typedef("y"))
         .declaration(Decl()
                  .Object("", ObjectDeclarationCategory::Parameter, ScopeKind::FunctionPrototype)
                  .ty_.Typedef("y")));
}

void DeclarationBinderTester::case0202()
{
    bind("void x ( int , y ) ;",
         Expectation()
         .declaration(Decl()
                  .Function("x", ScopeKind::File)
                  .ty_.Void()
                  .ty_.Derived(TypeKind::Function)
                  .ty_.addParam().Basic(BasicTypeKind::Int_S)
                  .ty_.addParam().Typedef("y"))
         .declaration(Decl()
                  .Object("", ObjectDeclarationCategory::Parameter, ScopeKind::FunctionPrototype)
                  .ty_.Basic(BasicTypeKind::Int_S))
         .declaration(Decl()
                  .Object("", ObjectDeclarationCategory::Parameter, ScopeKind::FunctionPrototype)
                  .ty_.Typedef("y")));
}

void DeclarationBinderTester::case0203()
{
    bind("void x ( y , int ) ;",
         Expectation()
         .declaration(Decl()
                  .Function("x", ScopeKind::File)
                  .ty_.Void()
                  .ty_.Derived(TypeKind::Function)
                  .ty_.addParam().Typedef("y")
                  .ty_.addParam().Basic(BasicTypeKind::Int_S))
         .declaration(Decl()
                  .Object("", ObjectDeclarationCategory::Parameter, ScopeKind::FunctionPrototype)
                  .ty_.Basic(BasicTypeKind::Int_S))
         .declaration(Decl()
                  .Object("", ObjectDeclarationCategory::Parameter, ScopeKind::FunctionPrototype)
                  .ty_.Typedef("y")));
}

void DeclarationBinderTester::case0204()
{
    bind("void x ( y , z ) ;",
         Expectation()
         .declaration(Decl()
                  .Function("x", ScopeKind::File)
                  .ty_.Void()
                  .ty_.Derived(TypeKind::Function)
                  .ty_.addParam().Typedef("y")
                  .ty_.addParam().Typedef("z"))
         .declaration(Decl()
                  .Object("", ObjectDeclarationCategory::Parameter, ScopeKind::FunctionPrototype)
                  .ty_.Typedef("z"))
         .declaration(Decl()
                  .Object("", ObjectDeclarationCategory::Parameter, ScopeKind::FunctionPrototype)
                  .ty_.Typedef("y")));

}

void DeclarationBinderTester::case0205()
{
    bind("void x ( int * ) ;",
         Expectation()
         .declaration(Decl()
                  .Function("x", ScopeKind::File)
                  .ty_.Void()
                  .ty_.Derived(TypeKind::Function)
                  .ty_.addParam().Basic(BasicTypeKind::Int_S)
                  .ty_.atParam().Derived(TypeKind::Pointer))
         .declaration(Decl()
                  .Object("", ObjectDeclarationCategory::Parameter, ScopeKind::FunctionPrototype)
                  .ty_.Basic(BasicTypeKind::Int_S)
                  .ty_.Derived(TypeKind::Pointer)));
}

void DeclarationBinderTester::case0206()
{
    bind("void x ( int [ ] ) ;",
         Expectation()
         .declaration(Decl()
                  .Function("x", ScopeKind::File)
                  .ty_.Void()
                  .ty_.Derived(TypeKind::Function)
                  .ty_.addParam().Basic(BasicTypeKind::Int_S)
                  .ty_.atParam().Derived(TypeKind::Pointer, CVR::None, Decay::FromArrayToPointer))
         .declaration(Decl()
                  .Object("", ObjectDeclarationCategory::Parameter, ScopeKind::FunctionPrototype)
                  .ty_.Basic(BasicTypeKind::Int_S)
                  .ty_.Derived(TypeKind::Pointer, CVR::None, Decay::FromArrayToPointer)));
}

void DeclarationBinderTester::case0207()
{
    bind("void x ( int [ 1 ] ) ;",
         Expectation()
         .declaration(Decl()
                  .Function("x", ScopeKind::File)
                  .ty_.Void()
                  .ty_.Derived(TypeKind::Function)
                  .ty_.addParam().Basic(BasicTypeKind::Int_S)
                  .ty_.atParam().Derived(TypeKind::Pointer, CVR::None, Decay::FromArrayToPointer))
         .declaration(Decl()
                  .Object("", ObjectDeclarationCategory::Parameter, ScopeKind::FunctionPrototype)
                  .ty_.Basic(BasicTypeKind::Int_S)
                  .ty_.Derived(TypeKind::Pointer, CVR::None, Decay::FromArrayToPointer)));
}

void DeclarationBinderTester::case0208(){}
void DeclarationBinderTester::case0209(){}
void DeclarationBinderTester::case0210(){}
void DeclarationBinderTester::case0211(){}
void DeclarationBinderTester::case0212(){}
void DeclarationBinderTester::case0213(){}
void DeclarationBinderTester::case0214(){}
void DeclarationBinderTester::case0215(){}
void DeclarationBinderTester::case0216(){}
void DeclarationBinderTester::case0217(){}
void DeclarationBinderTester::case0218(){}
void DeclarationBinderTester::case0219(){}
void DeclarationBinderTester::case0220(){}
void DeclarationBinderTester::case0221(){}
void DeclarationBinderTester::case0222(){}
void DeclarationBinderTester::case0223(){}
void DeclarationBinderTester::case0224(){}
void DeclarationBinderTester::case0225(){}
void DeclarationBinderTester::case0226(){}
void DeclarationBinderTester::case0227(){}
void DeclarationBinderTester::case0228(){}
void DeclarationBinderTester::case0229(){}
void DeclarationBinderTester::case0230(){}
void DeclarationBinderTester::case0231(){}
void DeclarationBinderTester::case0232(){}
void DeclarationBinderTester::case0233(){}
void DeclarationBinderTester::case0234(){}
void DeclarationBinderTester::case0235(){}
void DeclarationBinderTester::case0236(){}
void DeclarationBinderTester::case0237(){}
void DeclarationBinderTester::case0238(){}
void DeclarationBinderTester::case0239(){}
void DeclarationBinderTester::case0240(){}

void DeclarationBinderTester::case0241(){}
void DeclarationBinderTester::case0242(){}
void DeclarationBinderTester::case0243(){}
void DeclarationBinderTester::case0244(){}
void DeclarationBinderTester::case0245(){}
void DeclarationBinderTester::case0246(){}
void DeclarationBinderTester::case0247(){}
void DeclarationBinderTester::case0248(){}
void DeclarationBinderTester::case0249(){}
void DeclarationBinderTester::case0250(){}
void DeclarationBinderTester::case0251(){}
void DeclarationBinderTester::case0252(){}
void DeclarationBinderTester::case0253(){}
void DeclarationBinderTester::case0254(){}
void DeclarationBinderTester::case0255(){}
void DeclarationBinderTester::case0256(){}
void DeclarationBinderTester::case0257(){}
void DeclarationBinderTester::case0258(){}
void DeclarationBinderTester::case0259(){}
void DeclarationBinderTester::case0260(){}
void DeclarationBinderTester::case0261(){}
void DeclarationBinderTester::case0262(){}
void DeclarationBinderTester::case0263(){}
void DeclarationBinderTester::case0264(){}
void DeclarationBinderTester::case0265(){}
void DeclarationBinderTester::case0266(){}
void DeclarationBinderTester::case0267(){}
void DeclarationBinderTester::case0268(){}
void DeclarationBinderTester::case0269(){}
void DeclarationBinderTester::case0270(){}
void DeclarationBinderTester::case0271(){}
void DeclarationBinderTester::case0272(){}
void DeclarationBinderTester::case0273(){}
void DeclarationBinderTester::case0274(){}
void DeclarationBinderTester::case0275(){}
void DeclarationBinderTester::case0276(){}
void DeclarationBinderTester::case0277(){}
void DeclarationBinderTester::case0278(){}
void DeclarationBinderTester::case0279(){}
void DeclarationBinderTester::case0280(){}
void DeclarationBinderTester::case0281(){}
void DeclarationBinderTester::case0282(){}
void DeclarationBinderTester::case0283(){}
void DeclarationBinderTester::case0284(){}
void DeclarationBinderTester::case0285(){}
void DeclarationBinderTester::case0286(){}
void DeclarationBinderTester::case0287(){}
void DeclarationBinderTester::case0288(){}
void DeclarationBinderTester::case0289(){}
void DeclarationBinderTester::case0290(){}
void DeclarationBinderTester::case0291(){}
void DeclarationBinderTester::case0292(){}
void DeclarationBinderTester::case0293(){}
void DeclarationBinderTester::case0294(){}
void DeclarationBinderTester::case0295(){}
void DeclarationBinderTester::case0296(){}
void DeclarationBinderTester::case0297(){}
void DeclarationBinderTester::case0298(){}
void DeclarationBinderTester::case0299(){}

void DeclarationBinderTester::case0300()
{
    bind("void x ( int ( * y ) ( double ) ) ;",
         Expectation()
         .declaration(Decl()
                  .Function("x", ScopeKind::File)
                  .ty_.Void()
                  .ty_.Derived(TypeKind::Function)
                  .ty_.addParam().Basic(BasicTypeKind::Int_S)
                  .ty_.atParam().Derived(TypeKind::Function)
                  .ty_.atParam().addParam().Basic(BasicTypeKind::Double)
                  .ty_.atParam().Derived(TypeKind::Pointer))
         .declaration(Decl()
                  .Object("y", ObjectDeclarationCategory::Parameter, ScopeKind::FunctionPrototype)
                  .ty_.Basic(BasicTypeKind::Int_S)
                  .ty_.Derived(TypeKind::Function)
                  .ty_.addParam().Basic(BasicTypeKind::Double)
                  .ty_.Derived(TypeKind::Pointer)));
}

void DeclarationBinderTester::case0301()
{
    bind("void x ( int ( * y ) ( double ) , char ( * z ) ( float ) ) ;",
         Expectation()
         .declaration(Decl()
                  .Function("x", ScopeKind::File)
                  .ty_.Void()
                  .ty_.Derived(TypeKind::Function)
                  .ty_.addParam().Basic(BasicTypeKind::Int_S)
                  .ty_.atParam().Derived(TypeKind::Function)
                  .ty_.atParam().addParam().Basic(BasicTypeKind::Double)
                  .ty_.atParam().Derived(TypeKind::Pointer)
                  .ty_.addParam().Basic(BasicTypeKind::Char)
                  .ty_.atParam().Derived(TypeKind::Function)
                  .ty_.atParam().addParam().Basic(BasicTypeKind::Float)
                  .ty_.atParam().Derived(TypeKind::Pointer))
         .declaration(Decl()
                  .Object("y", ObjectDeclarationCategory::Parameter, ScopeKind::FunctionPrototype)
                  .ty_.Basic(BasicTypeKind::Int_S)
                  .ty_.Derived(TypeKind::Function)
                  .ty_.addParam().Basic(BasicTypeKind::Double)
                  .ty_.Derived(TypeKind::Pointer))
         .declaration(Decl()
                  .Object("z", ObjectDeclarationCategory::Parameter, ScopeKind::FunctionPrototype)
                  .ty_.Basic(BasicTypeKind::Char)
                  .ty_.Derived(TypeKind::Function)
                  .ty_.addParam().Basic(BasicTypeKind::Float)
                  .ty_.Derived(TypeKind::Pointer)));
}

void DeclarationBinderTester::case0302()
{
    bind("void x ( int * ( * y ) ( double ) ) ;",
         Expectation()
         .declaration(Decl()
                  .Function("x", ScopeKind::File)
                  .ty_.Void()
                  .ty_.Derived(TypeKind::Function)
                  .ty_.addParam().Basic(BasicTypeKind::Int_S)
                  .ty_.atParam().Derived(TypeKind::Pointer)
                  .ty_.atParam().Derived(TypeKind::Function)
                  .ty_.atParam().addParam().Basic(BasicTypeKind::Double)
                  .ty_.atParam().Derived(TypeKind::Pointer))
         .declaration(Decl()
                  .Object("y", ObjectDeclarationCategory::Parameter, ScopeKind::FunctionPrototype)
                  .ty_.Basic(BasicTypeKind::Int_S)
                  .ty_.Derived(TypeKind::Pointer)
                  .ty_.Derived(TypeKind::Function)
                  .ty_.addParam().Basic(BasicTypeKind::Double)
                  .ty_.Derived(TypeKind::Pointer)));
}

void DeclarationBinderTester::case0303()
{
    bind("void x ( int ( * * y ) ( double ) ) ;",
         Expectation()
         .declaration(Decl()
                  .Function("x", ScopeKind::File)
                  .ty_.Void()
                  .ty_.Derived(TypeKind::Function)
                  .ty_.addParam().Basic(BasicTypeKind::Int_S)
                  .ty_.atParam().Derived(TypeKind::Function)
                  .ty_.atParam().addParam().Basic(BasicTypeKind::Double)
                  .ty_.atParam().Derived(TypeKind::Pointer)
                  .ty_.atParam().Derived(TypeKind::Pointer))
         .declaration(Decl()
                  .Object("y", ObjectDeclarationCategory::Parameter, ScopeKind::FunctionPrototype)
                  .ty_.Basic(BasicTypeKind::Int_S)
                  .ty_.Derived(TypeKind::Function)
                  .ty_.addParam().Basic(BasicTypeKind::Double)
                  .ty_.Derived(TypeKind::Pointer)
                  .ty_.Derived(TypeKind::Pointer)));
}

void DeclarationBinderTester::case0304()
{
    bind("void x ( y ( * z ) ( w ) ) ;",
         Expectation()
         .declaration(Decl()
                  .Function("x", ScopeKind::File)
                  .ty_.Void()
                  .ty_.Derived(TypeKind::Function)
                  .ty_.addParam().Typedef("y")
                  .ty_.atParam().Derived(TypeKind::Function)
                  .ty_.atParam().addParam().Typedef("w")
                  .ty_.atParam().Derived(TypeKind::Pointer))
         .declaration(Decl()
                  .Object("z", ObjectDeclarationCategory::Parameter, ScopeKind::FunctionPrototype)
                  .ty_.Typedef("y")
                  .ty_.Derived(TypeKind::Function)
                  .ty_.addParam().Typedef("w")
                  .ty_.Derived(TypeKind::Pointer)));
}

void DeclarationBinderTester::case0305()
{
    bind("void x ( y * * ( * z ) ( w ) ) ;",
         Expectation()
         .declaration(Decl()
                  .Function("x", ScopeKind::File)
                  .ty_.Void()
                  .ty_.Derived(TypeKind::Function)
                  .ty_.addParam().Typedef("y")
                  .ty_.atParam().Derived(TypeKind::Pointer)
                  .ty_.atParam().Derived(TypeKind::Pointer)
                  .ty_.atParam().Derived(TypeKind::Function)
                  .ty_.atParam().addParam().Typedef("w")
                  .ty_.atParam().Derived(TypeKind::Pointer))
         .declaration(Decl()
                  .Object("z", ObjectDeclarationCategory::Parameter, ScopeKind::FunctionPrototype)
                  .ty_.Typedef("y")
                  .ty_.Derived(TypeKind::Pointer)
                  .ty_.Derived(TypeKind::Pointer)
                  .ty_.Derived(TypeKind::Function)
                  .ty_.addParam().Typedef("w")
                  .ty_.Derived(TypeKind::Pointer)));
}

void DeclarationBinderTester::case0306()
{
    bind("void x ( y * * ( * z ) ( double ) , int * u ) ;",
         Expectation()
         .declaration(Decl()
                  .Function("x", ScopeKind::File)
                  .ty_.Void()
                  .ty_.Derived(TypeKind::Function)
                  .ty_.addParam().Typedef("y")
                  .ty_.atParam().Derived(TypeKind::Pointer)
                  .ty_.atParam().Derived(TypeKind::Pointer)
                  .ty_.atParam().Derived(TypeKind::Function)
                  .ty_.atParam().addParam().Basic(BasicTypeKind::Double)
                  .ty_.atParam().Derived(TypeKind::Pointer)
                  .ty_.addParam().Basic(BasicTypeKind::Int_S)
                  .ty_.atParam().Derived(TypeKind::Pointer))
         .declaration(Decl()
                  .Object("z", ObjectDeclarationCategory::Parameter, ScopeKind::FunctionPrototype)
                  .ty_.Typedef("y")
                  .ty_.Derived(TypeKind::Pointer)
                  .ty_.Derived(TypeKind::Pointer)
                  .ty_.Derived(TypeKind::Function)
                  .ty_.addParam().Basic(BasicTypeKind::Double)
                  .ty_.Derived(TypeKind::Pointer))
         .declaration(Decl()
                  .Object("u", ObjectDeclarationCategory::Parameter, ScopeKind::FunctionPrototype)
                  .ty_.Basic(BasicTypeKind::Int_S)
                  .ty_.Derived(TypeKind::Pointer)));
}

void DeclarationBinderTester::case0307()
{
    bind("void x ( int ( * y ) ( z ) , int * w ) ;",
         Expectation()
         .declaration(Decl()
                  .Function("x", ScopeKind::File)
                  .ty_.Void()
                  .ty_.Derived(TypeKind::Function)
                  .ty_.addParam().Basic(BasicTypeKind::Int_S)
                  .ty_.atParam().Derived(TypeKind::Function)
                  .ty_.atParam().addParam().Typedef("z")
                  .ty_.atParam().Derived(TypeKind::Pointer)
                  .ty_.addParam().Basic(BasicTypeKind::Int_S)
                  .ty_.atParam().Derived(TypeKind::Pointer))
         .declaration(Decl()
                  .Object("w", ObjectDeclarationCategory::Parameter, ScopeKind::FunctionPrototype)
                  .ty_.Basic(BasicTypeKind::Int_S)
                  .ty_.Derived(TypeKind::Pointer))
         .declaration(Decl()
                  .Object("y", ObjectDeclarationCategory::Parameter, ScopeKind::FunctionPrototype)
                  .ty_.Basic(BasicTypeKind::Int_S)
                  .ty_.Derived(TypeKind::Function)
                  .ty_.addParam().Typedef("z")
                  .ty_.Derived(TypeKind::Pointer)));
}

void DeclarationBinderTester::case0308()
{
    bind("void x ( int * y , int ( * z ) ( w ) ) ;",
         Expectation()
         .declaration(Decl()
                  .Function("x", ScopeKind::File)
                  .ty_.Void()
                  .ty_.Derived(TypeKind::Function)
                  .ty_.addParam().Basic(BasicTypeKind::Int_S)
                  .ty_.atParam().Derived(TypeKind::Pointer)
                  .ty_.addParam().Typedef("z")
                  .ty_.atParam().Derived(TypeKind::Function)
                  .ty_.atParam().Basic(BasicTypeKind::Int_S)
                  .ty_.atParam().addParam().Typedef("w")
                  .ty_.atParam().Derived(TypeKind::Pointer))
         .declaration(Decl()
                  .Object("z", ObjectDeclarationCategory::Parameter, ScopeKind::FunctionPrototype)
                  .ty_.Basic(BasicTypeKind::Int_S)
                  .ty_.Derived(TypeKind::Function)
                  .ty_.addParam().Typedef("w")
                  .ty_.Derived(TypeKind::Pointer))
         .declaration(Decl()
                  .Object("y", ObjectDeclarationCategory::Parameter, ScopeKind::FunctionPrototype)
                  .ty_.Basic(BasicTypeKind::Int_S)
                  .ty_.Derived(TypeKind::Pointer)));
}

void DeclarationBinderTester::case0309()
{
    bind("void x ( int ( * y ) ( z * ) , int * w ) ;",
         Expectation()
         .declaration(Decl()
                  .Function("x", ScopeKind::File)
                  .ty_.Void()
                  .ty_.Derived(TypeKind::Function)
                  .ty_.addParam().Basic(BasicTypeKind::Int_S)
                  .ty_.atParam().Derived(TypeKind::Function)
                  .ty_.atParam().addParam().Typedef("z")
                  .ty_.atParam().atParam().Derived(TypeKind::Pointer)
                  .ty_.atParam().Derived(TypeKind::Pointer)
                  .ty_.addParam().Basic(BasicTypeKind::Int_S)
                  .ty_.atParam().Derived(TypeKind::Pointer))
         .declaration(Decl()
                  .Object("w", ObjectDeclarationCategory::Parameter, ScopeKind::FunctionPrototype)
                  .ty_.Basic(BasicTypeKind::Int_S)
                  .ty_.Derived(TypeKind::Pointer))
         .declaration(Decl()
                  .Object("y", ObjectDeclarationCategory::Parameter, ScopeKind::FunctionPrototype)
                  .ty_.Basic(BasicTypeKind::Int_S)
                  .ty_.Derived(TypeKind::Function)
                  .ty_.addParam().Typedef("z")
                  .ty_.atParam().Derived(TypeKind::Pointer)
                  .ty_.Derived(TypeKind::Pointer)));
}

void DeclarationBinderTester::case0310(){}
void DeclarationBinderTester::case0311(){}
void DeclarationBinderTester::case0312(){}
void DeclarationBinderTester::case0313(){}
void DeclarationBinderTester::case0314(){}
void DeclarationBinderTester::case0315(){}
void DeclarationBinderTester::case0316(){}
void DeclarationBinderTester::case0317(){}
void DeclarationBinderTester::case0318(){}
void DeclarationBinderTester::case0319(){}
void DeclarationBinderTester::case0320(){}
void DeclarationBinderTester::case0321(){}
void DeclarationBinderTester::case0322(){}
void DeclarationBinderTester::case0323(){}
void DeclarationBinderTester::case0324(){}
void DeclarationBinderTester::case0325(){}
void DeclarationBinderTester::case0326(){}
void DeclarationBinderTester::case0327(){}
void DeclarationBinderTester::case0328(){}
void DeclarationBinderTester::case0329(){}
void DeclarationBinderTester::case0330(){}
void DeclarationBinderTester::case0331(){}
void DeclarationBinderTester::case0332(){}
void DeclarationBinderTester::case0333(){}
void DeclarationBinderTester::case0334(){}

void DeclarationBinderTester::case0335()
{
    bind("void x ( int y ( double ) ) ;",
         Expectation()
         .declaration(Decl()
                  .Function("x", ScopeKind::File)
                  .ty_.Void()
                  .ty_.Derived(TypeKind::Function)
                  .ty_.addParam().Basic(BasicTypeKind::Int_S)
                  .ty_.atParam().Derived(TypeKind::Function)
                  .ty_.atParam().addParam().Basic(BasicTypeKind::Double)
                  .ty_.atParam().Derived(TypeKind::Pointer, CVR::None, Decay::FromFunctionToFunctionPointer))
         .declaration(Decl()
                  .Object("y", ObjectDeclarationCategory::Parameter, ScopeKind::FunctionPrototype)
                  .ty_.Basic(BasicTypeKind::Int_S)
                  .ty_.Derived(TypeKind::Function)
                  .ty_.addParam().Basic(BasicTypeKind::Double)
                  .ty_.Derived(TypeKind::Pointer, CVR::None, Decay::FromFunctionToFunctionPointer)));
}

void DeclarationBinderTester::case0336(){}
void DeclarationBinderTester::case0337(){}
void DeclarationBinderTester::case0338(){}
void DeclarationBinderTester::case0339(){}
void DeclarationBinderTester::case0340(){}
void DeclarationBinderTester::case0341(){}
void DeclarationBinderTester::case0342(){}
void DeclarationBinderTester::case0343(){}
void DeclarationBinderTester::case0344(){}
void DeclarationBinderTester::case0345(){}
void DeclarationBinderTester::case0346(){}
void DeclarationBinderTester::case0347(){}
void DeclarationBinderTester::case0348(){}
void DeclarationBinderTester::case0349(){}

void DeclarationBinderTester::case0350()
{
    bind("void x ( int ( * y ) [ ] ) ;",
         Expectation()
         .declaration(Decl()
                  .Function("x", ScopeKind::File)
                  .ty_.Void()
                  .ty_.Derived(TypeKind::Function)
                  .ty_.addParam().Basic(BasicTypeKind::Int_S)
                  .ty_.atParam().Derived(TypeKind::Array)
                  .ty_.atParam().Derived(TypeKind::Pointer))
         .declaration(Decl()
                  .Object("y", ObjectDeclarationCategory::Parameter, ScopeKind::FunctionPrototype)
                  .ty_.Basic(BasicTypeKind::Int_S)
                  .ty_.Derived(TypeKind::Array)
                  .ty_.Derived(TypeKind::Pointer)));
}

void DeclarationBinderTester::case0351()
{
    bind("void x ( int ( * y ) [ 1 ] ) ;",
         Expectation()
         .declaration(Decl()
                  .Function("x", ScopeKind::File)
                  .ty_.Void()
                  .ty_.Derived(TypeKind::Function)
                  .ty_.addParam().Basic(BasicTypeKind::Int_S)
                  .ty_.atParam().Derived(TypeKind::Array)
                  .ty_.atParam().Derived(TypeKind::Pointer))
         .declaration(Decl()
                  .Object("y", ObjectDeclarationCategory::Parameter, ScopeKind::FunctionPrototype)
                  .ty_.Basic(BasicTypeKind::Int_S)
                  .ty_.Derived(TypeKind::Array)
                  .ty_.Derived(TypeKind::Pointer)));
}

void DeclarationBinderTester::case0352()
{
    bind("void x ( int * ( * y ) [ 1 ] ) ;",
         Expectation()
         .declaration(Decl()
                  .Function("x", ScopeKind::File)
                  .ty_.Void()
                  .ty_.Derived(TypeKind::Function)
                  .ty_.addParam().Basic(BasicTypeKind::Int_S)
                  .ty_.atParam().Derived(TypeKind::Pointer)
                  .ty_.atParam().Derived(TypeKind::Array)
                  .ty_.atParam().Derived(TypeKind::Pointer))
         .declaration(Decl()
                  .Object("y", ObjectDeclarationCategory::Parameter, ScopeKind::FunctionPrototype)
                  .ty_.Basic(BasicTypeKind::Int_S)
                  .ty_.Derived(TypeKind::Pointer)
                  .ty_.Derived(TypeKind::Array)
                  .ty_.Derived(TypeKind::Pointer)));
}

void DeclarationBinderTester::case0353()
{
    bind("void x ( y ( * z ) [ ] ) ;",
         Expectation()
         .declaration(Decl()
                  .Function("x", ScopeKind::File)
                  .ty_.Void()
                  .ty_.Derived(TypeKind::Function)
                  .ty_.addParam().Typedef("y")
                  .ty_.atParam().Derived(TypeKind::Array)
                  .ty_.atParam().Derived(TypeKind::Pointer))
         .declaration(Decl()
                  .Object("z", ObjectDeclarationCategory::Parameter, ScopeKind::FunctionPrototype)
                  .ty_.Typedef("y")
                  .ty_.Derived(TypeKind::Array)
                  .ty_.Derived(TypeKind::Pointer)));
}

void DeclarationBinderTester::case0354()
{
    bind("void x ( y * ( * z ) [ ] ) ;",
         Expectation()
         .declaration(Decl()
                  .Function("x", ScopeKind::File)
                  .ty_.Void()
                  .ty_.Derived(TypeKind::Function)
                  .ty_.addParam().Typedef("y")
                  .ty_.atParam().Derived(TypeKind::Pointer)
                  .ty_.atParam().Derived(TypeKind::Array)
                  .ty_.atParam().Derived(TypeKind::Pointer))
         .declaration(Decl()
                  .Object("z", ObjectDeclarationCategory::Parameter, ScopeKind::FunctionPrototype)
                  .ty_.Typedef("y")
                  .ty_.Derived(TypeKind::Pointer)
                  .ty_.Derived(TypeKind::Array)
                  .ty_.Derived(TypeKind::Pointer)));
}

void DeclarationBinderTester::case0355()
{
    bind("void x ( int w , y * ( * z ) [ 1 ] ) ;",
         Expectation()
         .declaration(Decl()
                  .Function("x", ScopeKind::File)
                  .ty_.Void()
                  .ty_.Derived(TypeKind::Function)
                  .ty_.addParam().Basic(BasicTypeKind::Int_S)
                  .ty_.addParam().Typedef("y")
                  .ty_.atParam().Derived(TypeKind::Pointer)
                  .ty_.atParam().Derived(TypeKind::Array)
                  .ty_.atParam().Derived(TypeKind::Pointer))
         .declaration(Decl()
                  .Object("w", ObjectDeclarationCategory::Parameter, ScopeKind::FunctionPrototype)
                  .ty_.Basic(BasicTypeKind::Int_S))
         .declaration(Decl()
                  .Object("z", ObjectDeclarationCategory::Parameter, ScopeKind::FunctionPrototype)
                  .ty_.Typedef("y")
                  .ty_.Derived(TypeKind::Pointer)
                  .ty_.Derived(TypeKind::Array)
                  .ty_.Derived(TypeKind::Pointer)));
}

void DeclarationBinderTester::case0356()
{
    bind("void x ( y * ( * z ) [ 1 ] , int w ) ;",
         Expectation()
         .declaration(Decl()
                  .Function("x", ScopeKind::File)
                  .ty_.Void()
                  .ty_.Derived(TypeKind::Function)
                  .ty_.addParam().Typedef("y")
                  .ty_.atParam().Derived(TypeKind::Pointer)
                  .ty_.atParam().Derived(TypeKind::Array)
                  .ty_.atParam().Derived(TypeKind::Pointer)
                  .ty_.addParam().Basic(BasicTypeKind::Int_S))
         .declaration(Decl()
                  .Object("w", ObjectDeclarationCategory::Parameter, ScopeKind::FunctionPrototype)
                  .ty_.Basic(BasicTypeKind::Int_S))
         .declaration(Decl()
                  .Object("z", ObjectDeclarationCategory::Parameter, ScopeKind::FunctionPrototype)
                  .ty_.Typedef("y")
                  .ty_.Derived(TypeKind::Pointer)
                  .ty_.Derived(TypeKind::Array)
                  .ty_.Derived(TypeKind::Pointer)));
}

void DeclarationBinderTester::case0357()
{
    bind("void x ( v w , y * ( * z ) [ 1 ] ) ;",
          Expectation()
          .declaration(Decl()
                   .Function("x", ScopeKind::File)
                   .ty_.Void()
                   .ty_.Derived(TypeKind::Function)
                   .ty_.addParam().Typedef("v")
                   .ty_.addParam().Typedef("y")
                   .ty_.atParam().Derived(TypeKind::Pointer)
                   .ty_.atParam().Derived(TypeKind::Array)
                   .ty_.atParam().Derived(TypeKind::Pointer))
          .declaration(Decl()
                   .Object("w", ObjectDeclarationCategory::Parameter, ScopeKind::FunctionPrototype)
                   .ty_.Typedef("v"))
          .declaration(Decl()
                   .Object("z", ObjectDeclarationCategory::Parameter, ScopeKind::FunctionPrototype)
                   .ty_.Typedef("y")
                   .ty_.Derived(TypeKind::Pointer)
                   .ty_.Derived(TypeKind::Array)
                   .ty_.Derived(TypeKind::Pointer)));
}

void DeclarationBinderTester::case0358()
{
    bind("void x ( y * ( * z ) [ 1 ] , v w ) ;",
         Expectation()
         .declaration(Decl()
                  .Function("x", ScopeKind::File)
                  .ty_.Void()
                  .ty_.Derived(TypeKind::Function)
                  .ty_.addParam().Typedef("y")
                  .ty_.atParam().Derived(TypeKind::Pointer)
                  .ty_.atParam().Derived(TypeKind::Array)
                  .ty_.atParam().Derived(TypeKind::Pointer)
                  .ty_.addParam().Typedef("v"))
         .declaration(Decl()
                  .Object("w", ObjectDeclarationCategory::Parameter, ScopeKind::FunctionPrototype)
                  .ty_.Typedef("v"))
         .declaration(Decl()
                  .Object("z", ObjectDeclarationCategory::Parameter, ScopeKind::FunctionPrototype)
                  .ty_.Typedef("y")
                  .ty_.Derived(TypeKind::Pointer)
                  .ty_.Derived(TypeKind::Array)
                  .ty_.Derived(TypeKind::Pointer)));
}

void DeclarationBinderTester::case0359(){}
void DeclarationBinderTester::case0360(){}
void DeclarationBinderTester::case0361(){}
void DeclarationBinderTester::case0362(){}
void DeclarationBinderTester::case0363(){}
void DeclarationBinderTester::case0364(){}
void DeclarationBinderTester::case0365(){}
void DeclarationBinderTester::case0366(){}
void DeclarationBinderTester::case0367(){}
void DeclarationBinderTester::case0368(){}
void DeclarationBinderTester::case0369(){}
void DeclarationBinderTester::case0370(){}
void DeclarationBinderTester::case0371(){}
void DeclarationBinderTester::case0372(){}
void DeclarationBinderTester::case0373(){}
void DeclarationBinderTester::case0374(){}
void DeclarationBinderTester::case0375(){}
void DeclarationBinderTester::case0376(){}
void DeclarationBinderTester::case0377(){}
void DeclarationBinderTester::case0378(){}
void DeclarationBinderTester::case0379(){}
void DeclarationBinderTester::case0380(){}
void DeclarationBinderTester::case0381(){}
void DeclarationBinderTester::case0382(){}
void DeclarationBinderTester::case0383(){}
void DeclarationBinderTester::case0384(){}

void DeclarationBinderTester::case0385()
{
    bind("void x ( int y [ ] ) ;",
         Expectation()
         .declaration(Decl()
                  .Function("x", ScopeKind::File)
                  .ty_.Void()
                  .ty_.Derived(TypeKind::Function)
                  .ty_.addParam().Basic(BasicTypeKind::Int_S)
                  .ty_.atParam().Derived(TypeKind::Pointer, CVR::None, Decay::FromArrayToPointer))
         .declaration(Decl()
                  .Object("y", ObjectDeclarationCategory::Parameter, ScopeKind::FunctionPrototype)
                  .ty_.Basic(BasicTypeKind::Int_S)
                  .ty_.Derived(TypeKind::Pointer, CVR::None, Decay::FromArrayToPointer)));
}

void DeclarationBinderTester::case0386()
{
    bind("void x ( y z [ ] ) ;",
         Expectation()
         .declaration(Decl()
                  .Function("x", ScopeKind::File)
                  .ty_.Void()
                  .ty_.Derived(TypeKind::Function)
                  .ty_.addParam().Typedef("y")
                  .ty_.atParam().Derived(TypeKind::Pointer, CVR::None, Decay::FromArrayToPointer))
         .declaration(Decl()
                  .Object("z", ObjectDeclarationCategory::Parameter, ScopeKind::FunctionPrototype)
                  .ty_.Typedef("y")
                  .ty_.Derived(TypeKind::Pointer, CVR::None, Decay::FromArrayToPointer)));
}

void DeclarationBinderTester::case0387()
{
    bind("void x ( int * y [ 1 ] ) ;",
         Expectation()
         .declaration(Decl()
                  .Function("x", ScopeKind::File)
                  .ty_.Void()
                  .ty_.Derived(TypeKind::Function)
                  .ty_.addParam().Basic(BasicTypeKind::Int_S)
                  .ty_.atParam().Derived(TypeKind::Pointer)
                  .ty_.atParam().Derived(TypeKind::Pointer, CVR::None, Decay::FromArrayToPointer))
         .declaration(Decl()
                  .Object("y", ObjectDeclarationCategory::Parameter, ScopeKind::FunctionPrototype)
                  .ty_.Basic(BasicTypeKind::Int_S)
                  .ty_.Derived(TypeKind::Pointer)
                  .ty_.Derived(TypeKind::Pointer, CVR::None, Decay::FromArrayToPointer)));
}

void DeclarationBinderTester::case0388()
{
    bind("void x ( y * z [ 1 ] ) ;",
         Expectation()
         .declaration(Decl()
                  .Function("x", ScopeKind::File)
                  .ty_.Void()
                  .ty_.Derived(TypeKind::Function)
                  .ty_.addParam().Typedef("y")
                  .ty_.atParam().Derived(TypeKind::Pointer)
                  .ty_.atParam().Derived(TypeKind::Pointer, CVR::None, Decay::FromArrayToPointer))
         .declaration(Decl()
                  .Object("z", ObjectDeclarationCategory::Parameter, ScopeKind::FunctionPrototype)
                  .ty_.Typedef("y")
                  .ty_.Derived(TypeKind::Pointer)
                  .ty_.Derived(TypeKind::Pointer, CVR::None, Decay::FromArrayToPointer)));
}

void DeclarationBinderTester::case0389()
{
    bind("void x ( int * * y [ 1 ] ) ;",
         Expectation()
         .declaration(Decl()
                  .Function("x", ScopeKind::File)
                  .ty_.Void()
                  .ty_.Derived(TypeKind::Function)
                  .ty_.addParam().Basic(BasicTypeKind::Int_S)
                  .ty_.atParam().Derived(TypeKind::Pointer)
                  .ty_.atParam().Derived(TypeKind::Pointer)
                  .ty_.atParam().Derived(TypeKind::Pointer, CVR::None, Decay::FromArrayToPointer))
         .declaration(Decl()
                  .Object("y", ObjectDeclarationCategory::Parameter, ScopeKind::FunctionPrototype)
                  .ty_.Basic(BasicTypeKind::Int_S)
                  .ty_.Derived(TypeKind::Pointer)
                  .ty_.Derived(TypeKind::Pointer)
                  .ty_.Derived(TypeKind::Pointer, CVR::None, Decay::FromArrayToPointer)));
}

void DeclarationBinderTester::case0390()
{
    bind("void x ( y const * z [ 1 ] ) ;",
         Expectation()
         .declaration(Decl()
                  .Function("x", ScopeKind::File)
                  .ty_.Void()
                  .ty_.Derived(TypeKind::Function)
                  .ty_.addParam().Typedef("y", CVR::Const)
                  .ty_.atParam().Derived(TypeKind::Pointer)
                  .ty_.atParam().Derived(TypeKind::Pointer, CVR::None, Decay::FromArrayToPointer))
         .declaration(Decl()
                  .Object("z", ObjectDeclarationCategory::Parameter, ScopeKind::FunctionPrototype)
                  .ty_.Typedef("y", CVR::Const)
                  .ty_.Derived(TypeKind::Pointer)
                  .ty_.Derived(TypeKind::Pointer, CVR::None, Decay::FromArrayToPointer)));
}

void DeclarationBinderTester::case0391()
{
    bind("void x ( y z [ ] , w z ) ;",
         Expectation()
         .declaration(Decl()
                  .Function("x", ScopeKind::File)
                  .ty_.Void()
                  .ty_.Derived(TypeKind::Function)
                  .ty_.addParam().Typedef("y")
                  .ty_.atParam().Derived(TypeKind::Pointer, CVR::None, Decay::FromArrayToPointer)
                  .ty_.addParam().Typedef("w"))
         .declaration(Decl()
                  .Object("z", ObjectDeclarationCategory::Parameter, ScopeKind::FunctionPrototype)
                  .ty_.Typedef("w"))
         .declaration(Decl()
                  .Object("z", ObjectDeclarationCategory::Parameter, ScopeKind::FunctionPrototype)
                  .ty_.Typedef("y")
                  .ty_.Derived(TypeKind::Pointer, CVR::None, Decay::FromArrayToPointer)));
}

void DeclarationBinderTester::case0392()
{
    bind("void x ( w z , y z [ ] ) ;",
         Expectation()
         .declaration(Decl()
                  .Function("x", ScopeKind::File)
                  .ty_.Void()
                  .ty_.Derived(TypeKind::Function)
                  .ty_.addParam().Typedef("w")
                  .ty_.addParam().Typedef("y")
                  .ty_.atParam().Derived(TypeKind::Pointer, CVR::None, Decay::FromArrayToPointer))
         .declaration(Decl()
                  .Object("z", ObjectDeclarationCategory::Parameter, ScopeKind::FunctionPrototype)
                  .ty_.Typedef("w"))
         .declaration(Decl()
                  .Object("z", ObjectDeclarationCategory::Parameter, ScopeKind::FunctionPrototype)
                  .ty_.Typedef("y")
                  .ty_.Derived(TypeKind::Pointer, CVR::None, Decay::FromArrayToPointer)));
}

void DeclarationBinderTester::case0393(){}
void DeclarationBinderTester::case0394(){}
void DeclarationBinderTester::case0395(){}
void DeclarationBinderTester::case0396(){}
void DeclarationBinderTester::case0397(){}
void DeclarationBinderTester::case0398(){}
void DeclarationBinderTester::case0399(){}

void DeclarationBinderTester::case0400()
{
    bind("void ( * x ( ) ) ( ) ;",
         Expectation()
         .declaration(Decl()
                  .Function("x", ScopeKind::File)
                  .ty_.Void()
                  .ty_.Derived(TypeKind::Function)
                  .ty_.Derived(TypeKind::Pointer)
                  .ty_.nestAsReturn()
                  .ty_.Derived(TypeKind::Function)));
}

void DeclarationBinderTester::case0401()
{
    bind("void ( * x ( ) ) ( int ) ;",
         Expectation()
         .declaration(Decl()
                  .Function("x", ScopeKind::File)
                  .ty_.Void()
                  .ty_.Derived(TypeKind::Function)
                  .ty_.addParam().Basic(BasicTypeKind::Int_S)
                  .ty_.Derived(TypeKind::Pointer)
                  .ty_.nestAsReturn()
                  .ty_.Derived(TypeKind::Function)));
}

void DeclarationBinderTester::case0402()
{
    bind("void ( * x ( int ) ) ( ) ;",
         Expectation()
         .declaration(Decl()
                  .Function("x", ScopeKind::File)
                  .ty_.Void()
                  .ty_.Derived(TypeKind::Function)
                  .ty_.Derived(TypeKind::Pointer)
                  .ty_.nestAsReturn()
                  .ty_.Derived(TypeKind::Function)
                  .ty_.addParam().Basic(BasicTypeKind::Int_S)));
}

void DeclarationBinderTester::case0403()
{
    bind("void ( * x ( double ) ) ( int ) ;",
         Expectation()
         .declaration(Decl()
                  .Function("x", ScopeKind::File)
                  .ty_.Void()
                  .ty_.Derived(TypeKind::Function)
                  .ty_.addParam().Basic(BasicTypeKind::Int_S)
                  .ty_.Derived(TypeKind::Pointer)
                  .ty_.nestAsReturn()
                  .ty_.Derived(TypeKind::Function)
                  .ty_.addParam().Basic(BasicTypeKind::Double)));
}

void DeclarationBinderTester::case0404()
{
    bind("void * ( * x ( double ) ) ( int ) ;",
         Expectation()
         .declaration(Decl()
                  .Function("x", ScopeKind::File)
                  .ty_.Void()
                  .ty_.Derived(TypeKind::Pointer)
                  .ty_.Derived(TypeKind::Function)
                  .ty_.addParam().Basic(BasicTypeKind::Int_S)
                  .ty_.Derived(TypeKind::Pointer)
                  .ty_.nestAsReturn()
                  .ty_.Derived(TypeKind::Function)
                  .ty_.addParam().Basic(BasicTypeKind::Double)));
}

void DeclarationBinderTester::case0405()
{
    bind("void * ( * x ( double , char ) ) ( int , long ) ;",
         Expectation()
         .declaration(Decl()
                  .Function("x", ScopeKind::File)
                  .ty_.Void()
                  .ty_.Derived(TypeKind::Pointer)
                  .ty_.Derived(TypeKind::Function)
                  .ty_.addParam().Basic(BasicTypeKind::Int_S)
                  .ty_.addParam().Basic(BasicTypeKind::Long_S)
                  .ty_.Derived(TypeKind::Pointer)
                  .ty_.nestAsReturn()
                  .ty_.Derived(TypeKind::Function)
                  .ty_.addParam().Basic(BasicTypeKind::Double)
                  .ty_.addParam().Basic(BasicTypeKind::Char)));
}

void DeclarationBinderTester::case0406()
{
    bind("void * ( * x ( double * , char ) ) ( void * ) ;",
         Expectation()
         .declaration(Decl()
                  .Function("x", ScopeKind::File)
                  .ty_.Void()
                  .ty_.Derived(TypeKind::Pointer)
                  .ty_.Derived(TypeKind::Function)
                  .ty_.addParam().Void()
                  .ty_.atParam().Derived(TypeKind::Pointer)
                  .ty_.Derived(TypeKind::Pointer)
                  .ty_.nestAsReturn()
                  .ty_.Derived(TypeKind::Function)
                  .ty_.addParam().Basic(BasicTypeKind::Double)
                  .ty_.atParam().Derived(TypeKind::Pointer)
                  .ty_.addParam().Basic(BasicTypeKind::Char)));
}

void DeclarationBinderTester::case0407()
{
    bind("void ( * x ( ) ) ( y ) ;",
         Expectation()
         .declaration(Decl()
                  .Function("x", ScopeKind::File)
                  .ty_.Void()
                  .ty_.Derived(TypeKind::Function)
                  .ty_.addParam().Typedef("y")
                  .ty_.Derived(TypeKind::Pointer)
                  .ty_.nestAsReturn()
                  .ty_.Derived(TypeKind::Function)));
}

void DeclarationBinderTester::case0408()
{
    bind("void ( * x ( y ) ) ( ) ;",
         Expectation()
         .declaration(Decl()
                  .Function("x", ScopeKind::File)
                  .ty_.Void()
                  .ty_.Derived(TypeKind::Function)
                  .ty_.Derived(TypeKind::Pointer)
                  .ty_.nestAsReturn()
                  .ty_.Derived(TypeKind::Function)
                  .ty_.addParam().Typedef("y")));
}

void DeclarationBinderTester::case0409()
{
    bind("void ( * x ( y ) ) ( z ) ;",
         Expectation()
         .declaration(Decl()
                  .Function("x", ScopeKind::File)
                  .ty_.Void()
                  .ty_.Derived(TypeKind::Function)
                  .ty_.addParam().Typedef("z")
                  .ty_.Derived(TypeKind::Pointer)
                  .ty_.nestAsReturn()
                  .ty_.Derived(TypeKind::Function)
                  .ty_.addParam().Typedef("y")));
}

void DeclarationBinderTester::case0410()
{
    bind("void ( * x ( y * ) ) ( z ) ;",
         Expectation()
         .declaration(Decl()
                  .Function("x", ScopeKind::File)
                  .ty_.Void()
                  .ty_.Derived(TypeKind::Function)
                  .ty_.addParam().Typedef("z")
                  .ty_.Derived(TypeKind::Pointer)
                  .ty_.nestAsReturn()
                  .ty_.Derived(TypeKind::Function)
                  .ty_.addParam().Typedef("y")
                  .ty_.atParam().Derived(TypeKind::Pointer)));
}

void DeclarationBinderTester::case0411()
{
    // The `signal' function: http://c-faq.com/decl/spiral.anderson.html

    bind(" void ( * x ( int , void ( * y ) ( int ) ) ) ( int ) ;",
         Expectation()
         .declaration(Decl()
                  .Function("x", ScopeKind::File)
                  .ty_.Void()
                  .ty_.Derived(TypeKind::Function)
                  .ty_.addParam().Basic(BasicTypeKind::Int_S)
                  .ty_.Derived(TypeKind::Pointer)
                  .ty_.nestAsReturn()
                  .ty_.Derived(TypeKind::Function)
                  .ty_.addParam().Basic(BasicTypeKind::Int_S)
                  .ty_.addParam().Void()
                  .ty_.atParam().Derived(TypeKind::Function)
                  .ty_.atParam().addParam().Basic(BasicTypeKind::Int_S)
                  .ty_.atParam().Derived(TypeKind::Pointer)));
}

void DeclarationBinderTester::case0412()
{
    bind("x ( * y ( ) ) ( ) ;",
         Expectation()
         .declaration(Decl()
                  .Function("y", ScopeKind::File)
                  .ty_.Typedef("x")
                  .ty_.Derived(TypeKind::Function)
                  .ty_.Derived(TypeKind::Pointer)
                  .ty_.nestAsReturn()
                  .ty_.Derived(TypeKind::Function)));
}
void DeclarationBinderTester::case0413()
{
    bind("void x ( ) ( ) ;",
         Expectation()
         .diagnostic(Expectation::ErrorOrWarn::Error,
                     DeclarationBinder::DiagnosticsReporter::ID_of_FunctionReturningFunction)
         .ContinueTestDespiteOfErrors()
         .declaration(Decl()
                  .Function("x", ScopeKind::File)
                  .ty_.Void()
                  .ty_.Derived(TypeKind::Function)
                  .ty_.nestAsReturn()
                  .ty_.Derived(TypeKind::Function)));
}

void DeclarationBinderTester::case0414()
{
    bind("void ( x ( ) ) ( ) ;",
         Expectation()
         .diagnostic(Expectation::ErrorOrWarn::Error,
                     DeclarationBinder::DiagnosticsReporter::ID_of_FunctionReturningFunction)
         .ContinueTestDespiteOfErrors()
         .declaration(Decl()
                  .Function("x", ScopeKind::File)
                  .ty_.Void()
                  .ty_.Derived(TypeKind::Function)
                  .ty_.nestAsReturn()
                  .ty_.Derived(TypeKind::Function)));
}
void DeclarationBinderTester::case0415(){}
void DeclarationBinderTester::case0416(){}
void DeclarationBinderTester::case0417(){}
void DeclarationBinderTester::case0418(){}
void DeclarationBinderTester::case0419(){}
void DeclarationBinderTester::case0420(){}
void DeclarationBinderTester::case0421(){}
void DeclarationBinderTester::case0422(){}
void DeclarationBinderTester::case0423(){}
void DeclarationBinderTester::case0424(){}

void DeclarationBinderTester::case0425()
{
    bind("int ( * x ( ) ) [] ;",
         Expectation()
         .declaration(Decl()
                  .Function("x", ScopeKind::File)
                  .ty_.Basic(BasicTypeKind::Int_S)
                  .ty_.Derived(TypeKind::Array)
                  .ty_.Derived(TypeKind::Pointer)
                  .ty_.nestAsReturn()
                  .ty_.Derived(TypeKind::Function)));
}

void DeclarationBinderTester::case0426()
{
    bind("x ( * y ( ) ) [] ;",
         Expectation()
         .declaration(Decl()
                  .Function("y", ScopeKind::File)
                  .ty_.Typedef("x")
                  .ty_.Derived(TypeKind::Array)
                  .ty_.Derived(TypeKind::Pointer)
                  .ty_.nestAsReturn()
                  .ty_.Derived(TypeKind::Function)));
}

void DeclarationBinderTester::case0427()
{
    bind("int x ( ) [] ;",
         Expectation()
         .diagnostic(Expectation::ErrorOrWarn::Error,
                     DeclarationBinder::DiagnosticsReporter::ID_of_FunctionReturningArray)
         .ContinueTestDespiteOfErrors()
         .declaration(Decl()
                  .Function("x", ScopeKind::File)
                  .ty_.Basic(BasicTypeKind::Int_S)
                  .ty_.Derived(TypeKind::Array)
                  .ty_.nestAsReturn()
                  .ty_.Derived(TypeKind::Function)));
}

void DeclarationBinderTester::case0428()
{
    bind("int ( x ) ( ) [] ;",
         Expectation()
         .diagnostic(Expectation::ErrorOrWarn::Error,
                     DeclarationBinder::DiagnosticsReporter::ID_of_FunctionReturningArray)
         .ContinueTestDespiteOfErrors()
         .declaration(Decl()
                  .Function("x", ScopeKind::File)
                  .ty_.Basic(BasicTypeKind::Int_S)
                  .ty_.Derived(TypeKind::Array)
                  .ty_.nestAsReturn()
                  .ty_.Derived(TypeKind::Function)));
}

void DeclarationBinderTester::case0429(){}
void DeclarationBinderTester::case0430(){}
void DeclarationBinderTester::case0431(){}
void DeclarationBinderTester::case0432(){}
void DeclarationBinderTester::case0433(){}
void DeclarationBinderTester::case0434(){}
void DeclarationBinderTester::case0435(){}
void DeclarationBinderTester::case0436(){}
void DeclarationBinderTester::case0437(){}
void DeclarationBinderTester::case0438(){}
void DeclarationBinderTester::case0439(){}
void DeclarationBinderTester::case0440()
{
}

void DeclarationBinderTester::case0441(){}
void DeclarationBinderTester::case0442(){}
void DeclarationBinderTester::case0443(){}
void DeclarationBinderTester::case0444(){}
void DeclarationBinderTester::case0445(){}
void DeclarationBinderTester::case0446(){}
void DeclarationBinderTester::case0447(){}
void DeclarationBinderTester::case0448(){}
void DeclarationBinderTester::case0449(){}

void DeclarationBinderTester::case0450()
{
    bind("void x ( ) { int y ; }",
         Expectation()
         .declaration(Decl()
                  .Function("x", ScopeKind::File)
                  .ty_.Void()
                  .ty_.Derived(TypeKind::Function))
         .declaration(Decl()
                  .Object("y", ObjectDeclarationCategory::Variable, ScopeKind::Block)
                  .ty_.Basic(BasicTypeKind::Int_S)));
}

void DeclarationBinderTester::case0451()
{
    bind("void x ( ) { int * y ; double z ; }",
         Expectation()
         .declaration(Decl()
                  .Function("x", ScopeKind::File)
                  .ty_.Void()
                  .ty_.Derived(TypeKind::Function))
         .declaration(Decl()
                  .Object("y", ObjectDeclarationCategory::Variable, ScopeKind::Block)
                  .ty_.Basic(BasicTypeKind::Int_S)
                  .ty_.Derived(TypeKind::Pointer))
         .declaration(Decl()
                  .Object("z", ObjectDeclarationCategory::Variable, ScopeKind::Block)
                  .ty_.Basic(BasicTypeKind::Double)));
}

void DeclarationBinderTester::case0452(){}
void DeclarationBinderTester::case0453(){}
void DeclarationBinderTester::case0454(){}
void DeclarationBinderTester::case0455(){}
void DeclarationBinderTester::case0456(){}
void DeclarationBinderTester::case0457(){}
void DeclarationBinderTester::case0458(){}
void DeclarationBinderTester::case0459(){}
void DeclarationBinderTester::case0460(){}
void DeclarationBinderTester::case0461(){}
void DeclarationBinderTester::case0462(){}
void DeclarationBinderTester::case0463(){}
void DeclarationBinderTester::case0464(){}
void DeclarationBinderTester::case0465(){}
void DeclarationBinderTester::case0466(){}
void DeclarationBinderTester::case0467(){}
void DeclarationBinderTester::case0468(){}
void DeclarationBinderTester::case0469(){}
void DeclarationBinderTester::case0470(){}
void DeclarationBinderTester::case0471(){}
void DeclarationBinderTester::case0472(){}
void DeclarationBinderTester::case0473(){}
void DeclarationBinderTester::case0474(){}
void DeclarationBinderTester::case0475(){}
void DeclarationBinderTester::case0476(){}
void DeclarationBinderTester::case0477(){}
void DeclarationBinderTester::case0478(){}
void DeclarationBinderTester::case0479(){}
void DeclarationBinderTester::case0480(){}
void DeclarationBinderTester::case0481(){}
void DeclarationBinderTester::case0482(){}
void DeclarationBinderTester::case0483(){}
void DeclarationBinderTester::case0484(){}
void DeclarationBinderTester::case0485(){}
void DeclarationBinderTester::case0486(){}
void DeclarationBinderTester::case0487(){}
void DeclarationBinderTester::case0488(){}
void DeclarationBinderTester::case0489(){}
void DeclarationBinderTester::case0490(){}
void DeclarationBinderTester::case0491(){}
void DeclarationBinderTester::case0492(){}
void DeclarationBinderTester::case0493(){}
void DeclarationBinderTester::case0494(){}
void DeclarationBinderTester::case0495(){}
void DeclarationBinderTester::case0496(){}
void DeclarationBinderTester::case0497(){}
void DeclarationBinderTester::case0498(){}
void DeclarationBinderTester::case0499(){}

void DeclarationBinderTester::case0500(){}
void DeclarationBinderTester::case0501(){}
void DeclarationBinderTester::case0502(){}
void DeclarationBinderTester::case0503(){}
void DeclarationBinderTester::case0504(){}
void DeclarationBinderTester::case0505(){}
void DeclarationBinderTester::case0506(){}
void DeclarationBinderTester::case0507(){}
void DeclarationBinderTester::case0508(){}
void DeclarationBinderTester::case0509(){}
void DeclarationBinderTester::case0510(){}
void DeclarationBinderTester::case0511(){}
void DeclarationBinderTester::case0512(){}
void DeclarationBinderTester::case0513(){}
void DeclarationBinderTester::case0514(){}
void DeclarationBinderTester::case0515(){}
void DeclarationBinderTester::case0516(){}
void DeclarationBinderTester::case0517(){}
void DeclarationBinderTester::case0518(){}
void DeclarationBinderTester::case0519(){}
void DeclarationBinderTester::case0520(){}
void DeclarationBinderTester::case0521(){}
void DeclarationBinderTester::case0522(){}
void DeclarationBinderTester::case0523(){}
void DeclarationBinderTester::case0524(){}
void DeclarationBinderTester::case0525(){}
void DeclarationBinderTester::case0526(){}
void DeclarationBinderTester::case0527(){}
void DeclarationBinderTester::case0528(){}
void DeclarationBinderTester::case0529(){}
void DeclarationBinderTester::case0530(){}
void DeclarationBinderTester::case0531(){}
void DeclarationBinderTester::case0532(){}
void DeclarationBinderTester::case0533(){}
void DeclarationBinderTester::case0534(){}
void DeclarationBinderTester::case0535(){}
void DeclarationBinderTester::case0536(){}
void DeclarationBinderTester::case0537(){}
void DeclarationBinderTester::case0538(){}
void DeclarationBinderTester::case0539(){}
void DeclarationBinderTester::case0540(){}
void DeclarationBinderTester::case0541(){}
void DeclarationBinderTester::case0542(){}
void DeclarationBinderTester::case0543(){}
void DeclarationBinderTester::case0544(){}
void DeclarationBinderTester::case0545(){}
void DeclarationBinderTester::case0546(){}
void DeclarationBinderTester::case0547(){}
void DeclarationBinderTester::case0548(){}
void DeclarationBinderTester::case0549(){}
void DeclarationBinderTester::case0550(){}
void DeclarationBinderTester::case0551(){}
void DeclarationBinderTester::case0552(){}
void DeclarationBinderTester::case0553(){}
void DeclarationBinderTester::case0554(){}
void DeclarationBinderTester::case0555(){}
void DeclarationBinderTester::case0556(){}
void DeclarationBinderTester::case0557(){}
void DeclarationBinderTester::case0558(){}
void DeclarationBinderTester::case0559(){}
void DeclarationBinderTester::case0560(){}
void DeclarationBinderTester::case0561(){}
void DeclarationBinderTester::case0562(){}
void DeclarationBinderTester::case0563(){}
void DeclarationBinderTester::case0564(){}
void DeclarationBinderTester::case0565(){}
void DeclarationBinderTester::case0566(){}
void DeclarationBinderTester::case0567(){}
void DeclarationBinderTester::case0568(){}
void DeclarationBinderTester::case0569(){}
void DeclarationBinderTester::case0570(){}
void DeclarationBinderTester::case0571(){}
void DeclarationBinderTester::case0572(){}
void DeclarationBinderTester::case0573(){}
void DeclarationBinderTester::case0574(){}
void DeclarationBinderTester::case0575(){}
void DeclarationBinderTester::case0576(){}
void DeclarationBinderTester::case0577(){}
void DeclarationBinderTester::case0578(){}
void DeclarationBinderTester::case0579(){}
void DeclarationBinderTester::case0580(){}
void DeclarationBinderTester::case0581(){}
void DeclarationBinderTester::case0582(){}
void DeclarationBinderTester::case0583(){}
void DeclarationBinderTester::case0584(){}
void DeclarationBinderTester::case0585(){}
void DeclarationBinderTester::case0586(){}
void DeclarationBinderTester::case0587(){}
void DeclarationBinderTester::case0588(){}
void DeclarationBinderTester::case0589(){}
void DeclarationBinderTester::case0590(){}
void DeclarationBinderTester::case0591(){}
void DeclarationBinderTester::case0592(){}
void DeclarationBinderTester::case0593(){}
void DeclarationBinderTester::case0594(){}
void DeclarationBinderTester::case0595(){}
void DeclarationBinderTester::case0596(){}
void DeclarationBinderTester::case0597(){}
void DeclarationBinderTester::case0598(){}
void DeclarationBinderTester::case0599(){}






void DeclarationBinderTester::case0901()
{
    CROSS_REFERENCE_TEST(ParserTester::case0028);

    bind("const int ;");
}

void DeclarationBinderTester::case0902()
{
    bind("int x ;");
}

void DeclarationBinderTester::case0903()
{
    bind("void x ( ) ;");
}

void DeclarationBinderTester::case0904()
{
    bind("int x ( ) ;");
}

void DeclarationBinderTester::case0905()
{
    CROSS_REFERENCE_TEST(ParserTester::case0021);

    bind("int double x ;");
}

void DeclarationBinderTester::case0906()
{
    CROSS_REFERENCE_TEST(ParserTester::case0416);

    bind("struct x { y ; } ;");
}

void DeclarationBinderTester::case0907()
{
    CROSS_REFERENCE_TEST(ParserTester::case0422);

    bind("struct x { int ; } ;");
}

void DeclarationBinderTester::case0908()
{
    CROSS_REFERENCE_TEST(ParserTester::case0204);

    // Function returning a function.

    bind("void x ( ) ( ) { }");
}

void DeclarationBinderTester::case0909()
{
    CROSS_REFERENCE_TEST(ParserTester::case0205);

    // Function returning a function.

    bind("void x ( ) ( ) ;");
}

void DeclarationBinderTester::case0910()
{
    CROSS_REFERENCE_TEST(ParserTester::case0206);

    // Function returning an array.

    bind("int x ( ) [ ] ;");
}

void DeclarationBinderTester::case0911()
{
    CROSS_REFERENCE_TEST(ParserTester::case0207);

    bind("int ( x ) ( ) [ ] ;");
}

void DeclarationBinderTester::case0912()
{
    CROSS_REFERENCE_TEST(ParserTester::case0212);

    // 6.9.1-5

    bind("void x ( int ) { }");
}

void DeclarationBinderTester::case0913()
{
    CROSS_REFERENCE_TEST(ParserTester::case0040);

    bind("int ;");
}

void DeclarationBinderTester::case0914()
{
    CROSS_REFERENCE_TEST(ParserTester::case0406);

    bind("struct { int x ; } ;");
}

void DeclarationBinderTester::case0915()
{
    CROSS_REFERENCE_TEST(ParserTester::case0632);

    bind("void x ( ) { __thread x y ; }");
}

void DeclarationBinderTester::case0916()
{
    CROSS_REFERENCE_TEST(ParserTester::case0050);

    bind("double _Complex int x ;");
}

void DeclarationBinderTester::case0917()
{
    CROSS_REFERENCE_TEST(ParserTester::case0052);

    bind("int int x ;");
}

void DeclarationBinderTester::case0918()
{
    CROSS_REFERENCE_TEST(ParserTester::case0053);

    bind("_Complex _Complex x ;");
}

void DeclarationBinderTester::case0919()
{
    CROSS_REFERENCE_TEST(ParserTester::case0435);

    bind("int struct x { int y ; } z ;");
}

void DeclarationBinderTester::case0920()
{
    CROSS_REFERENCE_TEST(ParserTester::case0436);

    bind("struct x struct { int y ; } z ;");
}

void DeclarationBinderTester::case0921()
{
    CROSS_REFERENCE_TEST(ParserTester::case0437);

    bind("int struct x y ;");
}

void DeclarationBinderTester::case0922()
{
    CROSS_REFERENCE_TEST(ParserTester::case0438);

    bind("struct x int y ;");
}

void DeclarationBinderTester::case0923()
{
    CROSS_REFERENCE_TEST(ParserTester::case0737);

    bind("int __attribute__ ( ( ) ) double ;");
}

void DeclarationBinderTester::case0924()
{
    CROSS_REFERENCE_TEST(ParserTester::case0738);

    bind("x __attribute__ ( ( ) ) int ;");
}

void DeclarationBinderTester::case0925()
{
    CROSS_REFERENCE_TEST(ParserTester::case0297);

    bind("void x ( int y ) int y ; { }");
}

void DeclarationBinderTester::case0926()
{
    CROSS_REFERENCE_TEST(ParserTester::case0298);

    bind("int x ( int y ) z y ; { }");
}

void DeclarationBinderTester::case0927()
{
    bind("*1;");
}

void DeclarationBinderTester::case0928()
{
    bind("&1;");
}

void DeclarationBinderTester::case0929()
{
    CROSS_REFERENCE_TEST(ParserTester::case0446);

    bind("struct x { struct y { int z ; } ; } ;");
}

void DeclarationBinderTester::case0930(){}
void DeclarationBinderTester::case0931(){}
void DeclarationBinderTester::case0932(){}
void DeclarationBinderTester::case0933(){}
void DeclarationBinderTester::case0934(){}
void DeclarationBinderTester::case0935(){}
void DeclarationBinderTester::case0936(){}
void DeclarationBinderTester::case0937(){}
void DeclarationBinderTester::case0938(){}
void DeclarationBinderTester::case0939(){}
void DeclarationBinderTester::case0940(){}
void DeclarationBinderTester::case0941(){}
void DeclarationBinderTester::case0942(){}
void DeclarationBinderTester::case0943(){}
void DeclarationBinderTester::case0944(){}
void DeclarationBinderTester::case0945(){}
void DeclarationBinderTester::case0946(){}
void DeclarationBinderTester::case0947(){}
void DeclarationBinderTester::case0948(){}
void DeclarationBinderTester::case0949(){}
void DeclarationBinderTester::case0950(){}
void DeclarationBinderTester::case0951(){}
void DeclarationBinderTester::case0952(){}
void DeclarationBinderTester::case0953(){}
void DeclarationBinderTester::case0954(){}
void DeclarationBinderTester::case0955(){}
void DeclarationBinderTester::case0956(){}
void DeclarationBinderTester::case0957(){}
void DeclarationBinderTester::case0958(){}
void DeclarationBinderTester::case0959(){}
void DeclarationBinderTester::case0960(){}
void DeclarationBinderTester::case0961(){}
void DeclarationBinderTester::case0962(){}
void DeclarationBinderTester::case0963(){}
void DeclarationBinderTester::case0964(){}
void DeclarationBinderTester::case0965(){}
void DeclarationBinderTester::case0966(){}
void DeclarationBinderTester::case0967(){}
void DeclarationBinderTester::case0968(){}
void DeclarationBinderTester::case0969(){}
void DeclarationBinderTester::case0970(){}
void DeclarationBinderTester::case0971(){}
void DeclarationBinderTester::case0972(){}
void DeclarationBinderTester::case0973(){}
void DeclarationBinderTester::case0974(){}
void DeclarationBinderTester::case0975(){}
void DeclarationBinderTester::case0976(){}
void DeclarationBinderTester::case0977(){}
void DeclarationBinderTester::case0978(){}
void DeclarationBinderTester::case0979(){}
void DeclarationBinderTester::case0980(){}
void DeclarationBinderTester::case0981(){}
void DeclarationBinderTester::case0982(){}
void DeclarationBinderTester::case0983(){}
void DeclarationBinderTester::case0984(){}
void DeclarationBinderTester::case0985(){}
void DeclarationBinderTester::case0986(){}
void DeclarationBinderTester::case0987(){}
void DeclarationBinderTester::case0988(){}
void DeclarationBinderTester::case0989(){}
void DeclarationBinderTester::case0990(){}
void DeclarationBinderTester::case0991(){}
void DeclarationBinderTester::case0992(){}
void DeclarationBinderTester::case0993(){}
void DeclarationBinderTester::case0994(){}
void DeclarationBinderTester::case0995(){}
void DeclarationBinderTester::case0996(){}
void DeclarationBinderTester::case0997(){}
void DeclarationBinderTester::case0998(){}
void DeclarationBinderTester::case0999(){}
