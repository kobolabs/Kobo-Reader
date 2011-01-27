#!/bin/bash
set -e -u

# Doesn't do a build, so doens't use the common build commands
. $KOBO_SCRIPT_DIR/build-config.sh

bzip2 -d $ARCHIVESDIR/pci.ids.bz2 -c > pci.ids
