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

#include "syntax/Lexeme_ALL.h"
#include "parser/Unparser.h"

#include "syntax/SyntaxNodes.h"

#include <algorithm>
#include <cstring>
#include <iostream>
#include <string>
#include <sstream>

using namespace psy;
using namespace C;

void DeclarationBinderTester::case3001()
{
    bind("struct x { int y ; } ;",
         Expectation()
         .declaration(Decl()
                  .Type("x", SymbolKind::StructDeclaration)
                  .inNameSpace(NameSpace::Tags)
                  .withScopeKind(ScopeKind::File)));
}

void DeclarationBinderTester::case3002()
{
    bind("union x { int y ; } ;",
         Expectation()
         .declaration(Decl()
                  .Type("x", SymbolKind::UnionDeclaration)
                  .inNameSpace(NameSpace::Tags)
                  .withScopeKind(ScopeKind::File)));
}

void DeclarationBinderTester::case3003(){}
void DeclarationBinderTester::case3004(){}
void DeclarationBinderTester::case3005(){}
void DeclarationBinderTester::case3006(){}
void DeclarationBinderTester::case3007(){}
void DeclarationBinderTester::case3008(){}
void DeclarationBinderTester::case3009(){}
void DeclarationBinderTester::case3010(){}
void DeclarationBinderTester::case3011(){}
void DeclarationBinderTester::case3012(){}
void DeclarationBinderTester::case3013(){}
void DeclarationBinderTester::case3014(){}
void DeclarationBinderTester::case3015(){}
void DeclarationBinderTester::case3016(){}
void DeclarationBinderTester::case3017(){}
void DeclarationBinderTester::case3018(){}
void DeclarationBinderTester::case3019(){}
void DeclarationBinderTester::case3020(){}
void DeclarationBinderTester::case3021(){}
void DeclarationBinderTester::case3022(){}
void DeclarationBinderTester::case3023(){}
void DeclarationBinderTester::case3024(){}
void DeclarationBinderTester::case3025(){}
void DeclarationBinderTester::case3026(){}
void DeclarationBinderTester::case3027(){}
void DeclarationBinderTester::case3028(){}
void DeclarationBinderTester::case3029(){}
void DeclarationBinderTester::case3030(){}
void DeclarationBinderTester::case3031(){}
void DeclarationBinderTester::case3032(){}
void DeclarationBinderTester::case3033(){}
void DeclarationBinderTester::case3034(){}
void DeclarationBinderTester::case3035(){}
void DeclarationBinderTester::case3036(){}
void DeclarationBinderTester::case3037(){}
void DeclarationBinderTester::case3038(){}
void DeclarationBinderTester::case3039(){}
void DeclarationBinderTester::case3040(){}
void DeclarationBinderTester::case3041(){}
void DeclarationBinderTester::case3042(){}
void DeclarationBinderTester::case3043(){}
void DeclarationBinderTester::case3044(){}
void DeclarationBinderTester::case3045(){}
void DeclarationBinderTester::case3046(){}
void DeclarationBinderTester::case3047(){}
void DeclarationBinderTester::case3048(){}
void DeclarationBinderTester::case3049(){}

void DeclarationBinderTester::case3050()
{
    bind("enum x { y } ;",
         Expectation()
         .declaration(Decl()
                  .Type("x", SymbolKind::EnumDeclaration)
                  .inNameSpace(NameSpace::Tags)
                  .withScopeKind(ScopeKind::File))
         .declaration(Decl()
                  .Member("y", SymbolKind::EnumeratorDeclaration)
                  .ty_.Basic(BasicTypeKind::Int_S)));
}

void DeclarationBinderTester::case3051()
{
    bind("enum x { y , z } ;",
         Expectation()
         .declaration(Decl()
                  .Type("x", SymbolKind::EnumDeclaration)
                  .inNameSpace(NameSpace::Tags)
                  .withScopeKind(ScopeKind::File))
         .declaration(Decl()
                  .Member("y", SymbolKind::EnumeratorDeclaration)
                  .ty_.Basic(BasicTypeKind::Int_S))
         .declaration(Decl()
                  .Member("z", SymbolKind::EnumeratorDeclaration)
                  .ty_.Basic(BasicTypeKind::Int_S)));
}

void DeclarationBinderTester::case3052()
{
    bind("enum x { y = 0 , z } ;",
         Expectation()
         .declaration(Decl()
                  .Type("x", SymbolKind::EnumDeclaration)
                  .inNameSpace(NameSpace::Tags)
                  .withScopeKind(ScopeKind::File))
         .declaration(Decl()
                  .Member("y", SymbolKind::EnumeratorDeclaration)
                  .ty_.Basic(BasicTypeKind::Int_S))
         .declaration(Decl()
                  .Member("z", SymbolKind::EnumeratorDeclaration)
                  .ty_.Basic(BasicTypeKind::Int_S)));
}

void DeclarationBinderTester::case3053()
{
    bind("enum x { y , z , w } ;",
         Expectation()
         .declaration(Decl()
                  .Type("x", SymbolKind::EnumDeclaration)
                  .inNameSpace(NameSpace::Tags)
                  .withScopeKind(ScopeKind::File))
         .declaration(Decl()
                  .Member("y", SymbolKind::EnumeratorDeclaration)
                  .ty_.Basic(BasicTypeKind::Int_S))
         .declaration(Decl()
                  .Member("z", SymbolKind::EnumeratorDeclaration)
                  .ty_.Basic(BasicTypeKind::Int_S))
         .declaration(Decl()
                  .Member("w", SymbolKind::EnumeratorDeclaration)
                  .ty_.Basic(BasicTypeKind::Int_S)));
}

void DeclarationBinderTester::case3054()
{
    bind("enum x { y , z = 0 , w } ;",
         Expectation()
         .declaration(Decl()
                  .Type("x", SymbolKind::EnumDeclaration)
                  .inNameSpace(NameSpace::Tags)
                  .withScopeKind(ScopeKind::File))
         .declaration(Decl()
                  .Member("y", SymbolKind::EnumeratorDeclaration)
                  .ty_.Basic(BasicTypeKind::Int_S))
         .declaration(Decl()
                  .Member("z", SymbolKind::EnumeratorDeclaration)
                  .ty_.Basic(BasicTypeKind::Int_S))
         .declaration(Decl()
                  .Member("w", SymbolKind::EnumeratorDeclaration)
                  .ty_.Basic(BasicTypeKind::Int_S)));
}

void DeclarationBinderTester::case3055(){}
void DeclarationBinderTester::case3056(){}
void DeclarationBinderTester::case3057(){}
void DeclarationBinderTester::case3058(){}
void DeclarationBinderTester::case3059(){}
void DeclarationBinderTester::case3060(){}
void DeclarationBinderTester::case3061(){}
void DeclarationBinderTester::case3062(){}
void DeclarationBinderTester::case3063(){}
void DeclarationBinderTester::case3064(){}
void DeclarationBinderTester::case3065(){}
void DeclarationBinderTester::case3066(){}
void DeclarationBinderTester::case3067(){}
void DeclarationBinderTester::case3068(){}
void DeclarationBinderTester::case3069(){}
void DeclarationBinderTester::case3070(){}
void DeclarationBinderTester::case3071(){}
void DeclarationBinderTester::case3072(){}
void DeclarationBinderTester::case3073(){}
void DeclarationBinderTester::case3074(){}
void DeclarationBinderTester::case3075(){}
void DeclarationBinderTester::case3076(){}
void DeclarationBinderTester::case3077(){}
void DeclarationBinderTester::case3078(){}
void DeclarationBinderTester::case3079(){}
void DeclarationBinderTester::case3080(){}
void DeclarationBinderTester::case3081(){}
void DeclarationBinderTester::case3082(){}
void DeclarationBinderTester::case3083(){}
void DeclarationBinderTester::case3084(){}
void DeclarationBinderTester::case3085(){}
void DeclarationBinderTester::case3086(){}
void DeclarationBinderTester::case3087(){}
void DeclarationBinderTester::case3088(){}
void DeclarationBinderTester::case3089(){}
void DeclarationBinderTester::case3090(){}
void DeclarationBinderTester::case3091(){}
void DeclarationBinderTester::case3092(){}
void DeclarationBinderTester::case3093(){}
void DeclarationBinderTester::case3094(){}
void DeclarationBinderTester::case3095(){}
void DeclarationBinderTester::case3096(){}
void DeclarationBinderTester::case3097(){}
void DeclarationBinderTester::case3098(){}
void DeclarationBinderTester::case3099(){}

void DeclarationBinderTester::case3100()
{
    bind("typedef int x ;",
         Expectation()
             .declaration(Decl()
                          .Type("x")
                          .inNameSpace(NameSpace::OrdinaryIdentifiers)
                          .withScopeKind(ScopeKind::File)
                          .ty_.Basic(BasicTypeKind::Int_S)));
}

void DeclarationBinderTester::case3101()
{
    bind("typedef int * x ;",
         Expectation()
             .declaration(Decl()
                          .Type("x")
                          .inNameSpace(NameSpace::OrdinaryIdentifiers)
                          .withScopeKind(ScopeKind::File)
                          .ty_.Basic(BasicTypeKind::Int_S)
                          .ty_.Derived(TypeKind::Pointer)));
}

void DeclarationBinderTester::case3102()
{
    bind("typedef int ( * x ) ( ) ;",
         Expectation()
             .declaration(Decl()
                          .Type("x")
                          .inNameSpace(NameSpace::OrdinaryIdentifiers)
                          .withScopeKind(ScopeKind::File)
                          .ty_.Basic(BasicTypeKind::Int_S)
                          .ty_.Derived(TypeKind::Function)
                          .ty_.Derived(TypeKind::Pointer)));
}

void DeclarationBinderTester::case3103()
{
    bind("typedef int x , y ;",
         Expectation()
             .declaration(Decl()
                          .Type("x")
                          .inNameSpace(NameSpace::OrdinaryIdentifiers)
                          .withScopeKind(ScopeKind::File)
                          .ty_.Basic(BasicTypeKind::Int_S))
             .declaration(Decl()
                          .Type("y")
                          .inNameSpace(NameSpace::OrdinaryIdentifiers)
                          .withScopeKind(ScopeKind::File)
                          .ty_.Basic(BasicTypeKind::Int_S)));
}

void DeclarationBinderTester::case3104()
{
    bind("typedef int * x , y ;",
         Expectation()
             .declaration(Decl()
                          .Type("x")
                          .inNameSpace(NameSpace::OrdinaryIdentifiers)
                          .withScopeKind(ScopeKind::File)
                          .ty_.Basic(BasicTypeKind::Int_S)
                          .ty_.Derived(TypeKind::Pointer))
             .declaration(Decl()
                          .Type("y")
                          .inNameSpace(NameSpace::OrdinaryIdentifiers)
                          .withScopeKind(ScopeKind::File)
                          .ty_.Basic(BasicTypeKind::Int_S)));
}

void DeclarationBinderTester::case3105()
{
    bind("typedef x y ;",
         Expectation()
             .declaration(Decl()
                          .Type("y")
                          .inNameSpace(NameSpace::OrdinaryIdentifiers)
                          .withScopeKind(ScopeKind::File)
                          .ty_.Typedef("x")));
}

void DeclarationBinderTester::case3106()
{
    bind("typedef const int x ;",
         Expectation()
             .declaration(Decl()
                          .Type("x")
                          .inNameSpace(NameSpace::OrdinaryIdentifiers)
                          .withScopeKind(ScopeKind::File)
                          .ty_.Basic(BasicTypeKind::Int_S, CVR::Const)));
}

void DeclarationBinderTester::case3107()
{
    bind("typedef int const x ;",
         Expectation()
             .declaration(Decl()
                          .Type("x")
                          .inNameSpace(NameSpace::OrdinaryIdentifiers)
                          .withScopeKind(ScopeKind::File)
                          .ty_.Basic(BasicTypeKind::Int_S, CVR::Const)));
}

void DeclarationBinderTester::case3108()
{
    bind("typedef int * x , * y ;",
         Expectation()
             .declaration(Decl()
                          .Type("x")
                          .inNameSpace(NameSpace::OrdinaryIdentifiers)
                          .withScopeKind(ScopeKind::File)
                          .ty_.Basic(BasicTypeKind::Int_S)
                          .ty_.Derived(TypeKind::Pointer))
             .declaration(Decl()
                          .Type("y")
                          .inNameSpace(NameSpace::OrdinaryIdentifiers)
                          .withScopeKind(ScopeKind::File)
                          .ty_.Basic(BasicTypeKind::Int_S)
                          .ty_.Derived(TypeKind::Pointer)));
}

void DeclarationBinderTester::case3109()
{
    bind("typedef int x , * y ;",
         Expectation()
             .declaration(Decl()
                          .Type("x")
                          .inNameSpace(NameSpace::OrdinaryIdentifiers)
                          .withScopeKind(ScopeKind::File)
                          .ty_.Basic(BasicTypeKind::Int_S))
             .declaration(Decl()
                          .Type("y")
                          .inNameSpace(NameSpace::OrdinaryIdentifiers)
                          .withScopeKind(ScopeKind::File)
                          .ty_.Basic(BasicTypeKind::Int_S)
                          .ty_.Derived(TypeKind::Pointer)));
}

void DeclarationBinderTester::case3110()
{
    bind("typedef const int x , y ;",
         Expectation()
             .declaration(Decl()
                          .Type("x")
                          .inNameSpace(NameSpace::OrdinaryIdentifiers)
                          .withScopeKind(ScopeKind::File)
                          .ty_.Basic(BasicTypeKind::Int_S, CVR::Const))
             .declaration(Decl()
                          .Type("x")
                          .inNameSpace(NameSpace::OrdinaryIdentifiers)
                          .withScopeKind(ScopeKind::File)
                          .ty_.Basic(BasicTypeKind::Int_S, CVR::Const)));
}

void DeclarationBinderTester::case3111()
{
    bind("typedef int ( * x ) ( double ) ;",
         Expectation()
             .declaration(Decl()
                          .Type("x")
                          .inNameSpace(NameSpace::OrdinaryIdentifiers)
                          .withScopeKind(ScopeKind::File)
                          .ty_.Basic(BasicTypeKind::Int_S)
                          .ty_.Derived(TypeKind::Function)
                          .ty_.addParam().Basic(BasicTypeKind::Double)
                          .ty_.Derived(TypeKind::Pointer)));
}

void DeclarationBinderTester::case3112()
{
    bind("typedef int ( * x ) ( double p ) ;",
         Expectation()
             .declaration(Decl()
                          .Type("x")
                          .inNameSpace(NameSpace::OrdinaryIdentifiers)
                          .withScopeKind(ScopeKind::File)
                          .ty_.Basic(BasicTypeKind::Int_S)
                          .ty_.Derived(TypeKind::Function)
                          .ty_.addParam().Basic(BasicTypeKind::Double)
                          .ty_.Derived(TypeKind::Pointer)));
}

void DeclarationBinderTester::case3113()
{
    bind("typedef int ( * x ) ( double ) , ( * y ) ( double ) ;",
         Expectation()
             .declaration(Decl()
                          .Type("x")
                          .inNameSpace(NameSpace::OrdinaryIdentifiers)
                          .withScopeKind(ScopeKind::File)
                          .ty_.Basic(BasicTypeKind::Int_S)
                          .ty_.Derived(TypeKind::Function)
                          .ty_.addParam().Basic(BasicTypeKind::Double)
                          .ty_.Derived(TypeKind::Pointer))
         .declaration(Decl()
                      .Type("y")
                      .inNameSpace(NameSpace::OrdinaryIdentifiers)
                      .withScopeKind(ScopeKind::File)
                      .ty_.Basic(BasicTypeKind::Int_S)
                      .ty_.Derived(TypeKind::Function)
                      .ty_.addParam().Basic(BasicTypeKind::Double)
                      .ty_.Derived(TypeKind::Pointer)));
}

void DeclarationBinderTester::case3114(){}
void DeclarationBinderTester::case3115(){}
void DeclarationBinderTester::case3116(){}
void DeclarationBinderTester::case3117(){}
void DeclarationBinderTester::case3118(){}
void DeclarationBinderTester::case3119(){}
void DeclarationBinderTester::case3120(){}
void DeclarationBinderTester::case3121(){}
void DeclarationBinderTester::case3122(){}
void DeclarationBinderTester::case3123(){}
void DeclarationBinderTester::case3124(){}
void DeclarationBinderTester::case3125(){}
void DeclarationBinderTester::case3126(){}
void DeclarationBinderTester::case3127(){}
void DeclarationBinderTester::case3128(){}
void DeclarationBinderTester::case3129(){}
void DeclarationBinderTester::case3130(){}
void DeclarationBinderTester::case3131(){}
void DeclarationBinderTester::case3132(){}
void DeclarationBinderTester::case3133(){}
void DeclarationBinderTester::case3134(){}
void DeclarationBinderTester::case3135(){}
void DeclarationBinderTester::case3136(){}
void DeclarationBinderTester::case3137(){}
void DeclarationBinderTester::case3138(){}
void DeclarationBinderTester::case3139(){}
void DeclarationBinderTester::case3140(){}
void DeclarationBinderTester::case3141(){}
void DeclarationBinderTester::case3142(){}
void DeclarationBinderTester::case3143(){}
void DeclarationBinderTester::case3144(){}
void DeclarationBinderTester::case3145(){}
void DeclarationBinderTester::case3146(){}
void DeclarationBinderTester::case3147(){}
void DeclarationBinderTester::case3148(){}
void DeclarationBinderTester::case3149(){}
void DeclarationBinderTester::case3150(){}
void DeclarationBinderTester::case3151(){}
void DeclarationBinderTester::case3152(){}
void DeclarationBinderTester::case3153(){}
void DeclarationBinderTester::case3154(){}
void DeclarationBinderTester::case3155(){}
void DeclarationBinderTester::case3156(){}
void DeclarationBinderTester::case3157(){}
void DeclarationBinderTester::case3158(){}
void DeclarationBinderTester::case3159(){}
void DeclarationBinderTester::case3160(){}
void DeclarationBinderTester::case3161(){}
void DeclarationBinderTester::case3162(){}
void DeclarationBinderTester::case3163(){}
void DeclarationBinderTester::case3164(){}
void DeclarationBinderTester::case3165(){}
void DeclarationBinderTester::case3166(){}
void DeclarationBinderTester::case3167(){}
void DeclarationBinderTester::case3168(){}
void DeclarationBinderTester::case3169(){}
void DeclarationBinderTester::case3170(){}
void DeclarationBinderTester::case3171(){}
void DeclarationBinderTester::case3172(){}
void DeclarationBinderTester::case3173(){}
void DeclarationBinderTester::case3174(){}
void DeclarationBinderTester::case3175(){}
void DeclarationBinderTester::case3176(){}
void DeclarationBinderTester::case3177(){}
void DeclarationBinderTester::case3178(){}
void DeclarationBinderTester::case3179(){}
void DeclarationBinderTester::case3180(){}
void DeclarationBinderTester::case3181(){}
void DeclarationBinderTester::case3182(){}
void DeclarationBinderTester::case3183(){}
void DeclarationBinderTester::case3184(){}
void DeclarationBinderTester::case3185(){}
void DeclarationBinderTester::case3186(){}
void DeclarationBinderTester::case3187(){}
void DeclarationBinderTester::case3188(){}
void DeclarationBinderTester::case3189(){}
void DeclarationBinderTester::case3190(){}
void DeclarationBinderTester::case3191(){}
void DeclarationBinderTester::case3192(){}
void DeclarationBinderTester::case3193(){}
void DeclarationBinderTester::case3194(){}
void DeclarationBinderTester::case3195(){}
void DeclarationBinderTester::case3196(){}
void DeclarationBinderTester::case3197(){}
void DeclarationBinderTester::case3198(){}
void DeclarationBinderTester::case3199(){}

void DeclarationBinderTester::case3200(){}
void DeclarationBinderTester::case3201(){}
void DeclarationBinderTester::case3202(){}
void DeclarationBinderTester::case3203(){}
void DeclarationBinderTester::case3204(){}
void DeclarationBinderTester::case3205(){}
void DeclarationBinderTester::case3206(){}
void DeclarationBinderTester::case3207(){}
void DeclarationBinderTester::case3208(){}
void DeclarationBinderTester::case3209(){}
void DeclarationBinderTester::case3210(){}
void DeclarationBinderTester::case3211(){}
void DeclarationBinderTester::case3212(){}
void DeclarationBinderTester::case3213(){}
void DeclarationBinderTester::case3214(){}
void DeclarationBinderTester::case3215(){}
void DeclarationBinderTester::case3216(){}
void DeclarationBinderTester::case3217(){}
void DeclarationBinderTester::case3218(){}
void DeclarationBinderTester::case3219(){}
void DeclarationBinderTester::case3220(){}
void DeclarationBinderTester::case3221(){}
void DeclarationBinderTester::case3222(){}
void DeclarationBinderTester::case3223(){}
void DeclarationBinderTester::case3224(){}
void DeclarationBinderTester::case3225(){}
void DeclarationBinderTester::case3226(){}
void DeclarationBinderTester::case3227(){}
void DeclarationBinderTester::case3228(){}
void DeclarationBinderTester::case3229(){}
void DeclarationBinderTester::case3230(){}
void DeclarationBinderTester::case3231(){}
void DeclarationBinderTester::case3232(){}
void DeclarationBinderTester::case3233(){}
void DeclarationBinderTester::case3234(){}
void DeclarationBinderTester::case3235(){}
void DeclarationBinderTester::case3236(){}
void DeclarationBinderTester::case3237(){}
void DeclarationBinderTester::case3238(){}
void DeclarationBinderTester::case3239(){}
void DeclarationBinderTester::case3240(){}
void DeclarationBinderTester::case3241(){}
void DeclarationBinderTester::case3242(){}
void DeclarationBinderTester::case3243(){}
void DeclarationBinderTester::case3244(){}
void DeclarationBinderTester::case3245(){}
void DeclarationBinderTester::case3246(){}
void DeclarationBinderTester::case3247(){}
void DeclarationBinderTester::case3248(){}
void DeclarationBinderTester::case3249(){}
void DeclarationBinderTester::case3250(){}
void DeclarationBinderTester::case3251(){}
void DeclarationBinderTester::case3252(){}
void DeclarationBinderTester::case3253(){}
void DeclarationBinderTester::case3254(){}
void DeclarationBinderTester::case3255(){}
void DeclarationBinderTester::case3256(){}
void DeclarationBinderTester::case3257(){}
void DeclarationBinderTester::case3258(){}
void DeclarationBinderTester::case3259(){}
void DeclarationBinderTester::case3260(){}
void DeclarationBinderTester::case3261(){}
void DeclarationBinderTester::case3262(){}
void DeclarationBinderTester::case3263(){}
void DeclarationBinderTester::case3264(){}
void DeclarationBinderTester::case3265(){}
void DeclarationBinderTester::case3266(){}
void DeclarationBinderTester::case3267(){}
void DeclarationBinderTester::case3268(){}
void DeclarationBinderTester::case3269(){}
void DeclarationBinderTester::case3270(){}
void DeclarationBinderTester::case3271(){}
void DeclarationBinderTester::case3272(){}
void DeclarationBinderTester::case3273(){}
void DeclarationBinderTester::case3274(){}
void DeclarationBinderTester::case3275(){}
void DeclarationBinderTester::case3276(){}
void DeclarationBinderTester::case3277(){}
void DeclarationBinderTester::case3278(){}
void DeclarationBinderTester::case3279(){}
void DeclarationBinderTester::case3280(){}
void DeclarationBinderTester::case3281(){}
void DeclarationBinderTester::case3282(){}
void DeclarationBinderTester::case3283(){}
void DeclarationBinderTester::case3284(){}
void DeclarationBinderTester::case3285(){}
void DeclarationBinderTester::case3286(){}
void DeclarationBinderTester::case3287(){}
void DeclarationBinderTester::case3288(){}
void DeclarationBinderTester::case3289(){}
void DeclarationBinderTester::case3290(){}
void DeclarationBinderTester::case3291(){}
void DeclarationBinderTester::case3292(){}
void DeclarationBinderTester::case3293(){}
void DeclarationBinderTester::case3294(){}
void DeclarationBinderTester::case3295(){}
void DeclarationBinderTester::case3296(){}
void DeclarationBinderTester::case3297(){}
void DeclarationBinderTester::case3298(){}
void DeclarationBinderTester::case3299(){}

void DeclarationBinderTester::case3300()
{
    bind("typedef int x ; x z ;",
         Expectation()
             .declaration(Decl()
                          .Type("x")
                          .inNameSpace(NameSpace::OrdinaryIdentifiers)
                          .withScopeKind(ScopeKind::File)
                          .ty_.Basic(BasicTypeKind::Int_S))
            .declaration(Decl()
                         .Object("z", SymbolKind::VariableDeclaration)
                         .ty_.Typedef("x")));
}

void DeclarationBinderTester::case3301()
{
    bind("typedef int * x ; x z ;",
         Expectation()
             .declaration(Decl()
                          .Type("x")
                          .inNameSpace(NameSpace::OrdinaryIdentifiers)
                          .withScopeKind(ScopeKind::File)
                          .ty_.Basic(BasicTypeKind::Int_S)
                          .ty_.Derived(TypeKind::Pointer))
              .declaration(Decl()
                           .Object("z", SymbolKind::VariableDeclaration)
                           .ty_.Typedef("x")));
}

void DeclarationBinderTester::case3302()
{
    bind(R"(
struct x
{
   short y : 1 , : 7 ;
};
int z ;
         )",
         Expectation()
         .declaration(Decl()
                      .Member("y", SymbolKind::FieldDeclaration)
                      .ty_.Basic(BasicTypeKind::Short_S))
         .declaration(Decl()
                      .Object("z", SymbolKind::VariableDeclaration)
                      .ty_.Basic(BasicTypeKind::Int_S)));
}

void DeclarationBinderTester::case3303()
{
    bind(R"(
struct x { int _ ; } ;
float y ;
    )",
         Expectation()
         .declaration(Decl()
                      .Type("x", SymbolKind::StructDeclaration)
                      .inNameSpace(NameSpace::Tags))
         .declaration(Decl()
                      .Object("y", SymbolKind::VariableDeclaration)
                      .ty_.Basic(BasicTypeKind::Float)));
}

void DeclarationBinderTester::case3304()
{
    bind(R"(
enum x { _ } ;
float y ;
    )",
         Expectation()
         .declaration(Decl()
                      .Type("x", SymbolKind::EnumDeclaration)
                      .inNameSpace(NameSpace::Tags))
         .declaration(Decl()
                      .Object("y", SymbolKind::VariableDeclaration)
                      .ty_.Basic(BasicTypeKind::Float)));
}

void DeclarationBinderTester::case3305()
{
    bind(R"(
const char * const x ;
float y ;
    )",
         Expectation()
         .declaration(Decl()
                      .Object("x", SymbolKind::VariableDeclaration)
                      .ty_.Basic(BasicTypeKind::Char, CVR::Const)
                      .ty_.Derived(TypeKind::Pointer, CVR::Const))
         .declaration(Decl()
                      .Object("y", SymbolKind::VariableDeclaration)
                      .ty_.Basic(BasicTypeKind::Float)));
}

void DeclarationBinderTester::case3306()
{
    bind(R"(
void _ ( ) {
    sizeof ( double ) ;
    int x ;
}
         )",
         Expectation()
         .declaration(Decl()
                      .Object("x", SymbolKind::VariableDeclaration, ScopeKind::Block)
                      .ty_.Basic(BasicTypeKind::Int_S)));
}

void DeclarationBinderTester::case3307(){}
void DeclarationBinderTester::case3308(){}
void DeclarationBinderTester::case3309(){}
void DeclarationBinderTester::case3310(){}
void DeclarationBinderTester::case3311(){}
void DeclarationBinderTester::case3312(){}
void DeclarationBinderTester::case3313(){}
void DeclarationBinderTester::case3314(){}
void DeclarationBinderTester::case3315(){}
void DeclarationBinderTester::case3316(){}
void DeclarationBinderTester::case3317(){}
void DeclarationBinderTester::case3318(){}
void DeclarationBinderTester::case3319(){}
void DeclarationBinderTester::case3320(){}
void DeclarationBinderTester::case3321(){}
void DeclarationBinderTester::case3322(){}
void DeclarationBinderTester::case3323(){}
void DeclarationBinderTester::case3324(){}
void DeclarationBinderTester::case3325(){}
void DeclarationBinderTester::case3326(){}
void DeclarationBinderTester::case3327(){}
void DeclarationBinderTester::case3328(){}
void DeclarationBinderTester::case3329(){}
void DeclarationBinderTester::case3330(){}
void DeclarationBinderTester::case3331(){}
void DeclarationBinderTester::case3332(){}
void DeclarationBinderTester::case3333(){}
void DeclarationBinderTester::case3334(){}
void DeclarationBinderTester::case3335(){}
void DeclarationBinderTester::case3336(){}
void DeclarationBinderTester::case3337(){}
void DeclarationBinderTester::case3338(){}
void DeclarationBinderTester::case3339(){}
void DeclarationBinderTester::case3340(){}
void DeclarationBinderTester::case3341(){}
void DeclarationBinderTester::case3342(){}
void DeclarationBinderTester::case3343(){}
void DeclarationBinderTester::case3344(){}
void DeclarationBinderTester::case3345(){}
void DeclarationBinderTester::case3346(){}
void DeclarationBinderTester::case3347(){}
void DeclarationBinderTester::case3348(){}
void DeclarationBinderTester::case3349(){}
void DeclarationBinderTester::case3350(){}
void DeclarationBinderTester::case3351(){}
void DeclarationBinderTester::case3352(){}
void DeclarationBinderTester::case3353(){}
void DeclarationBinderTester::case3354(){}
void DeclarationBinderTester::case3355(){}
void DeclarationBinderTester::case3356(){}
void DeclarationBinderTester::case3357(){}
void DeclarationBinderTester::case3358(){}
void DeclarationBinderTester::case3359(){}
void DeclarationBinderTester::case3360(){}
void DeclarationBinderTester::case3361(){}
void DeclarationBinderTester::case3362(){}
void DeclarationBinderTester::case3363(){}
void DeclarationBinderTester::case3364(){}
void DeclarationBinderTester::case3365(){}
void DeclarationBinderTester::case3366(){}
void DeclarationBinderTester::case3367(){}
void DeclarationBinderTester::case3368(){}
void DeclarationBinderTester::case3369(){}
void DeclarationBinderTester::case3370(){}
void DeclarationBinderTester::case3371(){}
void DeclarationBinderTester::case3372(){}
void DeclarationBinderTester::case3373(){}
void DeclarationBinderTester::case3374(){}
void DeclarationBinderTester::case3375(){}
void DeclarationBinderTester::case3376(){}
void DeclarationBinderTester::case3377(){}
void DeclarationBinderTester::case3378(){}
void DeclarationBinderTester::case3379(){}
void DeclarationBinderTester::case3380(){}
void DeclarationBinderTester::case3381(){}
void DeclarationBinderTester::case3382(){}
void DeclarationBinderTester::case3383(){}
void DeclarationBinderTester::case3384(){}
void DeclarationBinderTester::case3385(){}
void DeclarationBinderTester::case3386(){}
void DeclarationBinderTester::case3387(){}
void DeclarationBinderTester::case3388(){}
void DeclarationBinderTester::case3389(){}
void DeclarationBinderTester::case3390(){}
void DeclarationBinderTester::case3391(){}
void DeclarationBinderTester::case3392(){}
void DeclarationBinderTester::case3393(){}
void DeclarationBinderTester::case3394(){}
void DeclarationBinderTester::case3395(){}
void DeclarationBinderTester::case3396(){}
void DeclarationBinderTester::case3397(){}
void DeclarationBinderTester::case3398(){}
void DeclarationBinderTester::case3399(){}

void DeclarationBinderTester::case3400(){}
void DeclarationBinderTester::case3401(){}
void DeclarationBinderTester::case3402(){}
void DeclarationBinderTester::case3403(){}
void DeclarationBinderTester::case3404(){}
void DeclarationBinderTester::case3405(){}
void DeclarationBinderTester::case3406(){}
void DeclarationBinderTester::case3407(){}
void DeclarationBinderTester::case3408(){}
void DeclarationBinderTester::case3409(){}
void DeclarationBinderTester::case3410(){}
void DeclarationBinderTester::case3411(){}
void DeclarationBinderTester::case3412(){}
void DeclarationBinderTester::case3413(){}
void DeclarationBinderTester::case3414(){}
void DeclarationBinderTester::case3415(){}
void DeclarationBinderTester::case3416(){}
void DeclarationBinderTester::case3417(){}
void DeclarationBinderTester::case3418(){}
void DeclarationBinderTester::case3419(){}
void DeclarationBinderTester::case3420(){}
void DeclarationBinderTester::case3421(){}
void DeclarationBinderTester::case3422(){}
void DeclarationBinderTester::case3423(){}
void DeclarationBinderTester::case3424(){}
void DeclarationBinderTester::case3425(){}
void DeclarationBinderTester::case3426(){}
void DeclarationBinderTester::case3427(){}
void DeclarationBinderTester::case3428(){}
void DeclarationBinderTester::case3429(){}
void DeclarationBinderTester::case3430(){}
void DeclarationBinderTester::case3431(){}
void DeclarationBinderTester::case3432(){}
void DeclarationBinderTester::case3433(){}
void DeclarationBinderTester::case3434(){}
void DeclarationBinderTester::case3435(){}
void DeclarationBinderTester::case3436(){}
void DeclarationBinderTester::case3437(){}
void DeclarationBinderTester::case3438(){}
void DeclarationBinderTester::case3439(){}
void DeclarationBinderTester::case3440(){}
void DeclarationBinderTester::case3441(){}
void DeclarationBinderTester::case3442(){}
void DeclarationBinderTester::case3443(){}
void DeclarationBinderTester::case3444(){}
void DeclarationBinderTester::case3445(){}
void DeclarationBinderTester::case3446(){}
void DeclarationBinderTester::case3447(){}
void DeclarationBinderTester::case3448(){}
void DeclarationBinderTester::case3449(){}
void DeclarationBinderTester::case3450(){}
void DeclarationBinderTester::case3451(){}
void DeclarationBinderTester::case3452(){}
void DeclarationBinderTester::case3453(){}
void DeclarationBinderTester::case3454(){}
void DeclarationBinderTester::case3455(){}
void DeclarationBinderTester::case3456(){}
void DeclarationBinderTester::case3457(){}
void DeclarationBinderTester::case3458(){}
void DeclarationBinderTester::case3459(){}
void DeclarationBinderTester::case3460(){}
void DeclarationBinderTester::case3461(){}
void DeclarationBinderTester::case3462(){}
void DeclarationBinderTester::case3463(){}
void DeclarationBinderTester::case3464(){}
void DeclarationBinderTester::case3465(){}
void DeclarationBinderTester::case3466(){}
void DeclarationBinderTester::case3467(){}
void DeclarationBinderTester::case3468(){}
void DeclarationBinderTester::case3469(){}
void DeclarationBinderTester::case3470(){}
void DeclarationBinderTester::case3471(){}
void DeclarationBinderTester::case3472(){}
void DeclarationBinderTester::case3473(){}
void DeclarationBinderTester::case3474(){}
void DeclarationBinderTester::case3475(){}
void DeclarationBinderTester::case3476(){}
void DeclarationBinderTester::case3477(){}
void DeclarationBinderTester::case3478(){}
void DeclarationBinderTester::case3479(){}
void DeclarationBinderTester::case3480(){}
void DeclarationBinderTester::case3481(){}
void DeclarationBinderTester::case3482(){}
void DeclarationBinderTester::case3483(){}
void DeclarationBinderTester::case3484(){}
void DeclarationBinderTester::case3485(){}
void DeclarationBinderTester::case3486(){}
void DeclarationBinderTester::case3487(){}
void DeclarationBinderTester::case3488(){}
void DeclarationBinderTester::case3489(){}
void DeclarationBinderTester::case3490(){}
void DeclarationBinderTester::case3491(){}
void DeclarationBinderTester::case3492(){}
void DeclarationBinderTester::case3493(){}
void DeclarationBinderTester::case3494(){}
void DeclarationBinderTester::case3495(){}
void DeclarationBinderTester::case3496(){}
void DeclarationBinderTester::case3497(){}
void DeclarationBinderTester::case3498(){}
void DeclarationBinderTester::case3499(){}

void DeclarationBinderTester::case3500()
{
    bind("int x ;",
         Expectation()
            .declaration(Decl()
                     .Object("x", SymbolKind::VariableDeclaration)
                     .ty_.Basic(BasicTypeKind::Int_S))
         .scopePath({}));
}

void DeclarationBinderTester::case3501()
{
    bind("void f() { int x ; }",
         Expectation()
            .declaration(Decl()
                     .Object("x", SymbolKind::VariableDeclaration, ScopeKind::Block)
                     .ty_.Basic(BasicTypeKind::Int_S))
            .scopePath({0}));
}

void DeclarationBinderTester::case3502()
{
    bind("void f() { { int x ; } }",
         Expectation()
            .declaration(Decl()
                     .Object("x", SymbolKind::VariableDeclaration, ScopeKind::Block)
                     .ty_.Basic(BasicTypeKind::Int_S))
         .scopePath({0, 0}));
}

void DeclarationBinderTester::case3503()
{
    bind("void f() { { } { int x ; } }",
         Expectation()
            .declaration(Decl()
                     .Object("x", SymbolKind::VariableDeclaration, ScopeKind::Block)
                     .ty_.Basic(BasicTypeKind::Int_S))
            .scopePath({0, 1}));
}

void DeclarationBinderTester::case3504()
{
    bind("void f() { } void g() { { } { int x ; } { } }",
         Expectation()
            .declaration(Decl()
                     .Object("x", SymbolKind::VariableDeclaration, ScopeKind::Block)
                     .ty_.Basic(BasicTypeKind::Int_S))
         .scopePath({1, 1}));
}

void DeclarationBinderTester::case3505()
{
    bind("void f() { } void g() { { } { } { { int x ; } } }",
         Expectation()
            .declaration(Decl()
                     .Object("x", SymbolKind::VariableDeclaration, ScopeKind::Block)
                     .ty_.Basic(BasicTypeKind::Int_S))
         .scopePath({1, 2, 0}));
}

void DeclarationBinderTester::case3506()
{
    bind(R"(
struct x { };
)",
         Expectation()
        .declaration(Decl()
             .Type("x", SymbolKind::StructDeclaration)
             .inNameSpace(NameSpace::Tags)
             .withScopeKind(ScopeKind::File))
         .scopePath({}));

}

void DeclarationBinderTester::case3507()
{
    bind("long __max_align_ll __attribute__( ( __aligned__ ( _Alignof ( double ) ) ) ) ;");
}

void DeclarationBinderTester::case3508()
{
    bind("long long __max_align_ll __attribute__( ( __aligned__ ( _Alignof ( long long ) ) ) ) ;");
}

void DeclarationBinderTester::case3509()
{
    bind("long __max_align_ll __attribute__( ( __aligned__ ( _Alignof ( struct x ) ) ) ) ;");
}

void DeclarationBinderTester::case3510()
{
    bind("struct x __max_align_ll __attribute__( ( __aligned__ ( _Alignof ( double ) ) ) ) ;");
}

void DeclarationBinderTester::case3511()
{
    bind("int x = sizeof ( long ) ;");
}

void DeclarationBinderTester::case3512()
{
    bind("int x = sizeof ( struct y ) ;");
}

void DeclarationBinderTester::case3513()
{
    bind("int x = sizeof ( y ) ;");
}

void DeclarationBinderTester::case3514()
{
    bind("typedef int x ; int y ( ) { return sizeof ( x ) ; }");
}

void DeclarationBinderTester::case3515(){}
void DeclarationBinderTester::case3516(){}
void DeclarationBinderTester::case3517(){}
void DeclarationBinderTester::case3518(){}
void DeclarationBinderTester::case3519(){}
void DeclarationBinderTester::case3520(){}
void DeclarationBinderTester::case3521(){}
void DeclarationBinderTester::case3522(){}
void DeclarationBinderTester::case3523(){}
void DeclarationBinderTester::case3524(){}
void DeclarationBinderTester::case3525(){}
void DeclarationBinderTester::case3526(){}
void DeclarationBinderTester::case3527(){}
void DeclarationBinderTester::case3528(){}
void DeclarationBinderTester::case3529(){}
void DeclarationBinderTester::case3530(){}
void DeclarationBinderTester::case3531(){}
void DeclarationBinderTester::case3532(){}
void DeclarationBinderTester::case3533(){}
void DeclarationBinderTester::case3534(){}
void DeclarationBinderTester::case3535(){}
void DeclarationBinderTester::case3536(){}
void DeclarationBinderTester::case3537(){}
void DeclarationBinderTester::case3538(){}
void DeclarationBinderTester::case3539(){}
void DeclarationBinderTester::case3540(){}
void DeclarationBinderTester::case3541(){}
void DeclarationBinderTester::case3542(){}
void DeclarationBinderTester::case3543(){}
void DeclarationBinderTester::case3544(){}
void DeclarationBinderTester::case3545(){}
void DeclarationBinderTester::case3546(){}
void DeclarationBinderTester::case3547(){}
void DeclarationBinderTester::case3548(){}
void DeclarationBinderTester::case3549(){}

void DeclarationBinderTester::case3550()
{
    bind("__extension__ asm ( \"nop\" ) ;");
}

void DeclarationBinderTester::case3551()
{
    bind("asm ( \"nop\" ) ;");
}

void DeclarationBinderTester::case3552(){}
void DeclarationBinderTester::case3553(){}
void DeclarationBinderTester::case3554(){}
void DeclarationBinderTester::case3555(){}
void DeclarationBinderTester::case3556(){}
void DeclarationBinderTester::case3557(){}
void DeclarationBinderTester::case3558(){}
void DeclarationBinderTester::case3559(){}
void DeclarationBinderTester::case3560(){}
void DeclarationBinderTester::case3561(){}
void DeclarationBinderTester::case3562(){}
void DeclarationBinderTester::case3563(){}
void DeclarationBinderTester::case3564(){}
void DeclarationBinderTester::case3565(){}
void DeclarationBinderTester::case3566(){}
void DeclarationBinderTester::case3567(){}
void DeclarationBinderTester::case3568(){}
void DeclarationBinderTester::case3569(){}
void DeclarationBinderTester::case3570(){}
void DeclarationBinderTester::case3571(){}
void DeclarationBinderTester::case3572(){}
void DeclarationBinderTester::case3573(){}
void DeclarationBinderTester::case3574(){}
void DeclarationBinderTester::case3575(){}
void DeclarationBinderTester::case3576(){}
void DeclarationBinderTester::case3577(){}
void DeclarationBinderTester::case3578(){}
void DeclarationBinderTester::case3579(){}
void DeclarationBinderTester::case3580(){}
void DeclarationBinderTester::case3581(){}
void DeclarationBinderTester::case3582(){}
void DeclarationBinderTester::case3583(){}
void DeclarationBinderTester::case3584(){}
void DeclarationBinderTester::case3585(){}
void DeclarationBinderTester::case3586(){}
void DeclarationBinderTester::case3587(){}
void DeclarationBinderTester::case3588(){}
void DeclarationBinderTester::case3589(){}
void DeclarationBinderTester::case3590(){}
void DeclarationBinderTester::case3591(){}
void DeclarationBinderTester::case3592(){}
void DeclarationBinderTester::case3593(){}
void DeclarationBinderTester::case3594(){}
void DeclarationBinderTester::case3595(){}
void DeclarationBinderTester::case3596(){}
void DeclarationBinderTester::case3597(){}
void DeclarationBinderTester::case3598(){}
void DeclarationBinderTester::case3599(){}
