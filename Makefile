## Reference: https://qiita.com/Tanoren/items/f8c843a05455f593249f, https://qiita.com/thaladabar/items/1f72a3359acb5cf7428e
CC      = gcc
CFLAGS  = -Wall -g -O2 -lm
LTRACEF = -ltrace
INCLUDE = -I./mylib
#TGT     = pkt
## About source
SRCDIR = mylib/
SRCS   = $(wildcard $(SRCDIR)*.c)
## About object
OBJDIR  = obj/
OBJS    = $(patsubst %.c, %.o,$(SRCS))
## Default params
PCAP     = "./../../data/mawi/201902011400.pcap"
TIME     = 1
VERBOSE  = 0
POWEROF2 = 1

## Rule
.PHONY: .c.o
.c.o :
	$(CC) $(INCLUDE) $(LTRACEF) $(CFLAGS) -o $@ -c $< 

.PHONY: pkt
pkt: $(OBJS) main_pkt.o
	$(CC) $(INCLUDE) $(LTRACEF) $(CFLAGS) $(OBJS) main_pkt.o -o pkt
	
.PHONY: exe
exe: pkt
	mkdir -p output/png
	mkdir -p output/csv
	@echo "=============== Parameters ==============="
	@echo PCAP=$(PCAP)
	@echo TIME=$(TIME)
	@echo VERBOSE=$(VERBOSE)
	@echo POWEROF2=$(POWEROF2)
	@echo "=============== Executing ================"
	./pkt $(PCAP) $(TIME) $(VERBOSE) $(POWEROF2)
	@echo "========== Plotting the results =========="
	python plot1.py -t $(TIME) -p $(POWEROF2)
	python plot1.py -t $(TIME) -p $(POWEROF2) --log

.PHONY: clean
clean:
	rm -f $(OBJDIR)*.o $(SRCDIR)*.o *.o pkt a.out

.PHONY: all
all: clean $(OBJS) pkt