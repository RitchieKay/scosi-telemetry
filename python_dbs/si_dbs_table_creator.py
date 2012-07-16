#***************************************************************************
#
# PROJECT  : SCOS-I TM 
#
# SUBSYSTEM: PYTHON_DBS
#
# FILENAME : si_dbs_table_creator.py
#
# AUTHOR   : Ritchie Kay 
#
# DATE     : 20.06.2010
#
# COPYRIGHT: Ritchie Kay, 2010
#
#***************************************************************************
#
# PURPOSE: si_dbs_table_creator provides an 'abstract' class to provide the
#          parent of table creators.
#
#***************************************************************************/
#
#
class si_dbs_table_creator(object):

    __input_dir = ''
    __output_dir = ''

    def __init__(self, input_dir, output_dir):
        self.__input_dir = input_dir
        self.__output_dir = output_dir

    def input_dir(self):
        return self.__input_dir;
    def output_dir(self):
        return self.__output_dir;

    def generate(self):
        pass
