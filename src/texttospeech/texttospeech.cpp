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

#include "texttospeech.h"

#include <KConfig>
#include <KConfigGroup>
#include <QLocale>
#include <QVector>
#include <QTextToSpeech>

namespace KPIMTextEdit {
TextToSpeech::TextToSpeech(QObject *parent)
    : QObject(parent)
    , mTextToSpeech(nullptr)
{
    reloadSettings();
}

TextToSpeech::~TextToSpeech()
{
}

void TextToSpeech::reloadSettings()
{
    KConfig config(QStringLiteral("texttospeechrc"));
    KConfigGroup grp = config.group("Settings");
    const QString engineName = grp.readEntry("engine");
    if (!mTextToSpeech) {
        mTextToSpeech = new QTextToSpeech(engineName, this);
        connect(mTextToSpeech, &QTextToSpeech::stateChanged, this, &TextToSpeech::slotStateChanged);
    } else if (mDefaultEngine != engineName) {
        disconnect(mTextToSpeech, &QTextToSpeech::stateChanged, this, &TextToSpeech::slotStateChanged);
        delete mTextToSpeech;
        mTextToSpeech = new QTextToSpeech(engineName, this);
        connect(mTextToSpeech, &QTextToSpeech::stateChanged, this, &TextToSpeech::slotStateChanged);
    }
    mDefaultEngine = engineName;
    mTextToSpeech->setRate(grp.readEntry("rate", 0.0));
    mTextToSpeech->setPitch(grp.readEntry("pitch", 0.0));
    mTextToSpeech->setVolume(grp.readEntry("volume", 0));
    mTextToSpeech->setLocale(QLocale(grp.readEntry("localeName")));
    //It doesn't have api for it mTextToSpeech->setVoice(grp.readEntry("voice"));
}

TextToSpeech *TextToSpeech::self()
{
    static TextToSpeech s_self;
    return &s_self;
}

void TextToSpeech::slotStateChanged()
{
    TextToSpeech::State state;
    switch (mTextToSpeech->state()) {
    case QTextToSpeech::Ready:
        state = TextToSpeech::Ready;
        break;
    case QTextToSpeech::Speaking:
        state = TextToSpeech::Speaking;
        break;
    case QTextToSpeech::Paused:
        state = TextToSpeech::Paused;
        break;
    case QTextToSpeech::BackendError:
        state = TextToSpeech::BackendError;
        break;
    }
    Q_EMIT stateChanged(state);
}

bool TextToSpeech::isReady() const
{
    return mTextToSpeech->state() != QTextToSpeech::BackendError;
}

void TextToSpeech::say(const QString &text)
{
    mTextToSpeech->say(text);
}

void TextToSpeech::stop()
{
    mTextToSpeech->stop();
}

void TextToSpeech::pause()
{
    mTextToSpeech->pause();
}

void TextToSpeech::resume()
{
    mTextToSpeech->resume();
}

void TextToSpeech::setRate(double rate)
{
    mTextToSpeech->setRate(rate);
}

void TextToSpeech::setPitch(double pitch)
{
    mTextToSpeech->setPitch(pitch);
}

void TextToSpeech::setVolume(int volume)
{
    mTextToSpeech->setVolume(volume);
}

int TextToSpeech::volume() const
{
    return mTextToSpeech->volume();
}

QVector<QLocale> TextToSpeech::availableLocales() const
{
    return mTextToSpeech->availableLocales();
}

QStringList TextToSpeech::availableVoices() const
{
    QStringList lst;
    const QVector<QVoice> voices = mTextToSpeech->availableVoices();
    lst.reserve(voices.count());
    for (const QVoice &voice : voices) {
        lst << voice.name();
    }
    return lst;
}

QStringList TextToSpeech::availableEngines() const
{
    return QTextToSpeech::availableEngines();
}

void TextToSpeech::setLocale(const QLocale &locale) const
{
    mTextToSpeech->setLocale(locale);
}

QLocale TextToSpeech::locale() const
{
    return mTextToSpeech->locale();
}
}
