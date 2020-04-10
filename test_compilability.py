#!/usr/bin/env python

import collections
import glob
import os
import subprocess
import sys

Expectation = collections.namedtuple('Expectation',
                                     ['IGNORE',
                                      'LEGITIMATE_ERROR',
                                      'KNOWN_ISSUE'])

baseline = Expectation(
    IGNORE={'T118.c'},
    LEGITIMATE_ERROR={'T211.c'},
    KNOWN_ISSUE={'T4.c',
                 'T165.c',
                 'T229.c'})


class Runner:
    def __init__(self,
                 cc: str,
                 c_files_dir: str,
                 expect: Expectation):
        self.cc = cc
        self.c_files_dir = c_files_dir

        self.expect = Expectation(
            IGNORE={os.path.join(c_files_dir, fn)
                    for fn in expect.IGNORE},
            LEGITIMATE_ERROR={os.path.join(c_files_dir, fn)
                              for fn in expect.LEGITIMATE_ERROR},
            KNOWN_ISSUE={os.path.join(c_files_dir, fn)
                         for fn in expect.KNOWN_ISSUE})

        self.cc_flags = '-Werror '\
                        '-Wall '\
                        '-Wno-incompatible-library-redeclaration '\
                        '-Wno-uninitialized '\
                        '-Wno-unused-variable '\
                        '-Wno-unused-function '\
                        '-Wno-switch '\
                        '-Wno-unused-value '\
                        '-Wno-implicit-int '\
                        '-Wno-return-type '\
                        '-Wno-builtin-requires-header '\
                        '-Wno-infinite-recursion '\
                        '-c '

    def _remove_old_stuff(self):
        exts = ['.o', '.cnip', '.cstr', '.inc']

        pat = os.path.join(self.c_files_dir, '*.c')
        for f in glob.glob(pat):
            for x in exts:
                fx = f + x
                try:
                    os.remove(fx)
                except:
                    pass

    def _execute_cnip(self, c_file_path: str):

        print(c_file_path)
        cmd = ['cnip.sh',
               '-f',
               '--no-stdlib',
               self.cc,
               self.cc_flags,
               c_file_path,
               '-o', c_file_path + '.o']
        try:
            return subprocess.call(cmd)
        except Exception as e:
            print(f'error in command {" ".join(cmd)}\n{e}')
            sys.exit(1)

    @staticmethod
    def _print_info(action: str,
                    c_file_path: str):
        prefix = '^' * len(action)
        print(f'\n  {prefix}\n  {action} ({c_file_path})\n')

    def _print_summary(self):
        s = f'\n  IGNORED: {len(self.expect.IGNORE)}' \
            f'\n  KNOWN ISSUES: {len(self.expect.KNOWN_ISSUE)}'
        print(s)

    def run(self):
        self._remove_old_stuff()

        pat = os.path.join(self.c_files_dir, '*.c')
        for f in glob.glob(pat):
            ok = self._execute_cnip(f)
            if ok == 0:
                continue

            if f in self.expect.IGNORE:
                Runner._print_info('IGNORE', f)
            elif f in self.expect.LEGITIMATE_ERROR:
                Runner._print_info('LEGITIMATE ERROR', f)
            elif f in self.expect.KNOWN_ISSUE:
                Runner._print_info('KNOWN ISSUE', f)
            else:
                sys.exit(1)

        self._print_summary()


if __name__ == '__main__':

    r = Runner('clang',
               'solver/test/cases',
               baseline)
    r.run()
