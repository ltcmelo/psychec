// Copyright (c) 2014-20 Leandro T. C. Melo <ltcmelo@gmail.com>
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

/*-------------------------------------------------------------*/
/* File "borrowed" from the UASIO project:                     */
/*                                                             */
/* https://github.com/ltcmelo/uaiso/blob/master/Common/Pimpl.h */
/*-------------------------------------------------------------*/

#ifndef PSYCHE_PIMPL_H__
#define PSYCHE_PIMPL_H__

#include <memory>

#define DECL_PIMPL(CLASS) \
    struct CLASS##Impl; \
    std::unique_ptr<CLASS##Impl> impl_;

#define DECL_PIMPL_SUB(CLASS) \
    struct CLASS##Impl; \
    CLASS##Impl* CAST() const { return ((CLASS##Impl*)impl_.get()); }

#define DECL_SHARED_DATA(CLASS) \
    struct CLASS##Impl; \
    std::shared_ptr<CLASS##Impl> impl_;

#define P impl_
#define P_CAST CAST()

#endif
