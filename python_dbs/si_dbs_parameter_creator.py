#***************************************************************************
#
# PROJECT  : SCOS-I TM 
#
# SUBSYSTEM: PYTHON_DBS
#
# FILENAME : si_dbs_parameter_creator.py
#
# AUTHOR   : Ritchie Kay 
#
# DATE     : 20.06.2010
#
# COPYRIGHT: Ritchie Kay, 2010
#
#***************************************************************************
#
# PURPOSE: si_dbs_parameter_creator creates two tables the param 
#          and pvt tables.
#          The param table contains informantion on the parametres, 
#          the pvt table is a table where runtime values for each 
#          parameter are stored.
#
#***************************************************************************/
#
#
import time
from struct import *
from si_dbs_table_creator import si_dbs_table_creator
import si_dbs_textual_calibration_creator
import si_dbs_numerical_calibration_creator
from si_dbs_param_utilities import *
import si_dbs_param_utilities
#
#
# si_dbs_parameter_creator creates two tables the param and pvt tables.
# The param table contains informantion on the parametres, the pvt table
# is a table where runtime values for each parameter are stored.
#
# The structure of the param table is:
#
# ++++++++++++++++++++++++++++++++++++++++
# |  Type   |  Bytes  |  Purpose         |
# ++++++++++++++++++++++++++++++++++++++++
# |  char   |    9    |  Param name      |
# |  char   |    1    |  Param type      |
# |  char   |    1    |  Param calib type|
# |  char   |    1    |  Param val type|
# |  uint   |    4    |  bit len         |
# |  uint   |    4    |  pvt loc         |
# |  uint   |    4    |  calib_loc       |
# |  uint   |    4    |  limit_loc       |
# |  uint   |    4    |  valid param loc |
# |  uint   |    4    |  valid param val |
# ++++++++++++++++++++++++++++++++++++++++
#
# The structure of the pvt table is:
#
# ++++++++++++++++++++++++++++++++++++++++
# |  Type   |  Bytes  |  Purpose         |
# ++++++++++++++++++++++++++++++++++++++++
# |  uint   |    4    |  time seconds    |
# |  uint   |    4    |  time useconds   |
# |  uint   |    4    |  validity        |
# |  uint   |    4    |  limits          |
# |  value  |    -    |  param value     |
# |  value  |    -    |  calib value     |
# ++++++++++++++++++++++++++++++++++++++++
#
#


class si_dbs_parameter_creator(si_dbs_table_creator):

    __param_file = ''
    __pvt_file = ''
    __pcf_file = ''
    __calib_creators = 0
    __pvt_location = {}
    __param_location = {}
    __validity_parameters = {}

    def __init__(self, ascii_dir, table_dir, calib_creators):
        super(si_dbs_parameter_creator, self).__init__(ascii_dir, table_dir)
	self.__calib_creators = calib_creators
        self.__param_file = self.output_dir() + '/param.tlb'
        self.__pvt_file = self.output_dir() + '/pvt.tlb'
        self.__pcf_file = self.input_dir() + '/pcf.dat'

    def generate(self):
        self.create_pvt()
        self.create_param()

    def param_location(self):
        return self.__param_location
    def pvt_location(self):
        return self.__pvt_location
    def validity_parameters(self):
        return self.__validity_parameters

    def create_param(self):

        param = open(self.__param_file, 'wb')
        pcf = open(self.__pcf_file, 'r')

        pcf_fields = {}

        # Read the pcf to initialise param locations
        for line in pcf:
            fields = line.strip().split('\t')
            if (len(fields[4]) == 0) or (len(fields[5]) == 0): continue
	    pcf_fields[fields[0]] = fields

        param.write(pack('I', len(pcf_fields)))

        # Read the PCF again and add validity records
        keys = pcf_fields.keys()
	keys.sort()
    
        location = 4
        for key in keys:
            fields = pcf_fields[key]
            self.__param_location[fields[0]] = location
            data = pack('9scccIIIIII', '---------', '-', '-','-',0,0,0,0,0,0)
	    location += len(data)
    
        for key in keys:
            fields = pcf_fields[key]
            (ptype, bytelen, bitlen) = get_type_len(int(fields[4]), int(fields[5]))

            valid_loc = 0
            valid_val = 1
            if len(fields[7]) != 0:
                valid_loc = self.__param_location[fields[7]]
                if len(fields[17]) != 0:
                    valid_val = int(fields[17])

                self.__validity_parameters[fields[7]] = True

            name = fields[0] + '\x00'*(9 - len(fields[0]))

            ctype = 'N'
            if len(fields[9]) != 0: ctype = fields[9][0]
            vtype = 'R'
            if len(fields[10]) != 0: vtype = fields[10][0]

            calib_loc = 0
            if len(fields[11]) and ctype == 'S':
	        calib_loc = self.__calib_creators['text'].textual_location()[fields[11]]
            elif len(fields[11]) and ctype == 'N':
	        calib_loc = self.__calib_creators['numeric'].numerical_location()[fields[11]]
            
            data = pack('9scccIIIIII',                                \
	                name,                                         \
			ptype,                                        \
			ctype        ,                                \
			vtype        ,                                \
			bitlen,                                       \
			self.__pvt_location[fields[0]],               \
			calib_loc,                                    \
			0,                                            \
			valid_loc,                                    \
			valid_val)
            param.write(data)

        param.close()
        pcf.close()

    def create_pvt(self):

        pvt = open(self.__pvt_file, 'wb')
        pcf = open(self.__pcf_file, 'r')

        t = time.time()
        tv_sec = int(t)
        tv_usec = int((t - tv_sec)*1E6)

        number = pack('4s', '\x00\x00\x00\x00')
        pvt.write(number)

        paramLoc = 4
        entries = 0
        for line in pcf:
            fields = line.strip().split('\t')
            if (len(fields[4]) == 0) or (len(fields[5]) == 0): continue

            (ptype, bytelen, bitlen) = si_dbs_param_utilities.get_type_len(int(fields[4]), int(fields[5]))

            pvt.write(pack('II', tv_sec, tv_usec))                 # time
            pvt.write(pack('I', 0))                                # validity
            pvt.write(pack('I', 0))                                # limits
            pvt.write(pack(str(bytelen) + 's', '\x00' * bytelen))  # raw value

            calib_length = 8
            if fields[9] == 'S': calib_length = 16

            pvt.write(pack(str(calib_length) + 's', '\x00' * calib_length))  # raw value
            self.__pvt_location[fields[0]] = paramLoc
            paramLoc = paramLoc + 16 + bytelen + calib_length
            entries = entries + 1

        pvt.seek(0)
        number = pack('I', entries)
        pvt.write(number)

        pvt.close()
        pcf.close()
