/*
   Copyright (C) 2014-2019 Laurent Montel <montel@kde.org>

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

#include "texttospeechconfiginterface.h"
#include <QTextToSpeech>
using namespace KPIMTextEdit;

TextToSpeechConfigInterface::TextToSpeechConfigInterface(QObject *parent)
    : AbstractTextToSpeechConfigInterface(parent)
    , mTextToSpeech(new QTextToSpeech(this))
{
}

TextToSpeechConfigInterface::~TextToSpeechConfigInterface()
{
}

QStringList TextToSpeechConfigInterface::availableVoices() const
{
    QStringList lst;
    const QVector<QVoice> voices = mTextToSpeech->availableVoices();
    lst.reserve(voices.count());
    for (const QVoice &voice : voices) {
        lst << voice.name();
    }
    return lst;
}

QStringList TextToSpeechConfigInterface::availableEngines() const
{
    return mTextToSpeech->availableEngines();
}

QVector<QLocale> TextToSpeechConfigInterface::availableLocales() const
{
    return mTextToSpeech->availableLocales();
}

QLocale TextToSpeechConfigInterface::locale() const
{
    return mTextToSpeech->locale();
}

void TextToSpeechConfigInterface::setEngine(const QString &engineName)
{
    delete mTextToSpeech;
    mTextToSpeech = new QTextToSpeech(engineName, this);
}
