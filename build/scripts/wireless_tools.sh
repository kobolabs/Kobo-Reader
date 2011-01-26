#!/bin/sh

tar zxf ../packages/wireless_tools.29.tar.gz
pushd wireless_tools.29
	cat Makefile | sed -e s/^CC\ =.*$/CC=arm-linux-gcc/g -e s/^RANLIB\ =.*$/RANLIB=arm-linux-ranlib/g -e s/^AR\ =.*$/AR=arm-linux-ar/g > Makefile.new
	mv Makefile.new Makefile
	make
	make PREFIX=/chroot install
popd
