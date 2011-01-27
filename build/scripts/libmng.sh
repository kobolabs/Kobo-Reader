#!/bin/bash
set -e -u
ARCHIVE=libmng-1.0.10.tar.gz
ARCHIVEDIR=libmng-1.0.10
. $KOBO_SCRIPT_DIR/build-common.sh

cp $ARCHIVEDIR/unmaintained/autogen.sh $ARCHIVEDIR
patch -d $ARCHIVEDIR -p0 < $PATCHESDIR/libmng-autogen.patch
pushd $ARCHIVEDIR
	./autogen.sh
	CPPFLAGS="${CPPFLAGS}" LDFLAGS="${LDFLAGS}" ./configure --prefix=/ --host=${CROSSTARGET}
	$MAKE -j$MAKE_JOBS
	$MAKE DESTDIR=/${DEVICEROOT} install
popd
markbuilt
