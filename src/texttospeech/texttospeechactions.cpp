/*
  Copyright (c) 2015 Montel Laurent <montel@kde.org>

  This program is free software; you can redistribute it and/or modify it
  under the terms of the GNU General Public License, version 2, as
  published by the Free Software Foundation.

  This program is distributed in the hope that it will be useful, but
  WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  General Public License for more details.

  You should have received a copy of the GNU General Public License along
  with this program; if not, write to the Free Software Foundation, Inc.,
  51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
*/

#include "texttospeechactions.h"
#include <KLocalizedString>
#include <QAction>

using namespace KPIMTextEdit;

class KPIMTextEdit::TextToSpeechActionsPrivate
{
public:
    TextToSpeechActionsPrivate()
        : mState(TextToSpeechWidget::Stop),
          mStopAction(Q_NULLPTR),
          mPlayPauseAction(Q_NULLPTR)
    {

    }
    void updateButtonState();
    TextToSpeechWidget::State mState;
    QAction *mStopAction;
    QAction *mPlayPauseAction;
};

TextToSpeechActions::TextToSpeechActions(QObject *parent)
    : QObject(parent),
      d(new KPIMTextEdit::TextToSpeechActionsPrivate)
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

void TextToSpeechActions::setState(const TextToSpeechWidget::State &state)
{
    if (d->mState != state) {
        d->mState = state;
        d->updateButtonState();
    }
}

void TextToSpeechActionsPrivate::updateButtonState()
{
    mPlayPauseAction->setIcon(QIcon::fromTheme((mState == TextToSpeechWidget::Stop) ? QStringLiteral("media-playback-start") : QStringLiteral("media-playback-pause")));
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
