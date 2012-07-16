/***************************************************************************
*
* PROJECT  : SCOS-I TM 
*
* SUBSYSTEM: PROC
*
* FILENAME : SI_PROC_PVT.H
*
* AUTHOR   : Ritchie Kay 
*
* DATE     : 25.06.2010
*
* COPYRIGHT: Ritchie Kay, 2010
*
****************************************************************************
*
* PURPOSE: si_proc_pvt provides functions for dealing with
*          the pvt table.
*	   
*
***************************************************************************/
#include <stdio.h>
#include <memory.h>
#include "sfs/si_sfs_tables.h"

/************************************************************************************************/
int si_proc_save_value(struct si_sfs_value * sfs_value,
                       struct si_sfs_value * calibrated_value,
		       struct si_sfs_valid * validity,
                       struct timeval * value_time,
		       unsigned int pvt_location);
/************************************************************************************************/
int si_proc_get_raw_value(unsigned int location,
                          struct si_sfs_value * raw_value);
/************************************************************************************************/
