/*
   Copyright (C) 2014-2020 Laurent Montel <montel@kde.org>

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

#ifndef TEXTTOSPEECHACTIONS_H
#define TEXTTOSPEECHACTIONS_H

#include <QObject>
#include "kpimtextedit_export.h"
#include "texttospeechwidget.h"
class QAction;
namespace KPIMTextEdit {
class TextToSpeechActionsPrivate;
/**
 * @brief The TextToSpeechActions class
 * @author Laurent Montel <montel@kde.org>
 */
class KPIMTEXTEDIT_EXPORT TextToSpeechActions : public QObject
{
    Q_OBJECT
public:
    explicit TextToSpeechActions(QObject *parent = nullptr);
    ~TextToSpeechActions();

    QAction *stopAction() const;

    QAction *playPauseAction() const;

    Q_REQUIRED_RESULT TextToSpeechWidget::State state() const;

public Q_SLOTS:
    void setState(TextToSpeechWidget::State);
    void slotStop();

Q_SIGNALS:
    void stateChanged(KPIMTextEdit::TextToSpeechWidget::State state);

private:
    void slotPlayPause();
    TextToSpeechActionsPrivate *const d;
};
}

#endif // TEXTTOSPEECHACTIONS_H
