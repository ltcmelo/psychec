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

void TypeCheckerTester::case0027()
{
    auto s = R"(
struct s {
    struct {
        struct {
            const int m ;
        } x ;
    } y ;
} z ;

void _ ( )
{
    z . y ;
}
)";

    checkTypes(s, Expectation());
}

void TypeCheckerTester::case0028()
{
    auto s = R"(
struct s {
    struct {
        struct {
            const int m ;
        } x ;
    } y ;
} z ;

void _ ( )
{
    z . e ;
}
)";

    checkTypes(s,
               Expectation()
                   .diagnostic(Expectation::ErrorOrWarn::Error,
                               TypeChecker::DiagnosticsReporter::ID_of_UnknownMemberOfTag));
}

void TypeCheckerTester::case0029()
{
    auto s = R"(
struct s {
    struct {
        struct {
            const int m ;
        } x ;
    } y ;
} z ;

void _ ( )
{
    z . y . x ;
}
)";

    checkTypes(s, Expectation());
}

void TypeCheckerTester::case0030()
{
    auto s = R"(
struct s {
    struct {
        struct {
            const int m ;
        } x ;
    } y ;
} z ;

void _ ( )
{
    z . y . e ;
}
)";

    checkTypes(s,
               Expectation()
                   .diagnostic(Expectation::ErrorOrWarn::Error,
                               TypeChecker::DiagnosticsReporter::ID_of_UnknownMemberOfTag));
}

void TypeCheckerTester::case0031()
{
    auto s = R"(
struct s {
    struct {
        struct {
            const int m ;
        } x ;
    } y ;
} z ;

void _ ( )
{
    z . y . x. m ;
}
)";

    checkTypes(s, Expectation());
}

void TypeCheckerTester::case0032()
{
    auto s = R"(
struct s {
    struct {
        struct {
            const int m ;
        } x ;
    } y ;
} z ;

void _ ( )
{
    z . y . z . e ;
}
)";

    checkTypes(s,
               Expectation()
                   .diagnostic(Expectation::ErrorOrWarn::Error,
                               TypeChecker::DiagnosticsReporter::ID_of_UnknownMemberOfTag));
}

void TypeCheckerTester::case0033()
{
    auto s = R"(
struct {
    struct {
        struct {
            const int m ;
        } x ;
    } y ;
} z ;

void _ ( )
{
    z . y . x ;
}
)";

    checkTypes(s, Expectation());
}

void TypeCheckerTester::case0034()
{
    auto s = R"(
struct s {
    struct {
        struct {
            const int m ;
        } ;
    } ;
} ;
void _ ()
{
    struct s x ;
    x . m ;
}
)";

    checkTypes(s, Expectation());
}


void TypeCheckerTester::case0035()
{
    auto s = R"(
struct s {
    struct {
        struct {
            const int m ;
        } ;
    } ;
} ;
void _ ()
{
    struct s * x ;
    x -> m ;
}
)";

    checkTypes(s, Expectation());
}

void TypeCheckerTester::case0036()
{
    auto s = R"(
struct s {
    struct {
        struct {
            const int m ;
        } ;
    } ;
} ;
void _ ()
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

void TypeCheckerTester::case0037()
{
    auto s = R"(
struct s {
    struct {
        struct {
            const int m ;
            const double n ;
        } ;
    } ;
} ;
void _ ()
{
    struct s x ;
    x . n ;
}
)";

    checkTypes(s, Expectation());
}

void TypeCheckerTester::case0038()
{
    auto s = R"(
struct s {
    struct {
        struct {
            const int m ;
        } ;
        const double n ;
    } ;
} ;
void _ ()
{
    struct s x ;
    x . n ;
}
)";

    checkTypes(s, Expectation());
}

void TypeCheckerTester::case0039()
{
    auto s = R"(
struct s {
    struct {
        struct {
            const int m ;
        } ;
    } ;
    const double n ;
} ;
void _ ()
{
    struct s x ;
    x . n ;
}
)";

    checkTypes(s, Expectation());
}
void TypeCheckerTester::case0040()
{
    auto s = R"(
struct s {
    union {
        struct {
           int i , j ;
        } ;
        struct {
           long k , l ;
        } w ;
    } ;
    int m ;
} x ;

void _ ()
{
    x . i ;
}
)";

    checkTypes(s, Expectation());
}

void TypeCheckerTester::case0041()
{
    auto s = R"(
struct {
    union {
        struct {
           int i , j ;
        } ;
        struct {
           long k , l ;
        } w ;
    } ;
    int m ;
} x ;

void _ ()
{
    x . i ;
}
)";

    checkTypes(s, Expectation());
}
void TypeCheckerTester::case0042()
{
    auto s = R"(
struct s { double m ; } x ;
void _ ( )
{
    x . m ;
}
)";

    checkTypes(s, Expectation());
}

void TypeCheckerTester::case0043()
{
    auto s = R"(
struct { double m ; } x ;
void _ ( )
{
    x . m ;
}
)";

    checkTypes(s, Expectation());
}

void TypeCheckerTester::case0044()
{
    auto s = R"(
struct s {
    union {
        struct {
           int i , j ;
        } ;
        struct {
           long k , l ;
        } w ;
    } ;
    int m ;
} x ;

void _ ()
{
    x . w . k ;
}
)";

    checkTypes(s, Expectation());
}

void TypeCheckerTester::case0045()
{
    auto s = R"(
struct s {
    union {
        struct {
           int i , j ;
        } ;
        struct {
           long k , l ;
        } w ;
    } ;
    int m ;
} x ;

void _ ()
{
    x . k ;
}
)";

    checkTypes(s,
               Expectation()
                   .diagnostic(Expectation::ErrorOrWarn::Error,
                               TypeChecker::DiagnosticsReporter::ID_of_UnknownMemberOfTag));
}

void TypeCheckerTester::case0046()
{
    auto s = R"(
struct s {
    struct {
        struct {
            struct {
                const int m ;
            } ;
        } ;
    } ;
} ;
void _ ()
{
    struct s * x ;
    x -> m ;
}
)";

    checkTypes(s, Expectation());
}

void TypeCheckerTester::case0047()
{
    auto s = R"(
struct s {
    struct {
        struct {
            struct {
                const int m ;
            } ;
            struct {
                const int n ;
            } ;
        } ;
    } ;
} ;
void _ ()
{
    struct s * x ;
    x -> m ;
    x -> n ;
}
)";

    checkTypes(s, Expectation());
}

void TypeCheckerTester::case0048()
{
    auto s = R"(
struct s {
    struct {
        struct {
            struct {
                const int m ;
            } ;
            struct {
                const int n ;
            } ;
        } ;
        struct {
            const int o ;
        } ;
    } ;
} ;
void _ ()
{
    struct s * x ;
    x -> m ;
    x -> n ;
    x -> o ;
}
)";

    checkTypes(s, Expectation());
}

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


void TypeCheckerTester::case0100()
{
    auto s = R"(
void _ ()
{
    int x ;
    x ++ ;
}
)";

    checkTypes(s, Expectation());
}

void TypeCheckerTester::case0101()
{
    auto s = R"(
void _ ()
{
    int x ;
    x -- ;
}
)";

    checkTypes(s, Expectation());
}

void TypeCheckerTester::case0102()
{
    // Not a type (but lvalueness) check error.
    auto s = R"(
void _ ()
{
    int const x ;
    x ++ ;
}
)";

    checkTypes(s, Expectation());
}

void TypeCheckerTester::case0103()
{
    auto s = R"(
void _ ()
{
    int * x ;
    x ++ ;
}
)";

    checkTypes(s, Expectation());
}

void TypeCheckerTester::case0104()
{
    auto s = R"(
typedef int t ;
void _ ()
{
    t x ;
    x ++ ;
}
)";

    checkTypes(s, Expectation());
}

void TypeCheckerTester::case0105()
{
    auto s = R"(
typedef int * t ;
void _ ()
{
    t x ;
    x ++ ;
}
)";

    checkTypes(s, Expectation());
}

void TypeCheckerTester::case0106()
{
    // Not a type (but lvalueness) check error.
    auto s = R"(
typedef int const t ;
void _ ()
{
    t x ;
    x ++ ;
}
)";

    checkTypes(s, Expectation());
}

void TypeCheckerTester::case0107()
{
    auto s = R"(
typedef struct s { int m ; } t ;
void _ ()
{
    t x ;
    x ++ ;
}
)";

    checkTypes(s,
               Expectation()
                  .diagnostic(Expectation::ErrorOrWarn::Error,
                              TypeChecker::DiagnosticsReporter::ID_of_InvalidOperator));
}

void TypeCheckerTester::case0108(){}
void TypeCheckerTester::case0109(){}

void TypeCheckerTester::case0130()
{
    auto s = R"(
void _ ( ) {
    int x ;
    x [ 0 ] ;
}
)";

    checkTypes(s,
               Expectation()
                   .diagnostic(Expectation::ErrorOrWarn::Error,
                               TypeChecker::DiagnosticsReporter::ID_of_ExpectedExpressionOfPointerOrArrayType));
}

void TypeCheckerTester::case0131()
{
    auto s = R"(
void _ ( ) {
    const int x ;
    x [ 0 ] ;
}
)";

    checkTypes(s,
               Expectation()
                   .diagnostic(Expectation::ErrorOrWarn::Error,
                               TypeChecker::DiagnosticsReporter::ID_of_ExpectedExpressionOfPointerOrArrayType));
}

void TypeCheckerTester::case0132()
{
    auto s = R"(
void _ ( ) {
    double * x ;
    x [ 0 ] ;
}
)";

    checkTypes(s, Expectation());
}

void TypeCheckerTester::case0133()
{
    auto s = R"(
void _ ( ) {
    const double * x ;
    x [ 0 ] ;
}
)";

    checkTypes(s, Expectation());
}

void TypeCheckerTester::case0134(){
    auto s = R"(
void _ ( ) {
    const double * const x ;
    x [ 0 ] ;
}
)";

    checkTypes(s, Expectation());
}

void TypeCheckerTester::case0135()
{
    auto s = R"(
void _ ( ) {
    double * const x ;
    x [ 0 ] ;
}
)";

    checkTypes(s, Expectation());
}

void TypeCheckerTester::case0136()
{
    auto s = R"(
void _ ( ) {
    double x [ 1 ] ;
    x [ 0 ] ;
}
)";

    checkTypes(s, Expectation());
}

void TypeCheckerTester::case0137()
{
    auto s = R"(
void _ ( ) {
    const double x [ 1 ] ;
    x [ 0 ] ;
}
)";

    checkTypes(s, Expectation());
}

void TypeCheckerTester::case0138()
{
    auto s = R"(
void _ ( ) {
    double * x ;
    x [ 0 ] [ 1 ] ;
}
)";

    checkTypes(s,
               Expectation()
                   .diagnostic(Expectation::ErrorOrWarn::Error,
                               TypeChecker::DiagnosticsReporter::ID_of_ExpectedExpressionOfPointerOrArrayType));
}

void TypeCheckerTester::case0139()
{
    auto s = R"(
void _ ( ) {
    double * * x ;
    x [ 0 ] [ 1 ] ;
}
)";

    checkTypes(s, Expectation());
}

void TypeCheckerTester::case0140()
{
    auto s = R"(
void _ ( ) {
    double x [ 1 ] ;
    x [ 0 ] [ 1 ] ;
}
)";

    checkTypes(s,
               Expectation()
                   .diagnostic(Expectation::ErrorOrWarn::Error,
                               TypeChecker::DiagnosticsReporter::ID_of_ExpectedExpressionOfPointerOrArrayType));
}

void TypeCheckerTester::case0141()
{
    auto s = R"(
void _ ( ) {
    double x [ 1 ] [ 2 ];
    x [ 0 ] [ 1 ] ;
}
)";

    checkTypes(s, Expectation());
}

void TypeCheckerTester::case0142()
{
    auto s = R"(
void _ ( ) {
    double * x ;
    x [ 1.0 ] ;
}
)";

    checkTypes(s,
               Expectation()
                   .diagnostic(Expectation::ErrorOrWarn::Error,
                               TypeChecker::DiagnosticsReporter::ID_of_ExpectedExpressionOfIntegerType));
}

void TypeCheckerTester::case0143()
{
    auto s = R"(
void _ ( ) {
    double x [ 1 ] ;
    x [ 1.0 ] ;
}
)";

    checkTypes(s,
               Expectation()
                   .diagnostic(Expectation::ErrorOrWarn::Error,
                               TypeChecker::DiagnosticsReporter::ID_of_ExpectedExpressionOfIntegerType));
}

void TypeCheckerTester::case0144()
{
    auto s = R"(
void _ ( ) {
    double x [ 1 ] ;
    int y ;
    x [ y ] ;
}
)";

    checkTypes(s, Expectation());
}

void TypeCheckerTester::case0145()
{
    auto s = R"(
void _ ( ) {
    double x [ 1 ] ;
    double y ;
    x [ y ] ;
}
)";

    checkTypes(s,
               Expectation()
                   .diagnostic(Expectation::ErrorOrWarn::Error,
                               TypeChecker::DiagnosticsReporter::ID_of_ExpectedExpressionOfIntegerType));
}

void TypeCheckerTester::case0146(){}
void TypeCheckerTester::case0147(){}
void TypeCheckerTester::case0148(){}
void TypeCheckerTester::case0149(){}
void TypeCheckerTester::case0150(){}
void TypeCheckerTester::case0151(){}
void TypeCheckerTester::case0152(){}
void TypeCheckerTester::case0153(){}
void TypeCheckerTester::case0154(){}
void TypeCheckerTester::case0155(){}
void TypeCheckerTester::case0156(){}
void TypeCheckerTester::case0157(){}
void TypeCheckerTester::case0158(){}
void TypeCheckerTester::case0159(){}


void TypeCheckerTester::case0200()
{
    auto s = R"(
void _ ()
{
    int x ;
    x + 1 ;
}
)";

    checkTypes(s, Expectation());
}

void TypeCheckerTester::case0201()
{
    auto s = R"(
void _ ()
{
    int x ;
    x += 1 ;
}
)";

    checkTypes(s, Expectation());
}

void TypeCheckerTester::case0202()
{
    auto s = R"(
void _ ()
{
    int x ;
    x + "1" ;
}
)";

    checkTypes(s,
               Expectation()
                  .diagnostic(Expectation::ErrorOrWarn::Error,
                              TypeChecker::DiagnosticsReporter::ID_of_InvalidOperator));
}

void TypeCheckerTester::case0203()
{
    auto s = R"(
void _ ()
{
    const int x ;
    x + 1 ;
}
)";

    checkTypes(s, Expectation());
}

void TypeCheckerTester::case0204()
{
    auto s = R"(
void _ ()
{
    short x ;
    x + 1 ;
}
)";

    checkTypes(s, Expectation());
}

void TypeCheckerTester::case0205()
{
    auto s = R"(
void _ ()
{
    long long x ;
    x + 1 ;
}
)";

    checkTypes(s, Expectation());
}

void TypeCheckerTester::case0206()
{
    auto s = R"(
void _ ()
{
    long x ;
    x + "a" ;
}
)";

    checkTypes(s,
               Expectation()
                  .diagnostic(Expectation::ErrorOrWarn::Error,
                              TypeChecker::DiagnosticsReporter::ID_of_InvalidOperator));
}

void TypeCheckerTester::case0207()
{
    auto s = R"(
void _ ()
{
    long long x ;
    x + 1 ;
}
)";

    checkTypes(s, Expectation());
}

void TypeCheckerTester::case0208()
{
    auto s = R"(
void _ ()
{
    long long x ;
    x + 1.0 ;
}
)";

    checkTypes(s, Expectation());
}

void TypeCheckerTester::case0209()
{
    auto s = R"(
void _ ()
{
    double x ;
    x + 1 ;
}
)";

    checkTypes(s, Expectation());
}

void TypeCheckerTester::case0210()
{
    auto s = R"(
void _ ()
{
    struct { int m; } x ;
    x + 1 ;
}
)";

    checkTypes(s,
               Expectation()
                  .diagnostic(Expectation::ErrorOrWarn::Error,
                              TypeChecker::DiagnosticsReporter::ID_of_InvalidOperator));
}

void TypeCheckerTester::case0211()
{
    auto s = R"(
void _ ()
{
    double x ;
    x += 1.0 ;
}
)";

    checkTypes(s, Expectation());
}

void TypeCheckerTester::case0212()
{
    auto s = R"(
void _ ()
{
    int * x ;
    x + 1 ;
}
)";

    checkTypes(s, Expectation());
}

void TypeCheckerTester::case0213()
{
    auto s = R"(
void _ ()
{
    int * x , * y ;
    x + y ;
}
)";

    checkTypes(s,
               Expectation()
                  .diagnostic(Expectation::ErrorOrWarn::Error,
                              TypeChecker::DiagnosticsReporter::ID_of_InvalidOperator));
}

void TypeCheckerTester::case0214()
{
    auto s = R"(
void _ ()
{
    int * x , * y ;
    x - y ;
}
)";

    checkTypes(s, Expectation());
}

void TypeCheckerTester::case0215()
{
    auto s = R"(
void _ ()
{
    int * x , * y ;
    x -= y ;
}
)";

    checkTypes(s, Expectation());
}

void TypeCheckerTester::case0216()
{
    auto s = R"(
void _ ()
{
    int * x , * y ;
    x < y ;
}
)";

    checkTypes(s, Expectation());
}

void TypeCheckerTester::case0217()
{
    auto s = R"(
void _ ()
{
    int x , y ;
    x <= y ;
}
)";

    checkTypes(s, Expectation());
}

void TypeCheckerTester::case0218()
{
    auto s = R"(
void _ ()
{
    long x , y ;
    x << y ;
}
)";

    checkTypes(s, Expectation());
}

void TypeCheckerTester::case0219()
{
    auto s = R"(
void _ ()
{
    long long x ;
    x * 1 ;
}
)";

    checkTypes(s, Expectation());
}

void TypeCheckerTester::case0220()
{
    auto s = R"(
void _ ()
{
    long x ;
    int y ;
    x / y ;
}
)";

    checkTypes(s, Expectation());
}

void TypeCheckerTester::case0221()
{
    auto s = R"(
void _ ()
{
    long x ;
    int * y ;
    x / y ;
}
)";

    checkTypes(s,
               Expectation()
                  .diagnostic(Expectation::ErrorOrWarn::Error,
                              TypeChecker::DiagnosticsReporter::ID_of_ExpectedExpressionOfArithmeticType));
}

void TypeCheckerTester::case0222()
{
    auto s = R"(
void _ ()
{
    long x ;
    int * y ;
    x * y ;
}
)";

    checkTypes(s,
               Expectation()
                  .diagnostic(Expectation::ErrorOrWarn::Error,
                              TypeChecker::DiagnosticsReporter::ID_of_ExpectedExpressionOfArithmeticType));
}

void TypeCheckerTester::case0223()
{
    auto s = R"(
void _ ()
{
    long x ;
    int y ;
    x == y ;
}
)";

    checkTypes(s, Expectation());
}

void TypeCheckerTester::case0224()
{
    auto s = R"(
void _ ()
{
    int x ;
    int y ;
    x == y ;
}
)";

    checkTypes(s, Expectation());
}

void TypeCheckerTester::case0225()
{
    auto s = R"(
void _ ()
{
    int x ;
    int y ;
    x != y ;
}
)";

    checkTypes(s, Expectation());
}

void TypeCheckerTester::case0226()
{
    auto s = R"(
void _ ()
{
    int * x ;
    int y ;
    x == y ;
}
)";

    checkTypes(s,
               Expectation()
                  .diagnostic(Expectation::ErrorOrWarn::Error,
                              TypeChecker::DiagnosticsReporter::ID_of_InvalidOperator));
}

void TypeCheckerTester::case0227()
{
    auto s = R"(
void _ ()
{
    int * x ;
    int * y ;
    x == y ;
}
)";

    checkTypes(s, Expectation());
}

void TypeCheckerTester::case0228()
{
    auto s = R"(
void _ ()
{
    int * x ;
    double * y ;
    x == y ;
}
)";

    checkTypes(s,
               Expectation()
                  .diagnostic(Expectation::ErrorOrWarn::Error,
                              TypeChecker::DiagnosticsReporter::ID_of_InvalidOperator));
}

void TypeCheckerTester::case0229()
{
    auto s = R"(
void _ ()
{
    const int * x ;
    int * y ;
    x == y ;
}
)";

    checkTypes(s, Expectation());
}

void TypeCheckerTester::case0230()
{
    auto s = R"(
void _ ()
{
    const int * x ;
    const int * y ;
    x == y ;
}
)";

    checkTypes(s, Expectation());
}

void TypeCheckerTester::case0231()
{
    auto s = R"(
void _ ()
{
    int * x ;
    int const * y ;
    x == y ;
}
)";

    checkTypes(s, Expectation());
}

void TypeCheckerTester::case0232()
{
    auto s = R"(
void _ ()
{
    void * x ;
    int * y ;
    x == y ;
}
)";

    checkTypes(s, Expectation());
}

void TypeCheckerTester::case0233()
{
    auto s = R"(
void _ ()
{
    int * x ;
    void * y ;
    x == y ;
}
)";

    checkTypes(s, Expectation());
}

void TypeCheckerTester::case0234()
{
    auto s = R"(
void _ ()
{
    const void * x ;
    void * y ;
    x == y ;
}
)";

    checkTypes(s, Expectation());
}

void TypeCheckerTester::case0235()
{
    auto s = R"(
void _ ()
{
    const void * x ;
    const void * y ;
    x == y ;
}
)";

    checkTypes(s, Expectation());
}

void TypeCheckerTester::case0236()
{
    auto s = R"(
void _ ()
{
    void * x ;
    const void * y ;
    x == y ;
}
)";

    checkTypes(s, Expectation());
}

void TypeCheckerTester::case0237()
{
    auto s = R"(
void _ ()
{
    int * x ;
    void * y ;
    x == y ;
}
)";

    checkTypes(s, Expectation());
}

void TypeCheckerTester::case0238()
{
    auto s = R"(
void _ ()
{
    void * x ;
    int * y ;
    x == y ;
}
)";

    checkTypes(s, Expectation());
}

void TypeCheckerTester::case0239()
{
    auto s = R"(
void _ ()
{
    int * x ;
    x == 0 ;
}
)";

    checkTypes(s, Expectation());
}

void TypeCheckerTester::case0240()
{
    auto s = R"(
void _ ()
{
    int * x ;
    0 == x ;
}
)";

    checkTypes(s, Expectation());
}

void TypeCheckerTester::case0241()
{
    auto s = R"(
void _ ()
{
    void * x ;
    x == 0 ;
}
)";

    checkTypes(s, Expectation());
}

void TypeCheckerTester::case0242()
{
    auto s = R"(
void _ ()
{
    void * x ;
    0 == x ;
}
)";

    checkTypes(s, Expectation());
}

void TypeCheckerTester::case0243()
{
    auto s = R"(
void _ ()
{
    int * x ;
    x == 1 ;
}
)";

    checkTypes(s,
               Expectation()
                  .diagnostic(Expectation::ErrorOrWarn::Error,
                              TypeChecker::DiagnosticsReporter::ID_of_InvalidOperator));
}

void TypeCheckerTester::case0244()
{
    auto s = R"(
void _ ()
{
    int * x ;
    x <= 1 ;
}
)";

    checkTypes(s,
               Expectation()
                  .diagnostic(Expectation::ErrorOrWarn::Error,
                              TypeChecker::DiagnosticsReporter::ID_of_InvalidOperator));
}

void TypeCheckerTester::case0245()
{
    auto s = R"(
void _ ()
{
    int * x ;
    x > 1 ;
}
)";

    checkTypes(s,
               Expectation()
                  .diagnostic(Expectation::ErrorOrWarn::Error,
                              TypeChecker::DiagnosticsReporter::ID_of_InvalidOperator));
}

void TypeCheckerTester::case0246()
{
    auto s = R"(
void _ ()
{
    int * x ;
    x <= 0 ;
}
)";

    checkTypes(s,
               Expectation()
                  .diagnostic(Expectation::ErrorOrWarn::Error,
                              TypeChecker::DiagnosticsReporter::ID_of_InvalidOperator));
}

void TypeCheckerTester::case0247()
{
    auto s = R"(
void _ ()
{
    int * x ;
    x > 0 ;
}
)";

    checkTypes(s,
               Expectation()
                  .diagnostic(Expectation::ErrorOrWarn::Error,
                              TypeChecker::DiagnosticsReporter::ID_of_InvalidOperator));
}

void TypeCheckerTester::case0248()
{
    auto s = R"(
void _ ()
{
    int * x ;
    0 < x ;
}
)";

    checkTypes(s,
               Expectation()
                  .diagnostic(Expectation::ErrorOrWarn::Error,
                              TypeChecker::DiagnosticsReporter::ID_of_InvalidOperator));
}

void TypeCheckerTester::case0249(){}
void TypeCheckerTester::case0250(){}
void TypeCheckerTester::case0251(){}
void TypeCheckerTester::case0252(){}
void TypeCheckerTester::case0253(){}
void TypeCheckerTester::case0254(){}
void TypeCheckerTester::case0255(){}
void TypeCheckerTester::case0256(){}
void TypeCheckerTester::case0257(){}
void TypeCheckerTester::case0258(){}
void TypeCheckerTester::case0259(){}

void TypeCheckerTester::case0300()
{
    auto s = R"(
void _ ()
{
    int x ;
    x ( ) ;
}
    )";

    checkTypes(
        s,
        Expectation()
            .diagnostic(
                Expectation::ErrorOrWarn::Error,
                TypeChecker::DiagnosticsReporter::ID_of_ExpectedExpressionOfFunctionOrFunctionPointerType));
}

void TypeCheckerTester::case0301()
{
    auto s = R"(
void _ ()
{
    double * x ;
    x ( ) ;
}
    )";

    checkTypes(
        s,
        Expectation()
            .diagnostic(
                Expectation::ErrorOrWarn::Error,
                TypeChecker::DiagnosticsReporter::ID_of_ExpectedExpressionOfFunctionOrFunctionPointerType));
}

void TypeCheckerTester::case0302()
{
    auto s = R"(
void _ ()
{
    double * * x ;
    x ( ) ;
}
    )";

    checkTypes(
        s,
        Expectation()
            .diagnostic(
                Expectation::ErrorOrWarn::Error,
                TypeChecker::DiagnosticsReporter::ID_of_ExpectedExpressionOfFunctionOrFunctionPointerType));
}

void TypeCheckerTester::case0303()
{
    auto s = R"(
void f () {}
void _ ()
{
    f ( );
}
    )";

    checkTypes(s, Expectation());
}

void TypeCheckerTester::case0304()
{
    auto s = R"(
void f () {}
void _ ()
{
    ( * f ) ( );
}
    )";

    checkTypes(s, Expectation());
}

void TypeCheckerTester::case0305()
{
    auto s = R"(
void f () {}
void _ ()
{
    ( * * * f ) ( );
}
    )";

    checkTypes(s, Expectation());
}

void TypeCheckerTester::case0306()
{
    auto s = R"(
void _ ()
{
    typedef void ( * t ) ();
    t x = _ ;
    x ( ) ;
}
    )";

    checkTypes(s, Expectation());
}

void TypeCheckerTester::case0307()
{
    auto s = R"(
void _ ()
{
    typedef void ( * t ) ();
    t x = _ ;
    ( * x ) ( ) ;
}
    )";

    checkTypes(s, Expectation());
}

void TypeCheckerTester::case0308()
{
    auto s = R"(
void f () {}
void _ ()
{
    ( f ) ( );
}
    )";

    checkTypes(s, Expectation());
}

void TypeCheckerTester::case0309()
{
    auto s = R"(
void f () {}
void _ ()
{
    ( & f ) ( );
}
    )";

    checkTypes(s, Expectation());
}


void TypeCheckerTester::case0310()
{
    auto s = R"(
void f () ;
void _ ()
{
    f ( );
}
    )";

    checkTypes(s, Expectation());
}

void TypeCheckerTester::case0311()
{
    auto s = R"(
void f () ;
void _ ()
{
    ( * f ) ( );
}
    )";

    checkTypes(s, Expectation());
}

void TypeCheckerTester::case0312()
{
    auto s = R"(
void f ( int x ) { }
void _ ()
{
    f ( );
}
    )";

    checkTypes(
        s,
        Expectation()
            .diagnostic(
                Expectation::ErrorOrWarn::Error,
                TypeChecker::DiagnosticsReporter::ID_of_TooFewArgumentsToFunctionCall));
}

void TypeCheckerTester::case0313()
{
    auto s = R"(
void f ( int x ) { }
void _ ()
{
    ( * f ) ( );
}
    )";

    checkTypes(
        s,
        Expectation()
            .diagnostic(
                Expectation::ErrorOrWarn::Error,
                TypeChecker::DiagnosticsReporter::ID_of_TooFewArgumentsToFunctionCall));
}

void TypeCheckerTester::case0314()
{
    auto s = R"(
void f ( int x , double y ) { }
void _ ()
{
    f ( );
}
    )";

    checkTypes(
        s,
        Expectation()
            .diagnostic(
                Expectation::ErrorOrWarn::Error,
                TypeChecker::DiagnosticsReporter::ID_of_TooFewArgumentsToFunctionCall));
}

void TypeCheckerTester::case0315()
{
    auto s = R"(
void f ( int x , double y ) { }
void _ ()
{
    f ( 1 );
}
    )";

    checkTypes(
        s,
        Expectation()
            .diagnostic(
                Expectation::ErrorOrWarn::Error,
                TypeChecker::DiagnosticsReporter::ID_of_TooFewArgumentsToFunctionCall));
}

void TypeCheckerTester::case0316(){}
void TypeCheckerTester::case0317(){}
void TypeCheckerTester::case0318(){}
void TypeCheckerTester::case0319(){}
void TypeCheckerTester::case0320(){}
void TypeCheckerTester::case0321(){}
void TypeCheckerTester::case0322(){}
void TypeCheckerTester::case0323(){}
void TypeCheckerTester::case0324(){}
void TypeCheckerTester::case0325(){}
void TypeCheckerTester::case0326(){}
void TypeCheckerTester::case0327(){}
void TypeCheckerTester::case0328(){}
void TypeCheckerTester::case0329(){}
void TypeCheckerTester::case0330(){}
void TypeCheckerTester::case0331(){}
void TypeCheckerTester::case0332(){}
void TypeCheckerTester::case0333(){}
void TypeCheckerTester::case0334(){}
void TypeCheckerTester::case0335(){}
void TypeCheckerTester::case0336(){}
void TypeCheckerTester::case0337(){}
void TypeCheckerTester::case0338(){}
void TypeCheckerTester::case0339(){}
void TypeCheckerTester::case0340(){}
void TypeCheckerTester::case0341(){}
void TypeCheckerTester::case0342(){}
void TypeCheckerTester::case0343(){}
void TypeCheckerTester::case0344(){}
void TypeCheckerTester::case0345(){}
void TypeCheckerTester::case0346(){}
void TypeCheckerTester::case0347(){}
void TypeCheckerTester::case0348(){}
void TypeCheckerTester::case0349(){}
void TypeCheckerTester::case0350(){}
void TypeCheckerTester::case0351(){}
void TypeCheckerTester::case0352(){}
void TypeCheckerTester::case0353(){}
void TypeCheckerTester::case0354(){}
void TypeCheckerTester::case0355(){}
void TypeCheckerTester::case0356(){}
void TypeCheckerTester::case0357(){}
void TypeCheckerTester::case0358(){}
void TypeCheckerTester::case0359(){}
void TypeCheckerTester::case0360(){}
void TypeCheckerTester::case0361(){}
void TypeCheckerTester::case0362(){}
void TypeCheckerTester::case0363(){}
void TypeCheckerTester::case0364(){}
void TypeCheckerTester::case0365(){}
void TypeCheckerTester::case0366(){}
void TypeCheckerTester::case0367(){}
void TypeCheckerTester::case0368(){}
void TypeCheckerTester::case0369(){}

void TypeCheckerTester::case0500()
{
    auto s = R"(
void _ ()
{
    int x ;
    + x ;
}
)";

    checkTypes(s, Expectation());
}

void TypeCheckerTester::case0501()
{
    auto s = R"(
void _ ()
{
    int x ;
    + x ;
}
)";

    checkTypes(s, Expectation());
}

void TypeCheckerTester::case0502()
{
    auto s = R"(
void _ ()
{
    int x ;
    + x ;
}
)";

    checkTypes(s, Expectation());
}

void TypeCheckerTester::case0503()
{
    auto s = R"(
void _ ()
{
    short x ;
    + x ;
}
)";

    checkTypes(s, Expectation());
}

void TypeCheckerTester::case0504()
{
    auto s = R"(
void _ ()
{
    float x ;
    + x ;
}
)";

    checkTypes(s, Expectation());
}

void TypeCheckerTester::case0505()
{
    auto s = R"(
void _ ()
{
    int long x ;
    + x ;
}
)";

    checkTypes(s, Expectation());
}

void TypeCheckerTester::case0506()
{
    auto s = R"(
void _ ()
{
    int long x ;
    - x ;
}
)";

    checkTypes(s, Expectation());
}

void TypeCheckerTester::case0507()
{
    auto s = R"(
void _ ()
{
    double x ;
    + x ;
}
)";

    checkTypes(s, Expectation());
}

void TypeCheckerTester::case0508()
{
    auto s = R"(
void _ ()
{
    double x ;
    - x ;
}
)";

    checkTypes(s, Expectation());
}

void TypeCheckerTester::case0509()
{
    auto s = R"(
void _ ()
{
    int * x ;
    + x ;
}
)";

    checkTypes(s,
               Expectation()
                   .diagnostic(Expectation::ErrorOrWarn::Error,
                               TypeChecker::DiagnosticsReporter::ID_of_ExpectedExpressionOfArithmeticType));
}

void TypeCheckerTester::case0510()
{
    auto s = R"(
void _ ()
{
    const int x ;
    - x ;
}
)";

    checkTypes(s, Expectation());
}

void TypeCheckerTester::case0511()
{
    auto s = R"(
void _ ()
{
    const double x ;
    - x ;
}
)";

    checkTypes(s, Expectation());
}

void TypeCheckerTester::case0512()
{
    auto s = R"(
void _ ()
{
    int x ;
    ++ x ;
}
)";

    checkTypes(s, Expectation());
}

void TypeCheckerTester::case0513()
{
    auto s = R"(
void _ ()
{
    int x ;
    -- x ;
}
)";

    checkTypes(s, Expectation());
}

void TypeCheckerTester::case0514()
{
    auto s = R"(
void _ ()
{
    short x ;
    ++ x ;
}
)";

    checkTypes(s, Expectation());
}

void TypeCheckerTester::case0515()
{
    auto s = R"(
void _ ()
{
    int const * x ;
    + x ;
}
)";

    checkTypes(s,
               Expectation()
                   .diagnostic(Expectation::ErrorOrWarn::Error,
                               TypeChecker::DiagnosticsReporter::ID_of_ExpectedExpressionOfArithmeticType));
}

void TypeCheckerTester::case0516()
{
    auto s = R"(
void _ ()
{
    int const * const x ;
    + x ;
}
)";

    checkTypes(s,
               Expectation()
                   .diagnostic(Expectation::ErrorOrWarn::Error,
                               TypeChecker::DiagnosticsReporter::ID_of_ExpectedExpressionOfArithmeticType));
}

void TypeCheckerTester::case0517()
{
    auto s = R"(
void _ ()
{
    int const * const x ;
    - x ;
}
)";

    checkTypes(s,
               Expectation()
                   .diagnostic(Expectation::ErrorOrWarn::Error,
                               TypeChecker::DiagnosticsReporter::ID_of_ExpectedExpressionOfArithmeticType));
}

void TypeCheckerTester::case0518()
{
    // Not a type (but lvalueness) check error.
    auto s = R"(
void _ ()
{
    const int x ;
    ++ x ;
}
)";

    checkTypes(s, Expectation());
}

void TypeCheckerTester::case0519()
{
    // Not a type (but lvalueness) check error.
    auto s = R"(
void _ ()
{
    const int x ;
    -- x ;
}
)";

    checkTypes(s, Expectation());
}

void TypeCheckerTester::case0520()
{
    // Not a type (but lvalueness) check error.
    auto s = R"(
void _ ()
{
    const int * const x ;
    ++ x ;
}
)";

    checkTypes(s, Expectation());
}

void TypeCheckerTester::case0521()
{
    auto s = R"(
void _ ()
{
    const int * x ;
    ++ x ;
}
)";

    checkTypes(s, Expectation());
}

void TypeCheckerTester::case0522()
{
    auto s = R"(
void _ ()
{
    int * x ;
    ++ x ;
}
)";

    checkTypes(s, Expectation());
}

void TypeCheckerTester::case0523()
{
    auto s = R"(
struct s { int m ; } ;
void _ ()
{
    struct s x ;
    ++ x ;
}
)";

    checkTypes(s,
               Expectation()
                   .diagnostic(Expectation::ErrorOrWarn::Error,
                               TypeChecker::DiagnosticsReporter::ID_of_InvalidOperator));
}

void TypeCheckerTester::case0524()
{
    auto s = R"(
struct s { int m ; } ;
void _ ()
{
    struct s * x ;
    ++ x ;
}
)";

    checkTypes(s, Expectation());
}

void TypeCheckerTester::case0525()
{
    auto s = R"(
void _ ()
{
    int x ;
    ~ x ;
}
)";

    checkTypes(s, Expectation());
}

void TypeCheckerTester::case0526()
{
    auto s = R"(
void _ ()
{
    unsigned long x ;
    ~ x ;
}
)";

    checkTypes(s, Expectation());
}

void TypeCheckerTester::case0527()
{
    auto s = R"(
void _ ()
{
    long x ;
    ~ x ;
}
)";

    checkTypes(s, Expectation());
}

void TypeCheckerTester::case0528()
{
    auto s = R"(
void _ ()
{
    unsigned long const x ;
    ~ x ;
}
)";

    checkTypes(s, Expectation());
}

void TypeCheckerTester::case0529()
{
    auto s = R"(
void _ ()
{
    float x ;
    ~ x ;
}
)";

    checkTypes(s,
               Expectation()
                   .diagnostic(Expectation::ErrorOrWarn::Error,
                               TypeChecker::DiagnosticsReporter::ID_of_ExpectedExpressionOfIntegerType));
}

void TypeCheckerTester::case0530()
{
    auto s = R"(
void _ ()
{
    double x ;
    ~ x ;
}
)";

    checkTypes(s,
               Expectation()
                   .diagnostic(Expectation::ErrorOrWarn::Error,
                               TypeChecker::DiagnosticsReporter::ID_of_ExpectedExpressionOfIntegerType));
}

void TypeCheckerTester::case0531()
{
    auto s = R"(
void _ ()
{
    const double x ;
    ~ x ;
}
)";

    checkTypes(s,
               Expectation()
                   .diagnostic(Expectation::ErrorOrWarn::Error,
                               TypeChecker::DiagnosticsReporter::ID_of_ExpectedExpressionOfIntegerType));
}

void TypeCheckerTester::case0532()
{
    auto s = R"(
void _ ()
{
    int * x ;
    ~ x ;
}
)";

    checkTypes(s,
               Expectation()
                   .diagnostic(Expectation::ErrorOrWarn::Error,
                               TypeChecker::DiagnosticsReporter::ID_of_ExpectedExpressionOfIntegerType));
}

void TypeCheckerTester::case0533()
{
    auto s = R"(
struct s { int m ; } ;
void _ ()
{
    struct s x ;
    ! x ;
}
)";

    checkTypes(s,
               Expectation()
                   .diagnostic(Expectation::ErrorOrWarn::Error,
                               TypeChecker::DiagnosticsReporter::ID_of_ExpectedExpressionOfScalarType));
}

void TypeCheckerTester::case0534()
{
    auto s = R"(
void _ ()
{
    int x ;
    ! x ;
}
)";

    checkTypes(s, Expectation());
}

void TypeCheckerTester::case0535()
{
    auto s = R"(
void _ ()
{
    int * x ;
    ! x ;
}
)";

    checkTypes(s, Expectation());
}

void TypeCheckerTester::case0536()
{
    auto s = R"(
void _ ()
{
    double * x ;
    ! x ;
}
)";

    checkTypes(s, Expectation());
}

void TypeCheckerTester::case0537()
{
    auto s = R"(
void _ ()
{
    int * const x ;
    ! x ;
}
)";

    checkTypes(s, Expectation());
}

void TypeCheckerTester::case0538()
{
    auto s = R"(
void _ ()
{
    int const * const x ;
    ! x ;
}
)";

    checkTypes(s, Expectation());
}

void TypeCheckerTester::case0539()
{
    auto s = R"(
void _ ()
{
    int x ;
    * x ;
}
)";

    checkTypes(s,
               Expectation()
                   .diagnostic(Expectation::ErrorOrWarn::Error,
                               TypeChecker::DiagnosticsReporter::ID_of_ExpectedExpressionOfPointerType));
}

void TypeCheckerTester::case0540()
{
    auto s = R"(
void _ ()
{
    int * x ;
    * x ;
}
)";

    checkTypes(s, Expectation());
}

void TypeCheckerTester::case0541()
{
    auto s = R"(
void _ ()
{
    int * const x ;
    * x ;
}
)";

    checkTypes(s, Expectation());
}

void TypeCheckerTester::case0542()
{
    auto s = R"(
void _ ()
{
    int const * const x ;
    * x ;
}
)";

    checkTypes(s, Expectation());
}

void TypeCheckerTester::case0543()
{
    auto s = R"(
struct s { int m ; } ;
void _ ()
{
    struct s * x ;
    * x ;
}
)";

    checkTypes(s, Expectation());
}

void TypeCheckerTester::case0544()
{
    auto s = R"(
typedef struct s { int m ; } t ;
void _ ()
{
    t * x ;
    * x ;
}
)";

    checkTypes(s, Expectation());
}

void TypeCheckerTester::case0545()
{
    auto s = R"(
typedef double t ;
void _ ()
{
    t * x ;
    ! x ;
}
)";

    checkTypes(s, Expectation());
}

void TypeCheckerTester::case0546()
{
    auto s = R"(
typedef struct s { int m ; } t ;
void _ ()
{
    const t * x ;
    * x ;
}
)";

    checkTypes(s, Expectation());
}

void TypeCheckerTester::case0547()
{
    auto s = R"(
typedef const struct s { int m ; } t ;
void _ ()
{
    const t * x ;
    * x ;
}
)";

    checkTypes(s, Expectation());
}

void TypeCheckerTester::case0548()
{
    auto s = R"(
typedef const struct s { int m ; } t ;
void _ ()
{
    t * x ;
    * x ;
}
)";

    checkTypes(s, Expectation());
}

void TypeCheckerTester::case0549()
{
    auto s = R"(
typedef const struct s { int m ; } t ;
void _ ()
{
    t x ;
    & x ;
}
)";

    checkTypes(s, Expectation());
}

void TypeCheckerTester::case0550()
{
    auto s = R"(
typedef const struct s { int m ; } t ;
void _ ()
{
    t * x ;
    & x ;
}
)";

    checkTypes(s, Expectation());
}

void TypeCheckerTester::case0551()
{
    auto s = R"(
typedef const struct s { int m ; } t ;
void _ ()
{
    t * * x ;
    & x ;
}
)";

    checkTypes(s, Expectation());
}

void TypeCheckerTester::case0552()
{
    auto s = R"(
typedef const struct s { int m ; } t ;
void _ ()
{
    t * const x ;
    & x ;
}
)";

    checkTypes(s, Expectation());
}

void TypeCheckerTester::case0553()
{
    auto s = R"(
typedef const struct s { int m ; } t ;
void _ ()
{
    t const * x ;
    & x ;
}
)";

    checkTypes(s, Expectation());
}

void TypeCheckerTester::case0554()
{
    auto s = R"(
void _ ()
{
    int x ;
    & x ;
}
)";

    checkTypes(s, Expectation());
}

void TypeCheckerTester::case0555()
{
    auto s = R"(
struct s { int m ; } t ;
void _ ()
{
    struct s * x ;
    & x ;
}
)";

    checkTypes(s, Expectation());
}

void TypeCheckerTester::case0556()
{
    auto s = R"(
struct s { int m ; } t ;
void _ ()
{
    struct s x ;
    & x ;
}
)";

    checkTypes(s, Expectation());
}

void TypeCheckerTester::case0557()
{
    auto s = R"(
typedef int t ;
void _ ()
{
    t x ;
    & x ;
}
)";

    checkTypes(s, Expectation());
}

void TypeCheckerTester::case0558()
{
    auto s = R"(
void _ ()
{
    float x ;
    & x ;
}
)";

    checkTypes(s, Expectation());
}

void TypeCheckerTester::case0559()
{
    // Not a type (but lvalueness) check error.
    auto s = R"(
void _ ()
{
     & 1 ;
}
)";

    checkTypes(s, Expectation());
}

void TypeCheckerTester::case0560()
{
    auto s = R"(
void _ ()
{
    const float x ;
    & x ;
}
)";

    checkTypes(s, Expectation());
}

void TypeCheckerTester::case0561()
{
    auto s = R"(
void _ ()
{
    const int * x ;
    & x ;
}
)";

    checkTypes(s, Expectation());
}

void TypeCheckerTester::case0562()
{
    auto s = R"(
void _ ()
{
    const int * const x ;
    & x ;
}
)";

    checkTypes(s, Expectation());
}

void TypeCheckerTester::case0563()
{
    auto s = R"(
typedef const int t ;
void _ ()
{
    t x ;
    & x ;
}
)";

    checkTypes(s, Expectation());
}

void TypeCheckerTester::case0564()
{
    auto s = R"(
typedef int t ;
void _ ()
{
    t x ;
    ++ x ;
}
)";

    checkTypes(s, Expectation());
}

void TypeCheckerTester::case0565()
{
    auto s = R"(
typedef const int t ;
void _ ()
{
    t x ;
    ++ x ;
}
)";

    checkTypes(s, Expectation());
}

void TypeCheckerTester::case0566()
{
    auto s = R"(
typedef int * t ;
void _ ()
{
    t x ;
    ++ x ;
}
)";

    checkTypes(s, Expectation());
}

void TypeCheckerTester::case0567()
{
    auto s = R"(
void f () {}
void _ ()
{
    * f ;
}
)";

    checkTypes(s, Expectation());
}

void TypeCheckerTester::case0568()
{
    auto s = R"(
void f () {}
void _ ()
{
    * * f ;
}
)";

    checkTypes(s, Expectation());
}

void TypeCheckerTester::case0569()
{
    auto s = R"(
void _ ()
{
    int x [ 1 ] ;
    * x ;
}
)";

    checkTypes(s, Expectation());
}


void TypeCheckerTester::case0700()
{
    auto s = R"(
void _ ()
{
    int x , y ;
    x = y ;
}
)";

    checkTypes(s, Expectation());
}

void TypeCheckerTester::case0701()
{
    auto s = R"(
void _ ()
{
    int x , y ;
    x = y ;
}
)";

    checkTypes(s, Expectation());
}

void TypeCheckerTester::case0702()
{
    auto s = R"(
struct s { int m ; } ;
void _ ()
{
    struct s x , y ;
    x = y ;
    y = x ;
}
)";

    checkTypes(s, Expectation());
}

void TypeCheckerTester::case0703()
{
    auto s = R"(
struct s { int m ; } ;
struct r { int m ; } ;
void _ ()
{
    struct s x ;
    struct r y ;
    x = y ;
}
)";

    checkTypes(
        s,
        Expectation()
            .diagnostic(
                Expectation::ErrorOrWarn::Error,
                TypeChecker::DiagnosticsReporter::ID_of_IncompatibleTypesInAssignment));
}

void TypeCheckerTester::case0704()
{
    auto s = R"(
struct s { int m ; } ;
typedef struct s t ;
typedef
void _ ()
{
    struct s x ;
    t y ;
    x = y ;
    y = x ;
}
)";

    checkTypes(s, Expectation());
}

void TypeCheckerTester::case0705()
{
    auto s = R"(
struct s { int m ; } ;
typedef struct r { int m ; } t ;
void _ ()
{
    struct s x ;
    t y ;
    x = y ;
}
)";

    checkTypes(
        s,
        Expectation()
            .diagnostic(
                Expectation::ErrorOrWarn::Error,
                TypeChecker::DiagnosticsReporter::ID_of_IncompatibleTypesInAssignment));
}

void TypeCheckerTester::case0706()
{
    auto s = R"(
struct s { int m ; } ;
typedef struct r { int m ; } t ;
void _ ()
{
    struct s x ;
    t y ;
    y = x ;
}
)";

    checkTypes(
        s,
        Expectation()
            .diagnostic(
                Expectation::ErrorOrWarn::Error,
                TypeChecker::DiagnosticsReporter::ID_of_IncompatibleTypesInAssignment));
}

void TypeCheckerTester::case0707()
{
    auto s = R"(
struct s { int m ; } ;
void _ ()
{
    struct s x ;
    int y ;
    x = y ;
}
)";

    checkTypes(
        s,
        Expectation()
            .diagnostic(
                Expectation::ErrorOrWarn::Error,
                TypeChecker::DiagnosticsReporter::ID_of_IncompatibleTypesInAssignment));
}

void TypeCheckerTester::case0708()
{
    auto s = R"(
struct s { int m ; } ;
void _ ()
{
    struct s x ;
    int y ;
    y = x ;
}
)";

    checkTypes(
        s,
        Expectation()
            .diagnostic(
                Expectation::ErrorOrWarn::Error,
                TypeChecker::DiagnosticsReporter::ID_of_IncompatibleTypesInAssignment));
}

void TypeCheckerTester::case0709()
{
    auto s = R"(
struct s { int m ; } ;
void _ ()
{
    struct s * x ;
    int y ;
    x = y ;
}
)";

    checkTypes(
        s,
        Expectation()
            .diagnostic(
                Expectation::ErrorOrWarn::Error,
                TypeChecker::DiagnosticsReporter::ID_of_IncompatibleTypesInAssignment));
}

void TypeCheckerTester::case0710()
{
    auto s = R"(
struct s { int m ; } ;
union u { int i ; double d ; } ;
void _ ()
{
    struct s x ;
    union u y ;
    x = y ;
}
)";

    checkTypes(
        s,
        Expectation()
            .diagnostic(
                Expectation::ErrorOrWarn::Error,
                TypeChecker::DiagnosticsReporter::ID_of_IncompatibleTypesInAssignment));
}

void TypeCheckerTester::case0711()
{
    auto s = R"(
void _ ()
{
    const int x ;
    int y ;
    x = y ;
}
)";

    checkTypes(
        s,
        Expectation()
            .diagnostic(
                Expectation::ErrorOrWarn::Error,
                TypeChecker::DiagnosticsReporter::ID_of_CannotAssignToExpressionOfConstQualifiedType));
}

void TypeCheckerTester::case0712()
{
    auto s = R"(
void _ ()
{
    const int x ;
    int y ;
    y = x ;
}
)";

    checkTypes(s, Expectation());
}

void TypeCheckerTester::case0713()
{
    auto s = R"(
void _ ()
{
    int * x , * y ;
    x = y ;
}
)";

    checkTypes(s, Expectation());
}

void TypeCheckerTester::case0714()
{
    auto s = R"(
void _ ()
{
    int * x , * y ;
    y = x ;
}
)";

    checkTypes(s, Expectation());
}

void TypeCheckerTester::case0715()
{
    // TODO: integer x pointer
    auto s = R"(
void _ ()
{
    int * x , y ;
    y = x ;
}
)";

    checkTypes(
        s,
        Expectation()
            .diagnostic(
                Expectation::ErrorOrWarn::Error,
                TypeChecker::DiagnosticsReporter::ID_of_IncompatibleTypesInAssignment));
}

void TypeCheckerTester::case0716()
{
    // TODO: integer x pointer
    auto s = R"(
void _ ()
{
    int * x , y ;
    x = y ;
}
)";

    checkTypes(
        s,
        Expectation()
            .diagnostic(
                Expectation::ErrorOrWarn::Error,
                TypeChecker::DiagnosticsReporter::ID_of_IncompatibleTypesInAssignment));
}

void TypeCheckerTester::case0717()
{
    auto s = R"(
void _ ()
{
    int * x ;
    void * y ;
    y = x ;
}
)";

    checkTypes(s, Expectation());
}

void TypeCheckerTester::case0718()
{
    auto s = R"(
void _ ()
{
    int * x ;
    void * y ;
    x = y ;
}
)";

    checkTypes(s, Expectation());
}

void TypeCheckerTester::case0719()
{
    auto s = R"(
void _ ()
{
    int * x ;
    x = 0 ;
}
)";

    checkTypes(s, Expectation());
}

void TypeCheckerTester::case0720()
{
    auto s = R"(
void _ ()
{
    int * x ;
    0 = x ;
}
)";

    checkTypes(
        s,
        Expectation()
            .diagnostic(
                Expectation::ErrorOrWarn::Error,
                TypeChecker::DiagnosticsReporter::ID_of_IncompatibleTypesInAssignment));
}

void TypeCheckerTester::case0721()
{
    auto s = R"(
void _ ()
{
    void * x ;
    x = 0 ;
}
)";

    checkTypes(s, Expectation());
}

void TypeCheckerTester::case0722()
{
    auto s = R"(
void _ ()
{
    void * x ;
    0 = x ;
}
)";

    checkTypes(
        s,
        Expectation()
            .diagnostic(
                Expectation::ErrorOrWarn::Error,
                TypeChecker::DiagnosticsReporter::ID_of_IncompatibleTypesInAssignment));
}

void TypeCheckerTester::case0723()
{
    auto s = R"(
void _ ()
{
    int * x ;
    double * y ;
    x = y;
}
)";

    checkTypes(
        s,
        Expectation()
            .diagnostic(
                Expectation::ErrorOrWarn::Error,
                TypeChecker::DiagnosticsReporter::ID_of_IncompatibleTypesInAssignment));
}

void TypeCheckerTester::case0724()
{
    auto s = R"(
void _ ()
{
    int * x ;
    double * y ;
    x = y;
}
)";

    checkTypes(
        s,
        Expectation()
            .diagnostic(
                Expectation::ErrorOrWarn::Error,
                TypeChecker::DiagnosticsReporter::ID_of_IncompatibleTypesInAssignment));
}

void TypeCheckerTester::case0725()
{
    // TODO: value change
    auto s = R"(
void _ ()
{
    int x ;
    double y ;
    x = y;
}
)";

    checkTypes(s, Expectation());
}

void TypeCheckerTester::case0726()
{
    auto s = R"(
void _ ()
{
    int x ;
    double y ;
    y = x ;
}
)";

    checkTypes(s, Expectation());
}

void TypeCheckerTester::case0727()
{
    // TODO: value change
    auto s = R"(
void _ ()
{
    int x ;
    long long y ;
    x = y;
}
)";

    checkTypes(s, Expectation());
}

void TypeCheckerTester::case0728()
{
    auto s = R"(
void _ ()
{
    int x [ 10 ] , y [ 10 ] ;
    x = y;
}
)";

    checkTypes(
        s,
        Expectation()
            .diagnostic(
                Expectation::ErrorOrWarn::Error,
                TypeChecker::DiagnosticsReporter::ID_of_CannotAssignToExpressionOfArrayType));
}

void TypeCheckerTester::case0729()
{
    auto s = R"(
void _ ()
{
    int x [ 10 ] , y [ 10 ] ;
    y = x ;
}
)";

    checkTypes(
        s,
        Expectation()
            .diagnostic(
                Expectation::ErrorOrWarn::Error,
                TypeChecker::DiagnosticsReporter::ID_of_CannotAssignToExpressionOfArrayType));
}

void TypeCheckerTester::case0730()
{
    auto s = R"(
struct s { const int m ; } ;
void _ ()
{
    struct s x , y ;
    y = x ;
}
)";

    checkTypes(
        s,
        Expectation()
            .diagnostic(
                Expectation::ErrorOrWarn::Error,
                TypeChecker::DiagnosticsReporter::ID_of_CannotAssignToExpressionOfConstQualifiedType));
}

void TypeCheckerTester::case0731()
{
    auto s = R"(
typedef struct a {
    struct b {
        struct c {
            const int m ;
        } xx ;
    } yy;
} t;
void _ ()
{
    t x = { { { 42 } } };
    t y ;
    y = x ;
}
)";

    checkTypes(
        s,
        Expectation()
            .diagnostic(
                Expectation::ErrorOrWarn::Error,
                TypeChecker::DiagnosticsReporter::ID_of_CannotAssignToExpressionOfConstQualifiedType));
}

void TypeCheckerTester::case0732()
{
    auto s = R"(
struct s {
    struct {
        struct {
            const int m ;
        } ;
    } ;
} ;
void _ ()
{
    struct s x = { { { 42 } } };
    struct s y ;
    y = x ;
}
)";

    checkTypes(
        s,
        Expectation()
            .diagnostic(
                Expectation::ErrorOrWarn::Error,
                TypeChecker::DiagnosticsReporter::ID_of_CannotAssignToExpressionOfConstQualifiedType));
}

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
