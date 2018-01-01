/*
   Copyright (C) 2014-2018 Laurent Montel <montel@kde.org>

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

#include "plaintexteditortest.h"
#include "texteditor/plaintexteditor/plaintexteditor.h"
#include <qtest.h>

PlainTextEditorTest::PlainTextEditorTest(QObject *parent)
    : QObject(parent)
{
}

PlainTextEditorTest::~PlainTextEditorTest()
{
}

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
