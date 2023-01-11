#!/bin/bash
echo "Start UDP Scanning : $1"
readonly START=$2
readonly END=$3
echo "Port from $START to $END"
for ((i = START; i<=END;i++))
do
./UDPPort-Scanner $1 $i
sleep 1
done

echo "End UDP Scanning"