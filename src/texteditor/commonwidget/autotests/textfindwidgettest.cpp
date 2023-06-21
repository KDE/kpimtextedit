/*
   SPDX-FileCopyrightText: 2022-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "textfindwidgettest.h"
#include "texteditor/commonwidget/textfindreplacewidget.h"
#include <QPushButton>
#include <QTest>
QTEST_MAIN(TextFindWidgetTest)
TextFindWidgetTest::TextFindWidgetTest(QObject *parent)
    : QObject{parent}
{
}

void TextFindWidgetTest::shouldHaveDefaultValues()
{
    KPIMTextEdit::TextFindWidget w;
    QVERIFY(w.searchLineEdit());
    QVERIFY(!w.isRegularExpression());
    QVERIFY(w.searchText().isEmpty());

    auto mFindNextBtn = w.findChild<QPushButton *>(QStringLiteral("mFindNextBtn"));
    QVERIFY(mFindNextBtn);
    QVERIFY(!mFindNextBtn->text().isEmpty());
    QVERIFY(!mFindNextBtn->toolTip().isEmpty());
    QVERIFY(!mFindNextBtn->isEnabled());

    auto mFindPrevBtn = w.findChild<QPushButton *>(QStringLiteral("mFindPrevBtn"));
    QVERIFY(mFindPrevBtn);
    QVERIFY(!mFindPrevBtn->text().isEmpty());
    QVERIFY(!mFindPrevBtn->toolTip().isEmpty());
    QVERIFY(!mFindPrevBtn->isEnabled());

    auto optionsBtn = w.findChild<QPushButton *>(QStringLiteral("optionsBtn"));
    QVERIFY(optionsBtn);
    QVERIFY(!optionsBtn->text().isEmpty());
    QVERIFY(!optionsBtn->toolTip().isEmpty());
    QVERIFY(optionsBtn->isEnabled());
}

#include "moc_textfindwidgettest.cpp"
