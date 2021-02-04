/*
  SPDX-FileCopyrightText: 2015-2021 Laurent Montel <montel@kde.org>

  SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "editorutiltest.h"
#include "../src/editorutil.h"
#include <QTest>
#include <QTextDocument>

EditorUtilTest::EditorUtilTest(QObject *parent)
    : QObject(parent)
{
}

EditorUtilTest::~EditorUtilTest()
{
}

void EditorUtilTest::testUpperCase_data()
{
    QTest::addColumn<QString>("input");
    QTest::addColumn<QString>("result");
    QTest::addColumn<int>("startcursorposition");
    QTest::addColumn<int>("endcursorposition");
    QTest::newRow("onelinewithoutselection") << QStringLiteral("foo") << QStringLiteral("foo") << -1 << -1;
    QTest::newRow("onelinewithselection") << QStringLiteral("foo") << QStringLiteral("FOO") << 0 << 3;
    QTest::newRow("onelinewithselectionuppercase") << QStringLiteral("FOO") << QStringLiteral("FOO") << 0 << 3;
}

void EditorUtilTest::testUpperCase()
{
    QFETCH(QString, input);
    QFETCH(QString, result);
    QFETCH(int, startcursorposition);
    QFETCH(int, endcursorposition);
    auto document = new QTextDocument(this);
    document->setPlainText(input);
    QTextCursor textCursor(document);
    if (startcursorposition != -1 && endcursorposition != -1) {
        textCursor.setPosition(startcursorposition);
        textCursor.movePosition(QTextCursor::NextCharacter, QTextCursor::KeepAnchor, endcursorposition - startcursorposition);
    }
    KPIMTextEdit::EditorUtil editorUtil;
    editorUtil.upperCase(textCursor);
    QCOMPARE(textCursor.document()->toPlainText(), result);
    delete document;
}

void EditorUtilTest::testLowerCase_data()
{
    QTest::addColumn<QString>("input");
    QTest::addColumn<QString>("result");
    QTest::addColumn<int>("startcursorposition");
    QTest::addColumn<int>("endcursorposition");
    QTest::newRow("onelinewithoutselection") << QStringLiteral("foo") << QStringLiteral("foo") << -1 << -1;
    QTest::newRow("onelinewithselection") << QStringLiteral("foo") << QStringLiteral("foo") << 0 << 3;
    QTest::newRow("onelinewithselectionuppercase") << QStringLiteral("FOO") << QStringLiteral("foo") << 0 << 3;
}

void EditorUtilTest::testLowerCase()
{
    QFETCH(QString, input);
    QFETCH(QString, result);
    QFETCH(int, startcursorposition);
    QFETCH(int, endcursorposition);
    auto document = new QTextDocument(this);
    document->setPlainText(input);
    QTextCursor textCursor(document);
    if (startcursorposition != -1 && endcursorposition != -1) {
        textCursor.setPosition(startcursorposition);
        textCursor.movePosition(QTextCursor::NextCharacter, QTextCursor::KeepAnchor, endcursorposition - startcursorposition);
    }
    KPIMTextEdit::EditorUtil editorUtil;
    editorUtil.lowerCase(textCursor);
    QCOMPARE(textCursor.document()->toPlainText(), result);
    delete document;
}

void EditorUtilTest::testSentenceCase_data()
{
    QTest::addColumn<QString>("input");
    QTest::addColumn<QString>("result");
    QTest::addColumn<int>("startcursorposition");
    QTest::addColumn<int>("endcursorposition");
    QTest::newRow("onelinewithoutselection") << QStringLiteral("foo") << QStringLiteral("foo") << -1 << -1;
    QTest::newRow("onelinewithselection") << QStringLiteral("foo") << QStringLiteral("Foo") << 0 << 3;
    QTest::newRow("bigline") << QStringLiteral("foo bla foo.\nddd") << QStringLiteral("Foo bla foo.\nDdd") << 0 << 16;
}

void EditorUtilTest::testSentenceCase()
{
    QFETCH(QString, input);
    QFETCH(QString, result);
    QFETCH(int, startcursorposition);
    QFETCH(int, endcursorposition);
    auto document = new QTextDocument(this);
    document->setPlainText(input);
    QTextCursor textCursor(document);
    if (startcursorposition != -1 && endcursorposition != -1) {
        textCursor.setPosition(startcursorposition);
        textCursor.movePosition(QTextCursor::NextCharacter, QTextCursor::KeepAnchor, endcursorposition - startcursorposition);
    }
    KPIMTextEdit::EditorUtil editorUtil;
    editorUtil.sentenceCase(textCursor);
    QCOMPARE(textCursor.document()->toPlainText(), result);
    delete document;
}

void EditorUtilTest::testReverseCase_data()
{
    QTest::addColumn<QString>("input");
    QTest::addColumn<QString>("result");
    QTest::addColumn<int>("startcursorposition");
    QTest::addColumn<int>("endcursorposition");
    QTest::newRow("onelinewithoutselection") << QStringLiteral("foo") << QStringLiteral("foo") << -1 << -1;
    QTest::newRow("onelinewithselection") << QStringLiteral("foo") << QStringLiteral("FOO") << 0 << 3;
    QTest::newRow("reverseCase") << QStringLiteral("fOo bla\tfOO") << QStringLiteral("FoO BLA\tFoo") << 0 << 12;
    QTest::newRow("notallineselected") << QStringLiteral("fOo bla\tfOO") << QStringLiteral("FoO bla\tfOO") << 0 << 3;
}

void EditorUtilTest::testReverseCase()
{
    QFETCH(QString, input);
    QFETCH(QString, result);
    QFETCH(int, startcursorposition);
    QFETCH(int, endcursorposition);

    auto document = new QTextDocument(this);
    document->setPlainText(input);
    QTextCursor textCursor(document);
    if (startcursorposition != -1 && endcursorposition != -1) {
        textCursor.setPosition(startcursorposition);
        textCursor.movePosition(QTextCursor::NextCharacter, QTextCursor::KeepAnchor, endcursorposition - startcursorposition);
    }
    KPIMTextEdit::EditorUtil editorUtil;
    editorUtil.reverseCase(textCursor);
    QCOMPARE(textCursor.document()->toPlainText(), result);
    delete document;
}

QTEST_MAIN(EditorUtilTest)
