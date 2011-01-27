#!/bin/bash
set -e -u
ARCHIVE=wireless_tools.29.tar.gz
ARCHIVEDIR=wireless_tools.29
. $KOBO_SCRIPT_DIR/build-common.sh

pushd $ARCHIVEDIR
	sed -i \
		-e s/^CC\ =.*$/CC=${CROSSTARGET}-gcc/g \
		-e s/^RANLIB\ =.*$/RANLIB=${CROSSTARGET}-ranlib/g \
		-e s/^AR\ =.*$/AR=${CROSSTARGET}-ar/g Makefile
	$MAKE -j$MAKE_JOBS
	$MAKE PREFIX=/${DEVICEROOT} install
popd
markbuilt
