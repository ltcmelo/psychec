#!/usr/bin/python


import sys
import os
from subprocess import call


class PartialProgramAnalyser:
    """ Partial program analyser """

    def __init__(self, in_file_name):
        self.in_file_name = in_file_name
        self.out_file_name = in_file_name + ".ctr"
        self.gen_file_name = in_file_name + ".gen.h"
        self.fixed_in_file_name = in_file_name[:-1] + "fixed.c"

    @staticmethod
    def write_log(msg):
        print "[Reconstructor] %s" % msg

    def gen_constraints(self):
        """ Invoke our constraint generator """
        self.write_log("Generating constraints")

        call(["./Gen", "%s" % self.in_file_name, "clean"])

    def solve_constraints(self):
        """ Invoke our constraint solver """
        self.write_log("Solving constraints")

        os.chdir("solver")
        # If there's an old generated file, delete it.
        if os.path.isfile("../%s" % self.gen_file_name):
            os.remove("../%s" % self.gen_file_name)
        call(["stack", "exec", "psychecsolver-exe", "--",
              "-i", "../%s" % self.out_file_name,
              "-o", "../%s" % self.gen_file_name])
        os.chdir("..")

    def fix_program(self):
        """ Create a new source (based on the original) that includes the solved stuff"""
        self.write_log("Creating new complete source")

        content = "/* Reconstructed from %s */\n" % self.in_file_name
        local_gen_file = self.gen_file_name.split('/')[-1:][0]
        content += '#include "%s"\n' % local_gen_file

        with open(self.in_file_name, "r") as f:
            content += f.read()

        with open(self.fixed_in_file_name, "w") as f:
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
