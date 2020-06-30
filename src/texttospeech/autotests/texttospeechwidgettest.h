/*
   SPDX-FileCopyrightText: 2014-2020 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#ifndef TEXTTOSPEECHWIDGETTEST_H
#define TEXTTOSPEECHWIDGETTEST_H

#include <QObject>
namespace KPIMTextEdit {
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

#endif // TEXTTOSPEECHWIDGETTEST_H
