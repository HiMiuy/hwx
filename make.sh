#!/bin/bash
set -ue
## ========== PARAMETERS ========== 
FILENAME="./data/mawi/201902011400_20190201140000.pcap"
FILENAME="./data/mawi/201902011400.pcap"
FILENAME="../hw0/data/mawi/201902011400.pcap"
REPORT_TIME=10
VERBOSE=0
CMAKE=1
ARGS="${FILENAME} ${REPORT_TIME} ${VERBOSE}"

## Make dir ##
mkdir -p output/png
mkdir -p output/csv
if [ $CMAKE == 0 ]; then
    make pktrun ARGS="${ARGS}"
else
    ./build/main_pkt ${ARGS}
fi

python3 plot1.py -t ${REPORT_TIME} -v ${VERBOSE}
