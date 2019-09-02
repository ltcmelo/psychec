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


from Diagnostics import *


class Command(object):
    def __init__(self, cmd):
        self.cmd = cmd
        self.sources = []
        self.c_version = 'c99'
        self.pedantic = False
        self.out_file_name = None


class GccCommand(Command):
    def __init__(self, line):
        super(GccCommand, self).__init__(line)
        self.collect()

    def collect(self):
        expect_out_file = False
        for w in self.cmd:
            # TODO: Deal with -E... Do nothing?

            # Collect file(s) being compiled.
            if w.endswith(".c"):
                self.sources.append(w)
                continue

            # Identify C version, if specified.
            if w.startswith("-std="):
                self.c_version = w[5:]
            if w == "-ansi":
                self.c_version = "c90"

            # Pedantic compilation.
            if w == "-pedantic-errors":
                self.pedantic = True

            # Keep track of output file, if specified.
            if expect_out_file:
                self.out_file_name = w
                expect_out_file = False

            if w == "-o":
                expect_out_file = True

        # Temporary
        if self.c_version != 'c99':
            DiagnosticReporter.warning(C_VERSION_NOT_SUPPORTED)
            self.c_version = 'c99'
