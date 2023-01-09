#!/bin/sh

now=$(date +%s)
cpuid_A=2
cpuid_B=4

echo "Benchmarking Loopback ICMP RTTs"

fname="${now}_icmp_loopback_rtt.out"
taskset -c $cpuid_A ./executables/icmp_loopback_RTT > raw_data/$fname

######################################################################

echo "Benchmarking Loopback TCP RTTs"

fname="${now}_tcp_loopback_rtt.out"

taskset -c $cpuid_B ./executables/tcp_loopback_server_RTT &
sleep 2
server_pid=$!

taskset -c $cpuid_A ./executables/tcp_loopback_client_RTT > raw_data/$fname
kill -9 $server_pid

######################################################################

echo "Benchmarking Loopback TCP connection overhead"

fname="${now}_tcp_loopback_conn_overhead.out"

taskset -c $cpuid_B ./executables/tcp_loopback_server_conn_overhead &
sleep 2
server_pid=$!

taskset -c $cpuid_A ./executables/tcp_loopback_client_conn_overhead > raw_data/$fname
kill -9 $server_pid

######################################################################

echo "Benchmarking Loopback TCP Peak Bandwidth"

fname="${now}_tcp_loopback_bw.out"

taskset -c $cpuid_B ./executables/tcp_loopback_server_bw &
sleep 2
server_pid=$!

taskset -c $cpuid_A ./executables/tcp_loopback_client_bw > raw_data/$fname
kill -9 $server_pid
