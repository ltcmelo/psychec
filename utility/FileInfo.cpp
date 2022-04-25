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

#include "FileInfo.h"

using namespace psy;

struct FileInfo::FileInfoImpl
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

std::string FileInfo::fullFileBaseName() const
{
    return fullDir() + "/" + fileBaseName();
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
