#!/bin/sh

tar zxf ../packages/jpegsrc.v6b.tar.gz
pushd jpeg-6b
	./configure --host=arm-linux --enable-shared --prefix=/chroot
	cat Makefile | sed -e s/^CC=.*$/CC=arm-linux-gcc/g > Makefile.new
	mv Makefile.new Makefile
	make
	make install
popd
