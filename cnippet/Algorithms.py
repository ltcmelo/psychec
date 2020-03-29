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
import shutil
import sys
from Diagnostics import (DiagnosticReporter,
                         EXCEPTION_COPYING_FILE_OBJECT,
                         EXCEPTION_COPYING_FILE_PATH)


def maybe_append(value, condition, sequence):
    return sequence.append(value) if condition else sequence


def delete_files(*files_path):
    for p in files_path:
        try:
            os.remove(p)
        except OSError:
            pass


def concat_file(src_path, dst_path):
    with open(dst_path, 'ab') as dst:
        with open(src_path, 'rb') as src:
            try:
                shutil.copyfileobj(src, dst)
            except:
                sys.exit(DiagnosticReporter.fatal(EXCEPTION_COPYING_FILE_OBJECT,
                                                  src_path, dst_path))


def copy_file(src_path, dst_path):
    try:
        shutil.copyfile(src_path, dst_path)
    except:
        sys.exit(DiagnosticReporter.fatal(EXCEPTION_COPYING_FILE_PATH, src_path, dst_path))


def flatten(l):
    return ' '.join(l)
