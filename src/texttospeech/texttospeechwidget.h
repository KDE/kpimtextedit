/*
   Copyright (C) 2014-2016 Laurent Montel <montel@kde.org>

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

#include <QPointer>
#include <QWidget>
#include "kpimtextedit_export.h"
#include "texttospeech.h"
namespace KPIMTextEdit
{
class AbstractTextToSpeechInterface;
class TextToSpeechWidgetPrivate;
class KPIMTEXTEDIT_EXPORT TextToSpeechWidget : public QWidget
{
    Q_OBJECT
public:
    explicit TextToSpeechWidget(QWidget *parent = Q_NULLPTR);
    ~TextToSpeechWidget();

    enum State {
        Stop = 0,
        Play,
        Pause
    };
    Q_ENUMS(State)

    State state() const;
    void setState(KPIMTextEdit::TextToSpeechWidget::State state);

    void setTextToSpeechInterface(AbstractTextToSpeechInterface *interface);

    bool isReady() const;

public Q_SLOTS:
    void say(const QString &text);

    void slotStateChanged(KPIMTextEdit::TextToSpeech::State state);

Q_SIGNALS:
    void stateChanged(KPIMTextEdit::TextToSpeechWidget::State state);

private Q_SLOTS:
    void slotVolumeChanged(int value);
    void slotConfigure();

private:
    TextToSpeechWidgetPrivate *const d;
};
}
#endif // TEXTTOSPEECHWIDGET_H
