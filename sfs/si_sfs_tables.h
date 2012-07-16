/***************************************************************************
*
* PROJECT  : SCOS-I TM 
*
* SUBSYSTEM: SFS
*
* FILENAME : SI_SFS_TABLES.H 
*
* AUTHOR   : Ritchie Kay 
*
* DATE     : 16.06.2010
*
* COPYRIGHT: Ritchie Kay, 2010
*
****************************************************************************
*
* PURPOSE: si_sfs_tables provides a definition of the memory structures in
*          use within the "SCOS-I Functional System - SFS.
*          In addition generic structures uses within the SFS are also
*          described.
*
****************************************************************************/

#ifndef SI_SFS_TABLES
#define SI_SFS_TABLES
#include "sfs/si_sfs_defs.h"
#include <sys/time.h>

/* 
* The sfs_param_info holds the type of the parameter as well as the location
* of the parameter in the pvt table as well as more to come...
*/

struct si_sfs_param_info
{
  char param_name[9];
  char param_type;
  char param_calib_type;
  char param_value_type;
  unsigned int bitlen;
  unsigned int pvt_location;
  unsigned int calib_location;
  unsigned int limits_location;
  unsigned int valid_location;
  unsigned int valid_value;

};
/*
* the sfs_packet_location_index holds the identifier of the packet (spid)
* as well as the location of the details of the parameter content of the 
* packet in the packet table. The index is the first part of the packet 
* location table which is followed by the records.
*/

struct si_sfs_packet_location_index
{
  unsigned int packet_id;
  unsigned int packet_location;
};
/*
*
*/
struct si_sfs_packet_location_header
{
  unsigned int number_param_entries;
  /* a depenceny parameter is one upon whose value another parameter in the pacet
  *  may depend. These are located first in the packet */
  unsigned int number_dependency_entries; 
};
/*
* The sfs_packet_locations represent the locations of the parameters in the
* param info table as well as the byte and bit offset of their value within
* the packet.
*/
struct si_sfs_packet_locations
{
  unsigned int param_location;
  unsigned int byte_offset;
  unsigned int bit_offset;
};

/*
* The si_sfs_pvt_entry_common represents the common data of an entry in the pvt table
* The pvt table entry contains this for each parameter followed by an encoding of
* the value by parameter type
*
*/
struct si_sfs_pvt_entry_common
{
  unsigned int tv_sec;
  unsigned int tv_usec;
  unsigned int validity;
  unsigned int limits;
};
/*
*
*
*/
struct si_sfs_calibration_header
{
  char calibration_type;
  unsigned int number_entries;
};

struct si_sfs_textual_calibration
{
  int low_raw;
  int high_raw;
  int value_defined;
  char         textual_value[16];

};

struct si_sfs_numerical_calibration
{
  int low_raw;
  int high_raw;
  double low_eng;
  double high_eng;
};
/*
* The following structure do not represent structures in mmap files
*/

/*
*  si_sfs_packet_data represents the data that must be known for processing
*  a packet.
*
*/

struct si_sfs_packet_data
{
  struct timeval packet_time;
  unsigned int packet_id;
  unsigned int stream_id;
  char * packet_data;
};
/*
* The si_sfs_value_xxxx set of structures provides a structure for passing
* about parameter values together with the time for which they apply as well
* as their validity.
*/
struct si_sfs_value
{
  unsigned int param_location;
  enum si_sfs_param_types value_type;
  union {
    unsigned int uival;
    int          ival;
    float        fval;
    double       dval;
    char *       sval;
  } value;
};

struct si_sfs_valid
{ 
  union {
    struct {
      unsigned int state_validity:1;
      unsigned int validity_error:1;
      unsigned int calibration_error:1;
      unsigned int the_rest:29;
    } bits;
    unsigned int valid;
  };

};
#endif
