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
from Algorithms import flatten
from Diagnostics import DiagnosticReporter, EXCEPTION_EXECUTING_PROCESS
from Tracing import trace_extern_cmd


def execute(parent, cmd, *args, **kwargs):
    """
    Execute an external process with the given command.
    """

    trace_extern_cmd(parent, flatten(cmd))
    try:
        return subprocess.call(cmd, *args, **kwargs)
    except:
        sys.exit(
            DiagnosticReporter.fatal(EXCEPTION_EXECUTING_PROCESS,
                                     cmd[0]))
