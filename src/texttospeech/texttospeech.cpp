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

#include "texttospeech.h"

#include <KConfig>
#include <KConfigGroup>
#include <QLocale>
#include <QVector>
#if KPIMTEXTEDIT_HAVE_TEXTTOSPEECH
#include <QtTextToSpeech/QTextToSpeech>
#endif

namespace KPIMTextEdit
{

class TextToSpeechPrivate
{
public:
    TextToSpeechPrivate()
        : textToSpeech(new TextToSpeech)
    {
    }

    ~TextToSpeechPrivate()
    {
        delete textToSpeech;
    }

    TextToSpeech *textToSpeech;
};

Q_GLOBAL_STATIC(TextToSpeechPrivate, sInstance)

TextToSpeech::TextToSpeech(QObject *parent)
    : QObject(parent)
#if KPIMTEXTEDIT_HAVE_TEXTTOSPEECH
    , mTextToSpeech(Q_NULLPTR)
#endif
{
    reloadSettings();
}

TextToSpeech::~TextToSpeech()
{

}

void TextToSpeech::reloadSettings()
{
#if KPIMTEXTEDIT_HAVE_TEXTTOSPEECH
    KConfig config(QStringLiteral("texttospeechrc"));
    KConfigGroup grp = config.group("Settings");
    const QString engineName = grp.readEntry("engine");
    if (!mTextToSpeech) {
#if QT_VERSION < QT_VERSION_CHECK(5, 7, 0)
        mTextToSpeech = new QTextToSpeech(this, engineName);
#else
        mTextToSpeech = new QTextToSpeech(engineName, QVariantMap(), this);
#endif
        connect(mTextToSpeech, &QTextToSpeech::stateChanged, this, &TextToSpeech::slotStateChanged);
    } else if (mDefaultEngine != engineName) {
        disconnect(mTextToSpeech, &QTextToSpeech::stateChanged, this, &TextToSpeech::slotStateChanged);
        delete mTextToSpeech;
#if QT_VERSION < QT_VERSION_CHECK(5, 7, 0)
        mTextToSpeech = new QTextToSpeech(this, engineName);
#else
        mTextToSpeech = new QTextToSpeech(engineName, QVariantMap(), this);
#endif
        connect(mTextToSpeech, &QTextToSpeech::stateChanged, this, &TextToSpeech::slotStateChanged);
    }
    mDefaultEngine = engineName;
    mTextToSpeech->setRate(grp.readEntry("rate", 0));
    mTextToSpeech->setPitch(grp.readEntry("pitch", 0));
    mTextToSpeech->setVolume(grp.readEntry("volume", 0));
    mTextToSpeech->setLocale(QLocale(grp.readEntry("localeName")));
    //It doesn't have api for it mTextToSpeech->setVoice(grp.readEntry("voice"));
#endif
}

TextToSpeech *TextToSpeech::self()
{
    return sInstance->textToSpeech; //will create it
}

void TextToSpeech::slotStateChanged()
{
#if KPIMTEXTEDIT_HAVE_TEXTTOSPEECH
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
#endif
}

bool TextToSpeech::isReady() const
{
#if KPIMTEXTEDIT_HAVE_TEXTTOSPEECH
    return (mTextToSpeech->state() != QTextToSpeech::BackendError);
#else
    return false;
#endif
}

void TextToSpeech::say(const QString &text)
{
#if KPIMTEXTEDIT_HAVE_TEXTTOSPEECH
    mTextToSpeech->say(text);
#else
    Q_UNUSED(text);
#endif

}

void TextToSpeech::stop()
{
#if KPIMTEXTEDIT_HAVE_TEXTTOSPEECH
    mTextToSpeech->stop();
#endif
}

void TextToSpeech::pause()
{
#if KPIMTEXTEDIT_HAVE_TEXTTOSPEECH
    mTextToSpeech->pause();
#endif
}

void TextToSpeech::resume()
{
#if KPIMTEXTEDIT_HAVE_TEXTTOSPEECH
    mTextToSpeech->resume();
#endif
}

void TextToSpeech::setRate(double rate)
{
#if KPIMTEXTEDIT_HAVE_TEXTTOSPEECH
    mTextToSpeech->setRate(rate);
#else
    Q_UNUSED(rate);
#endif
}

void TextToSpeech::setPitch(double pitch)
{
#if KPIMTEXTEDIT_HAVE_TEXTTOSPEECH
    mTextToSpeech->setPitch(pitch);
#else
    Q_UNUSED(pitch);
#endif
}

void TextToSpeech::setVolume(int volume)
{
#if KPIMTEXTEDIT_HAVE_TEXTTOSPEECH
    mTextToSpeech->setVolume(volume);
#else
    Q_UNUSED(volume);
#endif
}

int TextToSpeech::volume() const
{
#if KPIMTEXTEDIT_HAVE_TEXTTOSPEECH
    return mTextToSpeech->volume();
#else
    return 0;
#endif
}

QVector<QLocale> TextToSpeech::availableLocales() const
{
#if KPIMTEXTEDIT_HAVE_TEXTTOSPEECH
    return mTextToSpeech->availableLocales();
#else
    return QVector<QLocale>();
#endif
}

QStringList TextToSpeech::availableVoices() const
{
    QStringList lst;
#if KPIMTEXTEDIT_HAVE_TEXTTOSPEECH
    Q_FOREACH(const QVoice &voice, mTextToSpeech->availableVoices()) {
        lst << voice.name();
    }
#endif
    return lst;
}

QStringList TextToSpeech::availableEngines() const
{
#if KPIMTEXTEDIT_HAVE_TEXTTOSPEECH
    return QTextToSpeech::availableEngines();
#else
    return QStringList();
#endif
}

void TextToSpeech::setLocale(const QLocale &locale) const
{
#if KPIMTEXTEDIT_HAVE_TEXTTOSPEECH
    mTextToSpeech->setLocale(locale);
#else
    Q_UNUSED(locale);
#endif
}

QLocale TextToSpeech::locale() const
{
#if KPIMTEXTEDIT_HAVE_TEXTTOSPEECH
    return mTextToSpeech->locale();
#else
    return QLocale();
#endif
}

}
