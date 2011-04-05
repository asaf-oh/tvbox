#!/bin/sh

if [ $# -lt 1 ]; then
    echo "usage : $0 operation"
    echo "e.g   : $0 cp -v"
fi

OP=$*

for dir in `find tagfiles -mindepth 1 -maxdepth 1 -type d`; do
    cat=`basename $dir`
    $OP $dir/tagfile.tvn /opt/slack/13.1/slackware/$cat/tagfile
done