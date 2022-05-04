// Copyright (c) 2016/17/18/19/20/21/22 Leandro T. C. Melo <ltcmelo@gmail.com>
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

#ifndef PSYCHE_ASSERT_H__
#define PSYCHE_ASSERT_H__

#include <iostream>

#define PSY_ASSERT(COND, CODE, MSG) \
    do { \
        if (COND) {} \
        else { \
            std::cout << "[ASSERT FAILURE] at " \
                      << __FILE__ << ":" << __LINE__ << " " \
                      << MSG << std::endl; \
            CODE; \
        } \
    } while (0)

#define PSY_ASSERT_0(COND, CODE) PSY_ASSERT(COND, CODE, "<empty message>")

#define PSY_FAIL_ASSERT(CODE, MSG) \
    do { \
        std::cout << "[ASSERT FAILURE] at " \
                  << __FILE__ << ":" << __LINE__ << " " \
                  << MSG << std::endl; \
        CODE; \
    } while (0)

#define PSY_FAIL_ASSERT_0(CODE) PSY_FAIL_ASSERT(CODE, "<empty message>")

#endif
