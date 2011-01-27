#!/bin/bash
set -e -u

ARCHIVE=openssl-0.9.8l.tar.gz
ARCHIVEDIR=openssl-0.9.8l
. $KOBO_SCRIPT_DIR/build-common.sh

pushd ${ARCHIVEDIR}
	perl ./Configure linux-generic32 -DL_ENDIAN --install_prefix=/${DEVICEROOT} --openssldir=/usr --shared
	sed -i \
		-e s/^CC=.*$/CC=${CROSSTARGET}-gcc/g \
		-e s/^RANLIB=.*$/RANLIB=${CROSSTARGET}-ranlib/g \
		-e s/^AR=.*$/AR=${CROSSTARGET}-ar\ \$\(ARFLAGS\)\ r/g \
		Makefile
	# OpenSSL's Makefile is buggy; avoid parallel make
	$MAKE
	$MAKE install
popd
markbuilt
