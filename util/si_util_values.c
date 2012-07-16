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
#include <arpa/inet.h>
#include "memory.h"
#include "stdio.h"
#include "si_util_values.h"

/************************************************************************************************/
/*
*  read unsigned integer value from data td starting at bit position bitpos
*  with bit length bitlen
*/
/************************************************************************************************/
unsigned int si_util_get_uint(const char *td, unsigned int bitpos, unsigned int bitlen) 
{

  unsigned int st;

  //copy in an unsigned variable
  memcpy(&st, td, 4);
  st = ntohl(st);

  st = st << bitpos;                //clean with '0' the left hand side.
  st = st >> (32 - bitlen);         //clean with '0' the right hand side by
                                   //shifting to the right.
  if(bitpos+bitlen > 32)
  {
    unsigned char st1 = td[4];           // get next byte
    st1 = st1 >> (8 - ((bitpos+bitlen) - 32)); // get the overlapping bits
    st = st | st1;            // perform or to set in value
  }
  return st;
}
/*****************************************************************************************
*
*    Extract a integer of "len" bits from a string with
*  bit offset 0. We suppose the number is in A'2 complement
*    There are two cases:
*        POSITIVE NUMBER: e.g. length= 8 bits and value 00000011
*        extract the sing, is 0 or positive.  shift to the right (32-8)=24 pos.
*        (don't care about 0's to the left)
*            00000000 00000000 00000000 00000011    and cast to and integer
*        NEGATIVE NUMBER: must be in A 2 complement.
*        e.g. length 8 bits and value 11111000  (-8)    extract the sign, 
*        is 1 or negative , shift to the right (32-8)=24 pos.
*        (don't care about 1's to the left)
*            00000000 00000000 00000000 11111000
*        create a unsigned filled with 1's and shift to    the left 8 pos. 
*        to allocate the mask value. After that do and bitwise OR between them. 
*            11111111 11111111 11111111 00000000
*            00000000 00000000 00000000 11111000  .OR.
*            -----------------------------------
*            11111111 11111111 11111111 11111000 is -8 in A 2 complement
*    
********************************************************************************************/

int si_util_get_int(const char *td, unsigned int bitpos,unsigned int bitlen)
{

  static unsigned int one_temp= 0xFFFFFFFF;    //fill with 1's a mask

  unsigned int st;

  //copy in an unsigned variable
  memcpy(&st, td, 4);
  st = ntohl(st);

  st = st << bitpos;                //shift to the left to extract sign

  unsigned int sign = st & 0x80000000;  //extract the sign

  st = st >> (32 - bitlen);    //shift to the right inserting 0's in left

  //negative numbers
  if (sign != 0)
  {
      unsigned int temp= one_temp;    //fill with 1's a temporal variable
      if (bitlen < 32)
      {
        temp= temp << bitlen;            //put 0' in the data area by shifting
      }
      else
      {
        // if len = 32, temp << len returns all 1's instead of all 0's !!!
        temp = 0;
      }
      st = st | temp;            //OR between both values
  }

  if (bitpos+bitlen > 32)
  {
    unsigned char st1 = td[4]; // get next byte
    st1 = st1 >> (8 - ((bitpos+bitlen) - 32));
    st = st | st1;
  }

  return (int)st;
}

/************************************************************************************************/
/*    Extract a float from a string with
*     bit offset 0. They must have ALWAYS 4 bytes
*/
/************************************************************************************************/

float si_util_get_float(const char *td, unsigned int bitpos,unsigned int bitlen)
{
  if (bitlen != 32)
  {
    return -1;
  }

  float f_val;
  if (bitpos != 0)
  {
    unsigned int tmp;
    tmp = si_util_get_uint(td,bitpos,32);// get next 4 bytes properly adjusted 
    memcpy(&f_val,&tmp,4);
  }
  else
  {
    unsigned int st;
    memcpy(&st, td, 4);
    st = ntohl(st);
    memcpy(&f_val, &st, 4);
  }

  return f_val;

}
/************************************************************************************************/
/*
*    Extract a float from a string with
*    bit offset 0. They must have ALWAYS 8 bytes
*/
/************************************************************************************************/
double si_util_get_double(const char *td, unsigned int bitpos,unsigned int bitlen)
{
  if (bitlen != 64)
  {
    return -1;
  }
  double f_val;

  if (bitpos != 0)
  {
    unsigned int tmp;
    unsigned int tmp1;
    tmp  = si_util_get_uint(td,bitlen,32);  // get next 4 bytes properly adjusted
    tmp1 = si_util_get_uint(td+4,bitlen,32);// get next 4 bytes properly adjusted

    memcpy(&f_val,&tmp,4);      // copy first 4 bytes
    memcpy((&f_val)+4,&tmp1,4); // copy next 4 bytes
  }
  else
  {
    union {
      double d;
      struct {
        unsigned int first;
        unsigned int second;
      } twoint;
    } swp;

    memcpy(&swp.d, td, 8);

    unsigned int second = swp.twoint.second;   
    swp.twoint.second = ntohl(swp.twoint.first);
    swp.twoint.first = ntohl(second);

    f_val = swp.d;
  }
  return f_val;
}

/************************************************************************************************/
