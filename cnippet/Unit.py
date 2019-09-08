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


def make_unit(c_file_path, out_dir):
    """ Make compilation unit data """

    (_, file_name) = os.path.split(c_file_path)
    (base_name, _) = os.path.splitext(file_name)
    gen_path = out_dir + base_name

    Unit = namedtuple('Unit', [
                               'c_file_path',    # Original C file.
                               'i_file_path',    # Preprocessed (original) C file.
                               'cstr_file_path', # Constraints file.
                               'inc_file_path',  # Include-list file.
                               'poly_file_path', # File in polymorhpic version.
                               'cnip_file_path'  # Inferred file.
                              ])
    return Unit(c_file_path,
                gen_path + '.i',
                gen_path + '.cstr',
                gen_path + '.inc',
                gen_path + '.poly',
                gen_path + '.cnip')
