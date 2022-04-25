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

/*
 * This API is inspired by the API of Roslyn (the .NET Compiler Platform).
 */

#ifndef PSYCHE_DIAGNOSTIC_DESCRIPTOR_H__
#define PSYCHE_DIAGNOSTIC_DESCRIPTOR_H__

#include "../API.h"

#include "DiagnosticCategory.h"
#include "DiagnosticSeverity.h"

#include <string>

namespace psy {

/**
 * The DiagnosticDescriptor class.
 */
class PSY_API DiagnosticDescriptor
{
public:
    DiagnosticDescriptor(std::string id,
                         std::string title,
                         std::string description,
                         DiagnosticSeverity defaultSeverity,
                         DiagnosticCategory category);

    /**
     * The ID of \c this DiagnosticDescriptor.
     */
    const std::string& id() const;

    /**
     * The title of \c this DiagnosticDescriptor.
     */
    const std::string& title() const;

    /**
     * The description of \c this DiagnosticDescriptor.
     */
    const std::string& description() const;

    /**
     * The default severity of \c this DiagnosticDescriptor.
     */
    DiagnosticSeverity defaultSeverity() const;

    /**
     * The category of \c this DiagnosticDescriptor.
     */
    DiagnosticCategory category() const;

private:
    std::string id_;
    std::string title_;
    std::string description_;
    DiagnosticSeverity defaultSeverity_;
    DiagnosticCategory category_;
};

} // psy

#endif
