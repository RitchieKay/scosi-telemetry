#include <stdio.h>
#include "sfs/si_sfs_tables.h"
#include "util/si_util_utilities.h"

int main(int argc, char** argv)
{

  void * text_calib_ptr = si_util_get_text_calib_ptr();

  unsigned int number_entries = *(unsigned int *) text_calib_ptr;

  text_calib_ptr += sizeof(unsigned int);

  for (;number_entries > 0;number_entries--) {

    struct si_sfs_calibration_header * calib_header = 
         (struct si_sfs_calibration_header *) text_calib_ptr;


    text_calib_ptr += sizeof(struct si_sfs_calibration_header);

    printf("---------------------------------------------------------------------\n");
    printf("Calibration Type = %c\n", calib_header->calibration_type);
    printf("Number Entries   = %d\n", calib_header->number_entries);
   
    int i;
    for (i = 0; i < calib_header->number_entries; i++) { 

      struct si_sfs_textual_calibration * text_calib =
           (struct si_sfs_textual_calibration *)text_calib_ptr;

      printf("%d\t%d\t%d\t%s\n", text_calib->low_raw, text_calib->high_raw,
                                 text_calib->value_defined, text_calib->textual_value);

      text_calib_ptr += sizeof(struct si_sfs_textual_calibration);
    }
  }

  return 1;
}

