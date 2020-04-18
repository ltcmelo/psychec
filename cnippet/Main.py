#!/usr/bin/env python

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


import argparse
import os
import sys
from Driver import Driver
from Environment import EnvironmentController
from Version import Version
from Logger import Logger


def _parse_input():
    """
    Parse input, identify options, the host compiler and command.
    """

    parser = argparse.ArgumentParser(
        add_help=False,
        formatter_class=argparse.RawDescriptionHelpFormatter,
        description='\tcnippet - the compiler for C snippets\n',
        epilog='examples:\n'
               '  $cnip gcc file.c\n'
               '  $cnip -f gcc file.c -o exe\n'
               '  $cnip -f --no-stdlib gcc -c file.c\n')

    version_str = '%s' % Version()
    copyright_str = 'Copyright 2017 Leandro T. C. Melo'

    parser.add_argument('-v', '--version',
                        action='version',
                        version='%(prog)s ' + version_str + '\n' + copyright_str,
                        help="Show program's version number and exit.")

    parser.add_argument('-h', '--help',
                        action='help',
                        default=argparse.SUPPRESS,
                        help='Show this help message and exit.')

    parser.add_argument('--no-heuristic',
                        action='store_true',
                        help='Disable heuristics upon unresolved ambiguous syntax.')

    parser.add_argument('--no-typedef',
                        action='store_true',
                        help="Forbid 'typedef','struct', and 'union' declarations.")

    parser.add_argument('--no-stdlib',
                        action='store_true',
                        help="Don't attempt to match stdlib names.")

    parser.add_argument('-s', '--only-omissive',
                        action='store_true',
                        help='Only consider "omissive" functions for inference.')

    parser.add_argument('-p', '--generics',
                        action='store_true',
                        help='Enable parmetric polimorphism.')

    parser.add_argument('-f', '--non-commercial',
                        action='store_true',
                        help="Specify non-commercial use.")

    parser.add_argument('-g', '--debug',
                        action='append',
                        choices=['all'],
                        help='Debug specified component(s).')

    parser.add_argument('-x', '--trace-external',
                        action='store_true',
                        help='Trace invocation of external processes.')

    parser.add_argument('CC',
                        help="The host C compiler (e.g., 'gcc' or 'clang').")

    parser.add_argument('cc_cmd_line',
                        metavar='...',
                        nargs=argparse.REMAINDER,
                        help='The command to be forwarded to the C compiler.')

    # Hidden arguments.
    parser.add_argument('-d', '--dev',
                        action='store_true',
                        help=argparse.SUPPRESS)

    return parser.parse_args()


if __name__ == "__main__":

    args = _parse_input()

    # TODO: Eliminate "extra" dictionary.
    cnip_opts = dict(no_stdlib=args.no_stdlib,
                    no_typedef=args.no_typedef,
                    no_heuristic=args.no_heuristic,
                    only_omissive=args.only_omissive,
                    cc=args.CC,
                    cc_cmd_line=args.cc_cmd_line,
                    dev_mode=args.dev)

    Logger().configure(args.trace_external, args.debug)

    env = EnvironmentController(os.path.expanduser('~'))
    env.check_all(args.non_commercial)

    if args.dev:
        run_dir = os.path.dirname(os.path.realpath(__file__))
        os.environ['LD_LIBRARY_PATH'] = run_dir + '/../Plugin'
        os.environ['PATH'] += os.pathsep + run_dir + '/../External/PsycheC'

    # Let' go!
    driver = Driver(cnip_opts)
    code = driver.execute()
    sys.exit(code)
