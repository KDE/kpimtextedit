/*
   SPDX-FileCopyrightText: 2014-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "plaintexteditortest.h"
#include "texteditor/plaintexteditor/plaintexteditor.h"
#include <QTest>

PlainTextEditorTest::PlainTextEditorTest(QObject *parent)
    : QObject(parent)
{
}

PlainTextEditorTest::~PlainTextEditorTest() = default;

void PlainTextEditorTest::shouldHaveDefaultValue()
{
    KPIMTextEdit::PlainTextEditor editor;
    QCOMPARE(editor.spellCheckingSupport(), true);
    QCOMPARE(editor.textToSpeechSupport(), true);
    QCOMPARE(editor.searchSupport(), true);
    QCOMPARE(editor.webShortcutSupport(), true);
}

void PlainTextEditorTest::shouldChangeSpellCheckValue()
{
    KPIMTextEdit::PlainTextEditor editor;
    editor.setSpellCheckingSupport(false);
    QCOMPARE(editor.spellCheckingSupport(), false);

    editor.setTextToSpeechSupport(false);
    QCOMPARE(editor.textToSpeechSupport(), false);

    editor.setSearchSupport(false);
    QCOMPARE(editor.searchSupport(), false);

    editor.setWebShortcutSupport(false);
    QCOMPARE(editor.webShortcutSupport(), false);

    editor.setSpellCheckingSupport(true);
    QCOMPARE(editor.spellCheckingSupport(), true);

    editor.setTextToSpeechSupport(true);
    QCOMPARE(editor.textToSpeechSupport(), true);

    editor.setSearchSupport(true);
    QCOMPARE(editor.searchSupport(), true);

    editor.setWebShortcutSupport(true);
    QCOMPARE(editor.webShortcutSupport(), true);
}

QTEST_MAIN(PlainTextEditorTest)

#include "moc_plaintexteditortest.cpp"
