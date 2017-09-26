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

#include "FileInfo.h"

using namespace psyche;

struct psyche::FileInfo::FileInfoImpl
{
    FileInfoImpl(const std::string& fullFileName)
        : fullFileName_(fullFileName)
    {}

    std::string fullFileName_;
};

FileInfo::FileInfo(const std::string& fullFileName)
    : impl_(new FileInfoImpl(fullFileName))
{}

FileInfo::~FileInfo()
{}

std::string FileInfo::fileName() const
{
    size_t pos = impl_->fullFileName_.rfind('/');
    return impl_->fullFileName_.substr(pos + 1);
}

std::string FileInfo::fileBaseName() const
{
    size_t pos2 = impl_->fullFileName_.rfind('.');
    size_t pos1 = impl_->fullFileName_.rfind('/', pos2);
    return impl_->fullFileName_.substr(pos1 + 1, pos2 - (pos1 + 1));
}

std::string FileInfo::fullFileName() const
{
    return impl_->fullFileName_;
}

std::string FileInfo::dir() const
{
    size_t pos1 = std::string::npos;
    size_t pos2 = impl_->fullFileName_.rfind('/');
    if (pos2 == 0)
        return "";
    pos1 = impl_->fullFileName_.rfind('/', pos2 - 1);
    if (pos1 == std::string::npos)
        pos1 = 0;
    return impl_->fullFileName_.substr(pos1 + 1, pos2 - (pos1 + 1));
}

std::string FileInfo::fullDir() const
{
    size_t pos = impl_->fullFileName_.rfind('/');
    if (pos == std::string::npos)
        return "./";
    return impl_->fullFileName_.substr(0, pos + 1);
}

char FileInfo::dirSeparator()
{
    return '/';
}
