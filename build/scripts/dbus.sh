#!/bin/bash
set -e -u
ARCHIVE=dbus-1.2.4.6permissive.tar.gz
ARCHIVEDIR=dbus-1.2.4.6permissive
. $KOBO_SCRIPT_DIR/build-common.sh

patch -p0 < $PATCHESDIR/dbus-1.2.4.6permissive_config-in.patch
pushd $ARCHIVEDIR
	autoconf
	CPPFLAGS="${CPPFLAGS}" LIBS="${LIBS}" ./configure --prefix=/ --host=${CROSSTARGET} --disable-abstract-sockets --without-x --disable-static
	$MAKE -j$MAKE_JOBS
	$MAKE DESTDIR=/${DEVICEROOT} install
popd
markbuilt
