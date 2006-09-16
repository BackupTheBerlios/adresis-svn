#!/bin/bash

# Script that update the .pro files && build the app
# Author: David Cuarado krawek [at] gmail [dot] com
# Version: 0.1.0

PARAMETERS=""

OPTION_NODEBUG=-1
ASSUME_YES=0

STAT_FILE=/tmp/app_stat_file-$RANDOM

. scripts/global


function fails ()
{
	echo 'END=-1' > $STAT_FILE

	echo 
	qperror $*
	echo "------ dmesg --------" >> $LOG_FILE
	echo `dmesg | tail -n 10` >> $LOG_FILE
	echo "---------------------" >> $LOG_FILE
	echo "Send the file $LOG_FILE to $EMAIL"

	return -1
}


function verifyEnv()
{
	if [ ! -x $QMAKE -a $QMAKE ]
	then
		qperror "You need \"qmake\" in your path!"
		exit -1
	fi
	
	if [ ! -x $MAKE -a $MAKE ]
	then
		qperror "You doesn't have installed \"make\" tool"
		exit -1
	fi
	
	if [ ! $QMAKE ]
	then
		if [ -x "$QTDIR/bin/qmake" ]
		then
			QMAKE="$QTDIR/bin/qmake"
		else
			qperror "You don't have qmake in your PATH, or doesn't have Qtlibs, please install it and try"
			exit 1
		fi
	fi
}


function qtCC
{
	PARAMETERS="$PARAMETERS "
}

function updateMakefiles()
{
#	qpinfo "Updating Makefiles with parameters ${PARAMETERS}"
	echo -n "Wait a moment."
	DIRS=`find . -type d`
	for i in $DIRS
	do
		cd $i
		if [ -e *.pro ]
		then
			echo -n "."

			$QMAKE ${PARAMETERS} 2> /dev/null >/dev/null
		fi
		cd - 2> /dev/null >/dev/null
	done
	echo
}

function buildExecutable()
{

SERVER_LAUNCHER='adresis_server'
CLIENT_LAUNCHER='adresis_client'

	echo '#!/bin/bash' > $APP_HOME/$SERVER_LAUNCHER 
	echo "export ${APPNAME}_HOME=$APP_HOME" >> $APP_HOME/$SERVER_LAUNCHER
	echo "export LD_LIBRARY_PATH=$APP_HOME/lib" >> $APP_HOME/$SERVER_LAUNCHER
	echo "$APP_HOME/bin/adserver.bin $* " >> $APP_HOME/$SERVER_LAUNCHER


	echo '#!/bin/bash' > $APP_HOME/$CLIENT_LAUNCHER
	echo "export ${APPNAME}_HOME=$APP_HOME" >> $APP_HOME/$CLIENT_LAUNCHER
	echo "export LD_LIBRARY_PATH=$APP_HOME/lib">> $APP_HOME/$CLIENT_LAUNCHER
	echo "$APP_HOME/bin/adclient.bin $* ">> $APP_HOME/$CLIENT_LAUNCHER


	chmod 755 $APP_HOME/$SERVER_LAUNCHER $APP_HOME/$CLIENT_LAUNCHER
}

function addMenuEntry()
{
	true
#	if [ `whoami` == "root" ]
#	then
		# add to /usr/share/applications
#	else
		# add to  $HOME/.local/share/applications/
#	fi
}

function do_install()
{
	# this is a hack
	updateMakefiles
	$MAKE install 2>> $LOG_FILE >/dev/null || fails "Error while install!. Please send the file $LOG_FILE to $EMAIL"
	
	buildExecutable
 	addMenuEntry
}

function main()
{
	echo
	echo `date` >> $LOG_FILE
	qpinfo "###################################"
	
	if [ $OPTION_NODEBUG -eq 1 ]
	then
		qpinfo "-> Using NO-DEBUG support"
		PARAMETERS="$PARAMETERS DEFINES+=KT_NODEBUG"
	else
		qpinfo "-> Using DEBUG support"
	fi
	
	qpinfo "###################################"
	echo
	
	if [ $ASSUME_YES -ne 1 ]
	then
		qpelec "This is right (y/n)? "
		read UOG
	
		case $UOG in
			n|N|no|NO) exit -1 ;;
		esac
	fi
	
	updateMakefiles
	$QMAKE
	
	if [ $ASSUME_YES -ne 1 ]
	then
		qpelec "Do you wants clean the project (y/n)? "
		read UOG
	
		case $UOG in
			y|yes|si|s|Y|S)
				qpinfo "Cleaning..."
				$MAKE clean > /dev/null
			;;
		esac
	else
		$MAKE clean > /dev/null
	fi
	
########################
	qpinfo "Compiling $APPNAME $APPVER..."
	qpinfo "Go for a coffee cup ;)"
	
	echo > $STAT_FILE
	END=0
	INIT_TIME=`date +%s`
	( ( $MAKE  >/dev/null 2>> $LOG_FILE || fails "Error while compile!" ) && echo END=1 > $STAT_FILE ) & 
	
	while [ $END -eq 0 ]
	do
		if [ -f $STAT_FILE ]
		then
 	       		. $STAT_FILE
 	       		echo -n .
 	       		sleep 2
 	       	else
 	       		wait
 	       		break;
 	       	fi
	done
	END_TIME=`date +%s`
	echo
	rm $STAT_FILE
	
	if [ $END -eq -1 ]
	then
		exit -1
	fi
	
	qpinfo "Compiling successful! Time: `expr $END_TIME - $INIT_TIME` seconds"
	
	if [ $ASSUME_YES -ne 1 ]
	then
		qpelec "Do you wants install $APPNAME in \033[0;41m\"$APP_HOME\"\033[0;0m (y/n)? "
		read SN
	
		case $SN in
			y|yes|si|s) do_install ;;
		esac
	else
		do_install
	fi
}

function usage()
{
	echo "Usage: `basename $0` [option]"
	echo
	echo "Options: "
	echo "	-p,--prefix [PREFIX]"
	echo "	   Set the prefix"
	echo "	-d,--debug"
	echo "	   Activate debug"
	echo "	-Y,--assume-yes"
	echo "	   Assume yes"
	echo "	-h,--help"
	echo "	   This message"
	echo
	exit 0
}

if [ $# -eq 0 ]
then
	if [ ! $APP_HOME ]
	then
		usage
	fi
fi


TEMP=`getopt -o p:dYh:: --long prefix:,help,debug::,assume-yes -n "$0" -- "$@"`
eval set -- "$TEMP"
while [ true ]
do
        case "$1" in
		-h|--help) usage; shift;;
		-p|--prefix) APP_HOME=$2; shift 2 ;;
		-d|--debug) OPTION_NODEBUG=0; shift ;;
		-Y|--assume-yes) ASSUME_YES=1; OPTION_NODEBUG=1; shift ;; 
		--) shift ; break ;;
		*) break;
	esac
done

verifyEnv
detectQtVersion

if [ $OPTION_NODEBUG -eq -1 ]
then
	qpelec "Do you wants debug support (y/n)? "
	read UOG
	
	case $UOG in
		n|N|no|NO) OPTION_NODEBUG=1 ;;
	esac
fi

export APP_HOME
export INSTALL_ROOT=$APP_HOME

main

