#!/bin/sh
set -e
. ./headers.sh

rm -f compile_commands.json

for PROJECT in $PROJECTS; do
  echo "Building $PROJECT..."
  (cd $PROJECT && DESTDIR="$SYSROOT" bear --append --output ../compile_commands.json -- $MAKE install)
done

