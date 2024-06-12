rfkill unblock wifi > /dev/null 2>&1
ifconfig wlan0 up > /dev/null 2>&1
wpa_supplicant -B -i wlan0 -c /etc/wpa_supplicant.conf > /dev/null 2>&1
ip addr add 192.168.163.42/24 dev wlan0 > /dev/null 2>&1
ip addr add 192.168.55.55/24 dev eth0 > /dev/null 2>&1

