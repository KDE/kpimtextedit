/*
   SPDX-FileCopyrightText: 2014-2021 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "kpimtextedit_export.h"
#include "texttospeechwidget.h"
#include <QObject>
class QAction;
namespace KPIMTextEdit
{
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
    ~TextToSpeechActions() override;

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

