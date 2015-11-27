rm -f /etc/opkg/base-feeds.conf
echo 'src all http://iotdk.intel.com/repos/1.1/iotdk/all
src x86 http://iotdk.intel.com/repos/1.1/iotdk/x86
src i586 http://iotdk.intel.com/repos/1.1/iotdk/i586'\
 >> /etc/opkg/base-feeds.conf
opkg update
opkg install git
git clone https://github.com/jjrob13/IOT_Education.git
chmod +x IOT_Education/config_files/on_boot.sh
cp IOT_Education/config_files/on_boot.service /lib/systemd/system 
systemctl enable on_boot.service
systemctl start on_boot.service
