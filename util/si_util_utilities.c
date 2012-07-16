/***************************************************************************
*
* PROJECT  : SCOS-I TM 
*
* SUBSYSTEM: UTIL
*
* FILENAME : SI_UTIL_UTILITIES.C
*
* AUTHOR   : Ritchie Kay 
*
* DATE     : 16.06.2010
*
* COPYRIGHT: Ritchie Kay, 2010
*
****************************************************************************
*
* PURPOSE: si_util_utilities provides some generic utilities
*
****************************************************************************/
#include <stdlib.h>
#include <stdio.h>
#include <memory.h>
#include "si_util_utilities.h"
#include "si_util_mmap.h"

/************************************************************************************************/
/*
* Get pointers to mapped memory tables
*/
/************************************************************************************************/

void * si_util_map_table_file(char * name)
{
  const char * tables_dir = getenv("SCOSI_TABLE_DIR");
  char file[strlen(tables_dir) + strlen(name) + 2];
  strcpy(file, tables_dir);
  strcat(file, "/");
  strcat(file, name);
  unsigned int entries;
  printf("File = %s\n", file);
  void * ptr = mapFile(file, &entries);
  printf("Entries in %s = %d\n", name, entries);

  return ptr;

}
/************************************************************************************************/
/*
* Get PVT pointer
*/
/************************************************************************************************/
void * si_util_get_pvt_ptr()
{
  static void * si_util_pvt_ptr = NULL;

  if (!si_util_pvt_ptr) {
    si_util_pvt_ptr = si_util_map_table_file("pvt.tlb");
  }
  return si_util_pvt_ptr;
}

/************************************************************************************************/
/*
* Get PARAM pointer
*/
/************************************************************************************************/
void * si_util_get_param_ptr()
{
  static void * si_util_param_ptr = NULL;

  if (!si_util_param_ptr) {
    si_util_param_ptr = si_util_map_table_file("param.tlb");
  }
  return si_util_param_ptr;
}
/************************************************************************************************/
/*
* Get PACKET pointer
*/
/************************************************************************************************/
void * si_util_get_pkt_ptr()
{
  static void * si_util_pkt_ptr = NULL;

  if (!si_util_pkt_ptr) {
    si_util_pkt_ptr = si_util_map_table_file("pkt.tlb");
  }
  return si_util_pkt_ptr;
}
/************************************************************************************************/
/*
* Get TEXT_CALIB pointer
*/
/************************************************************************************************/
void * si_util_get_text_calib_ptr()
{
  static void * si_util_text_calib_ptr = NULL;

  if (!si_util_text_calib_ptr) {
    si_util_text_calib_ptr = si_util_map_table_file("text_calib.tlb");
  }
  return si_util_text_calib_ptr;
}
/*
* Get NUMERIC_CALIB pointer
*/
void * si_util_get_numeric_calib_ptr()
{
  static void * si_util_numeric_calib_ptr = NULL;

  if (!si_util_numeric_calib_ptr) {
    si_util_numeric_calib_ptr = si_util_map_table_file("numeric_calib.tlb");
  }
  return si_util_numeric_calib_ptr;
}


/************************************************************************************************/
/*
*  performs a binary search for an integer search_int through data searched_data
*  with record size record_size and with max values max_values
*/
/************************************************************************************************/

const char * si_util_uint_binary_search(int search_int,  
                                        const char * searched_data,
                                        unsigned int record_size,
                                        unsigned int max_values)
{
  int left_most = 0;
  int right_most = max_values;
  int direction = 1;
  int position = left_most;
  int last_position = right_most;

  int key_val;
  while ( position != last_position) {
    last_position = position;
    position += direction * (int)((right_most - left_most)/2);
  
    key_val = *(int *)&searched_data[position*record_size];

    if (key_val == search_int) {
      return &searched_data[position*record_size];
    } else if (key_val < search_int) {
      left_most = position; 
      direction = 1;
    } else if (key_val > search_int) {
      right_most = position;
      direction = -1;
    }
  }
  return NULL; 
}

/************************************************************************************************/

/************************************************************************************************/
const char * si_util_uint_binary_search_in_range(int search_int,
                                                 const char * searched_data,
                                                 unsigned int record_size,
                                                 unsigned int max_values)
{
  int left_most = 0;
  int right_most = max_values;
  int direction = 1;
  int position = left_most;
  int last_position = right_most;

  int first_val;
  int second_val;

  while ( position != last_position) {
    last_position = position;
    position += direction * (int)((right_most - left_most)/2);
    first_val = *(int *)&searched_data[position*record_size]; 
    second_val = *(int *)&searched_data[position*record_size+sizeof(int)]; 

    if ((first_val <= search_int) && second_val >= search_int) {
      return &searched_data[position*record_size];
    } else if (second_val < search_int) {
      left_most = position; 
      direction = 1;
    } else if (first_val > search_int) {
      right_most = position;
      direction = -1;
    }
  }
  return NULL; 
}
/************************************************************************************************/

