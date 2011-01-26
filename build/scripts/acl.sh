#!/bin/sh

tar zxf ../packages/acl_2.2.47-1.tar.gz
pushd acl-2.2.47
	LIBTOOL=/chroot/usr/local/bin/arm-linux-libtool CFLAGS="-I/chroot/include" CPPFLAGS="-I/chroot/include" LDFLAGS="-L/chroot/lib" ./configure --host=arm-linux --prefix=/chroot
	# yes, this is insane
	make PCFLAGS="-I/chroot/include"
	make
	make install
	make install-dev
	make install-lib
popd
