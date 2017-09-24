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

#ifndef PSYCHE_PLUGIN_H__
#define PSYCHE_PLUGIN_H__

#include <string>
#include <type_traits>

namespace psyche {

class DeclarationInterceptor;
class SourceInspector;
class VisitorObserver;

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

    static DeclarationInterceptor* createInterceptor();
    static SourceInspector* createInspector();
    static VisitorObserver* createObserver();

private:
    Plugin() = delete;

    template <class RetT>
    static RetT* create(const char* name);

    template <class ParamT>
    static void destroy(ParamT*, const char* name);

    static void* handle_;
    static DeclarationInterceptor* interceptor_;
    static SourceInspector* inspector_;
    static VisitorObserver* observer_;
};

} // namespace psyche

#endif
