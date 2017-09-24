/******************************************************************************
 Copyright (c) 2016-17 Leandro T. C. Melo (ltcmelo@gmail.com)

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

#include "Factory.h"
#include "ConstraintWriter.h"
#include "DeclarationInterceptor.h"
#include "VisitorObserver.h"

using namespace psyche;

std::unique_ptr<ConstraintWriter> Factory::makeConstraintWriter(std::ostream& os) const
{
    return std::make_unique<ConstraintWriter>(os);
}

std::unique_ptr<VisitorObserver> Factory::makeObserver() const
{
    return std::make_unique<VisitorObserver>();
}

std::unique_ptr<DeclarationInterceptor> Factory::makeInterceptor() const
{
    return std::make_unique<DeclarationInterceptor>();
}
