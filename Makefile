CC      = gcc
CFLAGS  = -Wall -g -O2 -lm
LTRACEF = -ltrace
INCLUDE = -I./mylib
TGT     = pkt
## About source
SRCDIR = mylib/
SRCS   = $(wildcard $(SRCDIR)*.c)
## About object
OBJDIR  = obj/
OBJS    = $(patsubst %.c, %.o,$(SRCS))
## Parameters
FILENAME    ="../../data/mawi/201902011400.pcap"
REPORT_TIME = 1  # Observation time
VERBOSE     = 0  # printf() in C if 1
POWEROF2    = 2  # the power of 2
ARGS        = $(FILENAME) $(REPORT_TIME) $(VERBOSE) $(POWEROF2)

.PHONY: .c.o
.c.o :
	$(CC) $(INCLUDE) $(LTRACEF) $(CFLAGS) -o $@ -c $< 

.PHONY: $(TGT)
$(TGT): $(OBJS) main_pkt.o
	$(CC) $(INCLUDE) $(LTRACEF) $(CFLAGS) $(OBJS) main_pkt.o -o $(TGT)
	
.PHONY: exe
exe: $(TGT)
	./$(TGT) $(ARGS)

.PHONY: clean
clean:
	rm -f $(OBJDIR)*.o $(SRCDIR)*.o *.o $(TGT) a.out

.PHONY: all
all: clean $(OBJS) $(TGT) 