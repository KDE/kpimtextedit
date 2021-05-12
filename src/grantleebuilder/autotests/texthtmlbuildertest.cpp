/*
   SPDX-FileCopyrightText: 2020-2021 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "texthtmlbuildertest.h"
#include "grantleebuilder/markupdirector.h"
#include "grantleebuilder/texthtmlbuilder.h"
#include <QRegularExpression>
#include <QTest>
#include <QTextDocument>
QTEST_MAIN(TextHTMLBuilderTest)
TextHTMLBuilderTest::TextHTMLBuilderTest(QObject *parent)
    : QObject(parent)
{
}

void TextHTMLBuilderTest::testHtmlWithTab()
{
    auto doc = new QTextDocument(this);
    QTextCursor cursor(doc);
    cursor.movePosition(QTextCursor::Start);
    cursor.insertText(QStringLiteral("\n"));
    cursor.insertText(QStringLiteral("\t"));
    cursor.insertText(QStringLiteral("foo"));

    auto hb = new KPIMTextEdit::TextHTMLBuilder();
    auto md = new KPIMTextEdit::MarkupDirector(hb);
    md->processDocument(doc);
    auto result = hb->getResult();

    auto regex =
        QRegularExpression(QStringLiteral("^<br /><p style=\"margin-top:0;margin-bottom:0;margin-left:0;margin-right:0;\">&nbsp;&nbsp;&nbsp; foo</p>\\n$"));

    const bool regexpHasResult = regex.match(result).hasMatch();
    if (!regexpHasResult) {
        qDebug() << " result found " << result;
    }
    QVERIFY(regexpHasResult);
    delete md;
    delete hb;
    delete doc;
}

void TextHTMLBuilderTest::testHtmlText_data()
{
    QTest::addColumn<QString>("text");
    QTest::addColumn<QString>("regexpText");
    QTest::addColumn<bool>("htmlFormat");
    QTest::newRow("link")
        << QStringLiteral("A <a href=\"http://www.kde.org\">link</a> to KDE.")
        << QStringLiteral(
               "^<p style=\"margin-top:0;margin-bottom:0;margin-left:0;margin-right:0;\">A <a href=\"http://www.kde.org\">link</a>&nbsp;to KDE.</p>\\n$")
        << true;
    QTest::newRow("text with espace")
        << QStringLiteral("         foo")
        << QStringLiteral(
               "^<p style=\"margin-top:0;margin-bottom:0;margin-left:0;margin-right:0;\">&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; foo</p>\\n$")
        << false;
    QTest::newRow("text with espace at begin") << QStringLiteral(" foo")
                                               << QStringLiteral("^<p style=\"margin-top:0;margin-bottom:0;margin-left:0;margin-right:0;\">&nbsp;foo</p>\\n$")
                                               << false;
}

void TextHTMLBuilderTest::testHtmlText()
{
    QFETCH(QString, text);
    QFETCH(QString, regexpText);
    QFETCH(bool, htmlFormat);

    auto doc = new QTextDocument();
    if (htmlFormat) {
        doc->setHtml(text);
    } else {
        doc->setPlainText(text);
    }

    auto hb = new KPIMTextEdit::TextHTMLBuilder();
    auto md = new KPIMTextEdit::MarkupDirector(hb);
    md->processDocument(doc);
    const auto result = hb->getResult();

    const QRegularExpression regex(regexpText);

    const bool regexpHasResult = regex.match(result).hasMatch();
    if (!regexpHasResult) {
        qDebug() << " result found " << result;
    }
    QVERIFY(regexpHasResult);
    delete md;
    delete hb;
    delete doc;
}

void TextHTMLBuilderTest::testSingleFormat()
{
    auto doc = new QTextDocument();

    // One format
    doc->setHtml(QStringLiteral("This <b>text</b> is bold."));

    auto hb = new KPIMTextEdit::TextHTMLBuilder();
    auto md = new KPIMTextEdit::MarkupDirector(hb);
    md->processDocument(doc);
    auto result = hb->getResult();
    QRegularExpression regex(
        QStringLiteral("^<p style=\"margin-top:0;margin-bottom:0;margin-left:0;margin-right:0;\">This <strong>text</strong>&nbsp;is bold.</p>\\n$"));

    QVERIFY(regex.match(result).hasMatch());
    delete md;
    delete hb;
    delete doc;
}

void TextHTMLBuilderTest::testDoubleFormat()
{
    auto doc = new QTextDocument();

    // One format
    doc->setHtml(QStringLiteral("Some <b><i>formatted</i></b> text."));

    auto hb = new KPIMTextEdit::TextHTMLBuilder();
    auto md = new KPIMTextEdit::MarkupDirector(hb);
    md->processDocument(doc);
    auto result = hb->getResult();
    QRegularExpression regex(
        QStringLiteral("^<p style=\"margin-top:0;margin-bottom:0;margin-left:0;margin-right:0;\">Some "
                       "(<strong><em>|<em><strong>)formatted(</em></strong>|</"
                       "strong></em>)&nbsp;text.</p>\\n$"));

    QVERIFY(regex.match(result).hasMatch());
    delete md;
    delete hb;
    delete doc;
}

void TextHTMLBuilderTest::testAnchor()
{
    auto doc = new QTextDocument();
    doc->setHtml(QStringLiteral("A <a href=\"http://www.kde.org\">link</a> to KDE."));

    auto hb = new KPIMTextEdit::TextHTMLBuilder();
    auto md = new KPIMTextEdit::MarkupDirector(hb);
    md->processDocument(doc);
    auto result = hb->getResult();

    QRegularExpression regex(QStringLiteral(
        "^<p style=\"margin-top:0;margin-bottom:0;margin-left:0;margin-right:0;\">A <a href=\"http://www.kde.org\">link</a>&nbsp;to KDE.</p>\\n$"));

    QVERIFY(regex.match(result).hasMatch());
    delete md;
    delete hb;
    delete doc;
}

void TextHTMLBuilderTest::testAnchorWithFormattedContent()
{
    auto doc = new QTextDocument();
    doc->setHtml(QStringLiteral("A <a href=\"http://www.kde.org\"><b>formatted</b> link</a> to KDE."));

    auto hb = new KPIMTextEdit::TextHTMLBuilder();
    auto md = new KPIMTextEdit::MarkupDirector(hb);
    md->processDocument(doc);
    auto result = hb->getResult();

    QRegularExpression regex(QStringLiteral(
        "^<p style=\"margin-top:0;margin-bottom:0;margin-left:0;margin-right:0;\">A <a href=\"http://www.kde.org\"><strong>formatted</strong>&nbsp;"
        "link</a>&nbsp;to KDE.</p>\\n$"));

    QVERIFY(regex.match(result).hasMatch());
    delete md;
    delete hb;
    delete doc;
}

void TextHTMLBuilderTest::testAdjacentAnchors()
{
    auto doc = new QTextDocument();
    doc->setHtml(
        QStringLiteral("Two <a href=\"http://www.kde.org\">links</a><a "
                       "href=\"http://www.google.com\">next</a> to eachother."));

    auto hb = new KPIMTextEdit::TextHTMLBuilder();
    auto md = new KPIMTextEdit::MarkupDirector(hb);
    md->processDocument(doc);
    auto result = hb->getResult();

    QRegularExpression regex(
        QStringLiteral("^<p style=\"margin-top:0;margin-bottom:0;margin-left:0;margin-right:0;\">Two <a href=\"http://www.kde.org\">links</a><a "
                       "href=\"http://www.google.com\">next</a>&nbsp;to eachother.</p>\\n$"));

    QVERIFY(regex.match(result).hasMatch());
    delete md;
    delete hb;
    delete doc;
}

void TextHTMLBuilderTest::testNestedFormatting()
{
    auto doc = new QTextDocument();
    doc->setHtml(QStringLiteral("This <b>text is <i>italic</i> and</b> bold."));

    auto hb = new KPIMTextEdit::TextHTMLBuilder();
    auto md = new KPIMTextEdit::MarkupDirector(hb);
    md->processDocument(doc);
    auto result = hb->getResult();

    QRegularExpression regex(QStringLiteral(
        "^<p style=\"margin-top:0;margin-bottom:0;margin-left:0;margin-right:0;\">This <strong>text is <em>italic</em>&nbsp;and</strong>&nbsp;bold.</p>\\n$"));

    QVERIFY(regex.match(result).hasMatch());
    delete md;
    delete hb;
    delete doc;
}

void TextHTMLBuilderTest::testSpan()
{
    auto doc = new QTextDocument();
    doc->setHtml(QStringLiteral("Some <span style=\"color:#ff0000;\">formatted</span> text."));

    auto hb = new KPIMTextEdit::TextHTMLBuilder();
    auto md = new KPIMTextEdit::MarkupDirector(hb);
    md->processDocument(doc);
    auto result = hb->getResult();

    auto regex = QRegularExpression(
        QStringLiteral("^<p style=\"margin-top:0;margin-bottom:0;margin-left:0;margin-right:0;\">Some <span style=\"color:#ff0000;\">formatted</span>&nbsp;"
                       "text.</p>\\n$"));

    QVERIFY(regex.match(result).hasMatch());
    delete md;
    delete hb;
    delete doc;
}

void TextHTMLBuilderTest::testDoubleSpan()
{
    auto doc = new QTextDocument();
    doc->setHtml(
        QStringLiteral("Some <span "
                       "style=\"color:#ff0000;background-color:#00ff00;"
                       "\">formatted</span> text."));

    auto hb = new KPIMTextEdit::TextHTMLBuilder();
    auto md = new KPIMTextEdit::MarkupDirector(hb);
    md->processDocument(doc);
    auto result = hb->getResult();

    auto regex =
        QRegularExpression(QStringLiteral("^<p style=\"margin-top:0;margin-bottom:0;margin-left:0;margin-right:0;\">Some <span "
                                          "style=\"(color:#ff0000|background-color:#00ff00);\"><span "
                                          "style=\"(color:#ff0000|background-color:#00ff00);\">formatted</span></"
                                          "span>&nbsp;text.</p>\\n$"));

    QVERIFY(regex.match(result).hasMatch());
    delete md;
    delete hb;
    delete doc;
}

void TextHTMLBuilderTest::testSpanNesting()
{
    auto doc = new QTextDocument();
    doc->setHtml(
        QStringLiteral("Paragraph <span style=\"background-color:#00ff00;\">with some <span "
                       "style=\"color:#ff0000;\">formatted</span> nested</span> text."));

    auto hb = new KPIMTextEdit::TextHTMLBuilder();
    auto md = new KPIMTextEdit::MarkupDirector(hb);
    md->processDocument(doc);
    auto result = hb->getResult();

    auto regex = QRegularExpression(QStringLiteral(
        "^<p style=\"margin-top:0;margin-bottom:0;margin-left:0;margin-right:0;\">Paragraph <span style=\"background-color:#00ff00;\">with some <span "
        "style=\"color:#ff0000;\">formatted</span>&nbsp;nested</span>&nbsp;text.</p>\\n$"));
    QVERIFY(regex.match(result).hasMatch());
    delete md;
    delete hb;
    delete doc;
}

void TextHTMLBuilderTest::testDoubleStartDifferentFinish()
{
    auto doc = new QTextDocument();
    doc->setHtml(QStringLiteral("Paragraph <i><b>with</b> some formatted</i> text."));

    auto hb = new KPIMTextEdit::TextHTMLBuilder();
    auto md = new KPIMTextEdit::MarkupDirector(hb);
    md->processDocument(doc);
    auto result = hb->getResult();

    auto regex = QRegularExpression(
        QStringLiteral("^<p style=\"margin-top:0;margin-bottom:0;margin-left:0;margin-right:0;\">Paragraph <em><strong>with</strong>&nbsp;some "
                       "formatted</em>&nbsp;text.</p>\\n$"));
    QVERIFY(regex.match(result).hasMatch());
    delete md;
    delete hb;
    delete doc;
}

void TextHTMLBuilderTest::testDoubleStartDifferentFinishReverseOrder()
{
    auto doc = new QTextDocument();
    doc->setHtml(QStringLiteral("Paragraph <b><i>with</i> some formatted</b> text."));

    auto hb = new KPIMTextEdit::TextHTMLBuilder();
    auto md = new KPIMTextEdit::MarkupDirector(hb);
    md->processDocument(doc);
    auto result = hb->getResult();

    auto regex =
        QRegularExpression(QStringLiteral("^<p style=\"margin-top:0;margin-bottom:0;margin-left:0;margin-right:0;\">Paragraph <strong><em>with</em>&nbsp;some "
                                          "formatted</strong>&nbsp;text.</p>\\n$"));

    QVERIFY(regex.match(result).hasMatch());
    delete md;
    delete hb;
    delete doc;
}

void TextHTMLBuilderTest::testDifferentStartDoubleFinish()
{
    auto doc = new QTextDocument();
    doc->setHtml(QStringLiteral("Paragraph <i>with some <b>formatted<b></i> text."));

    auto hb = new KPIMTextEdit::TextHTMLBuilder();
    auto md = new KPIMTextEdit::MarkupDirector(hb);
    md->processDocument(doc);
    auto result = hb->getResult();

    auto regex =
        QRegularExpression(QStringLiteral("^<p style=\"margin-top:0;margin-bottom:0;margin-left:0;margin-right:0;\">Paragraph <em>with some "
                                          "<strong>formatted</strong></em>&nbsp;text.</p>\\n$"));

    QVERIFY(regex.match(result).hasMatch());
    delete md;
    delete hb;
    delete doc;
}

void TextHTMLBuilderTest::testDifferentStartDoubleFinishReverseOrder()
{
    auto doc = new QTextDocument();
    doc->setHtml(QStringLiteral("Paragraph <b>with some <i>formatted</i></b> text."));

    auto hb = new KPIMTextEdit::TextHTMLBuilder();
    auto md = new KPIMTextEdit::MarkupDirector(hb);
    md->processDocument(doc);
    auto result = hb->getResult();

    auto regex =
        QRegularExpression(QStringLiteral("^<p style=\"margin-top:0;margin-bottom:0;margin-left:0;margin-right:0;\">Paragraph <strong>with some "
                                          "<em>formatted</em></strong>&nbsp;text.</p>\\n$"));

    QVERIFY(regex.match(result).hasMatch());
    delete md;
    delete hb;
    delete doc;
}

void TextHTMLBuilderTest::testOverlap()
{
    auto doc = new QTextDocument();
    doc->setHtml(QStringLiteral("Paragraph <b>with <i>some</i></b><i> formatted</i> text."));

    auto hb = new KPIMTextEdit::TextHTMLBuilder();
    auto md = new KPIMTextEdit::MarkupDirector(hb);
    md->processDocument(doc);
    auto result = hb->getResult();

    auto regex = QRegularExpression(
        QStringLiteral("^<p style=\"margin-top:0;margin-bottom:0;margin-left:0;margin-right:0;\">Paragraph <strong>with <em>some</em></strong><em>&nbsp;"
                       "formatted</em>&nbsp;text.</p>\\n$"));

    QVERIFY(regex.match(result).hasMatch());
    delete md;
    delete hb;
    delete doc;
}

void TextHTMLBuilderTest::testEdgeCaseLeft()
{
    auto doc = new QTextDocument();
    doc->setHtml(QStringLiteral("Paragraph <b>with some formatted text.</b>"));

    auto hb = new KPIMTextEdit::TextHTMLBuilder();
    auto md = new KPIMTextEdit::MarkupDirector(hb);
    md->processDocument(doc);
    auto result = hb->getResult();

    auto regex = QRegularExpression(QStringLiteral(
        "^<p style=\"margin-top:0;margin-bottom:0;margin-left:0;margin-right:0;\">Paragraph <strong>with some formatted text.</strong></p>\\n$"));

    QVERIFY(regex.match(result).hasMatch());
    delete md;
    delete hb;
    delete doc;
}

void TextHTMLBuilderTest::testEdgeCaseRight()
{
    auto doc = new QTextDocument();
    doc->setHtml(QStringLiteral("<b>Paragraph with some formatted</b> text."));

    auto hb = new KPIMTextEdit::TextHTMLBuilder();
    auto md = new KPIMTextEdit::MarkupDirector(hb);
    md->processDocument(doc);
    auto result = hb->getResult();

    auto regex = QRegularExpression(QStringLiteral(
        "^<p style=\"margin-top:0;margin-bottom:0;margin-left:0;margin-right:0;\"><strong>Paragraph with some formatted</strong>&nbsp;text.</p>\\n$"));

    QVERIFY(regex.match(result).hasMatch());
    delete md;
    delete hb;
    delete doc;
}

void TextHTMLBuilderTest::testImage()
{
    auto doc = new QTextDocument();
    doc->setHtml(
        QStringLiteral("Paragraph with an inline <img "
                       "src=\"http://kde.org/img/kde41.png\" /> image."));

    auto hb = new KPIMTextEdit::TextHTMLBuilder();
    auto md = new KPIMTextEdit::MarkupDirector(hb);
    md->processDocument(doc);
    auto result = hb->getResult();

    auto regex =
        QRegularExpression(QStringLiteral("^<p style=\"margin-top:0;margin-bottom:0;margin-left:0;margin-right:0;\">Paragraph with an inline <img "
                                          "src=\"http://kde.org/img/kde41.png\" />&nbsp;image.</p>\\n$"));

    QVERIFY(regex.match(result).hasMatch());
    delete md;
    delete hb;
    delete doc;
}

void TextHTMLBuilderTest::testImageResized()
{
    QString result;
    QRegularExpression regex;
    auto doc = new QTextDocument();

    // width
    doc->setHtml(
        QStringLiteral("Paragraph with an inline <img src=\"http://kde.org/img/kde41.png\" "
                       "width=\"10\" /> image."));

    auto hb = new KPIMTextEdit::TextHTMLBuilder();
    auto md = new KPIMTextEdit::MarkupDirector(hb);
    md->processDocument(doc);
    result = hb->getResult();

    regex = QRegularExpression(QStringLiteral(
        "^<p style=\"margin-top:0;margin-bottom:0;margin-left:0;margin-right:0;\">Paragraph with an inline <img src=\"http://kde.org/img/kde41.png\" "
        "width=\"10\" />&nbsp;image.</p>\\n$"));
    QVERIFY(regex.match(result).hasMatch());

    // height
    doc->setHtml(
        QStringLiteral("Paragraph with an inline <img src=\"http://kde.org/img/kde41.png\" "
                       "height=\"10\" /> image."));

    delete hb;
    delete md;
    hb = new KPIMTextEdit::TextHTMLBuilder();
    md = new KPIMTextEdit::MarkupDirector(hb);
    md->processDocument(doc);
    result = hb->getResult();

    regex = QRegularExpression(QStringLiteral(
        "^<p style=\"margin-top:0;margin-bottom:0;margin-left:0;margin-right:0;\">Paragraph with an inline <img src=\"http://kde.org/img/kde41.png\" "
        "height=\"10\" />&nbsp;image.</p>\\n$"));
    QVERIFY(regex.match(result).hasMatch());

    // height and width
    doc->setHtml(
        QStringLiteral("Paragraph with an inline <img src=\"http://kde.org/img/kde41.png\" "
                       "height=\"10\" width=\"10\" /> image."));

    delete hb;
    delete md;
    hb = new KPIMTextEdit::TextHTMLBuilder();
    md = new KPIMTextEdit::MarkupDirector(hb);
    md->processDocument(doc);
    result = hb->getResult();

    regex = QRegularExpression(QStringLiteral(
        "^<p style=\"margin-top:0;margin-bottom:0;margin-left:0;margin-right:0;\">Paragraph with an inline <img src=\"http://kde.org/img/kde41.png\" "
        "width=\"10\" height=\"10\" />&nbsp;image.</p>\\n$"));

    QVERIFY(regex.match(result).hasMatch());
    delete md;
    delete hb;
    delete doc;
}

void TextHTMLBuilderTest::testEachFormatTagSingly()
{
    QString result;
    QRegularExpression regex;
    auto doc = new QTextDocument();

    // Test bold
    doc->setHtml(QStringLiteral("Some <b>formatted</b> text."));
    auto hb = new KPIMTextEdit::TextHTMLBuilder();
    auto md = new KPIMTextEdit::MarkupDirector(hb);
    md->processDocument(doc);
    result = hb->getResult();

    regex = QRegularExpression(
        QStringLiteral("^<p style=\"margin-top:0;margin-bottom:0;margin-left:0;margin-right:0;\">Some <strong>formatted</strong>&nbsp;text.</p>\\n$"));
    QVERIFY(regex.match(result).hasMatch());

    // Test Italic
    doc->setHtml(QStringLiteral("Some <i>formatted</i> text."));
    delete hb;
    delete md;

    hb = new KPIMTextEdit::TextHTMLBuilder();
    md = new KPIMTextEdit::MarkupDirector(hb);
    md->processDocument(doc);
    result = hb->getResult();

    regex = QRegularExpression(
        QStringLiteral("^<p style=\"margin-top:0;margin-bottom:0;margin-left:0;margin-right:0;\">Some <em>formatted</em>&nbsp;text.</p>\\n$"));
    QVERIFY(regex.match(result).hasMatch());

    // Test Underline
    doc->setHtml(QStringLiteral("Some <u>formatted</u> text."));
    delete hb;
    delete md;

    hb = new KPIMTextEdit::TextHTMLBuilder();
    md = new KPIMTextEdit::MarkupDirector(hb);
    md->processDocument(doc);
    result = hb->getResult();

    regex =
        QRegularExpression(QStringLiteral("^<p style=\"margin-top:0;margin-bottom:0;margin-left:0;margin-right:0;\">Some <u>formatted</u>&nbsp;text.</p>\\n$"));
    QVERIFY(regex.match(result).hasMatch());

    // Test Strikeout
    doc->setHtml(QStringLiteral("Some <s>formatted</s> text."));
    delete hb;
    delete md;

    hb = new KPIMTextEdit::TextHTMLBuilder();
    md = new KPIMTextEdit::MarkupDirector(hb);
    md->processDocument(doc);
    result = hb->getResult();

    regex =
        QRegularExpression(QStringLiteral("^<p style=\"margin-top:0;margin-bottom:0;margin-left:0;margin-right:0;\">Some <s>formatted</s>&nbsp;text.</p>\\n$"));
    QVERIFY(regex.match(result).hasMatch());

    // Test Superscript
    doc->setHtml(QStringLiteral("Some <sup>formatted</sup> text."));
    delete hb;
    delete md;

    hb = new KPIMTextEdit::TextHTMLBuilder();
    md = new KPIMTextEdit::MarkupDirector(hb);
    md->processDocument(doc);
    result = hb->getResult();

    regex = QRegularExpression(
        QStringLiteral("^<p style=\"margin-top:0;margin-bottom:0;margin-left:0;margin-right:0;\">Some <sup>formatted</sup>&nbsp;text.</p>\\n$"));
    QVERIFY(regex.match(result).hasMatch());

    // Test Subscript
    doc->setHtml(QStringLiteral("Some <sub>formatted</sub> text."));
    delete hb;
    delete md;

    hb = new KPIMTextEdit::TextHTMLBuilder();
    md = new KPIMTextEdit::MarkupDirector(hb);
    md->processDocument(doc);
    result = hb->getResult();

    regex = QRegularExpression(
        QStringLiteral("^<p style=\"margin-top:0;margin-bottom:0;margin-left:0;margin-right:0;\">Some <sub>formatted</sub>&nbsp;text.</p>\\n$"));
    QVERIFY(regex.match(result).hasMatch());

    // Test Foreground
    doc->setHtml(QStringLiteral("Some <span style=\"color:#ff0000;\">formatted</span> text."));
    delete hb;
    delete md;

    hb = new KPIMTextEdit::TextHTMLBuilder();
    md = new KPIMTextEdit::MarkupDirector(hb);
    md->processDocument(doc);
    result = hb->getResult();

    regex = QRegularExpression(
        QStringLiteral("^<p style=\"margin-top:0;margin-bottom:0;margin-left:0;margin-right:0;\">Some <span style=\"color:#ff0000;\">formatted</span>&nbsp;"
                       "text.</p>\\n$"));
    QVERIFY(regex.match(result).hasMatch());

    // Test Background
    doc->setHtml(QStringLiteral("Some <span style=\"background-color:#ff0000;\">formatted</span> text."));
    delete hb;
    delete md;

    hb = new KPIMTextEdit::TextHTMLBuilder();
    md = new KPIMTextEdit::MarkupDirector(hb);
    md->processDocument(doc);
    result = hb->getResult();

    regex = QRegularExpression(QStringLiteral(
        "^<p style=\"margin-top:0;margin-bottom:0;margin-left:0;margin-right:0;\">Some <span style=\"background-color:#ff0000;\">formatted</span>&nbsp;"
        "text.</p>\\n$"));
    QVERIFY(regex.match(result).hasMatch());

    // Test Font Family
    doc->setHtml(QStringLiteral("Some <span style=\"font-family:courier;\">formatted</span> text."));
    delete hb;
    delete md;

    hb = new KPIMTextEdit::TextHTMLBuilder();
    md = new KPIMTextEdit::MarkupDirector(hb);
    md->processDocument(doc);
    result = hb->getResult();

    regex = QRegularExpression(QStringLiteral(
        "^<p style=\"margin-top:0;margin-bottom:0;margin-left:0;margin-right:0;\">Some <span style=\"font-family:courier;\">formatted</span>&nbsp;"
        "text.</p>\\n$"));
    QVERIFY(regex.match(result).hasMatch());

    // Test Font Size
    doc->setHtml(QStringLiteral("Some <span style=\"font-size:20pt;\">formatted</span> text."));
    delete hb;
    delete md;

    hb = new KPIMTextEdit::TextHTMLBuilder();
    md = new KPIMTextEdit::MarkupDirector(hb);
    md->processDocument(doc);
    result = hb->getResult();

    regex = QRegularExpression(
        QStringLiteral("^<p style=\"margin-top:0;margin-bottom:0;margin-left:0;margin-right:0;\">Some <span style=\"font-size:20pt;\">formatted</span>&nbsp;"
                       "text.</p>\\n$"));
    QVERIFY(regex.match(result).hasMatch());
    delete md;
    delete hb;
    delete doc;
}

void TextHTMLBuilderTest::testHorizontalRule()
{
    auto doc = new QTextDocument();
    doc->setHtml(
        QStringLiteral("<p style=\"margin-top:0;margin-bottom:0;\">Foo</p><hr "
                       "/><p style=\"margin-top:0;margin-bottom:0;\">Bar</p>"));

    auto hb = new KPIMTextEdit::TextHTMLBuilder();
    auto md = new KPIMTextEdit::MarkupDirector(hb);
    md->processDocument(doc);
    auto result = hb->getResult();

    auto regex =
        QRegularExpression(QStringLiteral("^<p style=\"margin-top:0;margin-bottom:0;margin-left:0;margin-right:0;\">Foo</p>\\n<hr />\\n<p "
                                          "style=\"margin-top:0;margin-bottom:0;margin-left:0;margin-right:0;\">Bar</p>\\n$"));

    QVERIFY(regex.match(result).hasMatch());
    delete md;
    delete hb;
    delete doc;
}

void TextHTMLBuilderTest::testNewlines()
{
    auto doc = new QTextDocument();
    doc->setHtml(QStringLiteral("<p>Foo<br /><br />\n<p>Bar</p>"));

    auto hb = new KPIMTextEdit::TextHTMLBuilder();
    auto md = new KPIMTextEdit::MarkupDirector(hb);
    md->processDocument(doc);
    auto result = hb->getResult();

    auto regex =
        QRegularExpression(QStringLiteral("^<p style=\"margin-top:12;margin-bottom:12;margin-left:0;margin-right:0;\">Foo</p>\\n<p>&nbsp;<p>&nbsp;</p>\\n<p "
                                          "style=\"margin-top:12;margin-bottom:12;margin-left:0;margin-right:0;\">Bar</p>\\n$"));
    QVERIFY(regex.match(result).hasMatch());
    delete md;
    delete hb;
    delete doc;
}

void TextHTMLBuilderTest::testNewlinesThroughQTextCursor()
{
    auto doc = new QTextDocument(this);
    QTextCursor cursor(doc);
    cursor.movePosition(QTextCursor::Start);
    cursor.insertText(QStringLiteral("Foo"));
    cursor.insertText(QStringLiteral("\n"));
    cursor.insertText(QStringLiteral("\n"));
    cursor.insertText(QStringLiteral("\n"));
    cursor.insertText(QStringLiteral("Bar"));

    auto hb = new KPIMTextEdit::TextHTMLBuilder();
    auto md = new KPIMTextEdit::MarkupDirector(hb);
    md->processDocument(doc);
    auto result = hb->getResult();

    auto regex =
        QRegularExpression(QStringLiteral("^<p style=\"margin-top:0;margin-bottom:0;margin-left:0;margin-right:0;\">Foo</p>\\n<br /><br /><p "
                                          "style=\"margin-top:0;margin-bottom:0;margin-left:0;margin-right:0;\">Bar</p>\\n$"));
    // qDebug() << "result " << result;
    QVERIFY(regex.match(result).hasMatch());
    delete md;
    delete hb;
    delete doc;
}

void TextHTMLBuilderTest::testInsertImage()
{
    auto doc = new QTextDocument(this);
    QTextCursor cursor(doc);
    cursor.movePosition(QTextCursor::Start);
    cursor.insertText(QStringLiteral("Foo"));
    cursor.insertText(QStringLiteral("\n"));
    cursor.insertText(QStringLiteral("\n"));
    cursor.insertText(QStringLiteral("\n"));
    cursor.insertText(QStringLiteral("Bar"));

    const QString imagePath = QStringLiteral(GRANTLEEBUILDER_DIR "/object-fill.svg");
    const QImage image(imagePath);
    QString imageNameToAdd = QStringLiteral("imagename");
    doc->addResource(QTextDocument::ImageResource, QUrl(imageNameToAdd), image);
    cursor.insertImage(imageNameToAdd);

    auto hb = new KPIMTextEdit::TextHTMLBuilder();
    auto md = new KPIMTextEdit::MarkupDirector(hb);
    md->processDocument(doc);
    auto result = hb->getResult();

    auto regex =
        QRegularExpression(QStringLiteral("^<p style=\"margin-top:0;margin-bottom:0;margin-left:0;margin-right:0;\">Foo</p>\\n<br /><br /><p "
                                          "style=\"margin-top:0;margin-bottom:0;margin-left:0;margin-right:0;\">Bar<img src=\"imagename\" /></p>\\n$"));
    QVERIFY(regex.match(result).hasMatch());
    delete md;
    delete hb;
    delete doc;
}

void TextHTMLBuilderTest::testInsertImageWithSize()
{
    auto doc = new QTextDocument(this);
    QTextCursor cursor(doc);
    cursor.movePosition(QTextCursor::Start);
    cursor.insertText(QStringLiteral("Foo"));
    cursor.insertText(QStringLiteral("\n"));
    cursor.insertText(QStringLiteral("\n"));
    cursor.insertText(QStringLiteral("\n"));
    cursor.insertText(QStringLiteral("Bar"));

    const QString imagePath = QStringLiteral(GRANTLEEBUILDER_DIR "/object-fill.svg");
    const QImage image(imagePath);
    QString imageNameToAdd = QStringLiteral("imagename");
    doc->addResource(QTextDocument::ImageResource, QUrl(imageNameToAdd), image);

    QTextImageFormat format;
    format.setName(imageNameToAdd);
    format.setWidth(100);
    format.setHeight(120);
    cursor.insertImage(format);

    auto hb = new KPIMTextEdit::TextHTMLBuilder();
    auto md = new KPIMTextEdit::MarkupDirector(hb);
    md->processDocument(doc);
    auto result = hb->getResult();

    auto regex = QRegularExpression(QStringLiteral(
        "^<p style=\"margin-top:0;margin-bottom:0;margin-left:0;margin-right:0;\">Foo</p>\\n<br /><br /><p "
        "style=\"margin-top:0;margin-bottom:0;margin-left:0;margin-right:0;\">Bar<img src=\"imagename\" width=\"100\" height=\"120\" /></p>\\n$"));
    QVERIFY(regex.match(result).hasMatch());
    delete md;
    delete hb;
    delete doc;
}

void TextHTMLBuilderTest::testTitle1()
{
    const int boundedLevel = 1;
    auto doc = new QTextDocument(this);
    QTextCursor cursor(doc);
    cursor.movePosition(QTextCursor::Start);
    cursor.insertText(QStringLiteral("Foo"));

    const int sizeAdjustment = boundedLevel > 0 ? 5 - boundedLevel : 0;
    QTextBlockFormat blkfmt;
    blkfmt.setHeadingLevel(boundedLevel);
    cursor.mergeBlockFormat(blkfmt);

    QTextCharFormat chrfmt;
    chrfmt.setFontWeight(boundedLevel > 0 ? QFont::Bold : QFont::Normal);
    chrfmt.setProperty(QTextFormat::FontSizeAdjustment, sizeAdjustment);
    QTextCursor selectCursor = cursor;
    QTextCursor top = selectCursor;
    top.setPosition(qMin(top.anchor(), top.position()));
    top.movePosition(QTextCursor::StartOfBlock);

    QTextCursor bottom = selectCursor;
    bottom.setPosition(qMax(bottom.anchor(), bottom.position()));
    bottom.movePosition(QTextCursor::EndOfBlock);

    selectCursor.setPosition(top.position(), QTextCursor::MoveAnchor);
    selectCursor.setPosition(bottom.position(), QTextCursor::KeepAnchor);
    selectCursor.mergeCharFormat(chrfmt);

    cursor.mergeBlockCharFormat(chrfmt);

    auto hb = new KPIMTextEdit::TextHTMLBuilder();
    auto md = new KPIMTextEdit::MarkupDirector(hb);
    md->processDocument(doc);
    auto result = hb->getResult();

    auto regex = QRegularExpression(QStringLiteral(
        "^<p style=\"margin-top:0;margin-bottom:0;margin-left:0;margin-right:0;\"><span style=\"font-size:29pt;\"><strong>Foo</strong></span></p>\n$"));
    // qDebug() << " result " << result;
    // TODO implement header support now.
    delete md;
    delete hb;
    delete doc;
    QVERIFY(regex.match(result).hasMatch());
}

void TextHTMLBuilderTest::testBug421908()
{
    auto doc = new QTextDocument();
    doc->setHtml(QStringLiteral("<p><span style=\" color:#aaaaff;\">some colored text<br />some colored text</span></p>"));

    auto hb = new KPIMTextEdit::TextHTMLBuilder();
    auto md = new KPIMTextEdit::MarkupDirector(hb);
    md->processDocument(doc);
    auto result = hb->getResult();

    auto regex =
        QRegularExpression(QStringLiteral("^<p style=\"margin-top:12;margin-bottom:12;margin-left:0;margin-right:0;\"><span style=\"color:#aaaaff;\">some "
                                          "colored text<br />some colored text</span></p>\n$"));
    QVERIFY(regex.match(result).hasMatch());
    delete md;
    delete hb;
    delete doc;
}

void TextHTMLBuilderTest::testBug421908_2()
{
    auto doc = new QTextDocument();
    doc->setHtml(
        QStringLiteral("<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\">phone: "
                       "+123456 7890<br />mail: some@mail.com</p>"
                       "<p style=\"-qt-paragraph-type:empty; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; "
                       "text-indent:0px;\"><br /></p>"
                       "<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span "
                       "style=\" font-size:7pt;\">small text</span></p>"));

    auto hb = new KPIMTextEdit::TextHTMLBuilder();
    auto md = new KPIMTextEdit::MarkupDirector(hb);
    md->processDocument(doc);
    auto result = hb->getResult();

    auto regex = QRegularExpression(
        QStringLiteral("<p style=\"margin-top:0;margin-bottom:0;margin-left:0;margin-right:0;\">phone: \\+123456 7890<br />mail: some@mail.com</p>\n<br /><p "
                       "style=\"margin-top:0;margin-bottom:0;margin-left:0;margin-right:0;\"><span style=\"font-size:7pt;\">small text</span></p>$"));
    // qDebug() << " result "<< result;
    QVERIFY(regex.match(result).hasMatch());
    delete md;
    delete hb;
    delete doc;
}

void TextHTMLBuilderTest::testBug421908_full()
{
    auto doc = new QTextDocument();
    doc->setHtml(
        QStringLiteral("<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; "
                       "text-indent:0px;\"><!--StartFragment-->phone: +123456 7890<br />mail: some@mail.com</p>"
                       "<p style=\"-qt-paragraph-type:empty; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; "
                       "text-indent:0px;\"><br /></p>"
                       "<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span "
                       "style=\" font-size:7pt;\">small text</span></p>"
                       "<p style=\"-qt-paragraph-type:empty; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; "
                       "text-indent:0px;\"><br /></p>"
                       "<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span "
                       "style=\" color:#aaaaff;\">some colored text<br />some colored text</span></p>"
                       "<p style=\"-qt-paragraph-type:empty; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; "
                       "text-indent:0px;\"><br /><!--EndFragment--></p>"));

    auto hb = new KPIMTextEdit::TextHTMLBuilder();
    auto md = new KPIMTextEdit::MarkupDirector(hb);
    md->processDocument(doc);
    auto result = hb->getResult();

    auto regex = QRegularExpression(
        QStringLiteral("<p style=\"margin-top:0;margin-bottom:0;margin-left:0;margin-right:0;\">phone: \\+123456 7890<br />mail: some@mail.com</p>\n<br /><p "
                       "style=\"margin-top:0;margin-bottom:0;margin-left:0;margin-right:0;\"><span style=\"font-size:7pt;\">small text</span></p>\n<br /><p "
                       "style=\"margin-top:0;margin-bottom:0;margin-left:0;margin-right:0;\"><span style=\"color:\\#aaaaff;\">some colored text<br />some "
                       "colored text</span></p>\n<br />$"));
    // qDebug() << " result "<< result;
    QVERIFY(regex.match(result).hasMatch());
    delete md;
    delete hb;
    delete doc;
}

void TextHTMLBuilderTest::testBug436880()
{
    auto doc = new QTextDocument();
    doc->setHtml(QStringLiteral("<p dir='rtl'>test</ p>"));

    auto hb = new KPIMTextEdit::TextHTMLBuilder();
    auto md = new KPIMTextEdit::MarkupDirector(hb);
    md->processDocument(doc);
    auto result = hb->getResult();

    // qDebug() << " result " << result;
    auto regex = QRegularExpression(QStringLiteral("^<p style=\"margin-top:12;margin-bottom:12;margin-left:0;margin-right:0;\" dir='rtl'>test</p>\n"));

    QVERIFY(regex.match(result).hasMatch());
    delete md;
    delete hb;
    delete doc;
}
