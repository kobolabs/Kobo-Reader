#!/bin/bash
set -e -u
ARCHIVE=libtool-2.2.8.tar.gz
ARCHIVEDIR=libtool-2.2.8
. $KOBO_SCRIPT_DIR/build-common.sh

pushd $ARCHIVEDIR
	./configure --host=${CROSSTARGET} --program-prefix=${CROSSTARGET}-
	$MAKE -j$MAKE_JOBS
	$MAKE DESTDIR=/${DEVICEROOT} install
popd
markbuilt
