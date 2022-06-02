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

void BinderTest::case3001()
{
    bind("struct x { int y ; } ;",
         Expectation()
         .binding(DeclSummary()
                  .Type("struct x", NamedTypeKind::Tag)
                  .withNameKind(SymbolNameKind::Tagged)
                  .withNameSpaceKind(NameSpaceKind::Tags)
                  .withTagKind(TagSymbolNameKind::Structure)
                  .withScopeKind(ScopeKind::File)));
}

void BinderTest::case3002()
{
    bind("union x { int y ; } ;",
         Expectation()
         .binding(DeclSummary()
                  .Type("union x", NamedTypeKind::Tag)
                  .withNameKind(SymbolNameKind::Tagged)
                  .withNameSpaceKind(NameSpaceKind::Tags)
                  .withTagKind(TagSymbolNameKind::Union)
                  .withScopeKind(ScopeKind::File)));
}

void BinderTest::case3003(){}
void BinderTest::case3004(){}
void BinderTest::case3005(){}
void BinderTest::case3006(){}
void BinderTest::case3007(){}
void BinderTest::case3008(){}
void BinderTest::case3009(){}
void BinderTest::case3010(){}
void BinderTest::case3011(){}
void BinderTest::case3012(){}
void BinderTest::case3013(){}
void BinderTest::case3014(){}
void BinderTest::case3015(){}
void BinderTest::case3016(){}
void BinderTest::case3017(){}
void BinderTest::case3018(){}
void BinderTest::case3019(){}
void BinderTest::case3020(){}
void BinderTest::case3021(){}
void BinderTest::case3022(){}
void BinderTest::case3023(){}
void BinderTest::case3024(){}
void BinderTest::case3025(){}
void BinderTest::case3026(){}
void BinderTest::case3027(){}
void BinderTest::case3028(){}
void BinderTest::case3029(){}
void BinderTest::case3030(){}
void BinderTest::case3031(){}
void BinderTest::case3032(){}
void BinderTest::case3033(){}
void BinderTest::case3034(){}
void BinderTest::case3035(){}
void BinderTest::case3036(){}
void BinderTest::case3037(){}
void BinderTest::case3038(){}
void BinderTest::case3039(){}
void BinderTest::case3040(){}
void BinderTest::case3041(){}
void BinderTest::case3042(){}
void BinderTest::case3043(){}
void BinderTest::case3044(){}
void BinderTest::case3045(){}
void BinderTest::case3046(){}
void BinderTest::case3047(){}
void BinderTest::case3048(){}
void BinderTest::case3049(){}

void BinderTest::case3050()
{
    bind("enum x { y } ;",
         Expectation()
         .binding(DeclSummary()
                  .Type("enum x", NamedTypeKind::Tag)
                  .withNameKind(SymbolNameKind::Tagged)
                  .withNameSpaceKind(NameSpaceKind::Tags)
                  .withTagKind(TagSymbolNameKind::Enumeration)
                  .withScopeKind(ScopeKind::File))
         .binding(DeclSummary()
                  .Value("y", ValueKind::Enumerator)
                  .TySpec.basis("int", NamedTypeKind::Builtin, BuiltinTypeKind::Int)));
}

void BinderTest::case3051()
{
    bind("enum x { y , z } ;",
         Expectation()
         .binding(DeclSummary()
                  .Type("enum x", NamedTypeKind::Tag)
                  .withNameKind(SymbolNameKind::Tagged)
                  .withNameSpaceKind(NameSpaceKind::Tags)
                  .withTagKind(TagSymbolNameKind::Enumeration)
                  .withScopeKind(ScopeKind::File))
         .binding(DeclSummary()
                  .Value("y", ValueKind::Enumerator)
                  .TySpec.basis("int", NamedTypeKind::Builtin, BuiltinTypeKind::Int))
         .binding(DeclSummary()
                  .Value("z", ValueKind::Enumerator)
                  .TySpec.basis("int", NamedTypeKind::Builtin, BuiltinTypeKind::Int)));
}

void BinderTest::case3052()
{
    bind("enum x { y = 0 , z } ;",
         Expectation()
         .binding(DeclSummary()
                  .Type("enum x", NamedTypeKind::Tag)
                  .withNameKind(SymbolNameKind::Tagged)
                  .withNameSpaceKind(NameSpaceKind::Tags)
                  .withTagKind(TagSymbolNameKind::Enumeration)
                  .withScopeKind(ScopeKind::File))
         .binding(DeclSummary()
                  .Value("y", ValueKind::Enumerator)
                  .TySpec.basis("int", NamedTypeKind::Builtin, BuiltinTypeKind::Int))
         .binding(DeclSummary()
                  .Value("z", ValueKind::Enumerator)
                  .TySpec.basis("int", NamedTypeKind::Builtin, BuiltinTypeKind::Int)));
}

void BinderTest::case3053()
{
    bind("enum x { y , z , w } ;",
         Expectation()
         .binding(DeclSummary()
                  .Type("enum x", NamedTypeKind::Tag)
                  .withNameKind(SymbolNameKind::Tagged)
                  .withNameSpaceKind(NameSpaceKind::Tags)
                  .withTagKind(TagSymbolNameKind::Enumeration)
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

void BinderTest::case3054()
{
    bind("enum x { y , z = 0 , w } ;",
         Expectation()
         .binding(DeclSummary()
                  .Type("enum x", NamedTypeKind::Tag)
                  .withNameKind(SymbolNameKind::Tagged)
                  .withNameSpaceKind(NameSpaceKind::Tags)
                  .withTagKind(TagSymbolNameKind::Enumeration)
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

void BinderTest::case3055(){}
void BinderTest::case3056(){}
void BinderTest::case3057(){}
void BinderTest::case3058(){}
void BinderTest::case3059(){}
void BinderTest::case3060(){}
void BinderTest::case3061(){}
void BinderTest::case3062(){}
void BinderTest::case3063(){}
void BinderTest::case3064(){}
void BinderTest::case3065(){}
void BinderTest::case3066(){}
void BinderTest::case3067(){}
void BinderTest::case3068(){}
void BinderTest::case3069(){}
void BinderTest::case3070(){}
void BinderTest::case3071(){}
void BinderTest::case3072(){}
void BinderTest::case3073(){}
void BinderTest::case3074(){}
void BinderTest::case3075(){}
void BinderTest::case3076(){}
void BinderTest::case3077(){}
void BinderTest::case3078(){}
void BinderTest::case3079(){}
void BinderTest::case3080(){}
void BinderTest::case3081(){}
void BinderTest::case3082(){}
void BinderTest::case3083(){}
void BinderTest::case3084(){}
void BinderTest::case3085(){}
void BinderTest::case3086(){}
void BinderTest::case3087(){}
void BinderTest::case3088(){}
void BinderTest::case3089(){}
void BinderTest::case3090(){}
void BinderTest::case3091(){}
void BinderTest::case3092(){}
void BinderTest::case3093(){}
void BinderTest::case3094(){}
void BinderTest::case3095(){}
void BinderTest::case3096(){}
void BinderTest::case3097(){}
void BinderTest::case3098(){}
void BinderTest::case3099(){}

void BinderTest::case3100(){}
void BinderTest::case3101(){}
void BinderTest::case3102(){}
void BinderTest::case3103(){}
void BinderTest::case3104(){}
void BinderTest::case3105(){}
void BinderTest::case3106(){}
void BinderTest::case3107(){}
void BinderTest::case3108(){}
void BinderTest::case3109(){}
void BinderTest::case3110(){}
void BinderTest::case3111(){}
void BinderTest::case3112(){}
void BinderTest::case3113(){}
void BinderTest::case3114(){}
void BinderTest::case3115(){}
void BinderTest::case3116(){}
void BinderTest::case3117(){}
void BinderTest::case3118(){}
void BinderTest::case3119(){}
void BinderTest::case3120(){}
void BinderTest::case3121(){}
void BinderTest::case3122(){}
void BinderTest::case3123(){}
void BinderTest::case3124(){}
void BinderTest::case3125(){}
void BinderTest::case3126(){}
void BinderTest::case3127(){}
void BinderTest::case3128(){}
void BinderTest::case3129(){}
void BinderTest::case3130(){}
void BinderTest::case3131(){}
void BinderTest::case3132(){}
void BinderTest::case3133(){}
void BinderTest::case3134(){}
void BinderTest::case3135(){}
void BinderTest::case3136(){}
void BinderTest::case3137(){}
void BinderTest::case3138(){}
void BinderTest::case3139(){}
void BinderTest::case3140(){}
void BinderTest::case3141(){}
void BinderTest::case3142(){}
void BinderTest::case3143(){}
void BinderTest::case3144(){}
void BinderTest::case3145(){}
void BinderTest::case3146(){}
void BinderTest::case3147(){}
void BinderTest::case3148(){}
void BinderTest::case3149(){}
void BinderTest::case3150(){}
void BinderTest::case3151(){}
void BinderTest::case3152(){}
void BinderTest::case3153(){}
void BinderTest::case3154(){}
void BinderTest::case3155(){}
void BinderTest::case3156(){}
void BinderTest::case3157(){}
void BinderTest::case3158(){}
void BinderTest::case3159(){}
void BinderTest::case3160(){}
void BinderTest::case3161(){}
void BinderTest::case3162(){}
void BinderTest::case3163(){}
void BinderTest::case3164(){}
void BinderTest::case3165(){}
void BinderTest::case3166(){}
void BinderTest::case3167(){}
void BinderTest::case3168(){}
void BinderTest::case3169(){}
void BinderTest::case3170(){}
void BinderTest::case3171(){}
void BinderTest::case3172(){}
void BinderTest::case3173(){}
void BinderTest::case3174(){}
void BinderTest::case3175(){}
void BinderTest::case3176(){}
void BinderTest::case3177(){}
void BinderTest::case3178(){}
void BinderTest::case3179(){}
void BinderTest::case3180(){}
void BinderTest::case3181(){}
void BinderTest::case3182(){}
void BinderTest::case3183(){}
void BinderTest::case3184(){}
void BinderTest::case3185(){}
void BinderTest::case3186(){}
void BinderTest::case3187(){}
void BinderTest::case3188(){}
void BinderTest::case3189(){}
void BinderTest::case3190(){}
void BinderTest::case3191(){}
void BinderTest::case3192(){}
void BinderTest::case3193(){}
void BinderTest::case3194(){}
void BinderTest::case3195(){}
void BinderTest::case3196(){}
void BinderTest::case3197(){}
void BinderTest::case3198(){}
void BinderTest::case3199(){}

void BinderTest::case3200(){}
void BinderTest::case3201(){}
void BinderTest::case3202(){}
void BinderTest::case3203(){}
void BinderTest::case3204(){}
void BinderTest::case3205(){}
void BinderTest::case3206(){}
void BinderTest::case3207(){}
void BinderTest::case3208(){}
void BinderTest::case3209(){}
void BinderTest::case3210(){}
void BinderTest::case3211(){}
void BinderTest::case3212(){}
void BinderTest::case3213(){}
void BinderTest::case3214(){}
void BinderTest::case3215(){}
void BinderTest::case3216(){}
void BinderTest::case3217(){}
void BinderTest::case3218(){}
void BinderTest::case3219(){}
void BinderTest::case3220(){}
void BinderTest::case3221(){}
void BinderTest::case3222(){}
void BinderTest::case3223(){}
void BinderTest::case3224(){}
void BinderTest::case3225(){}
void BinderTest::case3226(){}
void BinderTest::case3227(){}
void BinderTest::case3228(){}
void BinderTest::case3229(){}
void BinderTest::case3230(){}
void BinderTest::case3231(){}
void BinderTest::case3232(){}
void BinderTest::case3233(){}
void BinderTest::case3234(){}
void BinderTest::case3235(){}
void BinderTest::case3236(){}
void BinderTest::case3237(){}
void BinderTest::case3238(){}
void BinderTest::case3239(){}
void BinderTest::case3240(){}
void BinderTest::case3241(){}
void BinderTest::case3242(){}
void BinderTest::case3243(){}
void BinderTest::case3244(){}
void BinderTest::case3245(){}
void BinderTest::case3246(){}
void BinderTest::case3247(){}
void BinderTest::case3248(){}
void BinderTest::case3249(){}
void BinderTest::case3250(){}
void BinderTest::case3251(){}
void BinderTest::case3252(){}
void BinderTest::case3253(){}
void BinderTest::case3254(){}
void BinderTest::case3255(){}
void BinderTest::case3256(){}
void BinderTest::case3257(){}
void BinderTest::case3258(){}
void BinderTest::case3259(){}
void BinderTest::case3260(){}
void BinderTest::case3261(){}
void BinderTest::case3262(){}
void BinderTest::case3263(){}
void BinderTest::case3264(){}
void BinderTest::case3265(){}
void BinderTest::case3266(){}
void BinderTest::case3267(){}
void BinderTest::case3268(){}
void BinderTest::case3269(){}
void BinderTest::case3270(){}
void BinderTest::case3271(){}
void BinderTest::case3272(){}
void BinderTest::case3273(){}
void BinderTest::case3274(){}
void BinderTest::case3275(){}
void BinderTest::case3276(){}
void BinderTest::case3277(){}
void BinderTest::case3278(){}
void BinderTest::case3279(){}
void BinderTest::case3280(){}
void BinderTest::case3281(){}
void BinderTest::case3282(){}
void BinderTest::case3283(){}
void BinderTest::case3284(){}
void BinderTest::case3285(){}
void BinderTest::case3286(){}
void BinderTest::case3287(){}
void BinderTest::case3288(){}
void BinderTest::case3289(){}
void BinderTest::case3290(){}
void BinderTest::case3291(){}
void BinderTest::case3292(){}
void BinderTest::case3293(){}
void BinderTest::case3294(){}
void BinderTest::case3295(){}
void BinderTest::case3296(){}
void BinderTest::case3297(){}
void BinderTest::case3298(){}
void BinderTest::case3299(){}

void BinderTest::case3300(){}
void BinderTest::case3301(){}
void BinderTest::case3302(){}
void BinderTest::case3303(){}
void BinderTest::case3304(){}
void BinderTest::case3305(){}
void BinderTest::case3306(){}
void BinderTest::case3307(){}
void BinderTest::case3308(){}
void BinderTest::case3309(){}
void BinderTest::case3310(){}
void BinderTest::case3311(){}
void BinderTest::case3312(){}
void BinderTest::case3313(){}
void BinderTest::case3314(){}
void BinderTest::case3315(){}
void BinderTest::case3316(){}
void BinderTest::case3317(){}
void BinderTest::case3318(){}
void BinderTest::case3319(){}
void BinderTest::case3320(){}
void BinderTest::case3321(){}
void BinderTest::case3322(){}
void BinderTest::case3323(){}
void BinderTest::case3324(){}
void BinderTest::case3325(){}
void BinderTest::case3326(){}
void BinderTest::case3327(){}
void BinderTest::case3328(){}
void BinderTest::case3329(){}
void BinderTest::case3330(){}
void BinderTest::case3331(){}
void BinderTest::case3332(){}
void BinderTest::case3333(){}
void BinderTest::case3334(){}
void BinderTest::case3335(){}
void BinderTest::case3336(){}
void BinderTest::case3337(){}
void BinderTest::case3338(){}
void BinderTest::case3339(){}
void BinderTest::case3340(){}
void BinderTest::case3341(){}
void BinderTest::case3342(){}
void BinderTest::case3343(){}
void BinderTest::case3344(){}
void BinderTest::case3345(){}
void BinderTest::case3346(){}
void BinderTest::case3347(){}
void BinderTest::case3348(){}
void BinderTest::case3349(){}
void BinderTest::case3350(){}
void BinderTest::case3351(){}
void BinderTest::case3352(){}
void BinderTest::case3353(){}
void BinderTest::case3354(){}
void BinderTest::case3355(){}
void BinderTest::case3356(){}
void BinderTest::case3357(){}
void BinderTest::case3358(){}
void BinderTest::case3359(){}
void BinderTest::case3360(){}
void BinderTest::case3361(){}
void BinderTest::case3362(){}
void BinderTest::case3363(){}
void BinderTest::case3364(){}
void BinderTest::case3365(){}
void BinderTest::case3366(){}
void BinderTest::case3367(){}
void BinderTest::case3368(){}
void BinderTest::case3369(){}
void BinderTest::case3370(){}
void BinderTest::case3371(){}
void BinderTest::case3372(){}
void BinderTest::case3373(){}
void BinderTest::case3374(){}
void BinderTest::case3375(){}
void BinderTest::case3376(){}
void BinderTest::case3377(){}
void BinderTest::case3378(){}
void BinderTest::case3379(){}
void BinderTest::case3380(){}
void BinderTest::case3381(){}
void BinderTest::case3382(){}
void BinderTest::case3383(){}
void BinderTest::case3384(){}
void BinderTest::case3385(){}
void BinderTest::case3386(){}
void BinderTest::case3387(){}
void BinderTest::case3388(){}
void BinderTest::case3389(){}
void BinderTest::case3390(){}
void BinderTest::case3391(){}
void BinderTest::case3392(){}
void BinderTest::case3393(){}
void BinderTest::case3394(){}
void BinderTest::case3395(){}
void BinderTest::case3396(){}
void BinderTest::case3397(){}
void BinderTest::case3398(){}
void BinderTest::case3399(){}

void BinderTest::case3400(){}
void BinderTest::case3401(){}
void BinderTest::case3402(){}
void BinderTest::case3403(){}
void BinderTest::case3404(){}
void BinderTest::case3405(){}
void BinderTest::case3406(){}
void BinderTest::case3407(){}
void BinderTest::case3408(){}
void BinderTest::case3409(){}
void BinderTest::case3410(){}
void BinderTest::case3411(){}
void BinderTest::case3412(){}
void BinderTest::case3413(){}
void BinderTest::case3414(){}
void BinderTest::case3415(){}
void BinderTest::case3416(){}
void BinderTest::case3417(){}
void BinderTest::case3418(){}
void BinderTest::case3419(){}
void BinderTest::case3420(){}
void BinderTest::case3421(){}
void BinderTest::case3422(){}
void BinderTest::case3423(){}
void BinderTest::case3424(){}
void BinderTest::case3425(){}
void BinderTest::case3426(){}
void BinderTest::case3427(){}
void BinderTest::case3428(){}
void BinderTest::case3429(){}
void BinderTest::case3430(){}
void BinderTest::case3431(){}
void BinderTest::case3432(){}
void BinderTest::case3433(){}
void BinderTest::case3434(){}
void BinderTest::case3435(){}
void BinderTest::case3436(){}
void BinderTest::case3437(){}
void BinderTest::case3438(){}
void BinderTest::case3439(){}
void BinderTest::case3440(){}
void BinderTest::case3441(){}
void BinderTest::case3442(){}
void BinderTest::case3443(){}
void BinderTest::case3444(){}
void BinderTest::case3445(){}
void BinderTest::case3446(){}
void BinderTest::case3447(){}
void BinderTest::case3448(){}
void BinderTest::case3449(){}
void BinderTest::case3450(){}
void BinderTest::case3451(){}
void BinderTest::case3452(){}
void BinderTest::case3453(){}
void BinderTest::case3454(){}
void BinderTest::case3455(){}
void BinderTest::case3456(){}
void BinderTest::case3457(){}
void BinderTest::case3458(){}
void BinderTest::case3459(){}
void BinderTest::case3460(){}
void BinderTest::case3461(){}
void BinderTest::case3462(){}
void BinderTest::case3463(){}
void BinderTest::case3464(){}
void BinderTest::case3465(){}
void BinderTest::case3466(){}
void BinderTest::case3467(){}
void BinderTest::case3468(){}
void BinderTest::case3469(){}
void BinderTest::case3470(){}
void BinderTest::case3471(){}
void BinderTest::case3472(){}
void BinderTest::case3473(){}
void BinderTest::case3474(){}
void BinderTest::case3475(){}
void BinderTest::case3476(){}
void BinderTest::case3477(){}
void BinderTest::case3478(){}
void BinderTest::case3479(){}
void BinderTest::case3480(){}
void BinderTest::case3481(){}
void BinderTest::case3482(){}
void BinderTest::case3483(){}
void BinderTest::case3484(){}
void BinderTest::case3485(){}
void BinderTest::case3486(){}
void BinderTest::case3487(){}
void BinderTest::case3488(){}
void BinderTest::case3489(){}
void BinderTest::case3490(){}
void BinderTest::case3491(){}
void BinderTest::case3492(){}
void BinderTest::case3493(){}
void BinderTest::case3494(){}
void BinderTest::case3495(){}
void BinderTest::case3496(){}
void BinderTest::case3497(){}
void BinderTest::case3498(){}
void BinderTest::case3499(){}

void BinderTest::case3500(){}
void BinderTest::case3501(){}
void BinderTest::case3502(){}
void BinderTest::case3503(){}
void BinderTest::case3504(){}
void BinderTest::case3505(){}
void BinderTest::case3506(){}
void BinderTest::case3507(){}
void BinderTest::case3508(){}
void BinderTest::case3509(){}
void BinderTest::case3510(){}
void BinderTest::case3511(){}
void BinderTest::case3512(){}
void BinderTest::case3513(){}
void BinderTest::case3514(){}
void BinderTest::case3515(){}
void BinderTest::case3516(){}
void BinderTest::case3517(){}
void BinderTest::case3518(){}
void BinderTest::case3519(){}
void BinderTest::case3520(){}
void BinderTest::case3521(){}
void BinderTest::case3522(){}
void BinderTest::case3523(){}
void BinderTest::case3524(){}
void BinderTest::case3525(){}
void BinderTest::case3526(){}
void BinderTest::case3527(){}
void BinderTest::case3528(){}
void BinderTest::case3529(){}
void BinderTest::case3530(){}
void BinderTest::case3531(){}
void BinderTest::case3532(){}
void BinderTest::case3533(){}
void BinderTest::case3534(){}
void BinderTest::case3535(){}
void BinderTest::case3536(){}
void BinderTest::case3537(){}
void BinderTest::case3538(){}
void BinderTest::case3539(){}
void BinderTest::case3540(){}
void BinderTest::case3541(){}
void BinderTest::case3542(){}
void BinderTest::case3543(){}
void BinderTest::case3544(){}
void BinderTest::case3545(){}
void BinderTest::case3546(){}
void BinderTest::case3547(){}
void BinderTest::case3548(){}
void BinderTest::case3549(){}
void BinderTest::case3550(){}
void BinderTest::case3551(){}
void BinderTest::case3552(){}
void BinderTest::case3553(){}
void BinderTest::case3554(){}
void BinderTest::case3555(){}
void BinderTest::case3556(){}
void BinderTest::case3557(){}
void BinderTest::case3558(){}
void BinderTest::case3559(){}
void BinderTest::case3560(){}
void BinderTest::case3561(){}
void BinderTest::case3562(){}
void BinderTest::case3563(){}
void BinderTest::case3564(){}
void BinderTest::case3565(){}
void BinderTest::case3566(){}
void BinderTest::case3567(){}
void BinderTest::case3568(){}
void BinderTest::case3569(){}
void BinderTest::case3570(){}
void BinderTest::case3571(){}
void BinderTest::case3572(){}
void BinderTest::case3573(){}
void BinderTest::case3574(){}
void BinderTest::case3575(){}
void BinderTest::case3576(){}
void BinderTest::case3577(){}
void BinderTest::case3578(){}
void BinderTest::case3579(){}
void BinderTest::case3580(){}
void BinderTest::case3581(){}
void BinderTest::case3582(){}
void BinderTest::case3583(){}
void BinderTest::case3584(){}
void BinderTest::case3585(){}
void BinderTest::case3586(){}
void BinderTest::case3587(){}
void BinderTest::case3588(){}
void BinderTest::case3589(){}
void BinderTest::case3590(){}
void BinderTest::case3591(){}
void BinderTest::case3592(){}
void BinderTest::case3593(){}
void BinderTest::case3594(){}
void BinderTest::case3595(){}
void BinderTest::case3596(){}
void BinderTest::case3597(){}
void BinderTest::case3598(){}
void BinderTest::case3599(){}
