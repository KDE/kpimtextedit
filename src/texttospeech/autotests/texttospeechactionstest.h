/*
   SPDX-FileCopyrightText: 2014-2020 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#ifndef TEXTTOSPEECHACTIONSTEST_H
#define TEXTTOSPEECHACTIONSTEST_H

#include <QObject>

class TextToSpeechActionsTest : public QObject
{
    Q_OBJECT
public:
    explicit TextToSpeechActionsTest(QObject *parent = nullptr);
    ~TextToSpeechActionsTest();
private Q_SLOTS:
    void shouldHaveDefaultValue();
    void shouldChangeButtonEnableStateWhenChangeState();
    void shouldChangeStateWhenClickOnPlayPause();
    void shouldChangeStateWhenClickOnStop();
    void shouldEmitStateChanged();
};

#endif // TEXTTOSPEECHACTIONSTEST_H
