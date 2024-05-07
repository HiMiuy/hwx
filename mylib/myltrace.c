#include <stdio.h>
#include <time.h>
#include <unistd.h>
#include "headers.h"

uint64_t countv4 = 0;
uint64_t countv6 = 0;
uint64_t bin[BIN] = {0};

long print_ip(struct sockaddr *ip) {
    /* Check the family and get addr as long */
    if (ip->sa_family == AF_INET) {
        struct in_addr source_ip_addr;
		struct sockaddr_in *v4 = (struct sockaddr_in *)ip;
		source_ip_addr=v4->sin_addr;
        return ntohl(source_ip_addr.s_addr);
	}
    else return 0;
}

void libtrace_cleanup(libtrace_t *trace, libtrace_packet_t *packet) {
    if (trace)
        trace_destroy(trace);
    if (packet)
        trace_destroy_packet(packet);
}

void output_csv(FILE* file1, FILE* file2, double time1, int time2) {
    int i;
    fprintf(file1, "%f, %d, %lld, %lld\n", time1, time2, countv4, countv6);
    fprintf(file2, "%f, %d ", time1, time2);
    for (i = 0; i < BIN; i++) {
        fprintf(file2, ", %lld", bin[i]);
        bin[i] = 0;
    }
    fprintf(file2, "\n");
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
    
    FILE *outfile, *outfilebin,*outfilebinraw;
    char outstr[100] = "./output/csv/time_pkts";
    strcat(outstr, argv[2]);
    strcat(outstr, ".csv");
    char outbin[100] = "./output/csv/bin";
    strcat(outbin, argv[2]);
    strcat(outbin, ".csv");

    char outbinraw[100] = "./output/csv/binraw";
    strcat(outbinraw, argv[2]);
    strcat(outbinraw, ".json");
    printf("%s\n", outstr);
    
    outfile = fopen(outstr, "w");
    outfilebin  = fopen(outbin, "w");
    outfilebinraw  = fopen(outbinraw, "w");
    if (outfile == NULL) {
        printf("cannot open.\n");
        exit(1);
    }
    fprintf(outfile,"# timestamp, timestep, pktv4counts, pktv6counts\n");
    fprintf(outfilebinraw, "{\"%d\": [", time_step);
    bool flg = 1;
    /* -------- Check all packets -------- */
    while (trace_read_packet(trace, packet)>0) {
        /* Get timestamp */
        double ts = trace_get_seconds(packet);
        double ts_m = ts/10e-6;
        /* Process each packet */
        if (prepkt_time == 0) prepkt_time = ts_m;
        else {
            double diff_time = ts_m - prepkt_time; // micro seconds
            double bin_size = pow(2,4);
            int pos = diff_time/bin_size;
            bin[pos]++;
            prepkt_time = ts_m;
            if (VERBOSE) {
                if (flg) {
                    fprintf(outfilebinraw, "%f", diff_time);
                    flg = 0;
                }
                else {
                    fprintf(outfilebinraw, ", %f", diff_time);
                }
            }
        }
        per_packet(packet);

        while (ts > next_report) {
            if (next_report == 0) {
                next_report = ts + REPORT_TIME;
                continue;
            }
            
            if (VERBOSE) {
                /*
                int i;
                printf("TimeStep: %d\n", time_step);
                printf("\tThe number of IPv4 packets: %lld\n", countv4);
                printf("\tThe number of IPv6 packets: %lld\n", countv6);
                for (i = 0; i < BIN; i++) {
                    printf("%lld ", bin[i]);
                }
                printf("\n");
                */
            }
            output_csv(outfile, outfilebin, next_report, time_step);
            fprintf(outfilebinraw, "], \n\"%d\": [", time_step+1);
            countv4 = 0;
            countv6 = 0;
            flg = 1;
            next_report += REPORT_TIME;
            time_step++;
        }
    }
    fprintf(outfilebinraw, "]}\n");
    fclose(outfile);
    fclose(outfilebin);
    fclose(outfilebinraw);
}
void per_packet(libtrace_packet_t *packet) {
    uint8_t proto;
    uint16_t ethertype;
    uint32_t rem;
	void *ltheader = NULL;
	libtrace_ip_t *ip = NULL;
	libtrace_ip6_t *ip6 = NULL;
    ltheader = trace_get_layer3(packet, &ethertype, &rem);
    /*
    struct sockaddr_storage addr;
	struct sockaddr *addr_ptr;
    // get srcAddr 
    addr_ptr = trace_get_source_address(packet, (struct sockaddr *)&addr);
    long ip_src = print_ip(addr_ptr);
    // get dstAddr 
    addr_ptr = trace_get_destination_address(packet, (struct sockaddr *)&addr);
    long ip_dst = print_ip(addr_ptr);    
    */
    /* get the payload */
    size_t payload_length;
    if (ethertype == TRACE_ETHERTYPE_IP) {
        ip = (libtrace_ip_t *)ltheader;
        ltheader = trace_get_payload_from_ip(ip, &proto, &rem);
        payload_length = ntohs(ip->ip_len) - (4 * ip->ip_hl);
        int point = payload_length/100;
        bin[point]++;
        countv4++;
    }
    else if (ethertype == TRACE_ETHERTYPE_IPV6) {
        ip6 = (libtrace_ip6_t *)ltheader;
        ltheader = trace_get_payload_from_ip6(ip6, &proto, &rem);
        payload_length = ntohs(ip6->plen);
        countv6++;
    }
}