#!/bin/bash

OUT_FILE=$(hostname)_report.html

# map stdout to <hostname>_report.html
#
rm $OUT_FILE
exec 1<> $OUT_FILE

################ functions to spit html ################################
#
function spit_start {
	echo "<tr>"
	echo "<td valign=\"top\"> $1 </td>"
	echo "<td><pre>"
	echo " "

	# tell the user what we are up to
	echo "Collecting Info for $1" | sed -e 's/<.*>//g' >&2
}

function spit_end {
	echo " "
	echo "</pre>"
	echo "</td>"
	echo "</tr>"
}

function spit_pre {
	echo "<html>"
	echo "<head>"
	echo "<style type=\"text/css\">"
	echo "table { margin: 1em; border-collapse: collapse; }"
	echo "td, th { padding: .3em; border: 2px #ccc solid; }"
	echo "</style>"
	echo "</head>"

	echo "<body><table>"
}

function spit_post {
	echo "</table></body></html>"
}

function spit_title {
	echo "<h2> $1 </h2>"
}

function spit_section_header {
	echo "<tr><td colspan=\"2\"> $1 </td></tr>"
}
#
################# end html spitters ######################################

#
# are we root?
#
if (( EUID != 0 ))
then
	echo "You must be root to run this script" >&2
	exit 1
fi

#
# Tell the nice people what we are going to do
#
echo "This script will collect interesting data from your system" >&2
echo "The results will be placed in $OUT_FILE, feel free to have a look." >&2
echo "The best way to view it is a full browser, but lynx will do in a pinch." >&2


#
# from here it's a mix is spits and commands
#
#
spit_pre

spit_title "Test 01 Report"


spit_section_header "Work must be done on test day."
spit_start "When was I built?"
ls -l /var/log/anaconda.log
spit_end

spit_start "When is now?"
date
spit_end


spit_section_header "Part A: Build and patch web01 (3 points)"

spit_start Hostname
hostname
spit_end

spit_start "IP Address configured with network manager"
nmcli connection show enp0s3 | grep -e ipv4.method -e ipv4.addresses
spit_end

spit_start "Hosts file"
grep -e web01 -e yoda /etc/hosts
spit_end

spit_section_header "Part B: Apache (2 points)"
spit_start "New default page contains <strong>Welcome to web01</strong>"
curl http://web01/ | grep -e Welcome -e web01
spit_end

spit_section_header "Part C: Kickstart file (2 points)"
echo "coming soon"
spit_end

spit_section_header "Part D: bash added to miniPatch(3 points)"
yum list bash
spit_end


spit_section_header "Complete Kickstart file from which I was built (for reference only)"
cat /root/anaconda-ks.cfg
spot_post
