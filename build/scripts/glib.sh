#!/bin/bash
set -e -u
ARCHIVE=glib-2.22.0.tar.bz2
ARCHIVEDIR=glib-2.22.0
. $KOBO_SCRIPT_DIR/build-common.sh

pushd $ARCHIVEDIR
	( cat <<EOF
glib_cv_stack_grows=yes
ac_cv_func_posix_getpwuid_r=yes
ac_cv_func_posix_getgrgid_r=yes
glib_cv_uscore=yes
EOF
	) > glib.config.cache

	CFLAGS="${CFLAGS}" LDFLAGS="${LDFLAGS}" LIBS="-liconv" ./configure --prefix=/${DEVICEROOT} --host=${CROSSTARGET} --disable-man --disable-gtk-doc --disable-silent-rules --cache=glib.config.cache --disable-static --with-libiconv=gnu
	$MAKE -j$MAKE_JOBS
	$MAKE install

	cp glibconfig.h /${DEVICEROOT}/include
popd

markbuilt
