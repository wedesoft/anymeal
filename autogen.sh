#!/bin/sh
git log > ChangeLog
libtoolize
aclocal
automake -a --foreign
autoconf
