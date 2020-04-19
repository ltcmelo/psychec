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


from Singleton import Singleton
from typing import Set


def xtrace(parent, cmd):
    return Logger().xtrace(parent, cmd)


def debug(parent, msg):
    Logger().debug(parent, msg)


class Logger(metaclass=Singleton):

    def __init__(self):
        self.xtrace_enabled: bool = False
        self.debug_enabled: Set[str] = set()

    def configure(self, xtrace, debug):
        self.xtrace_enabled = xtrace
        if debug:
            self.debug_enabled = set(debug)

    class Handle:
        def __init__(self, ctx):
            self.ctx = ctx

        def __enter__(self):
            return self

        def __exit__(self, exc_type, exc_val, exc_tb):
            if self.ctx:
                print(f'... {self.r}\n')

        def report(self, r):
            self.r = r

    @staticmethod
    def prefix(parent: str):
        return f'<cnippet :: {parent.upper()}>'

    def xtrace(self, parent, cmd):
        if self.xtrace_enabled:
            print(f'{Logger.prefix(parent)} $\n{cmd}')
        return Logger.Handle(self.xtrace_enabled)

    def debug(self, parent, msg):
        if parent in self.debug_enabled or 'all' in self.debug_enabled:
            print(f'{Logger.prefix(parent)} {msg}\n')
