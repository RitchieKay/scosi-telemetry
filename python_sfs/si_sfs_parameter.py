#!/usr/bin/python

import os
import scosi_value
from scosi_value import *

class si_sfs_parameter:

    __all_parameters = {}

    def __init__(self):
        
	info_file = os.getenv('RUNTIME_DIR') + '/runtime_info.dat'
        info = open(info_file, 'r')

        for line in info:
	    line_str = line.strip() + '\t'
            (name, type, location, calib_type, description, unit) = line_str.split('\t')[0:6]
            self.__all_parameters[name] = {'name':name, 'type':type, 'location':location,  \
	                                   'calib_type':calib_type, \
					    'description':description, 'unit':unit}

        info.close()

    def get_parameter(self, parameter):

       value = sfs_value(0)

       if self.__all_parameters[parameter]['type'] == 'U':
           value = valueUInt()
       elif self.__all_parameters[parameter]['type'] == 'I':
           value = valueInt()
       elif self.__all_parameters[parameter]['type'] == 'F':
           value = valueFloat()
       elif self.__all_parameters[parameter]['type'] == 'D':
           value = valueDouble()

       value.setLocation(int(self.__all_parameters[parameter]['location']))
       value.setCalibrationType(self.__all_parameters[parameter]['calib_type'])

       return value

    def get_parameter_info(self, parameter):
        return self.__all_parameters[parameter]
