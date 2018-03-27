#!/bin/sh

case `uname` in
    Darwin) libtoolize=glibtoolize ;;
    *)      libtoolize=libtoolize  ;;
esac

set -x
AUTOMAKE=${AUTOMAKE:-automake} ACLOCAL=${ACLOCAL:-aclocal}
export AUTOMAKE ACLOCAL
$libtoolize --force --automake --copy
${AUTORECONF:-autoreconf} -i
find . \( -name 'run*' -o -name '*.sh' \) -a -type f | xargs chmod +x
chmod +x scripts/*
