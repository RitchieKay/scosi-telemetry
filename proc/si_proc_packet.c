/***************************************************************************
*
* PROJECT  : SCOS-I TM 
*
* SUBSYSTEM: SFS
*
* FILENAME : SI_PROC_PACKET.C 
*
* AUTHOR   : Ritchie Kay 
*
* DATE     : 19.06.2010
*
* COPYRIGHT: Ritchie Kay, 2010
*
****************************************************************************
*
* PURPOSE: si_proc_packet provides the basic utilities of the proc
*          library for processing packets.
*
*
***************************************************************************/
#include "si_proc_packet.h"
#include "si_proc_parameter.h"
#include "util/si_util_packet.h"
#include <stdio.h>

/************************************************************************************************/
/*
*  Process a packet identified by the packet id, the packet data is supplied
*  in packet_data
*/
/************************************************************************************************/

int si_proc_process_sfs_packet(struct si_sfs_packet_data * sfs_packet_data)
{
  /* 
  * Call the utility routine for processing a packet supplying the callback
  * function si_proc_process_value to be called every time a value is
  * extracted.
  */

  unsigned int packet_location = 0;


  /* First we process the dependency parameters */
  if (si_util_process_sfs_packet(sfs_packet_data, 
                                 packet_location,
                                 SI_UTIL_DEPENDENCY,
                                 &si_proc_process_value) < 0) {
    return -1;
  }
  /* then the rest */
  if (si_util_process_sfs_packet(sfs_packet_data,
                                 packet_location,
                                 SI_UTIL_NON_DEPENDENCY,
                                 &si_proc_process_value) < 0) {
    return -1;
  }

  return 1;
}

/************************************************************************************************/
