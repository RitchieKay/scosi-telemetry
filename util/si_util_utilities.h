/***************************************************************************
*
* PROJECT  : SCOS-I TM 
*
* SUBSYSTEM: UTIL
*
* FILENAME : SI_UTIL_UTILITIES.H 
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
#ifndef SI_UTIL_UTILITIES
#define SI_UTIL_UTILITIES
/*
* Get pointers to mapped memory tables
*/
void * si_util_get_pvt_ptr();
void * si_util_get_param_ptr();
void * si_util_get_pkt_ptr();
void * si_util_get_text_calib_ptr();
void * si_util_get_numeric_calib_ptr();
/*
*  performs a binary search for an integer search_int through data searched_data
*  with record size record_size and with max values max_values
*/

const char * si_util_uint_binary_search(int search_int,  
                                        const char * searched_data,
                                        unsigned int record_size,
                                        unsigned int max_values);
/*
* very similar bit now expects that the first two entries in the record are a
* range and therefore expects the search_int to lie within this range
*/
const char * si_util_uint_binary_search_in_range(int search_int,  
                                        const char * searched_data,
                                        unsigned int record_size,
                                        unsigned int max_values);


#endif
