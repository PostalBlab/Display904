#!/usr/bin/env bash 

export PATH=$PATH:/home/postal/development/xdsl/cross-compiler-mips/bin/


if [ "${1}" == "cross" ];
then
	echo "mips-gcc -static display904.c   display904"
	mips-gcc -static display904.c -o display904
	if [ $? -eq 0 ];
	then
		echo "copy display904"
		cp display904 mount/
	fi
else 
	echo "gcc display904.c  touchpanel904.c -o display904 -DDEBUG"
	gcc display904.c  touchpanel904.c -o display904 -DDEBUG
fi
