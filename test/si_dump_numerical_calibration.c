#include <stdio.h>
#include "sfs/si_sfs_tables.h"
#include "util/si_util_utilities.h"

int main(int argc, char** argv)
{

  void * numeric_calib_ptr = si_util_get_numeric_calib_ptr();

  unsigned int number_entries = *(unsigned int *) numeric_calib_ptr;

  numeric_calib_ptr += sizeof(unsigned int);

  for (;number_entries > 0;number_entries--) {

    struct si_sfs_calibration_header * calib_header = 
         (struct si_sfs_calibration_header *) numeric_calib_ptr;


    numeric_calib_ptr += sizeof(struct si_sfs_calibration_header);

    printf("---------------------------------------------------------------------\n");
    printf("Calibration Type = %c\n", calib_header->calibration_type);
    printf("Number Entries   = %d\n", calib_header->number_entries);
   
    int i;
    for (i = 0; i < calib_header->number_entries; i++) { 

      struct si_sfs_numerical_calibration * numeric_calib =
           (struct si_sfs_numerical_calibration *)numeric_calib_ptr;

      printf("%d\t%d\t%f\t%f\n", numeric_calib->low_raw, numeric_calib->high_raw,
                                 numeric_calib->low_eng, numeric_calib->high_eng);

      numeric_calib_ptr += sizeof(struct si_sfs_numerical_calibration);
    }
  }

  return 1;
}


