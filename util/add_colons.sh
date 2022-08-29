#!/bin/bash

# 
# format a MAC address with colons
#
# 080027A79BD7 --> 08:00:27:A7:9B:D7
#

A=$1

echo ${A:0:2}:${A:2:2}:${A:4:2}:${A:6:2}:${A:8:2}:${A:10:2}
