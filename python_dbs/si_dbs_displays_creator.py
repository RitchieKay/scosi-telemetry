#***************************************************************************
#
# PROJECT  : SCOS-I TM 
#
# SUBSYSTEM: PYTHON_DBS
#
# FILENAME : si_dbs_display_creator.py
#
# AUTHOR   : Ritchie Kay 
#
# DATE     : 27.06.2010
#
# COPYRIGHT: Ritchie Kay, 2010
#
#***************************************************************************
#
# PURPOSE: si_dbs_display_creator creates a AND display definition files
#
#***************************************************************************/
#
#

from si_dbs_table_creator import si_dbs_table_creator

class si_dbs_displays_creator(si_dbs_table_creator):

    def __init__(self, ascii_dir, displays_dir):
        super(si_dbs_displays_creator, self).__init__(ascii_dir, displays_dir)

    def generate(self):
        self.create_displays()

    def create_displays(self):

        dpf = open(self.input_dir() + '/dpf.dat', 'r')

        displays = {}
        for line in dpf:
            (display_name, display_type, display_description) = line.strip().split('\t') 
            displays[display_name] = {'type':display_type, 'description':display_description}

        dpf.close()

        dpc = open(self.input_dir() + '/dpc.dat', 'r')
        display_details = {}
        for line in dpc:
            (display_name, param_name, location, commut, form, text) = line.strip().split('\t')[0:6] 
            if len(param_name) > 0:
                try:
                    display_details[display_name].append({'param':param_name, 'location':location,'form':form}) 
                except KeyError:
                    display_details[display_name] = [{'param':param_name, 'location':location,'form':form}]

        dpc.close()

        for display_name in display_details.keys():

            display_file = open(self.output_dir() + '/' + display_name, 'w')

            display_file.write(display_name + '\t' +  displays[display_name]['description'] + '\t' + displays[display_name]['type'] + '\n')
            for details in display_details[display_name]:

                display_file.write(details['location'] + '\t' + details['param'] + '\t' + details['form'] + '\n')
        
            display_file.close() 
