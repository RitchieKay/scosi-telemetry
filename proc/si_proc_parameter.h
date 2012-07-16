/***************************************************************************
*
* PROJECT  : SCOS-I TM 
*
* SUBSYSTEM: SFS
*
* FILENAME : SI_PROC_PARAMETER.H 
*
* AUTHOR   : Ritchie Kay 
*
* DATE     : 16.06.2010
*
* COPYRIGHT: Ritchie Kay, 2010
*
****************************************************************************
*
* PURPOSE: si_proc_parameter provides functions for dealing with
*          parameters that have been extracted from tm packet.
*	   Currently the values are simply written to the pvt table,
*	   whose location is known through the global variables:
*	   
*	   si_sfs_param_table and si_sfs_pvt_table
*
***************************************************************************/
#ifndef SI_PROC_PARAMETER
#define SI_PROC_PARAMETER

#include "sfs/si_sfs_tables.h"

int si_proc_process_value(struct si_sfs_value * sfs_value,
                          struct si_sfs_packet_data * sfs_packet_data);



int si_proc_value_valid(struct si_sfs_value * sfs_value);
#endif
