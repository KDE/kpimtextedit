/*
   SPDX-FileCopyrightText: 2014-2021 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "texttospeechinterface.h"
#include "texttospeech.h"

using namespace KPIMTextEdit;

class Q_DECL_HIDDEN KPIMTextEdit::TextToSpeechInterfacePrivate
{
public:
    TextToSpeechInterfacePrivate(TextToSpeechWidget *textToSpeechWidget)
        : mTextToSpeechWidget(textToSpeechWidget)
    {
    }

    TextToSpeechWidget *const mTextToSpeechWidget;
};

TextToSpeechInterface::TextToSpeechInterface(TextToSpeechWidget *textToSpeechWidget, QObject *parent)
    : AbstractTextToSpeechInterface(parent)
    , d(new KPIMTextEdit::TextToSpeechInterfacePrivate(textToSpeechWidget))
{
    KPIMTextEdit::TextToSpeech::self(); // init
    connect(d->mTextToSpeechWidget, &KPIMTextEdit::TextToSpeechWidget::stateChanged, this, &TextToSpeechInterface::stateChanged);
    connect(KPIMTextEdit::TextToSpeech::self(),
            &KPIMTextEdit::TextToSpeech::stateChanged,
            d->mTextToSpeechWidget,
            &KPIMTextEdit::TextToSpeechWidget::slotStateChanged);
}

TextToSpeechInterface::~TextToSpeechInterface()
{
    delete d;
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
