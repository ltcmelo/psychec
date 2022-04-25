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

#include "Plugin.h"

#include "plugin-api/DeclarationInterceptor.h"
#include "plugin-api/SourceInspector.h"

#include <dlfcn.h>
#include <cassert>
#include <iostream>

using namespace cnip;
using namespace psy;
using namespace C;

void* Plugin::handle_ { nullptr };
DeclarationInterceptor* Plugin::interceptor_ { nullptr };
SourceInspector* Plugin::inspector_ { nullptr };
VisitorObserver* Plugin::observer_ { nullptr };

namespace {

std::string appendSuffix(std::string s)
{
    return s +
        #if __APPLE__
            ".dylib";
        #else
            ".so";
        #endif
}

} // anonymous

void Plugin::load(const std::string& name)
{
    const std::string file = appendSuffix(name);
    handle_ = dlopen(file.c_str(), RTLD_LOCAL | RTLD_LAZY);
}

bool Plugin::isLoaded()
{
    return handle_;
}

void Plugin::unload()
{
    if (!handle_)
        return;

    destroy(interceptor_, "deleteInterceptor");
    destroy(inspector_, "deleteInspector");
    destroy(observer_, "deleteObserver");
}

DeclarationInterceptor* Plugin::createInterceptor()
{
    interceptor_ = create<DeclarationInterceptor>("newInterceptor");
    return interceptor_;
}

SourceInspector* Plugin::createInspector()
{
    inspector_ = create<SourceInspector>("newInspector");
    return inspector_;
}

VisitorObserver* Plugin::createObserver()
{
    observer_ = create<VisitorObserver>("newObserver");
    return observer_;
}

template <class RetT>
RetT* Plugin::create(const char* name)
{
    if (!handle_)
        return nullptr;

    using FuncT = RetT* (*) ();

    dlerror();
    FuncT func = (FuncT)dlsym(handle_, name);
    if (dlerror())
        return nullptr;

    return (*func)();
}

template <class ParamT>
void Plugin::destroy(ParamT* p, const char* name)
{
    using FuncT = void (*) (ParamT*);

    if (p) {
        dlerror();
        FuncT func = (FuncT)dlsym(handle_, name);
        if (!dlerror())
            (*func)(p);
    }
}

// Explicit instantiations
template DeclarationInterceptor* Plugin::create<DeclarationInterceptor>(const char*);
template SourceInspector* Plugin::create<SourceInspector>(const char*);
template VisitorObserver* Plugin::create<VisitorObserver>(const char*);

template void Plugin::destroy<DeclarationInterceptor>(DeclarationInterceptor*, const char*);
template void Plugin::destroy<SourceInspector>(SourceInspector*, const char*);
template void Plugin::destroy<VisitorObserver>(VisitorObserver*, const char*);
