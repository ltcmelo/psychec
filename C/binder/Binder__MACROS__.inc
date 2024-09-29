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

//#define DBG_BINDER
#ifdef DBG_BINDER
# define DBG_SYM_STACK_ENABLED
# define DBG_TY_STACK_ENABLED
#endif

#ifdef DBG_SYM_STACK_ENABLED
# define DBG_SYM_STACK(sym) \
    do { \
        std::cout << __func__ << " " << std::flush; \
        std::cout << to_string(sym->kind()) << std::endl; \
    } while (0)
#else
# define DBG_SYM_STACK(sym) do {} while (0)
#endif

#ifdef DBG_TY_STACK_ENABLED
# define DBG_TY_STACK(ty) \
    do { \
        std::cout << __func__ << " " << std::flush; \
        std::cout << to_string(*ty) << std::endl; \
    } while (0)
#else
# define DBG_TY_STACK(sym) do {} while (0)
#endif

#define VALID_TOP(STACK, RET) \
    PSY_ASSERT_2(!STACK.empty(), RET); \
    PSY_ASSERT_2(STACK.top(), RET);
#define TOP(V, STACK, RET) \
    VALID_TOP(STACK, RET); \
    V = STACK.top();
#define DECL_TOP(V, STACK, RET) \
    VALID_TOP(STACK, RET); \
    auto V = STACK.top();
#define TOP_SYM_retQ(V) TOP(V, syms_, return Action::Quit)
#define TOP_SYM_ret(V) TOP(V, syms_, return)
#define TOP_TY_retQ(V) TOP(V, tys_, return Action::Quit)
#define TOP_TY_ret(V) TOP(V, tys_, return)
#define TOP_SCOPE_retQ(V) TOP(V, scopes_, return Action::Quit)
#define TOP_SCOPE_ret(V) TOP(V, scopes_, return)
#define DECL_TOP_SYM_retQ(V) DECL_TOP(V, syms_, return Action::Quit)
#define DECL_TOP_SYM_ret(V) DECL_TOP(V, syms_, return)
#define DECL_TOP_TY_retQ(V) DECL_TOP(V, tys_, return Action::Quit)
#define DECL_TOP_TY_ret(V) DECL_TOP(V, tys_, return)
#define DECL_TOP_SCOPE_retQ(V) DECL_TOP(V, scopes_, return Action::Quit)
#define DECL_TOP_SCOPE_ret(V) DECL_TOP(V, scopes_, return)
