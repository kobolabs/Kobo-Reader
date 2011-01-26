#!/bin/sh

tar zxf ../packages/libxml2-2.7.6.tar.gz
pushd libxml2-2.7.6
	./configure --prefix=/usr --host=arm-linux --without-debug --without-docbook --without-fexceptions --without-history --without-mem-debug --without-python --without-coverage
	make
	make DESTDIR=/chroot install
popd
