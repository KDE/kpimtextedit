/*
   SPDX-FileCopyrightText: 2014-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once
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
