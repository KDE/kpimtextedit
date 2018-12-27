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

#include "richtexteditortest.h"
#include "texteditor/richtexteditor/richtexteditor.h"
#include <qtest.h>

RichTextEditorTest::RichTextEditorTest(QObject *parent)
    : QObject(parent)
{
}

RichTextEditorTest::~RichTextEditorTest()
{
}

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
