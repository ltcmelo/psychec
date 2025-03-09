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

#include "TypeCheckerTester.h"

#include "sema/TypeChecker.h"

using namespace psy;
using namespace C;

const std::string TypeCheckerTester::Name = "TYPE-CHECKER";

void TypeCheckerTester::testTypeChecker()
{
    return run<TypeCheckerTester>(tests_);
}

void TypeCheckerTester::checkTypes(std::string text, Expectation X)
{
    (static_cast<InternalsTestSuite*>(suite_)->checkTypes(text, X));
}

void TypeCheckerTester::case0000()
{
    auto s = R"(
struct s { double m ; } ;
void _ ( )
{
    struct s x ;
    x . n ;
}
)";

    checkTypes(s,
               Expectation()
                   .diagnostic(Expectation::ErrorOrWarn::Error,
                               TypeChecker::DiagnosticsReporter::ID_of_UnknownMemberOfTag));
}

void TypeCheckerTester::case0001()
{
    auto s = R"(
struct s { double m ; } ;
void _ ( )
{
    struct s * x ;
    x -> n ;
}
)";

    checkTypes(s,
               Expectation()
                   .diagnostic(Expectation::ErrorOrWarn::Error,
                               TypeChecker::DiagnosticsReporter::ID_of_UnknownMemberOfTag));
}

void TypeCheckerTester::case0002()
{
    auto s = R"(
struct s { double m ; } ;
void _ ( )
{
    struct s x ;
    x -> m ;
}
)";

    checkTypes(s,
               Expectation()
                   .diagnostic(Expectation::ErrorOrWarn::Error,
                               TypeChecker::DiagnosticsReporter::ID_of_InvalidOperator));
}

void TypeCheckerTester::case0003()
{
    auto s = R"(
struct s { double m ; } ;
void _ ( )
{
    struct s * x ;
    x . m ;
}
)";

    checkTypes(s,
               Expectation()
                   .diagnostic(Expectation::ErrorOrWarn::Error,
                               TypeChecker::DiagnosticsReporter::ID_of_InvalidOperator));
}

void TypeCheckerTester::case0004()
{
    auto s = R"(
struct s { double m ; } ;
void _ ( )
{
    struct s x ;
    x . m ;
}
)";

    checkTypes(s, Expectation());
}

void TypeCheckerTester::case0005()
{
    auto s = R"(
struct s { double m ; } ;
void _ ( )
{
    struct s * x ;
    x -> m ;
}
)";

    checkTypes(s, Expectation());
}

void TypeCheckerTester::case0006()
{
    auto s = R"(
typedef struct s { double m ; } t ;
void _ ( )
{
    t x ;
    x . m ;
}
)";

    checkTypes(s, Expectation());
}

void TypeCheckerTester::case0007()
{
    auto s = R"(
typedef struct s { double m ; } t ;
void _ ( )
{
    t x ;
    x -> m ;
}
)";

    checkTypes(s,
               Expectation()
                   .diagnostic(Expectation::ErrorOrWarn::Error,
                               TypeChecker::DiagnosticsReporter::ID_of_InvalidOperator));
}

void TypeCheckerTester::case0008()
{
    auto s = R"(
typedef struct s { double m ; } t ;
void _ ( )
{
    t x ;
    x . n ;
}
)";

    checkTypes(s,
               Expectation()
                   .diagnostic(Expectation::ErrorOrWarn::Error,
                               TypeChecker::DiagnosticsReporter::ID_of_UnknownMemberOfTag));
}

void TypeCheckerTester::case0009()
{
    auto s = R"(
typedef struct s { double m ; } * t ;
void _ ( )
{
    t x ;
    x -> m ;
}
)";

    checkTypes(s, Expectation());
}

void TypeCheckerTester::case0010()
{
    auto s = R"(
typedef struct s { double m ; } * t ;
void _ ( )
{
    t x ;
    x . m ;
}
)";

    checkTypes(s,
               Expectation()
                   .diagnostic(Expectation::ErrorOrWarn::Error,
                               TypeChecker::DiagnosticsReporter::ID_of_InvalidOperator));
}

void TypeCheckerTester::case0011()
{
    auto s = R"(
typedef struct s { double m ; } * t ;
void _ ( )
{
    t x ;
    x -> n ;
}
)";

    checkTypes(s,
               Expectation()
                   .diagnostic(Expectation::ErrorOrWarn::Error,
                               TypeChecker::DiagnosticsReporter::ID_of_UnknownMemberOfTag));
}

void TypeCheckerTester::case0012()
{
    auto s = R"(
typedef struct s { double m ; } t ;
typedef t * tp ;
void _ ( )
{
    tp x ;
    x -> m ;
}
)";

    checkTypes(s, Expectation());
}

void TypeCheckerTester::case0013()
{
    auto s = R"(
typedef struct s { double m ; } t ;
typedef t * tp ;
void _ ( )
{
    tp x ;
    x . m ;
}
)";

    checkTypes(s,
               Expectation()
                   .diagnostic(Expectation::ErrorOrWarn::Error,
                               TypeChecker::DiagnosticsReporter::ID_of_InvalidOperator));
}

void TypeCheckerTester::case0014()
{
    auto s = R"(
typedef struct s { double m ; } t ;
typedef t * tp ;
void _ ( )
{
    tp x ;
    x -> n ;
}
)";

    checkTypes(s,
               Expectation()
                   .diagnostic(Expectation::ErrorOrWarn::Error,
                               TypeChecker::DiagnosticsReporter::ID_of_UnknownMemberOfTag));
}

void TypeCheckerTester::case0015()
{
    auto s = R"(
typedef struct s { double m ; } t ;
void _ ( )
{
    t * x ;
    x -> m ;
}
)";

    checkTypes(s, Expectation());
}

void TypeCheckerTester::case0016()
{
    auto s = R"(
typedef struct s { double m ; } t ;
void _ ( )
{
    t * x ;
    x . m ;
}
)";

    checkTypes(s,
               Expectation()
                   .diagnostic(Expectation::ErrorOrWarn::Error,
                               TypeChecker::DiagnosticsReporter::ID_of_InvalidOperator));
}

void TypeCheckerTester::case0017()
{
    auto s = R"(
typedef struct s { double m ; } t ;
void _ ( )
{
    t * x ;
    x -> n ;
}
)";

    checkTypes(s,
               Expectation()
                   .diagnostic(Expectation::ErrorOrWarn::Error,
                               TypeChecker::DiagnosticsReporter::ID_of_UnknownMemberOfTag));
}

void TypeCheckerTester::case0018()
{
    auto s = R"(
typedef struct s { double m ; } t ;
void _ ( )
{
    t const x ;
    x . m ;
}
)";

    checkTypes(s, Expectation());
}

void TypeCheckerTester::case0019()
{
    auto s = R"(
typedef struct s { double m ; } const t ;
void _ ( )
{
    t const x ;
    x . m ;
}
)";

    checkTypes(s, Expectation());
}

void TypeCheckerTester::case0020()
{
    auto s = R"(
typedef struct s { double m ; } const t ;
void _ ( )
{
    t x ;
    x . m ;
}
)";

    checkTypes(s, Expectation());
}

void TypeCheckerTester::case0021()
{
    auto s = R"(
typedef struct s { double m ; } t ;
typedef const t tc ;
void _ ( )
{
    tc x ;
    x . m ;
}
)";

    checkTypes(s, Expectation());
}

void TypeCheckerTester::case0022()
{
    auto s = R"(
typedef struct s { double m ; } * const t ;
void _ ( )
{
    t x ;
    x -> m ;
}
)";

    checkTypes(s, Expectation());
}

void TypeCheckerTester::case0023()
{
    auto s = R"(
typedef struct s { double m ; } const * const t ;
void _ ( )
{
    t x ;
    x -> m ;
}
)";

    checkTypes(s, Expectation());
}

void TypeCheckerTester::case0024()
{
    auto s = R"(
typedef struct s { double m ; } const * t ;
void _ ( )
{
    t x ;
    x -> m ;
}
)";

    checkTypes(s, Expectation());
}

void TypeCheckerTester::case0025()
{
    auto s = R"(
typedef struct s { double m ; } const * t ;
void _ ( )
{
    t const x ;
    x -> m ;
}
)";

    checkTypes(s, Expectation());
}

void TypeCheckerTester::case0026()
{
    auto s = R"(
typedef struct s { double m ; } t ;
typedef t const * const ctpc ;
void _ ( )
{
    ctpc const x ;
    x -> m ;
}
)";

    checkTypes(s, Expectation());
}

void TypeCheckerTester::case0027(){}
void TypeCheckerTester::case0028(){}
void TypeCheckerTester::case0029(){}
void TypeCheckerTester::case0030(){}
void TypeCheckerTester::case0031(){}
void TypeCheckerTester::case0032(){}
void TypeCheckerTester::case0033(){}
void TypeCheckerTester::case0034(){}
void TypeCheckerTester::case0035(){}
void TypeCheckerTester::case0036(){}
void TypeCheckerTester::case0037(){}
void TypeCheckerTester::case0038(){}
void TypeCheckerTester::case0039(){}
void TypeCheckerTester::case0040(){}
void TypeCheckerTester::case0041(){}
void TypeCheckerTester::case0042(){}
void TypeCheckerTester::case0043(){}
void TypeCheckerTester::case0044(){}
void TypeCheckerTester::case0045(){}
void TypeCheckerTester::case0046(){}
void TypeCheckerTester::case0047(){}
void TypeCheckerTester::case0048(){}
void TypeCheckerTester::case0049(){}
void TypeCheckerTester::case0050(){}
void TypeCheckerTester::case0051(){}
void TypeCheckerTester::case0052(){}
void TypeCheckerTester::case0053(){}
void TypeCheckerTester::case0054(){}
void TypeCheckerTester::case0055(){}
void TypeCheckerTester::case0056(){}
void TypeCheckerTester::case0057(){}
void TypeCheckerTester::case0058(){}
void TypeCheckerTester::case0059(){}

void TypeCheckerTester::case0500(){}
void TypeCheckerTester::case0501(){}
void TypeCheckerTester::case0502(){}
void TypeCheckerTester::case0503(){}
void TypeCheckerTester::case0504(){}
void TypeCheckerTester::case0505(){}
void TypeCheckerTester::case0506(){}
void TypeCheckerTester::case0507(){}
void TypeCheckerTester::case0508(){}
void TypeCheckerTester::case0509(){}
void TypeCheckerTester::case0510(){}
void TypeCheckerTester::case0511(){}
void TypeCheckerTester::case0512(){}
void TypeCheckerTester::case0513(){}
void TypeCheckerTester::case0514(){}
void TypeCheckerTester::case0515(){}
void TypeCheckerTester::case0516(){}
void TypeCheckerTester::case0517(){}
void TypeCheckerTester::case0518(){}
void TypeCheckerTester::case0519(){}
void TypeCheckerTester::case0520(){}
void TypeCheckerTester::case0521(){}
void TypeCheckerTester::case0522(){}
void TypeCheckerTester::case0523(){}
void TypeCheckerTester::case0524(){}
void TypeCheckerTester::case0525(){}
void TypeCheckerTester::case0526(){}
void TypeCheckerTester::case0527(){}
void TypeCheckerTester::case0528(){}
void TypeCheckerTester::case0529(){}
void TypeCheckerTester::case0530(){}
void TypeCheckerTester::case0531(){}
void TypeCheckerTester::case0532(){}
void TypeCheckerTester::case0533(){}
void TypeCheckerTester::case0534(){}
void TypeCheckerTester::case0535(){}
void TypeCheckerTester::case0536(){}
void TypeCheckerTester::case0537(){}
void TypeCheckerTester::case0538(){}
void TypeCheckerTester::case0539(){}
void TypeCheckerTester::case0540(){}
void TypeCheckerTester::case0541(){}
void TypeCheckerTester::case0542(){}
void TypeCheckerTester::case0543(){}
void TypeCheckerTester::case0544(){}
void TypeCheckerTester::case0545(){}
void TypeCheckerTester::case0546(){}
void TypeCheckerTester::case0547(){}
void TypeCheckerTester::case0548(){}
void TypeCheckerTester::case0549(){}
void TypeCheckerTester::case0550(){}
void TypeCheckerTester::case0551(){}
void TypeCheckerTester::case0552(){}
void TypeCheckerTester::case0553(){}
void TypeCheckerTester::case0554(){}
void TypeCheckerTester::case0555(){}
void TypeCheckerTester::case0556(){}
void TypeCheckerTester::case0557(){}
void TypeCheckerTester::case0558(){}
void TypeCheckerTester::case0559(){}

void TypeCheckerTester::case0700(){}
void TypeCheckerTester::case0701(){}
void TypeCheckerTester::case0702(){}
void TypeCheckerTester::case0703(){}
void TypeCheckerTester::case0704(){}
void TypeCheckerTester::case0705(){}
void TypeCheckerTester::case0706(){}
void TypeCheckerTester::case0707(){}
void TypeCheckerTester::case0708(){}
void TypeCheckerTester::case0709(){}
void TypeCheckerTester::case0710(){}
void TypeCheckerTester::case0711(){}
void TypeCheckerTester::case0712(){}
void TypeCheckerTester::case0713(){}
void TypeCheckerTester::case0714(){}
void TypeCheckerTester::case0715(){}
void TypeCheckerTester::case0716(){}
void TypeCheckerTester::case0717(){}
void TypeCheckerTester::case0718(){}
void TypeCheckerTester::case0719(){}
void TypeCheckerTester::case0720(){}
void TypeCheckerTester::case0721(){}
void TypeCheckerTester::case0722(){}
void TypeCheckerTester::case0723(){}
void TypeCheckerTester::case0724(){}
void TypeCheckerTester::case0725(){}
void TypeCheckerTester::case0726(){}
void TypeCheckerTester::case0727(){}
void TypeCheckerTester::case0728(){}
void TypeCheckerTester::case0729(){}
void TypeCheckerTester::case0730(){}
void TypeCheckerTester::case0731(){}
void TypeCheckerTester::case0732(){}
void TypeCheckerTester::case0733(){}
void TypeCheckerTester::case0734(){}
void TypeCheckerTester::case0735(){}
void TypeCheckerTester::case0736(){}
void TypeCheckerTester::case0737(){}
void TypeCheckerTester::case0738(){}
void TypeCheckerTester::case0739(){}
void TypeCheckerTester::case0740(){}
void TypeCheckerTester::case0741(){}
void TypeCheckerTester::case0742(){}
void TypeCheckerTester::case0743(){}
void TypeCheckerTester::case0744(){}
void TypeCheckerTester::case0745(){}
void TypeCheckerTester::case0746(){}
void TypeCheckerTester::case0747(){}
void TypeCheckerTester::case0748(){}
void TypeCheckerTester::case0749(){}
void TypeCheckerTester::case0750(){}
void TypeCheckerTester::case0751(){}
void TypeCheckerTester::case0752(){}
void TypeCheckerTester::case0753(){}
void TypeCheckerTester::case0754(){}
void TypeCheckerTester::case0755(){}
void TypeCheckerTester::case0756(){}
void TypeCheckerTester::case0757(){}
void TypeCheckerTester::case0758(){}
void TypeCheckerTester::case0759(){}
