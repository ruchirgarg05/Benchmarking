#!/bin/sh

now=$(date +%s)

echo "Benchmarking Memory Latency"
fname="${now}_mem_access_latency.csv"
echo "bytes_read,n_cycles" > raw_data/$fname
# nice -n -20 executables/mem_access_latency >> raw_data/$fname
taskset -c 3 executables/mem_access_latency >> raw_data/$fname

# sudo nice -n -20 executables/mem_access_latency

# sudo nice -n -20 executables/mem_access_latency