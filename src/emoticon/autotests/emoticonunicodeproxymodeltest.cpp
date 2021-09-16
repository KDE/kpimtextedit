/*
  SPDX-FileCopyrightText: 2021 Laurent Montel <montel@kde.org>

  SPDX-License-Identifier: LGPL-2.0-or-later
*/
#include "emoticonunicodeproxymodeltest.h"
#include "emoticon/emoticonunicodeproxymodel.h"
#include <QTest>
QTEST_MAIN(EmoticonUnicodeProxyModelTest)
EmoticonUnicodeProxyModelTest::EmoticonUnicodeProxyModelTest(QObject *parent)
    : QObject(parent)
{
}

void EmoticonUnicodeProxyModelTest::shouldHaveDefaultValues()
{
    KPIMTextEdit::EmoticonUnicodeProxyModel m;
    QCOMPARE(m.categories(), KPIMTextEdit::EmoticonUnicodeUtils::EmoticonStruct::Unknown);
}
