/***************************************************************************
*
* PROJECT  : SCOS-I TM 
*
* SUBSYSTEM: UTIL
*
* FILENAME : SI_UTIL_MMAP.H 
*
* AUTHOR   : Ritchie Kay 
*
* DATE     : 16.06.2010
*
* COPYRIGHT: Ritchie Kay, 2010
*
****************************************************************************
*
* PURPOSE: si_util_mmap provides methods for mapping/unmapping to the
*          mmap files used within the SFS
*
****************************************************************************/
#ifndef SI_UTIL_MMAP
#define SI_UTIL_MMAP

/*
* Map to a file identified by name, number of entries in file is
* returned in entries
*/

void * mapFile(const char * name, unsigned int *entries);
/*
* Unmap to a file identified by name and pointer to memory 
* where it is held.
*/
void unmapFile(const char * name, void * ptr);

#endif
