/*
   SPDX-FileCopyrightText: 2014-2021 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "texttospeech.h"

#include <KConfig>
#include <KConfigGroup>
#include <QLocale>
#include <QTextToSpeech>
#include <QVector>

using namespace KPIMTextEdit;
TextToSpeech::TextToSpeech(QObject *parent)
    : QObject(parent)
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
    // It doesn't have api for it mTextToSpeech->setVoice(grp.readEntry("voice"));
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

void TextToSpeech::setVolume(double volume)
{
    mTextToSpeech->setVolume(volume);
}

double TextToSpeech::volume() const
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
