#!/bin/bash
set -e -u
ARCHIVE=udev-142.tar.gz
ARCHIVEDIR=udev-142
. $KOBO_SCRIPT_DIR/build-common.sh

pushd $ARCHIVEDIR
	LIBTOOL="${LIBTOOL}" CFLAGS="${CFLAGS}" LDFLAGS="${LDFLAGS} -lattr" ./configure --host=${CROSSTARGET} --prefix= --disable-introspection --with-pci-ids-path=../pci.ids
	$MAKE -j$MAKE_JOBS
	$MAKE DESTDIR=/${DEVICEROOT} install
popd
markbuilt
