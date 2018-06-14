/*
   Copyright (C) 2014-2018 Laurent Montel <montel@kde.org>

   This library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Library General Public
   License as published by the Free Software Foundation; either
   version 2 of the License, or (at your option) any later version.

   This library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Library General Public License for more details.

   You should have received a copy of the GNU Library General Public License
   along with this library; see the file COPYING.LIB.  If not, write to
   the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
   Boston, MA 02110-1301, USA.
*/

#ifndef TEXTTOSPEECHWIDGET_H
#define TEXTTOSPEECHWIDGET_H

#include <QWidget>
#include "kpimtextedit_export.h"
#include "texttospeech.h"
namespace KPIMTextEdit {
class AbstractTextToSpeechInterface;
class TextToSpeechWidgetPrivate;
class KPIMTEXTEDIT_EXPORT TextToSpeechWidget : public QWidget
{
    Q_OBJECT
public:
    explicit TextToSpeechWidget(QWidget *parent = nullptr);
    ~TextToSpeechWidget();

    enum State {
        Stop = 0,
        Play,
        Pause
    };
    Q_ENUMS(State)

    Q_REQUIRED_RESULT State state() const;
    void setState(KPIMTextEdit::TextToSpeechWidget::State state);

    void setTextToSpeechInterface(AbstractTextToSpeechInterface *interface);

    Q_REQUIRED_RESULT bool isReady() const;

public Q_SLOTS:
    void say(const QString &text);

    void slotStateChanged(KPIMTextEdit::TextToSpeech::State state);

Q_SIGNALS:
    void stateChanged(KPIMTextEdit::TextToSpeechWidget::State state);

private:
    void slotVolumeChanged(int value);
    void slotConfigure();
    TextToSpeechWidgetPrivate *const d;
};
}
#endif // TEXTTOSPEECHWIDGET_H
