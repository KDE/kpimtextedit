/*
   SPDX-FileCopyrightText: 2014-2021 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#ifndef TEXTGOTOLINEWIDGETTEST_H
#define TEXTGOTOLINEWIDGETTEST_H
#include <QObject>

class TextGoToLineWidgetTest : public QObject
{
    Q_OBJECT
public:
    explicit TextGoToLineWidgetTest(QObject *parent = nullptr);

private Q_SLOTS:
    void shouldHaveDefaultValuesOnCreation();
    void shouldEmitGoToLineSignalWhenPressOnButton();
    void shouldEmitGoToLineSignalCorrectValueWhenPressOnButton();
    void shouldHideWidgetWhenClickOnCloseButton();
    void shouldHideWidgetWhenPressEscape();
    void shouldEmitGoToLineSignalWhenSpinboxHasFocusAndWePressEnter();
    void shouldHasFocusEachTimeThatItShown();
    void shouldSetFocusWhenWeRecallGotToLine();
    void shouldChangeMaximumValue();
};

#endif // TEXTGOTOLINEWIDGETTEST_H
