/*
  Copyright (c) 2014-2015 Montel Laurent <montel@kde.org>

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

#include "texttospeechinterface.h"
#include "texttospeech.h"

using namespace KPIMTextEdit;

TextToSpeechInterface::TextToSpeechInterface(TextToSpeechWidget *textToSpeechWidget, QObject *parent)
    : AbstractTextToSpeechInterface(parent),
      mTextToSpeechWidget(textToSpeechWidget)
{
    KPIMTextEdit::TextToSpeech::self(); //init
    connect(mTextToSpeechWidget, &KPIMTextEdit::TextToSpeechWidget::stateChanged, this, &TextToSpeechInterface::stateChanged);
    connect(KPIMTextEdit::TextToSpeech::self(), &KPIMTextEdit::TextToSpeech::stateChanged, mTextToSpeechWidget, &KPIMTextEdit::TextToSpeechWidget::slotStateChanged);
}

TextToSpeechInterface::~TextToSpeechInterface()
{
}

bool TextToSpeechInterface::isReady() const
{
    return KPIMTextEdit::TextToSpeech::self()->isReady();
}

void TextToSpeechInterface::say(const QString &text)
{
    mTextToSpeechWidget->setState(KPIMTextEdit::TextToSpeechWidget::Play);
    mTextToSpeechWidget->show();
    KPIMTextEdit::TextToSpeech::self()->say(text);
}

int TextToSpeechInterface::volume() const
{
    return KPIMTextEdit::TextToSpeech::self()->volume();
}

void TextToSpeechInterface::setVolume(int value)
{
    KPIMTextEdit::TextToSpeech::self()->setVolume(value);
}

void TextToSpeechInterface::reloadSettings()
{
    KPIMTextEdit::TextToSpeech::self()->reloadSettings();
}

void TextToSpeechInterface::stateChanged(TextToSpeechWidget::State state)
{
    switch (state) {
    case TextToSpeechWidget::Stop:
        KPIMTextEdit::TextToSpeech::self()->stop();
        break;
    case TextToSpeechWidget::Play:
        KPIMTextEdit::TextToSpeech::self()->resume();
        break;
    case TextToSpeechWidget::Pause:
        KPIMTextEdit::TextToSpeech::self()->pause();
        break;
    }
}
