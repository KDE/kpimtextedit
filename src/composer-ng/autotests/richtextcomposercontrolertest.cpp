/*
   SPDX-FileCopyrightText: 2015-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "richtextcomposercontrolertest.h"
using namespace Qt::Literals::StringLiterals;

#include "../richtextcomposer.h"
#include "../richtextcomposercontroler.h"
#include <KActionCollection>
#include <QStandardPaths>
#include <QTest>

RichTextComposerControlerTest::RichTextComposerControlerTest(QObject *parent)
    : QObject(parent)
{
    QStandardPaths::setTestModeEnabled(true);
}

RichTextComposerControlerTest::~RichTextComposerControlerTest() = default;

void RichTextComposerControlerTest::shouldAlignLeft()
{
    KPIMTextEdit::RichTextComposer composer;
    auto actionCollection = new KActionCollection(&composer);
    composer.createActions(actionCollection);
    KPIMTextEdit::RichTextComposerControler controller(&composer);
    composer.show();
    QVERIFY(QTest::qWaitForWindowExposed(&composer));
    controller.alignLeft();
    // QVERIFY(controller.richTextComposer()->hasFocus());
    QCOMPARE(controller.richTextComposer()->alignment(), Qt::AlignLeft);
    QVERIFY(controller.richTextComposer()->acceptRichText());
}

void RichTextComposerControlerTest::shouldAlignRight()
{
    KPIMTextEdit::RichTextComposer composer;
    auto actionCollection = new KActionCollection(&composer);
    composer.createActions(actionCollection);
    KPIMTextEdit::RichTextComposerControler controller(&composer);
    composer.show();
    QVERIFY(QTest::qWaitForWindowExposed(&composer));
    controller.alignRight();
    QVERIFY(controller.richTextComposer()->hasFocus());
    QCOMPARE(controller.richTextComposer()->alignment(), Qt::AlignRight);
    QVERIFY(controller.richTextComposer()->acceptRichText());
}

void RichTextComposerControlerTest::shouldAlignJustify()
{
    KPIMTextEdit::RichTextComposer composer;
    auto actionCollection = new KActionCollection(&composer);
    composer.createActions(actionCollection);
    KPIMTextEdit::RichTextComposerControler controller(&composer);
    composer.show();
    QVERIFY(QTest::qWaitForWindowExposed(&composer));
    controller.alignJustify();
    // QVERIFY(controller.richTextComposer()->hasFocus());
    QCOMPARE(controller.richTextComposer()->alignment(), Qt::AlignJustify);
    QVERIFY(controller.richTextComposer()->acceptRichText());
}

void RichTextComposerControlerTest::shouldAlignCenter()
{
    KPIMTextEdit::RichTextComposer composer;
    auto actionCollection = new KActionCollection(&composer);
    composer.createActions(actionCollection);
    KPIMTextEdit::RichTextComposerControler controller(&composer);
    composer.show();
    QVERIFY(QTest::qWaitForWindowExposed(&composer));
    controller.alignCenter();
    // QVERIFY(controller.richTextComposer()->hasFocus());
    QCOMPARE(controller.richTextComposer()->alignment(), Qt::AlignHCenter);
    QVERIFY(controller.richTextComposer()->acceptRichText());
}

void RichTextComposerControlerTest::shouldHaveDefaultValue()
{
    KPIMTextEdit::RichTextComposer composer;
    KPIMTextEdit::RichTextComposerControler controller(&composer);
    QVERIFY(!controller.painterActive());
    QVERIFY(!controller.richTextComposer()->acceptRichText());
}

void RichTextComposerControlerTest::shouldBoldText()
{
    KPIMTextEdit::RichTextComposer composer;
    auto actionCollection = new KActionCollection(&composer);
    composer.createActions(actionCollection);
    KPIMTextEdit::RichTextComposerControler controller(&composer);
    composer.show();
    QVERIFY(QTest::qWaitForWindowExposed(&composer));
    controller.setTextBold(true);
    // QVERIFY(controller.richTextComposer()->hasFocus());
    QVERIFY(controller.richTextComposer()->acceptRichText());
    // TODO text format.
}

void RichTextComposerControlerTest::shouldItalicText()
{
    KPIMTextEdit::RichTextComposer composer;
    auto actionCollection = new KActionCollection(&composer);
    composer.createActions(actionCollection);
    KPIMTextEdit::RichTextComposerControler controller(&composer);
    composer.show();
    QVERIFY(QTest::qWaitForWindowExposed(&composer));
    controller.setTextItalic(true);
    // QVERIFY(controller.richTextComposer()->hasFocus());
    QVERIFY(controller.richTextComposer()->acceptRichText());
    // TODO text format.
}

void RichTextComposerControlerTest::shouldTextUnderline()
{
    KPIMTextEdit::RichTextComposer composer;
    auto actionCollection = new KActionCollection(&composer);
    composer.createActions(actionCollection);
    KPIMTextEdit::RichTextComposerControler controller(&composer);
    composer.show();
    QVERIFY(QTest::qWaitForWindowExposed(&composer));
    controller.setTextUnderline(true);
    // QVERIFY(controller.richTextComposer()->hasFocus());
    QVERIFY(controller.richTextComposer()->acceptRichText());
    // TODO text format.
}

void RichTextComposerControlerTest::shouldTextStrikeOut()
{
    KPIMTextEdit::RichTextComposer composer;
    auto actionCollection = new KActionCollection(&composer);
    composer.createActions(actionCollection);
    KPIMTextEdit::RichTextComposerControler controller(&composer);
    composer.show();
    QVERIFY(QTest::qWaitForWindowExposed(&composer));
    controller.setTextStrikeOut(true);
    // QVERIFY(controller.richTextComposer()->hasFocus());
    QVERIFY(controller.richTextComposer()->acceptRichText());
    // TODO text format.
}

void RichTextComposerControlerTest::shouldFontFamily()
{
    KPIMTextEdit::RichTextComposer composer;
    auto actionCollection = new KActionCollection(&composer);
    composer.createActions(actionCollection);
    KPIMTextEdit::RichTextComposerControler controller(&composer);
    composer.show();
    QVERIFY(QTest::qWaitForWindowExposed(&composer));
    // TODO
}

void RichTextComposerControlerTest::shouldFontSize()
{
    KPIMTextEdit::RichTextComposer composer;
    auto actionCollection = new KActionCollection(&composer);
    composer.createActions(actionCollection);
    KPIMTextEdit::RichTextComposerControler controller(&composer);
    composer.show();
    QVERIFY(QTest::qWaitForWindowExposed(&composer));
    // TODO
}

void RichTextComposerControlerTest::shouldFont()
{
    KPIMTextEdit::RichTextComposer composer;
    auto actionCollection = new KActionCollection(&composer);
    composer.createActions(actionCollection);
    KPIMTextEdit::RichTextComposerControler controller(&composer);
    composer.show();
    QVERIFY(QTest::qWaitForWindowExposed(&composer));
    // TODO
}

void RichTextComposerControlerTest::shouldTextSuperScript()
{
    KPIMTextEdit::RichTextComposer composer;
    auto actionCollection = new KActionCollection(&composer);
    composer.createActions(actionCollection);
    KPIMTextEdit::RichTextComposerControler controller(&composer);
    composer.show();
    QVERIFY(QTest::qWaitForWindowExposed(&composer));
    controller.setTextSuperScript(true);
    QVERIFY(controller.richTextComposer()->hasFocus());
    QVERIFY(controller.richTextComposer()->acceptRichText());
    // TODO
}

void RichTextComposerControlerTest::shouldTextSubScript()
{
    KPIMTextEdit::RichTextComposer composer;
    auto actionCollection = new KActionCollection(&composer);
    composer.createActions(actionCollection);
    KPIMTextEdit::RichTextComposerControler controller(&composer);
    composer.show();
    QVERIFY(QTest::qWaitForWindowExposed(&composer));
    controller.setTextSubScript(true);
    QVERIFY(controller.richTextComposer()->hasFocus());
    QVERIFY(controller.richTextComposer()->acceptRichText());

    // TODO
}

void RichTextComposerControlerTest::shouldRemoveQuote_data()
{
    QTest::addColumn<QString>("input");
    QTest::addColumn<QString>("output");
    QTest::newRow("removequote1") << u">foo\n>bla\n>blo\n"_s << u"foo\nbla\nblo\n"_s;
    QTest::newRow("withoutquote") << u"bli\nblo\bla\n"_s << u"bli\nblo\bla\n"_s;
    QTest::newRow("removequote2") << u">foo\n>bla\n>blo"_s << u"foo\nbla\nblo"_s;
    QTest::newRow("empty") << QString() << QString();
    // Bug David, new line with quote
    QTest::newRow("removequotewithnewline") << u">foo\n>\n>bla\n>blo\n"_s << u"foo\n\nbla\nblo\n"_s;

    QTest::newRow("removequote2") << u">foo\n\nbla\n>blo\nbli"_s << u"foo\n\nbla\nblo\nbli"_s;
    QTest::newRow("removequote2withspace") << u"> foo\n\nbla\n> blo\nbli"_s << u"foo\n\nbla\nblo\nbli"_s;

    QTest::newRow("twoquotes") << u">>foo\n\nbla\n>blo\nbli"_s << u">foo\n\nbla\nblo\nbli"_s;

    QTest::newRow("quoteintext") << u"foo>>\n\nbla\n>blo\nbli"_s << u"foo>>\n\nbla\nblo\nbli"_s;
    QTest::newRow("emptyline") << u"foo>>\n\n\nbla\n>blo\nbli"_s << u"foo>>\n\n\nbla\nblo\nbli"_s;
}

void RichTextComposerControlerTest::shouldRemoveQuote()
{
    QFETCH(QString, input);
    QFETCH(QString, output);
    KPIMTextEdit::RichTextComposer composer;
    auto actionCollection = new KActionCollection(&composer);
    composer.createActions(actionCollection);
    KPIMTextEdit::RichTextComposerControler controller(&composer);
    composer.show();
    QVERIFY(QTest::qWaitForWindowExposed(&composer));

    composer.setPlainText(input);
    controller.slotRemoveQuotes();
    QCOMPARE(composer.toPlainText(), output);
}

void RichTextComposerControlerTest::shouldRemoveQuoteWithSpecificQuote_data()
{
    QTest::addColumn<QString>("input");
    QTest::addColumn<QString>("output");
    QTest::addColumn<QString>("quote");
    QTest::newRow("empty") << QString() << QString() << QString();
    QTest::newRow("empty1") << QString() << QString() << u"QT"_s;
    QTest::newRow("removequote1") << u">foo\n>bla\n>blo\n"_s << u">foo\n>bla\n>blo\n"_s << u"QT"_s;
    QTest::newRow("removequote2") << u"QTfoo\nQTbla\n>blo\n"_s << u"foo\nbla\n>blo\n"_s << u"QT"_s;
    QTest::newRow("removequote3") << u"|foo\n|bla\n>blo\n"_s << u"foo\nbla\n>blo\n"_s << u"|"_s;
}

void RichTextComposerControlerTest::shouldRemoveQuoteWithSpecificQuote()
{
    QFETCH(QString, input);
    QFETCH(QString, output);
    QFETCH(QString, quote);
    KPIMTextEdit::RichTextComposer composer;
    composer.setQuotePrefixName(quote);
    auto actionCollection = new KActionCollection(&composer);
    composer.createActions(actionCollection);
    KPIMTextEdit::RichTextComposerControler controller(&composer);
    composer.show();
    QVERIFY(QTest::qWaitForWindowExposed(&composer));

    composer.setPlainText(input);
    controller.slotRemoveQuotes();
    QCOMPARE(composer.toPlainText(), output);
}

void RichTextComposerControlerTest::shouldAddQuote_data()
{
    //    QTest::addColumn<QString>("input");
    //    QTest::addColumn<QString>("output");
    //    QTest::addColumn<QString>("quote");
    //    QTest::newRow("empty") << QString() << QString() << QString();
    //    QTest::newRow("empty1") << QString() << QString() << u"QT"_s;
    //    QTest::newRow("removequote1") << u">foo\n>bla\n>blo\n"_s << u"QT>foo\nQT>bla\nQT>blo\n"_s << u"QT"_s;
    //    QTest::newRow("removequote2") << u"QTfoo\nQTbla\n>blo\n"_s << u"QTQTfoo\nQTQTbla\n>QTblo\n"_s << u"QT"_s;
    //    QTest::newRow("removequote3") << u"|foo\n|bla\n>blo\n"_s << u"||foo\n||bla\n|>blo\n"_s << u"|"_s;
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
    //    KPIMTextEdit::RichTextComposerControler controller(&composer);
    //    composer.show();
    //    QVERIFY(QTest::qWaitForWindowExposed(&composer));

    //    composer.setPlainText(input);
    //    controller.slotAddQuotes();
    //    QCOMPARE(composer.toPlainText(), QString(output + QChar::ParagraphSeparator));
}

QTEST_MAIN(RichTextComposerControlerTest)

#include "moc_richtextcomposercontrolertest.cpp"
