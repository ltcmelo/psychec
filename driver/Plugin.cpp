/******************************************************************************
 Copyright (c) 2016,17 Leandro T. C. Melo (ltcmelo@gmail.com)

 This library is free software; you can redistribute it and/or modify it under
 the terms of the GNU Lesser General Public License as published by the Free
 Software Foundation; either version 2.1 of the License, or (at your option)
 any later version.

 This library is distributed in the hope that it will be useful, but WITHOUT
 ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public License
 for more details.

 You should have received a copy of the GNU Lesser General Public License along
 with this library; if not, write to the Free Software Foundation, Inc., 51
 Franklin Street, Fifth Floor, Boston, MA  02110-1301 USA
 *****************************************************************************/

#include "Plugin.h"
#include "DeclarationInterceptor.h"
#include "SourceInspector.h"
#include "VisitorObserver.h"
#include <dlfcn.h>
#include <cassert>
#include <iostream>

using namespace psyche;

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

} // anonymous namespace


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
