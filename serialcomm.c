#include "lpc_types.h"
#include "serial.h"
#include "serialcomm.h"
#include <string.h>
#include "sd.h"
char cmdBuffer[100];
//Change BYTE to char if it does not work
uint32_t cmdPos;

void readCmdBuff(COMMAND* cmd){
    // Check for the function sent by the GUI
    cmdPos += read_usb_serial_none_blocking(&cmdBuffer[cmdPos], 10);
    // write_usb_serial_blocking(cmdBuffer,100);
    // Parse buffer for commands
	//
    int i;
    int start;
    int end;
    int length;


    for(i=0; i < sizeof(cmdBuffer); i++)
    {
        if(cmdBuffer[i] == '^')
        {
            start = i;
        }
        else if(cmdBuffer[i] == '*' && start > -1)
        {
            end = i;
            break;
        }
    }



    // If we found a command then extract it and parse it

    if ((start > -1) && (end > -1) && (start < end)){

        length = end - start - 1;
        char cmd_str[100];
        for(i=0; i<length; i++)
        {
            cmd_str[i] = cmdBuffer[start + i + 1];
        }


        cmd_str[i] = 0x00; // Need an extra terminator ->  we delimit with null bytes
        write_usb_serial_blocking(cmd_str,length);
        // Shift buffer down the preexisting character are read. command is read
        for(i=0; i <= end; i++)
        {
            cmdBuffer[i] = cmdBuffer[end + 1 + i];
        }

        // TODO: Parse the command into the struct. Separate the input into Command Then Data
        parseCommand(cmd_str, cmd);

    }
    else
    {
	//Default State is NULL
        cmd->cmd = NULL;
    }
}

void parseCommand(char* str, COMMAND* cmd){
    // Command format: ^[Command][Data]*
    //(*cmd).cmd = str[0];
	//Load Command into struct under cmd
    cmd->cmd = str[0];
    char* data[30];
    int i = 0;
	//Load Data into struct under infile
    strcpy(cmd->infile, &str[1]);
}