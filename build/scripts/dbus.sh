#!/bin/sh

tar zxf ../packages/dbus-1.2.4.6permissive.tar.gz
patch -p0 < ../patches/dbus-1.2.4.6permissive_config-in.patch
pushd dbus-1.2.4.6permissive
	autoconf
	CPPFLAGS=-I/chroot/include LIBS=-L/chroot/lib ./configure --prefix=/ --host=arm-linux --disable-abstract-sockets --without-x --disable-static
	make && make DESTDIR=/chroot install
popd
