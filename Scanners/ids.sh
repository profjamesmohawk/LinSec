#!/bin/bash

{
find /etc -type f -exec md5sum {} \; 
find / -type f -executable -exec md5sum {} \;
} > i.$(date +%s)
