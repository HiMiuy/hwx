#include <stdio.h>
#include <time.h>
#include <unistd.h>
#include "headers.h"

uint64_t countv4 = 0;
uint64_t countv6 = 0;
//uint64_t bin[BIN];
uint64_t *bin;
uint64_t *eachbin;
int now_binsize;

void libtrace_cleanup(libtrace_t *trace, libtrace_packet_t *packet) {
    if (trace)
        trace_destroy(trace);
    if (packet)
        trace_destroy_packet(packet);
}

void output_csv(FILE* file1, FILE* file2, double time1, int time2) {
    fprintf(file1, "%f, %d, %lld, %lld\n", time1, time2, countv4, countv6);
    int i;
    //fprintf(file2, "%d ", time2);
    for (i = 0; i < now_binsize; i++) {
        fprintf(file2, "%lld ", eachbin[i]);
        eachbin[i] = 0; // to 0
    }
    fprintf(file2, "\n");
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
    int  POWEROF2   = atoi(argv[4]);
    double next_report = 0;
    double prepkt_time = 0;
    int    time_step   = 0;
    /* Output parameters */
    printf("Read file        : %s\n", READ_PCAP);
    printf("Observation time : %f\n", REPORT_TIME);
    printf("Power of 2       : %d\n", POWEROF2);

    bin = (uint64_t*)calloc(BIN, sizeof(uint64_t));
    eachbin = (uint64_t*)calloc(BIN, sizeof(uint64_t));
    now_binsize = BIN;

    FILE *outfile;
    char outstr[100] = "./output/csv/time_pkts";
    strcat(outstr, argv[2]);
    strcat(outstr, ".csv");

    FILE *allbin;
    char allbinstr[100] = "./output/csv/allbin";
    strcat(allbinstr, argv[2]);
    strcat(allbinstr, "_");
    strcat(allbinstr, argv[4]);
    strcat(allbinstr, "power.csv");

    printf("%s\n", outstr);
    
    outfile = fopen(outstr, "w");
    allbin  = fopen(allbinstr, "w");
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
            int pos = (int)diff_time >> POWEROF2;
            /* debag */ 
            //double bin_size = pow(2,POWEROF2);
            //int pos = diff_time/bin_size;
            
            while(pos >= now_binsize) {
                now_binsize += BIN;
                bin = (uint64_t*)realloc(bin, sizeof(uint64_t)*now_binsize);
                eachbin = (uint64_t*)realloc(eachbin, sizeof(uint64_t)*now_binsize);
            }
            bin[pos]++;
            eachbin[pos]++;
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
            output_csv(outfile, allbin, next_report, time_step);
            countv4 = 0;
            countv6 = 0;
            next_report += REPORT_TIME;
            time_step++;
        }
    }

    FILE *outfilebin;
    char outbin[100] = "./output/csv/bin";
    strcat(outbin, argv[2]);
    strcat(outbin, "_");
    strcat(outbin, argv[4]);
    strcat(outbin, "power.csv");
    outfilebin     = fopen(outbin, "w");
    int i;
    
    for (i =0; i < now_binsize; i++) {
        fprintf(outfilebin, "%lld ", bin[i]);
    }
    free(bin);
    fclose(outfile);
    fclose(outfilebin);
}