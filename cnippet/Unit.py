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
    'Unit',
    [
        'c_file_path',     # The original C file.
        'i_file_path',     # A preprocessed version of the original C file.
        'cstr_file_path',  # The constraints file.
        'inc_file_path',   # A file with a list of stdlib `#include's.
        'poly_file_path',  # A "polymorphic" version of the original C file.
        'cnip_file_path'   # The file containing inference results.
    ])


def make_unit(c_file_path, out_dir):
    """
    Make a compilation unit.
    """

    (_, file_name) = os.path.split(c_file_path)
    (base_name, _) = os.path.splitext(file_name)
    gen_path = out_dir + base_name

    return Unit(c_file_path,
                gen_path + '.i',
                gen_path + '.cstr',
                gen_path + '.inc',
                gen_path + '.poly',
                gen_path + '.cnip')
