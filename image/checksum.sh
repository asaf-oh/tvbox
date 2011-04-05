#!/bin/sh


function usage
{
    echo "usage : $0 directory"
    exit 0
}

function fail
{
    echo "$1" >&2
    exit 10
}

[ $# -eq 1 ] || usage

cd $1 || fail "Could not enter directory : $1"

echo "MD5 message digest                Filename"

for file in `find . -type f`; do
    md5=`md5sum $file | cut -d' ' -f1`
    echo "$md5  $file"
done