/*
   SPDX-FileCopyrightText: 2015-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "richtextcomposercontrolertest.h"
#include "../richtextcomposer.h"
#include "../richtextcomposercontroler.h"
#include <KActionCollection>
#include <QStandardPaths>
#include <QTest>

RichTextComposerControlerTest::RichTextComposerControlerTest(QObject *parent)
    : QObject(parent)
{
    QStandardPaths::setTestModeEnabled(true);
    if (qEnvironmentVariableIntValue("KDECI_CANNOT_CREATE_WINDOWS")) {
        QSKIP("KDE CI can't create a window on this platform, skipping some gui tests");
    }
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
    QTest::newRow("removequote1") << QStringLiteral(">foo\n>bla\n>blo\n") << QStringLiteral("foo\nbla\nblo\n");
    QTest::newRow("withoutquote") << QStringLiteral("bli\nblo\bla\n") << QStringLiteral("bli\nblo\bla\n");
    QTest::newRow("removequote2") << QStringLiteral(">foo\n>bla\n>blo") << QStringLiteral("foo\nbla\nblo");
    QTest::newRow("empty") << QString() << QString();
    // Bug David, new line with quote
    QTest::newRow("removequotewithnewline") << QStringLiteral(">foo\n>\n>bla\n>blo\n") << QStringLiteral("foo\n\nbla\nblo\n");

    QTest::newRow("removequote2") << QStringLiteral(">foo\n\nbla\n>blo\nbli") << QStringLiteral("foo\n\nbla\nblo\nbli");
    QTest::newRow("removequote2withspace") << QStringLiteral("> foo\n\nbla\n> blo\nbli") << QStringLiteral("foo\n\nbla\nblo\nbli");

    QTest::newRow("twoquotes") << QStringLiteral(">>foo\n\nbla\n>blo\nbli") << QStringLiteral(">foo\n\nbla\nblo\nbli");

    QTest::newRow("quoteintext") << QStringLiteral("foo>>\n\nbla\n>blo\nbli") << QStringLiteral("foo>>\n\nbla\nblo\nbli");
    QTest::newRow("emptyline") << QStringLiteral("foo>>\n\n\nbla\n>blo\nbli") << QStringLiteral("foo>>\n\n\nbla\nblo\nbli");
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
    //    KPIMTextEdit::RichTextComposerControler controller(&composer);
    //    composer.show();
    //    QVERIFY(QTest::qWaitForWindowExposed(&composer));

    //    composer.setPlainText(input);
    //    controller.slotAddQuotes();
    //    QCOMPARE(composer.toPlainText(), QString(output + QChar::ParagraphSeparator));
}

QTEST_MAIN(RichTextComposerControlerTest)

#include "moc_richtextcomposercontrolertest.cpp"
