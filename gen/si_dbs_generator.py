#!/usr/bin/python

import os
import time
from struct import *

from si_dbs_parameter_creator import si_dbs_parameter_creator
from si_dbs_packet_creator import si_dbs_packet_creator
from si_dbs_textual_calibration_creator import si_dbs_textual_calibration_creator
from si_dbs_numerical_calibration_creator import si_dbs_numerical_calibration_creator
from si_dbs_runtime_creator import si_dbs_runtime_creator
from si_dbs_displays_creator import si_dbs_displays_creator


# globals
ascii_dir = os.getenv('ASCII_DIR')
tables_dir = os.getenv('SCOSI_TABLE_DIR')
runtime_dir = os.getenv('RUNTIME_DIR')
displays_dir = os.getenv('DISPLAYS_DIR')
dbgen_dir = os.getenv('DBGEN_DIR')

text_calib_creator = si_dbs_textual_calibration_creator(ascii_dir, tables_dir)
numeric_calib_creator = si_dbs_numerical_calibration_creator(ascii_dir, tables_dir)

calib_creators = {}
calib_creators['text'] = text_calib_creator
calib_creators['numeric'] = numeric_calib_creator

param_creator = si_dbs_parameter_creator(ascii_dir, tables_dir, calib_creators)
packet_creator = si_dbs_packet_creator(ascii_dir, tables_dir, param_creator)
runtime_creator = si_dbs_runtime_creator(ascii_dir, runtime_dir, param_creator)
displays_creator = si_dbs_displays_creator(ascii_dir, displays_dir)

creators = [text_calib_creator, numeric_calib_creator, param_creator, packet_creator, runtime_creator, displays_creator]

for creator in creators:
    creator.generate()


