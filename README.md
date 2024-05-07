# Get the number of packets in report time interval

This is a C language implementation the number of packets in report time interval you decide using Libtrace.
I ran it only in Ubuntu18 environment.

## Requirement
- python3
- cmake(?)
- C
- Libtrace ([Libtrace](https://github.com/LibtraceTeam/libtrace))


1. Initialize an environment to output the result as figure with:
```bash
# Use source
source ./setup.sh
or
bash ./setupy.sh
source venv/bin/activate
```

2. Execute program with:
```bash
# The result is placed in ./output
bash make.sh
or
./build/main_pkt "filename of pcap" "observation time"
```

3. Clean up env with:
```bash
bash ./remove.sh
```