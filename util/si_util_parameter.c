/***************************************************************************
*
* PROJECT  : SCOS-I TM 
*
* SUBSYSTEM: UTIL
*
* FILENAME : SI_UTIL_PARAMETER.C
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
#include <stdlib.h>
#include <stdio.h>
#include <memory.h>
#include "si_util_parameter.h"
#include "si_util_utilities.h"
#include "sfs/si_sfs_tables.h"

/************************************************************************************************/
/* Definitions for routines used internally */

/************************************************************************************************/
int si_util_textual_calibration(struct si_sfs_param_info * param_info, 
                                struct si_sfs_value * raw_value,
                                struct si_sfs_value * calib_value);

int si_util_numerical_calibration(struct si_sfs_param_info * param_info, 
                                  struct si_sfs_value * raw_value,
                                  struct si_sfs_value * calib_value);

/************************************************************************************************/
/*
* si_util_calibrate_parameter is exposed to users of the util library
*/
/************************************************************************************************/
int si_util_calibrate_parameter(struct si_sfs_param_info * param_info, 
                                struct si_sfs_value * raw_value, 
                                struct si_sfs_value * calib_value)
{
  switch (param_info->param_calib_type) {
    case SI_SFS_STATUS:
      return si_util_textual_calibration(param_info, raw_value, calib_value);
      break;
    case SI_SFS_NUMERICAL:
      return si_util_numerical_calibration(param_info, raw_value, calib_value);
      break;
    default:
      return -1;
  }
  return 0;
}
/************************************************************************************************/
/*
*
*/

/************************************************************************************************/
int si_util_textual_calibration(struct si_sfs_param_info * param_info,
                                struct si_sfs_value * raw_value,
                                struct si_sfs_value * calib_value)
{
  if (param_info->calib_location <= 0) {
    return -1;
  }

  struct si_sfs_calibration_header * calib_hdr = 
    (struct si_sfs_calibration_header *) (si_util_get_text_calib_ptr() + param_info->calib_location);

  /* Obtain the raw value as an integer */
  int value;
  switch (raw_value->value_type) {
        case SI_SFS_INT:
          value = raw_value->value.ival;
          break;
        case SI_SFS_UINT:
          value = (int)raw_value->value.uival;
          break;
        case SI_SFS_STRING:
        case SI_SFS_DOUBLE:
        case SI_SFS_FLOAT:
        default:
          return -1;
          break;
  }

  const char * calib_records = (const char *)(si_util_get_text_calib_ptr()
                             + param_info->calib_location
                             +  sizeof(struct si_sfs_calibration_header));
  struct si_sfs_textual_calibration * text_calib;

  calib_value->value_type = SI_SFS_NONE;

  switch (calib_hdr->calibration_type) {
    case SI_SFS_ENUMERATION:

      if (value > calib_hdr->number_entries) {
        return -1;
      }

      text_calib = (struct si_sfs_textual_calibration *)calib_records;

      if (text_calib[value].value_defined == 0) {
        return -1;
      }

      calib_value->value_type = SI_SFS_STRING;
      calib_value->value.sval = (char *)&text_calib[value].textual_value;

      break;
    case SI_SFS_TEXTUAL:

      text_calib = (struct si_sfs_textual_calibration *)  
	           si_util_uint_binary_search_in_range(value,
                                                       calib_records,
	                                               sizeof(struct si_sfs_textual_calibration),
                                                       calib_hdr->number_entries);
      if (!text_calib || (text_calib->value_defined == 0)) {
        return -1;
      }
      calib_value->value_type = SI_SFS_STRING;
      calib_value->value.sval = (char *)&text_calib->textual_value;

      break;
  default:
    return -1;
  }
  return 1;
}
/************************************************************************************************/
/*
* si_util_numerical_calibration performs a numerical calibration of the parameter value
*/
/************************************************************************************************/
int si_util_numerical_calibration(struct si_sfs_param_info * param_info, 
                                  struct si_sfs_value * raw_value,
                                  struct si_sfs_value * calib_value) 
{
  /* Check that calibration is defined, otherwise return */
  if (param_info->calib_location <= 0) {
    return -1;
  }
  /* Whatever else happens the return value is a double */
  calib_value->value_type = SI_SFS_NONE;

  /* Get the calibration header based on the location */
  struct si_sfs_calibration_header * calib_hdr =
    (struct si_sfs_calibration_header *) (si_util_get_numeric_calib_ptr() + param_info->calib_location);

  /* Get the value as an integer */
  /* Obtain the raw value as an integer */
  int value;
  switch (raw_value->value_type) {
        case SI_SFS_INT:
          value = raw_value->value.ival;
          break;
        case SI_SFS_UINT:
          value = (int)raw_value->value.uival;
          break;
        case SI_SFS_STRING:
        case SI_SFS_DOUBLE:
        case SI_SFS_FLOAT:
        default:
          return -1;
          break;
  }

  /* Get pointer to start of calibration records */
  const char * calib_records = (const char *)(si_util_get_numeric_calib_ptr()
                             + param_info->calib_location
                             +  sizeof(struct si_sfs_calibration_header));

  /* 
  *  Perform a binary search in range - i.e. the search routine expects the data to start with
  *  two integers and searches for the record whose search value lies between then
  */
  struct si_sfs_numerical_calibration * numeric_calib = (struct si_sfs_numerical_calibration *)
                    si_util_uint_binary_search_in_range(value,
                                                        calib_records,
                                                        sizeof(struct si_sfs_numerical_calibration),
                                                        calib_hdr->number_entries);
   if (!numeric_calib) {
      return -1;
   }

   /* Now calculate the calibrated value based on the records */
   calib_value->value.dval = 1.0*(value - numeric_calib->low_raw)/(numeric_calib->high_raw - numeric_calib->low_raw)
                           * (numeric_calib->high_eng - numeric_calib->low_eng) + numeric_calib->low_raw;

   calib_value->value_type = SI_SFS_DOUBLE;
   return 1;
}
/************************************************************************************************/
