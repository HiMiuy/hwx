#ifndef _MYLTRACE_H_
#define _MYLTRACE_H_

extern long print_ip(struct sockaddr*);
extern void libtrace_cleanup(libtrace_t*, libtrace_packet_t*);
extern void my_program(libtrace_t*, libtrace_packet_t*, char**);
extern void per_packet(libtrace_packet_t*);
extern void output_csv(FILE*, FILE*, double, int);
extern uint64_t countv4;
extern uint64_t countv6;
#define BIN 20
//extern uint64_t bin[BIN];
extern uint64_t *bin;
extern uint64_t *eachbin;
extern int now_binsize;


#endif
