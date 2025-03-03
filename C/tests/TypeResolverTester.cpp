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

#include "TypeResolverTester.h"

#include "sema/TypeResolver.h"

using namespace psy;
using namespace C;

const std::string TypeResolverTester::Name = "TYPE-RESOLVER";

void TypeResolverTester::testTypeResolver()
{
    return run<TypeResolverTester>(tests_);
}

void TypeResolverTester::resolve(std::string text, Expectation X)
{
    (static_cast<InternalsTestSuite*>(suite_)->resolveTypes(text, X));
}

void TypeResolverTester::case0000()
{
    auto s = R"(
typedef double x ;
x y ;
)";

    resolve(s,
            Expectation()
            .declaration(Decl()
                         .Object("y", ObjectDeclarationCategory::Variable)
                         .ty_.Basic(BasicTypeKind::Double)));
}

void TypeResolverTester::case0001()
{
    auto s = R"(
typedef double x ;
x * y ;
)";

    resolve(s,
            Expectation()
            .declaration(Decl()
                         .Object("y", ObjectDeclarationCategory::Variable)
                         .ty_.Derived(TypeKind::Pointer)
                         .ty_.Basic(BasicTypeKind::Double)));
}

void TypeResolverTester::case0002()
{
    auto s = R"(
typedef double x ;
x const y ;
)";

    resolve(s,
            Expectation()
            .declaration(Decl()
                         .Object("y", ObjectDeclarationCategory::Variable)
                         .ty_.Basic(BasicTypeKind::Double, CVR::Const)));
}

void TypeResolverTester::case0003()
{
    auto s = R"(
typedef double const x ;
x const y ;
)";

    resolve(s,
            Expectation()
            .declaration(Decl()
                         .Object("y", ObjectDeclarationCategory::Variable)
                         .ty_.Basic(BasicTypeKind::Double, CVR::Const)));
}

void TypeResolverTester::case0004()
{
    auto s = R"(
typedef double x ;
x * * y ;
)";

    resolve(s,
            Expectation()
            .declaration(Decl()
                         .Object("y", ObjectDeclarationCategory::Variable)
                         .ty_.Derived(TypeKind::Pointer)
                         .ty_.Derived(TypeKind::Pointer)
                         .ty_.Basic(BasicTypeKind::Double)));
}

void TypeResolverTester::case0005()
{
    auto s = R"(
typedef double x ;
x y [ 1 ] ;
)";

    resolve(s,
            Expectation()
            .declaration(Decl()
                         .Object("y", ObjectDeclarationCategory::Variable)
                         .ty_.Derived(TypeKind::Array)
                         .ty_.Basic(BasicTypeKind::Double)));
}

void TypeResolverTester::case0006()
{
    auto s = R"(
typedef double * x ;
x y ;
)";

    resolve(s,
            Expectation()
            .declaration(Decl()
                         .Object("y", ObjectDeclarationCategory::Variable)
                         .ty_.Derived(TypeKind::Pointer)
                         .ty_.Basic(BasicTypeKind::Double)));
}

void TypeResolverTester::case0007()
{
    auto s = R"(
typedef double * * x ;
x y ;
)";

    resolve(s,
            Expectation()
            .declaration(Decl()
                         .Object("y", ObjectDeclarationCategory::Variable)
                         .ty_.Derived(TypeKind::Pointer)
                         .ty_.Derived(TypeKind::Pointer)
                         .ty_.Basic(BasicTypeKind::Double)));
}

void TypeResolverTester::case0008()
{
    auto s = R"(
typedef double x ;
typedef x y ;
y z ;
)";

    resolve(s,
            Expectation()
            .declaration(Decl()
                         .Object("z", ObjectDeclarationCategory::Variable)
                         .ty_.Basic(BasicTypeKind::Double)));
}

void TypeResolverTester::case0009()
{
    auto s = R"(
typedef double x ;
x y ( ) ;
)";

    resolve(s,
            Expectation()
            .declaration(Decl()
                         .Function("y")
                         .ty_.Basic(BasicTypeKind::Double)
                         .ty_.Derived(TypeKind::Function)));
}

void TypeResolverTester::case0010()
{
    auto s = R"(
typedef double x ;
x y ( int w ) ;
)";

    resolve(s,
            Expectation()
            .declaration(Decl()
                         .Function("y")
                         .ty_.Basic(BasicTypeKind::Double)
                         .ty_.addParam().Basic(BasicTypeKind::Int_S)
                         .ty_.Derived(TypeKind::Function)));
}

void TypeResolverTester::case0011()
{
    auto s = R"(
typedef double x ;
x y ( int w , double z ) ;
)";

    resolve(s,
            Expectation()
            .declaration(Decl()
                         .Function("y")
                         .ty_.Basic(BasicTypeKind::Double)
                         .ty_.addParam().Basic(BasicTypeKind::Int_S)
                         .ty_.addParam().Basic(BasicTypeKind::Double)
                         .ty_.Derived(TypeKind::Function)));
}

void TypeResolverTester::case0012()
{
    auto s = R"(
typedef double x ;
x y ( int w , x z ) ;
)";

    resolve(s,
            Expectation()
            .declaration(Decl()
                         .Function("y")
                         .ty_.Basic(BasicTypeKind::Double)
                         .ty_.addParam().Basic(BasicTypeKind::Int_S)
                         .ty_.addParam().Basic(BasicTypeKind::Double)
                         .ty_.Derived(TypeKind::Function)));
}

void TypeResolverTester::case0013()
{
    auto s = R"(
typedef double x ;
typedef int t ;
x y ( t w , double z ) ;
)";

    resolve(s,
            Expectation()
            .declaration(Decl()
                         .Function("y")
                         .ty_.Basic(BasicTypeKind::Double)
                         .ty_.addParam().Basic(BasicTypeKind::Int_S)
                         .ty_.addParam().Basic(BasicTypeKind::Double)
                         .ty_.Derived(TypeKind::Function)));
}

void TypeResolverTester::case0014()
{
    auto s = R"(
typedef double x ;
typedef int t ;
x y ( t w , x z ) ;
)";

    resolve(s,
            Expectation()
            .declaration(Decl()
                         .Function("y")
                         .ty_.Basic(BasicTypeKind::Double)
                         .ty_.addParam().Basic(BasicTypeKind::Int_S)
                         .ty_.addParam().Basic(BasicTypeKind::Double)
                         .ty_.Derived(TypeKind::Function)));
}

void TypeResolverTester::case0015()
{
    auto s = R"(
typedef double x ;
typedef int t ;
const x y ( t w , x z ) ;
)";

    resolve(s,
            Expectation()
            .declaration(Decl()
                         .Function("y")
                         .ty_.Basic(BasicTypeKind::Double, CVR::Const)
                         .ty_.addParam().Basic(BasicTypeKind::Int_S)
                         .ty_.addParam().Basic(BasicTypeKind::Double)
                         .ty_.Derived(TypeKind::Function)));
}

void TypeResolverTester::case0016()
{
    auto s = R"(
typedef double x ;
typedef int t ;
const x y ( t const w , x z ) ;
)";

    resolve(s,
            Expectation()
            .declaration(Decl()
                         .Function("y")
                         .ty_.Basic(BasicTypeKind::Double, CVR::Const)
                         .ty_.addParam().Basic(BasicTypeKind::Int_S, CVR::Const)
                         .ty_.addParam().Basic(BasicTypeKind::Double)
                         .ty_.Derived(TypeKind::Function)));
}

void TypeResolverTester::case0017()
{
    auto s = R"(
typedef double x ;
typedef int t ;
x y ( t * * w , x z ) ;
)";

    resolve(s,
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

void TypeResolverTester::case0018()
{
    auto s = R"(
typedef double x ;
typedef int t ;
x y ( t * * w , x * * z ) ;
)";

    resolve(s,
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

void TypeResolverTester::case0019()
{
    auto s = R"(
typedef double x ;
typedef int t ;
double y ( t w , x z ) ;
)";

    resolve(s,
            Expectation()
            .declaration(Decl()
                         .Function("y")
                         .ty_.Basic(BasicTypeKind::Double)
                         .ty_.addParam().Basic(BasicTypeKind::Int_S)
                         .ty_.addParam().Basic(BasicTypeKind::Double)
                         .ty_.Derived(TypeKind::Function)));
}

void TypeResolverTester::case0020()
{
    auto s = R"(
typedef double const x ;
x y ;
)";

    resolve(s,
            Expectation()
            .declaration(Decl()
                         .Object("y", ObjectDeclarationCategory::Variable)
                         .ty_.Basic(BasicTypeKind::Double, CVR::Const)));
}

void TypeResolverTester::case0021()
{
    auto s = R"(
typedef double * x ;
x * y ;
)";

    resolve(s,
            Expectation()
            .declaration(Decl()
                         .Object("y", ObjectDeclarationCategory::Variable)
                         .ty_.Derived(TypeKind::Pointer)
                         .ty_.Derived(TypeKind::Pointer)
                         .ty_.Basic(BasicTypeKind::Double)));
}

void TypeResolverTester::case0022()
{
    auto s = R"(
typedef double * x ;
typedef x y ;
y * z ;
)";

    resolve(s,
            Expectation()
            .declaration(Decl()
                         .Object("z", ObjectDeclarationCategory::Variable)
                         .ty_.Derived(TypeKind::Pointer)
                         .ty_.Derived(TypeKind::Pointer)
                         .ty_.Basic(BasicTypeKind::Double)));
}

void TypeResolverTester::case0023()
{
    auto s = R"(
typedef double * x ;
typedef x * y ;
y * z ;
)";

    resolve(s,
            Expectation()
            .declaration(Decl()
                         .Object("z", ObjectDeclarationCategory::Variable)
                         .ty_.Derived(TypeKind::Pointer)
                         .ty_.Derived(TypeKind::Pointer)
                         .ty_.Derived(TypeKind::Pointer)
                         .ty_.Basic(BasicTypeKind::Double)));
}

void TypeResolverTester::case0024()
{
    auto s = R"(
typedef double x ;
typedef x y ;
y z ;
)";

    resolve(s,
            Expectation()
            .declaration(Decl()
                         .Object("z", ObjectDeclarationCategory::Variable)
                         .ty_.Basic(BasicTypeKind::Double)));
}

void TypeResolverTester::case0025()
{
    auto s = R"(
void f ( ) { int x ; x y ; }
)";

    resolve(s,
            Expectation()
            .diagnostic(Expectation::ErrorOrWarn::Error,
                        TypeResolver::DiagnosticsReporter::ID_of_ExpectedTypedefDeclaration)
            .declaration(
                Decl().Object("y", ObjectDeclarationCategory::Variable, ScopeKind::Block)
                      .ty_.Unknown()));
}

void TypeResolverTester::case0026()
{
    auto s = R"(
void f ( ) { x y ; }
)";

    resolve(s,
            Expectation()
            .diagnostic(Expectation::ErrorOrWarn::Error,
                        TypeResolver::DiagnosticsReporter::ID_of_TypeDeclarationNotFound)
            .declaration(
                Decl().Object("y", ObjectDeclarationCategory::Variable, ScopeKind::Block)
                      .ty_.Unknown()));
}

void TypeResolverTester::case0027()
{
}

void TypeResolverTester::case0028()
{
    auto s = R"(
struct x { int y ; } ; void f ( ) { struct x z ; }
)";

    resolve(s,
            Expectation()
            .declaration(
                Decl().Object("z", ObjectDeclarationCategory::Variable, ScopeKind::Block)
                      .ty_.Tag("x", TagTypeKind::Struct)));
}

void TypeResolverTester::case0029()
{
    auto s = R"(
struct x { int y ; } ; void f ( ) { union x z ; }
)";

    resolve(s,
            Expectation()
            .diagnostic(Expectation::ErrorOrWarn::Error,
                        TypeResolver::DiagnosticsReporter::ID_of_TagTypeDoesNotMatchTagDeclaration)
            .declaration(
                Decl().Object("z", ObjectDeclarationCategory::Variable, ScopeKind::Block)
                      .ty_.Unknown()));
}
