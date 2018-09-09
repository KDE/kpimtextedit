/*
   Copyright (C) 2014-2018 Laurent Montel <montel@kde.org>

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
#if KPIMTEXTEDIT_HAVE_TEXTTOSPEECH
#include <QTextToSpeech>
#endif
using namespace KPIMTextEdit;

TextToSpeechConfigInterface::TextToSpeechConfigInterface(QObject *parent)
    : AbstractTextToSpeechConfigInterface(parent)
#if KPIMTEXTEDIT_HAVE_TEXTTOSPEECH
    , mTextToSpeech(new QTextToSpeech(this))
#endif
{
}

TextToSpeechConfigInterface::~TextToSpeechConfigInterface()
{
}

QStringList TextToSpeechConfigInterface::availableVoices() const
{
    QStringList lst;
#if KPIMTEXTEDIT_HAVE_TEXTTOSPEECH
    const QVector<QVoice> voices = mTextToSpeech->availableVoices();
    lst.reserve(voices.count());
    for (const QVoice &voice : voices) {
        lst << voice.name();
    }
#endif
    return lst;
}

QStringList TextToSpeechConfigInterface::availableEngines() const
{
#if KPIMTEXTEDIT_HAVE_TEXTTOSPEECH
    return mTextToSpeech->availableEngines();
#else
    return QStringList();
#endif
}

QVector<QLocale> TextToSpeechConfigInterface::availableLocales() const
{
#if KPIMTEXTEDIT_HAVE_TEXTTOSPEECH
    return mTextToSpeech->availableLocales();
#else
    return QVector<QLocale>();
#endif
}

QLocale TextToSpeechConfigInterface::locale() const
{
#if KPIMTEXTEDIT_HAVE_TEXTTOSPEECH
    return mTextToSpeech->locale();
#else
    return QLocale();
#endif
}

void TextToSpeechConfigInterface::setEngine(const QString &engineName)
{
#if KPIMTEXTEDIT_HAVE_TEXTTOSPEECH
    delete mTextToSpeech;
    mTextToSpeech = new QTextToSpeech(engineName, this);
#endif
}
