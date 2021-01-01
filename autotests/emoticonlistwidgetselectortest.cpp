/*
  SPDX-FileCopyrightText: 2019-2021 Laurent Montel <montel@kde.org>

  SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "emoticonlistwidgetselectortest.h"
#include "emoticon/emoticonlistwidgetselector.h"
#include <QTest>
QTEST_MAIN(EmoticonListWidgetSelectorTest)

EmoticonListWidgetSelectorTest::EmoticonListWidgetSelectorTest(QObject *parent)
    : QObject(parent)
{
}

void EmoticonListWidgetSelectorTest::shouldHaveDefaultValues()
{
    KPIMTextEdit::EmoticonListWidgetSelector w;
    QCOMPARE(w.viewMode(), QListView::IconMode);
    QCOMPARE(w.selectionMode(), QAbstractItemView::SingleSelection);
    QVERIFY(!w.dragEnabled());
}
