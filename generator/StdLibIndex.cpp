/******************************************************************************
 Copyright (c) 2017 Leandro T. C. Melo (ltcmelo@gmail.com)

 This library is free software; you can redistribute it and/or modify it under
 the terms of the GNU Lesser General Public License as published by the Free
 Software Foundation; either version 2.1 of the License, or (at your option)
 any later version.

 This library is distributed in the hope that it will be useful, but WITHOUT
 ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public License
 for more details.

 You should have received a copy of the GNU Lesser General Public License along
 with this library; if not, write to the Free Software Foundation, Inc., 51
 Franklin Street, Fifth Floor, Boston, MA  02110-1301 USA
 *****************************************************************************/

#include "StdLibIndex.h"
#include "Control.h"
#include <algorithm>
#include <cstring>
#include <iterator>
#include <unordered_set>

using namespace psyche;
using namespace CPlusPlus;

// C89/90
const StdLibIndex::Index StdLibIndex::c89idx_
    {
        { "assert.h",
            {
                { "assert", SymbolKind::Value }
            }
        },
        { "errno.h",
            {
                { "errno", SymbolKind::Value }
            }
        },
        { "ctype.h",
            {
                { "isalnum", SymbolKind::Value },
                { "isalpha", SymbolKind::Value },
                { "islower", SymbolKind::Value },
                { "isupper", SymbolKind::Value },
                { "isdigit", SymbolKind::Value },
                { "isxdigit", SymbolKind::Value },
                { "iscntrl", SymbolKind::Value },
                { "isgraph", SymbolKind::Value },
                { "isspace", SymbolKind::Value },
                { "isprint", SymbolKind::Value },
                { "ispunch", SymbolKind::Value },
                { "tolower", SymbolKind::Value },
                { "toupper", SymbolKind::Value }
            }
        },
        { "setjmp.h",
            {
                { "setjmp", SymbolKind::Value },
                { "longjmp", SymbolKind::Value },
                { "jmp_buf", SymbolKind::Type }
            }
        },
        { "signal.h",
            {
                { "signal", SymbolKind::Value },
                { "raise", SymbolKind::Value },
                { "sig_atomic_t", SymbolKind::Type },
                { "SIG_DFL", SymbolKind::Value },
                { "SIG_IGN", SymbolKind::Value },
                { "SIG_ERR", SymbolKind::Value },
                { "SIGTERM", SymbolKind::Value },
                { "SIGSEGV", SymbolKind::Value },
                { "SIGINT", SymbolKind::Value },
                { "SIGILL", SymbolKind::Value },
                { "SIGABRT", SymbolKind::Value },
                { "SIGFPE", SymbolKind::Value },
            }
        },
        { "stdlib.h",
            {
                { "abort", SymbolKind::Value },
                { "exit", SymbolKind::Value },
                { "atexit", SymbolKind::Value },
                { "EXIT_SUCCESS", SymbolKind::Value },
                { "EXIT_FAILURE", SymbolKind::Value },
                { "system", SymbolKind::Value },
                { "getenv", SymbolKind::Value },
                { "malloc", SymbolKind::Value },
                { "calloc", SymbolKind::Value },
                { "realloc", SymbolKind::Value },
                { "free", SymbolKind::Value },
                { "atof", SymbolKind::Value },
                { "atoi", SymbolKind::Value },
                { "atol", SymbolKind::Value },
                { "strtol", SymbolKind::Value },
                { "strtoul", SymbolKind::Value },
                { "strtod", SymbolKind::Value },
                { "strtold", SymbolKind::Value },
                { "mblen", SymbolKind::Value },
                { "mbtowc", SymbolKind::Value },
                { "wctomb", SymbolKind::Value },
                { "mbstowcs", SymbolKind::Value },
                { "wcstombs", SymbolKind::Value },
            }
        },
        { "string.h",
            {
                { "strcpy", SymbolKind::Value },
                { "strncpy", SymbolKind::Value },
                { "strcat", SymbolKind::Value },
                { "strncat", SymbolKind::Value },
                { "strxfrm", SymbolKind::Value },
                { "strlen", SymbolKind::Value },
                { "strcmp", SymbolKind::Value },
                { "strncmp", SymbolKind::Value },
                { "strcoll", SymbolKind::Value },
                { "strchr", SymbolKind::Value },
                { "strspn", SymbolKind::Value },
                { "strcspn", SymbolKind::Value }
            }
        },
        { "stdio.h",
            {
                { "fopen", SymbolKind::Value },
                { "freopen", SymbolKind::Value },
                { "fclose", SymbolKind::Value },
                { "fflush", SymbolKind::Value },
                { "setbuf", SymbolKind::Value },
                { "setvbuf", SymbolKind::Value },
                { "fwide", SymbolKind::Value },
                { "fread", SymbolKind::Value },
                { "fwrite", SymbolKind::Value },
                { "fgetc", SymbolKind::Value },
                { "getc", SymbolKind::Value },
                { "fgets", SymbolKind::Value },
                { "fputc", SymbolKind::Value },
                { "putc", SymbolKind::Value },
                { "fputs", SymbolKind::Value },
                { "getchar", SymbolKind::Value },
                { "gets", SymbolKind::Value },
                { "putchar", SymbolKind::Value },
                { "puts", SymbolKind::Value },
                { "ungetc", SymbolKind::Value },
                { "fgetwc", SymbolKind::Value },
                { "getwc", SymbolKind::Value },
                { "fgetws", SymbolKind::Value },
                { "fputwc", SymbolKind::Value },
                { "putwc", SymbolKind::Value },
                { "fputws", SymbolKind::Value },
                { "getwchar", SymbolKind::Value },
                { "putwchar", SymbolKind::Value },
                { "ungetwc", SymbolKind::Value },
                { "scanf", SymbolKind::Value },
                { "fscanf", SymbolKind::Value },
                { "sscanf", SymbolKind::Value },
                { "printf", SymbolKind::Value },
                { "fprintf", SymbolKind::Value },
                { "sprintf", SymbolKind::Value },
                { "vprintf", SymbolKind::Value },
                { "vfprintf", SymbolKind::Value },
                { "vsprintf", SymbolKind::Value },
                { "wscanf", SymbolKind::Value },
                { "swscanf", SymbolKind::Value },
                { "ftell", SymbolKind::Value },
                { "fgetpos", SymbolKind::Value },
                { "fseek", SymbolKind::Value },
                { "fsetpos", SymbolKind::Value },
                { "rewind", SymbolKind::Value },
                { "clearerr", SymbolKind::Value },
                { "feof", SymbolKind::Value },
                { "ferror", SymbolKind::Value },
                { "perror", SymbolKind::Value },
                { "remove", SymbolKind::Value },
                { "rename", SymbolKind::Value },
                { "tmpfile", SymbolKind::Value },
                { "tmpnam", SymbolKind::Value },
                { "FILE", SymbolKind::Type },
                { "fpos_t", SymbolKind::Type },
                { "stdin", SymbolKind::Value },
                { "stdout", SymbolKind::Value },
                { "stderr", SymbolKind::Value },
                { "EOF", SymbolKind::Value },
                { "FOPEN_MAX", SymbolKind::Value },
                { "FILENAME_MAX", SymbolKind::Value },
                { "BUFSIZ", SymbolKind::Value },
                { "_IOFBF", SymbolKind::Value },
                { "_IOLBF", SymbolKind::Value },
                { "_IONBF", SymbolKind::Value },
                { "SEEK_SET", SymbolKind::Value },
                { "SEEK_CUR", SymbolKind::Value },
                { "SEEK_END", SymbolKind::Value },
                { "TMP_MAX", SymbolKind::Value },
                { "L_tmpnam", SymbolKind::Value }
            }
        },
        { "wchar.h",
            {
                { "wchar_t", SymbolKind::Value },
                { "mbstate_t", SymbolKind::Value },
            }
        },
        { "wctype.h",
            {
                { "iswalnum", SymbolKind::Value },
                { "iswalpha", SymbolKind::Value },
                { "iswlower", SymbolKind::Value },
                { "iswupper", SymbolKind::Value },
                { "iswdigit", SymbolKind::Value },
                { "iswxdigit", SymbolKind::Value },
                { "iswcntrl", SymbolKind::Value },
                { "iswgraph", SymbolKind::Value },
                { "iswspace", SymbolKind::Value },
                { "iswblank", SymbolKind::Value },
                { "iswprint", SymbolKind::Value },
                { "iswpunct", SymbolKind::Value },
                { "iswctype", SymbolKind::Value },
                { "wctype_t", SymbolKind::Type },
                { "wctrans_t", SymbolKind::Type },
                { "wint_t", SymbolKind::Type },
                { "towlower", SymbolKind::Value },
                { "towupper", SymbolKind::Value },
                { "towctrans", SymbolKind::Value },
                { "wctrans", SymbolKind::Value }
            }
        }
    };

// C99
const StdLibIndex::Index StdLibIndex::c99idx_
    {
        { "ctype.h",
            {
                { "isblank", SymbolKind::Value }
            }
        },
        { "stdlib.h",
            {
                { "_Exit", SymbolKind::Value },
                { "atoll", SymbolKind::Value },
                { "strtoll", SymbolKind::Value },
                { "strtoull", SymbolKind::Value },
                { "strtof", SymbolKind::Value },
                { "strtold", SymbolKind::Value },
                { "strtoimax", SymbolKind::Value },
                { "strtoumax", SymbolKind::Value }
            }
        },
        { "inttypes.h",
            {
                { "strtoimax", SymbolKind::Value },
                { "strtoumax", SymbolKind::Value }
            }
        }
    };

// C11
const StdLibIndex::Index StdLibIndex::c11idx_
    {
        { "assert.h",
            {
                { "static_assert", SymbolKind::Value }
            }
        },
        { "errno.h",
            {
                { "errno_t", SymbolKind::Type }
            }
        },
        { "string.h",
            {
                { "strcpy_s", SymbolKind::Value },
                { "strncpy_s", SymbolKind::Value },
                { "strcat_s", SymbolKind::Value },
                { "strncat_s", SymbolKind::Value },
                { "strnlen_s", SymbolKind::Value }
            }
        },
        { "stdlib.h",
            {
                { "quick_exit", SymbolKind::Value },
                { "at_quick_exit", SymbolKind::Value },
                { "getenv_s", SymbolKind::Value },
                { "aligned_alloc", SymbolKind::Value },
                { "wctomb_s", SymbolKind::Value },
                { "mbstowcs_s", SymbolKind::Value },
                { "wcstombs_s", SymbolKind::Value },
            }
        }
    };


StdLibIndex::StdLibIndex(Version std)
    : std_(std)
{}

std::vector<std::string> StdLibIndex::inspect(const Control& control) const
{
    std::unordered_set<std::string> deps;

    auto f = [&deps, &control] (const Index& idx) {
        for (const auto& v : idx) {
            if (deps.count(v.first)) // Maybe the header has already been inserted.
                continue;
            for (const auto& s : v.second) {
                if (control.findIdentifier(s.first, strlen(s.first))) {
                    deps.insert(v.first);
                    break;
                }
            }
        }
    };

    f(c89idx_);
    if (std_ == Version::C99)
        f(c99idx_);
    if (std_ == Version::C11)
        f(c11idx_);

    std::vector<std::string> v;
    std::move(deps.begin(), deps.end(), std::back_inserter(v));

    return v;
}

bool StdLibIndex::recognizes(const char* ident) const
{
    auto f = [ident] (const Index& idx) {
        for (const auto& v : idx) {
            for (const auto& kw : v.second) {
                if (!strncmp(kw.first, ident, strlen(kw.first)))
                    return true;
            }
        }
        return false;
    };

    return f(c89idx_) ? true :
                        std_ == Version::C99 && f(c99idx_) ?
                            true :
                            std_ == Version::C11 ? f(c11idx_) : false;
}
