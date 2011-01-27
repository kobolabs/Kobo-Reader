#!/bin/bash
set -e -u
ARCHIVE=gettext-0.17.tar.gz
ARCHIVEDIR=gettext-0.17
. $KOBO_SCRIPT_DIR/build-common.sh

patch -p0 < $PATCHESDIR/gettext-0.17_wchar-fix.patch
pushd $ARCHIVEDIR
## note gettext needs the trailing / in /${DEVICEROOT}/ !
	./configure --host=${CROSSTARGET} --prefix=/${DEVICEROOT}/ --disable-java --disable-native-java --disable-static
	$MAKE -j$MAKE_JOBS
	$MAKE install
popd
markbuilt
