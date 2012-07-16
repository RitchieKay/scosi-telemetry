#include <stdio.h>
#include "sfs/si_sfs_tables.h"
#include "util/si_util_utilities.h"

int main(int argc, char** argv)
{

  void * packets_ptr = si_util_get_pkt_ptr();

  const unsigned int number_entries = *(unsigned int *) packets_ptr;

  packets_ptr += sizeof(unsigned int);
  int i;
  printf("---------------------------------------------------------------------\n");
  for (i = 0; i < number_entries; i++) {

    struct si_sfs_packet_location_index * location_index = 
         (struct si_sfs_packet_location_index *) packets_ptr;

    packets_ptr += sizeof(struct si_sfs_packet_location_index);

    void * location_ptr = si_util_get_pkt_ptr() + location_index->packet_location;

    printf("Packet ID = %d\n", location_index->packet_id);
    printf("Location = %d\n", location_index->packet_location);
    printf("---------------------------------------------------------------------\n");

      struct si_sfs_packet_location_header * location_hdr =
         (struct si_sfs_packet_location_header *) location_ptr;

      printf("Number param entries      = %d\n", location_hdr-> number_param_entries);
      printf("Number dependency entries = %d\n", location_hdr-> number_dependency_entries);
      printf("---------------------------------------------------------------------\n");

      location_ptr += sizeof(struct si_sfs_packet_location_header);

      int j;
      for (j = 0; j < location_hdr-> number_param_entries; j++) {
        if (j == location_hdr-> number_dependency_entries) {
          printf("---------------------------------------------------------------------\n");
        }

        struct si_sfs_packet_locations * location =
            (struct si_sfs_packet_locations *) location_ptr;

        printf("Param location = %d, byte offset = %d, bit offset = %d\n",
               location->param_location,
               location->byte_offset,
               location->bit_offset);


        location_ptr += sizeof(struct si_sfs_packet_locations);
      }
 
    printf("---------------------------------------------------------------------\n");

  }

  return 1;
}


