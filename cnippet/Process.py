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


import sys
import subprocess
from Algorithms import *
from Diagnostics import *
from Tracing import *


def call_process(caller_id, cmd, *args, **kwargs):
    """ Invoke external process with the given command. """

    trace_extern_cmd(caller_id, list2str(cmd))
    try:
        code = subprocess.call(cmd, *args, **kwargs)
    except:
        sys.exit(DiagnosticReporter.fatal(EXCEPTION_INVOKING_PROCESS, cmd[0]))
    return code
