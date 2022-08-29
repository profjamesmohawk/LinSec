#
# Sample Kickstart file for COMP-10032
#
# james, summer 2022
#

#version=RHEL8

# install from the command line with no interaction
cmdline

# agree to RHEL end user legal agreement :( 
eula --agreed

# install from b01 repo
url --url=http://10.1.1.100/83/BaseOS

# include the AppStream repo on b01 too
repo --name="AppStream" --baseurl=http://10.1.1.100/83/AppStream
repo --name="miniPatch" --baseurl=http://10.1.1.100/miniPatch

# log to syslog on b01
#logging --host=10.1.1.20 --level=debug --port=514

%packages
@minimal-environment
nano 
bc 
bash-completion
%end

# Keyboard layouts
keyboard --xlayouts='us'

# System language
lang en_CA.UTF-8

# Network information
network  --bootproto dhcp


# storage configuration
#

# use only sda
ignoredisk --only-use=sda

# remove any existing partitions
clearpart --all --drives=sda

# Let the installer set our partitioning scheme
#autopart

# a classic partition for /boot
part /boot --size=500 --fstype=ext4

# Use the rest of the disk for LVM, leaving some free PE
part pv.01 --size=1 --grow
volgroup VG01 pv.01
logvol swap --recommended --vgname=VG01  --name=LV_swap
logvol / --vgname=VG01 --size=1000  --fstype=ext4 --name=LV_root
logvol /usr --vgname=VG01 --size=2500  --fstype=ext4 --name=LV_usr
logvol /tmp --vgname=VG01 --size=200  --fstype=ext4 --name=LV_tmp
logvol /var --vgname=VG01 --size=500 --fstype=ext4 --name=LV_var
logvol /home --vgname=VG01 --size=200  --fstype=ext4 --name=LV_home


# System timezone
timezone America/New_York --isUtc

# Root password (adminpass)
rootpw --iscrypted $6$iUoPTAnSZ6Rv4RHQ$lmzXSayAdj4VeWvkGt6VYJ0nLacw.rOQWpmJ2dE1iOn6XjS/kcGtW8qeG6RHMJtNgdVbi00CqpOPb3g8lCZYd.

reboot

# Set the passwd policy.
# This is a feature of the AnacondaUI, so it must be enclosed with an %anaconda ... %end block
#
%anaconda
pwpolicy root --minlen=6 --minquality=1 --notstrict --nochanges --notempty
pwpolicy user --minlen=6 --minquality=1 --notstrict --nochanges --emptyok
pwpolicy luks --minlen=6 --minquality=1 --notstrict --nochanges --notempty
%end

# Lines in the %post block will be executed by the shell after installation
#
%post

# we will manage our own repos thank-you
subscription-manager config --rhsm.manage_repos=0

# configure bash the way james likes it ;)
cat >/root/.bashrc << EOF
if [ -f /etc/bashrc ]; then
	. /etc/bashrc
fi
set -o vi
alias lt='ls -lrt'
alias hg='history | grep'
EOF

##########  Lab 2: customization changes ########################
#
#
cd /etc/yum.repos.d
curl -O http://10.1.1.100/yum.repos.d/miniPatch.repo
curl -O http://10.1.1.100/yum.repos.d/yodaBaseOS.repo
curl -O http://10.1.1.100/yum.repos.d/yodaAppStream.repo

cd /etc
curl -O http://10.1.1.100/hosts
#
#
###################### end customization changes ################

%end
