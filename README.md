# Get the number of packets in report time interval
Results are placed in `output/png/`. <br>
I want to practice how to use Github, make, and cmake. <br>
<br>
This is a C language implementation the number of packets in report time interval you decide using Libtrace.
I ran it only in my environment (macOS Sonoma 14.4.1).

## Requirement
- python3
- cmake (Practice to use)
- C
- [Libtrace](https://github.com/LibtraceTeam/libtrace)

## Execution
1. Initialize an environment to output the result as figure with:
```bash
# Use source
source ./setup.sh
or
bash ./setupy.sh
source .venv/bin/activate
```

2. Execute program with:
```bash
# The result is placed in ./output
## bash
bash make.sh (See bash file before run) 
```
or
```bash
## make
make exe POWEROF2=2 (Specify a single argument)
make exe PCAP="./file.pcap" TIME=1 VERBOSE=0 POWEROF2=1 (Specify multi arguments)
```
or
```bash
## cmake
./build/main_pkt "./file.pcap" 1 0 1
python plot1.py -t 1 -v 0 -p 1 --log (yaxis is log-scale)
python plot1.py -t 1 -v 0 -p 1       (yaxis is normal)
```

3. Clean up env with:
```bash
bash ./remove.sh
```