#***************************************************************************
#
# PROJECT  : SCOS-I TM 
#
# SUBSYSTEM: PYTHON_DBS
#
# FILENAME : si_dbs_numerical_calibration_creator.py
#
# AUTHOR   : Ritchie Kay 
#
# DATE     : 20.06.2010
#
# COPYRIGHT: Ritchie Kay, 2010
#
#***************************************************************************
#
# PURPOSE: si_dbs_numerical_calibration_creator creates the calib table
#          The calib table defines the characteristics of 
#          calibration curves  
#
#***************************************************************************/
#
# The structure of the calibration table is:
#
# ++++++++++++++++++++++++++++++++++++++++
# |  Type   |  Bytes  |  Purpose         |
# ++++++++++++++++++++++++++++++++++++++++
# |  char   |    1    |  calib type      |
# |  uint   |    4    |  entries         |
# ++++++++++++++++++++++++++++++++++++++++
# | Repeats 'entries' times              |
# ++++++++++++++++++++++++++++++++++++++++
# |  int    |    4    |  raw value_1     |
# |  int    |    4    |  raw value_2     |
# |  double |    4    |  calib value_1   |
# |  double |    4    |  calib value_2   |
# ++++++++++++++++++++++++++++++++++++++++
#
import time
from struct import *

from si_dbs_table_creator import si_dbs_table_creator
from si_dbs_param_utilities import *

class si_dbs_numerical_calibration_creator(si_dbs_table_creator):

    __calib_file = ''
    __caf_file = ''
    __cap_file = ''
    __numerical_location = {}
    __numerical_location = {}

    def __init__(self, ascii_dir, table_dir):
        super(si_dbs_numerical_calibration_creator, self).__init__(ascii_dir, table_dir)
        self.__calib_file = self.output_dir() + '/numeric_calib.tlb'
        self.__caf_file = self.input_dir() + '/caf.dat'
        self.__cap_file = self.input_dir() + '/cap.dat'

    def generate(self):
        self.create_numerical()

    def numerical_location(self):
        return self.__numerical_location

    def numerical_location(self):
        return self.__numerical_location

    def create_numerical(self):

        calib = open(self.__calib_file, 'wb')
        caf = open(self.__caf_file, 'r')
        cap = open(self.__cap_file, 'r')

        caf_entries = {}
        cap_entries = {}

        for line in caf:
            fields = line.strip().split('\t')
            caf_entries[fields[0]] = fields[2:5]
            cap_entries[fields[0]] = []

        for line in cap:
            fields = line.strip().split('\t')
	    rawVal = 0
	    if caf_entries[fields[0]][1] == 'U':
	        if caf_entries[fields[0]][2] == 'O':
		   rawVal = int(fields[1], 8)
                elif caf_entries[fields[0]][2] == 'H':
		   rawVal = int(fields[1], 16)
                else:
	           rawVal = fields[1]
	    engVal = float(fields[2])
            cap_entries[fields[0]].append([rawVal, engVal])

        calib.write(pack('I',len(cap_entries)))
        position = 4

        keys = cap_entries.keys()
	keys.sort()

        for index in keys:
            self.__numerical_location[index] = position
            cap_entries[index].sort()
            # Not enumeration so we write out all the records
            # Write out the type of the calibration, in this case an enumeration
            data = pack('cI', 'N', len(cap_entries[index])-1)
            calib.write(data)
            position += len(data)

            # Go through every record
            # We create records starting composed of two adjacent entries
            for i in range(len(cap_entries[index])-1):
                record = cap_entries[index][i]
                next_record = cap_entries[index][i+1]
                # Write out the values
                data = pack('iidd', 
                            int(record[0]),
                            int(next_record[0]),
                            float(record[1]),
                            float(next_record[1]))
                calib.write(data)
                position += len(data)

        caf.close()
        cap.close()
        calib.close()

