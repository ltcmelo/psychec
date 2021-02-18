// Copyright (c) 2016/17/18/19/20/21 Leandro T. C. Melo <ltcmelo@gmail.com>
// Copyright (c) 2008 Roberto Raggi <roberto.raggi@gmail.com>
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

#ifndef PSYCHE_DIAGNOSTIC_COLLECTOR_H__
#define PSYCHE_DIAGNOSTIC_COLLECTOR_H__

#include "Diagnostic.h"

#include "../API.h"

#include <cstdarg>
#include <string>

namespace psy {
namespace C {

class PSY_API DiagnosticCollector
{
public:
    enum Severity {
        Warning,
        Error,
        Fatal
    };

    DiagnosticCollector();
    virtual ~DiagnosticCollector();

    virtual void collect(DiagnosticSeverity severity,
                         DiagnosticCategory nature,
                         const char* fileName,
                         unsigned line,
                         unsigned column,
                         const std::string& message);


    // TODO: remove
    virtual void collect(Severity severity,
                         const char* fileName,
                         unsigned line,
                         unsigned column,
                         const char *format,
                         va_list args);

    // TODO: remove
    bool seenBlockingIssue() const { return blockingIssue_; }

    int errorsCount() const { return erroCnt_; }
    int warningsCount() const { return warnCnt_; }

    void reset();

private:
    bool blockingIssue_ { false }; // TODO: remove

    int erroCnt_ = 0;
    int warnCnt_ = 0;
};

} // C
} // psy

#endif
