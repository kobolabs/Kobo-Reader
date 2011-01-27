#!/bin/bash
set -e -u
ARCHIVE=libiconv-1.13.1.tar.gz
ARCHIVEDIR=libiconv-1.13.1
. $KOBO_SCRIPT_DIR/build-common.sh

pushd $ARCHIVEDIR
	./configure --disable-rpath --prefix=/${DEVICEROOT} --with-libiconv-prefix=/ --host=${CROSSTARGET}
	$MAKE -j$MAKE_JOBS
	$MAKE install
popd
markbuilt
