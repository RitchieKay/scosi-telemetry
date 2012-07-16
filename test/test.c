#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/mman.h>
#include <stdio.h>
#include <unistd.h>
#include <time.h>

#include "util/si_util_utilities.h"
#include "proc/si_proc_utilities.h"
#include "proc/si_proc_packet.h"

int main(int argc, char** argv)
{
  si_proc_init();

  struct timeval packet_time;
  char * ptr = (char *)malloc(500);
  
  static int d = 0;

  unsigned int lasttime = time(NULL);
  do {
    gettimeofday(&packet_time, NULL);

    int i;
    for (i = 0; i < 500 ; i++) {
//    ptr[i] = i/2;
      ptr[i] = d%255;
    }
    d++;

    struct si_sfs_packet_data packet_data = {packet_time, 52000, 1, ptr};

    si_proc_process_sfs_packet(&packet_data);

    unsigned int count = 0;
    unsigned int now;

    while(1) {
      now = time(NULL);

      if ((now-lasttime) > 10) {

        now = time(NULL);
        printf("Packet rate = %f per second. %d packets in last period.\n", 1.0*count/(now-lasttime), count);
        count = 0;
        lasttime = now;

      }

      si_proc_process_sfs_packet(&packet_data);

      count++;
    }
    si_proc_cleanup();
    sleep(1);
  } while (1);
  free(ptr);
  return 1;
}

