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


from Diagnostics import DiagnosticReporter, C_VERSION_NOT_SUPPORTED


class CommandSummary(object):
    """
    The command to the C compiler.
    """

    def __init__(self, cmd):
        self.cmd = cmd

        self.c_version = 'c99'
        self.c_files = []
        self.D_lst = []
        self.U_lst = []
        self.I_lst = []
        self.pedantic = False
        self.out_file_name = None

    @staticmethod
    def _prefixed_lst(prefix: str, lst):
        lst = [prefix + el for el in lst]
        return lst

    def defined_macros(self, prefix: str):
        return CommandSummary._prefixed_lst(prefix, self.D_lst)

    def undefined_macros(self, prefix: str):
        return CommandSummary._prefixed_lst(prefix, self.U_lst)

    def include_paths(self, prefix: str):
        return CommandSummary._prefixed_lst(prefix, self.I_lst)

    def collect(self):
        expect_out_file = False
        for v in self.cmd:
            # TODO: Deal vith -E... Do nothing?

            if v.startswith("-std="):
                self.c_version = v[5:]
            if v == "-ansi":
                self.c_version = "c90"

            if v.endswith('.c'):
                self.c_files.append(v)
                continue

            if v.startswith('-D'):
                self.D_lst.append(v[2:].strip())
                continue

            if v.startswith('-U'):
                self.U_lst.append(v[2:].strip())
                continue

            if v.startswith('-I'):
                self.I_lst.append(v[2:].strip())
                continue

            if v == "-pedantic-errors":
                self.pedantic = True

            # Keep track of output file, if specified.
            if expect_out_file:
                self.out_file_name = v
                expect_out_file = False
            if v == "-o":
                expect_out_file = True

        # Temporary
        if self.c_version != 'c99':
            DiagnosticReporter.warning(C_VERSION_NOT_SUPPORTED)
            self.c_version = 'c99'

        return self
