// Copyright (c) 2020/21/22 Leandro T. C. Melo <ltcmelo@gmail.com>
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

#include "ReparserTester.h"

using namespace psy;
using namespace C;

const std::string ReparserTester::Name = "REPARSER";

void ReparserTester::testReparser()
{
    return run<ReparserTester>(tests_);
}

void ReparserTester::reparse_withSyntaxCorrelation(std::string text, Expectation X)
{
    (static_cast<InternalsTestSuite*>(suite_)->reparse_withSyntaxCorrelation(text, X));
}

void ReparserTester::reparse_withTypeSynonymVerification(std::string text, Expectation X)
{
    (static_cast<InternalsTestSuite*>(suite_)->reparse_withTypeSynonymVerification(text, X));
}

void ReparserTester::reparse_withGuidelineImposition(std::string text, Expectation X)
{
    (static_cast<InternalsTestSuite*>(suite_)->reparse_withGuidelineImposition(text, X));
}

static std::vector<SyntaxKind> body(std::vector<SyntaxKind>&& v)
{
    std::vector<SyntaxKind> w = { TranslationUnit,
                                  FunctionDefinition,
                                  BuiltinTypeSpecifier,
                                  FunctionDeclarator,
                                  IdentifierDeclarator,
                                  ParameterSuffix,
                                  CompoundStatement };
    w.insert(w.end(), v.begin(), v.end());
    return w;
}

void ReparserTester::case0001()
{
    auto s = R"(
int _ ( )
{
    x z ;
    x * y ;
}
)";

    reparse_withSyntaxCorrelation(
                s,
                Expectation().AST(body({ DeclarationStatement,
                                         VariableAndOrFunctionDeclaration,
                                         TypedefName,
                                         IdentifierDeclarator,
                                         DeclarationStatement,
                                         VariableAndOrFunctionDeclaration,
                                         TypedefName,
                                         PointerDeclarator,
                                         IdentifierDeclarator })));
}

void ReparserTester::case0002()
{
    auto s = R"(
int _ ( )
{
    x * y ;
    x z ;
}
)";

    reparse_withSyntaxCorrelation(
                s,
                Expectation().AST(body({ DeclarationStatement,
                                         VariableAndOrFunctionDeclaration,
                                         TypedefName,
                                         PointerDeclarator,
                                         IdentifierDeclarator,
                                         DeclarationStatement,
                                         VariableAndOrFunctionDeclaration,
                                         TypedefName,
                                         IdentifierDeclarator })));
}

void ReparserTester::case0003()
{
    auto s = R"(
int _ ( )
{
    x * y ;
    x + y ;
}
)";

    reparse_withSyntaxCorrelation(
                s,
                Expectation().AST(body({ ExpressionStatement,
                                         MultiplyExpression,
                                         IdentifierName,
                                         IdentifierName,
                                         ExpressionStatement,
                                         AddExpression,
                                         IdentifierName,
                                         IdentifierName })));
}

void ReparserTester::case0004()
{
    auto s = R"(
int _ ( )
{
    x + y ;
    x * y ;
}
)";

    reparse_withSyntaxCorrelation(
                s,
                Expectation().AST(body({ ExpressionStatement,
                                         AddExpression,
                                         IdentifierName,
                                         IdentifierName,
                                         ExpressionStatement,
                                         MultiplyExpression,
                                         IdentifierName,
                                         IdentifierName })));
}

void ReparserTester::case0005(){}
void ReparserTester::case0006(){}
void ReparserTester::case0007(){}
void ReparserTester::case0008(){}
void ReparserTester::case0009(){}
void ReparserTester::case0010(){}
void ReparserTester::case0011(){}
void ReparserTester::case0012(){}
void ReparserTester::case0013(){}
void ReparserTester::case0014(){}
void ReparserTester::case0015(){}
void ReparserTester::case0016(){}
void ReparserTester::case0017(){}
void ReparserTester::case0018(){}
void ReparserTester::case0019(){}
void ReparserTester::case0020(){}
void ReparserTester::case0021(){}
void ReparserTester::case0022(){}
void ReparserTester::case0023(){}
void ReparserTester::case0024(){}
void ReparserTester::case0025(){}
void ReparserTester::case0026(){}
void ReparserTester::case0027(){}
void ReparserTester::case0028(){}
void ReparserTester::case0029(){}
void ReparserTester::case0030(){}
void ReparserTester::case0031(){}
void ReparserTester::case0032(){}
void ReparserTester::case0033(){}
void ReparserTester::case0034(){}
void ReparserTester::case0035(){}
void ReparserTester::case0036(){}
void ReparserTester::case0037(){}
void ReparserTester::case0038(){}
void ReparserTester::case0039(){}
void ReparserTester::case0040(){}
void ReparserTester::case0041(){}
void ReparserTester::case0042(){}
void ReparserTester::case0043(){}
void ReparserTester::case0044(){}
void ReparserTester::case0045(){}
void ReparserTester::case0046(){}
void ReparserTester::case0047(){}
void ReparserTester::case0048(){}
void ReparserTester::case0049(){}
void ReparserTester::case0050(){}
void ReparserTester::case0051(){}
void ReparserTester::case0052(){}
void ReparserTester::case0053(){}
void ReparserTester::case0054(){}
void ReparserTester::case0055(){}
void ReparserTester::case0056(){}
void ReparserTester::case0057(){}
void ReparserTester::case0058(){}
void ReparserTester::case0059(){}
void ReparserTester::case0060(){}
void ReparserTester::case0061(){}
void ReparserTester::case0062(){}
void ReparserTester::case0063(){}
void ReparserTester::case0064(){}
void ReparserTester::case0065(){}
void ReparserTester::case0066(){}
void ReparserTester::case0067(){}
void ReparserTester::case0068(){}
void ReparserTester::case0069(){}
void ReparserTester::case0070(){}
void ReparserTester::case0071(){}
void ReparserTester::case0072(){}
void ReparserTester::case0073(){}
void ReparserTester::case0074(){}
void ReparserTester::case0075(){}
void ReparserTester::case0076(){}
void ReparserTester::case0077(){}
void ReparserTester::case0078(){}
void ReparserTester::case0079(){}
void ReparserTester::case0080(){}
void ReparserTester::case0081(){}
void ReparserTester::case0082(){}
void ReparserTester::case0083(){}
void ReparserTester::case0084(){}
void ReparserTester::case0085(){}
void ReparserTester::case0086(){}
void ReparserTester::case0087(){}
void ReparserTester::case0088(){}
void ReparserTester::case0089(){}
void ReparserTester::case0090(){}
void ReparserTester::case0091(){}
void ReparserTester::case0092(){}
void ReparserTester::case0093(){}
void ReparserTester::case0094(){}
void ReparserTester::case0095(){}
void ReparserTester::case0096(){}
void ReparserTester::case0097(){}
void ReparserTester::case0098(){}
void ReparserTester::case0099(){}

void ReparserTester::case0100(){}
void ReparserTester::case0101(){}
void ReparserTester::case0102(){}
void ReparserTester::case0103(){}
void ReparserTester::case0104(){}
void ReparserTester::case0105(){}
void ReparserTester::case0106(){}
void ReparserTester::case0107(){}
void ReparserTester::case0108(){}
void ReparserTester::case0109(){}
void ReparserTester::case0110(){}
void ReparserTester::case0111(){}
void ReparserTester::case0112(){}
void ReparserTester::case0113(){}
void ReparserTester::case0114(){}
void ReparserTester::case0115(){}
void ReparserTester::case0116(){}
void ReparserTester::case0117(){}
void ReparserTester::case0118(){}
void ReparserTester::case0119(){}
void ReparserTester::case0120(){}
void ReparserTester::case0121(){}
void ReparserTester::case0122(){}
void ReparserTester::case0123(){}
void ReparserTester::case0124(){}
void ReparserTester::case0125(){}
void ReparserTester::case0126(){}
void ReparserTester::case0127(){}
void ReparserTester::case0128(){}
void ReparserTester::case0129(){}
void ReparserTester::case0130(){}
void ReparserTester::case0131(){}
void ReparserTester::case0132(){}
void ReparserTester::case0133(){}
void ReparserTester::case0134(){}
void ReparserTester::case0135(){}
void ReparserTester::case0136(){}
void ReparserTester::case0137(){}
void ReparserTester::case0138(){}
void ReparserTester::case0139(){}
void ReparserTester::case0140(){}
void ReparserTester::case0141(){}
void ReparserTester::case0142(){}
void ReparserTester::case0143(){}
void ReparserTester::case0144(){}
void ReparserTester::case0145(){}
void ReparserTester::case0146(){}
void ReparserTester::case0147(){}
void ReparserTester::case0148(){}
void ReparserTester::case0149(){}
void ReparserTester::case0150(){}
void ReparserTester::case0151(){}
void ReparserTester::case0152(){}
void ReparserTester::case0153(){}
void ReparserTester::case0154(){}
void ReparserTester::case0155(){}
void ReparserTester::case0156(){}
void ReparserTester::case0157(){}
void ReparserTester::case0158(){}
void ReparserTester::case0159(){}
void ReparserTester::case0160(){}
void ReparserTester::case0161(){}
void ReparserTester::case0162(){}
void ReparserTester::case0163(){}
void ReparserTester::case0164(){}
void ReparserTester::case0165(){}
void ReparserTester::case0166(){}
void ReparserTester::case0167(){}
void ReparserTester::case0168(){}
void ReparserTester::case0169(){}
void ReparserTester::case0170(){}
void ReparserTester::case0171(){}
void ReparserTester::case0172(){}
void ReparserTester::case0173(){}
void ReparserTester::case0174(){}
void ReparserTester::case0175(){}
void ReparserTester::case0176(){}
void ReparserTester::case0177(){}
void ReparserTester::case0178(){}
void ReparserTester::case0179(){}
void ReparserTester::case0180(){}
void ReparserTester::case0181(){}
void ReparserTester::case0182(){}
void ReparserTester::case0183(){}
void ReparserTester::case0184(){}
void ReparserTester::case0185(){}
void ReparserTester::case0186(){}
void ReparserTester::case0187(){}
void ReparserTester::case0188(){}
void ReparserTester::case0189(){}
void ReparserTester::case0190(){}
void ReparserTester::case0191(){}
void ReparserTester::case0192(){}
void ReparserTester::case0193(){}
void ReparserTester::case0194(){}
void ReparserTester::case0195(){}
void ReparserTester::case0196(){}
void ReparserTester::case0197(){}
void ReparserTester::case0198(){}
void ReparserTester::case0199(){}

void ReparserTester::case0200(){}
void ReparserTester::case0201(){}
void ReparserTester::case0202(){}
void ReparserTester::case0203(){}
void ReparserTester::case0204(){}
void ReparserTester::case0205(){}
void ReparserTester::case0206(){}
void ReparserTester::case0207(){}
void ReparserTester::case0208(){}
void ReparserTester::case0209(){}
void ReparserTester::case0210(){}
void ReparserTester::case0211(){}
void ReparserTester::case0212(){}
void ReparserTester::case0213(){}
void ReparserTester::case0214(){}
void ReparserTester::case0215(){}
void ReparserTester::case0216(){}
void ReparserTester::case0217(){}
void ReparserTester::case0218(){}
void ReparserTester::case0219(){}
void ReparserTester::case0220(){}
void ReparserTester::case0221(){}
void ReparserTester::case0222(){}
void ReparserTester::case0223(){}
void ReparserTester::case0224(){}
void ReparserTester::case0225(){}
void ReparserTester::case0226(){}
void ReparserTester::case0227(){}
void ReparserTester::case0228(){}
void ReparserTester::case0229(){}
void ReparserTester::case0230(){}
void ReparserTester::case0231(){}
void ReparserTester::case0232(){}
void ReparserTester::case0233(){}
void ReparserTester::case0234(){}
void ReparserTester::case0235(){}
void ReparserTester::case0236(){}
void ReparserTester::case0237(){}
void ReparserTester::case0238(){}
void ReparserTester::case0239(){}
void ReparserTester::case0240(){}
void ReparserTester::case0241(){}
void ReparserTester::case0242(){}
void ReparserTester::case0243(){}
void ReparserTester::case0244(){}
void ReparserTester::case0245(){}
void ReparserTester::case0246(){}
void ReparserTester::case0247(){}
void ReparserTester::case0248(){}
void ReparserTester::case0249(){}
void ReparserTester::case0250(){}
void ReparserTester::case0251(){}
void ReparserTester::case0252(){}
void ReparserTester::case0253(){}
void ReparserTester::case0254(){}
void ReparserTester::case0255(){}
void ReparserTester::case0256(){}
void ReparserTester::case0257(){}
void ReparserTester::case0258(){}
void ReparserTester::case0259(){}
void ReparserTester::case0260(){}
void ReparserTester::case0261(){}
void ReparserTester::case0262(){}
void ReparserTester::case0263(){}
void ReparserTester::case0264(){}
void ReparserTester::case0265(){}
void ReparserTester::case0266(){}
void ReparserTester::case0267(){}
void ReparserTester::case0268(){}
void ReparserTester::case0269(){}
void ReparserTester::case0270(){}
void ReparserTester::case0271(){}
void ReparserTester::case0272(){}
void ReparserTester::case0273(){}
void ReparserTester::case0274(){}
void ReparserTester::case0275(){}
void ReparserTester::case0276(){}
void ReparserTester::case0277(){}
void ReparserTester::case0278(){}
void ReparserTester::case0279(){}
void ReparserTester::case0280(){}
void ReparserTester::case0281(){}
void ReparserTester::case0282(){}
void ReparserTester::case0283(){}
void ReparserTester::case0284(){}
void ReparserTester::case0285(){}
void ReparserTester::case0286(){}
void ReparserTester::case0287(){}
void ReparserTester::case0288(){}
void ReparserTester::case0289(){}
void ReparserTester::case0290(){}
void ReparserTester::case0291(){}
void ReparserTester::case0292(){}
void ReparserTester::case0293(){}
void ReparserTester::case0294(){}
void ReparserTester::case0295(){}
void ReparserTester::case0296(){}
void ReparserTester::case0297(){}
void ReparserTester::case0298(){}
void ReparserTester::case0299(){}
