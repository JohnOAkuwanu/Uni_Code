//This is a guard so that the header file is only defined once
#ifndef _SERIALCOMM_H_
#define _SERIALCOMM_H_

//Series of commands from the first character sent down the terminal
#define PY2C_PLAY 'P'
#define PY2C_COPY 'C'
#define PY2C_DELETE 'D'
#define PY2C_VOLUME 'V'
#define PY2C_REVERSE 'R'
#define PY2C_RENAME 'N'
#define PY2C_PROBE 'T'
#define PY2C_FILEINFO 'F'
#define PY2C_LOADDATA 'L'

//The command struct. Two parts command then data
typedef struct {
  char cmd;
  char infile[17];
} COMMAND;

#endif

void readCmdBuff(COMMAND*);
void parseCommand(char*, COMMAND*);