###
## george's awesome ld_preload interposing /dev/fb0 ioctl dumper
##
## build:
##  CC=arm-linux-gcc make
##  CC=gcc make
##
## usage:
## LD_PRELOAD=/path/to/libioctldumper.so /bin/ebrmain
###

libioctldumper:
	$(CC) -g -fPIC -rdynamic -c ioctl-dumper.c
	$(CC) -g -shared -nostartfiles -o libioctldumper.so ioctl-dumper.o -ldl

clean:
	rm -rf *.o
	rm -rf *.so
