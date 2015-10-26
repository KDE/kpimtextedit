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

#include "texttospeechconfiginterface.h"
#include "texttospeech.h"

using namespace KPIMTextEdit;

TextToSpeechConfigInterface::TextToSpeechConfigInterface(QObject *parent)
    : AbstractTextToSpeechConfigInterface(parent)
{

}

TextToSpeechConfigInterface::~TextToSpeechConfigInterface()
{

}

QStringList TextToSpeechConfigInterface::availableVoices() const
{
    return KPIMTextEdit::TextToSpeech::self()->availableVoices();
}

QStringList TextToSpeechConfigInterface::availableEngines() const
{
    return KPIMTextEdit::TextToSpeech::self()->availableEngines();
}

QVector<QLocale> TextToSpeechConfigInterface::availableLocales() const
{
    return KPIMTextEdit::TextToSpeech::self()->availableLocales();
}

QLocale TextToSpeechConfigInterface::locale() const
{
    return KPIMTextEdit::TextToSpeech::self()->locale();
}

void TextToSpeechConfigInterface::setLocale(const QLocale &locale)
{
    KPIMTextEdit::TextToSpeech::self()->setLocale(locale);
}
