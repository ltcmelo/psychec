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


import os.path
import sys
from Algorithms import delete_files, flatten, concat_file, copy_file
from CCompilerFacade import CCompilerFacade
from Diagnostics import (DiagnosticReporter,
                         FILE_DOES_NOT_EXIST,
                         HOST_C_COMPILER_FORWARDING_FAILED,
                         HOST_C_COMPILER_NOT_FOUND)
from Process import execute
from PsycheFacade import PsycheFacade
from Logger import debug
from Unit import make_unit


class Driver:
    """
    The driver.
    """

    _id = 'driver'

    def __init__(self, cnip_opts):
        self.cnip_opts = cnip_opts
        self.cc: CCompilerFacade = CCompilerFacade(cnip_opts)
        self.psyche: PsycheFacade = PsycheFacade(cnip_opts)

    @staticmethod
    def _delete_old_files(unit):
        """
        Delete old files, from any previous run.
        """

        delete_files(unit.i_file_path,
                     unit.cstr_file_path,
                     unit.inc_file_path,
                     unit.poly_file_path,
                     unit.cnip_file_path)

    def _compile_unit(self, unit, cc_cmd):
        """
        Perform the entire type-inference workflow for a unit.
        """

        Driver._delete_old_files(unit)

        self.psyche.generate_constraints(unit, cc_cmd)

        if not os.path.isfile(unit.cstr_file_path):
            copy_file(unit.c_file_path, unit.cnip_file_path)
            return

        self.psyche.solve_constraints(unit)

        if os.path.isfile(unit.poly_file_path):
            concat_file(unit.poly_file_path, unit.cnip_file_path)
        else:
            concat_file(unit.c_file_path, unit.cnip_file_path)

        if os.path.isfile(unit.inc_file_path):
            concat_file(unit.cnip_file_path, unit.inc_file_path)
            copy_file(unit.inc_file_path, unit.cnip_file_path)

    def execute(self):
        """
        Entry point.
        """

        debug(Driver._id, flatten(self.cnip_opts['cc_cmd_line']))

        if not self.cc.is_supported():
            sys.exit(DiagnosticReporter.fatal(HOST_C_COMPILER_NOT_FOUND))

        cc_cmd = self.cc.parse_command()

        if not cc_cmd.out_file_name:
            gen_dir = ''
        else:
            (gen_dir, _) = os.path.split(cc_cmd.out_file_name)
            if gen_dir:
                gen_dir += '/'
            else:
                gen_dir = ''

        # The adjusted command that is forwarded to the host C compiler is the
        # one provided by the user, with the input file replaced.
        new_cmd = self.cnip_opts['cc_cmd_line']

        for c_file in cc_cmd.c_files:
            if not os.path.isfile(c_file):
                sys.exit(DiagnosticReporter.fatal(FILE_DOES_NOT_EXIST, c_file))

            if self.cc.check_syntax(c_file) == 0:
                # If there are missing declarations in the source, this check
                # would've failed. Since it didn't, there's nothing to infer.
                continue

            unit = make_unit(c_file, gen_dir)
            self._compile_unit(unit, cc_cmd)

            debug(Driver._id,
                     f'replace {unit.c_file_path} for {unit.cnip_file_path} in command')
            new_cmd = [w.replace(unit.c_file_path, unit.cnip_file_path)
                       for w in new_cmd]

        cmd = [self.cnip_opts['cc'],
               '-x',
               'c'] + new_cmd

        ok = execute(Driver._id, cmd)
        if ok != 0:
            sys.exit(DiagnosticReporter.fatal(HOST_C_COMPILER_FORWARDING_FAILED))

        return 0
