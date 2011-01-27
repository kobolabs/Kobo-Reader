#!/bin/bash
set -e -u
ARCHIVE=dosfstools-3.0.6.tar.gz
ARCHIVEDIR=dosfstools-3.0.6
. $KOBO_SCRIPT_DIR/build-common.sh

pushd $ARCHIVEDIR
	CC=${CC} CFLAGS="-D__user= -DBLKSSZGET=_IO\(0x12,104\) ${CFLAGS}" $MAKE -j$MAKE_JOBS
	cp mkdosfs /${DEVICEROOT}/bin
	cp dosfsck /${DEVICEROOT}/bin
popd
markbuilt
