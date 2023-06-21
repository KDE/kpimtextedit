/*
   SPDX-FileCopyrightText: 2022-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "textreplacewidgettest.h"
#include "texteditor/commonwidget/textfindreplacewidget.h"
#include <QLineEdit>
#include <QPushButton>
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
    auto mReplace = w.findChild<QLineEdit *>(QStringLiteral("mReplace"));
    QVERIFY(mReplace);
    QVERIFY(mReplace->isClearButtonEnabled());
    QVERIFY(mReplace->text().isEmpty());

    auto mReplaceBtn = w.findChild<QPushButton *>(QStringLiteral("mReplaceBtn"));
    QVERIFY(mReplaceBtn);
    QVERIFY(!mReplaceBtn->text().isEmpty());

    auto mReplaceAllBtn = w.findChild<QPushButton *>(QStringLiteral("mReplaceAllBtn"));
    QVERIFY(mReplaceAllBtn);
    QVERIFY(!mReplaceAllBtn->text().isEmpty());
}

#include "moc_textreplacewidgettest.cpp"
