#!/bin/bash

ln -s ../packages packages

pushd scripts

for i in \
	openssl \
	iconv \
	expat \
	dbus \
	gettext \
	glib \
	libusb \
	zlib \
	libzip \
	libxml2 \
	dosfstools \
	wireless_tools \
	wpa_supplicant \
	libpng \
	jpegsrc \
	busybox \
	libtool \
	attr \
	acl \
	pciids \
	udev \
	lcms \
	libmng \
; do
	./$i.sh

done

popd
