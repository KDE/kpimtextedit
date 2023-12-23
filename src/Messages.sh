#! /bin/sh
# SPDX-License-Identifier: BSD-3-Clause
# SPDX-FileCopyrightText: none

$XGETTEXT `find . -name "*.cpp" -o -name "*.h" | grep -v '/tests/' | grep -v '/autotests/'` -o $podir/libkpimtextedit6.pot
