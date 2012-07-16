#***************************************************************************
#
# PROJECT  : SCOS-I TM 
#
# SUBSYSTEM: PYTHON_DBS
#
# FILENAME : si_dbs_packet_creator.py
#
# AUTHOR   : Ritchie Kay 
#
# DATE     : 20.06.2010
#
# COPYRIGHT: Ritchie Kay, 2010
#
#***************************************************************************
#
# PURPOSE: si_dbs_packet_creator creates the packet table containing the
#          parameter contents of fixed packets.
#
#***************************************************************************/
#
#***************************************************************************/
#
# The packet table starts with an index with format:
#
# ++++++++++++++++++++++++++++++++++++++++
# |  Type   |  Bytes  |  Purpose         |
# ++++++++++++++++++++++++++++++++++++++++
# |  uint   |    4    |  packet_id       |
# |  uint   |    4    |  location        |
# ++++++++++++++++++++++++++++++++++++++++
#
# The location in the index points are records with the format.
#
# ++++++++++++++++++++++++++++++++++++++++
# |  Type   |  Bytes  |  Purpose         |
# ++++++++++++++++++++++++++++++++++++++++
# |  uint   |    4    | param_entries    |
# |  uint   |    4    |dependency_entries|
# ++++++++++++++++++++++++++++++++++++++++
# | Repeats 'param_entries' times        |
# ++++++++++++++++++++++++++++++++++++++++
# |  uint   |    4    |  param loc       |
# |  unit   |    4    |  byte offset     |
# |  unit   |    4    |  bit offset      |
# ++++++++++++++++++++++++++++++++++++++++
#
# Note that a dependency entry is one upon whose value characteristic of other
# parameters in the packet (validity, limit) may depend.
#
import time
from struct import *
from si_dbs_table_creator import si_dbs_table_creator
import si_dbs_parameter_creator
import si_dbs_param_utilities 

class si_dbs_packet_creator(si_dbs_table_creator):

    __pkt_file = ''
    __plf_file = ''
    __param_creator = 0

    def __init__(self, ascii_dir, table_dir, param_creator):
        super(si_dbs_packet_creator, self).__init__(ascii_dir, table_dir)
        self.__pkt_file = self.output_dir() + '/pkt.tlb'
        self.__plf_file = self.input_dir() + '/plf.dat'
        self.__param_creator = param_creator

    def generate(self):
        self.create_packet()

    def create_packet(self):

        pkt = open(self.__pkt_file, 'wb')
        plf = open(self.__plf_file, 'r')

        packetsDef = {}

        for line in plf:
            fields = line.strip().split('\t')

            spid = int(fields[1])
            values = [fields[0], int(fields[2]), int(fields[3])]

            try:
                packetsDef[spid].append(values)
            except KeyError:
                packetsDef[spid] = [values]

        spids = packetsDef.keys()
        spids.sort()

        pkt.write(pack('I', len(spids)))

        # Write the size of the index
        location = 0
        pkt.write(pack('I', len(spids)))
        location += 4 # what we have just written
        location += (8 * len(spids)) # skip over where we will put the index 

        pkt.seek(location) # Move the file to location
        packetLocations = {}
        for spid in spids:

            packetLocations[spid] = location
            pkt.write(pack('I', len(packetsDef[spid]))) # This is the number of records
            location += 4
            location += 4 # Leave room for an entry for number of dependency parameters

            non_validities = [] # validities first

            # Loop through all parameters in packet adding the validity parameters
            # and remembering the rest for later

            numberDependency = 0

            for entries in packetsDef[spid]:
#                try:
#                    self.__param_creator.validity_parameters()[entries[0]]
##                    data = pack('III',                                               \
##                                self.__param_creator.param_location()[entries[0]],   \
##                                entries[1],                                          \
##                                entries[2])
#                    data = pack('I', self.__param_creator.param_location()[entries[0]])
#                    pkt.write(data)
#                    location += len(data)
#                    data = pack('I', entries[1])
#                    pkt.write(data)
#                    location += len(data)
#                    data = pack('I', entries[2])
#                    pkt.write(data)
#                    location += len(data)
#                  
#                    print spid, entries[0], self.__param_creator.param_location()[entries[0]], entries[1], entries[2]
#                    numberDependency += 1
#                except KeyError:
                    non_validities.append(entries)

            # Write number of dependencies
            oldLocation = location
            pkt.seek(packetLocations[spid] + 4)
            pkt.write(pack('I', numberDependency))
            pkt.seek(oldLocation)

            # Now add the non-validity parameters
            for entries in non_validities:
                data = pack('III',                                               \
                            self.__param_creator.param_location()[entries[0]],   \
                            entries[1],                                          \
                            entries[2])
                pkt.write(data)
                location += len(data)

        # Write out the index
        pkt.seek(4)
        spids = packetLocations.keys()
	spids.sort()
	
        for spid in spids:
            pkt.write(pack('I', spid))
            pkt.write(pack('I', packetLocations[spid]))

        pkt.close()
        plf.close()

