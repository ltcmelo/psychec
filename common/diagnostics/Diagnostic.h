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

#ifndef PSYCHE_DIAGNOSTIC_H__
#define PSYCHE_DIAGNOSTIC_H__

#include "../API.h"

#include "DiagnosticCategory.h"
#include "DiagnosticDescriptor.h"
#include "DiagnosticSeverity.h"

#include "../location/Location.h"

#include <ostream>
#include <string>

namespace psy {

namespace C { class InternalsTestSuite; }

/**
 * The Diagnostics class.
 */
class PSY_API Diagnostic
{
public:
    Diagnostic(DiagnosticDescriptor descriptor,
               Location location,
               std::string snippet = "");

    /**
     * The DiagnosticDescriptor of \c this Diagnostic.
     */
    const DiagnosticDescriptor& descriptor() const { return descriptor_; }

    /**
     * The actual severity (according to compilation options) of \c this Diagnostic.
     */
    DiagnosticSeverity severity() const;

    /**
     * The Location of \c this Location.
     */
    const Location& location() const { return location_; }

    /**
     * The code snippet associated to \c this Diagnostic.
     */
    const std::string& snippet() const { return snippet_; }

private:
    DiagnosticDescriptor descriptor_;
    Location location_;
    std::string snippet_;
    std::string::size_type outputIndent_;

    friend std::ostream& operator<<(std::ostream& os, const Diagnostic& diagnostic);
    friend class C::InternalsTestSuite;
};

PSY_API std::ostream& operator<<(std::ostream& os, const Diagnostic& diagnostic);

} // psy

#endif
