#!/bin/bash
set -e -u
ARCHIVE=wpa_supplicant-0.7.1.tar.gz
ARCHIVEDIR=wpa_supplicant-0.7.1
. $KOBO_SCRIPT_DIR/build-common.sh

cp $ARCHIVEDIR/wpa_supplicant/defconfig $ARCHIVEDIR/wpa_supplicant/.config
patch -p0 < $PATCHESDIR/wpa_supplicant-logging.patch
pushd $ARCHIVEDIR/wpa_supplicant
	CC="${CC}" CFLAGS="${CFLAGS}" LDFLAGS="${LDFLAGS}" LIBS="${LDFLAGS}" $MAKE DESTDIR=/${DEVICEROOT} BINDIR=/bin install
popd
markbuilt
