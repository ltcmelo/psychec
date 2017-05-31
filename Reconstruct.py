#!/usr/bin/python

import sys
import os
from subprocess import call


log_enabled = False


class PartialProgramAnalyser:
    """ Partial program analyser """

    def __init__(self, in_file_name):
        self.in_file_name = in_file_name
        self.gen_file_name = in_file_name[:-2] + "_gen.h"
        self.fixed_file_name = in_file_name[:-2] + "_fixed.c"

    @staticmethod
    def write_log(msg):
        if log_enabled:
            print "[Reconstructor] %s" % msg

    def gen_constraints(self):
        """ Invoke our constraint generator """
        self.write_log("Generating constraints")

        # If there's an old constraints file, delete it.
        if os.path.isfile("a.cstr"):
            os.remove("a.cstr")

        call(["./psychecgen", "%s" % self.in_file_name])

    def solve_constraints(self):
        """ Invoke our constraint solver """
        self.write_log("Solving constraints")

        os.chdir("solver")
        call(["stack", "exec", "psychecsolver-exe", "--",
              "-i", "../a.cstr",
              "-o", "../%s" % self.gen_file_name])
        os.chdir("..")

    def fix_program(self):
        """ Create a new source that includes the solved stuff"""
        self.write_log("Creating new complete source")

        content = "/* Reconstructed from %s */\n" % self.in_file_name
        local_gen_file = self.gen_file_name.split('/')[-1:][0]
        content += '#include "%s"\n' % local_gen_file

        with open(self.in_file_name, "r") as f:
            content += f.read()

        with open(self.fixed_file_name, "w") as f:
            f.write(content)

    def analyse(self):
        self.gen_constraints()
        self.solve_constraints()
        self.fix_program()


if __name__ == "__main__":
    if (len(sys.argv)) != 2:
        print "Usage: ./Reconstruct.py relative/path/file.c"
        sys.exit(1)

    analyser = PartialProgramAnalyser(sys.argv[1])
    analyser.analyse()
