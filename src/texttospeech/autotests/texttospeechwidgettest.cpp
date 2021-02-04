/*
   SPDX-FileCopyrightText: 2014-2021 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "texttospeechwidgettest.h"
#include "../abstracttexttospeechinterface.h"
#include "texttospeech/texttospeechwidget.h"
#include <QSignalSpy>
#include <QSlider>
#include <QTest>
#include <QToolButton>
#include <qtestmouse.h>

Q_DECLARE_METATYPE(KPIMTextEdit::TextToSpeechWidget::State)

TextToSpeechWidgetTest::TextToSpeechWidgetTest(QObject *parent)
    : QObject(parent)
{
    qRegisterMetaType<KPIMTextEdit::TextToSpeechWidget::State>();
    QIcon::setThemeName(QStringLiteral("breeze"));
}

void TextToSpeechWidgetTest::addInterface(KPIMTextEdit::TextToSpeechWidget *widget)
{
    auto interface = new KPIMTextEdit::AbstractTextToSpeechInterface(this);
    widget->setTextToSpeechInterface(interface);
}

void TextToSpeechWidgetTest::shouldHaveDefaultValue()
{
    KPIMTextEdit::TextToSpeechWidget textToSpeechWidget;
    addInterface(&textToSpeechWidget);
    QCOMPARE(textToSpeechWidget.state(), KPIMTextEdit::TextToSpeechWidget::Stop);

    auto *closeButton = textToSpeechWidget.findChild<QToolButton *>(QStringLiteral("close-button"));
    QVERIFY(closeButton);

    auto *stopButton = textToSpeechWidget.findChild<QToolButton *>(QStringLiteral("stopbutton"));
    QVERIFY(stopButton);
    QVERIFY(stopButton->isEnabled());
    QVERIFY(!stopButton->icon().isNull());

    auto *playPauseButton = textToSpeechWidget.findChild<QToolButton *>(QStringLiteral("playpausebutton"));
    QVERIFY(playPauseButton);
    QVERIFY(!playPauseButton->isEnabled());
    QVERIFY(!playPauseButton->icon().isNull());

    auto *volume = textToSpeechWidget.findChild<QSlider *>(QStringLiteral("volumeslider"));
    QVERIFY(volume);

    auto *configureButton = textToSpeechWidget.findChild<QToolButton *>(QStringLiteral("configurebutton"));
    QVERIFY(configureButton);
    QVERIFY(!configureButton->icon().isNull());
}

void TextToSpeechWidgetTest::shouldChangeButtonEnableStateWhenChangeState()
{
    KPIMTextEdit::TextToSpeechWidget textToSpeechWidget;
    addInterface(&textToSpeechWidget);
    textToSpeechWidget.setState(KPIMTextEdit::TextToSpeechWidget::Play);

    auto *stopButton = textToSpeechWidget.findChild<QToolButton *>(QStringLiteral("stopbutton"));
    QVERIFY(stopButton->isEnabled());

    auto *playPauseButton = textToSpeechWidget.findChild<QToolButton *>(QStringLiteral("playpausebutton"));
    QVERIFY(playPauseButton->isEnabled());

    textToSpeechWidget.setState(KPIMTextEdit::TextToSpeechWidget::Pause);
    QVERIFY(stopButton->isEnabled());
    QVERIFY(playPauseButton->isEnabled());

    textToSpeechWidget.setState(KPIMTextEdit::TextToSpeechWidget::Stop);
    QVERIFY(stopButton->isEnabled());
    QVERIFY(!playPauseButton->isEnabled());
}

void TextToSpeechWidgetTest::shouldChangeStateWhenClickOnPlayPause()
{
    KPIMTextEdit::TextToSpeechWidget textToSpeechWidget;
    addInterface(&textToSpeechWidget);

    textToSpeechWidget.setState(KPIMTextEdit::TextToSpeechWidget::Play);
    auto *playPauseButton = textToSpeechWidget.findChild<QToolButton *>(QStringLiteral("playpausebutton"));
    QCOMPARE(textToSpeechWidget.state(), KPIMTextEdit::TextToSpeechWidget::Play);

    QTest::mouseClick(playPauseButton, Qt::LeftButton);
    QCOMPARE(textToSpeechWidget.state(), KPIMTextEdit::TextToSpeechWidget::Pause);

    QTest::mouseClick(playPauseButton, Qt::LeftButton);
    QCOMPARE(textToSpeechWidget.state(), KPIMTextEdit::TextToSpeechWidget::Play);
}

void TextToSpeechWidgetTest::shouldChangeStateWhenClickOnStop()
{
    KPIMTextEdit::TextToSpeechWidget textToSpeechWidget;
    addInterface(&textToSpeechWidget);
    textToSpeechWidget.setState(KPIMTextEdit::TextToSpeechWidget::Play);

    auto *stopButton = textToSpeechWidget.findChild<QToolButton *>(QStringLiteral("stopbutton"));
    QTest::mouseClick(stopButton, Qt::LeftButton);
    QCOMPARE(textToSpeechWidget.state(), KPIMTextEdit::TextToSpeechWidget::Stop);
}

void TextToSpeechWidgetTest::shouldEmitStateChanged()
{
    KPIMTextEdit::TextToSpeechWidget textToSpeechWidget;
    addInterface(&textToSpeechWidget);
    QSignalSpy spy(&textToSpeechWidget, &KPIMTextEdit::TextToSpeechWidget::stateChanged);
    textToSpeechWidget.setState(KPIMTextEdit::TextToSpeechWidget::Play);
    QCOMPARE(spy.count(), 0);

    auto *stopButton = textToSpeechWidget.findChild<QToolButton *>(QStringLiteral("stopbutton"));

    auto *playPauseButton = textToSpeechWidget.findChild<QToolButton *>(QStringLiteral("playpausebutton"));

    QTest::mouseClick(playPauseButton, Qt::LeftButton);
    QCOMPARE(spy.count(), 1);
    QCOMPARE(spy.at(0).at(0).value<KPIMTextEdit::TextToSpeechWidget::State>(), KPIMTextEdit::TextToSpeechWidget::Pause);
    QTest::mouseClick(playPauseButton, Qt::LeftButton);
    QCOMPARE(spy.count(), 2);
    QCOMPARE(spy.at(1).at(0).value<KPIMTextEdit::TextToSpeechWidget::State>(), KPIMTextEdit::TextToSpeechWidget::Play);
    QTest::mouseClick(playPauseButton, Qt::LeftButton);
    QCOMPARE(spy.count(), 3);
    QCOMPARE(spy.at(2).at(0).value<KPIMTextEdit::TextToSpeechWidget::State>(), KPIMTextEdit::TextToSpeechWidget::Pause);
    QTest::mouseClick(stopButton, Qt::LeftButton);
    QCOMPARE(spy.count(), 4);
    QCOMPARE(spy.at(3).at(0).value<KPIMTextEdit::TextToSpeechWidget::State>(), KPIMTextEdit::TextToSpeechWidget::Stop);
}

QTEST_MAIN(TextToSpeechWidgetTest)
