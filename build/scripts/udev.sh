#!/bin/sh

tar zxf ../packages/udev-142.tar.gz
pushd udev-142
	LIBTOOL=/chroot/usr/local/bin/arm-linux-libtool CFLAGS="-I/chroot/include" LDFLAGS="-L/chroot/lib -lattr" ./configure --host=arm-linux --prefix= --disable-introspection --with-pci-ids-path=../pci.ids
	make
	make DESTDIR=/chroot install
popd
