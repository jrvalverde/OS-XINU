
#ifndef _tftp_h
#define _tftp_h

#define OURTFTP_PORT           30000
#define TFTP_PORT              30000
#define TIMEOUTCOUNT	           5

typedef struct _req_init
  {
    short type;
    char data[512];
  } req_init;

typedef struct _req_data
  {
    short type;
    short block;
    char data[512];
  } req_data;
typedef struct _req_ack
  {
    short type;
    short block;
  } req_ack;

#endif

