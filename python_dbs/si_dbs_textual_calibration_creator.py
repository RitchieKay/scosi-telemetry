#***************************************************************************
#
# PROJECT  : SCOS-I TM 
#
# SUBSYSTEM: PYTHON_DBS
#
# FILENAME : si_dbs_textual_calibration_creator.py
#
# AUTHOR   : Ritchie Kay 
#
# DATE     : 20.06.2010
#
# COPYRIGHT: Ritchie Kay, 2010
#
#***************************************************************************
#
# PURPOSE: si_dbs_textual_calibration_creator creates the calib table
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
# |  uint   |    4    |  low value       |
# |  uint   |    4    |  high value      |
# |  uint   |    4    |  value defined?  |
# |  char   |   16    |  calib value     |
# ++++++++++++++++++++++++++++++++++++++++
#
import time
from struct import *

from si_dbs_table_creator import si_dbs_table_creator
from si_dbs_param_utilities import *

class si_dbs_textual_calibration_creator(si_dbs_table_creator):

    __calib_file = ''
    __txf_file = ''
    __txp_file = ''
    __textual_location = {}
    __numerical_location = {}

    def __init__(self, ascii_dir, table_dir):
        super(si_dbs_textual_calibration_creator, self).__init__(ascii_dir, table_dir)
        self.__calib_file = self.output_dir() + '/text_calib.tlb'
        self.__txf_file = self.input_dir() + '/txf.dat'
        self.__txp_file = self.input_dir() + '/txp.dat'

    def generate(self):
        self.create_textual()

    def textual_location(self):
        return self.__textual_location

    def numerical_location(self):
        return self.__numerical_location

    def create_textual(self):

        calib = open(self.__calib_file, 'wb')
        txf = open(self.__txf_file, 'r')
        txp = open(self.__txp_file, 'r')

        txf_entries = {}
        txp_entries = {}
        txp_max = {}

        for line in txf:
            fields = line.strip().split('\t')
            txf_entries[fields[0]] = fields[2]
            txp_entries[fields[0]] = []
            txp_max[fields[0]] = 0

        for line in txp:
            fields = line.strip().split('\t')
            txp_entries[fields[0]].append([int(fields[1]), int(fields[2]), fields[3]])

            if txf_entries[fields[0]] == 'U' and int(fields[2]) > txp_max[fields[0]]:
                txp_max[fields[0]] = int(fields[2])

        calib.write(pack('I',len(txp_entries)))
        position = 4

        keys = txp_entries.keys()
	keys.sort()
	
        for index in keys:
            self.__textual_location[index] = position
            txp_entries[index].sort()
            # Unsigned int and less than 16 as max value, so we treat as an enumeration
            if txp_max[index] < 16:
                # Write out the type of the calibration, in this case an enumeration
                # Write out the number of entries in the enumeration
                data = pack('cI', 'E', txp_max[index] + 1)
                calib.write(data)
                position += len(data)
                # For each value
                for value in range(txp_max[index] + 1):
                    found = False
                    for record in txp_entries[index]:
                        if value >= int(record[0]) and value <= int(record[1]):
                            # Write out that value is defined
                            data = pack('iii16s',
			                value,
					value,
					1,
                                                           # Write out the value padded to 16 bytes
					record[2] + '\x00'*(16 - len(record[2])))
                            calib.write(data)
                            position += len(data)
                            found = True
                            break
                    if not found:
                        #Write out that the value is not defined
                        data = pack('iii16s',
			             value,
				     value,
			             0,
				     '\x00'*16)
                        calib.write(data)
                        position += len(data)

            # Not enumeration so we write out all the records
            else:
                # Write out the type of the calibration, in this case an enumeration
                # Write out the number of entries in the calibration
                data = pack('cI', 'T',len(txp_entries[index]))
                calib.write(data)
                position += len(data)
                # Go through every record
                for record in txp_entries[index]:
                    # Write out the values
                    data = pack('II', int(record[0]), int(record[1]))
                    calib.write(data)
                    position += len(data)
                    # Write out that values are as they have to be
                    data = pack('I', 1)
                    calib.write(data)
                    position += len(data)
                    # Write out the value padded to 16 bytes
                    data = pack('16s', record[2] + '\x00'*(16 - len(record[2])))
                    calib.write(data)
                    position += len(data)

        txf.close()
        txp.close()

        calib.close()

