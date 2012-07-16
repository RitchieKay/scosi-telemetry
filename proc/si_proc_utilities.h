/***************************************************************************
*
* PROJECT  : SCOS-I TM 
*
* SUBSYSTEM: SFS
*
* FILENAME : SI_PROC_UTILITIES.H 
*
* AUTHOR   : Ritchie Kay 
*
* DATE     : 16.06.2010
*
* COPYRIGHT: Ritchie Kay, 2010
*
****************************************************************************
*
* PURPOSE: si_proc_utilities provides the basic utilities of the proc
*          library. Methods are provided for initializing and cleanup
*          the proc process - meaning mapping to the memory mapped files
*          etc. Also the global variables that are provided to identify
*          callback functions to call when values are extracted from
*          packets are set up to identify proc functions.
*
*          The globals that are set up here are as follows:
*
*          si_sfs_pvt_table - pointer to PVT table
*          si_sfs_param_table - pointer to param table
*          si_sfs_packet_table - pointer to packet table
*          si_process_uint_value_ptr - pointer to function to process uint values
*
***************************************************************************/
#ifndef SI_PROC_UTILITIES
#define SI_PROC_UTILITIES
#include "sfs/si_sfs_tables.h"

int si_proc_init();
int si_proc_cleanup();
#endif
