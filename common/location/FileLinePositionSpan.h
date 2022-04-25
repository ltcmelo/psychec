// Copyright (c) 2020/21/22 Leandro T. C. Melo <ltcmelo@gmail.com>
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

#ifndef PSYCHE_FILE_LINE_POSITION_SPAN_H__
#define PSYCHE_FILE_LINE_POSITION_SPAN_H__

#include "../API.h"

#include "LinePositionSpan.h"

#include <ostream>
#include <string>

namespace psy {

/**
 * \brief The FileLinePositionSpan class.
 *
 * An abstraction representation of a position, in terms of line,
 * within a file.
 *
 * \note
 * This API is inspired by that of \c Microsoft.CodeAnalysis.FileLinePositionSpan
 * from Roslyn, the .NET Compiler Platform.
 */
class PSY_API FileLinePositionSpan
{
public:
    FileLinePositionSpan(std::string path, LinePositionSpan span)
        : path_(std::move(path))
        , span_(std::move(span))
    {}

    FileLinePositionSpan(std::string path,
                         const LinePosition& start,
                         const LinePosition& end)
        : path_(std::move(path))
        , span_(start, end)
    {}

    /**
     * The path of the file.
     */
    std::string path() const { return path_; }

    /**
     * The line span within the file.
     */
    LinePositionSpan span() const { return span_; }

    /**
     * The line position of the end of the span.
     */
    LinePosition endLinePosition() const { return span_.end(); }

    /**
     * The line position of the start of the span.
     */
    LinePosition starLinePosition() const { return span_.start(); }

private:
    std::string path_;
    LinePositionSpan span_;
};

bool operator==(const FileLinePositionSpan& a, const FileLinePositionSpan& b);

std::ostream& operator<<(std::ostream& os, const FileLinePositionSpan& span);

} // psy

#endif
