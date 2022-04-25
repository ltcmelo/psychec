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

#ifndef CNIPPET_PLUGIN_H__
#define CNIPPET_PLUGIN_H__

#include <string>
#include <type_traits>

namespace psy {
namespace C {

class DeclarationInterceptor;
class SourceInspector;
class VisitorObserver;

} // C
} // psy

namespace cnip {

/*!
 * \brief The Plugin class
 *
 * To be extended...
 */
class Plugin final
{
public:
    static void load(const std::string& name);
    static bool isLoaded();
    static void unload();

    static psy::C::DeclarationInterceptor* createInterceptor();
    static psy::C::SourceInspector* createInspector();
    static psy::C::VisitorObserver* createObserver();

private:
    Plugin() = delete;

    template <class RetT>
    static RetT* create(const char* name);

    template <class ParamT>
    static void destroy(ParamT*, const char* name);

    static void* handle_;
    static psy::C::DeclarationInterceptor* interceptor_;
    static psy::C::SourceInspector* inspector_;
    static psy::C::VisitorObserver* observer_;
};

} // cnip

#endif
