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
import pathlib
import sys
from Algorithms import maybe_append
from CCompilerFacade import CCompilerFacade
from Diagnostics import (DiagnosticReporter,
                         CONSTRAINT_GENERATION_FOR_FILE_FAILED,
                         CONSTRAINT_SOLVING_FOR_FILE_FAILED)
from Process import execute
from Unit import Unit


class PsycheCFacade:
    """
    A facade to the Psyche inference engine.
    """

    @staticmethod
    def ID():
        return PsycheCFacade.__name__

    _GENERATOR = 'psychecgen'
    _SOLVER = 'psychecsolver-exe'

    def __init__(self, cnip_opts):
        self.cc = cnip_opts['cc']
        self.no_typedef = cnip_opts['no_typedef']
        self.no_heuristic = cnip_opts['no_heuristic']
        self.no_stdlib = cnip_opts['no_stdlib']

    def generate_constraints(self,
                             unit: Unit,
                             cc_cmd_summary):
        """
        Generate constraints for a unit.
        """

        cmd = [PsycheCFacade._GENERATOR,
               unit.c_file,
               '-o', unit.cstr_file,
               '--cc', self.cc,
               '--cc-std', cc_cmd_summary.c_version]

        cmd += CCompilerFacade.defined_macros('--cc-D')
        cmd += CCompilerFacade.undefined_macros('--cc-U')

        cmd += cc_cmd_summary.defined_macros('-cc-D ')
        cmd += cc_cmd_summary.undefined_macros('--cc-U ')
        cmd += cc_cmd_summary.include_paths('--cc-I ')

        maybe_append('--no-typedef', self.no_typedef, cmd)
        maybe_append('--no-heuristic', self.no_heuristic, cmd)

        if not self.no_stdlib:
            cmd.append('-p')
            dir_path = pathlib.Path(__file__).parent.parent.absolute()
            cmd.append(os.path.join(dir_path, 'libpsychecstd'))

        code = execute(PsycheCFacade.ID(), cmd)
        if code != 0:
            sys.exit(
                DiagnosticReporter.fatal(CONSTRAINT_GENERATION_FOR_FILE_FAILED,
                                         unit.c_file,
                                         error=code))

    def solve_constraints(self, unit: Unit):
        """
        Solve the constraint.
        """

        cmd = [PsycheCFacade._SOLVER,
               '--',
               '-i',
               unit.cstr_file,
               '-o',
               unit.cnip_file]

        if not self.no_stdlib:
            cmd.append('--match-stdlib=approx')

        ok = execute(PsycheCFacade.ID(), cmd)
        if ok != 0:
            sys.exit(
                DiagnosticReporter.fatal(CONSTRAINT_SOLVING_FOR_FILE_FAILED,
                                         unit.c_file))
