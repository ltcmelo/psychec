// Copyright (c) 2024 Leandro T. C. Melo <ltcmelo@gmail.com>
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

#include "TypeCanonicalizerAndResolverTester.h"

#include "sema/TypeCanonicalizer.h"

using namespace psy;
using namespace C;

const std::string TypeCanonicalizerAndResolverTester::Name = "TYPE-RESOLVER";

void TypeCanonicalizerAndResolverTester::testTypeCanonicalizerAndResolver()
{
    return run<TypeCanonicalizerAndResolverTester>(tests_);
}

void TypeCanonicalizerAndResolverTester::canonicalizerAndResolveTypes(std::string text, Expectation X)
{
    (static_cast<InternalsTestSuite*>(suite_)->canonicalizerAndResolveTypes(text, X));
}

void TypeCanonicalizerAndResolverTester::case0000()
{
    auto s = R"(
typedef double x ;
x y ;
)";

    canonicalizerAndResolveTypes(s,
            Expectation()
            .declaration(Decl()
                         .Object("y", ObjectDeclarationCategory::Variable)
                         .ty_.Basic(BasicTypeKind::Double)));
}

void TypeCanonicalizerAndResolverTester::case0001()
{
    auto s = R"(
typedef double x ;
x * y ;
)";

    canonicalizerAndResolveTypes(s,
            Expectation()
            .declaration(Decl()
                         .Object("y", ObjectDeclarationCategory::Variable)
                         .ty_.Derived(TypeKind::Pointer)
                         .ty_.Basic(BasicTypeKind::Double)));
}

void TypeCanonicalizerAndResolverTester::case0002()
{
    auto s = R"(
typedef double x ;
x const y ;
)";

    canonicalizerAndResolveTypes(s,
            Expectation()
            .declaration(Decl()
                         .Object("y", ObjectDeclarationCategory::Variable)
                         .ty_.Basic(BasicTypeKind::Double, CVR::Const)));
}

void TypeCanonicalizerAndResolverTester::case0003()
{
    auto s = R"(
typedef double const x ;
x const y ;
)";

    canonicalizerAndResolveTypes(s,
            Expectation()
            .declaration(Decl()
                         .Object("y", ObjectDeclarationCategory::Variable)
                         .ty_.Basic(BasicTypeKind::Double, CVR::Const)));
}

void TypeCanonicalizerAndResolverTester::case0004()
{
    auto s = R"(
typedef double x ;
x * * y ;
)";

    canonicalizerAndResolveTypes(s,
            Expectation()
            .declaration(Decl()
                         .Object("y", ObjectDeclarationCategory::Variable)
                         .ty_.Derived(TypeKind::Pointer)
                         .ty_.Derived(TypeKind::Pointer)
                         .ty_.Basic(BasicTypeKind::Double)));
}

void TypeCanonicalizerAndResolverTester::case0005()
{
    auto s = R"(
typedef double x ;
x y [ 1 ] ;
)";

    canonicalizerAndResolveTypes(s,
            Expectation()
            .declaration(Decl()
                         .Object("y", ObjectDeclarationCategory::Variable)
                         .ty_.Derived(TypeKind::Array)
                         .ty_.Basic(BasicTypeKind::Double)));
}

void TypeCanonicalizerAndResolverTester::case0006()
{
    auto s = R"(
typedef double * x ;
x y ;
)";

    canonicalizerAndResolveTypes(s,
            Expectation()
            .declaration(Decl()
                         .Object("y", ObjectDeclarationCategory::Variable)
                         .ty_.Derived(TypeKind::Pointer)
                         .ty_.Basic(BasicTypeKind::Double)));
}

void TypeCanonicalizerAndResolverTester::case0007()
{
    auto s = R"(
typedef double * * x ;
x y ;
)";

    canonicalizerAndResolveTypes(s,
            Expectation()
            .declaration(Decl()
                         .Object("y", ObjectDeclarationCategory::Variable)
                         .ty_.Derived(TypeKind::Pointer)
                         .ty_.Derived(TypeKind::Pointer)
                         .ty_.Basic(BasicTypeKind::Double)));
}

void TypeCanonicalizerAndResolverTester::case0008()
{
    auto s = R"(
typedef double x ;
typedef x y ;
y z ;
)";

    canonicalizerAndResolveTypes(s,
            Expectation()
            .declaration(Decl()
                         .Object("z", ObjectDeclarationCategory::Variable)
                         .ty_.Basic(BasicTypeKind::Double)));
}

void TypeCanonicalizerAndResolverTester::case0009()
{
    auto s = R"(
typedef double x ;
x y ( ) ;
)";

    canonicalizerAndResolveTypes(s,
            Expectation()
            .declaration(Decl()
                         .Function("y")
                         .ty_.Basic(BasicTypeKind::Double)
                         .ty_.Derived(TypeKind::Function)));
}

void TypeCanonicalizerAndResolverTester::case0010()
{
    auto s = R"(
typedef double x ;
x y ( int w ) ;
)";

    canonicalizerAndResolveTypes(s,
            Expectation()
            .declaration(Decl()
                         .Function("y")
                         .ty_.Basic(BasicTypeKind::Double)
                         .ty_.addParam().Basic(BasicTypeKind::Int_S)
                         .ty_.Derived(TypeKind::Function)));
}

void TypeCanonicalizerAndResolverTester::case0011()
{
    auto s = R"(
typedef double x ;
x y ( int w , double z ) ;
)";

    canonicalizerAndResolveTypes(s,
            Expectation()
            .declaration(Decl()
                         .Function("y")
                         .ty_.Basic(BasicTypeKind::Double)
                         .ty_.addParam().Basic(BasicTypeKind::Int_S)
                         .ty_.addParam().Basic(BasicTypeKind::Double)
                         .ty_.Derived(TypeKind::Function)));
}

void TypeCanonicalizerAndResolverTester::case0012()
{
    auto s = R"(
typedef double x ;
x y ( int w , x z ) ;
)";

    canonicalizerAndResolveTypes(s,
            Expectation()
            .declaration(Decl()
                         .Function("y")
                         .ty_.Basic(BasicTypeKind::Double)
                         .ty_.addParam().Basic(BasicTypeKind::Int_S)
                         .ty_.addParam().Basic(BasicTypeKind::Double)
                         .ty_.Derived(TypeKind::Function)));
}

void TypeCanonicalizerAndResolverTester::case0013()
{
    auto s = R"(
typedef double x ;
typedef int t ;
x y ( t w , double z ) ;
)";

    canonicalizerAndResolveTypes(s,
            Expectation()
            .declaration(Decl()
                         .Function("y")
                         .ty_.Basic(BasicTypeKind::Double)
                         .ty_.addParam().Basic(BasicTypeKind::Int_S)
                         .ty_.addParam().Basic(BasicTypeKind::Double)
                         .ty_.Derived(TypeKind::Function)));
}

void TypeCanonicalizerAndResolverTester::case0014()
{
    auto s = R"(
typedef double x ;
typedef int t ;
x y ( t w , x z ) ;
)";

    canonicalizerAndResolveTypes(s,
            Expectation()
            .declaration(Decl()
                         .Function("y")
                         .ty_.Basic(BasicTypeKind::Double)
                         .ty_.addParam().Basic(BasicTypeKind::Int_S)
                         .ty_.addParam().Basic(BasicTypeKind::Double)
                         .ty_.Derived(TypeKind::Function)));
}

void TypeCanonicalizerAndResolverTester::case0015()
{
    auto s = R"(
typedef double x ;
typedef int t ;
const x y ( t w , x z ) ;
)";

    canonicalizerAndResolveTypes(s,
            Expectation()
            .declaration(Decl()
                         .Function("y")
                         .ty_.Basic(BasicTypeKind::Double, CVR::Const)
                         .ty_.addParam().Basic(BasicTypeKind::Int_S)
                         .ty_.addParam().Basic(BasicTypeKind::Double)
                         .ty_.Derived(TypeKind::Function)));
}

void TypeCanonicalizerAndResolverTester::case0016()
{
    auto s = R"(
typedef double x ;
typedef int t ;
const x y ( t const w , x z ) ;
)";

    canonicalizerAndResolveTypes(s,
            Expectation()
            .declaration(Decl()
                         .Function("y")
                         .ty_.Basic(BasicTypeKind::Double, CVR::Const)
                         .ty_.addParam().Basic(BasicTypeKind::Int_S, CVR::Const)
                         .ty_.addParam().Basic(BasicTypeKind::Double)
                         .ty_.Derived(TypeKind::Function)));
}

void TypeCanonicalizerAndResolverTester::case0017()
{
    auto s = R"(
typedef double x ;
typedef int t ;
x y ( t * * w , x z ) ;
)";

    canonicalizerAndResolveTypes(s,
            Expectation()
            .declaration(Decl()
                         .Function("y")
                         .ty_.Basic(BasicTypeKind::Double)
                         .ty_.addParam().Basic(BasicTypeKind::Int_S)
                         .ty_.atParam().Derived(TypeKind::Pointer)
                         .ty_.atParam().Derived(TypeKind::Pointer)
                         .ty_.addParam().Basic(BasicTypeKind::Double)
                         .ty_.Derived(TypeKind::Function)));
}

void TypeCanonicalizerAndResolverTester::case0018()
{
    auto s = R"(
typedef double x ;
typedef int t ;
x y ( t * * w , x * * z ) ;
)";

    canonicalizerAndResolveTypes(s,
            Expectation()
            .declaration(Decl()
                         .Function("y")
                         .ty_.Basic(BasicTypeKind::Double)
                         .ty_.addParam().Basic(BasicTypeKind::Int_S)
                         .ty_.atParam().Derived(TypeKind::Pointer)
                         .ty_.atParam().Derived(TypeKind::Pointer)
                         .ty_.addParam().Basic(BasicTypeKind::Double)
                         .ty_.atParam().Derived(TypeKind::Pointer)
                         .ty_.atParam().Derived(TypeKind::Pointer)
                         .ty_.Derived(TypeKind::Function)));
}

void TypeCanonicalizerAndResolverTester::case0019()
{
    auto s = R"(
typedef double x ;
typedef int t ;
double y ( t w , x z ) ;
)";

    canonicalizerAndResolveTypes(s,
            Expectation()
            .declaration(Decl()
                         .Function("y")
                         .ty_.Basic(BasicTypeKind::Double)
                         .ty_.addParam().Basic(BasicTypeKind::Int_S)
                         .ty_.addParam().Basic(BasicTypeKind::Double)
                         .ty_.Derived(TypeKind::Function)));
}

void TypeCanonicalizerAndResolverTester::case0020()
{
    auto s = R"(
typedef double const x ;
x y ;
)";

    canonicalizerAndResolveTypes(s,
            Expectation()
            .declaration(Decl()
                         .Object("y", ObjectDeclarationCategory::Variable)
                         .ty_.Basic(BasicTypeKind::Double, CVR::Const)));
}

void TypeCanonicalizerAndResolverTester::case0021()
{
    auto s = R"(
typedef double * x ;
x * y ;
)";

    canonicalizerAndResolveTypes(s,
            Expectation()
            .declaration(Decl()
                         .Object("y", ObjectDeclarationCategory::Variable)
                         .ty_.Derived(TypeKind::Pointer)
                         .ty_.Derived(TypeKind::Pointer)
                         .ty_.Basic(BasicTypeKind::Double)));
}

void TypeCanonicalizerAndResolverTester::case0022()
{
    auto s = R"(
typedef double * x ;
typedef x y ;
y * z ;
)";

    canonicalizerAndResolveTypes(s,
            Expectation()
            .declaration(Decl()
                         .Object("z", ObjectDeclarationCategory::Variable)
                         .ty_.Derived(TypeKind::Pointer)
                         .ty_.Derived(TypeKind::Pointer)
                         .ty_.Basic(BasicTypeKind::Double)));
}

void TypeCanonicalizerAndResolverTester::case0023()
{
    auto s = R"(
typedef double * x ;
typedef x * y ;
y * z ;
)";

    canonicalizerAndResolveTypes(s,
            Expectation()
            .declaration(Decl()
                         .Object("z", ObjectDeclarationCategory::Variable)
                         .ty_.Derived(TypeKind::Pointer)
                         .ty_.Derived(TypeKind::Pointer)
                         .ty_.Derived(TypeKind::Pointer)
                         .ty_.Basic(BasicTypeKind::Double)));
}

void TypeCanonicalizerAndResolverTester::case0024()
{
    auto s = R"(
typedef double x ;
typedef x y ;
y z ;
)";

    canonicalizerAndResolveTypes(s,
            Expectation()
            .declaration(Decl()
                         .Object("z", ObjectDeclarationCategory::Variable)
                         .ty_.Basic(BasicTypeKind::Double)));
}

void TypeCanonicalizerAndResolverTester::case0025()
{
    auto s = R"(
void f ( ) { int x ; x y ; }
)";

    canonicalizerAndResolveTypes(s,
            Expectation()
            .diagnostic(Expectation::ErrorOrWarn::Error,
                        TypeCanonicalizer::DiagnosticsReporter::ID_of_ExpectedTypedefDeclaration)
            .declaration(
                Decl().Object("y", ObjectDeclarationCategory::Variable, ScopeKind::Block)
                      .ty_.Unknown()));
}

void TypeCanonicalizerAndResolverTester::case0026()
{
    auto s = R"(
void f ( ) { x y ; }
)";

    canonicalizerAndResolveTypes(s,
            Expectation()
            .diagnostic(Expectation::ErrorOrWarn::Error,
                        TypeCanonicalizer::DiagnosticsReporter::ID_of_TypeDeclarationNotFound)
            .declaration(
                Decl().Object("y", ObjectDeclarationCategory::Variable, ScopeKind::Block)
                      .ty_.Unknown()));
}

void TypeCanonicalizerAndResolverTester::case0027()
{
    auto s = R"(
void _ ( )
{
    struct s x ;
    x . m ;
}
)";

    canonicalizerAndResolveTypes(s,
            Expectation()
            .diagnostic(Expectation::ErrorOrWarn::Error,
                        TypeCanonicalizer::DiagnosticsReporter::ID_of_TypeDeclarationNotFound));
}

void TypeCanonicalizerAndResolverTester::case0028()
{
    auto s = R"(
struct x { int y ; } ; void f ( ) { struct x z ; }
)";

    canonicalizerAndResolveTypes(s,
            Expectation()
            .declaration(
                Decl().Object("z", ObjectDeclarationCategory::Variable, ScopeKind::Block)
                      .ty_.Tag("x", TagTypeKind::Struct)));
}

void TypeCanonicalizerAndResolverTester::case0029()
{
    auto s = R"(
struct x { int y ; } ; void f ( ) { union x z ; }
)";

    canonicalizerAndResolveTypes(s,
            Expectation()
            .diagnostic(Expectation::ErrorOrWarn::Error,
                        TypeCanonicalizer::DiagnosticsReporter::ID_of_TagTypeDoesNotMatchTagDeclaration)
            .declaration(
                Decl().Object("z", ObjectDeclarationCategory::Variable, ScopeKind::Block)
                      .ty_.Unknown()));
}

void TypeCanonicalizerAndResolverTester::case0030()
{
    auto s = R"(
typedef double x ;
typedef x y ;
typedef y z ;
z w ;
)";

    canonicalizerAndResolveTypes(s,
            Expectation()
            .declaration(Decl()
                         .Object("w", ObjectDeclarationCategory::Variable)
                         .ty_.Basic(BasicTypeKind::Double)));
}

void TypeCanonicalizerAndResolverTester::case0031()
{
    auto s = R"(
typedef double x ;
typedef x y ;
typedef y z ;
typedef z w ;
w p ;
)";

    canonicalizerAndResolveTypes(s,
            Expectation()
            .declaration(Decl()
                         .Object("p", ObjectDeclarationCategory::Variable)
                         .ty_.Basic(BasicTypeKind::Double)));
}

void TypeCanonicalizerAndResolverTester::case0032()
{
    auto s = R"(
typedef double x ;
typedef x y ;
typedef y z ;
typedef z w ;
const w p ;
)";

    canonicalizerAndResolveTypes(s,
            Expectation()
            .declaration(Decl()
                         .Object("p", ObjectDeclarationCategory::Variable)
                         .ty_.Basic(BasicTypeKind::Double, CVR::Const)));
}

void TypeCanonicalizerAndResolverTester::case0033()
{
    auto s = R"(
typedef const double x ;
typedef x y ;
typedef y z ;
typedef z w ;
w p ;
)";

    canonicalizerAndResolveTypes(s,
            Expectation()
            .declaration(Decl()
                         .Object("p", ObjectDeclarationCategory::Variable)
                         .ty_.Basic(BasicTypeKind::Double, CVR::Const)));
}

void TypeCanonicalizerAndResolverTester::case0034()
{
    auto s = R"(
typedef double x ;
typedef x y ;
typedef const y z ;
typedef z w ;
w p ;
)";

    canonicalizerAndResolveTypes(s,
            Expectation()
            .declaration(Decl()
                         .Object("p", ObjectDeclarationCategory::Variable)
                         .ty_.Basic(BasicTypeKind::Double, CVR::Const)));
}

void TypeCanonicalizerAndResolverTester::case0035()
{
    auto s = R"(
typedef const double x ;
typedef x y ;
typedef y z ;
typedef z w ;
const w p ;
)";

    canonicalizerAndResolveTypes(s,
            Expectation()
            .declaration(Decl()
                         .Object("p", ObjectDeclarationCategory::Variable)
                         .ty_.Basic(BasicTypeKind::Double, CVR::Const)));
}

void TypeCanonicalizerAndResolverTester::case0036()
{
    auto s = R"(
struct t { int m ; } ;
typedef struct t x ;
typedef x y ;
typedef y z ;
typedef z w ;
w p ;
)";

    canonicalizerAndResolveTypes(s,
            Expectation()
            .declaration(Decl()
                         .Object("p", ObjectDeclarationCategory::Variable)
                         .ty_.Tag("t", TagTypeKind::Struct)));
}

void TypeCanonicalizerAndResolverTester::case0037()
{
    auto s = R"(
typedef struct t { int m ; } x ;
typedef x y ;
typedef y z ;
typedef z w ;
w p ;
)";

    canonicalizerAndResolveTypes(s,
            Expectation()
            .declaration(Decl()
                         .Object("p", ObjectDeclarationCategory::Variable)
                         .ty_.Tag("t", TagTypeKind::Struct)));
}

void TypeCanonicalizerAndResolverTester::case0038()
{
    auto s = R"(
typedef struct { int m ; } x ;
typedef x y ;
typedef y z ;
typedef z w ;
w p ;
)";

    canonicalizerAndResolveTypes(s,
            Expectation()
            .declaration(Decl()
                         .Object("p", ObjectDeclarationCategory::Variable)
                         .ty_.Tag("", TagTypeKind::Struct)));
}

void TypeCanonicalizerAndResolverTester::case0039(){}
