/*
   SPDX-FileCopyrightText: 2014-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "richtexteditortest.h"
#include "texteditor/richtexteditor/richtexteditor.h"
#include <QTest>

RichTextEditorTest::RichTextEditorTest(QObject *parent)
    : QObject(parent)
{
}

RichTextEditorTest::~RichTextEditorTest() = default;

void RichTextEditorTest::shouldHaveDefaultValue()
{
    KPIMTextEdit::RichTextEditor editor;
    QCOMPARE(editor.spellCheckingSupport(), true);
    QCOMPARE(editor.textToSpeechSupport(), true);
    QCOMPARE(editor.searchSupport(), true);
    QCOMPARE(editor.activateLanguageMenu(), true);
    QCOMPARE(editor.showAutoCorrectButton(), false);
    QCOMPARE(editor.allowTabSupport(), true);
    QCOMPARE(editor.webShortcutSupport(), true);
    QCOMPARE(editor.emojiSupport(), false);
}

void RichTextEditorTest::shouldChangeSpellCheckValue()
{
    KPIMTextEdit::RichTextEditor editor;
    editor.setSpellCheckingSupport(false);
    QCOMPARE(editor.spellCheckingSupport(), false);

    editor.setTextToSpeechSupport(false);
    QCOMPARE(editor.textToSpeechSupport(), false);

    editor.setSearchSupport(false);
    QCOMPARE(editor.searchSupport(), false);

    editor.setAllowTabSupport(false);
    QCOMPARE(editor.allowTabSupport(), false);

    editor.setWebShortcutSupport(false);
    QCOMPARE(editor.webShortcutSupport(), false);

    editor.setSpellCheckingSupport(true);
    QCOMPARE(editor.spellCheckingSupport(), true);

    editor.setTextToSpeechSupport(true);
    QCOMPARE(editor.textToSpeechSupport(), true);

    editor.setSearchSupport(true);
    QCOMPARE(editor.searchSupport(), true);

    editor.setAllowTabSupport(true);
    QCOMPARE(editor.allowTabSupport(), true);

    editor.setWebShortcutSupport(true);
    QCOMPARE(editor.webShortcutSupport(), true);
}

QTEST_MAIN(RichTextEditorTest)

#include "moc_richtexteditortest.cpp"
