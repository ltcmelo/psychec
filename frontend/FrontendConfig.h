// Copyright (c) 2008 Roberto Raggi <roberto.raggi@gmail.com>
//
// Modifications:
// Copyright (c) 2016,17 Leandro T. C. Melo (ltcmelo@gmail.com)
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

#ifndef CFE_API_H__
#define CFE_API_H__

// From https://gcc.gnu.org/wiki/Visibility
#if defined _WIN32 || defined __CYGWIN__
  #ifdef EXPORT_API
    #ifdef __GNUC__
      #define CFE_API __attribute__ ((dllexport))
    #else
      #define CFE_API __declspec(dllexport)
    #endif
  #else
    #ifdef __GNUC__
      #define CFE_API __attribute__ ((dllimport))
    #else
      #define CFE_API __declspec(dllimport)
    #endif
  #endif
  #define CFE_API_LOCAL
#else
  #if __GNUC__ >= 4
    #define CFE_API __attribute__ ((visibility ("default")))
    #define CFE_API_LOCAL  __attribute__ ((visibility ("hidden")))
  #else
    #define CFE_API
    #define CFE_API_LOCAL
  #endif
#endif

#ifndef UNLIKELY
  #ifdef __GNUC__
    #define UNLIKELY(expr) __builtin_expect(!!(expr), false)
  #else
    #define UNLIKELY(expr) (expr)
  #endif
#endif

#endif
