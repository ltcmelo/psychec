# Copyright (c) Leandro T. C. Melo

from os import listdir, path
from os.path import isfile, join
import subprocess
import sys

# A utility that updates the expected C output files of the solver's tests. Particularly
# useful for batch operations, when there's change in format/style. Of course, it should
# only be used if we're certain the generated files are indeed correct - at the minimum,
# the script CompileTests.sh must be used. The new expected it take directly from the run.


def fix():
    dir = "solver/test/cases/"
    all_files = [f for f in listdir(dir) if isfile(join(dir, f))]
    c_files = [f for f in all_files if f.endswith(".c") and not f.endswith("_fixed.c")]

    for cfile in c_files:
        full_name = dir + cfile

        invok = ["python", "Reconstruct.py", full_name]
        print "Invocation: %s" % " ".join(invok)
        subprocess.call(invok)

        generated = full_name[:-2] + "_gen.h"
        if not path.isfile(generated):
            print "*** Skipping... File not generated!"
            continue

        with open(generated, "r") as f:
            content = f.read()

        new_content = content[content.find("/* Forward declarations */"):]
        expected = full_name + ".expected"
        with open(expected, "w") as f:
            f.write(new_content)


if __name__ == "__main__":
    fix()
