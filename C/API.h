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

#ifndef PYSCHE_C_API_H__
#define PYSCHE_C_API_H__

// From https://gcc.gnu.org/wiki/Visibility
#if defined _WIN32 || defined __CYGWIN__
  #ifdef EXPORT_C_API
    #ifdef __GNUC__
      #define PSY_C_API __attribute__ ((dllexport))
    #else
      #define PSY_C_API __declspec(dllexport)
    #endif
  #else
    #ifdef __GNUC__
      #define PSY_C_API __attribute__ ((dllimport))
    #else
      #define PSY_C_API __declspec(dllimport)
    #endif
  #endif
  #define PSY_C_NON_API
#else
  #if __GNUC__ >= 4
    #define PSY_C_API __attribute__ ((visibility ("default")))
    #define PSY_C_NON_API __attribute__ ((visibility ("hidden")))
  #else
    #define PSY_C_API
    #define PSY_C_NON_API
  #endif
#endif

#endif
