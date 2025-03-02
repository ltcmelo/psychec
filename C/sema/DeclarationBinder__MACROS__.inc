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
# define DBG_DECL_STACK_ENABLED
# define DBG_TY_STACK_ENABLED
#endif

#ifdef DBG_DECL_STACK_ENABLED
# define DBG_DECL_STACK(decl) \
    do { \
        std::cout << __func__ << " " << std::flush; \
        std::cout << decl->kind() << std::endl; \
    } while (0)
#else
# define DBG_DECL_STACK(sym) do {} while (0)
#endif

#ifdef DBG_TY_STACK_ENABLED
# define DBG_TY_STACK(ty) \
    do { \
        std::cout << __func__ << " " << std::flush; \
        std::cout << ty << std::endl; \
    } while (0)
#else
# define DBG_TY_STACK(sym) do {} while (0)
#endif

#define VALID_TOP(STACK, RET) \
    PSY_ASSERT_2(!STACK.empty(), RET); \
    PSY_ASSERT_2(STACK.top(), RET);
#define AT_TOP(V, STACK, RET) \
    VALID_TOP(STACK, RET); \
    V = STACK.top();
#define TY_AT_TOP(V, E) AT_TOP(V, tys_, return E)
#define SCOPE_AT_TOP(V, E) AT_TOP(V, scopes_, return E)
#define DECL_AT_TOP(V, E) AT_TOP(V, decls_, return E)
