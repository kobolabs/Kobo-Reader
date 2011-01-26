#!/bin/sh

tar zxf ../packages/openssl-0.9.8l.tar.gz
pushd openssl-0.9.8l
	perl ./Configure linux-generic32 -DL_ENDIAN --install_prefix=/chroot --openssldir=/usr --shared
	cat Makefile | sed -e s/^CC=.*$/CC=arm-linux-gcc/g | sed -e s/^RANLIB=.*$/RANLIB=arm-linux-ranlib/g | sed -e s/^AR=.*$/AR=arm-linux-ar\ \$\(ARFLAGS\)\ r/g > Makefile.new
	mv Makefile.new Makefile
	make && make install
popd
