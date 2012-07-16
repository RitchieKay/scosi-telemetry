/***************************************************************************
*
* PROJECT  : SCOS-I TM 
*
* SUBSYSTEM: SFS
*
* FILENAME : SI_PROC_UTILITIES.C 
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
*          etc. 
*
***************************************************************************/

#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/mman.h>
#include <stdio.h>
#include <unistd.h>

#include "util/si_util_mmap.h"
#include "util/si_util_packet.h"
#include "util/si_util_utilities.h"
#include "sfs/si_sfs_tables.h"
#include "si_proc_utilities.h"
#include "si_proc_parameter.h"

/************************************************************************************************/

/************************************************************************************************/
int si_proc_init()
{

  if (! ( si_util_get_pvt_ptr() &&
          si_util_get_param_ptr() &&
          si_util_get_pkt_ptr() &&
          si_util_get_text_calib_ptr() &&
          si_util_get_numeric_calib_ptr() ) ) {
   
    printf ("Unable to initialise memory tables.\n");
    return -1;
  }
  return 1;
}
/************************************************************************************************/
/*
* cleanup after proc
*/
/************************************************************************************************/
int si_proc_cleanup()
{
/*
*  unmapFile("pvt.tlb", si_sfs_pvt_table); 
*  unmapFile("pkt.tlb", si_sfs_packet_table);
*  unmapFile("param.tlb", si_sfs_param_table);
*/
  return 1;
}
/************************************************************************************************/
