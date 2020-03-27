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
from Diagnostics import DiagnosticReporter, ERROR_FETCHING_GIT_SHA


class Version:
    """ Cnippet's version information """

    _id = 'version'

    major = 1
    minor = 0
    patch = 0

    description = 'pre-alpha evaluation'

    def __str__(self):
        return self.__repr__()

    def __repr__(self):
        return '%s.%s.%s %s' % (Version.major,
                                Version.minor,
                                Version.patch,
                                Version.description)

    @staticmethod
    def number():
        return '%s.%s.%s' % (Version.major,
                             Version.minor,
                             Version.patch)

    @staticmethod
    def git_sha():
        """
        Get git HEAD's sha.
        """

        cmd = ['git', 'rev-parse', 'HEAD']
        process = subprocess.Popen(cmd, stdout=subprocess.PIPE)
        out, err = process.communicate()
        if err:
            sys.exit(DiagnosticReporter.fatal(ERROR_FETCHING_GIT_SHA))
        return out[:7]
