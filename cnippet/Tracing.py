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


def trace_extern_cmd(parent, cmd):
    _trace(parent, cmd, TraceManager.INFO)


def trace_op(parent, op):
    _trace(parent, op, TraceManager.DETAIL)


def _trace(parent, msg, level):
    if (TraceManager().is_trace_active(parent)
            and TraceManager().is_level_on(level)):
        print('[cnippet-%s] %s' % (parent, msg))


class TraceManager(metaclass=Singleton):

    DISABLED = ''
    INFO = 'info'
    DETAIL = 'detail'

    def __init__(self):
        self.active = set()
        self.level = TraceManager.DISABLED

    def configure(self, traces, level):
        self.active = set(traces) if traces else set()
        self.level = level

    def is_trace_active(self, parent):
        return parent in self.active or 'all' in self.active

    def is_level_on(self, level):
        if self.level == TraceManager.DISABLED:
            return False
        if self.level == TraceManager.DETAIL:
            return True
        return True if level == TraceManager.INFO else False
