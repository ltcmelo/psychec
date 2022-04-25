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

#ifndef PSYCHE_LOCATION_H__
#define PSYCHE_LOCATION_H__

#include "../API.h"

#include "FileLinePositionSpan.h"

#include <ostream>
#include <string>

namespace psy {

/**
 * \brief The Location class.
 *
 * \note
 * This API is inspired by that of \c Microsoft.CodeAnalysis.Location
 * from Roslyn, the .NET Compiler Platform.
 */
class PSY_API Location
{
public:
    static Location create(std::string filePath, LinePositionSpan lineSpan);
    static Location create(FileLinePositionSpan fileLineSpan);

    /**
     * The file, line, and column of \c this Location.
     */
    const FileLinePositionSpan& lineSpan() const { return fileLineSpan_; }

private:
    Location(std::string filePath, LinePositionSpan lineSpan);
    Location(FileLinePositionSpan fileLineSpan);

    FileLinePositionSpan fileLineSpan_;
};

bool operator==(const Location& a, const Location& b);

std::ostream& operator<<(std::ostream& os, const Location& loc);

} // psy

#endif
