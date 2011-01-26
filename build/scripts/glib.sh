#!/bin/sh

tar jxf ../packages/glib-2.22.0.tar.bz2
pushd glib-2.22.0
	( cat <<EOF
glib_cv_stack_grows=yes
ac_cv_func_posix_getpwuid_r=yes
ac_cv_func_posix_getgrgid_r=yes
glib_cv_uscore=yes
EOF
	) > glib.config.cache

	CFLAGS=-I/chroot/include LDFLAGS="-L/chroot/lib" LIBS="-liconv -lgettextlib -lgettextpo -lgettextsrc" ./configure --prefix=/chroot --host=arm-linux --disable-man --disable-gtk-doc --disable-silent-rules --cache=glib.config.cache --disable-static --with-libiconv=gnu
	make && make install

	cp glibconfig.h /chroot/include
popd

