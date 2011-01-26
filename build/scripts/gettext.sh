#!/bin/sh

tar zxf ../packages/gettext-0.17.tar.gz
patch -p0 < ../patches/gettext-0.17_wchar-fix.patch
pushd gettext-0.17
## note gettext needs the trailing / in /chroot/ !
	./configure --host=arm-linux --prefix=/chroot/ --disable-java --disable-native-java --disable-static
	make && make install
popd
