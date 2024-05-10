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
or
## make
make POWEROF2=2 (Specify a single argument)
make ARGS="./place/of/data.pcap 1 0 1" (Specify multi arguments)
    -> ("filename of pcap" "observation time" "verbose" "power of 2")
or
## cmake
./build/main_pkt "filename of pcap" "observation time" "verbose" "power of 2"
python plot1.py -t "observation time" -v "verbose" -p "power" --log
```

3. Clean up env with:
```bash
bash ./remove.sh
```