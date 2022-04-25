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

#include "DiagnosticDescriptor.h"

using namespace psy;

DiagnosticDescriptor::DiagnosticDescriptor(std::string id,
                                           std::string title,
                                           std::string description,
                                           DiagnosticSeverity defaultSeverity,
                                           DiagnosticCategory category)
    : id_(std::move(id))
    , title_(std::move(title))
    , description_(description)
    , defaultSeverity_(defaultSeverity)
    , category_(category)
{}

const std::string& DiagnosticDescriptor::id() const
{
    return id_;
}

const std::string& DiagnosticDescriptor::title() const
{
    return title_;
}

const std::string& DiagnosticDescriptor::description() const
{
    return description_;
}

DiagnosticSeverity DiagnosticDescriptor::defaultSeverity() const
{
    return defaultSeverity_;
}

DiagnosticCategory DiagnosticDescriptor::category() const
{
    return category_;
}
