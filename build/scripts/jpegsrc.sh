#!/bin/bash
set -e -u
ARCHIVE=jpegsrc.v6b.tar.gz
ARCHIVEDIR=jpeg-6b
. $KOBO_SCRIPT_DIR/build-common.sh

patch -p0 < $PATCHESDIR/jpeg6b-config-sub.patch
pushd $ARCHIVEDIR
	./configure --host=${CROSSTARGET} --enable-shared --prefix=/${DEVICEROOT}
	sed -i -e s/^CC=.*$/CC=${CC}/g Makefile
	$MAKE -j$MAKE_JOBS
	$MAKE install
popd
markbuilt
