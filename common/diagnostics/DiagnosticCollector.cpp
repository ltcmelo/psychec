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

#include "DiagnosticCollector.h"

#include <cstdio>
#include <iostream>
#include <unordered_map>

using namespace psy;
using namespace C;

DiagnosticCollector::DiagnosticCollector()
{}

DiagnosticCollector::~DiagnosticCollector()
{}

void DiagnosticCollector::reset()
{
    erroCnt_ = 0;
    warnCnt_ = 0;

    blockingIssue_ = false;
}

void DiagnosticCollector::collect(DiagnosticSeverity severity,
                                  DiagnosticCategory category,
                                  const char* fileName,
                                  unsigned int line,
                                  unsigned int column,
                                  const std::string &message)
{
    switch (severity) {
        case DiagnosticSeverity::Warning:
            ++warnCnt_;
            break;

        case DiagnosticSeverity::Error:
            ++erroCnt_;
            break;

        default: break;
    }

    std::cout << fileName
              << ":" << line << ":" << column
              << " (" << category << ") "
              << severity
              << ":" << message;
}

void DiagnosticCollector::collect(Severity severity,
                                  const char* fileName,
                                  unsigned line,
                                  unsigned column,
                                  const char *format,
                                  va_list args)
{
    static std::unordered_map<int, const char*> map {
        { Warning, "warning" },
        { Error, "error" },
        { Fatal, "fatal" }
    };

    switch (severity) {
        case Warning:
            ++warnCnt_;
            break;

        case Error:
            ++erroCnt_;
            blockingIssue_ = true;
            break;

        default: break;
    }


    fprintf(stdout, "%s:%u:%u: %s: ", fileName, line, column, map[severity]);
    vfprintf(stdout, format, args);
}
