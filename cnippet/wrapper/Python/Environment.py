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


import os
import sys
from Diagnostics import DiagnosticReporter, ERROR_CREATING_CONFIG_DIRECTORY
from LicenseFile import LicenseFile
from Logger import debug


class EnvironmentController:
    """
    Controls environment data.
    """

    _id = 'env'

    def __init__(self, home_dir_path):
        self.home_dir_path = home_dir_path
        self.config_dir_path = os.path.join(home_dir_path, '.cnippet')

    def _ensure_config_dir_exists(self):
        """
        Ensure that the application directory exists.
        """

        if not os.path.isdir(self.config_dir_path):
            debug(EnvironmentController._id,
                     'create config directory in %s' % self.home_dir_path)
            try:
                os.makedirs(self.config_dir_path)
            except OSError:
                # Check again due to concurrent access.
                if not os.path.isdir(self.config_dir_path):
                    sys.exit(
                        DiagnosticReporter.fatal(ERROR_CREATING_CONFIG_DIRECTORY))

    def _verify_license(self):
        """
        Verify whether a license file exists and check its content.
        """

        lic_path = os.path.join(self.config_dir_path, 'license.lic')
        LicenseFile(lic_path).verify()

    def check_all(self, non_commercial_use):
        """
        Perform checks...
        """

        self._ensure_config_dir_exists()
        if not non_commercial_use:
            self._verify_license()
