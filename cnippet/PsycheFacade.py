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


class PsycheFacade:
    """
    A facade to the Psyche inference engine.
    """

    _id = 'psychec'

    def __init__(self, cnip_opt):
        self.generator = 'psychecgen'
        self.solver = 'psychecsolver-exe'
        self.no_typedef = cnip_opt['no_typedef']
        self.no_heuristic = cnip_opt['no_heuristic']
        self.no_stdlib = cnip_opt['no_stdlib']
        self.cc = cnip_opt['cc']

    def generate_constraints(self,
                             unit: Unit,
                             cc_cmd):
        """
        Generate constraints for a unit.
        """

        cmd = [self.generator,
               unit.c_file_path,
               '-o', unit.cstr_file_path,
               '--cc', self.cc,
               '--cc-std', cc_cmd.c_version]

        cmd += CCompilerFacade.predefined_macros('--cc-D')
        cmd += CCompilerFacade.undefined_macros('--cc-U')

        maybe_append('--no-typedef', self.no_typedef, cmd)
        maybe_append('--no-heuristic', self.no_heuristic, cmd)

        if not self.no_stdlib:
            cmd.append('-p')
            dir_path = pathlib.Path(__file__).parent.parent.absolute()
            cmd.append(os.path.join(dir_path, 'libpsychecstd'))

        code = execute(PsycheFacade._id, cmd)
        if code != 0:
            sys.exit(
                DiagnosticReporter.fatal(CONSTRAINT_GENERATION_FOR_FILE_FAILED,
                                         unit.c_file_path,
                                         error=code))

    def solve_constraints(self, unit: Unit):
        """
        Solve the constraint.
        """

        cmd = [self.solver,
               '--',
               '-i',
               unit.cstr_file_path,
               '-o',
               unit.cnip_file_path]

        if not self.no_stdlib:
            cmd.append('--match-stdlib=approx')

        ok = execute(PsycheFacade._id, cmd)
        if ok != 0:
            sys.exit(
                DiagnosticReporter.fatal(CONSTRAINT_SOLVING_FOR_FILE_FAILED,
                                         unit.c_file_path))
