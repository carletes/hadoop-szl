#!/bin/sh

top_srcdir="$(cd $(dirname $0) && pwd)"
m4_dir="$top_srcdir/m4"
test -d "$m4_dir" || mkdir "$m4_dir"

(cd "$top_srcdir" && autoreconf -ifv)
