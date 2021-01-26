/*
   SPDX-FileCopyrightText: 2015-2021 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "richtextexternalcomposertest.h"
#include "../richtextcomposer.h"
#include "../richtextexternalcomposer.h"
#include <QTest>

RichTextExternalComposerTest::RichTextExternalComposerTest(QObject *parent)
    : QObject(parent)
{
}

RichTextExternalComposerTest::~RichTextExternalComposerTest()
{
}

void RichTextExternalComposerTest::shouldHaveDefaultValue()
{
    KPIMTextEdit::RichTextComposer composer;
    KPIMTextEdit::RichTextExternalComposer richTextExternal(&composer);
    QVERIFY(!richTextExternal.useExternalEditor());
    QVERIFY(richTextExternal.externalEditorPath().isEmpty());
    QVERIFY(!richTextExternal.isInProgress());
}

void RichTextExternalComposerTest::shouldChangeUseExternalComposer()
{
    KPIMTextEdit::RichTextComposer composer;
    KPIMTextEdit::RichTextExternalComposer richTextExternal(&composer);
    richTextExternal.setUseExternalEditor(true);
    QVERIFY(richTextExternal.useExternalEditor());
    richTextExternal.setUseExternalEditor(false);
    QVERIFY(!richTextExternal.useExternalEditor());
}

QTEST_MAIN(RichTextExternalComposerTest)
