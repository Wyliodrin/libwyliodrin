#!/bin/sh
echo "/* Internet over USB setup */"

echo "Setting up the default gateway ..."
/sbin/route add default gw 192.168.7.1
 
echo "Updating the nameserver entry ..."
echo "nameserver 8.8.8.8" >> /etc/resolv.conf

echo "/* Done */"