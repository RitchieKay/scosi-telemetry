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
#ifndef SI_UTIL_PACKET
#define SI_UTIL_PACKET
#include "sfs/si_sfs_tables.h"
#include "sfs/si_sfs_funcs.h"
#include "sfs/si_sfs_defs.h"
/*
*  Process a packet identified by the packet id, the packet data is supplied
*  in packet_data
*/

int si_util_process_sfs_packet(struct si_sfs_packet_data * sfs_packet_data, 
                               unsigned int packet_location,
                               enum si_util_packet_processing processing_option,
                               si_process_value_fptr fptr);

/*
* Internal function for processing all the parameters defined for a packet
* as identified through the location of the start of the records in the
* packet table
*
*/
int si_util_process_sfs_location(void * packet_location_ptr, 
                                 struct si_sfs_packet_data * sfs_packet_data, 
                                 enum si_util_packet_processing processing_option,
                                 si_process_value_fptr fptr);

/*
*  get location in packet table for packet identified by packet_id. At the moment
*  we are just doing a straightforward search. Ultimately we will do a binary search
*  which is probably adequete.
*/
unsigned int si_util_get_packet_location(unsigned int packet_id);

#endif
