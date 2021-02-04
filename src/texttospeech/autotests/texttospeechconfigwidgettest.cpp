/*
   SPDX-FileCopyrightText: 2014-2021 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "texttospeechconfigwidgettest.h"
#include "texttospeech/abstracttexttospeechconfiginterface.h"
#include "texttospeech/texttospeechconfigwidget.h"

#include <QComboBox>
#include <QSignalSpy>
#include <QSlider>
#include <QTest>

TextToSpeechConfigWidgetTest::TextToSpeechConfigWidgetTest(QObject *parent)
    : QObject(parent)
{
}

TextToSpeechConfigWidgetTest::~TextToSpeechConfigWidgetTest()
{
}

void TextToSpeechConfigWidgetTest::addInterface(KPIMTextEdit::TextToSpeechConfigWidget *widget)
{
    auto interface = new KPIMTextEdit::AbstractTextToSpeechConfigInterface(this);
    widget->setTextToSpeechConfigInterface(interface);
}

void TextToSpeechConfigWidgetTest::shouldHaveDefaultValue()
{
    KPIMTextEdit::TextToSpeechConfigWidget textToSpeechConfigWidget;
    addInterface(&textToSpeechConfigWidget);
    auto *volume = textToSpeechConfigWidget.findChild<QSlider *>(QStringLiteral("volume"));
    QVERIFY(volume);

    auto *rate = textToSpeechConfigWidget.findChild<QSlider *>(QStringLiteral("rate"));
    QVERIFY(rate);

    auto *pitch = textToSpeechConfigWidget.findChild<QSlider *>(QStringLiteral("pitch"));
    QVERIFY(pitch);

    auto *language = textToSpeechConfigWidget.findChild<QComboBox *>(QStringLiteral("language"));
    QVERIFY(language);
    // FIXME
    // QVERIFY(language->count()>0);

    auto *availableEngine = textToSpeechConfigWidget.findChild<QComboBox *>(QStringLiteral("engine"));
    QVERIFY(availableEngine);

    auto *voice = textToSpeechConfigWidget.findChild<QComboBox *>(QStringLiteral("voice"));
    QVERIFY(voice);
}

void TextToSpeechConfigWidgetTest::shouldEmitConfigChangedWhenChangeConfigValue()
{
    KPIMTextEdit::TextToSpeechConfigWidget textToSpeechConfigWidget;
    addInterface(&textToSpeechConfigWidget);
    QSignalSpy spy(&textToSpeechConfigWidget, &KPIMTextEdit::TextToSpeechConfigWidget::configChanged);
    auto *volume = textToSpeechConfigWidget.findChild<QSlider *>(QStringLiteral("volume"));
    volume->setValue(5);
    QCOMPARE(spy.count(), 1);

    auto *rate = textToSpeechConfigWidget.findChild<QSlider *>(QStringLiteral("rate"));
    rate->setValue(5);
    QCOMPARE(spy.count(), 2);

    auto *pitch = textToSpeechConfigWidget.findChild<QSlider *>(QStringLiteral("pitch"));
    pitch->setValue(5);
    QCOMPARE(spy.count(), 3);

    auto *language = textToSpeechConfigWidget.findChild<QComboBox *>(QStringLiteral("language"));
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
