/*
   SPDX-FileCopyrightText: 2014-2021 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include <QObject>
namespace KPIMTextEdit
{
class TextToSpeechWidget;
}
class TextToSpeechWidgetTest : public QObject
{
    Q_OBJECT
public:
    explicit TextToSpeechWidgetTest(QObject *parent = nullptr);

private:
    void addInterface(KPIMTextEdit::TextToSpeechWidget *widget);

private Q_SLOTS:
    void shouldHaveDefaultValue();
    void shouldChangeButtonEnableStateWhenChangeState();
    void shouldChangeStateWhenClickOnPlayPause();
    void shouldChangeStateWhenClickOnStop();
    void shouldEmitStateChanged();
};

