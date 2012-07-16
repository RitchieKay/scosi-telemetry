/***************************************************************************
*
* PROJECT  : SCOS-I TM 
*
* SUBSYSTEM: UTIL
*
* FILENAME : SI_UTIL_VALUES.H 
*
* AUTHOR   : Ritchie Kay 
*
* DATE     : 17.06.2010
*
* COPYRIGHT: Ritchie Kay, 2010
*
****************************************************************************
*
* PURPOSE: si_util_values provides methods for extracting different types pf
*          values from data
*
****************************************************************************/

/*
*  read unsigned integer value from data td starting at bit position bitpos
*  with bit length bitlen
*/
unsigned int si_util_get_uint(const char *td, unsigned int bitpos, unsigned int bitlen); 

/*
*  read integer value from data td starting at bit position bitpos
*  with bit length bitlen
*/
int si_util_get_int(const char *td, unsigned int bitpos,unsigned int bitlen);

/*
*  read float value from data td starting at bit position bitpos
*  with bit length bitlen
*/
float si_util_get_float(const char *td, unsigned int bitpos,unsigned int bitlen);
/*
*  read double value from data td starting at bit position bitpos
*  with bit length bitlen
*/
double si_util_get_double(const char *td, unsigned int bitpos,unsigned int bitlen);
