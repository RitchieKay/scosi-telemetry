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


/*
*  Process a packet identified by the packet id, the packet data is supplied
*  in packet_data
*/

#ifndef SI_PROC_PACKET
#define SI_PROC_PACKET
#include "sfs/si_sfs_tables.h"
/*
*  Process a packet identified by the packet id, the packet data is supplied
*  in packet_data
*/

int si_proc_process_sfs_packet(struct si_sfs_packet_data * sfs_packet_data);
#endif
