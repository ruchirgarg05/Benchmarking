#!/bin/sh

bin2hex() {
	   echo "obase=16;ibase=2;$1"|bc
}

banned_cpu_bitmask=$(bin2hex 00011110) # We isolate CPUs 1,2,3,4

sed -i "s/.*IRQBALANCE_BANNED_CPUS=.*/IRQBALANCE_BANNED_CPUS=$banned_cpu_bitmask/" /etc/default/irqbalance

ISOLCPUS_VAL="isolcpus=1,2,3,4"

# Replace isolcpus and reboot

sed -i "s/GRUB_CMDLINE_LINUX_DEFAULT=.*/GRUB_CMDLINE_LINUX_DEFAULT=\"vultr $ISOLCPUS_VAL\"/" /etc/default/grub
update-grub
reboot
