/***************************************************************************
*
* PROJECT  : SCOS-I TM 
*
* SUBSYSTEM: PROC
*
* FILENAME : SI_PROC_PVT.C 
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
#include "util/si_util_utilities.h"
#include "sfs/si_sfs_tables.h"
#include "si_proc_pvt.h"

/************************************************************************************************/
/*
* Process a value and store in PVT
*/
/************************************************************************************************/
int si_proc_save_value(struct si_sfs_value * raw_value,
                       struct si_sfs_value * calibrated_value,
		       struct si_sfs_valid * validity,
                       struct timeval * value_time,
		       unsigned int location)
{
  if (si_util_get_pvt_ptr() == NULL) return -1;

  char * const pvt_ptr = (char *)si_util_get_pvt_ptr();
  struct si_sfs_pvt_entry_common * pvt_entry_common = 
          (struct si_sfs_pvt_entry_common *) &pvt_ptr[location];

  int pos = location;

  /* Add the time entry to the pvt table read from the packet data */
  memcpy(pvt_entry_common, value_time, sizeof(*value_time));
  pos += sizeof(*value_time);

  /* Step over the validity and limits, we add this value later */
  memcpy(&pvt_entry_common->validity, validity, sizeof(validity));
  pos += sizeof(pvt_entry_common->validity);
  pos += sizeof(pvt_entry_common->limits);

  /* Store the value */
  switch (raw_value->value_type) {
    case SI_SFS_UINT:
      memcpy(&pvt_ptr[pos], &raw_value->value.uival, sizeof(unsigned int));
      pos += sizeof(unsigned int);
      break;
    case SI_SFS_INT:
      memcpy(&pvt_ptr[pos], &raw_value->value.ival, sizeof(int));
      pos += sizeof(int);
      break;
    case SI_SFS_FLOAT:
      memcpy(&pvt_ptr[pos], &raw_value->value.fval, sizeof(float));
      pos += sizeof(float);
      break;
    case SI_SFS_DOUBLE:
      memcpy(&pvt_ptr[pos], &raw_value->value.dval, sizeof(double));
      pos += sizeof(double);
      break;
    default:
      return -1;
      break;
  }

  /* Store the calibrated value */
  switch (calibrated_value->value_type) {
    case SI_SFS_DOUBLE:
      memcpy(&pvt_ptr[pos], &calibrated_value->value.uival, sizeof(double));
      break;
    case SI_SFS_STRING:
      memset(&pvt_ptr[pos], 0, 16);
      memcpy(&pvt_ptr[pos], calibrated_value->value.sval, strlen(calibrated_value->value.sval));
      break;
    case SI_SFS_NONE:
    default:
      memset(&pvt_ptr[pos], 0, 16);
      return -1;
      break;
  }

  return 1;
}

/************************************************************************************************/
/************************************************************************************************/
/*
* Get a value from PVT
*/
/************************************************************************************************/
int si_proc_get_raw_value(unsigned int location,
                          struct si_sfs_value * raw_value)
{

  if (si_util_get_pvt_ptr() == NULL) return -1;

  char * const pvt_ptr = (char *)si_util_get_pvt_ptr() + location +
                         sizeof(struct si_sfs_pvt_entry_common);

  /* Store the value */
  switch (raw_value->value_type) {
    case SI_SFS_UINT:
      memcpy(&raw_value->value.uival, pvt_ptr, sizeof(unsigned int));
      break;
    case SI_SFS_INT:
      memcpy(&raw_value->value.ival, pvt_ptr, sizeof(int));
      break;
    case SI_SFS_FLOAT:
      memcpy(&raw_value->value.fval, pvt_ptr, sizeof(float));
      break;
    case SI_SFS_DOUBLE:
      memcpy(&raw_value->value.dval, pvt_ptr, sizeof(double));
      break;
    default:
      return -1;
      break;
  }
  return 1;
}


