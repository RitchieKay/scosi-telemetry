/***************************************************************************
*
* PROJECT  : SCOS-I TM 
*
* SUBSYSTEM: SFS
*
* FILENAME : SI_SFS_FUNCS.H 
*
* AUTHOR   : Ritchie Kay 
*
* DATE     : 24.06.2010
*
* COPYRIGHT: Ritchie Kay, 2010
*
****************************************************************************
*
* PURPOSE: si_sfs_funcs provides definitions of functions defined in the SFS systems
*
****************************************************************************/
#ifndef SI_SFS_FUNCS
#define SI_SFS_FUNCS
#include "si_sfs_tables.h"

/* These functions provide pointers to functions that are used to process
*  parameter values dependent on their type
*/
typedef int (*si_process_value_fptr)(struct si_sfs_value * sfs_value,
                                     struct si_sfs_packet_data * sfs_packet_data);

#endif
