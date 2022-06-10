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

void BinderTester::case3001()
{
    bind("struct x { int y ; } ;",
         Expectation()
         .binding(DeclSummary()
                  .Type("struct x", NamedTypeKind::Structure)
                  .withNameKind(SymbolNameKind::Tag)
                  .withNameSpaceKind(NameSpaceKind::Tags)
                  .withTagChoice(TagSymbolName::TagChoice::Struct)
                  .withScopeKind(ScopeKind::File)));
}

void BinderTester::case3002()
{
    bind("union x { int y ; } ;",
         Expectation()
         .binding(DeclSummary()
                  .Type("union x", NamedTypeKind::Union)
                  .withNameKind(SymbolNameKind::Tag)
                  .withNameSpaceKind(NameSpaceKind::Tags)
                  .withTagChoice(TagSymbolName::TagChoice::Union)
                  .withScopeKind(ScopeKind::File)));
}

void BinderTester::case3003(){}
void BinderTester::case3004(){}
void BinderTester::case3005(){}
void BinderTester::case3006(){}
void BinderTester::case3007(){}
void BinderTester::case3008(){}
void BinderTester::case3009(){}
void BinderTester::case3010(){}
void BinderTester::case3011(){}
void BinderTester::case3012(){}
void BinderTester::case3013(){}
void BinderTester::case3014(){}
void BinderTester::case3015(){}
void BinderTester::case3016(){}
void BinderTester::case3017(){}
void BinderTester::case3018(){}
void BinderTester::case3019(){}
void BinderTester::case3020(){}
void BinderTester::case3021(){}
void BinderTester::case3022(){}
void BinderTester::case3023(){}
void BinderTester::case3024(){}
void BinderTester::case3025(){}
void BinderTester::case3026(){}
void BinderTester::case3027(){}
void BinderTester::case3028(){}
void BinderTester::case3029(){}
void BinderTester::case3030(){}
void BinderTester::case3031(){}
void BinderTester::case3032(){}
void BinderTester::case3033(){}
void BinderTester::case3034(){}
void BinderTester::case3035(){}
void BinderTester::case3036(){}
void BinderTester::case3037(){}
void BinderTester::case3038(){}
void BinderTester::case3039(){}
void BinderTester::case3040(){}
void BinderTester::case3041(){}
void BinderTester::case3042(){}
void BinderTester::case3043(){}
void BinderTester::case3044(){}
void BinderTester::case3045(){}
void BinderTester::case3046(){}
void BinderTester::case3047(){}
void BinderTester::case3048(){}
void BinderTester::case3049(){}

void BinderTester::case3050()
{
    bind("enum x { y } ;",
         Expectation()
         .binding(DeclSummary()
                  .Type("enum x", NamedTypeKind::Enumeration)
                  .withNameKind(SymbolNameKind::Tag)
                  .withNameSpaceKind(NameSpaceKind::Tags)
                  .withTagChoice(TagSymbolName::TagChoice::Enum)
                  .withScopeKind(ScopeKind::File))
         .binding(DeclSummary()
                  .Value("y", ValueKind::Enumerator)
                  .TySpec.basis("int", NamedTypeKind::Builtin, BuiltinTypeKind::Int)));
}

void BinderTester::case3051()
{
    bind("enum x { y , z } ;",
         Expectation()
         .binding(DeclSummary()
                  .Type("enum x", NamedTypeKind::Enumeration)
                  .withNameKind(SymbolNameKind::Tag)
                  .withNameSpaceKind(NameSpaceKind::Tags)
                  .withTagChoice(TagSymbolName::TagChoice::Enum)
                  .withScopeKind(ScopeKind::File))
         .binding(DeclSummary()
                  .Value("y", ValueKind::Enumerator)
                  .TySpec.basis("int", NamedTypeKind::Builtin, BuiltinTypeKind::Int))
         .binding(DeclSummary()
                  .Value("z", ValueKind::Enumerator)
                  .TySpec.basis("int", NamedTypeKind::Builtin, BuiltinTypeKind::Int)));
}

void BinderTester::case3052()
{
    bind("enum x { y = 0 , z } ;",
         Expectation()
         .binding(DeclSummary()
                  .Type("enum x", NamedTypeKind::Enumeration)
                  .withNameKind(SymbolNameKind::Tag)
                  .withNameSpaceKind(NameSpaceKind::Tags)
                  .withTagChoice(TagSymbolName::TagChoice::Enum)
                  .withScopeKind(ScopeKind::File))
         .binding(DeclSummary()
                  .Value("y", ValueKind::Enumerator)
                  .TySpec.basis("int", NamedTypeKind::Builtin, BuiltinTypeKind::Int))
         .binding(DeclSummary()
                  .Value("z", ValueKind::Enumerator)
                  .TySpec.basis("int", NamedTypeKind::Builtin, BuiltinTypeKind::Int)));
}

void BinderTester::case3053()
{
    bind("enum x { y , z , w } ;",
         Expectation()
         .binding(DeclSummary()
                  .Type("enum x", NamedTypeKind::Enumeration)
                  .withNameKind(SymbolNameKind::Tag)
                  .withNameSpaceKind(NameSpaceKind::Tags)
                  .withTagChoice(TagSymbolName::TagChoice::Enum)
                  .withScopeKind(ScopeKind::File))
         .binding(DeclSummary()
                  .Value("y", ValueKind::Enumerator)
                  .TySpec.basis("int", NamedTypeKind::Builtin, BuiltinTypeKind::Int))
         .binding(DeclSummary()
                  .Value("z", ValueKind::Enumerator)
                  .TySpec.basis("int", NamedTypeKind::Builtin, BuiltinTypeKind::Int))
         .binding(DeclSummary()
                  .Value("w", ValueKind::Enumerator)
                  .TySpec.basis("int", NamedTypeKind::Builtin, BuiltinTypeKind::Int)));
}

void BinderTester::case3054()
{
    bind("enum x { y , z = 0 , w } ;",
         Expectation()
         .binding(DeclSummary()
                  .Type("enum x", NamedTypeKind::Enumeration)
                  .withNameKind(SymbolNameKind::Tag)
                  .withNameSpaceKind(NameSpaceKind::Tags)
                  .withTagChoice(TagSymbolName::TagChoice::Enum)
                  .withScopeKind(ScopeKind::File))
         .binding(DeclSummary()
                  .Value("y", ValueKind::Enumerator)
                  .TySpec.basis("int", NamedTypeKind::Builtin, BuiltinTypeKind::Int))
         .binding(DeclSummary()
                  .Value("z", ValueKind::Enumerator)
                  .TySpec.basis("int", NamedTypeKind::Builtin, BuiltinTypeKind::Int))
         .binding(DeclSummary()
                  .Value("w", ValueKind::Enumerator)
                  .TySpec.basis("int", NamedTypeKind::Builtin, BuiltinTypeKind::Int)));
}

void BinderTester::case3055(){}
void BinderTester::case3056(){}
void BinderTester::case3057(){}
void BinderTester::case3058(){}
void BinderTester::case3059(){}
void BinderTester::case3060(){}
void BinderTester::case3061(){}
void BinderTester::case3062(){}
void BinderTester::case3063(){}
void BinderTester::case3064(){}
void BinderTester::case3065(){}
void BinderTester::case3066(){}
void BinderTester::case3067(){}
void BinderTester::case3068(){}
void BinderTester::case3069(){}
void BinderTester::case3070(){}
void BinderTester::case3071(){}
void BinderTester::case3072(){}
void BinderTester::case3073(){}
void BinderTester::case3074(){}
void BinderTester::case3075(){}
void BinderTester::case3076(){}
void BinderTester::case3077(){}
void BinderTester::case3078(){}
void BinderTester::case3079(){}
void BinderTester::case3080(){}
void BinderTester::case3081(){}
void BinderTester::case3082(){}
void BinderTester::case3083(){}
void BinderTester::case3084(){}
void BinderTester::case3085(){}
void BinderTester::case3086(){}
void BinderTester::case3087(){}
void BinderTester::case3088(){}
void BinderTester::case3089(){}
void BinderTester::case3090(){}
void BinderTester::case3091(){}
void BinderTester::case3092(){}
void BinderTester::case3093(){}
void BinderTester::case3094(){}
void BinderTester::case3095(){}
void BinderTester::case3096(){}
void BinderTester::case3097(){}
void BinderTester::case3098(){}
void BinderTester::case3099(){}

void BinderTester::case3100(){}
void BinderTester::case3101(){}
void BinderTester::case3102(){}
void BinderTester::case3103(){}
void BinderTester::case3104(){}
void BinderTester::case3105(){}
void BinderTester::case3106(){}
void BinderTester::case3107(){}
void BinderTester::case3108(){}
void BinderTester::case3109(){}
void BinderTester::case3110(){}
void BinderTester::case3111(){}
void BinderTester::case3112(){}
void BinderTester::case3113(){}
void BinderTester::case3114(){}
void BinderTester::case3115(){}
void BinderTester::case3116(){}
void BinderTester::case3117(){}
void BinderTester::case3118(){}
void BinderTester::case3119(){}
void BinderTester::case3120(){}
void BinderTester::case3121(){}
void BinderTester::case3122(){}
void BinderTester::case3123(){}
void BinderTester::case3124(){}
void BinderTester::case3125(){}
void BinderTester::case3126(){}
void BinderTester::case3127(){}
void BinderTester::case3128(){}
void BinderTester::case3129(){}
void BinderTester::case3130(){}
void BinderTester::case3131(){}
void BinderTester::case3132(){}
void BinderTester::case3133(){}
void BinderTester::case3134(){}
void BinderTester::case3135(){}
void BinderTester::case3136(){}
void BinderTester::case3137(){}
void BinderTester::case3138(){}
void BinderTester::case3139(){}
void BinderTester::case3140(){}
void BinderTester::case3141(){}
void BinderTester::case3142(){}
void BinderTester::case3143(){}
void BinderTester::case3144(){}
void BinderTester::case3145(){}
void BinderTester::case3146(){}
void BinderTester::case3147(){}
void BinderTester::case3148(){}
void BinderTester::case3149(){}
void BinderTester::case3150(){}
void BinderTester::case3151(){}
void BinderTester::case3152(){}
void BinderTester::case3153(){}
void BinderTester::case3154(){}
void BinderTester::case3155(){}
void BinderTester::case3156(){}
void BinderTester::case3157(){}
void BinderTester::case3158(){}
void BinderTester::case3159(){}
void BinderTester::case3160(){}
void BinderTester::case3161(){}
void BinderTester::case3162(){}
void BinderTester::case3163(){}
void BinderTester::case3164(){}
void BinderTester::case3165(){}
void BinderTester::case3166(){}
void BinderTester::case3167(){}
void BinderTester::case3168(){}
void BinderTester::case3169(){}
void BinderTester::case3170(){}
void BinderTester::case3171(){}
void BinderTester::case3172(){}
void BinderTester::case3173(){}
void BinderTester::case3174(){}
void BinderTester::case3175(){}
void BinderTester::case3176(){}
void BinderTester::case3177(){}
void BinderTester::case3178(){}
void BinderTester::case3179(){}
void BinderTester::case3180(){}
void BinderTester::case3181(){}
void BinderTester::case3182(){}
void BinderTester::case3183(){}
void BinderTester::case3184(){}
void BinderTester::case3185(){}
void BinderTester::case3186(){}
void BinderTester::case3187(){}
void BinderTester::case3188(){}
void BinderTester::case3189(){}
void BinderTester::case3190(){}
void BinderTester::case3191(){}
void BinderTester::case3192(){}
void BinderTester::case3193(){}
void BinderTester::case3194(){}
void BinderTester::case3195(){}
void BinderTester::case3196(){}
void BinderTester::case3197(){}
void BinderTester::case3198(){}
void BinderTester::case3199(){}

void BinderTester::case3200(){}
void BinderTester::case3201(){}
void BinderTester::case3202(){}
void BinderTester::case3203(){}
void BinderTester::case3204(){}
void BinderTester::case3205(){}
void BinderTester::case3206(){}
void BinderTester::case3207(){}
void BinderTester::case3208(){}
void BinderTester::case3209(){}
void BinderTester::case3210(){}
void BinderTester::case3211(){}
void BinderTester::case3212(){}
void BinderTester::case3213(){}
void BinderTester::case3214(){}
void BinderTester::case3215(){}
void BinderTester::case3216(){}
void BinderTester::case3217(){}
void BinderTester::case3218(){}
void BinderTester::case3219(){}
void BinderTester::case3220(){}
void BinderTester::case3221(){}
void BinderTester::case3222(){}
void BinderTester::case3223(){}
void BinderTester::case3224(){}
void BinderTester::case3225(){}
void BinderTester::case3226(){}
void BinderTester::case3227(){}
void BinderTester::case3228(){}
void BinderTester::case3229(){}
void BinderTester::case3230(){}
void BinderTester::case3231(){}
void BinderTester::case3232(){}
void BinderTester::case3233(){}
void BinderTester::case3234(){}
void BinderTester::case3235(){}
void BinderTester::case3236(){}
void BinderTester::case3237(){}
void BinderTester::case3238(){}
void BinderTester::case3239(){}
void BinderTester::case3240(){}
void BinderTester::case3241(){}
void BinderTester::case3242(){}
void BinderTester::case3243(){}
void BinderTester::case3244(){}
void BinderTester::case3245(){}
void BinderTester::case3246(){}
void BinderTester::case3247(){}
void BinderTester::case3248(){}
void BinderTester::case3249(){}
void BinderTester::case3250(){}
void BinderTester::case3251(){}
void BinderTester::case3252(){}
void BinderTester::case3253(){}
void BinderTester::case3254(){}
void BinderTester::case3255(){}
void BinderTester::case3256(){}
void BinderTester::case3257(){}
void BinderTester::case3258(){}
void BinderTester::case3259(){}
void BinderTester::case3260(){}
void BinderTester::case3261(){}
void BinderTester::case3262(){}
void BinderTester::case3263(){}
void BinderTester::case3264(){}
void BinderTester::case3265(){}
void BinderTester::case3266(){}
void BinderTester::case3267(){}
void BinderTester::case3268(){}
void BinderTester::case3269(){}
void BinderTester::case3270(){}
void BinderTester::case3271(){}
void BinderTester::case3272(){}
void BinderTester::case3273(){}
void BinderTester::case3274(){}
void BinderTester::case3275(){}
void BinderTester::case3276(){}
void BinderTester::case3277(){}
void BinderTester::case3278(){}
void BinderTester::case3279(){}
void BinderTester::case3280(){}
void BinderTester::case3281(){}
void BinderTester::case3282(){}
void BinderTester::case3283(){}
void BinderTester::case3284(){}
void BinderTester::case3285(){}
void BinderTester::case3286(){}
void BinderTester::case3287(){}
void BinderTester::case3288(){}
void BinderTester::case3289(){}
void BinderTester::case3290(){}
void BinderTester::case3291(){}
void BinderTester::case3292(){}
void BinderTester::case3293(){}
void BinderTester::case3294(){}
void BinderTester::case3295(){}
void BinderTester::case3296(){}
void BinderTester::case3297(){}
void BinderTester::case3298(){}
void BinderTester::case3299(){}

void BinderTester::case3300(){}
void BinderTester::case3301(){}
void BinderTester::case3302(){}
void BinderTester::case3303(){}
void BinderTester::case3304(){}
void BinderTester::case3305(){}
void BinderTester::case3306(){}
void BinderTester::case3307(){}
void BinderTester::case3308(){}
void BinderTester::case3309(){}
void BinderTester::case3310(){}
void BinderTester::case3311(){}
void BinderTester::case3312(){}
void BinderTester::case3313(){}
void BinderTester::case3314(){}
void BinderTester::case3315(){}
void BinderTester::case3316(){}
void BinderTester::case3317(){}
void BinderTester::case3318(){}
void BinderTester::case3319(){}
void BinderTester::case3320(){}
void BinderTester::case3321(){}
void BinderTester::case3322(){}
void BinderTester::case3323(){}
void BinderTester::case3324(){}
void BinderTester::case3325(){}
void BinderTester::case3326(){}
void BinderTester::case3327(){}
void BinderTester::case3328(){}
void BinderTester::case3329(){}
void BinderTester::case3330(){}
void BinderTester::case3331(){}
void BinderTester::case3332(){}
void BinderTester::case3333(){}
void BinderTester::case3334(){}
void BinderTester::case3335(){}
void BinderTester::case3336(){}
void BinderTester::case3337(){}
void BinderTester::case3338(){}
void BinderTester::case3339(){}
void BinderTester::case3340(){}
void BinderTester::case3341(){}
void BinderTester::case3342(){}
void BinderTester::case3343(){}
void BinderTester::case3344(){}
void BinderTester::case3345(){}
void BinderTester::case3346(){}
void BinderTester::case3347(){}
void BinderTester::case3348(){}
void BinderTester::case3349(){}
void BinderTester::case3350(){}
void BinderTester::case3351(){}
void BinderTester::case3352(){}
void BinderTester::case3353(){}
void BinderTester::case3354(){}
void BinderTester::case3355(){}
void BinderTester::case3356(){}
void BinderTester::case3357(){}
void BinderTester::case3358(){}
void BinderTester::case3359(){}
void BinderTester::case3360(){}
void BinderTester::case3361(){}
void BinderTester::case3362(){}
void BinderTester::case3363(){}
void BinderTester::case3364(){}
void BinderTester::case3365(){}
void BinderTester::case3366(){}
void BinderTester::case3367(){}
void BinderTester::case3368(){}
void BinderTester::case3369(){}
void BinderTester::case3370(){}
void BinderTester::case3371(){}
void BinderTester::case3372(){}
void BinderTester::case3373(){}
void BinderTester::case3374(){}
void BinderTester::case3375(){}
void BinderTester::case3376(){}
void BinderTester::case3377(){}
void BinderTester::case3378(){}
void BinderTester::case3379(){}
void BinderTester::case3380(){}
void BinderTester::case3381(){}
void BinderTester::case3382(){}
void BinderTester::case3383(){}
void BinderTester::case3384(){}
void BinderTester::case3385(){}
void BinderTester::case3386(){}
void BinderTester::case3387(){}
void BinderTester::case3388(){}
void BinderTester::case3389(){}
void BinderTester::case3390(){}
void BinderTester::case3391(){}
void BinderTester::case3392(){}
void BinderTester::case3393(){}
void BinderTester::case3394(){}
void BinderTester::case3395(){}
void BinderTester::case3396(){}
void BinderTester::case3397(){}
void BinderTester::case3398(){}
void BinderTester::case3399(){}

void BinderTester::case3400(){}
void BinderTester::case3401(){}
void BinderTester::case3402(){}
void BinderTester::case3403(){}
void BinderTester::case3404(){}
void BinderTester::case3405(){}
void BinderTester::case3406(){}
void BinderTester::case3407(){}
void BinderTester::case3408(){}
void BinderTester::case3409(){}
void BinderTester::case3410(){}
void BinderTester::case3411(){}
void BinderTester::case3412(){}
void BinderTester::case3413(){}
void BinderTester::case3414(){}
void BinderTester::case3415(){}
void BinderTester::case3416(){}
void BinderTester::case3417(){}
void BinderTester::case3418(){}
void BinderTester::case3419(){}
void BinderTester::case3420(){}
void BinderTester::case3421(){}
void BinderTester::case3422(){}
void BinderTester::case3423(){}
void BinderTester::case3424(){}
void BinderTester::case3425(){}
void BinderTester::case3426(){}
void BinderTester::case3427(){}
void BinderTester::case3428(){}
void BinderTester::case3429(){}
void BinderTester::case3430(){}
void BinderTester::case3431(){}
void BinderTester::case3432(){}
void BinderTester::case3433(){}
void BinderTester::case3434(){}
void BinderTester::case3435(){}
void BinderTester::case3436(){}
void BinderTester::case3437(){}
void BinderTester::case3438(){}
void BinderTester::case3439(){}
void BinderTester::case3440(){}
void BinderTester::case3441(){}
void BinderTester::case3442(){}
void BinderTester::case3443(){}
void BinderTester::case3444(){}
void BinderTester::case3445(){}
void BinderTester::case3446(){}
void BinderTester::case3447(){}
void BinderTester::case3448(){}
void BinderTester::case3449(){}
void BinderTester::case3450(){}
void BinderTester::case3451(){}
void BinderTester::case3452(){}
void BinderTester::case3453(){}
void BinderTester::case3454(){}
void BinderTester::case3455(){}
void BinderTester::case3456(){}
void BinderTester::case3457(){}
void BinderTester::case3458(){}
void BinderTester::case3459(){}
void BinderTester::case3460(){}
void BinderTester::case3461(){}
void BinderTester::case3462(){}
void BinderTester::case3463(){}
void BinderTester::case3464(){}
void BinderTester::case3465(){}
void BinderTester::case3466(){}
void BinderTester::case3467(){}
void BinderTester::case3468(){}
void BinderTester::case3469(){}
void BinderTester::case3470(){}
void BinderTester::case3471(){}
void BinderTester::case3472(){}
void BinderTester::case3473(){}
void BinderTester::case3474(){}
void BinderTester::case3475(){}
void BinderTester::case3476(){}
void BinderTester::case3477(){}
void BinderTester::case3478(){}
void BinderTester::case3479(){}
void BinderTester::case3480(){}
void BinderTester::case3481(){}
void BinderTester::case3482(){}
void BinderTester::case3483(){}
void BinderTester::case3484(){}
void BinderTester::case3485(){}
void BinderTester::case3486(){}
void BinderTester::case3487(){}
void BinderTester::case3488(){}
void BinderTester::case3489(){}
void BinderTester::case3490(){}
void BinderTester::case3491(){}
void BinderTester::case3492(){}
void BinderTester::case3493(){}
void BinderTester::case3494(){}
void BinderTester::case3495(){}
void BinderTester::case3496(){}
void BinderTester::case3497(){}
void BinderTester::case3498(){}
void BinderTester::case3499(){}

void BinderTester::case3500(){}
void BinderTester::case3501(){}
void BinderTester::case3502(){}
void BinderTester::case3503(){}
void BinderTester::case3504(){}
void BinderTester::case3505(){}
void BinderTester::case3506(){}
void BinderTester::case3507(){}
void BinderTester::case3508(){}
void BinderTester::case3509(){}
void BinderTester::case3510(){}
void BinderTester::case3511(){}
void BinderTester::case3512(){}
void BinderTester::case3513(){}
void BinderTester::case3514(){}
void BinderTester::case3515(){}
void BinderTester::case3516(){}
void BinderTester::case3517(){}
void BinderTester::case3518(){}
void BinderTester::case3519(){}
void BinderTester::case3520(){}
void BinderTester::case3521(){}
void BinderTester::case3522(){}
void BinderTester::case3523(){}
void BinderTester::case3524(){}
void BinderTester::case3525(){}
void BinderTester::case3526(){}
void BinderTester::case3527(){}
void BinderTester::case3528(){}
void BinderTester::case3529(){}
void BinderTester::case3530(){}
void BinderTester::case3531(){}
void BinderTester::case3532(){}
void BinderTester::case3533(){}
void BinderTester::case3534(){}
void BinderTester::case3535(){}
void BinderTester::case3536(){}
void BinderTester::case3537(){}
void BinderTester::case3538(){}
void BinderTester::case3539(){}
void BinderTester::case3540(){}
void BinderTester::case3541(){}
void BinderTester::case3542(){}
void BinderTester::case3543(){}
void BinderTester::case3544(){}
void BinderTester::case3545(){}
void BinderTester::case3546(){}
void BinderTester::case3547(){}
void BinderTester::case3548(){}
void BinderTester::case3549(){}
void BinderTester::case3550(){}
void BinderTester::case3551(){}
void BinderTester::case3552(){}
void BinderTester::case3553(){}
void BinderTester::case3554(){}
void BinderTester::case3555(){}
void BinderTester::case3556(){}
void BinderTester::case3557(){}
void BinderTester::case3558(){}
void BinderTester::case3559(){}
void BinderTester::case3560(){}
void BinderTester::case3561(){}
void BinderTester::case3562(){}
void BinderTester::case3563(){}
void BinderTester::case3564(){}
void BinderTester::case3565(){}
void BinderTester::case3566(){}
void BinderTester::case3567(){}
void BinderTester::case3568(){}
void BinderTester::case3569(){}
void BinderTester::case3570(){}
void BinderTester::case3571(){}
void BinderTester::case3572(){}
void BinderTester::case3573(){}
void BinderTester::case3574(){}
void BinderTester::case3575(){}
void BinderTester::case3576(){}
void BinderTester::case3577(){}
void BinderTester::case3578(){}
void BinderTester::case3579(){}
void BinderTester::case3580(){}
void BinderTester::case3581(){}
void BinderTester::case3582(){}
void BinderTester::case3583(){}
void BinderTester::case3584(){}
void BinderTester::case3585(){}
void BinderTester::case3586(){}
void BinderTester::case3587(){}
void BinderTester::case3588(){}
void BinderTester::case3589(){}
void BinderTester::case3590(){}
void BinderTester::case3591(){}
void BinderTester::case3592(){}
void BinderTester::case3593(){}
void BinderTester::case3594(){}
void BinderTester::case3595(){}
void BinderTester::case3596(){}
void BinderTester::case3597(){}
void BinderTester::case3598(){}
void BinderTester::case3599(){}
