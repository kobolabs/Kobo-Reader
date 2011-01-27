#!/bin/bash
set -e -u
ARCHIVE=libxml2-2.7.6.tar.gz
ARCHIVEDIR=libxml2-2.7.6
. $KOBO_SCRIPT_DIR/build-common.sh

pushd $ARCHIVEDIR
	./configure --prefix=/usr --host=${CROSSTARGET} --without-debug --without-docbook --without-fexceptions --without-history --without-mem-debug --without-python --without-coverage
	$MAKE -j$MAKE_JOBS
	$MAKE DESTDIR=/${DEVICEROOT} install
popd
markbuilt
