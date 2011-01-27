#!/bin/bash
set -e -u
ARCHIVE=zlib-1.2.4.tar.bz2
ARCHIVEDIR=zlib-1.2.4
. $KOBO_SCRIPT_DIR/build-common.sh

pushd $ARCHIVEDIR
	LD=${CROSSTARGET}-ld CC=${CC} ./configure --prefix=/
	$MAKE -j$MAKE_JOBS
	$MAKE DESTDIR=/${DEVICEROOT} install
popd
markbuilt
