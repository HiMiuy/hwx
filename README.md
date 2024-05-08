# Get the number of packets in report time interval
Results are placed in `output/png/` <br>
This is a C language implementation the number of packets in report time interval you decide using Libtrace.
I ran it only in Ubuntu18 environment.
I want to practice how to use Github, make, and cmake.

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
bash make.sh
or
./build/main_pkt "filename of pcap" "observation time" "verbose"
python plot1.py -t "observation time" -v "verbose"
```

3. Clean up env with:
```bash
bash ./remove.sh
```