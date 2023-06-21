/*
   SPDX-FileCopyrightText: 2014-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "textgotolinewidgettest.h"
#include "texteditor/commonwidget/textgotolinewidget.h"
#include <QPushButton>
#include <QSignalSpy>
#include <QSpinBox>
#include <QTest>
#include <QToolButton>
#include <qtestkeyboard.h>
#include <qtestmouse.h>

TextGoToLineWidgetTest::TextGoToLineWidgetTest(QObject *parent)
    : QObject(parent)
{
}

void TextGoToLineWidgetTest::shouldHaveDefaultValuesOnCreation()
{
    KPIMTextEdit::TextGoToLineWidget edit;
    edit.show();
    QVERIFY(QTest::qWaitForWindowExposed(&edit));
    auto line = edit.findChild<QSpinBox *>(QStringLiteral("line"));
    QVERIFY(line);
    QCOMPARE(line->minimum(), 1);
    auto gotolinebutton = edit.findChild<QPushButton *>(QStringLiteral("gotoline"));
    QVERIFY(gotolinebutton);
    auto closebutton = edit.findChild<QToolButton *>(QStringLiteral("closebutton"));
    QVERIFY(closebutton);
    QVERIFY(line->hasFocus());
}

void TextGoToLineWidgetTest::shouldEmitGoToLineSignalWhenPressOnButton()
{
    KPIMTextEdit::TextGoToLineWidget edit;
    auto gotolinebutton = edit.findChild<QPushButton *>(QStringLiteral("gotoline"));
    QSignalSpy spy(&edit, &KPIMTextEdit::TextGoToLineWidget::moveToLine);
    QTest::mouseClick(gotolinebutton, Qt::LeftButton);
    QCOMPARE(spy.count(), 1);
}

void TextGoToLineWidgetTest::shouldEmitGoToLineSignalCorrectValueWhenPressOnButton()
{
    KPIMTextEdit::TextGoToLineWidget edit;
    auto gotolinebutton = edit.findChild<QPushButton *>(QStringLiteral("gotoline"));
    auto line = edit.findChild<QSpinBox *>(QStringLiteral("line"));
    line->setValue(5);
    QCOMPARE(line->value(), 5);
    QSignalSpy spy(&edit, &KPIMTextEdit::TextGoToLineWidget::moveToLine);
    QTest::mouseClick(gotolinebutton, Qt::LeftButton);
    QCOMPARE(spy.count(), 1);
    QCOMPARE(spy.at(0).at(0).toInt(), 5);
}

void TextGoToLineWidgetTest::shouldHideWidgetWhenClickOnCloseButton()
{
    KPIMTextEdit::TextGoToLineWidget edit;
    edit.show();
    QVERIFY(QTest::qWaitForWindowExposed(&edit));
    QVERIFY(edit.isVisible());
    auto closebutton = edit.findChild<QToolButton *>(QStringLiteral("closebutton"));
    QTest::mouseClick(closebutton, Qt::LeftButton);
    QVERIFY(!edit.isVisible());
}

void TextGoToLineWidgetTest::shouldHideWidgetWhenPressEscape()
{
    KPIMTextEdit::TextGoToLineWidget edit;
    edit.show();
    QVERIFY(QTest::qWaitForWindowExposed(&edit));
    QTest::keyPress(&edit, Qt::Key_Escape);
    QCOMPARE(edit.isVisible(), false);
}

void TextGoToLineWidgetTest::shouldEmitGoToLineSignalWhenSpinboxHasFocusAndWePressEnter()
{
    KPIMTextEdit::TextGoToLineWidget edit;
    edit.show();
    QVERIFY(QTest::qWaitForWindowExposed(&edit));
    auto line = edit.findChild<QSpinBox *>(QStringLiteral("line"));
    line->setFocus();
    QVERIFY(line->hasFocus());
    line->setValue(5);
    QSignalSpy spy(&edit, &KPIMTextEdit::TextGoToLineWidget::moveToLine);
    QTest::keyPress(line, Qt::Key_Enter);
    QCOMPARE(spy.count(), 1);
    QCOMPARE(spy.at(0).at(0).toInt(), 5);
}

void TextGoToLineWidgetTest::shouldHasFocusEachTimeThatItShown()
{
    KPIMTextEdit::TextGoToLineWidget edit;
    edit.show();
    QVERIFY(QTest::qWaitForWindowExposed(&edit));
    auto line = edit.findChild<QSpinBox *>(QStringLiteral("line"));
    QVERIFY(line);
    QVERIFY(line->hasFocus());
    edit.hide();
    QVERIFY(!line->hasFocus());
    edit.show();
    QVERIFY(QTest::qWaitForWindowExposed(&edit));
    // FIXME QVERIFY(line->hasFocus());
}

void TextGoToLineWidgetTest::shouldSetFocusWhenWeRecallGotToLine()
{
    KPIMTextEdit::TextGoToLineWidget edit;
    edit.show();
    QVERIFY(QTest::qWaitForWindowExposed(&edit));
    auto line = edit.findChild<QSpinBox *>(QStringLiteral("line"));
    QVERIFY(line->hasFocus());
    edit.setFocus();
    QVERIFY(!line->hasFocus());
    edit.goToLine();
    QVERIFY(line->hasFocus());
}

void TextGoToLineWidgetTest::shouldChangeMaximumValue()
{
    KPIMTextEdit::TextGoToLineWidget edit;
    edit.show();
    QVERIFY(QTest::qWaitForWindowExposed(&edit));
    auto line = edit.findChild<QSpinBox *>(QStringLiteral("line"));

    QCOMPARE(line->value(), 1);
    QCOMPARE(line->minimum(), 1);

    edit.setMaximumLineCount(50);
    QCOMPARE(line->value(), 1);
    QCOMPARE(line->minimum(), 1);
    QCOMPARE(line->maximum(), 50);

    edit.setMaximumLineCount(10);
    QCOMPARE(line->value(), 1);
    QCOMPARE(line->minimum(), 1);
    QCOMPARE(line->maximum(), 10);
}

QTEST_MAIN(TextGoToLineWidgetTest)

#include "moc_textgotolinewidgettest.cpp"
