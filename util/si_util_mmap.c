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

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/mman.h>
#include <stdio.h>
#include <unistd.h>
#include <memory.h>

#include "si_util_mmap.h"

/************************************************************************************************/
/*
* Map to a file identified by name, number of entries in file is
* returned in entries
*/
/************************************************************************************************/
void * mapFile(const char * name, unsigned int *entries)
{
  /* open file */
  int fd = open(name, O_RDWR | O_CREAT , S_IRUSR | S_IWUSR); 

  if (fd < 0) {
    perror("Could not open file: ");
    return 0;
  }

  struct stat details; 
  stat(name, &details);  
  int fileLength = details.st_size;

  /* Map to file */
  void * ptr = mmap(NULL, fileLength, PROT_READ | PROT_WRITE , MAP_SHARED , fd, 0);

  if ((int)ptr == -1) {
    perror("Failed to create mmap:"); 
    return 0;
  }

  *entries = *(unsigned int *) ptr; 

  return ptr;
}

/************************************************************************************************/
/*
* Unmap to a file identified by name and pointer to memory 
* where it is held.
*/
/************************************************************************************************/
void unmapFile(const char * name, void * ptr)
{
  struct stat details; 
  stat(name, &details);  
  int fileLength = details.st_size;

  munmap(ptr, fileLength);
}
/************************************************************************************************/
