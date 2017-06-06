/*
   Copyright (c) 2015-2017 Laurent Montel <montel@kde.org>

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

#include "richtextcomposercontrolertest.h"
#include "../richtextcomposercontroler.h"
#include "../richtextcomposer.h"
#include <KActionCollection>
#include <qtest.h>

RichTextComposerControlerTest::RichTextComposerControlerTest(QObject *parent)
    : QObject(parent)
{
}

RichTextComposerControlerTest::~RichTextComposerControlerTest()
{
}

void RichTextComposerControlerTest::shouldAlignLeft()
{
    KPIMTextEdit::RichTextComposer composer;
    KActionCollection *actionCollection = new KActionCollection(&composer);
    composer.createActions(actionCollection);
    KPIMTextEdit::RichTextComposerControler controler(&composer);
    composer.show();
    QTest::qWaitForWindowExposed(&composer);
    controler.alignLeft();
    QVERIFY(controler.richTextComposer()->hasFocus());
    QCOMPARE(controler.richTextComposer()->alignment(), Qt::AlignLeft);
    QVERIFY(controler.richTextComposer()->acceptRichText());
}

void RichTextComposerControlerTest::shouldAlignRight()
{
    KPIMTextEdit::RichTextComposer composer;
    KActionCollection *actionCollection = new KActionCollection(&composer);
    composer.createActions(actionCollection);
    KPIMTextEdit::RichTextComposerControler controler(&composer);
    composer.show();
    QTest::qWaitForWindowExposed(&composer);
    controler.alignRight();
    QVERIFY(controler.richTextComposer()->hasFocus());
    QCOMPARE(controler.richTextComposer()->alignment(), Qt::AlignRight);
    QVERIFY(controler.richTextComposer()->acceptRichText());
}

void RichTextComposerControlerTest::shouldAlignJustify()
{
    KPIMTextEdit::RichTextComposer composer;
    KActionCollection *actionCollection = new KActionCollection(&composer);
    composer.createActions(actionCollection);
    KPIMTextEdit::RichTextComposerControler controler(&composer);
    composer.show();
    QTest::qWaitForWindowExposed(&composer);
    controler.alignJustify();
    QVERIFY(controler.richTextComposer()->hasFocus());
    QCOMPARE(controler.richTextComposer()->alignment(), Qt::AlignJustify);
    QVERIFY(controler.richTextComposer()->acceptRichText());
}

void RichTextComposerControlerTest::shouldAlignCenter()
{
    KPIMTextEdit::RichTextComposer composer;
    KActionCollection *actionCollection = new KActionCollection(&composer);
    composer.createActions(actionCollection);
    KPIMTextEdit::RichTextComposerControler controler(&composer);
    composer.show();
    QTest::qWaitForWindowExposed(&composer);
    controler.alignCenter();
    QVERIFY(controler.richTextComposer()->hasFocus());
    QCOMPARE(controler.richTextComposer()->alignment(), Qt::AlignHCenter);
    QVERIFY(controler.richTextComposer()->acceptRichText());
}

void RichTextComposerControlerTest::shouldHaveDefaultValue()
{
    KPIMTextEdit::RichTextComposer composer;
    KPIMTextEdit::RichTextComposerControler controler(&composer);
    QVERIFY(!controler.painterActive());
    QVERIFY(!controler.richTextComposer()->acceptRichText());
}


void RichTextComposerControlerTest::shouldBoldText()
{
    KPIMTextEdit::RichTextComposer composer;
    KActionCollection *actionCollection = new KActionCollection(&composer);
    composer.createActions(actionCollection);
    KPIMTextEdit::RichTextComposerControler controler(&composer);
    composer.show();
    QTest::qWaitForWindowExposed(&composer);
    controler.setTextBold(true);
    QVERIFY(controler.richTextComposer()->hasFocus());
    QVERIFY(controler.richTextComposer()->acceptRichText());
    //TODO text format.
}

void RichTextComposerControlerTest::shouldItalicText()
{
    KPIMTextEdit::RichTextComposer composer;
    KActionCollection *actionCollection = new KActionCollection(&composer);
    composer.createActions(actionCollection);
    KPIMTextEdit::RichTextComposerControler controler(&composer);
    composer.show();
    QTest::qWaitForWindowExposed(&composer);
    controler.setTextItalic(true);
    QVERIFY(controler.richTextComposer()->hasFocus());
    QVERIFY(controler.richTextComposer()->acceptRichText());
    //TODO text format.
}

void RichTextComposerControlerTest::shouldTextUnderline()
{
    KPIMTextEdit::RichTextComposer composer;
    KActionCollection *actionCollection = new KActionCollection(&composer);
    composer.createActions(actionCollection);
    KPIMTextEdit::RichTextComposerControler controler(&composer);
    composer.show();
    QTest::qWaitForWindowExposed(&composer);
    controler.setTextUnderline(true);
    QVERIFY(controler.richTextComposer()->hasFocus());
    QVERIFY(controler.richTextComposer()->acceptRichText());
    //TODO text format.
}

void RichTextComposerControlerTest::shouldTextStrikeOut()
{
    KPIMTextEdit::RichTextComposer composer;
    KActionCollection *actionCollection = new KActionCollection(&composer);
    composer.createActions(actionCollection);
    KPIMTextEdit::RichTextComposerControler controler(&composer);
    composer.show();
    QTest::qWaitForWindowExposed(&composer);
    controler.setTextStrikeOut(true);
    QVERIFY(controler.richTextComposer()->hasFocus());
    QVERIFY(controler.richTextComposer()->acceptRichText());
    //TODO text format.
}

void RichTextComposerControlerTest::shouldFontFamily()
{
    KPIMTextEdit::RichTextComposer composer;
    KActionCollection *actionCollection = new KActionCollection(&composer);
    composer.createActions(actionCollection);
    KPIMTextEdit::RichTextComposerControler controler(&composer);
    composer.show();
    QTest::qWaitForWindowExposed(&composer);
    //TODO
}

void RichTextComposerControlerTest::shouldFontSize()
{
    KPIMTextEdit::RichTextComposer composer;
    KActionCollection *actionCollection = new KActionCollection(&composer);
    composer.createActions(actionCollection);
    KPIMTextEdit::RichTextComposerControler controler(&composer);
    composer.show();
    QTest::qWaitForWindowExposed(&composer);
    //TODO
}

void RichTextComposerControlerTest::shouldFont()
{
    KPIMTextEdit::RichTextComposer composer;
    KActionCollection *actionCollection = new KActionCollection(&composer);
    composer.createActions(actionCollection);
    KPIMTextEdit::RichTextComposerControler controler(&composer);
    composer.show();
    QTest::qWaitForWindowExposed(&composer);
    //TODO
}

void RichTextComposerControlerTest::shouldTextSuperScript()
{
    KPIMTextEdit::RichTextComposer composer;
    KActionCollection *actionCollection = new KActionCollection(&composer);
    composer.createActions(actionCollection);
    KPIMTextEdit::RichTextComposerControler controler(&composer);
    composer.show();
    QTest::qWaitForWindowExposed(&composer);
    controler.setTextSuperScript(true);
    QVERIFY(controler.richTextComposer()->hasFocus());
    QVERIFY(controler.richTextComposer()->acceptRichText());
    //TODO
}

void RichTextComposerControlerTest::shouldTextSubScript()
{
    KPIMTextEdit::RichTextComposer composer;
    KActionCollection *actionCollection = new KActionCollection(&composer);
    composer.createActions(actionCollection);
    KPIMTextEdit::RichTextComposerControler controler(&composer);
    composer.show();
    QTest::qWaitForWindowExposed(&composer);
    controler.setTextSubScript(true);
    QVERIFY(controler.richTextComposer()->hasFocus());
    QVERIFY(controler.richTextComposer()->acceptRichText());

    //TODO
}

void RichTextComposerControlerTest::shouldRemoveQuote_data()
{
    QTest::addColumn<QString>("input");
    QTest::addColumn<QString>("output");
    QTest::newRow("removequote1") << QStringLiteral(">foo\n>bla\n>blo\n") << QStringLiteral("foo\nbla\nblo\n");
    QTest::newRow("withoutquote") << QStringLiteral("bli\nblo\bla\n") << QStringLiteral("bli\nblo\bla\n");
    QTest::newRow("removequote2") << QStringLiteral(">foo\n>bla\n>blo") << QStringLiteral("foo\nbla\nblo");
    QTest::newRow("empty") << QString() << QString();
    //Bug David, new line with quote
    QTest::newRow("removequotewithnewline") << QStringLiteral(">foo\n>\n>bla\n>blo\n") << QStringLiteral("foo\n\nbla\nblo\n");

    QTest::newRow("removequote2") << QStringLiteral(">foo\n\nbla\n>blo\nbli") << QStringLiteral("foo\n\nbla\nblo\nbli");

    QTest::newRow("twoquotes") << QStringLiteral(">>foo\n\nbla\n>blo\nbli") << QStringLiteral(">foo\n\nbla\nblo\nbli");

    QTest::newRow("quoteintext") << QStringLiteral("foo>>\n\nbla\n>blo\nbli") << QStringLiteral("foo>>\n\nbla\nblo\nbli");
    QTest::newRow("emptyline") << QStringLiteral("foo>>\n\n\nbla\n>blo\nbli") << QStringLiteral("foo>>\n\n\nbla\nblo\nbli");
}

void RichTextComposerControlerTest::shouldRemoveQuote()
{
    QFETCH(QString, input);
    QFETCH(QString, output);
    KPIMTextEdit::RichTextComposer composer;
    KActionCollection *actionCollection = new KActionCollection(&composer);
    composer.createActions(actionCollection);
    KPIMTextEdit::RichTextComposerControler controler(&composer);
    composer.show();
    QTest::qWaitForWindowExposed(&composer);

    composer.setPlainText(input);
    controler.slotRemoveQuotes();
    QCOMPARE(composer.toPlainText(), output);
}

void RichTextComposerControlerTest::shouldRemoveQuoteWithSpecificQuote_data()
{
    QTest::addColumn<QString>("input");
    QTest::addColumn<QString>("output");
    QTest::addColumn<QString>("quote");
    QTest::newRow("empty") << QString() << QString() << QString();
    QTest::newRow("empty1") << QString() << QString() << QStringLiteral("QT");
    QTest::newRow("removequote1") << QStringLiteral(">foo\n>bla\n>blo\n") << QStringLiteral(">foo\n>bla\n>blo\n") << QStringLiteral("QT");
    QTest::newRow("removequote2") << QStringLiteral("QTfoo\nQTbla\n>blo\n") << QStringLiteral("foo\nbla\n>blo\n") << QStringLiteral("QT");
    QTest::newRow("removequote3") << QStringLiteral("|foo\n|bla\n>blo\n") << QStringLiteral("foo\nbla\n>blo\n") << QStringLiteral("|");
}

void RichTextComposerControlerTest::shouldRemoveQuoteWithSpecificQuote()
{
    QFETCH(QString, input);
    QFETCH(QString, output);
    QFETCH(QString, quote);
    KPIMTextEdit::RichTextComposer composer;
    composer.setQuotePrefixName(quote);
    KActionCollection *actionCollection = new KActionCollection(&composer);
    composer.createActions(actionCollection);
    KPIMTextEdit::RichTextComposerControler controler(&composer);
    composer.show();
    QTest::qWaitForWindowExposed(&composer);

    composer.setPlainText(input);
    controler.slotRemoveQuotes();
    QCOMPARE(composer.toPlainText(), output);
}

void RichTextComposerControlerTest::shouldAddQuote_data()
{
//    QTest::addColumn<QString>("input");
//    QTest::addColumn<QString>("output");
//    QTest::addColumn<QString>("quote");
//    QTest::newRow("empty") << QString() << QString() << QString();
//    QTest::newRow("empty1") << QString() << QString() << QStringLiteral("QT");
//    QTest::newRow("removequote1") << QStringLiteral(">foo\n>bla\n>blo\n") << QStringLiteral("QT>foo\nQT>bla\nQT>blo\n") << QStringLiteral("QT");
//    QTest::newRow("removequote2") << QStringLiteral("QTfoo\nQTbla\n>blo\n") << QStringLiteral("QTQTfoo\nQTQTbla\n>QTblo\n") << QStringLiteral("QT");
//    QTest::newRow("removequote3") << QStringLiteral("|foo\n|bla\n>blo\n") << QStringLiteral("||foo\n||bla\n|>blo\n") << QStringLiteral("|");

}

void RichTextComposerControlerTest::shouldAddQuote()
{
//    QFETCH(QString, input);
//    QFETCH(QString, output);
//    QFETCH(QString, quote);
//    KPIMTextEdit::RichTextComposer composer;
//    composer.setQuotePrefixName(quote);
//    KActionCollection *actionCollection = new KActionCollection(&composer);
//    composer.createActions(actionCollection);
//    KPIMTextEdit::RichTextComposerControler controler(&composer);
//    composer.show();
//    QTest::qWaitForWindowExposed(&composer);

//    composer.setPlainText(input);
//    controler.slotAddQuotes();
//    QCOMPARE(composer.toPlainText(), QString(output + QChar::ParagraphSeparator));
}


QTEST_MAIN(RichTextComposerControlerTest)
