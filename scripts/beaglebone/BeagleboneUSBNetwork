#!/bin/bash

echo "/* Internet over USB setup */"

#wlan0 is the internet facing interface, eth1 is the BeagleBone USB connection
iptables --table nat --append POSTROUTING --out-interface wlan0 -j MASQUERADE
iptables --append FORWARD --in-interface eth1 -j ACCEPT
echo 1 > /proc/sys/net/ipv4/ip_forward

echo "/* Done */"
