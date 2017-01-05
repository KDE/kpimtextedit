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

#include "texttospeechwidgettest.h"
#include "../abstracttexttospeechinterface.h"
#include <qtest.h>
#include "texttospeech/texttospeechwidget.h"
#include <QToolButton>
#include <qtestmouse.h>
#include <QSignalSpy>
#include <QSlider>

Q_DECLARE_METATYPE(KPIMTextEdit::TextToSpeechWidget::State)

TextToSpeechWidgetTest::TextToSpeechWidgetTest(QObject *parent)
    : QObject(parent)
{
    qRegisterMetaType<KPIMTextEdit::TextToSpeechWidget::State>();
}

void TextToSpeechWidgetTest::addInterface(KPIMTextEdit::TextToSpeechWidget *widget)
{
    KPIMTextEdit::AbstractTextToSpeechInterface *interface = new KPIMTextEdit::AbstractTextToSpeechInterface(this);
    widget->setTextToSpeechInterface(interface);
}

void TextToSpeechWidgetTest::shouldHaveDefaultValue()
{
    KPIMTextEdit::TextToSpeechWidget textToSpeechWidget;
    addInterface(&textToSpeechWidget);
    QCOMPARE(textToSpeechWidget.state(), KPIMTextEdit::TextToSpeechWidget::Stop);

    QToolButton *closeButton = textToSpeechWidget.findChild<QToolButton *>(QStringLiteral("close-button"));
    QVERIFY(closeButton);

    QToolButton *stopButton = textToSpeechWidget.findChild<QToolButton *>(QStringLiteral("stopbutton"));
    QVERIFY(stopButton);
    QVERIFY(stopButton->isEnabled());
    QVERIFY(!stopButton->icon().isNull());

    QToolButton *playPauseButton = textToSpeechWidget.findChild<QToolButton *>(QStringLiteral("playpausebutton"));
    QVERIFY(playPauseButton);
    QVERIFY(!playPauseButton->isEnabled());
    QVERIFY(!playPauseButton->icon().isNull());

    QSlider *volume = textToSpeechWidget.findChild<QSlider *>(QStringLiteral("volumeslider"));
    QVERIFY(volume);

    QToolButton *configureButton = textToSpeechWidget.findChild<QToolButton *>(QStringLiteral("configurebutton"));
    QVERIFY(configureButton);
    QVERIFY(!configureButton->icon().isNull());
}

void TextToSpeechWidgetTest::shouldChangeButtonEnableStateWhenChangeState()
{
    KPIMTextEdit::TextToSpeechWidget textToSpeechWidget;
    addInterface(&textToSpeechWidget);
    textToSpeechWidget.setState(KPIMTextEdit::TextToSpeechWidget::Play);

    QToolButton *stopButton = textToSpeechWidget.findChild<QToolButton *>(QStringLiteral("stopbutton"));
    QVERIFY(stopButton->isEnabled());

    QToolButton *playPauseButton = textToSpeechWidget.findChild<QToolButton *>(QStringLiteral("playpausebutton"));
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
    QToolButton *playPauseButton = textToSpeechWidget.findChild<QToolButton *>(QStringLiteral("playpausebutton"));
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

    QToolButton *stopButton = textToSpeechWidget.findChild<QToolButton *>(QStringLiteral("stopbutton"));
    QTest::mouseClick(stopButton, Qt::LeftButton);
    QCOMPARE(textToSpeechWidget.state(), KPIMTextEdit::TextToSpeechWidget::Stop);

}

void TextToSpeechWidgetTest::shouldEmitStateChanged()
{
    KPIMTextEdit::TextToSpeechWidget textToSpeechWidget;
    addInterface(&textToSpeechWidget);
    QSignalSpy spy(&textToSpeechWidget, SIGNAL(stateChanged(KPIMTextEdit::TextToSpeechWidget::State)));
    textToSpeechWidget.setState(KPIMTextEdit::TextToSpeechWidget::Play);
    QCOMPARE(spy.count(), 0);

    QToolButton *stopButton = textToSpeechWidget.findChild<QToolButton *>(QStringLiteral("stopbutton"));

    QToolButton *playPauseButton = textToSpeechWidget.findChild<QToolButton *>(QStringLiteral("playpausebutton"));

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
