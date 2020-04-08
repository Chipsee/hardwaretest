#!/bin/sh
#check if the system is already running in ap mode.
#if it does, stop the ap and revert to station mode
UhcpdRunning=`ps x | grep -c udhcpd`
if [ $UhcpdRunning -eq 2 ]; then
	killall udhcpd
fi
HostapdRunning=`ps x | grep -c hostapd`
if [ $HostapdRunning -eq 2 ]; then
	killall hostapd
	sleep 1
	echo 0 > /proc/sys/net/ipv4/ip_forward
	iptables -F
	ifconfig wlan0 down
	iwconfig wlan0 mode managed
	ifconfig wlan0 up
	sleep 1
fi

#start the supplicant in case it is not running already
if [ -e /var/run/wpa_supplicant/wlan0 ]; then
	rm /var/run/wpa_supplicant/wlan0
fi

wpaAlreadyInstalled=`ps x | grep -c wpa_supplicant`

if [ $wpaAlreadyInstalled -eq 2 ]; then
	killall wpa_supplicant
	wpa_supplicant -d -Dnl80211 -c/etc/wpa_supplicant.conf -iwlan0 -B
	sleep 1
elif [ $wpaAlreadyInstalled -eq 1 ]; then
	wpa_supplicant -d -Dnl80211 -c/etc/wpa_supplicant.conf -iwlan0 -B
	sleep 1
fi
#check if udhcpc is already started for wlan0 if it doesnt, start it
udhcpAlreadyInstalled=`ps x | grep -c -E "udhcpc -R -b -p /var/run/udhcpc.wlan0.pid"`

if [ $udhcpAlreadyInstalled -eq 2 ]; then
	killall udhcpc
	udhcpc -R -b -p /var/run/udhcpc.wlan0.pid -i wlan0
	sleep 1
elif [ $udhcpAlreadyInstalled -eq 1 ]; then
	udhcpc -R -b -p /var/run/udhcpc.wlan0.pid -i wlan0
	sleep 1
fi
