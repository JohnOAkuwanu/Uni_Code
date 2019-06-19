PY2C_Comm()
{
while(1){
//Instantiation of COMMAND struct
  COMMAND cmd;
   char* update;
   int i;
//This reads the data from the serial and processes it
   readCmdBuff(&cmd);
   uart_debug_msg("");

/*
This checks to see what letter is stored as the command within the COMMAND struct - to know what process to implement to the data stored in infile
P = PY2C_PLAY
C = PY2C_COPY
N = PY2C_RENAME
T = PY2C_PROBE
D = PY2C_DELETE
L = PY2C_LOADDATA
F = PY2C_FILEINFO
All defined in serialcomm.h
*/
   if(cmd.cmd == PY2C_PLAY)
   {
     //uart_debug_msg("received");
     //write_usb_serial_blocking("data from C", 15);
     uart_debug_msg(cmd.infile);
     //uart_debug_msg(cmd.outfile);
     break;
   }


   if(cmd.cmd == PY2C_COPY)
   {
     //uart_debug_msg(cmd.infile);
     update = cmd.infile;
     sd_copy_file(update, "afive.txt");
     break;
   }

   if(cmd.cmd == PY2C_RENAME)
   {
     sd_file_rename("a5.wav", "afive.txt");
     break;
   }


   if(cmd.cmd == PY2C_PROBE)
   {
     int count; //No. of file entries/
    char strings[40][16]; //String array
    count = sd_read_directory("", strings);

    int p;
    for(p=0; p < count; p++)
    {
      uart_debug_msg(strings[p]);
    }
     break;
   }

   if(cmd.cmd == PY2C_DELETE)
   {
     delete_file("newfil.txt");
     break;
   }

   if(cmd.cmd == PY2C_FILEINFO)
   {
     BYTE wav_buffer[44];
     sd_file_open("a5.wav");
     sd_read_file(wav_buffer, 44);

     //FILE SIZE CHECK

     uint32_t file_size = ((wav_buffer[7] << 24) |
                           (wav_buffer[6] << 16) |
                           (wav_buffer[5] << 8 ) |
                            wav_buffer[4]);

    char jef[30];
    sprintf(jef, "Size: %d", file_size);
    uart_debug_msg(jef);


    if(cmd.cmd == PY2C_LOADDATA)
    {
      uint32_t file_size = ((wav_buffer[7] << 24) |
                            (wav_buffer[6] << 16) |
                            (wav_buffer[5] << 8 ) |
                             wav_buffer[4]);

      BYTE wav_buffer[44];
      sd_file_open("a5.wav");
      sd_read_file(wav_buffer, 44);

      //FILE SIZE CHECK

      while(f_eof(&fil) != 0) {
        for(i = 0; i < SD_BUFFER_SIZE; i++)
        {
          uart_debug_hex(wav_buffer[i]);
        }
      }

     /*
     struct WAVHEADER header = wav_header_parse("A5.WAV");
     char jef[30];
     sprintf(jef, "File Size: %d", header.file_size);
     uart_debug_msg(jef);
     */
     break;
   }
   }
 }
}