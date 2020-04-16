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


class Diagnostic:

    def __init__(self,
                 code: str,
                 desc: str):
        self.code = code
        self.desc = desc

    def __str__(self):
        return 'diagnostic: %s %s' % (self.code, self.desc)

    def __repr__(self):
        return self.code + self.desc


# Predefined diagnostics
HOST_C_COMPILER_NOT_FOUND = Diagnostic(1, 'host C compiler not found')
FILE_DOES_NOT_EXIST = Diagnostic(2, "file doesn't exist: %s")
PREPROCESSING_FILE_FAILED = Diagnostic(3, 'preprocessing failed: %s')
EXCEPTION_EXECUTING_PROCESS = Diagnostic(4, 'exception invoking process: %s')
CONSTRAINT_GENERATION_FOR_FILE_FAILED = Diagnostic(5, 'constraint generation failed: %s')
CONSTRAINT_SOLVING_FOR_FILE_FAILED = Diagnostic(6, 'constraint solving failed: %s')
LICENSE_FILE_NOT_FOUND = Diagnostic(7, 'license file not found')
CORRUPTED_LICENSE_FILE = Diagnostic(8, 'corrupted license file')
LICENSE_EXPIRED = Diagnostic(9, 'license expired')
INCOMPATIBLE_LICENSE_PRODUCT_VERSION = Diagnostic(10, "license doesn't apply for product version")
HOST_C_COMPILER_FORWARDING_FAILED = Diagnostic(11, 'command forwarded to host C compiler failed')
ERROR_CREATING_CONFIG_DIRECTORY = Diagnostic(12, 'error creating config directory')
ERROR_FETCHING_GIT_SHA = Diagnostic(13, 'cannot fetch git sha')
EXCEPTION_COPYING_FILE_PATH = Diagnostic(14, 'exception copying path: %s to %s')
EXCEPTION_COPYING_FILE_OBJECT = Diagnostic(15, 'exception copying object: %s to %s')
C_VERSION_NOT_SUPPORTED = Diagnostic(16, 'unsupported C version (falling back to C99)')


class DiagnosticReporter:
    """
    The diagnostic's reporter.
    """

    _FATAL = 'fatal'
    _ERROR = 'error'
    _WARNING = 'warning'

    @staticmethod
    def _display(level, diagnostic, *args, **kwargs):
        s = 'cnippet: %s: %s' % (level, diagnostic.desc % args)
        if kwargs:
            s += ' ('
            for k, v in kwargs.items():
                s += f'{k} {v}'
            s += ')'
        print(s)
        return diagnostic.code

    @staticmethod
    def fatal(diagnostic, *args, **kwargs):
        return DiagnosticReporter._display(DiagnosticReporter._FATAL,
                                           diagnostic,
                                           *args,
                                           **kwargs)

    @staticmethod
    def error(diagnostic, *args, **kwargs):
        return DiagnosticReporter._display(DiagnosticReporter._ERROR,
                                           diagnostic,
                                           *args,
                                           **kwargs)

    @staticmethod
    def warning(diagnostic, *args, **kwargs):
        return DiagnosticReporter._display(DiagnosticReporter._WARNING,
                                           diagnostic,
                                           *args,
                                           **kwargs)
