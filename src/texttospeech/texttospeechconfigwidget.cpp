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

#include "texttospeechconfigwidget.h"
#include "abstracttexttospeechconfiginterface.h"
#include "texttospeechconfiginterface.h"
#include "texttospeechlanguagecombobox.h"
#include <KLocalizedString>

#include <QFormLayout>
#include <QSlider>
#include <QComboBox>
#include <QTimer>
#include <KConfig>
#include <KConfigGroup>

using namespace KPIMTextEdit;
TextToSpeechConfigWidget::TextToSpeechConfigWidget(QWidget *parent)
    : QWidget(parent)
    , mAbstractTextToSpeechConfigInterface(new TextToSpeechConfigInterface(this))
{
    QFormLayout *layout = new QFormLayout(this);
    mVolume = new QSlider(this);
    mVolume->setObjectName(QStringLiteral("volume"));
    mVolume->setRange(0, 100);
    mVolume->setOrientation(Qt::Horizontal);
    mVolume->setTickPosition(QSlider::TicksBelow);
    connect(mVolume, &QSlider::valueChanged, this, &TextToSpeechConfigWidget::valueChanged);

    layout->addRow(i18n("Volume:"), mVolume);

    mRate = new QSlider(this);
    mRate->setTickPosition(QSlider::TicksBelow);
    mRate->setObjectName(QStringLiteral("rate"));
    mRate->setRange(-100, 100);
    mRate->setOrientation(Qt::Horizontal);
    layout->addRow(i18n("Rate:"), mRate);
    connect(mRate, &QSlider::valueChanged, this, &TextToSpeechConfigWidget::valueChanged);

    mPitch = new QSlider(this);
    mPitch->setRange(-100, 100);
    mPitch->setTickPosition(QSlider::TicksBelow);
    connect(mPitch, &QSlider::valueChanged, this, &TextToSpeechConfigWidget::valueChanged);
    mPitch->setObjectName(QStringLiteral("pitch"));
    mPitch->setOrientation(Qt::Horizontal);
    layout->addRow(i18n("Pitch:"), mPitch);

    mAvailableEngine = new QComboBox(this);
    mAvailableEngine->setObjectName(QStringLiteral("engine"));
    layout->addRow(i18n("Engine:"), mAvailableEngine);
    connect(mAvailableEngine, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &TextToSpeechConfigWidget::slotEngineChanged);
    connect(mAvailableEngine, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &TextToSpeechConfigWidget::valueChanged);

    mLanguage = new KPIMTextEdit::TextToSpeechLanguageComboBox(this);
    mLanguage->setObjectName(QStringLiteral("language"));
    layout->addRow(i18n("Language:"), mLanguage);
    connect(mAvailableEngine, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &TextToSpeechConfigWidget::slotLanguageChanged);
    connect(mLanguage, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &TextToSpeechConfigWidget::valueChanged);

    mVoice = new QComboBox(this);
    mVoice->setObjectName(QStringLiteral("voice"));
    layout->addRow(i18n("Voice:"), mVoice);
    connect(mVoice, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &TextToSpeechConfigWidget::valueChanged);

    QTimer::singleShot(0, this, &TextToSpeechConfigWidget::slotUpdateSettings);
}

TextToSpeechConfigWidget::~TextToSpeechConfigWidget()
{
}

void TextToSpeechConfigWidget::valueChanged()
{
    Q_EMIT configChanged(true);
}

void TextToSpeechConfigWidget::updateLocale()
{
    KConfig config(QStringLiteral("texttospeechrc"));
    KConfigGroup grp = config.group("Settings");
    const QString localeName = grp.readEntry("localeName");
    if (localeName.isEmpty()) {
        return;
    }
    mLanguage->selectLocaleName(localeName);
}

void TextToSpeechConfigWidget::readConfig()
{
    KConfig config(QStringLiteral("texttospeechrc"));
    KConfigGroup grp = config.group("Settings");
    mRate->setValue(static_cast<int>(grp.readEntry("rate", 0.0) * 100));
    mPitch->setValue(static_cast<int>(grp.readEntry("pitch", 0.0) * 100));
    mVolume->setValue(static_cast<int>(grp.readEntry("volume", 50)));
    updateLocale();
}

void TextToSpeechConfigWidget::writeConfig()
{
    KConfig config(QStringLiteral("texttospeechrc"));
    KConfigGroup grp = config.group("Settings");
    grp.writeEntry("volume", mVolume->value());
    grp.writeEntry("rate", (static_cast<int>((static_cast<double>(mRate->value()) / 100.0))));
    grp.writeEntry("pitch", (static_cast<int>((static_cast<double>(mPitch->value()) / 100.0))));
    grp.writeEntry("localeName", mLanguage->currentData().toLocale().name());
    grp.writeEntry("engine", mAvailableEngine->currentData().toString());
    grp.writeEntry("voice", mVoice->currentData().toString());
}

void TextToSpeechConfigWidget::slotUpdateSettings()
{
    updateAvailableLocales();
    updateAvailableEngine();
    updateAvailableVoices();
}

void TextToSpeechConfigWidget::setTextToSpeechConfigInterface(AbstractTextToSpeechConfigInterface *interface)
{
    delete mAbstractTextToSpeechConfigInterface;
    mAbstractTextToSpeechConfigInterface = interface;
    slotUpdateSettings();
}

void TextToSpeechConfigWidget::restoreDefaults()
{
    mRate->setValue(0);
    mPitch->setValue(0);
    mVolume->setValue(50);

    //TODO
}

void TextToSpeechConfigWidget::updateAvailableEngine()
{
    mAvailableEngine->clear();
    mAvailableEngine->addItem(i18nc("Default tts engine", "Default"), QString());
    const QStringList lst = mAbstractTextToSpeechConfigInterface->availableEngines();
    for (const QString &engine : lst) {
        mAvailableEngine->addItem(engine, engine);
    }
    updateEngine();
}

void TextToSpeechConfigWidget::updateAvailableVoices()
{
    mVoice->clear();
    const QStringList lst = mAbstractTextToSpeechConfigInterface->availableVoices();
    for (const QString &voice : lst) {
        mVoice->addItem(voice, voice);
    }
    updateVoice();
}

void TextToSpeechConfigWidget::updateVoice()
{
    KConfig config(QStringLiteral("texttospeechrc"));
    KConfigGroup grp = config.group("Settings");
    const QString voice = grp.readEntry("voice");
    int index = mVoice->findData(voice);
    if (index == -1) {
        index = 0;
    }
    mVoice->setCurrentIndex(index);
}

void TextToSpeechConfigWidget::updateEngine()
{
    KConfig config(QStringLiteral("texttospeechrc"));
    KConfigGroup grp = config.group("Settings");
    const QString engineName = grp.readEntry("engine");
    int index = mAvailableEngine->findData(engineName);
    if (index == -1) {
        index = 0;
    }
    mAvailableEngine->setCurrentIndex(index);
}

void TextToSpeechConfigWidget::updateAvailableLocales()
{
    mLanguage->clear();
    const QVector<QLocale> locales = mAbstractTextToSpeechConfigInterface->availableLocales();
    QLocale current = mAbstractTextToSpeechConfigInterface->locale();
    mLanguage->updateAvailableLocales(locales, current);
    updateLocale();
}

void TextToSpeechConfigWidget::slotEngineChanged()
{
    mAbstractTextToSpeechConfigInterface->setEngine(mAvailableEngine->currentData().toString());
    updateAvailableLocales();
}

void TextToSpeechConfigWidget::slotLanguageChanged()
{
    //QLocale locale = mLanguage->currentData().value<QLocale>();
}
