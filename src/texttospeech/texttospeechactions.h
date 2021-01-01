/*
   SPDX-FileCopyrightText: 2014-2021 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
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
