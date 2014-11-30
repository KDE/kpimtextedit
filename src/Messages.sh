#! /bin/sh
$XGETTEXT `find . src -name "*.cpp" -o -name "*.h"` -o $podir/libkpimtextedit.pot
