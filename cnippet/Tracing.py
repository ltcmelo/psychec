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


from Singleton import *


def trace_extern_cmd(caller_id, cmd):
    _trace(caller_id, cmd, TraceManager.INFO)


def trace_op(caller_id, op):
    _trace(caller_id, op, TraceManager.DETAIL)


def _trace(caller_id, msg, level):
    if TraceManager().is_trace_active(caller_id) and TraceManager().is_level_on(level):
        print('[cnippet-%s] %s' % (caller_id, msg))


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

    def is_trace_active(self, caller_id):
        return caller_id in self.active or 'all' in self.active

    def is_level_on(self, level):
        if self.level == TraceManager.DISABLED:
            return False
        if self.level == TraceManager.DETAIL:
            return True
        return True if level == TraceManager.INFO else False
