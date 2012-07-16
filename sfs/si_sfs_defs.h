/***************************************************************************
*
* PROJECT  : SCOS-I TM 
*
* SUBSYSTEM: SFS
*
* FILENAME : SI_SFS_DEFS.H 
*
* AUTHOR   : Ritchie Kay 
*
* DATE     : 24.06.2010
*
* COPYRIGHT: Ritchie Kay, 2010
*
****************************************************************************
*
* PURPOSE: si_sfs_defs provides definitions of enumerations and constants
*          defined in the SFS systems
*
****************************************************************************/
#ifndef SI_SFS_DEFS
#define SI_SFS_DEFS
/*
*  ENUMERATIONS
*/
enum si_sfs_param_types 
{
  SI_SFS_UINT   = 'U',
  SI_SFS_INT    = 'I',
  SI_SFS_FLOAT  = 'F',
  SI_SFS_DOUBLE = 'D',
  SI_SFS_STRING = 'S'
};
enum si_sfs_value_types
{
  SI_SFS_NONE = 'N',
  SI_SFS_RAW = 'R',
  SI_SFS_SYNTH = 'D',
  SI_SFS_HCSYNTH = 'H',
  SI_SFS_CONSTANT = 'C'
};
enum si_sfs_calibration_type
{
  SI_SFS_STATUS = 'S',
  SI_SFS_NUMERICAL = 'N'
};
enum si_sfs_textual_calibration_type
{
  SI_SFS_ENUMERATION = 'E',
  SI_SFS_TEXTUAL = 'T'
};
/* Determines which parameters in a packet are process */
enum si_util_packet_processing {
  SI_UTIL_ALL,
  SI_UTIL_DEPENDENCY,
  SI_UTIL_NON_DEPENDENCY
};
/*
* CONSTANTS
*/


#endif
