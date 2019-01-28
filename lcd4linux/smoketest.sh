#! /bin/bash

#  $Id: smoketest.sh 1177 2012-02-22 03:11:31Z michael $
#  $URL: https://ssl.bulix.org/svn/lcd4linux/trunk/smoketest.sh $


rm -f smoketest.log lcd4linux

make distclean
./bootstrap

for driver in ASTUSB BeckmannEgle BWCT CrystalFontz Curses Cwlinux D4D DPF EA232graphic EFN FutabaVFD G15 GLCD2USB HD44780 IRLCD LCD2USB LCDLinux LCDTerm LEDMatrix LPH7508 LUIse LW_ABP M50530 MatrixOrbital MatrixOrbitalGX MilfordInstruments Noritake NULL Pertelian PHAnderson picoLCD picoLCDGraphic PNG PPM RouterBoard Sample SamsungSPF serdisplib SimpleLCD T6963 TeakLCM Trefon ULA200 USBHUB USBLCD WincorNixdorf X11; do

    make distclean
    ./configure --with-drivers=$driver
    make -s -j 8
    
    if [ -x lcd4linux ]; then
	echo "Success: drv_$driver" >>smoketest.log
    else
	echo "FAILED:  drv_$driver" >>smoketest.log
    fi
    
done

for plugin in apm asterisk button_exec cpuinfo dbus diskstats dvb exec event fifo file gps hddtemp huawei i2c_sensors iconv imon isdn kvv loadavg meminfo mpd mpris_dbus mysql netdev netinfo pop3 ppp proc_stat python qnaplog sample seti statfs uname uptime w1retap wireless xmms; do

    make distclean
    ./configure --with-drivers=NULL --with-plugins=$plugin
    make -s -j 8
    
    if [ -x lcd4linux ]; then
	echo "Success: plugin_$plugin" >>smoketest.log
    else
	echo "FAILED:  plugin_$plugin" >>smoketest.log
    fi
    
done

make distclean
./configure
