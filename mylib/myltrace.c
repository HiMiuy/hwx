#include <stdio.h>
#include <time.h>
#include <unistd.h>
#include "headers.h"

uint64_t countv4 = 0;
uint64_t countv6 = 0;
//uint64_t bin[BIN];
uint64_t *bin;
void libtrace_cleanup(libtrace_t *trace, libtrace_packet_t *packet) {
    if (trace)
        trace_destroy(trace);
    if (packet)
        trace_destroy_packet(packet);
}

void output_csv(FILE* file1, double time1, int time2) {
    fprintf(file1, "%f, %d, %lld, %lld\n", time1, time2, countv4, countv6);
}

void per_packet(libtrace_packet_t *packet) {
    uint8_t proto;
    uint16_t ethertype;
    uint32_t rem;
	void *ltheader = NULL;
	libtrace_ip_t *ip = NULL;
    ltheader = trace_get_layer3(packet, &ethertype, &rem);

    if (ethertype == TRACE_ETHERTYPE_IP) {
        ip = (libtrace_ip_t *)ltheader;
        ltheader = trace_get_payload_from_ip(ip, &proto, &rem);
        countv4++;
    }
}

void my_program(libtrace_t *trace, libtrace_packet_t *packet, char* argv[]) {
    /* Read parameters from command line */
    char *READ_PCAP;
    READ_PCAP = (char*)calloc(strlen(argv[1]),sizeof(char));
    READ_PCAP = argv[1];
    double REPORT_TIME = atof(argv[2]);
    bool VERBOSE       = atoi(argv[3]);
    double next_report = 0;
    double prepkt_time = 0;
    int    time_step   = 0;
    /* Output parameters */
    printf("Read file        : %s\n", READ_PCAP);
    printf("Observation time : %f\n",REPORT_TIME);

    bin = (uint64_t*)calloc(BIN, sizeof(uint64_t));
    int now_binsize = BIN;

    FILE *outfile;
    char outstr[100] = "./output/csv/time_pkts";
    strcat(outstr, argv[2]);
    strcat(outstr, ".csv");

    printf("%s\n", outstr);
    
    outfile        = fopen(outstr, "w");
    if (outfile == NULL) {
        printf("cannot open.\n");
        exit(1);
    }
    /* -------- Check all packets -------- */
    while (trace_read_packet(trace, packet)>0) {
        /* Get timestamp */
        double ts = trace_get_seconds(packet);
        double ts_m = ts/10e-6;
        /* Process each packet */
        if (prepkt_time == 0) prepkt_time = ts_m;
        else {
            double diff_time = ts_m - prepkt_time; // micro seconds
            //double bin_size = pow(2,4);
            //double bin_size = 2;
            //int pos = diff_time/bin_size;

            double bin_size = 2;
            //int pos = (int)diff_time >> (int)bin_size;
            int pos = diff_time/bin_size;
            while(pos >= now_binsize) {
                now_binsize += BIN;
                printf("%d\n", now_binsize);
                bin = (uint64_t*)realloc(bin, sizeof(uint64_t)*now_binsize);
            }
            bin[pos]++;
            prepkt_time = ts_m;
        }
        per_packet(packet);

        while (ts > next_report) {
            if (next_report == 0) {
                next_report = ts + REPORT_TIME;
                continue;
            }
            
            if (VERBOSE) {
                printf("TimeStep: %d ", time_step);
                printf("\tThe number of IPv4 packets: %lld\n", countv4);
            }
            output_csv(outfile, next_report, time_step);
            countv4 = 0;
            countv6 = 0;
            next_report += REPORT_TIME;
            time_step++;
        }
    }

    FILE *outfilebin;
    char outbin[100] = "./output/csv/bin";
    strcat(outbin, argv[2]);
    strcat(outbin, ".csv");
    outfilebin     = fopen(outbin, "w");
    int i;
    
    for (i =0; i < now_binsize; i++) {
        fprintf(outfilebin, "%lld ", bin[i]);
    }
    free(bin);
    fclose(outfile);
    fclose(outfilebin);
}