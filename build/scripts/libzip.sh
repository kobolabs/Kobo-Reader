#!/bin/bash
set -e -u
ARCHIVE=libzip-0.9.3.tar.gz
ARCHIVEDIR=libzip-0.9.3
. $KOBO_SCRIPT_DIR/build-common.sh

pushd $ARCHIVEDIR
	CPPFLAGS="${CPPFLAGS}" LDFLAGS="${LDFLAGS}" LIBS="-lz" ./configure --prefix=/usr --with-zlib=/${DEVICEROOT}/lib --host=${CROSSTARGET}
	$MAKE -j$MAKE_JOBS
	$MAKE DESTDIR=/${DEVICEROOT} install
popd
markbuilt
