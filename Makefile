CC=gcc
#OPTS=-Werror -O0
OPTS=-O0
#-g for default debug information
#-Werror for warning as error
OPTS_02=-Werror -O2
OPTS_03=-Werror -O3

define standard_compile
	$(CC) $(OPTS) -o executables/$2 measurements/$1/$2/$2.c
endef
define standard_compile_O3
	$(CC) -Werror -O3 -o executables/$2 measurements/$1/$2/$2.c
endef
# standard_compile Usage:
# e.g. $(eval $(call standard_compile,dir,foo))
# is equivalent to
# foo: build
# 	$(CC) $(OPTS) -o build/foo operations/dir/foo/foo.c

all: build \
	 loop_overhead \
	 reading_time_overhead \
	 creation_kthreads \
	 creation_processes \
	 system_call \
	 procedure_call \
	 context_switch_kthreads \
	 context_switch_processes  \
	 page_fault_time
build: 
	mkdir -p executables
	mkdir -p raw_data

################### 1_cpu_scheduling_and_os_services ###################
## loop_overhead

reading_time_overhead: build
	$(CC) $(OPTS) -o executables/reading_time_overhead measurements/CPU/measurement_overhead/reading_time_overhead.c

loop_overhead: build
	$(CC) $(OPTS) -o executables/loop_overhead measurements/CPU/measurement_overhead/loop_overhead.c


## task_creation
creation_kthreads: build
	$(CC) $(OPTS) -o executables/creation_kthreads measurements/CPU/task_creation_time/creation_kthreads.c

creation_processes: build
	$(CC) $(OPTS) -o executables/creation_processes measurements/CPU/task_creation_time/creation_processes.c

# system call
system_call: build
	$(CC) $(OPTS) -o executables/system_call measurements/CPU/system_call/system_call.c

# procedure call
procedure_call: build
	$(CC) $(OPTS) -o executables/procedure_call measurements/CPU/procedure_call/procedure_call.c

## context_switch_time
context_switch_kthreads: build
	$(CC) $(OPTS) -o executables/context_switch_kthreads measurements/CPU/context_switch_time/context_switch_kthreads.c

context_switch_processes: build
	$(CC) $(OPTS) -o executables/context_switch_processes measurements/CPU/context_switch_time/context_switch_processes.c

################### File System ###################

## file cache size
copy_file_cache_size_script: build
	cp measurements/file_system/file_cache_size/file_cache_size.sh executables/file_cache_size.sh

file_cache_size: build  copy_file_cache_size_script
	$(call standard_compile_O3,file_system,file_cache_size)


copy_file_contention_script: build
	cp measurements/file_system/contention/file_contention.sh executables/file_contention.sh

file_contention: build copy_file_contention_script
	$(CC) $(OPTS) -o executables/contention measurements/file_system/contention/contention.c

################### 2_memory ###################

mem_access_latency: build
	$(CC) -o executables/mem_access_latency measurements/memory/mem_access_latency.c -lm

mem_access_bandwidth: build
	$(CC) $(OPTS) -o executables/mem_access_bw measurements/memory/mem_access_bw.c -lm -funroll-all-loops

page_fault_time: build
	$(CC) $(OPTS) -o executables/page_fault_time measurements/memory/page_fault_time/page_fault_time.c

ram_bandwidth: build
	python3 measurements/memory/ram_bandwidth/prepare.py -s 8 -l 1024
	$(CC) -O3 -o build/ram_bandwidth operations/2_memory/ram_bandwidth/ram_bandwidth.c

build/temp_1GB_file: build
	mkfile -n 1g build/temp_1GB_file
	
################### 3_network ###################

rtt: build
	$(CC) $(OPTS_02) -o executables/icmp_loopback_RTT measurements/network/rtt/icmp_loopback_RTT.c
	$(CC) $(OPTS_02) -o executables/tcp_loopback_client_RTT measurements/network/rtt/tcp_loopback_client_RTT.c
	$(CC) $(OPTS_02) -o executables/tcp_loopback_server_RTT measurements/network/rtt/tcp_loopback_server_RTT.c

	$(CC) $(CFLAGS) $(OPTS_02) -o executables/icmp_remote_client_RTT measurements/network/rtt/icmp_remote_client_RTT.c
	$(CC) $(CFLAGS) $(OPTS_02) -o executables/icmp_remote_server_RTT measurements/network/rtt/icmp_remote_server_RTT.c

	$(CC) $(CFLAGS) $(OPTS_02) -o executables/tcp_remote_client_RTT measurements/network/rtt/tcp_remote_client_RTT.c
	$(CC) $(CFLAGS) $(OPTS_02) -o executables/tcp_remote_server_RTT measurements/network/rtt/tcp_remote_server_RTT.c

conn_overhead: build
	$(CC) $(OPTS_02) -o executables/tcp_loopback_client_conn_overhead measurements/network/conn_overhead/tcp_loopback_client_conn_overhead.c
	$(CC) $(OPTS_02) -o executables/tcp_loopback_server_conn_overhead measurements/network/conn_overhead/tcp_loopback_server_conn_overhead.c
	
	$(CC) $(CFLAGS) $(OPTS_02) -o executables/tcp_remote_client_conn_overhead measurements/network/conn_overhead/tcp_remote_client_conn_overhead.c
	$(CC) $(CFLAGS) $(OPTS_02) -o executables/tcp_remote_server_conn_overhead measurements/network/conn_overhead/tcp_remote_server_conn_overhead.c

bw: build
	$(CC) $(OPTS_02) -o executables/tcp_loopback_client_bw measurements/network/bw/tcp_loopback_client_bw.c -lm
	$(CC) $(OPTS_02) -o executables/tcp_loopback_server_bw measurements/network/bw/tcp_loopback_server_bw.c -lm
	
	$(CC) $(CFLAGS) $(OPTS_02) -o executables/tcp_remote_client_bw measurements/network/bw/tcp_remote_client_bw.c -lm
	$(CC) $(CFLAGS) $(OPTS_02) -o executables/tcp_remote_server_bw measurements/network/bw/tcp_remote_server_bw.c -lm

################### 4_file_system ###################


clean:
	rm -f executables/*;
