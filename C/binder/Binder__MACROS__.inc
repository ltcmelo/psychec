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

//#define DEBUG_BINDER
#ifdef DEBUG_BINDER
# define DEBUG_SYM_STACK_ENABLED
# define DEBUG_TY_STACK_ENABLED
#endif

#ifdef DEBUG_SYM_STACK_ENABLED
# define DEBUG_SYM_STACK(sym) \
    do { \
        std::cout << __func__ << " " << std::flush; \
        std::cout << to_string(sym->kind()) << std::endl; \
    } while (0)
#else
# define DEBUG_SYM_STACK(sym) do {} while (0)
#endif

#ifdef DEBUG_TY_STACK_ENABLED
# define DEBUG_TY_STACK(ty) \
    do { \
        std::cout << __func__ << " " << std::flush; \
        std::cout << to_string(*ty) << std::endl; \
    } while (0)
#else
# define DEBUG_TY_STACK(sym) do {} while (0)
#endif

#define SYM_AT_TOP(S) \
    PSY_ASSERT(!syms_.empty(), return Action::Quit); \
    auto S = syms_.top();

#define TY_AT_TOP(T) \
    PSY_ASSERT(!tys_.empty(), return Action::Quit); \
    auto T = tys_.top();

#define SCOPE_AT_TOP(S) \
    PSY_ASSERT(!scopes_.empty(), return Action::Quit); \
    auto S = scopes_.top();

#define RETURN_IF_QUIT(CALL) \
    do { \
        auto action = CALL; \
        if (action == Action::Quit) \
            return action; \
    } while (0)

