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

#ifndef CNIPPET_C_CONFIGURATION_H__
#define CNIPPET_C_CONFIGURATION_H__

#include "Configuration.h"

#include "C/parser/LanguageDialect.h"

#include <string>
#include <vector>

using namespace psy;
using namespace C;

namespace cnip {

/*!
 * \brief The CConfiguration class.
 */
class CConfiguration : public Configuration
{
public:
    CConfiguration(const cxxopts::ParseResult& parsedCmdLine);

    static void extend(cxxopts::Options& cmdLineOpts);

    bool isValid() const;

    /* Compiler */
    std::string compiler_;

    /* Language */
    std::string std_;

    /* Preprocessor */
    bool ppIncludes_;
    std::vector<std::string> searchPaths_;
    std::vector<std::string> definedMacros_;
    std::vector<std::string> undefedMacros_;

    /* Parser */
    std::string commentMode_;
    std::string ambigMode_;

    bool inferTypes_;
};

} // cnip

#endif
