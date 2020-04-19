# -----------------------------------------------------------------------------
# Copyright (c) 2017 Leandro T. C. Melo (LTCMELO@GMAIL.COM)
#
# All rights reserved. Unauthorized copying of this file, through any
# medium, is strictly prohibited.
#
# This software is provided on an "AS IS" BASIS, WITHOUT WARRANTIES OR
# CONDITIONS OF ANY KIND, explicit or implicit. In no event shall the
# author be liable for any claim or damages.
# -----------------------------------------------------------------------------


import subprocess
import sys
from CommandSummary import CommandSummary
from Diagnostics import DiagnosticReporter, PREPROCESSING_FILE_FAILED
from Process import execute


class CCompilerFacade:
    """
    Facade to the host C compiler.
    """

    @staticmethod
    def ID():
        return CCompilerFacade.__name__

    GCC = 'GCC'
    Clang = 'Clang'

    def __init__(self, cnip_opts):
        self.cc = cnip_opts['cc']
        self.cc_cmd_line = cnip_opts["cc_cmd_line"]
        self.cc_family = None
        self.cc_cmd_summary = None

    @staticmethod
    def defined_macros(prefix):
        """
        Defined common builtin/platform macros.
        """

        macros = [
            # Calling conventions
            prefix, '__cdecl=',
            prefix, '__stdcall=',
            prefix, '__thiscall=',

            # Nullability attributes
            prefix, '_Nullable=',
            prefix, '_Nonnull=',

            # GNU alternate keywords
            prefix, '__extension__=',

            # Microsoft
            prefix, "'__declspec(a)='"
        ]

        return macros

    @staticmethod
    def undefined_macros(prefix):
        """
        Undefine common builtin/platform macros.
        """

        macros = [
            # Clang' block language.
            prefix, '__BLOCKS__',
        ]

        return macros

    def is_supported(self) -> bool:
        """
        Tell whether the host C compiler is supported.
        """

        # Look at predefined macros: $ echo | gcc -dM -E -
        echo = subprocess.Popen('echo', stdout=subprocess.PIPE)
        cmd = [self.cc, '-dM', '-E', '-']
        try:
            macros = subprocess.check_output(cmd, stdin=echo.stdout)
        except:
            return False

        #  __GNU__ is predefined in GCC/Clang; __clang__, only in Clang.
        if b'__clang__' in macros:
            self.cc_family = CCompilerFacade.Clang
        elif b'__GNUC__' in macros:
            self.cc_family = CCompilerFacade.GCC

        return True

    def parse_command(self) -> CommandSummary:
        """
        Parse the compiler command to extract compilation options.
        """

        assert self.cc_family

        self.cc_cmd_summary = CommandSummary(self.cc_cmd_line)
        return self.cc_cmd_summary.collect()

    def original_options(self):
        cmd = self.cc_cmd_summary.defined_macros('-D')
        cmd += self.cc_cmd_summary.undefined_macros('-U')
        cmd += self.cc_cmd_summary.include_paths('-I')
        return cmd

    def check_syntax(self, c_file_name):
        """
        Check the "syntax" of the file -- this is an abuse of terminology, since
        `fsyntax-only' performs symbol lookup, i.e., if a declaration is missing,
        an error is thrown.
        """

        cmd = [self.cc, '-fsyntax-only', c_file_name]

        common_opts = [
            '-Werror=incompatible-pointer-types',
            '-Werror=implicit-function-declaration'
        ]
        cmd += common_opts

        by_cc_opts = {
            CCompilerFacade.GCC: '-Werror=builtin-declaration-mismatch',
            CCompilerFacade.Clang: '-Werror=incompatible-library-redeclaration'
        }
        cmd.append(by_cc_opts[self.cc_family])

        cmd += self.original_options()

        return execute(CCompilerFacade.ID(),
                       cmd,
                       stdout=subprocess.PIPE,
                       stderr=subprocess.PIPE)

    def preprocess(self, c_file_name, pp_file_name):
        """
        Preprocess the file.
        """

        cmd = [self.cc,
               '-E',
               '-x',
               'c',
               c_file_name,
               '-o',
               pp_file_name]

        cmd += CCompilerFacade.defined_macros('-D')
        cmd += CCompilerFacade.undefined_macros('-U')

        cmd += self.original_options()

        code = execute(CCompilerFacade.ID(), cmd)
        if code != 0:
            sys.exit(
                DiagnosticReporter.fatal(PREPROCESSING_FILE_FAILED,
                                         c_file_name))
