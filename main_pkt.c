#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <string.h>

#include "libtrace.h"

#include "headers.h"

int main (int argc, char * argv[]) {
    libtrace_t *trace = NULL;
    libtrace_packet_t *packet = NULL;
    /* Ensure we have at least one argument after the program name */
    if (argc < 2) {
        fprintf(stderr, "Usage: %s inputURI\n", argv[0]);
        return 1;
    }
    packet = trace_create_packet();
    if (packet == NULL) {
        perror("Creating libtrace packet");
        libtrace_cleanup(trace, packet);
        return 1;
    }
    /* Try to read a pcap file */
    trace = trace_create(argv[1]);
    if (trace_is_err(trace)) {
        trace_perror(trace,"Opening trace file");
        libtrace_cleanup(trace, packet);
        return 1;
    }
    if (trace_start(trace) == -1) {
        trace_perror(trace,"Starting trace");
        libtrace_cleanup(trace, packet);
        return 1;
    }
    /* In my program */
    my_program(trace, packet, argv);

    if (trace_is_err(trace)) {
        trace_perror(trace,"Reading packets");
        libtrace_cleanup(trace, packet);
        return 1;
    }

    libtrace_cleanup(trace, packet);
    return 0;
    
}