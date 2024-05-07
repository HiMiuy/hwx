CC = gcc
CFLAGS = -Wall -g -O2 -lm
LTRACEF = -ltrace
INCLUDE = -I./mylib
CMS_SOURCE = ./mylib/cms.c ./mylib/myhash.c
CMS_HEADER = $(patsubst %.c,%.h, $(CMS_SOURCE))
PKT_SOURCE = ./mylib/myltrace.c ./mylib/myhash.c
PKT_HEADER = $(patsubst %.c,%.h, $(PKT_SOURCE))
#FILENAME   ="./data/mawi/201902011400_20190201140000.pcap"
FILENAME   ="./data/mawi/201902011400.pcap"
REPORT_TIME= 1
VERBOSE = 0
ARGS = $(FILENAME) $(REPORT_TIME) $(VERBOSE)

all: pktrun #cmsrun

cmsrun: cms
	./cms.o

cms: $(CMS_SOURCE) $(CMS_HEADER) main_cms.c
	$(CC) $(INCLUDE) $(LTRACEF) $(CMS_SOURCE) $(CFLAGS) main_cms.c -o cms.o

pktrun: pkt
	./pkt.o $(ARGS)
pkt: $(PKT_SOURCE) $(PKT_HEADER) main_pkt.c
	$(CC) $(INCLUDE) $(LTRACEF) $(PKT_SOURCE) $(CFLAGS) main_pkt.c -o pkt.o

clean:
	rm -f *.o