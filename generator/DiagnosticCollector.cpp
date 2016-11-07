/******************************************************************************
 * Copyright (c) 2016 Leandro T. C. Melo (ltcmelo@gmail.com)
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301
 * USA
 *****************************************************************************/

#include "DiagnosticCollector.h"
#include "Literals.h"
#include <cstdio>
#include <iostream>

using namespace CPlusPlus;
using namespace psyche;

void DiagnosticCollector::report(int level,
                                 const StringLiteral *fileName,
                                 unsigned line, unsigned column,
                                 const char *format, va_list ap)
{
    fprintf(stderr, "error at %s:%u:%u, ", fileName->chars(), line, column);
    vfprintf(stderr, format, ap);
    std::cout << std::endl;
    hasError_ = true;
}
