/***************************************************************************
*
* PROJECT  : SCOS-I TM 
*
* SUBSYSTEM: UTIL
*
* FILENAME : SI_UTIL_PACKET.H 
*
* AUTHOR   : Ritchie Kay 
*
* DATE     : 16.06.2010
*
* COPYRIGHT: Ritchie Kay, 2010
*
****************************************************************************
*
* PURPOSE: si_util_packet provides methods for processing packets
*
****************************************************************************/
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/mman.h>
#include <memory.h>
#include <stdio.h>
#include <unistd.h>
#include <time.h>

#include "si_util_packet.h"
#include "si_util_values.h"
#include "si_util_utilities.h"
#include "sfs/si_sfs_tables.h"

/************************************************************************************************/
/*
* Process a packet given by packet_data identified by packet_id (spid)
*/
/************************************************************************************************/
int si_util_process_sfs_packet(struct si_sfs_packet_data * sfs_packet_data, 
                               unsigned int  packet_location,
                               enum si_util_packet_processing processing_option,
                               si_process_value_fptr fptr)
{
  if (si_util_get_pkt_ptr() == NULL) return -1;


 /* The packet table starts off with an index which we have to search in order to find the
  *  location of the packet parameter records
  */

  packet_location = si_util_get_packet_location(sfs_packet_data->packet_id); 

  if (packet_location < 0) return -1;

  /* Now we need to process the records at that location */ 
  return si_util_process_sfs_location(si_util_get_pkt_ptr() + packet_location, 
                                      sfs_packet_data,
                                      processing_option,
                                      fptr);
}

/************************************************************************************************/
/*
* Process a packet location. Note this takes a pointer to a packet location and so does not look
* directly in the standard packet table. This allows functions to create their own packet table
* (perhaps only listing a subset of parameter in the packet) and have this processed.
*/
/************************************************************************************************/
int si_util_process_sfs_location(void * packet_location_ptr, 
                                 struct si_sfs_packet_data * sfs_packet_data,
                                 enum si_util_packet_processing processing_option,
                                 si_process_value_fptr fptr)
{
  /* We need the pointer to the param table to be defined
  *  otherwise we can't do anything.
  */
  if (si_util_get_param_ptr() == NULL) return -1;

  int ret_val;

  struct si_sfs_packet_location_header * location_header = 
         (struct si_sfs_packet_location_header *)packet_location_ptr;

  unsigned int i1, i2, i3, i4, i5;
  i1 = *(unsigned int *) packet_location_ptr;
  i2 = *(unsigned int *) (packet_location_ptr + 4);
  i3 = *(unsigned int *) (packet_location_ptr + 8);
  i4 = *(unsigned int *) (packet_location_ptr + 12);
  i5 = *(unsigned int *) (packet_location_ptr + 16);

  packet_location_ptr += sizeof(struct si_sfs_packet_location_header); /* skip the entries field */


  unsigned int number_entries = 0;

  /* Work out how many entries to read and where to start depending on the processing type */
  switch (processing_option) {
    case SI_UTIL_DEPENDENCY:
      number_entries = location_header->number_dependency_entries;
      break;
    case SI_UTIL_NON_DEPENDENCY:
      number_entries = location_header->number_param_entries - location_header->number_dependency_entries;
      packet_location_ptr += location_header->number_dependency_entries * sizeof(struct si_sfs_packet_locations);
      break;
    case SI_UTIL_ALL:
    default:
      number_entries = location_header->number_param_entries;
  }


  /* Loop through the parameter entries processing them in turn */
  int i;
  for (i = 0; i < number_entries ; i++) {
    struct si_sfs_packet_locations * packet_location_entry = (struct si_sfs_packet_locations *) packet_location_ptr;
    /* The entries in the packet table identify the parameters by a location in the 
    *  param info table. 
    *  We use that to identify the type of the parameter, extract the value and then
    *  populate a value structure to pass on for further processing to any function
    *  defined by fptr`
    */

    struct si_sfs_param_info * param_info_entry = (struct si_sfs_param_info *) (si_util_get_param_ptr() + packet_location_entry->param_location);
    struct si_sfs_value sfs_value;
    sfs_value.param_location = packet_location_entry->param_location;
    sfs_value.value_type = param_info_entry->param_type;

    switch (param_info_entry->param_type) {
      case SI_SFS_UINT:
        sfs_value.value.uival = si_util_get_uint(&sfs_packet_data->packet_data[packet_location_entry->byte_offset], 
                                  packet_location_entry->bit_offset, 
                                  param_info_entry->bitlen);
        break;
      case SI_SFS_INT:
        sfs_value.value.ival = si_util_get_int(&sfs_packet_data->packet_data[packet_location_entry->byte_offset],              
                                 packet_location_entry->bit_offset,            
                                 param_info_entry->bitlen);
        break;
      case SI_SFS_FLOAT:
        sfs_value.value.fval = si_util_get_float(&sfs_packet_data->packet_data[packet_location_entry->byte_offset],              
                                   packet_location_entry->bit_offset,            
                                   param_info_entry->bitlen);
        break;
      case SI_SFS_DOUBLE:
        sfs_value.value.dval = si_util_get_double(&sfs_packet_data->packet_data[packet_location_entry->byte_offset],              
                                   packet_location_entry->bit_offset,            
                                   param_info_entry->bitlen);
        break;
      default:
        break;
    }
    /* Call callback function if supplied) */
    if (fptr) {
      ret_val = fptr(&sfs_value,
                     sfs_packet_data);
    }

    packet_location_ptr += sizeof(struct si_sfs_packet_locations);
  } /* end of processing loop */
  return ret_val;
}
/************************************************************************************************/
/*
*  get location in packet table for packet identified by packet_id. At the moment
*  we are just doing a straightforward search. Ultimately we will do a binary search
*  which is probably adequete.
*/
/************************************************************************************************/
unsigned int si_util_get_packet_location(unsigned int packet_id)
{
  /* 
  *  First we set up a test to see if being called with the same packet_id as
  *  last time. If so don't search, just return last value
  */
  static int first_time = 1;  
  static struct si_sfs_packet_location_index last_packet = {0,0};

// if (!first_time && (packet_id == last_packet.packet_id)) {
//    return last_packet.packet_location;
//  }

  /* Return if we don't have a packet table */
  
  if (si_util_get_pkt_ptr() == NULL) return -1;

  void * ptr = si_util_get_pkt_ptr();

  unsigned int entries = *(unsigned int *) ptr; 
  ptr += sizeof(unsigned int); /* skip the entries field */


  struct si_sfs_packet_location_index * location =
     (struct si_sfs_packet_location_index *) si_util_uint_binary_search(packet_id,
                                                                        (const char *)ptr,
                                                                        sizeof(struct si_sfs_packet_location_index),
                                                                        entries); 

  if (location == NULL) {
    return -1;
  } else {
    last_packet = *location;
    first_time = 0;
    return location->packet_location;
  }
}
/************************************************************************************************/
