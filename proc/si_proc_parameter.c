/***************************************************************************
*
* PROJECT  : SCOS-I TM 
*
* SUBSYSTEM: SFS
*
* FILENAME : SI_PROC_PARAMETER.C 
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
*	   Currently the values are simply written to the pvt table.
*	   
*
***************************************************************************/
#include <stdio.h>
#include <memory.h>
#include "sfs/si_sfs_tables.h"
#include "util/si_util_utilities.h"
#include "util/si_util_parameter.h"
#include "si_proc_parameter.h"
#include "si_proc_pvt.h"

/************************************************************************************************/
/*
* Process a value and store in PVT
*/
/************************************************************************************************/
int si_proc_process_value(struct si_sfs_value * sfs_value,
                          struct si_sfs_packet_data * sfs_packet_data)
{

  if (si_util_get_param_ptr() == NULL) return -1;

  struct si_sfs_param_info * const param_info_entry = (struct si_sfs_param_info *) (si_util_get_param_ptr() + sfs_value->param_location);

//  printf("Processing %s\n", param_info_entry->param_name);
  struct si_sfs_value calibrated_value;
  calibrated_value.value_type = SI_SFS_NONE;

  struct si_sfs_valid validity;
  validity.valid = 0;

  // Calculate the validity
  int is_valid = si_proc_value_valid(sfs_value);
  if (is_valid == 1) {
    validity.bits.state_validity = 0;
    validity.bits.validity_error = 0;
  } else if (is_valid == 0) {
    validity.bits.state_validity = 1;
    validity.bits.validity_error = 0;
  } else {
    validity.bits.state_validity = 0;
    validity.bits.validity_error = 1;
  }

  /* Calibrate if can be calibrated */
  if ((param_info_entry->param_type == SI_SFS_UINT) || 
      (param_info_entry->param_type == SI_SFS_INT)) {

    /* Call routine to perform calibration */
    int ret_val = si_util_calibrate_parameter(param_info_entry, sfs_value, &calibrated_value);

    if (ret_val != -1) {
      /* Set calibration error validity flag to 0 */
      validity.bits.calibration_error = 0;
    } else {
      /* Set calibration error validity flag to 1 */
      validity.bits.calibration_error = 1;
    }
  }

 /* Save the value */
  si_proc_save_value(sfs_value, 
                     &calibrated_value,
		     &validity,
		     &sfs_packet_data->packet_time,
		     param_info_entry->pvt_location);

  return 1;
}
/************************************************************************************************/

/************************************************************************************************/
/*
* Get validity of parameter
*/
/************************************************************************************************/
int si_proc_value_valid(struct si_sfs_value * sfs_value)
{
  int ret_val = 0;
  struct si_sfs_param_info * const param_info_entry = (struct si_sfs_param_info *) 
             (si_util_get_param_ptr() + sfs_value->param_location);

  /* valid by default if no parameter defined */
  if (param_info_entry->valid_location <= 0) return 1;

  struct si_sfs_param_info * const valid_param_info_entry = (struct si_sfs_param_info *) 
            (si_util_get_param_ptr() + param_info_entry->valid_location);

  struct si_sfs_value valid_value;
  valid_value.value_type = valid_param_info_entry->param_type;
  
  si_proc_get_raw_value(valid_param_info_entry->pvt_location, 
                        &valid_value);

  /* validity parameters must be integers */
  if (valid_value.value_type == SI_SFS_UINT) {
    ret_val = valid_value.value.uival == param_info_entry->valid_value ? 1 : 0;
  } else if (valid_value.value_type == SI_SFS_INT) {
    ret_val = valid_value.value.ival == param_info_entry->valid_value ? 1 : 0;
  } else {
    ret_val = -1;
  }
  return ret_val;
}
/************************************************************************************************/
