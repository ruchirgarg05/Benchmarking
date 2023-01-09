#!/bin/sh

now=$(date +%s)
cpuid_A=2 # Client Server CPU
cpuid_B=4 # Remote Server CPU

USER="root"
REMOTE_IP=149.28.102.216
KEY_PATH="~/cse221_vultr"

cmd="make CFLAGS='-DIPV4_ADDR=\\\"$REMOTE_IP\\\"' rtt"

# echo "Benchmarking Remote ICMP RTTs"

# echo "Setting up remote proces..."
# ssh $USER@$REMOTE_IP 'cd CSE221_F22_Project; make rtt; ./executables/icmp_remote_server_RTT'

# echo "Executing on client..."
# fname="${now}_icmp_remote_rtt.out"
# taskset -c $cpuid_A ./executables/icmp_remote_RTT > raw_data/$fname

######################################################################

echo "Benchmarking remote TCP RTTs"

echo "Setting up remote process..."

ssh -i $KEY_PATH $USER@$REMOTE_IP bash -s << EOF
	cd CSE221_F22_Project 
	make CFLAGS='-DIPV4_ADDR=\"$REMOTE_IP\"' rtt
	nohup taskset -c $cpuid_B ./executables/tcp_remote_server_RTT > tcp_rtt.out 2>&1 &
EOF

sleep 5

echo "Executing on client..."
fname="${now}_tcp_remote_rtt.out"

taskset -c $cpuid_A ./executables/tcp_remote_client_RTT > raw_data/$fname

echo "Killing remote server process..."
ssh -i $KEY_PATH $USER@$REMOTE_IP bash -s << 'EOF'
	kill $(pgrep -f tcp_remote_server_RTT)
EOF
sleep 2

######################################################################

echo "Benchmarking remote TCP connection overhead"

echo "Setting up remote process..."
ssh -i $KEY_PATH $USER@$REMOTE_IP bash -s << EOF
	cd CSE221_F22_Project 
	make CFLAGS='-DIPV4_ADDR=\"$REMOTE_IP\"' conn_overhead
	nohup taskset -c $cpuid_B ./executables/tcp_remote_server_conn_overhead > conn_overhead.out 2>&1 &
EOF

sleep 5

echo "Executing on client..."
fname="${now}_tcp_remote_conn_overhead.out"

taskset -c $cpuid_A ./executables/tcp_remote_client_conn_overhead > raw_data/$fname

echo "Killing remote server process..."
ssh -i $KEY_PATH $USER@$REMOTE_IP bash -s << 'EOF'
	kill $(pgrep -f tcp_remote_server)
EOF
sleep 2

######################################################################

echo "Benchmarking remote TCP Peak Bandwidth"

echo "Setting up remote process..."
ssh -i $KEY_PATH $USER@$REMOTE_IP bash -s << EOF
	cd CSE221_F22_Project 
	make CFLAGS='-DIPV4_ADDR=\"$REMOTE_IP\"' bw 
	nohup taskset -c $cpuid_B ./executables/tcp_remote_server_bw > bw.out 2>&1 &
EOF

sleep 5
echo "Executing on client..."

fname="${now}_tcp_remote_bw.out"

taskset -c $cpuid_A ./executables/tcp_remote_client_bw > raw_data/$fname

echo "Killing remote server process..."
ssh -i $KEY_PATH $USER@$REMOTE_IP bash -s << 'EOF'
	kill $(pgrep -f tcp_remote_server)
EOF
sleep 2
