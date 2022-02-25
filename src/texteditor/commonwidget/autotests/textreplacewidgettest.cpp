/*
   SPDX-FileCopyrightText: 2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "textreplacewidgettest.h"
#include "texteditor/commonwidget/textfindreplacewidget.h"
#include <QTest>
QTEST_MAIN(TextReplaceWidgetTest)
TextReplaceWidgetTest::TextReplaceWidgetTest(QObject *parent)
    : QObject{parent}
{
}

void TextReplaceWidgetTest::shouldHaveDefaultValues()
{
    KPIMTextEdit::TextReplaceWidget w;
    QVERIFY(w.replaceLineEdit());
}
