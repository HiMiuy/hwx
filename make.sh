#!/bin/bash
set -ue
## ========== PARAMETERS ========== 
FILENAME="./data/mawi/201902011400_20190201140000.pcap"
FILENAME="./data/mawi/201902011400.pcap"

REPORT_TIME=1
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

echo "Plotting..."
python3 plot1.py -t ${REPORT_TIME} -v ${VERBOSE}
