/******************************************************************************
 Copyright (c) 2017 Leandro T. C. Melo (ltcmelo@gmail.com)

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

#ifndef PLUGIN_CONFIG_H__
#define PLUGIN_CONFIG_H__

// From https://gcc.gnu.org/wiki/Visibility
#if defined _WIN32 || defined __CYGWIN__
  #ifdef EXPORT_API
    #ifdef __GNUC__
      #define PLUGIN_API __attribute__ ((dllexport))
    #else
      #define PLUGIN_API __declspec(dllexport)
    #endif
  #else
    #ifdef __GNUC__
      #define PLUGIN_API __attribute__ ((dllimport))
    #else
      #define PLUGIN_API __declspec(dllimport)
    #endif
  #endif
  #define PLUGIN_API_LOCAL
#else
  #if __GNUC__ >= 4
    #define PLUGIN_API __attribute__ ((visibility ("default")))
    #define PLUGIN_API_LOCAL  __attribute__ ((visibility ("hidden")))
  #else
    #define PLUGIN_API
    #define PLUGIN_API_LOCAL
  #endif
#endif

#endif
