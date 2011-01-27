#!/bin/bash
set -e -u
ARCHIVE=attr_2.4.43-1.tar.gz
ARCHIVEDIR=attr-2.4.43
. $KOBO_SCRIPT_DIR/build-common.sh

pushd $ARCHIVEDIR
	LIBTOOL="${LIBTOOL}" CC="${CC}" ./configure --host=${CROSSTARGET} --prefix=/${DEVICEROOT}
	$MAKE install
	$MAKE install-dev
	$MAKE install-lib
popd
markbuilt
