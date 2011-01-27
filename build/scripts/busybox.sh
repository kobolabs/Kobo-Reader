#!/bin/bash
set -e -u
ARCHIVE=busybox-1.17.1.tar.bz2
ARCHIVEDIR=busybox-1.17.1
. $KOBO_SCRIPT_DIR/build-common.sh

patch -p0 < $PATCHESDIR/busybox-1.17.1.patch
patch -p0 < $PATCHESDIR/busybox-1.17.1-make.patch
pushd $ARCHIVEDIR
	$MAKE defconfig
	$MAKE -j$MAKE_JOBS CROSS_COMPILE="${CROSSTARGET}-" install
	cp -a _install/* /${DEVICEROOT}
popd
markbuilt
