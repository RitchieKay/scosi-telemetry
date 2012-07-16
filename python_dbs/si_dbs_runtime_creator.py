#***************************************************************************
#
# PROJECT  : SCOS-I TM 
#
# SUBSYSTEM: PYTHON_DBS
#
# FILENAME : si_dbs_runtime_creator.py
#
# AUTHOR   : Ritchie Kay 
#
# DATE     : 20.06.2010
#
# COPYRIGHT: Ritchie Kay, 2010
#
#***************************************************************************
#
# PURPOSE: si_dbs_runtime_creator creates a python startup script for
#          making paramters available within the python interpreter.
#
#***************************************************************************/
#
#
import time

from si_dbs_table_creator import si_dbs_table_creator
import si_dbs_parameter_creator
import si_dbs_param_utilities 

class si_dbs_runtime_creator(si_dbs_table_creator):

    __runtime_script = ''
    __runtime_info = ''
    __param_creator = 0

    def __init__(self, ascii_dir, runtime_dir, param_creator):
        super(si_dbs_runtime_creator, self).__init__(ascii_dir, runtime_dir)
        self.__runtime_script = self.output_dir() + '/runtime_setup.py'
        self.__runtime_info = self.output_dir() + '/runtime_info.dat'
	self.__param_creator = param_creator

    def generate(self):
        self.create_runtime_setup()

    def create_runtime_setup(self):

        setup = open(self.__runtime_script, 'w')
        info = open(self.__runtime_info, 'w')
	self.write_header(setup)
        pcf = open(self.input_dir() + '/pcf.dat', 'r')

        setup.write('parameters = {}\n')

        for line in pcf:
            fields = line.strip().split('\t')

            if (len(fields[4]) == 0) or (len(fields[5]) == 0): continue

            (ptype, bytelen, bitlen) = si_dbs_param_utilities.get_type_len(int(fields[4]), int(fields[5]))

            if ptype == 'U':
                valueStr = 'valueUInt'
            elif ptype == 'I':
                valueStr = 'valueInt'
            elif ptype == 'F':
                valueStr = 'valueFloat'
            elif ptype == 'D':
                valueStr = 'valueDouble'
            else:
                continue

            setup.write(fields[0] + ' =  ' + valueStr + '()\n')
            setup.write(fields[0] + '.setLocation(' + str(self.__param_creator.pvt_location()[fields[0]]) + ')\n')
            setup.write('parameters[\'' + fields[0] + '\'] = ' + fields[0] + '\n')

            info.write(fields[0] + '\t' + ptype +'\t')   
	    info.write(str(self.__param_creator.pvt_location()[fields[0]]) + \
		       '\t' + fields[9] + '\t' + fields[1] + '\t' + fields[3] + '\n')

            if (len(fields[9]) != 0) and (len(fields[11]) != 0):
                setup.write(fields[0] + '.setCalibrationType(\'' + fields[9] + '\')\n')
        pcf.close()
        setup.close();
        info.close();

    def write_header(self, output_file):

        output_file.write("import mmap\n")
        output_file.write("import os\n")
        output_file.write("from struct import unpack\n")
        output_file.write("import scosi_value\n")
        output_file.write("from scosi_value import *\n")

        output_file.write("\n")
	output_file.write("\n")

        output_file.write("pvt_file = os.getenv('SCOSI_TABLE_DIR') + '/pvt.tlb'\n")
        output_file.write("pvt = open(pvt_file, 'r+b')\n")
        output_file.write("pvt_map = mmap.mmap(pvt.fileno(), os.stat(pvt.name).st_size)\n")

        output_file.write("\n")
        output_file.write("entries = unpack('i', pvt_map[0:4])[0]\n")
        output_file.write("scosi_value.pvt_map = pvt_map\n")
	output_file.write("\n")

