// Copyright (c) 2016/17/18/19/20/21 Leandro T. C. Melo <ltcmelo@gmail.com>
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

#include "CompilerFacade.h"

#include "Process.h"

#include <cstdlib>
#include <fstream>
#include <iostream>
#include <string>

using namespace psy;

CompilerFacade::CompilerFacade(const std::string& hostCC,
                               const std::string& std,
                               const std::vector<std::string>& macroDefs,
                               const std::vector<std::string>& macroUndefs)
    : hostCC_(hostCC)
    , std_(std)
    , macroDefs_(macroDefs)
    , macroUndefs_(macroUndefs)
{}

std::pair<int, std::string> CompilerFacade::preprocess(const std::string& source)
{
    // Template base filename to pass to 'mkstemp'
    const std::string base_template("psyche_XXXXXX");

    // Default temporary location
    const std::string default_dir("/tmp");

    // Read the environment
    const char *env_tmp_dir = std::getenv("TMPDIR");

    // Calculat temporary directory
    const std::string tmp_dir(env_tmp_dir != nullptr ? env_tmp_dir : default_dir);

    // Calculate the full path
    const std::string pathed_template(tmp_dir + "/" + base_template);

    // now get a char* to be able to call mkstemp
    char *tmp_template = new char[pathed_template.size() + 1];
    std::copy(pathed_template.begin(), pathed_template.end(), tmp_template);
    tmp_template[pathed_template.size()] = '\0';

    // Convert our template into a full-file
    int err = mkstemp(tmp_template);

    // mkstemp returns -1 on error ...
    if (err == -1) {
        // ... so do we
        return std::make_pair(err, "");
    }

    // Let's get a std::string for convenience
    std::string tmp_name(tmp_template);

    // Delete our template
    delete[] tmp_template;

    // Write our source to our temporary file
    std::ofstream tmp_stream(tmp_name);
    tmp_stream << source;

    // build-up the preprocessor invocation
    std::string in = hostCC_;
    in += macroSetup();
    in += " ";
    in += "-std=" + std_ + " ";
    in += "-E -x c -CC ";
    in += tmp_name;

    // call the preprocessor
    auto ret = Process().execute(in);

    // remove the temporary file
    std::remove(tmp_name.c_str());

    return ret;
}

std::string CompilerFacade::macroSetup() const
{
    std::string s;
    for (const auto& d : macroDefs_)
        s += " -D " + d;
    for (const auto& u : macroUndefs_)
        s += " -U " + u;
    return s;
}
