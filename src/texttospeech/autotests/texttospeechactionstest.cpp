/*
   SPDX-FileCopyrightText: 2014-2021 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "texttospeechactionstest.h"
#include "../texttospeechactions.h"
#include <QAction>
#include <QSignalSpy>
#include <QTest>

Q_DECLARE_METATYPE(KPIMTextEdit::TextToSpeechWidget::State)

TextToSpeechActionsTest::TextToSpeechActionsTest(QObject *parent)
    : QObject(parent)
{
    qRegisterMetaType<KPIMTextEdit::TextToSpeechWidget::State>();
    QIcon::setThemeName(QStringLiteral("breeze"));
}

TextToSpeechActionsTest::~TextToSpeechActionsTest()
{
}

void TextToSpeechActionsTest::shouldHaveDefaultValue()
{
    KPIMTextEdit::TextToSpeechActions act;
    QVERIFY(act.stopAction());
    QVERIFY(act.playPauseAction());
    QCOMPARE(act.state(), KPIMTextEdit::TextToSpeechWidget::Stop);

    QVERIFY(act.stopAction()->isEnabled());
    QVERIFY(!act.stopAction()->icon().isNull());

    QVERIFY(!act.playPauseAction()->isEnabled());
    QVERIFY(!act.playPauseAction()->icon().isNull());
}

void TextToSpeechActionsTest::shouldChangeButtonEnableStateWhenChangeState()
{
    KPIMTextEdit::TextToSpeechActions act;
    act.setState(KPIMTextEdit::TextToSpeechWidget::Play);

    QVERIFY(act.stopAction()->isEnabled());
    QVERIFY(act.playPauseAction()->isEnabled());

    act.setState(KPIMTextEdit::TextToSpeechWidget::Pause);
    QVERIFY(act.stopAction()->isEnabled());
    QVERIFY(act.playPauseAction()->isEnabled());

    act.setState(KPIMTextEdit::TextToSpeechWidget::Stop);

    QVERIFY(act.stopAction()->isEnabled());
    QVERIFY(!act.playPauseAction()->isEnabled());
}

void TextToSpeechActionsTest::shouldChangeStateWhenClickOnPlayPause()
{
    KPIMTextEdit::TextToSpeechActions act;
    act.setState(KPIMTextEdit::TextToSpeechWidget::Play);
    QCOMPARE(act.state(), KPIMTextEdit::TextToSpeechWidget::Play);

    act.playPauseAction()->trigger();
    QCOMPARE(act.state(), KPIMTextEdit::TextToSpeechWidget::Pause);

    act.playPauseAction()->trigger();
    QCOMPARE(act.state(), KPIMTextEdit::TextToSpeechWidget::Play);
}

void TextToSpeechActionsTest::shouldChangeStateWhenClickOnStop()
{
    KPIMTextEdit::TextToSpeechActions act;
    act.setState(KPIMTextEdit::TextToSpeechWidget::Play);

    act.stopAction()->trigger();
    QCOMPARE(act.state(), KPIMTextEdit::TextToSpeechWidget::Stop);
}

void TextToSpeechActionsTest::shouldEmitStateChanged()
{
    KPIMTextEdit::TextToSpeechActions act;
    act.setState(KPIMTextEdit::TextToSpeechWidget::Play);
    QSignalSpy spy(&act, &KPIMTextEdit::TextToSpeechActions::stateChanged);
    act.setState(KPIMTextEdit::TextToSpeechWidget::Play);
    QCOMPARE(spy.count(), 0);

    act.playPauseAction()->trigger();
    QCOMPARE(spy.count(), 1);
    QCOMPARE(spy.at(0).at(0).value<KPIMTextEdit::TextToSpeechWidget::State>(), KPIMTextEdit::TextToSpeechWidget::Pause);
    act.playPauseAction()->trigger();
    QCOMPARE(spy.count(), 2);
    QCOMPARE(spy.at(1).at(0).value<KPIMTextEdit::TextToSpeechWidget::State>(), KPIMTextEdit::TextToSpeechWidget::Play);
    act.playPauseAction()->trigger();
    QCOMPARE(spy.count(), 3);
    QCOMPARE(spy.at(2).at(0).value<KPIMTextEdit::TextToSpeechWidget::State>(), KPIMTextEdit::TextToSpeechWidget::Pause);
    act.stopAction()->trigger();
    QCOMPARE(spy.count(), 4);
    QCOMPARE(spy.at(3).at(0).value<KPIMTextEdit::TextToSpeechWidget::State>(), KPIMTextEdit::TextToSpeechWidget::Stop);
}

QTEST_MAIN(TextToSpeechActionsTest)
