/*
   SPDX-FileCopyrightText: 2014-2021 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#ifndef TEXTTOSPEECHINTERFACE_H
#define TEXTTOSPEECHINTERFACE_H

#include <QObject>
#include "texttospeechwidget.h"
#include "abstracttexttospeechinterface.h"
#include "kpimtextedit_export.h"
namespace KPIMTextEdit {
class TextToSpeechWidget;
class TextToSpeechInterfacePrivate;
/**
 * @brief The TextToSpeechInterface class
 * @author Laurent Montel <montel@kde.org>
 */
class KPIMTEXTEDIT_EXPORT TextToSpeechInterface : public AbstractTextToSpeechInterface
{
    Q_OBJECT
public:
    explicit TextToSpeechInterface(TextToSpeechWidget *textToSpeechWidget, QObject *parent = nullptr);
    ~TextToSpeechInterface() override;

    Q_REQUIRED_RESULT bool isReady() const override;
    void say(const QString &text) override;
    Q_REQUIRED_RESULT int volume() const override;
    void setVolume(int value) override;
    void reloadSettings() override;

private:
    void stateChanged(KPIMTextEdit::TextToSpeechWidget::State state);
    TextToSpeechInterfacePrivate *const d;
};
}

#endif // TEXTTOSPEECHINTERFACE_H
