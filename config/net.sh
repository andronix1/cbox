#!/bin/bash

set -e

if [ ! "0" -eq "$UID" ]; then
    echo "must be run in sudo"
    exit 1
fi


PID=$1

if [ -z "$PID" ]; then
   echo "usage $0 <PID>"
   exit 1
fi

NS=runner2

(ip netns list | grep "^$NS" > /dev/null) && sudo ip netns delete $NS

for LINK in "rbr" "br-reth"; do
	(sudo ip link list | grep "$LINK" > /dev/null) && sudo ip link delete "$LINK"
done

ip netns attach $NS $PID
ip link add reth type veth peer name br-reth
ip link set reth netns $NS
ip netns exec $NS ip addr add 192.168.1.11/24 dev reth
ip link add name rbr type bridge
ip link set rbr up
ip link set br-reth up
ip netns exec $NS ip link set reth up
ip link set br-reth master rbr
ip addr add 192.168.1.10/24 brd + dev rbr
ip netns exec $NS ip route add default via 192.168.1.10
