#!/bin/bash 

if [ "$1" == "hpe910" ]; then
	rm -rf 4gmotor 
	(time git clone git@gitlab.com:edger/4gmotor) &> /tmp/cc.txt
	time ./b_hpe910_motor.sh  >> /dev/null
	echo "hpe910 motor" 
	cat /tmp/cc.txt 
	rm -rf 4gmotor 
elif [ "$1" == "hpe410" ]; then
	rm -rf autocrat
	(time git clone git@gitlab.com:edger/autocrat) &> /tmp/cc.txt
	time ./b_hpe410_motor.sh  >> /dev/null
	echo "hpe410 motor" 
	cat /tmp/cc.txt 
	rm -rf autocrat
elif [ "$1" == "hab910" ]; then
	rm -rf hab910_motor
	(time git clone git@gitlab.com:edger/hab910_motor) &> /tmp/cc.txt
	time ./b_hab910_motor.sh  >> /dev/null
	echo "hab910 motor" 
	cat /tmp/cc.txt 
	rm -rf hab910_motor
else 
	echo "$0 [hpe910|hpe410|hab910]"
	exit 1
fi

