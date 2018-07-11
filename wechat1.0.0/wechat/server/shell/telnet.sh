#!/bin/bash
for k in $(seq 1 50000)
do
    telnet 127.0.0.1 6888 &
done
