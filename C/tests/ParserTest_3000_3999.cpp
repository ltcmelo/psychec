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

#include "ParserTest.h"

#include "BinderTest.h"

#include "parser/Parser.h"
#include "parser/Unparser.h"

using namespace psy;
using namespace C;

void ParserTest::case3000()
{
    parseExpression("va_arg ( x , int )",
                    Expectation().AST( { VAArgumentExpression,
                                         IdentifierName,
                                         TypeName,
                                         BuiltinTypeSpecifier,
                                         AbstractDeclarator }));

}

void ParserTest::case3001()
{
    parseExpression("va_arg ( x , y )",
                    Expectation().AST( { VAArgumentExpression,
                                         IdentifierName,
                                         TypeName,
                                         TypedefName,
                                         AbstractDeclarator }));
}

void ParserTest::case3002()
{

}

void ParserTest::case3003()
{

}

void ParserTest::case3004()
{

}

void ParserTest::case3005()
{

}

void ParserTest::case3006()
{

}

void ParserTest::case3007()
{

}

void ParserTest::case3008()
{

}

void ParserTest::case3009()
{

}

void ParserTest::case3010()
{
    parseExpression("offsetof ( struct x , y )",
                    Expectation().AST( { OffsetOfExpression,
                                         TypeName,
                                         StructTypeSpecifier,
                                         AbstractDeclarator,
                                         OffsetOfDesignator }));
}

void ParserTest::case3011()
{
    parseExpression("offsetof ( struct x , y . z )",
                    Expectation().AST( { OffsetOfExpression,
                                         TypeName,
                                         StructTypeSpecifier,
                                         AbstractDeclarator,
                                         OffsetOfDesignator,
                                         FieldDesignator }));
}

void ParserTest::case3012()
{
    parseExpression("offsetof ( struct x , y [ 0 ] )",
                    Expectation().AST( { OffsetOfExpression,
                                         TypeName,
                                         StructTypeSpecifier,
                                         AbstractDeclarator,
                                         OffsetOfDesignator,
                                         ArrayDesignator,
                                         IntegerConstantExpression }));
}

void ParserTest::case3013()
{
    parseExpression("offsetof ( struct x , y  [ 0 ] . z )",
                    Expectation().AST( { OffsetOfExpression,
                                         TypeName,
                                         StructTypeSpecifier,
                                         AbstractDeclarator,
                                         OffsetOfDesignator,
                                         ArrayDesignator,
                                         IntegerConstantExpression,
                                         FieldDesignator }));
}

void ParserTest::case3014()
{

}

void ParserTest::case3015()
{

}

void ParserTest::case3016()
{

}

void ParserTest::case3017()
{

}

void ParserTest::case3018()
{

}

void ParserTest::case3019()
{

}

void ParserTest::case3020()
{
    parseExpression("__func__",
                    Expectation().AST( { PredefinedName }));
}

void ParserTest::case3021()
{
    parseExpression("(__func__)",
                    Expectation().AST( { ParenthesizedExpression,
                                         PredefinedName }));
}

void ParserTest::case3022()
{
    parseExpression("sizeof( __func__ )",
                    Expectation().AST( { SizeofExpression,
                                         ExpressionAsTypeReference,
                                         ParenthesizedExpression,
                                         PredefinedName }));
}

void ParserTest::case3023()
{
    parseExpression("sizeof __func__",
                    Expectation().AST( { SizeofExpression,
                                         ExpressionAsTypeReference,
                                         PredefinedName }));
}

void ParserTest::case3024()
{
    parseExpression("x = __func__",
                    Expectation().AST( { BasicAssignmentExpression,
                                         IdentifierName,
                                         PredefinedName }));
}

void ParserTest::case3025()
{

}

void ParserTest::case3026()
{

}

void ParserTest::case3027()
{

}

void ParserTest::case3028()
{

}

void ParserTest::case3029()
{

}

void ParserTest::case3030()
{

}

void ParserTest::case3031()
{

}

void ParserTest::case3032()
{

}

void ParserTest::case3033()
{

}

void ParserTest::case3034()
{

}

void ParserTest::case3035()
{

}

void ParserTest::case3036()
{

}

void ParserTest::case3037()
{

}

void ParserTest::case3038()
{

}

void ParserTest::case3039()
{

}

void ParserTest::case3040()
{

}

void ParserTest::case3041()
{

}

void ParserTest::case3042()
{

}

void ParserTest::case3043()
{

}

void ParserTest::case3044()
{

}

void ParserTest::case3045()
{

}

void ParserTest::case3046()
{

}

void ParserTest::case3047()
{

}

void ParserTest::case3048()
{

}

void ParserTest::case3049()
{

}

void ParserTest::case3050()
{

}

void ParserTest::case3051()
{

}

void ParserTest::case3052()
{

}

void ParserTest::case3053()
{

}

void ParserTest::case3054()
{

}

void ParserTest::case3055()
{

}

void ParserTest::case3056()
{

}

void ParserTest::case3057()
{

}

void ParserTest::case3058()
{

}

void ParserTest::case3059()
{

}

void ParserTest::case3060()
{

}

void ParserTest::case3061()
{

}

void ParserTest::case3062()
{

}

void ParserTest::case3063()
{
}

void ParserTest::case3064()
{
}

void ParserTest::case3065()
{

}

void ParserTest::case3066()
{

}

void ParserTest::case3067()
{

}

void ParserTest::case3068()
{
}

void ParserTest::case3069()
{
}

void ParserTest::case3070()
{
}

void ParserTest::case3071()
{
}

void ParserTest::case3072()
{
}

void ParserTest::case3073()
{
}

void ParserTest::case3074() {}
void ParserTest::case3075() {}
void ParserTest::case3076() {}
void ParserTest::case3077() {}
void ParserTest::case3078() {}
void ParserTest::case3079() {}
void ParserTest::case3080() {}
void ParserTest::case3081() {}
void ParserTest::case3082() {}
void ParserTest::case3083() {}
void ParserTest::case3084() {}
void ParserTest::case3085() {}
void ParserTest::case3086() {}
void ParserTest::case3087() {}
void ParserTest::case3088() {}
void ParserTest::case3089() {}
void ParserTest::case3090() {}
void ParserTest::case3091() {}
void ParserTest::case3092() {}
void ParserTest::case3093() {}
void ParserTest::case3094() {}
void ParserTest::case3095() {}
void ParserTest::case3096() {}
void ParserTest::case3097() {}
void ParserTest::case3098() {}
void ParserTest::case3099() {}

void ParserTest::case3100()
{

}

void ParserTest::case3101()
{

}

void ParserTest::case3102()
{

}

void ParserTest::case3103()
{

}

void ParserTest::case3104()
{
}

void ParserTest::case3105()
{
}

void ParserTest::case3106()
{
}

void ParserTest::case3107()
{
}

void ParserTest::case3108() {}
void ParserTest::case3109() {}
void ParserTest::case3110() {}
void ParserTest::case3111() {}
void ParserTest::case3112() {}
void ParserTest::case3113() {}
void ParserTest::case3114() {}
void ParserTest::case3115() {}
void ParserTest::case3116() {}
void ParserTest::case3117() {}
void ParserTest::case3118() {}
void ParserTest::case3119() {}
void ParserTest::case3120() {}
void ParserTest::case3121() {}
void ParserTest::case3122() {}
void ParserTest::case3123() {}
void ParserTest::case3124() {}
void ParserTest::case3125() {}
void ParserTest::case3126() {}
void ParserTest::case3127() {}
void ParserTest::case3128() {}
void ParserTest::case3129() {}
void ParserTest::case3130() {}
void ParserTest::case3131() {}
void ParserTest::case3132() {}
void ParserTest::case3133() {}
void ParserTest::case3134() {}
void ParserTest::case3135() {}
void ParserTest::case3136() {}
void ParserTest::case3137() {}
void ParserTest::case3138() {}
void ParserTest::case3139() {}
void ParserTest::case3140() {}
void ParserTest::case3141() {}
void ParserTest::case3142() {}
void ParserTest::case3143() {}
void ParserTest::case3144() {}
void ParserTest::case3145() {}
void ParserTest::case3146() {}
void ParserTest::case3147() {}
void ParserTest::case3148() {}
void ParserTest::case3149() {}
void ParserTest::case3150() {}
void ParserTest::case3151() {}
void ParserTest::case3152() {}
void ParserTest::case3153() {}
void ParserTest::case3154() {}
void ParserTest::case3155() {}
void ParserTest::case3156() {}
void ParserTest::case3157() {}
void ParserTest::case3158() {}
void ParserTest::case3159() {}
void ParserTest::case3160() {}
void ParserTest::case3161() {}
void ParserTest::case3162() {}
void ParserTest::case3163() {}
void ParserTest::case3164() {}
void ParserTest::case3165() {}
void ParserTest::case3166() {}
void ParserTest::case3167() {}
void ParserTest::case3168() {}
void ParserTest::case3169() {}
void ParserTest::case3170() {}
void ParserTest::case3171() {}
void ParserTest::case3172() {}
void ParserTest::case3173() {}
void ParserTest::case3174() {}
void ParserTest::case3175() {}
void ParserTest::case3176() {}
void ParserTest::case3177() {}
void ParserTest::case3178() {}
void ParserTest::case3179() {}
void ParserTest::case3180() {}
void ParserTest::case3181() {}
void ParserTest::case3182() {}
void ParserTest::case3183() {}
void ParserTest::case3184() {}
void ParserTest::case3185() {}
void ParserTest::case3186() {}
void ParserTest::case3187() {}
void ParserTest::case3188() {}
void ParserTest::case3189() {}
void ParserTest::case3190() {}
void ParserTest::case3191() {}
void ParserTest::case3192() {}
void ParserTest::case3193() {}
void ParserTest::case3194() {}
void ParserTest::case3195() {}
void ParserTest::case3196() {}
void ParserTest::case3197() {}
void ParserTest::case3198() {}
void ParserTest::case3199() {}

void ParserTest::case3200()
{
}

void ParserTest::case3201()
{
}

void ParserTest::case3202()
{
}

void ParserTest::case3203()
{
}

void ParserTest::case3204()
{
}

void ParserTest::case3205()
{
}

void ParserTest::case3206()
{
}

void ParserTest::case3207()
{
}
void ParserTest::case3208() {}
void ParserTest::case3209() {}
void ParserTest::case3210() {}
void ParserTest::case3211() {}
void ParserTest::case3212() {}
void ParserTest::case3213() {}
void ParserTest::case3214() {}
void ParserTest::case3215() {}
void ParserTest::case3216() {}
void ParserTest::case3217() {}
void ParserTest::case3218() {}
void ParserTest::case3219() {}
void ParserTest::case3220() {}
void ParserTest::case3221() {}
void ParserTest::case3222() {}
void ParserTest::case3223() {}
void ParserTest::case3224() {}
void ParserTest::case3225() {}
void ParserTest::case3226() {}
void ParserTest::case3227() {}
void ParserTest::case3228() {}
void ParserTest::case3229() {}
void ParserTest::case3230() {}
void ParserTest::case3231() {}
void ParserTest::case3232() {}
void ParserTest::case3233() {}
void ParserTest::case3234() {}
void ParserTest::case3235() {}
void ParserTest::case3236() {}
void ParserTest::case3237() {}
void ParserTest::case3238() {}
void ParserTest::case3239() {}
void ParserTest::case3240() {}
void ParserTest::case3241() {}
void ParserTest::case3242() {}
void ParserTest::case3243() {}
void ParserTest::case3244() {}
void ParserTest::case3245() {}
void ParserTest::case3246() {}
void ParserTest::case3247() {}
void ParserTest::case3248() {}
void ParserTest::case3249() {}
void ParserTest::case3250() {}
void ParserTest::case3251() {}
void ParserTest::case3252() {}
void ParserTest::case3253() {}
void ParserTest::case3254() {}
void ParserTest::case3255() {}
void ParserTest::case3256() {}
void ParserTest::case3257() {}
void ParserTest::case3258() {}
void ParserTest::case3259() {}
void ParserTest::case3260() {}
void ParserTest::case3261() {}
void ParserTest::case3262() {}
void ParserTest::case3263() {}
void ParserTest::case3264() {}
void ParserTest::case3265() {}
void ParserTest::case3266() {}
void ParserTest::case3267() {}
void ParserTest::case3268() {}
void ParserTest::case3269() {}
void ParserTest::case3270() {}
void ParserTest::case3271() {}
void ParserTest::case3272() {}
void ParserTest::case3273() {}
void ParserTest::case3274() {}
void ParserTest::case3275() {}
void ParserTest::case3276() {}
void ParserTest::case3277() {}
void ParserTest::case3278() {}
void ParserTest::case3279() {}
void ParserTest::case3280() {}
void ParserTest::case3281() {}
void ParserTest::case3282() {}
void ParserTest::case3283() {}
void ParserTest::case3284() {}
void ParserTest::case3285() {}
void ParserTest::case3286() {}
void ParserTest::case3287() {}
void ParserTest::case3288() {}
void ParserTest::case3289() {}
void ParserTest::case3290() {}
void ParserTest::case3291() {}
void ParserTest::case3292() {}
void ParserTest::case3293() {}
void ParserTest::case3294() {}
void ParserTest::case3295() {}
void ParserTest::case3296() {}
void ParserTest::case3297() {}
void ParserTest::case3298() {}
void ParserTest::case3299() {}

void ParserTest::case3300()
{
}

void ParserTest::case3301()
{
}

void ParserTest::case3302()
{
}

void ParserTest::case3303()
{
}

void ParserTest::case3304()
{
}

void ParserTest::case3305()
{
}

void ParserTest::case3306()
{
}

void ParserTest::case3307()
{
}

void ParserTest::case3308() {}
void ParserTest::case3309() {}
void ParserTest::case3310() {}
void ParserTest::case3311() {}
void ParserTest::case3312() {}
void ParserTest::case3313() {}
void ParserTest::case3314() {}
void ParserTest::case3315() {}
void ParserTest::case3316() {}
void ParserTest::case3317() {}
void ParserTest::case3318() {}
void ParserTest::case3319() {}
void ParserTest::case3320() {}
void ParserTest::case3321() {}
void ParserTest::case3322() {}
void ParserTest::case3323() {}
void ParserTest::case3324() {}
void ParserTest::case3325() {}
void ParserTest::case3326() {}
void ParserTest::case3327() {}
void ParserTest::case3328() {}
void ParserTest::case3329() {}
void ParserTest::case3330() {}
void ParserTest::case3331() {}
void ParserTest::case3332() {}
void ParserTest::case3333() {}
void ParserTest::case3334() {}
void ParserTest::case3335() {}
void ParserTest::case3336() {}
void ParserTest::case3337() {}
void ParserTest::case3338() {}
void ParserTest::case3339() {}
void ParserTest::case3340() {}
void ParserTest::case3341() {}
void ParserTest::case3342() {}
void ParserTest::case3343() {}
void ParserTest::case3344() {}
void ParserTest::case3345() {}
void ParserTest::case3346() {}
void ParserTest::case3347() {}
void ParserTest::case3348() {}
void ParserTest::case3349() {}
void ParserTest::case3350() {}
void ParserTest::case3351() {}
void ParserTest::case3352() {}
void ParserTest::case3353() {}
void ParserTest::case3354() {}
void ParserTest::case3355() {}
void ParserTest::case3356() {}
void ParserTest::case3357() {}
void ParserTest::case3358() {}
void ParserTest::case3359() {}
void ParserTest::case3360() {}
void ParserTest::case3361() {}
void ParserTest::case3362() {}
void ParserTest::case3363() {}
void ParserTest::case3364() {}
void ParserTest::case3365() {}
void ParserTest::case3366() {}
void ParserTest::case3367() {}
void ParserTest::case3368() {}
void ParserTest::case3369() {}
void ParserTest::case3370() {}
void ParserTest::case3371() {}
void ParserTest::case3372() {}
void ParserTest::case3373() {}
void ParserTest::case3374() {}
void ParserTest::case3375() {}
void ParserTest::case3376() {}
void ParserTest::case3377() {}
void ParserTest::case3378() {}
void ParserTest::case3379() {}
void ParserTest::case3380() {}
void ParserTest::case3381() {}
void ParserTest::case3382() {}
void ParserTest::case3383() {}
void ParserTest::case3384() {}
void ParserTest::case3385() {}
void ParserTest::case3386() {}
void ParserTest::case3387() {}
void ParserTest::case3388() {}
void ParserTest::case3389() {}
void ParserTest::case3390() {}
void ParserTest::case3391() {}
void ParserTest::case3392() {}
void ParserTest::case3393() {}
void ParserTest::case3394() {}
void ParserTest::case3395() {}
void ParserTest::case3396() {}
void ParserTest::case3397() {}
void ParserTest::case3398() {}
void ParserTest::case3399() {}

void ParserTest::case3400()
{
}

void ParserTest::case3401()
{
}

void ParserTest::case3402()
{
}

void ParserTest::case3403()
{
}

void ParserTest::case3404()
{
}
void ParserTest::case3405() {}
void ParserTest::case3406() {}
void ParserTest::case3407() {}
void ParserTest::case3408() {}
void ParserTest::case3409() {}
void ParserTest::case3410() {}
void ParserTest::case3411() {}
void ParserTest::case3412() {}
void ParserTest::case3413() {}
void ParserTest::case3414() {}
void ParserTest::case3415() {}
void ParserTest::case3416() {}
void ParserTest::case3417() {}
void ParserTest::case3418() {}
void ParserTest::case3419() {}
void ParserTest::case3420() {}
void ParserTest::case3421() {}
void ParserTest::case3422() {}
void ParserTest::case3423() {}
void ParserTest::case3424() {}
void ParserTest::case3425() {}
void ParserTest::case3426() {}
void ParserTest::case3427() {}
void ParserTest::case3428() {}
void ParserTest::case3429() {}
void ParserTest::case3430() {}
void ParserTest::case3431() {}
void ParserTest::case3432() {}
void ParserTest::case3433() {}
void ParserTest::case3434() {}
void ParserTest::case3435() {}
void ParserTest::case3436() {}
void ParserTest::case3437() {}
void ParserTest::case3438() {}
void ParserTest::case3439() {}
void ParserTest::case3440() {}
void ParserTest::case3441() {}
void ParserTest::case3442() {}
void ParserTest::case3443() {}
void ParserTest::case3444() {}
void ParserTest::case3445() {}
void ParserTest::case3446() {}
void ParserTest::case3447() {}
void ParserTest::case3448() {}
void ParserTest::case3449() {}
void ParserTest::case3450() {}
void ParserTest::case3451() {}
void ParserTest::case3452() {}
void ParserTest::case3453() {}
void ParserTest::case3454() {}
void ParserTest::case3455() {}
void ParserTest::case3456() {}
void ParserTest::case3457() {}
void ParserTest::case3458() {}
void ParserTest::case3459() {}
void ParserTest::case3460() {}
void ParserTest::case3461() {}
void ParserTest::case3462() {}
void ParserTest::case3463() {}
void ParserTest::case3464() {}
void ParserTest::case3465() {}
void ParserTest::case3466() {}
void ParserTest::case3467() {}
void ParserTest::case3468() {}
void ParserTest::case3469() {}
void ParserTest::case3470() {}
void ParserTest::case3471() {}
void ParserTest::case3472() {}
void ParserTest::case3473() {}
void ParserTest::case3474() {}
void ParserTest::case3475() {}
void ParserTest::case3476() {}
void ParserTest::case3477() {}
void ParserTest::case3478() {}
void ParserTest::case3479() {}
void ParserTest::case3480() {}
void ParserTest::case3481() {}
void ParserTest::case3482() {}
void ParserTest::case3483() {}
void ParserTest::case3484() {}
void ParserTest::case3485() {}
void ParserTest::case3486() {}
void ParserTest::case3487() {}
void ParserTest::case3488() {}
void ParserTest::case3489() {}
void ParserTest::case3490() {}
void ParserTest::case3491() {}
void ParserTest::case3492() {}
void ParserTest::case3493() {}
void ParserTest::case3494() {}
void ParserTest::case3495() {}
void ParserTest::case3496() {}
void ParserTest::case3497() {}
void ParserTest::case3498() {}
void ParserTest::case3499() {}

void ParserTest::case3500() {}
void ParserTest::case3501() {}
void ParserTest::case3502() {}
void ParserTest::case3503() {}
void ParserTest::case3504() {}
void ParserTest::case3505() {}
void ParserTest::case3506() {}
void ParserTest::case3507() {}
void ParserTest::case3508() {}
void ParserTest::case3509() {}
void ParserTest::case3510() {}
void ParserTest::case3511() {}
void ParserTest::case3512() {}
void ParserTest::case3513() {}
void ParserTest::case3514() {}
void ParserTest::case3515() {}
void ParserTest::case3516() {}
void ParserTest::case3517() {}
void ParserTest::case3518() {}
void ParserTest::case3519() {}
void ParserTest::case3520() {}
void ParserTest::case3521() {}
void ParserTest::case3522() {}
void ParserTest::case3523() {}
void ParserTest::case3524() {}
void ParserTest::case3525() {}
void ParserTest::case3526() {}
void ParserTest::case3527() {}
void ParserTest::case3528() {}
void ParserTest::case3529() {}
void ParserTest::case3530() {}
void ParserTest::case3531() {}
void ParserTest::case3532() {}
void ParserTest::case3533() {}
void ParserTest::case3534() {}
void ParserTest::case3535() {}
void ParserTest::case3536() {}
void ParserTest::case3537() {}
void ParserTest::case3538() {}
void ParserTest::case3539() {}
void ParserTest::case3540() {}
void ParserTest::case3541() {}
void ParserTest::case3542() {}
void ParserTest::case3543() {}
void ParserTest::case3544() {}
void ParserTest::case3545() {}
void ParserTest::case3546() {}
void ParserTest::case3547() {}
void ParserTest::case3548() {}
void ParserTest::case3549() {}
void ParserTest::case3550() {}
void ParserTest::case3551() {}
void ParserTest::case3552() {}
void ParserTest::case3553() {}
void ParserTest::case3554() {}
void ParserTest::case3555() {}
void ParserTest::case3556() {}
void ParserTest::case3557() {}
void ParserTest::case3558() {}
void ParserTest::case3559() {}
void ParserTest::case3560() {}
void ParserTest::case3561() {}
void ParserTest::case3562() {}
void ParserTest::case3563() {}
void ParserTest::case3564() {}
void ParserTest::case3565() {}
void ParserTest::case3566() {}
void ParserTest::case3567() {}
void ParserTest::case3568() {}
void ParserTest::case3569() {}
void ParserTest::case3570() {}
void ParserTest::case3571() {}
void ParserTest::case3572() {}
void ParserTest::case3573() {}
void ParserTest::case3574() {}
void ParserTest::case3575() {}
void ParserTest::case3576() {}
void ParserTest::case3577() {}
void ParserTest::case3578() {}
void ParserTest::case3579() {}
void ParserTest::case3580() {}
void ParserTest::case3581() {}
void ParserTest::case3582() {}
void ParserTest::case3583() {}
void ParserTest::case3584() {}
void ParserTest::case3585() {}
void ParserTest::case3586() {}
void ParserTest::case3587() {}
void ParserTest::case3588() {}
void ParserTest::case3589() {}
void ParserTest::case3590() {}
void ParserTest::case3591() {}
void ParserTest::case3592() {}
void ParserTest::case3593() {}
void ParserTest::case3594() {}
void ParserTest::case3595() {}
void ParserTest::case3596() {}
void ParserTest::case3597() {}
void ParserTest::case3598() {}
void ParserTest::case3599() {}

void ParserTest::case3600() {}
void ParserTest::case3601() {}
void ParserTest::case3602() {}
void ParserTest::case3603() {}
void ParserTest::case3604() {}
void ParserTest::case3605() {}
void ParserTest::case3606() {}
void ParserTest::case3607() {}
void ParserTest::case3608() {}
void ParserTest::case3609() {}
void ParserTest::case3610() {}
void ParserTest::case3611() {}
void ParserTest::case3612() {}
void ParserTest::case3613() {}
void ParserTest::case3614() {}
void ParserTest::case3615() {}
void ParserTest::case3616() {}
void ParserTest::case3617() {}
void ParserTest::case3618() {}
void ParserTest::case3619() {}
void ParserTest::case3620() {}
void ParserTest::case3621() {}
void ParserTest::case3622() {}
void ParserTest::case3623() {}
void ParserTest::case3624() {}
void ParserTest::case3625() {}
void ParserTest::case3626() {}
void ParserTest::case3627() {}
void ParserTest::case3628() {}
void ParserTest::case3629() {}
void ParserTest::case3630() {}
void ParserTest::case3631() {}
void ParserTest::case3632() {}
void ParserTest::case3633() {}
void ParserTest::case3634() {}
void ParserTest::case3635() {}
void ParserTest::case3636() {}
void ParserTest::case3637() {}
void ParserTest::case3638() {}
void ParserTest::case3639() {}
void ParserTest::case3640() {}
void ParserTest::case3641() {}
void ParserTest::case3642() {}
void ParserTest::case3643() {}
void ParserTest::case3644() {}
void ParserTest::case3645() {}
void ParserTest::case3646() {}
void ParserTest::case3647() {}
void ParserTest::case3648() {}
void ParserTest::case3649() {}
void ParserTest::case3650() {}
void ParserTest::case3651() {}
void ParserTest::case3652() {}
void ParserTest::case3653() {}
void ParserTest::case3654() {}
void ParserTest::case3655() {}
void ParserTest::case3656() {}
void ParserTest::case3657() {}
void ParserTest::case3658() {}
void ParserTest::case3659() {}
void ParserTest::case3660() {}
void ParserTest::case3661() {}
void ParserTest::case3662() {}
void ParserTest::case3663() {}
void ParserTest::case3664() {}
void ParserTest::case3665() {}
void ParserTest::case3666() {}
void ParserTest::case3667() {}
void ParserTest::case3668() {}
void ParserTest::case3669() {}
void ParserTest::case3670() {}
void ParserTest::case3671() {}
void ParserTest::case3672() {}
void ParserTest::case3673() {}
void ParserTest::case3674() {}
void ParserTest::case3675() {}
void ParserTest::case3676() {}
void ParserTest::case3677() {}
void ParserTest::case3678() {}
void ParserTest::case3679() {}
void ParserTest::case3680() {}
void ParserTest::case3681() {}
void ParserTest::case3682() {}
void ParserTest::case3683() {}
void ParserTest::case3684() {}
void ParserTest::case3685() {}
void ParserTest::case3686() {}
void ParserTest::case3687() {}
void ParserTest::case3688() {}
void ParserTest::case3689() {}
void ParserTest::case3690() {}
void ParserTest::case3691() {}
void ParserTest::case3692() {}
void ParserTest::case3693() {}
void ParserTest::case3694() {}
void ParserTest::case3695() {}
void ParserTest::case3696() {}
void ParserTest::case3697() {}
void ParserTest::case3698() {}
void ParserTest::case3699() {}

void ParserTest::case3700() {}
void ParserTest::case3701() {}
void ParserTest::case3702() {}
void ParserTest::case3703() {}
void ParserTest::case3704() {}
void ParserTest::case3705() {}
void ParserTest::case3706() {}
void ParserTest::case3707() {}
void ParserTest::case3708() {}
void ParserTest::case3709() {}
void ParserTest::case3710() {}
void ParserTest::case3711() {}
void ParserTest::case3712() {}
void ParserTest::case3713() {}
void ParserTest::case3714() {}
void ParserTest::case3715() {}
void ParserTest::case3716() {}
void ParserTest::case3717() {}
void ParserTest::case3718() {}
void ParserTest::case3719() {}
void ParserTest::case3720() {}
void ParserTest::case3721() {}
void ParserTest::case3722() {}
void ParserTest::case3723() {}
void ParserTest::case3724() {}
void ParserTest::case3725() {}
void ParserTest::case3726() {}
void ParserTest::case3727() {}
void ParserTest::case3728() {}
void ParserTest::case3729() {}
void ParserTest::case3730() {}
void ParserTest::case3731() {}
void ParserTest::case3732() {}
void ParserTest::case3733() {}
void ParserTest::case3734() {}
void ParserTest::case3735() {}
void ParserTest::case3736() {}
void ParserTest::case3737() {}
void ParserTest::case3738() {}
void ParserTest::case3739() {}
void ParserTest::case3740() {}
void ParserTest::case3741() {}
void ParserTest::case3742() {}
void ParserTest::case3743() {}
void ParserTest::case3744() {}
void ParserTest::case3745() {}
void ParserTest::case3746() {}
void ParserTest::case3747() {}
void ParserTest::case3748() {}
void ParserTest::case3749() {}
void ParserTest::case3750() {}
void ParserTest::case3751() {}
void ParserTest::case3752() {}
void ParserTest::case3753() {}
void ParserTest::case3754() {}
void ParserTest::case3755() {}
void ParserTest::case3756() {}
void ParserTest::case3757() {}
void ParserTest::case3758() {}
void ParserTest::case3759() {}
void ParserTest::case3760() {}
void ParserTest::case3761() {}
void ParserTest::case3762() {}
void ParserTest::case3763() {}
void ParserTest::case3764() {}
void ParserTest::case3765() {}
void ParserTest::case3766() {}
void ParserTest::case3767() {}
void ParserTest::case3768() {}
void ParserTest::case3769() {}
void ParserTest::case3770() {}
void ParserTest::case3771() {}
void ParserTest::case3772() {}
void ParserTest::case3773() {}
void ParserTest::case3774() {}
void ParserTest::case3775() {}
void ParserTest::case3776() {}
void ParserTest::case3777() {}
void ParserTest::case3778() {}
void ParserTest::case3779() {}
void ParserTest::case3780() {}
void ParserTest::case3781() {}
void ParserTest::case3782() {}
void ParserTest::case3783() {}
void ParserTest::case3784() {}
void ParserTest::case3785() {}
void ParserTest::case3786() {}
void ParserTest::case3787() {}
void ParserTest::case3788() {}
void ParserTest::case3789() {}
void ParserTest::case3790() {}
void ParserTest::case3791() {}
void ParserTest::case3792() {}
void ParserTest::case3793() {}
void ParserTest::case3794() {}
void ParserTest::case3795() {}
void ParserTest::case3796() {}
void ParserTest::case3797() {}
void ParserTest::case3798() {}
void ParserTest::case3799() {}

void ParserTest::case3800() {}
void ParserTest::case3801() {}
void ParserTest::case3802() {}
void ParserTest::case3803() {}
void ParserTest::case3804() {}
void ParserTest::case3805() {}
void ParserTest::case3806() {}
void ParserTest::case3807() {}
void ParserTest::case3808() {}
void ParserTest::case3809() {}
void ParserTest::case3810() {}
void ParserTest::case3811() {}
void ParserTest::case3812() {}
void ParserTest::case3813() {}
void ParserTest::case3814() {}
void ParserTest::case3815() {}
void ParserTest::case3816() {}
void ParserTest::case3817() {}
void ParserTest::case3818() {}
void ParserTest::case3819() {}
void ParserTest::case3820() {}
void ParserTest::case3821() {}
void ParserTest::case3822() {}
void ParserTest::case3823() {}
void ParserTest::case3824() {}
void ParserTest::case3825() {}
void ParserTest::case3826() {}
void ParserTest::case3827() {}
void ParserTest::case3828() {}
void ParserTest::case3829() {}
void ParserTest::case3830() {}
void ParserTest::case3831() {}
void ParserTest::case3832() {}
void ParserTest::case3833() {}
void ParserTest::case3834() {}
void ParserTest::case3835() {}
void ParserTest::case3836() {}
void ParserTest::case3837() {}
void ParserTest::case3838() {}
void ParserTest::case3839() {}
void ParserTest::case3840() {}
void ParserTest::case3841() {}
void ParserTest::case3842() {}
void ParserTest::case3843() {}
void ParserTest::case3844() {}
void ParserTest::case3845() {}
void ParserTest::case3846() {}
void ParserTest::case3847() {}
void ParserTest::case3848() {}
void ParserTest::case3849() {}
void ParserTest::case3850() {}
void ParserTest::case3851() {}
void ParserTest::case3852() {}
void ParserTest::case3853() {}
void ParserTest::case3854() {}
void ParserTest::case3855() {}
void ParserTest::case3856() {}
void ParserTest::case3857() {}
void ParserTest::case3858() {}
void ParserTest::case3859() {}
void ParserTest::case3860() {}
void ParserTest::case3861() {}
void ParserTest::case3862() {}
void ParserTest::case3863() {}
void ParserTest::case3864() {}
void ParserTest::case3865() {}
void ParserTest::case3866() {}
void ParserTest::case3867() {}
void ParserTest::case3868() {}
void ParserTest::case3869() {}
void ParserTest::case3870() {}
void ParserTest::case3871() {}
void ParserTest::case3872() {}
void ParserTest::case3873() {}
void ParserTest::case3874() {}
void ParserTest::case3875() {}
void ParserTest::case3876() {}
void ParserTest::case3877() {}
void ParserTest::case3878() {}
void ParserTest::case3879() {}
void ParserTest::case3880() {}
void ParserTest::case3881() {}
void ParserTest::case3882() {}
void ParserTest::case3883() {}
void ParserTest::case3884() {}
void ParserTest::case3885() {}
void ParserTest::case3886() {}
void ParserTest::case3887() {}
void ParserTest::case3888() {}
void ParserTest::case3889() {}
void ParserTest::case3890() {}
void ParserTest::case3891() {}
void ParserTest::case3892() {}
void ParserTest::case3893() {}
void ParserTest::case3894() {}
void ParserTest::case3895() {}
void ParserTest::case3896() {}
void ParserTest::case3897() {}
void ParserTest::case3898() {}
void ParserTest::case3899() {}

void ParserTest::case3900() {}
void ParserTest::case3901() {}
void ParserTest::case3902() {}
void ParserTest::case3903() {}
void ParserTest::case3904() {}
void ParserTest::case3905() {}
void ParserTest::case3906() {}
void ParserTest::case3907() {}
void ParserTest::case3908() {}
void ParserTest::case3909() {}
void ParserTest::case3910() {}
void ParserTest::case3911() {}
void ParserTest::case3912() {}
void ParserTest::case3913() {}
void ParserTest::case3914() {}
void ParserTest::case3915() {}
void ParserTest::case3916() {}
void ParserTest::case3917() {}
void ParserTest::case3918() {}
void ParserTest::case3919() {}
void ParserTest::case3920() {}
void ParserTest::case3921() {}
void ParserTest::case3922() {}
void ParserTest::case3923() {}
void ParserTest::case3924() {}
void ParserTest::case3925() {}
void ParserTest::case3926() {}
void ParserTest::case3927() {}
void ParserTest::case3928() {}
void ParserTest::case3929() {}
void ParserTest::case3930() {}
void ParserTest::case3931() {}
void ParserTest::case3932() {}
void ParserTest::case3933() {}
void ParserTest::case3934() {}
void ParserTest::case3935() {}
void ParserTest::case3936() {}
void ParserTest::case3937() {}
void ParserTest::case3938() {}
void ParserTest::case3939() {}
void ParserTest::case3940() {}
void ParserTest::case3941() {}
void ParserTest::case3942() {}
void ParserTest::case3943() {}
void ParserTest::case3944() {}
void ParserTest::case3945() {}
void ParserTest::case3946() {}
void ParserTest::case3947() {}
void ParserTest::case3948() {}
void ParserTest::case3949() {}
void ParserTest::case3950() {}
void ParserTest::case3951() {}
void ParserTest::case3952() {}
void ParserTest::case3953() {}
void ParserTest::case3954() {}
void ParserTest::case3955() {}
void ParserTest::case3956() {}
void ParserTest::case3957() {}
void ParserTest::case3958() {}
void ParserTest::case3959() {}
void ParserTest::case3960() {}
void ParserTest::case3961() {}
void ParserTest::case3962() {}
void ParserTest::case3963() {}
void ParserTest::case3964() {}
void ParserTest::case3965() {}
void ParserTest::case3966() {}
void ParserTest::case3967() {}
void ParserTest::case3968() {}
void ParserTest::case3969() {}
void ParserTest::case3970() {}
void ParserTest::case3971() {}
void ParserTest::case3972() {}
void ParserTest::case3973() {}
void ParserTest::case3974() {}
void ParserTest::case3975() {}
void ParserTest::case3976() {}
void ParserTest::case3977() {}
void ParserTest::case3978() {}
void ParserTest::case3979() {}
void ParserTest::case3980() {}
void ParserTest::case3981() {}
void ParserTest::case3982() {}
void ParserTest::case3983() {}
void ParserTest::case3984() {}
void ParserTest::case3985() {}
void ParserTest::case3986() {}
void ParserTest::case3987() {}
void ParserTest::case3988() {}
void ParserTest::case3989() {}
void ParserTest::case3990() {}
void ParserTest::case3991() {}
void ParserTest::case3992() {}
void ParserTest::case3993() {}
void ParserTest::case3994() {}
void ParserTest::case3995() {}
void ParserTest::case3996() {}
void ParserTest::case3997() {}
void ParserTest::case3998() {}
void ParserTest::case3999() {}
