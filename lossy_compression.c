/* Standard Libraries */
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

/* LPC Libraries */
#include "LPC17xx.h"
#include "lpc_types.h"
#include "lpc17xx_pinsel.h"
#include "lpc17xx_gpio.h"

/* Our Libraries */
#include "delay.h"
#include "uart.h"
#include "ticktime.h"
#include "ssp.h"
#include "sd.h"
#include "rtc.h"
#include "fatfs/ff.h"
#include "sd_file.h"

FIL fil;
FIL fil2;

void compression_lossy(BYTE * header_buffer, BYTE * data_buffer, int buffer_size, int header_size, char filename1[], char filename2[]) {

  int a;

  memset(data_buffer, 0, buffer_size);

  sd_file_open_read_mode(filename1);
  sd_file_open_write_mode(filename2);

  sd_read_file(header_buffer, header_size);
  sd_write_file(header_buffer, header_size);

  while(sd_check_end() == false) {

    sd_read_file(data_buffer, buffer_size);

    for(a = 0; a < buffer_size; a++) {
      if(a % 4 != 0) {
        sd_write_file(data_buffer[a], buffer_size / buffer_size);
      }
    }

    memset(data_buffer, 0, buffer_size);
  }

  uart_debug_msg("File compressed");

  sd_seek_file(0x00);
  f_lseek(&fil2, 0x00);

  sd_all_file_close();
}


void decompression_lossy(BYTE * header_buffer, BYTE * data_buffer, int buffer_size, int header_size, char filename1[], char filename2[]) {

  int a;

  sd_file_open_read_mode(filename1);
  sd_file_open_write_mode(filename2);

  sd_read_file(header_buffer, header_size);
  sd_write_file(header_buffer, header_size);

  memset(data_buffer, 0 , buffer_size);  //Reset the buffer to 0 before the read/write cycle to ensure no data is read incorrectly

  while(sd_check_end() == false) {  //Only run if end of file is not reached

    sd_read_file(data_buffer, buffer_size);

    for(a = 0; a < buffer_size; a++) {  //
	    if(a % 4 == 0){
      	sd_write_file(((data_buffer[a - 1] + data_buffer[a + 1]) / 2), buffer_size / buffer_size);
	    }
    }

    memset(data_buffer, 0 , buffer_size);  //Reset each value in the buffer to 0 after each read/write cycle

  }

  uart_debug_msg("File decompressed");
  sd_all_file_close();

}

/*
void compression_lossless(BYTE * buffer, int buffer_size) {
  return NULL;
}
*/