/******************************************************************************
 * Copyright (c) 2016 Leandro T. C. Melo (ltcmelo@gmail.com)
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301
 * USA
 *****************************************************************************/

#include "PrintfScanner.h"
#include "Assert.h"

using namespace psyche;

PrintfScanner::PrintfScanner(const std::string& format)
    : format_(format)
{}

std::vector<PrintfScanner::FormatSpec> PrintfScanner::evaluate() const
{
    std::vector<FormatSpec> specs;
    const int length = format_.length();
    for (int i = 0; i < length; ++i) {
        if (format_[i] != '%')
            continue;
        if (++i < length && format_[i] == '%')
            continue; // It's a percent sign escape.

        // Relaxed implementation of the conversion specifiers.
        bool done = false;
        while (i < length && !done) {
            switch (format_[i]) {
            case '-':
            case '+':
            case ' ':
            case '#':
            case '*':
            case '.':
            case '0':
            case '1':
            case '2':
            case '3':
            case '4':
            case '5':
            case '6':
            case '7':
            case '8':
            case '9':
                break;

            case 'c':
                specs.push_back(Char);
                done = true;
                break;

            case 's':
                specs.push_back(String);
                done = true;
                break;

            case 'd':
            case 'i':
            case 'o':
            case 'x':
            case 'X':
            case 'u':
                specs.push_back(Int);
                done = true;
                break;

            case 'f':
            case 'F':
            case 'e':
            case 'E':
            case 'a':
            case 'A':
            case 'g':
            case 'G':
                specs.push_back(FloatingPoint);
                done = true;
                break;

            default: {
                std::string msg = "unknown escape: ";
                msg += format_[i];
                PSYCHE_ASSERT(false, return specs, msg);
                break;
            }
            }
            ++i;
        }
    }

    return specs;
}
