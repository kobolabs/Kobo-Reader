#!/bin/bash
set -e -u
ARCHIVE=libusb-0.1.12.tar.gz
ARCHIVEDIR=libusb-0.1.12
. $KOBO_SCRIPT_DIR/build-common.sh

pushd $ARCHIVEDIR
	./configure --host=${CROSSTARGET} --prefix=/
	$MAKE -j$MAKE_JOBS
	$MAKE DESTDIR=/${DEVICEROOT} install
popd
markbuilt
