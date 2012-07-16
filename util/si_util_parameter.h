/***************************************************************************
*
* PROJECT  : SCOS-I TM 
*
* SUBSYSTEM: UTIL
*
* FILENAME : SI_UTIL_PARAMETER.H
*
* AUTHOR   : Ritchie Kay 
*
* DATE     : 22.06.2010
*
* COPYRIGHT: Ritchie Kay, 2010
*
****************************************************************************
*
* PURPOSE: si_util_parameter provides utilities for calibrating and limit
*          checking parameter values
*
****************************************************************************/
#ifndef SI_UTIL_PARAMETER
#define SI_UTIL_PARAMETER

#include <stdlib.h>
#include <stdio.h>
#include <memory.h>
#include "sfs/si_sfs_tables.h"

int si_util_calibrate_parameter(struct si_sfs_param_info * param_info, 
                                struct si_sfs_value * raw_value,
                                struct si_sfs_value * calib_value);

#endif
