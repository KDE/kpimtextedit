/*
   Copyright (C) 2014-2017 Laurent Montel <montel@kde.org>

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

#include "texttospeechinterface.h"
#include "texttospeech.h"

using namespace KPIMTextEdit;

class KPIMTextEdit::TextToSpeechInterfacePrivate
{
public:
    TextToSpeechInterfacePrivate(TextToSpeechWidget *textToSpeechWidget)
        : mTextToSpeechWidget(textToSpeechWidget)
    {

    }
    TextToSpeechWidget *mTextToSpeechWidget;
};

TextToSpeechInterface::TextToSpeechInterface(TextToSpeechWidget *textToSpeechWidget, QObject *parent)
    : AbstractTextToSpeechInterface(parent),
      d(new KPIMTextEdit::TextToSpeechInterfacePrivate(textToSpeechWidget))
{
    KPIMTextEdit::TextToSpeech::self(); //init
    connect(d->mTextToSpeechWidget, &KPIMTextEdit::TextToSpeechWidget::stateChanged, this, &TextToSpeechInterface::stateChanged);
    connect(KPIMTextEdit::TextToSpeech::self(), &KPIMTextEdit::TextToSpeech::stateChanged, d->mTextToSpeechWidget, &KPIMTextEdit::TextToSpeechWidget::slotStateChanged);
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
    d->mTextToSpeechWidget->setState(KPIMTextEdit::TextToSpeechWidget::Play);
    d->mTextToSpeechWidget->show();
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
