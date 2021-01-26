/*
   SPDX-FileCopyrightText: 2014-2021 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#ifndef TEXTTOSPEECHCONFIGWIDGETTEST_H
#define TEXTTOSPEECHCONFIGWIDGETTEST_H

#include <QObject>
namespace KPIMTextEdit
{
class TextToSpeechConfigWidget;
}
class TextToSpeechConfigWidgetTest : public QObject
{
    Q_OBJECT
public:
    explicit TextToSpeechConfigWidgetTest(QObject *parent = nullptr);
    ~TextToSpeechConfigWidgetTest();

private Q_SLOTS:
    void shouldHaveDefaultValue();
    void shouldEmitConfigChangedWhenChangeConfigValue();

private:
    void addInterface(KPIMTextEdit::TextToSpeechConfigWidget *widget);
};

#endif // TEXTTOSPEECHCONFIGWIDGETTEST_H
