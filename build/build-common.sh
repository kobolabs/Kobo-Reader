#!/bin/bash
. $KOBO_SCRIPT_DIR/build-config.sh

# Argument processing
while [ $# -gt 0 ]; do
	if [ "$1" = "clean" ]; then
		rm -rf "$ARCHIVEDIR"
		rm status/`basename $0 .sh`
		exit 0
	else
		echo "ERROR: unknown argument $1 to $0"
		exit 1
	fi
	shift 1
done

# Do we need to do the build at all?
# Doesn't consider dependencies; you still have to clean for that. Should be using `make'.
mkdir -p status
if [ -e status/`basename $0 .sh` ]; then
	echo "No need to rebuild `basename $0 .sh`"
	exit 0
fi

rm -rf "${ARCHIVEDIR}"
ARCHIVESUFFIX="${ARCHIVE##*.}"
if [ "$ARCHIVESUFFIX" = "bz2" ]; then
	tar xjf ${ARCHIVESDIR}/${ARCHIVE}
elif [ "$ARCHIVESUFFIX" = "gz" ]; then
	tar xzf ${ARCHIVESDIR}/${ARCHIVE}
else
	echo "Unknown archive suffix $ARCHIVESUFFIX on $ARCHIVE"
	exit 1
fi

function markbuilt() {
	touch status/`basename $0 .sh`
}
