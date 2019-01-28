dnl $Id: plugins.m4 1195 2013-02-19 23:17:43Z volker $
dnl $URL: https://ssl.bulix.org/svn/lcd4linux/trunk/plugins.m4 $


dnl LCD4Linux Plugins conf part
dnl
dnl Copyright (C) 1999, 2000, 2001, 2002, 2003 Michael Reinelt <michael@reinelt.co.at>
dnl Copyright (C) 2004, 2005, 2006, 2007 The LCD4Linux Team <lcd4linux-devel@users.sourceforge.net>
dnl
dnl This file is part of LCD4Linux.
dnl
dnl LCD4Linux is free software; you can redistribute it and/or modify
dnl it under the terms of the GNU General Public License as published by
dnl the Free Software Foundation; either version 2, or (at your option)
dnl any later version.
dnl
dnl LCD4Linux is distributed in the hope that it will be useful,
dnl but WITHOUT ANY WARRANTY; without even the implied warranty of
dnl MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
dnl GNU General Public License for more details.
dnl
dnl You should have received a copy of the GNU General Public License
dnl along with this program; if not, write to the Free Software
dnl Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.

AC_MSG_CHECKING([which plugins to compile])
AC_ARG_WITH(
  plugins, 
  [  --with-plugins=<list>   choose which plugins to compile.]
  [                        type --with-plugins=list for a list]	
  [                        of avaible plugins]
  [                        plugins may be excluded with 'all,!<plugin>',]	
  [                        (try 'all,\!<plugin>' if your shell complains...)],
  plugins=$withval, 
  plugins=all
)

plugins=`echo $plugins|sed 's/,/ /g'`

for plugin in $plugins; do

   case $plugin in 
      !*) 
         val="no"
         plugin=`echo $plugin|cut -c 2-`
         ;;
       *) 
         val="yes"
         ;;
   esac
	
   case "$plugin" in
      list)
         AC_MSG_RESULT(
            [available plugins:]
            [ apm,asterisk,button_exec,cpuinfo,dbus,diskstats,dvb,exec,event,]
            [ fifo,file,gps,hddtemp,huawei,i2c_sensors,iconv,imon,isdn,kvv,]
            [ loadavg,meminfo,mpd,mpris_dbus,mysql,netdev,netinfo,pop3,ppp,]
	    [ proc_stat,python,qnaplog,raspi,sample,seti,statfs,uname,uptime,]
            [ w1retap,wireless,xmms])
         AC_MSG_ERROR([run ./configure --with-plugins=...])
         ;;
      all)
         PLUGIN_APM="yes"
       	 PLUGIN_ASTERISK="yes"
	 PLUGIN_BUTTON_EXEC="yes"
         PLUGIN_CPUINFO="yes"
         PLUGIN_DBUS="yes"
         PLUGIN_DISKSTATS="yes"
         PLUGIN_DVB="yes"
         PLUGIN_EXEC="yes"
         PLUGIN_EVENT="yes"
         PLUGIN_FIFO="yes"
         PLUGIN_FILE="yes"
         PLUGIN_GPS="yes"
         PLUGIN_HDDTEMP="yes"
         PLUGIN_HUAWEI="yes"
         PLUGIN_I2C_SENSORS="yes"
         PLUGIN_ICONV="yes"
         PLUGIN_IMON="yes"
         PLUGIN_ISDN="yes"
         PLUGIN_KVV="yes"
         PLUGIN_LOADAVG="yes"
         PLUGIN_MEMINFO="yes"
         PLUGIN_MPD="yes"
	 PLUGIN_MPRIS_DBUS="yes"
         PLUGIN_MYSQL="yes"
         PLUGIN_NETDEV="yes"
         PLUGIN_NETINFO="yes"
         PLUGIN_POP3="yes"
         PLUGIN_PPP="yes"
         PLUGIN_PROC_STAT="yes"
         PLUGIN_PYTHON=$with_python
         PLUGIN_QNAPLOG="yes"
         PLUGIN_RASPI="yes"
         PLUGIN_SAMPLE="yes"
         PLUGIN_SETI="yes"
         PLUGIN_STATFS="yes"
         PLUGIN_UNAME="yes"
         PLUGIN_UPTIME="yes"
         PLUGIN_W1RETAP="yes"    	 
         PLUGIN_WIRELESS="yes"
         PLUGIN_XMMS="yes"   
         ;;
      none)
         PLUGIN_APM="no"
       	 PLUGIN_ASTERISK="no"
	 PLUGIN_BUTTON_EXEC="no"
         PLUGIN_CPUINFO="no"
         PLUGIN_DBUS="no"
         PLUGIN_DISKSTATS="no"
         PLUGIN_DVB="no"
         PLUGIN_EXEC="no"
         PLUGIN_EVENT="no"
         PLUGIN_FIFO="no"
         PLUGIN_FILE="no"
         PLUGIN_GPS="no"
         PLUGIN_HDDTEMP="no"
         PLUGIN_HUAWEI="no"
         PLUGIN_I2C_SENSORS="no"
         PLUGIN_ICONV="no"
         PLUGIN_IMON="no"
         PLUGIN_ISDN="no"
         PLUGIN_KVV="no"
         PLUGIN_LOADAVG="no"
         PLUGIN_MEMINFO="no"
         PLUGIN_MPD="no"
	 PLUGIN_MPRIS_DBUS="no"
         PLUGIN_MYSQL="no"
         PLUGIN_NETDEV="no"
         PLUGIN_NETINFO="no"
         PLUGIN_POP3="no"
         PLUGIN_PPP="no"
         PLUGIN_PROC_STAT="no"
         PLUGIN_PYTHON="no"
         PLUGIN_QNAPLOG="no"
         PLUGIN_RASPI="no"
         PLUGIN_SAMPLE="no"
         PLUGIN_SETI="no"
         PLUGIN_STATFS="no"
         PLUGIN_UNAME="no"
         PLUGIN_UPTIME="no"
         PLUGIN_W1RETAP="no"    	 
         PLUGIN_WIRELESS="no"
         PLUGIN_XMMS="no"   
         ;;
      apm)
         PLUGIN_APM=$val
         ;;
      button_exec)
	 PLUGIN_BUTTON_EXEC=$val
	 ;;
      asterisk)
         PLUGIN_ASTERISK=$val
         ;;
      cpuinfo)
         PLUGIN_CPUINFO=$val
         ;;
      dbus)
         PLUGIN_DBUS=$val
         ;;
      diskstats)
         PLUGIN_DISKSTATS=$val
         ;;
      dvb)
         PLUGIN_DVB=$val
         ;;
      exec)
         PLUGIN_EXEC=$val
         ;;
      event)
         PLUGIN_EVENT=$val
         ;;
      fifo)
         PLUGIN_FIFO=$val
         ;;
      file)
         PLUGIN_FILE=$val
         ;;
      gps)
         PLUGIN_GPS=$val
         ;;
      hddtemp)
         PLUGIN_HDDTEMP=$val
         ;;
      huawei)
         PLUGIN_HUAWEI=$val
         ;;
      i2c_sensors)
         PLUGIN_I2C_SENSORS=$val
	 ;;
      iconv)
      	PLUGIN_ICONV=$val
      	;;
      imon)
         PLUGIN_IMON=$val
         ;;
      isdn)
         PLUGIN_ISDN=$val
         ;;
      kvv)
         PLUGIN_KVV=$val
         ;;
      loadavg)
         PLUGIN_LOADAVG=$val
         ;;
      meminfo)
         PLUGIN_MEMINFO=$val
         ;;
      mpd)
         PLUGIN_MPD=$val
	 ;;
      mpris_dbus)
         PLUGIN_MPRIS_DBUS=$val
         ;;	 
      mysql)
         PLUGIN_MYSQL=$val
         ;;
      netdev)
         PLUGIN_NETDEV=$val
         ;;
      netinfo)
         PLUGIN_NETINFO=$val
         ;;
      pop3)
         PLUGIN_POP3=$val
         ;;
      ppp)
         PLUGIN_PPP=$val
         ;;
      proc_stat)
         PLUGIN_PROC_STAT=$val
         ;;
      python)
         PLUGIN_PYTHON=$val
         ;;
      qnaplog)
         PLUGIN_QNAPLOG=$val
         ;;
      raspi)
         PLUGIN_RASPI=$val
         ;;
      sample)
         PLUGIN_SAMPLE=$val
         ;;
      seti)
         PLUGIN_SETI=$val
         ;;
      statfs)
         PLUGIN_STATFS=$val
         ;;
      uname)
         PLUGIN_UNAME=$val
         ;;
      uptime)
         PLUGIN_UPTIME=$val
         ;;
      w1retap)
         PLUGIN_W1RETAP=$val
         ;;
      wireless)
         PLUGIN_WIRELESS=$val
         ;;
      xmms)
         PLUGIN_XMMS=$val
         ;;         
      *) 	
         AC_MSG_ERROR([Unknown plugin '$plugin'])
         ;;
   esac
done

AC_MSG_RESULT([done])

# Advanced Power Management
if test "$PLUGIN_APM" = "yes"; then
   AC_CHECK_HEADERS(asm/types.h, [has_asm_types="true"], [has_asm_types="false"])
   if test "$has_asm_types" = "true"; then
      PLUGINS="$PLUGINS plugin_apm.o"
      AC_DEFINE(PLUGIN_APM,1,[apm plugin])
   else
      AC_MSG_WARN(asm/types.h header not found: apm plugin disabled)
   fi
fi

if test "$PLUGIN_BUTTON_EXEC" = "yes"; then
   PLUGINS="$PLUGINS plugin_button_exec.o"
   AC_DEFINE(PLUGIN_BUTTON_EXEC,1,[button_exec plugin])
fi

if test "$PLUGIN_ASTERISK" = "yes"; then
   PLUGINS="$PLUGINS plugin_asterisk.o"
   AC_DEFINE(PLUGIN_ASTERISK,1,[asterisk plugin])
fi

# /proc/cpuinfo
if test "$PLUGIN_CPUINFO" = "yes"; then
   PLUGINS="$PLUGINS plugin_cpuinfo.o"
   AC_DEFINE(PLUGIN_CPUINFO,1,[cpuinfo plugin])
fi

#DBus
if test "$PLUGIN_DBUS" = "yes"; then
   PKG_CHECK_MODULES(DBUS, dbus-1 >= 1.0.0, HAVE_DBUS="yes", HAVE_DBUS="no")
   if test "x$HAVE_DBUS" == "xyes"; then
      PLUGINS="$PLUGINS plugin_dbus.o"
      PLUGINLIBS="$PLUGINLIBS $DBUS_LIBS"
      CPPFLAGS="$CPPFLAGS $DBUS_CFLAGS"
      AC_DEFINE(PLUGIN_DBUS,1,[dbus plugin])

      DBUS_VERSION=$($PKG_CONFIG --modversion dbus-1)
      DBUS_VERSION_MAJOR=$(echo $DBUS_VERSION | cut -d . -f 1)
      DBUS_VERSION_MINOR=$(echo $DBUS_VERSION | cut -d . -f 2)
      DBUS_VERSION_MICRO=$(echo $DBUS_VERSION | cut -d . -f 3)

      AC_DEFINE_UNQUOTED(DBUS_VERSION_MAJOR, $DBUS_VERSION_MAJOR, [DBus Major Version])
      AC_DEFINE_UNQUOTED(DBUS_VERSION_MINOR, $DBUS_VERSION_MINOR, [DBus Minor Version])
      AC_DEFINE_UNQUOTED(DBUS_VERSION_MICRO, $DBUS_VERSION_MICRO, [DBus Micro Version])
   else
      AC_MSG_WARN(dbus-1 not found check that PKG_CONFIG_PATH is set correctly: dbus plugin disabled)
   fi
fi

# /proc/diskstat
if test "$PLUGIN_DISKSTATS" = "yes"; then
   PLUGINS="$PLUGINS plugin_diskstats.o"
   AC_DEFINE(PLUGIN_DISKSTATS,1,[diskstats plugin])
fi

# Digital Video Broadcasting
if test "$PLUGIN_DVB" = "yes"; then
   AC_CHECK_HEADERS(asm/types.h, [has_asm_types="true"], [has_asm_types="false"])
   if test "$has_asm_types" = "true"; then
       AC_CHECK_HEADERS(linux/dvb/frontend.h, [has_dvb_header="true"], [has_dvb_header="false"])
       if test "$has_dvb_header" = "true"; then
          PLUGINS="$PLUGINS plugin_dvb.o"
          AC_DEFINE(PLUGIN_DVB,1,[dvb plugin])
       else
          PLUGINS="$PLUGINS plugin_dvb.o"
          AC_MSG_WARN(linux/dvb/frontend.h header not found: using ioctl)
       fi
   else
       AC_MSG_WARN(asm/types.h header not found: dvb plugin disabled)
   fi
fi

# start external commands (exec)
if test "$PLUGIN_EXEC" = "yes"; then
   PLUGINS="$PLUGINS plugin_exec.o"
   AC_DEFINE(PLUGIN_EXEC,1,[exec plugin])
fi

# event
if test "$PLUGIN_EVENT" = "yes"; then
   PLUGINS="$PLUGINS plugin_event.o"
   AC_DEFINE(PLUGIN_EVENT,1,[event plugin])
fi

# file
if test "$PLUGIN_FILE" = "yes"; then
   PLUGINS="$PLUGINS plugin_file.o"
   AC_DEFINE(PLUGIN_FILE,1,[file plugin])
fi

# FIFO
if test "$PLUGIN_FIFO" = "yes"; then
   PLUGINS="$PLUGINS plugin_fifo.o"
   AC_DEFINE(PLUGIN_FIFO,1,[fifo plugin])
fi

# GPS
if test "$PLUGIN_GPS" = "yes"; then
   AC_CHECK_HEADERS(nmeap.h, [has_nmeap_header="true"], [has_nmeap_header="false"])
   if test "$has_nmeap_header" = "true"; then	
      AC_CHECK_LIB(nmeap, nmeap_init, [has_libnmeap_lib="true"], [has_libnmeap_lib="false"])
      if test "$has_libnmeap_lib" = "true"; then
        PLUGINS="$PLUGINS plugin_gps.o"
        PLUGINLIBS="$PLUGINLIBS -lnmeap"
        AC_DEFINE(PLUGIN_GPS,1,[gps plugin])
      else
        AC_MSG_WARN(libnmeap lib not found: gps plugin disabled)
      fi
   else
      AC_MSG_WARN(nmeap.h header not found: gps plugin disabled)
   fi 
fi

# hddtemp
if test "$PLUGIN_HDDTEMP" = "yes"; then
   PLUGINS="$PLUGINS plugin_hddtemp.o"
   AC_DEFINE(PLUGIN_HDDTEMP,1,[hddtemp plugin])
fi

# Huawei
if test "$PLUGIN_HUAWEI" = "yes"; then
   PLUGINS="$PLUGINS plugin_huawei.o"
   AC_DEFINE(PLUGIN_HUAWEI,1,[huawei plugin])
fi

# I2C
if test "$PLUGIN_I2C_SENSORS" = "yes"; then
   PLUGINS="$PLUGINS plugin_i2c_sensors.o"
   AC_DEFINE(PLUGIN_I2C_SENSORS,1,[i2c sensors plugin])
fi

# IConv
if test "$PLUGIN_ICONV" = "yes"; then
   AM_ICONV
   if test "$am_cv_func_iconv" = "yes"; then 
      PLUGINS="$PLUGINS plugin_iconv.o"
      PLUGINLIBS="$PLUGINLIBS $LIBICONV"
      AC_DEFINE(PLUGIN_ICONV,1,[iconv charset converter plugin])
   else
      AC_MSG_WARN(iconv not found: iconv plugin disabled)
   fi
fi

# ISDN monitor
if test "$PLUGIN_IMON" = "yes"; then
   AC_CHECK_HEADERS(linux/errno.h, [has_linux_errno="true"], [has_linux_errno="false"])
   if test "$has_linux_errno" = "true"; then
       PLUGINS="$PLUGINS plugin_imon.o"
       AC_DEFINE(PLUGIN_IMON,1,[imon plugin])
   else
       AC_MSG_WARN(linux/errno.h header not found: imon plugin disabled)
   fi
fi

# ISDN
if test "$PLUGIN_ISDN" = "yes"; then
   AC_CHECK_HEADERS(linux/isdn.h, [has_isdn_header="true"], [has_isdn_header="false"])
   if test "$has_dvb_header" = "false"; then
      AC_MSG_WARN(linux/isdn.h header not found: isdn plugin CPS disabled)
   fi   
   PLUGINS="$PLUGINS plugin_isdn.o"
   AC_DEFINE(PLUGIN_ISDN,1,[ISDN plugin])
fi

# Karlsruher Verkehrsverbund
if test "$PLUGIN_KVV" = "yes"; then
   PLUGINS="$PLUGINS plugin_kvv.o"
   AC_DEFINE(PLUGIN_KVV,1,[kvv plugin])
fi

# load average
if test "$PLUGIN_LOADAVG" = "yes"; then
   PLUGINS="$PLUGINS plugin_loadavg.o"
   AC_DEFINE(PLUGIN_LOADAVG,1,[loadavg plugin])
fi

# meminfo
if test "$PLUGIN_MEMINFO" = "yes"; then
   PLUGINS="$PLUGINS plugin_meminfo.o"
   AC_DEFINE(PLUGIN_MEMINFO,1,[meminfo plugin])
fi

# MPD
if test "$PLUGIN_MPD" = "yes"; then
   AC_CHECK_LIB(mpdclient, [mpd_connection_new], [has_mpd_header="true"], [has_mpd_header="false"])
   if test "$has_mpd_header" = "true"; then
      PLUGINS="$PLUGINS plugin_mpd.o"
      PLUGINLIBS="$PLUGINLIBS `pkg-config libmpdclient --libs`"
      CPPFLAGS="$CPPFLAGS `pkg-config libmpdclient --cflags`"
      AC_DEFINE(PLUGIN_MPD,1,[mpd plugin])      
   else
      AC_MSG_WARN(libmpdclient.h header not found: mpd plugin disabled)
      AC_MSG_WARN(get libmpdclient.h from http://www.musicpd.org/libmpdclient.shtml)
      AC_MSG_WARN(and copy those 2 files in the lcd4linux directory.)
   fi
fi

# MPRIS D-Bus
if test "$PLUGIN_MPRIS_DBUS" = "yes"; then
   AC_CHECK_HEADERS(dbus/dbus.h, [has_dbus_header="true"], [has_dbus_header="false"])
   if test "$has_dbus_header" = "true"; then
      AC_CHECK_LIB(dbus-1, dbus_bus_get, [has_libdbus1_lib="true"], [has_libdbus1_lib="false"])
      if test "$has_libdbus1_lib" = "true"; then
	  PLUGINS="$PLUGINS plugin_mpris_dbus.o"
	  PLUGINLIBS="$PLUGINLIBS -ldbus-1"
	  AC_DEFINE(PLUGIN_MPRIS_DBUS,1,[mpris_dbus plugin])
      else
	  AC_MSG_WARN(libdbus-1 lib not found: mpris_dbus plugin disabled)
      fi
   else
      AC_MSG_WARN(dbus/dbus.h header not found: mpris_dbus plugin disabled)
   fi
fi


# MySQL
if test "$PLUGIN_MYSQL" = "yes"; then
   AC_CHECK_HEADERS(mysql/mysql.h, [has_mysql_header="true"], [has_mysql_header="false"])
   if test "$has_mysql_header" = "true"; then	
      AC_CHECK_LIB(mysqlclient, mysql_init, [has_mysql_lib="true"], [has_mysql_lib="false"])
      if test "$has_mysql_lib" = "true"; then
        PLUGINS="$PLUGINS plugin_mysql.o"
        PLUGINLIBS="$PLUGINLIBS -lmysqlclient"
        AC_DEFINE(PLUGIN_MYSQL,1,[mysql plugin])
      else
        AC_MSG_WARN(mysqlclient lib not found: mysql plugin disabled)
      fi
   else
      AC_MSG_WARN(mysql/mysql.h header not found: mysql plugin disabled)
   fi 
fi

# /proc/net/dev
if test "$PLUGIN_NETDEV" = "yes"; then
   PLUGINS="$PLUGINS plugin_netdev.o"
   AC_DEFINE(PLUGIN_NETDEV,1,[netdev plugin])
fi

# configuration of network devices
if test "$PLUGIN_NETINFO" = "yes"; then
   PLUGINS="$PLUGINS plugin_netinfo.o"
   AC_DEFINE(PLUGIN_NETINFO,1,[netinfo plugin])
fi

# POP3
if test "$PLUGIN_POP3" = "yes"; then
   PLUGINS="$PLUGINS plugin_pop3.o"
   AC_DEFINE(PLUGIN_POP3,1,[POP3 plugin])
fi

# PPP
if test "$PLUGIN_PPP" = "yes"; then
   AC_CHECK_HEADERS(net/if_ppp.h, [has_ppp_header="true"], [has_ppp_header="false"])
   if test "$has_ppp_header" = "true"; then
      PLUGINS="$PLUGINS plugin_ppp.o"
      AC_DEFINE(PLUGIN_PPP,1,[ppp plugin])
   else
      AC_MSG_WARN(net/if_ppp.h header not found: ppp plugin disabled)
   fi 
fi

# /proc/stat
if test "$PLUGIN_PROC_STAT" = "yes"; then
   PLUGINS="$PLUGINS plugin_proc_stat.o"
   AC_DEFINE(PLUGIN_PROC_STAT,1,[proc_stat plugin])
fi

# python
if test "$PLUGIN_PYTHON" = "yes"; then
   if test "$with_python" != "yes"; then
      AC_MSG_WARN(python support not enabled: python plugin disabled (use --with-python to enable))
   else
      if test -z "$python_path"; then
         AC_MSG_WARN(python headers not found: python plugin disabled)
      else
         PLUGINS="$PLUGINS plugin_python.o"
         CPPFLAGS="$CPPFLAGS $PYTHON_CPPFLAGS"
         PLUGINLIBS="$PLUGINLIBS $PYTHON_LDFLAGS $PYTHON_EXTRA_LIBS"
         AC_DEFINE(PLUGIN_PYTHON,1,[python plugin])
      fi 
   fi 
fi

# Qnaplog
if test "$PLUGIN_QNAPLOG" = "yes"; then
   AC_CHECK_HEADERS(sqlite3.h, [has_sqlite3_header="true"], [has_sqlite3_header="false"])
   if test "$has_sqlite3_header" = "true"; then	
      AC_CHECK_LIB(sqlite3, sqlite3_initialize, [has_sqlite3_lib="true"], [has_sqlite3_lib="false"])
      if test "$has_sqlite3_lib" = "true"; then
        PLUGINS="$PLUGINS plugin_qnaplog.o"
        PLUGINLIBS="$PLUGINLIBS -lsqlite3"
        AC_DEFINE(PLUGIN_QNAPLOG,1,[qnaplog plugin])
      else
        AC_MSG_WARN(sqlite3 lib not found: qnaplog plugin disabled)
      fi
   else
      AC_MSG_WARN(sqlite3.h header not found: qnaplog plugin disabled)
   fi 
fi

# raspi (Raspberry PI)
if test "$PLUGIN_RASPI" = "yes"; then
   PLUGINS="$PLUGINS plugin_raspi.o"
   AC_DEFINE(PLUGIN_RASPI,1,[raspi plugin])
fi

# sample
if test "$PLUGIN_SAMPLE" = "yes"; then
   PLUGINS="$PLUGINS plugin_sample.o"
   AC_DEFINE(PLUGIN_SAMPLE,1,[sample plugin])
fi

# SETI
if test "$PLUGIN_SETI" = "yes"; then
   PLUGINS="$PLUGINS plugin_seti.o"
   AC_DEFINE(PLUGIN_SETI,1,[seti plugin])
fi

# statfs()
if test "$PLUGIN_STATFS" = "yes"; then
   AC_CHECK_HEADERS(sys/vfs.h, [has_vfs_header="true"], [has_vfs_header="false"])
   if test "$has_vfs_header" = "true"; then
       PLUGINS="$PLUGINS plugin_statfs.o"
       AC_DEFINE(PLUGIN_STATFS,1,[statfs plugin])
   else
      AC_MSG_WARN(sys/vfs.h header not found: statfs plugin disabled)
   fi 
fi

# uname
if test "$PLUGIN_UNAME" = "yes"; then
   PLUGINS="$PLUGINS plugin_uname.o"
   AC_DEFINE(PLUGIN_UNAME,1,[uname plugin])
fi

# uptime
if test "$PLUGIN_UPTIME" = "yes"; then
   PLUGINS="$PLUGINS plugin_uptime.o"
   AC_DEFINE(PLUGIN_UPTIME,1,[uptime plugin])
fi

if test "$PLUGIN_W1RETAP" = "yes"; then
   PLUGINS="$PLUGINS plugin_w1retap.o"
   AC_DEFINE(PLUGIN_W1RETAP,1,[w1retap plugin])
fi

# WLAN
if test "$PLUGIN_WIRELESS" = "yes"; then
   AC_CHECK_HEADERS(linux/wireless.h, [has_wireless_header="true"], [has_wireless_header="false"], [#include <sys/socket.h>])
   if test "$has_wireless_header" = "true"; then
       PLUGINS="$PLUGINS plugin_wireless.o"
       AC_DEFINE(PLUGIN_WIRELESS,1,[wireless plugin])
   else
      AC_MSG_WARN(linux/wireless.h header not found: wireless plugin disabled)
   fi 
fi

# XMMS
if test "$PLUGIN_XMMS" = "yes"; then
   PLUGINS="$PLUGINS plugin_xmms.o"
   AC_DEFINE(PLUGIN_XMMS,1,[xmms plugin])
fi

AC_SUBST(PLUGINS)
AC_SUBST(PLUGINLIBS)
