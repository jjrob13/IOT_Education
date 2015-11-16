#!/bin/sh
/usr/sbin/rfkill unblock bluetooth
/usr/bin/hciconfig hci0 up
/usr/bin/hciconfig hci0 piscan
systemctl start obex
python /home/root/ArduinoFiles/ip.py
nohup Capstone/thread_test >> log/thread_test.log&
