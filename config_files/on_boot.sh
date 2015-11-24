#!/bin/sh
/usr/sbin/rfkill unblock bluetooth
/usr/bin/hciconfig hci0 up
/usr/bin/hciconfig hci0 piscan
systemctl start obex
nohup python /home/root/IOT_Education/config_files/ip.py &
