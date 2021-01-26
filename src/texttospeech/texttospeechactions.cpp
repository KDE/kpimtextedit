/*
   SPDX-FileCopyrightText: 2014-2021 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "texttospeechactions.h"
#include <KLocalizedString>
#include <QAction>

using namespace KPIMTextEdit;

class Q_DECL_HIDDEN KPIMTextEdit::TextToSpeechActionsPrivate
{
public:
    TextToSpeechActionsPrivate()
    {
    }

    void updateButtonState();
    TextToSpeechWidget::State mState = TextToSpeechWidget::Stop;
    QAction *mStopAction = nullptr;
    QAction *mPlayPauseAction = nullptr;
};

TextToSpeechActions::TextToSpeechActions(QObject *parent)
    : QObject(parent)
    , d(new KPIMTextEdit::TextToSpeechActionsPrivate)
{
    d->mStopAction = new QAction(i18n("Stop"), this);
    d->mStopAction->setObjectName(QStringLiteral("stopbutton"));
    d->mStopAction->setIcon(QIcon::fromTheme(QStringLiteral("media-playback-stop")));
    d->mStopAction->setToolTip(i18n("Stop"));
    connect(d->mStopAction, &QAction::triggered, this, &TextToSpeechActions::slotStop);

    d->mPlayPauseAction = new QAction(this);
    d->mPlayPauseAction->setObjectName(QStringLiteral("playpausebutton"));
    d->mPlayPauseAction->setIcon(QIcon::fromTheme(QStringLiteral("media-playback-start")));
    connect(d->mPlayPauseAction, &QAction::triggered, this, &TextToSpeechActions::slotPlayPause);

    d->updateButtonState();
}

TextToSpeechActions::~TextToSpeechActions()
{
    delete d;
}

QAction *TextToSpeechActions::stopAction() const
{
    return d->mStopAction;
}

QAction *TextToSpeechActions::playPauseAction() const
{
    return d->mPlayPauseAction;
}

TextToSpeechWidget::State TextToSpeechActions::state() const
{
    return d->mState;
}

void TextToSpeechActions::setState(TextToSpeechWidget::State state)
{
    if (d->mState != state) {
        d->mState = state;
        d->updateButtonState();
    }
}

void TextToSpeechActionsPrivate::updateButtonState()
{
    mPlayPauseAction->setIcon(
        QIcon::fromTheme((mState == TextToSpeechWidget::Stop) ? QStringLiteral("media-playback-start") : QStringLiteral("media-playback-pause")));
    mPlayPauseAction->setEnabled((mState != TextToSpeechWidget::Stop));
    const QString text = (mState != TextToSpeechWidget::Play) ? i18n("Pause") : i18n("Play");
    mPlayPauseAction->setToolTip(text);
    mPlayPauseAction->setText(text);
}

void TextToSpeechActions::slotPlayPause()
{
    if (d->mState == KPIMTextEdit::TextToSpeechWidget::Pause) {
        d->mState = KPIMTextEdit::TextToSpeechWidget::Play;
    } else if (d->mState == KPIMTextEdit::TextToSpeechWidget::Play) {
        d->mState = KPIMTextEdit::TextToSpeechWidget::Pause;
    } else if (d->mState == KPIMTextEdit::TextToSpeechWidget::Stop) {
        d->mState = KPIMTextEdit::TextToSpeechWidget::Play;
    } else {
        return;
    }
    d->updateButtonState();
    Q_EMIT stateChanged(d->mState);
}

void TextToSpeechActions::slotStop()
{
    if (d->mState != KPIMTextEdit::TextToSpeechWidget::Stop) {
        d->mState = KPIMTextEdit::TextToSpeechWidget::Stop;
        d->updateButtonState();
        Q_EMIT stateChanged(d->mState);
    }
}
