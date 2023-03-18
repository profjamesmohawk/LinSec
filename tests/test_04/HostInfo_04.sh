#!/bin/bash

OUT_FILE=$(hostname)_t4_report.html

# map stdout to <hostname>_t4_report.html
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
echo "Be patient, it could take a few minutes to run." >&2


#
# from here it's a mix is spits and commands
#
#
spit_pre

spit_title "COMP-10032 Host Info Report - Hands On Evaluation 4"

spit_start Hostname
hostname
spit_end

spit_start "When was I built?"
ls -l /var/log/anaconda/anaconda.log
spit_end

spit_start "When is now?"
date
spit_end


spit_section_header "Part A: httpd"

spit_start "systemctl settings"
systemctl status httpd
spit_end

spit_start "test with curl"
curl http://localhost:8888
spit_end

spit_section_header "Part B: Kickstart"

spit_start "Kickstart fragment encoded with base64 </br>(use based64 -d to read)"
curl http://10.1.1.100/Kickstart/default.ks |grep amita| base64
spit_end

spit_section_header "Part C: YUM"

spit_start "looking for cpio 2.12-10.el8 "
yum list cpio
spit_end


spit_section_header "Appendix:"
spit_start "Complete Kickstart file"
curl http://10.1.1.100/Kickstart/default.ks | base64
spit_end


spit_post
