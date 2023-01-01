/*
  SPDX-FileCopyrightText: 2021-2023 Laurent Montel <montel@kde.org>

  SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "emoticontexteditselectortest.h"
#include "emoticon/emoticontexteditselector.h"
#include "emoticon/emoticonunicodetab.h"
#include <QLineEdit>
#include <QTest>
#include <QVBoxLayout>
QTEST_MAIN(EmoticonTextEditSelectorTest)

EmoticonTextEditSelectorTest::EmoticonTextEditSelectorTest(QObject *parent)
    : QObject(parent)
{
}

void EmoticonTextEditSelectorTest::shouldHaveDefaultValues()
{
    KPIMTextEdit::EmoticonTextEditSelector w;
    auto mainLayout = w.findChild<QVBoxLayout *>(QStringLiteral("mainLayout"));
    QVERIFY(mainLayout);
    QCOMPARE(mainLayout->contentsMargins(), QMargins{});

    auto mUnicodeTab = w.findChild<KPIMTextEdit::EmoticonUnicodeTab *>(QStringLiteral("mUnicodeTab"));
    QVERIFY(mUnicodeTab);

    auto mSearchUnicodeLineEdit = w.findChild<QLineEdit *>(QStringLiteral("mSearchUnicodeLineEdit"));
    QVERIFY(mSearchUnicodeLineEdit);
    QVERIFY(mSearchUnicodeLineEdit->isClearButtonEnabled());
    QVERIFY(mSearchUnicodeLineEdit->text().isEmpty());
    QVERIFY(!mSearchUnicodeLineEdit->placeholderText().isEmpty());
}
