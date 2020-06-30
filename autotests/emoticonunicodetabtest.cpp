/*
  SPDX-FileCopyrightText: 2019-2020 Laurent Montel <montel@kde.org>

  This library is free software; you can redistribute it and/or modify it
  under the terms of the GNU Library General Public License as published by
  the Free Software Foundation; either version 2 of the License, or (at your
  option) any later version.

  This library is distributed in the hope that it will be useful, but WITHOUT
  ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
  FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Library General Public
  License for more details.

  You should have received a copy of the GNU Library General Public License
  along with this library; see the file COPYING.LIB.  If not, write to the
  Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA
  02110-1301, USA.
*/

#include "emoticonunicodetabtest.h"
#include "emoticon/emoticonunicodetab.h"
#include <QTest>

QTEST_MAIN(EmoticonUnicodeTabTest)
EmoticonUnicodeTabTest::EmoticonUnicodeTabTest(QObject *parent)
    : QObject(parent)
{
}

void EmoticonUnicodeTabTest::shouldHaveDefaultValues()
{
    KPIMTextEdit::EmoticonUnicodeTab w;
    QVERIFY(w.count() > 0);
    QVERIFY(w.tabBarAutoHide());
}
