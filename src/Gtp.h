#ifndef _GTP_H_
#define _GTP_H_

const int GTP_COMMAND_NUM = 30;
const int GTP_COMMAND_SIZE = 64;
const int BUF_SIZE = 256;

#define DELIM  " "
#define PROGRAM_NAME  "Rayon"
#define PROGRAM_VERSION  "8.0.1+3.0"
#define PROTOCOL_VERSION  "2"

#if defined (_WIN32)
#define STRCPY(dst, size, src) strcpy_s((dst), (size), (src))
#define STRTOK(src, token, next) strtok_s((src), (token), (next))
#else
#define STRCPY(dst, size, src) strcpy((dst), (src))
#define STRTOK(src, token, next) strtok((src), (token))
#endif

typedef struct {
  char command[GTP_COMMAND_SIZE];
  void (*function)();
} GTP_command_t;

#define CHOMP(command) if(command[strlen(command)-1] == '\n') command[strlen(command)-1] = '\0'

// シミュレーションの手を使う
void SetSimMove( bool );
// gtp本体
void GTP_main( void );

#endif
