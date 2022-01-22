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
from collections import namedtuple


Unit = namedtuple(
    'Unit', [
        'c_file',     # The original C file.
        'i_file',     # A preprocessed version of the original C file.
        'cstr_file',  # The constraints file.
        'inc_file',   # A file with a list of stdlib `#include's.
        'poly_file',  # A "polymorphic" version of the original C file.
        'cnip_file'   # The file containing inference results.
    ])


def make_unit(c_file, out_dir):
    """
    Make a compilation unit.
    """

    (_, c_file_name) = os.path.split(c_file)
    (c_file_base_name, _) = os.path.splitext(c_file_name)
    out_file_base_name = out_dir + c_file_base_name

    return Unit(c_file,
                out_file_base_name + '.i',
                out_file_base_name + '.cstr',
                out_file_base_name + '.inc',
                out_file_base_name + '.poly',
                out_file_base_name + '.cnip.c')
