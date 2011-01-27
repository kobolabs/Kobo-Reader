#!/bin/bash
set -e -u
ARCHIVE=expat-2.0.1.tar.gz
ARCHIVEDIR=expat-2.0.1
. $KOBO_SCRIPT_DIR/build-common.sh

pushd $ARCHIVEDIR
	./configure --prefix=/ --host=${CROSSTARGET} --disable-static
	$MAKE -j$MAKE_JOBS
	$MAKE DESTDIR=/${DEVICEROOT} install
popd
markbuilt
