/*
  SPDX-FileCopyrightText: 2019-2022 Laurent Montel <montel@kde.org>

  SPDX-License-Identifier: LGPL-2.0-or-later
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
}
