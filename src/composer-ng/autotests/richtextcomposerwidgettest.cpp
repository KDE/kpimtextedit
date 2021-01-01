/*
   SPDX-FileCopyrightText: 2015-2021 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "richtextcomposerwidgettest.h"
#include "../richtextcomposerwidget.h"
#include <QTest>
#include <kpimtextedit/richtextcomposer.h>

RichTextComposerWidgetTest::RichTextComposerWidgetTest(QObject *parent)
    : QObject(parent)
{
}

RichTextComposerWidgetTest::~RichTextComposerWidgetTest()
{
}

void RichTextComposerWidgetTest::shouldHaveDefaultValue()
{
    KPIMTextEdit::RichTextComposerWidget w;
    auto *composer = w.findChild<KPIMTextEdit::RichTextComposer *>(QStringLiteral("richtextcomposer"));
    QVERIFY(composer);
    QVERIFY(w.richTextComposer());
}

QTEST_MAIN(RichTextComposerWidgetTest)
