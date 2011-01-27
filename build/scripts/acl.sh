#!/bin/bash
set -e -u
ARCHIVE=acl_2.2.47-1.tar.gz
ARCHIVEDIR=acl-2.2.47
. $KOBO_SCRIPT_DIR/build-common.sh

pushd $ARCHIVEDIR
	LIBTOOL="${LIBTOOL}" CFLAGS="${CFLAGS}" CPPFLAGS="${CPPFLAGS}" LDFLAGS="${LDFLAGS}" ./configure --host=$CROSSTARGET --prefix=/${DEVICEROOT}
	$MAKE CPPFLAGS="${CPPFLAGS}" -j$MAKE_JOBS
	$MAKE install
	$MAKE install-dev
	$MAKE install-lib
popd
markbuilt
