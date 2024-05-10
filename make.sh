#!/bin/bash
set -ue
## ========== PARAMETERS ========== 
PCAP="./../../data/mawi/201902011400.pcap"
TIME=1
VERBOSE=0
POWEROF2=1
CMAKE=0

## Make dir ##
mkdir -p output/png
mkdir -p output/csv
if [ $CMAKE == 0 ]; then
    make exe PCAP=${PCAP} TIME=${TIME} VERBOSE=${VERBOSE} POWEROF2=${POWEROF2}
else
    ./build/main_pkt ${PCAP} ${TIME} ${VERBOSE} ${POWEROF2}
fi

echo "Plotting..."
python plot1.py -t ${TIME} -p ${POWEROF2}
python plot1.py -t ${TIME} -p ${POWEROF2} --log
