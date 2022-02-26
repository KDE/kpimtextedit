/*
   SPDX-FileCopyrightText: 2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "plaintexteditfindbartest.h"
#include "texteditor/plaintexteditor/plaintexteditfindbar.h"
#include <QPlainTextEdit>
#include <QTest>

QTEST_MAIN(PlainTextEditFindBarTest)
PlainTextEditFindBarTest::PlainTextEditFindBarTest(QObject *parent)
    : QObject{parent}
{
}

void PlainTextEditFindBarTest::shouldHaveDefaultValues()
{
    QPlainTextEdit edit;
    KPIMTextEdit::PlainTextEditFindBar w(&edit);
    QVERIFY(w.text().isEmpty());
}
