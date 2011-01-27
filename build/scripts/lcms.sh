#!/bin/bash
set -e -u
ARCHIVE=lcms-1.18a.tar.gz
ARCHIVEDIR=lcms-1.18
. $KOBO_SCRIPT_DIR/build-common.sh

pushd $ARCHIVEDIR
	CPPFLAGS="-I/${DEVICEROOT}/include" LDFLAGS="-L/${DEVICEROOT}/lib" ./configure --prefix=/ --host=${CROSSTARGET}
	$MAKE -j$MAKE_JOBS
	$MAKE DESTDIR=/${DEVICEROOT} install
popd
markbuilt
