#!/bin/bash


libcamera-vid -v -f -n -o - -t 0 -b 2000000 | nc 192.168.1.4 5777
