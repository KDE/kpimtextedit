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

#include "texttospeechconfigwidgettest.h"
#include "texttospeech/texttospeechconfigwidget.h"
#include "texttospeech/abstracttexttospeechconfiginterface.h"

#include <QTest>
#include <QSlider>
#include <QSignalSpy>
#include <QComboBox>

TextToSpeechConfigWidgetTest::TextToSpeechConfigWidgetTest(QObject *parent)
    : QObject(parent)
{
}

TextToSpeechConfigWidgetTest::~TextToSpeechConfigWidgetTest()
{
}

void TextToSpeechConfigWidgetTest::addInterface(KPIMTextEdit::TextToSpeechConfigWidget *widget)
{
    KPIMTextEdit::AbstractTextToSpeechConfigInterface *interface = new KPIMTextEdit::AbstractTextToSpeechConfigInterface(this);
    widget->setTextToSpeechConfigInterface(interface);
}

void TextToSpeechConfigWidgetTest::shouldHaveDefaultValue()
{
    KPIMTextEdit::TextToSpeechConfigWidget textToSpeechConfigWidget;
    addInterface(&textToSpeechConfigWidget);
    QSlider *volume = textToSpeechConfigWidget.findChild<QSlider *>(QStringLiteral("volume"));
    QVERIFY(volume);

    QSlider *rate = textToSpeechConfigWidget.findChild<QSlider *>(QStringLiteral("rate"));
    QVERIFY(rate);

    QSlider *pitch = textToSpeechConfigWidget.findChild<QSlider *>(QStringLiteral("pitch"));
    QVERIFY(pitch);

    QComboBox *language = textToSpeechConfigWidget.findChild<QComboBox *>(QStringLiteral("language"));
    QVERIFY(language);
    //FIXME
    //QVERIFY(language->count()>0);

    QComboBox *availableEngine = textToSpeechConfigWidget.findChild<QComboBox *>(QStringLiteral("engine"));
    QVERIFY(availableEngine);

    QComboBox *voice = textToSpeechConfigWidget.findChild<QComboBox *>(QStringLiteral("voice"));
    QVERIFY(voice);
}

void TextToSpeechConfigWidgetTest::shouldEmitConfigChangedWhenChangeConfigValue()
{
    KPIMTextEdit::TextToSpeechConfigWidget textToSpeechConfigWidget;
    addInterface(&textToSpeechConfigWidget);
    QSignalSpy spy(&textToSpeechConfigWidget, SIGNAL(configChanged(bool)));
    QSlider *volume = textToSpeechConfigWidget.findChild<QSlider *>(QStringLiteral("volume"));
    volume->setValue(5);
    QCOMPARE(spy.count(), 1);

    QSlider *rate = textToSpeechConfigWidget.findChild<QSlider *>(QStringLiteral("rate"));
    rate->setValue(5);
    QCOMPARE(spy.count(), 2);

    QSlider *pitch = textToSpeechConfigWidget.findChild<QSlider *>(QStringLiteral("pitch"));
    pitch->setValue(5);
    QCOMPARE(spy.count(), 3);

    QComboBox *language = textToSpeechConfigWidget.findChild<QComboBox *>(QStringLiteral("language"));
    language->blockSignals(true);
    QStringList lst;
    lst << QStringLiteral("foo");
    lst << QStringLiteral("foo");
    lst << QStringLiteral("foo");
    lst << QStringLiteral("foo");
    language->addItems(lst);
    language->blockSignals(false);
    language->setCurrentIndex(3);
    QCOMPARE(spy.count(), 4);
}

QTEST_MAIN(TextToSpeechConfigWidgetTest)
