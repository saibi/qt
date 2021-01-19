#!/bin/bash 

rm -rf nex_gui

if [ "$1" == "hab8000x" ]; then
	(time git clone git@gitlab.com:edger/nex_gui) &> /tmp/cc.txt
	time ./b_hab8000x_gui.sh  >> /dev/null
	echo "hab8000x gui" 
	cat /tmp/cc.txt
elif [ "$1" == "hab910" ]; then
	(time git clone git@gitlab.com:edger/nex_gui) &> /tmp/cc.txt
	time ./b_hab910_gui.sh  >> /dev/null
	echo "hab910 gui" 
	cat /tmp/cc.txt
elif [ "$1" == "hpe910" ]; then
	(time git clone git@gitlab.com:edger/nex_gui) &> /tmp/cc.txt
	time ./b_hpe910_gui.sh  >> /dev/null
	echo "hpe910 gui" 
	cat /tmp/cc.txt
elif [ "$1" == "hpe410" ]; then
	(time git clone git@gitlab.com:edger/nex_gui) &> /tmp/cc.txt 
	time ./b_hpe410_gui.sh  >> /dev/null
	echo "hpe410 gui" 
	cat /tmp/cc.txt
else 
	echo "usage $0 [hab8000x|hab910|hpe910|hpe410]"
	exit 1 
fi

rm -rf nex_gui
