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

TextToSpeechActions::TextToSpeechActions(QObject *parent)
    : QObject(parent),
      mState(TextToSpeechWidget::Stop),
      mStopAction(Q_NULLPTR),
      mPlayPauseAction(Q_NULLPTR)
{
    mStopAction = new QAction(i18n("Stop"), this);
    mStopAction->setObjectName(QStringLiteral("stopbutton"));
    mStopAction->setIcon(QIcon::fromTheme(QStringLiteral("media-playback-stop")));
    mStopAction->setToolTip(i18n("Stop"));
    connect(mStopAction, &QAction::triggered, this, &TextToSpeechActions::slotStop);

    mPlayPauseAction = new QAction(this);
    mPlayPauseAction->setObjectName(QStringLiteral("playpausebutton"));
    mPlayPauseAction->setIcon(QIcon::fromTheme(QStringLiteral("media-playback-start")));
    connect(mPlayPauseAction, &QAction::triggered, this, &TextToSpeechActions::slotPlayPause);

    updateButtonState();
}

TextToSpeechActions::~TextToSpeechActions()
{

}

QAction *TextToSpeechActions::stopAction() const
{
    return mStopAction;
}

QAction *TextToSpeechActions::playPauseAction() const
{
    return mPlayPauseAction;
}

TextToSpeechWidget::State TextToSpeechActions::state() const
{
    return mState;
}

void TextToSpeechActions::setState(const TextToSpeechWidget::State &state)
{
    if (mState != state) {
        mState = state;
        updateButtonState();
    }
}

void TextToSpeechActions::updateButtonState()
{
    mPlayPauseAction->setIcon(QIcon::fromTheme((mState == TextToSpeechWidget::Stop) ? QStringLiteral("media-playback-start") : QStringLiteral("media-playback-pause")));
    mPlayPauseAction->setEnabled((mState != TextToSpeechWidget::Stop));
    const QString text = (mState != TextToSpeechWidget::Play) ? i18n("Pause") : i18n("Play");
    mPlayPauseAction->setToolTip(text);
    mPlayPauseAction->setText(text);
}

void TextToSpeechActions::slotPlayPause()
{
    if (mState == KPIMTextEdit::TextToSpeechWidget::Pause) {
        mState = KPIMTextEdit::TextToSpeechWidget::Play;
    } else if (mState == KPIMTextEdit::TextToSpeechWidget::Play) {
        mState = KPIMTextEdit::TextToSpeechWidget::Pause;
    } else if (mState == KPIMTextEdit::TextToSpeechWidget::Stop) {
        mState = KPIMTextEdit::TextToSpeechWidget::Play;
    } else {
        return;
    }
    updateButtonState();
    Q_EMIT stateChanged(mState);
}

void TextToSpeechActions::slotStop()
{
    if (mState != KPIMTextEdit::TextToSpeechWidget::Stop) {
        mState = KPIMTextEdit::TextToSpeechWidget::Stop;
        updateButtonState();
        Q_EMIT stateChanged(mState);
    }
}
