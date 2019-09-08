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
from Algorithms import *
from CommandLine import *
from CompilerFacade import *
from Diagnostics import *
from PsycheFacade import *
from Tracing import *
from Unit import *


class Driver:
    """ The compiler's driver """

    _id = 'driver'

    def __init__(self, cnip_opt):
        self.cnip_opt = cnip_opt
        self.cc = CompilerFacade(cnip_opt)
        self.psyche = PsycheFacade(cnip_opt)


    @staticmethod
    def _delete_old_files(unit):
        """ Delete old files from any previous run. """

        remove_if_exists(unit.cstr_file_path)
        remove_if_exists(unit.inc_file_path)
        remove_if_exists(unit.cnip_file_path)


    def _compile_unit(self, unit, cc_opts):
        """ Perform the entire type-inference workflow for each target unit. """

        # Possible old files might exist, delete them.
        Driver._delete_old_files(unit)

        # Invoke the psyche generator with the stdlib plugin, which will: (i) identify whether an
        # stdlib header is necessary; (ii) preprocess the "new" source, containing such includes;
        # and (iii) generate the constraints.
        self.psyche.generate(unit, cc_opts)

        # Constraints are produced only if the original source is incomplete (i.e. when type
        # inference is required).
        if not os.path.isfile(unit.cstr_file_path):
            # When there's nothing to solve, we make the original source the final one.
            copy_file(unit.c_file_path, unit.cnip_file_path)
        else:
            self.psyche.solve(unit)

            # Append the original source or its polymorphic version to the file we inferred.
            # If missing stdlib headers were detected, account for the "include list" file.
            if os.path.isfile(unit.poly_file_path):
                concat_file(unit.poly_file_path, unit.cnip_file_path)
            else:
                concat_file(unit.c_file_path, unit.cnip_file_path)
            if os.path.isfile(unit.inc_file_path):
                concat_file(unit.cnip_file_path, unit.inc_file_path)
                copy_file(unit.inc_file_path, unit.cnip_file_path)


    def execute(self):
        """ Entry point for the driver. """

        trace_op(Driver._id, list2str(self.cnip_opt['host_cc_cmd']))

        if not self.cc.verify_support():
            sys.exit(DiagnosticReporter.fatal(HOST_C_COMPILER_NOT_FOUND))

        cc_opts = self.cc.parse_command()

        # The directory for the generated files is the same as the output directory
        # specified in the host C compiler's command. Unless, none is provided. In such
        # case, the working directory is used (just like GCC does).
        if not cc_opts.out_file_name:
            gen_dir = ''
        else:
            (gen_dir, _) = os.path.split(cc_opts.out_file_name)
            if gen_dir:
                gen_dir += '/'
            else:
                gen_dir = ''  # Output specified as working directory.

        # The "new" command to be passed to the host C compiler has the same structure as
        # the one originally provided by the user. But input source file will be replaced
        # by those manipulated by us.
        new_cmd = self.cnip_opt['host_cc_cmd']

        for c_file_path in cc_opts.sources:
            if not os.path.isfile(c_file_path):
                sys.exit(DiagnosticReporter.fatal(FILE_DOES_NOT_EXIST, c_file_path))

            # If the original source already passes the "syntax" check, we don't touch it. But if
            # an error is diagnosed, it could be due to an absent declaration.
            if self.cc.check_syntax(c_file_path) == 0:
                continue

            unit = make_unit(c_file_path, gen_dir)
            self._compile_unit(unit, cc_opts)

            # Replace, in the command line, the source originally provided by the user
            # for the one we have produced (already preprocessed).
            trace_op(Driver._id, 'replace %s for %s in command' % (unit.c_file_path,
                                                                   unit.cnip_file_path))
            new_cmd = [w.replace(unit.c_file_path, unit.cnip_file_path) for w in new_cmd]

        # In the final command, enforce C compilation mode, since we're compiling a file
        # with the `.cnip' extension (no worry about duplicating this option).
        cmd = [self.cnip_opt['host_cc'], '-x', 'c'] + new_cmd
        ok = call_process(Driver._id, cmd)
        if ok != 0:
            sys.exit(DiagnosticReporter.fatal(HOST_C_COMPILER_FORWARDING_FAILED))
        return 0
