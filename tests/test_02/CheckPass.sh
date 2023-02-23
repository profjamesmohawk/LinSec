#!/bin/bash

#
# check passwd against expected passwords
#
# reads base64 encoded shadow formatted lines from stdin
#

PWL="one two"

base64 -d | cut -f 2 -d ':' \
	| {
		        IFS='$'
        while
            read P0 C S D
        do
		 for PASS in smartboss coolboss theman talkingmoose 
		 do
            newD=$(openssl passwd -$C -stdin -salt $S <<< $PASS)

                #echo "\$${C}\$${S}\$${D}"
				#echo "$newD"
            if
                [ "$newD" = "\$${C}\$${S}\$${D}" ]
            then
                echo "hash looks good: $PASS "
            else
                echo "hash looks bad : $PASS"
            fi
		 done
        done
	}
