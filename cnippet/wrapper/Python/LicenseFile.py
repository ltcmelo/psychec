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


import base64
import datetime
import hashlib
import os
import re
import sys
from Diagnostics import (DiagnosticReporter,
                         LICENSE_FILE_NOT_FOUND,
                         CORRUPTED_LICENSE_FILE,
                         INCOMPATIBLE_LICENSE_PRODUCT_VERSION,
                         LICENSE_EXPIRED)
from Version import Version


class LicenseFile:
    """
    The license file.
    """

    def __init__(self, path):
        """
        Validate and initialize data.
        """

        if not os.path.isfile(path):
            sys.exit(DiagnosticReporter.fatal(LICENSE_FILE_NOT_FOUND))

        if os.stat(path).st_size == 0:
            sys.exit(DiagnosticReporter.fatal(CORRUPTED_LICENSE_FILE))

        with open(path, 'r') as f:
            content = f.read()
        lines = content.splitlines()

        if len(lines) != 6:
            sys.exit(DiagnosticReporter.fatal(CORRUPTED_LICENSE_FILE))

        self.product = LicenseFile._check_line(lines[1]).strip()
        self.version = LicenseFile._check_line(lines[2]).strip()
        self.key = LicenseFile._check_line(lines[3]).strip()
        self.expiration = LicenseFile._check_line(lines[4]).strip()
        self.email = LicenseFile._check_line(lines[5]).strip()

        version_pat = re.compile('\d+\.\d+\.\d+')
        if not version_pat.match(self.version):
            sys.exit(DiagnosticReporter.fatal(CORRUPTED_LICENSE_FILE))

    def verify(self):
        """
        Verify integrity of the license file is integral, cnippet's version, and
        expiration date.
        """

        secret = '$*we#j238@#WA!%'
        h = hashlib.sha1()
        h.update(secret[10:12])
        h.update(base64.b64encode(self.product))
        h.update(secret[1:4])
        h.update(self.version[0])
        h.update(secret[6:9])
        h.update(self.email)
        h.update(self.expiration)
        digest = h.hexdigest()

        # If the hash doesn't match, data has been corrupted.
        if self.key != digest:
            sys.exit(DiagnosticReporter.fatal(CORRUPTED_LICENSE_FILE))

        # Verify product version.
        if int(self.version[0]) != Version().major:
            sys.exit(DiagnosticReporter.fatal(INCOMPATIBLE_LICENSE_PRODUCT_VERSION))

        # Verify expiration.
        exp_date = datetime.datetime.strptime(self.expiration, '%d/%B/%Y')
        if datetime.datetime.today() > exp_date:
            sys.exit(DiagnosticReporter.fatal(LICENSE_EXPIRED))

    @staticmethod
    def _check_line(line):
        """
        Check the line format.
        """
        parts = line.split(':')
        if len(parts) != 2:
            sys.exit(DiagnosticReporter.fatal(CORRUPTED_LICENSE_FILE))
        return parts[1]
