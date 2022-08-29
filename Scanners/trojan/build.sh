#!/bin/bash

# simple script to build and package the CO10032 trojans
# yes, if I was a real man I use make

gcc -o trojan_a CO10032_trojan_a.c
gcc -o trojan_b CO10032_trojan_b.c
gcc -o trojan_c CO10032_trojan_c.c

tar zcf trojan_a_b.tgz trojan_a trojan_b
tar zcf trojan_c.tgz trojan_c
