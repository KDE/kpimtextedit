/*
   SPDX-FileCopyrightText: 2020-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "texthtmlbuildertest.h"
using namespace Qt::Literals::StringLiterals;

#include "grantleebuilder/markupdirector.h"
#include "grantleebuilder/texthtmlbuilder.h"
#include <QRegularExpression>
#include <QTest>
#include <QTextDocument>
#include <QTextTable>
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
    cursor.insertText(u"\n"_s);
    cursor.insertText(u"\t"_s);
    cursor.insertText(u"foo"_s);

    auto hb = new KPIMTextEdit::TextHTMLBuilder();
    auto md = new KPIMTextEdit::MarkupDirector(hb);
    md->processDocument(doc);
    auto result = hb->getResult();

    auto regex = QRegularExpression(u"^<br /><p style=\"margin-top:0;margin-bottom:0;margin-left:0;margin-right:0;\">&nbsp;&nbsp;&nbsp; foo</p>\\n$"_s);

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
        << u"A <a href=\"http://www.kde.org\">link</a> to KDE."_s
        << QStringLiteral(
               "^<p style=\"margin-top:0;margin-bottom:0;margin-left:0;margin-right:0;\">A <a href=\"http://www.kde.org\">link</a>&nbsp;to KDE.</p>\\n$")
        << true;
    QTest::newRow("text with space")
        << u"         foo"_s
        << QStringLiteral(
               "^<p style=\"margin-top:0;margin-bottom:0;margin-left:0;margin-right:0;\">&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; foo</p>\\n$")
        << false;
    QTest::newRow("text with leading space") << u" foo"_s << u"^<p style=\"margin-top:0;margin-bottom:0;margin-left:0;margin-right:0;\">&nbsp;foo</p>\\n$"_s
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
    doc->setHtml(u"This <b>text</b> is bold."_s);

    auto hb = new KPIMTextEdit::TextHTMLBuilder();
    auto md = new KPIMTextEdit::MarkupDirector(hb);
    md->processDocument(doc);
    auto result = hb->getResult();
    QRegularExpression regex(u"^<p style=\"margin-top:0;margin-bottom:0;margin-left:0;margin-right:0;\">This <strong>text</strong>&nbsp;is bold.</p>\\n$"_s);

    QVERIFY(regex.match(result).hasMatch());
    delete md;
    delete hb;
    delete doc;
}

void TextHTMLBuilderTest::testDoubleFormat()
{
    auto doc = new QTextDocument();

    // One format
    doc->setHtml(u"Some <b><i>formatted</i></b> text."_s);

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
    doc->setHtml(u"A <a href=\"http://www.kde.org\">link</a> to KDE."_s);

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
    doc->setHtml(u"A <a href=\"http://www.kde.org\"><b>formatted</b> link</a> to KDE."_s);

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
                       "href=\"http://www.google.com\">next</a> to each other."));

    auto hb = new KPIMTextEdit::TextHTMLBuilder();
    auto md = new KPIMTextEdit::MarkupDirector(hb);
    md->processDocument(doc);
    auto result = hb->getResult();

    QRegularExpression regex(
        QStringLiteral("^<p style=\"margin-top:0;margin-bottom:0;margin-left:0;margin-right:0;\">Two <a href=\"http://www.kde.org\">links</a><a "
                       "href=\"http://www.google.com\">next</a>&nbsp;to each other.</p>\\n$"));

    QVERIFY(regex.match(result).hasMatch());
    delete md;
    delete hb;
    delete doc;
}

void TextHTMLBuilderTest::testNestedFormatting()
{
    auto doc = new QTextDocument();
    doc->setHtml(u"This <b>text is <i>italic</i> and</b> bold."_s);

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
    doc->setHtml(u"Some <span style=\"color:#ff0000;\">formatted</span> text."_s);

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
    doc->setHtml(u"Paragraph <i><b>with</b> some formatted</i> text."_s);

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

void TextHTMLBuilderTest::testBug504090()
{
    auto doc = new QTextDocument();
    doc->setHtml(u"You should see a <u>_<foo>_</u> on this line."_s);

    auto hb = new KPIMTextEdit::TextHTMLBuilder();
    auto md = new KPIMTextEdit::MarkupDirector(hb);
    md->processDocument(doc);
    auto result = hb->getResult();

    qDebug() << " result" << result;
    auto regex =
        QRegularExpression(QStringLiteral("^<p style=\"margin-top:0;margin-bottom:0;margin-left:0;margin-right:0;\">Paragraph <strong><em>with</em>&nbsp;some "
                                          "formatted</strong>&nbsp;text.</p>\\n$"));

    QEXPECT_FAIL("", "Problem with <u>_<foo>_</u>", Continue);
    QVERIFY(regex.match(result).hasMatch());
    delete md;
    delete hb;
    delete doc;
}

void TextHTMLBuilderTest::testDoubleStartDifferentFinishReverseOrder()
{
    auto doc = new QTextDocument();
    doc->setHtml(u"Paragraph <b><i>with</i> some formatted</b> text."_s);

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
    doc->setHtml(u"Paragraph <i>with some <b>formatted<b></i> text."_s);

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
    doc->setHtml(u"Paragraph <b>with some <i>formatted</i></b> text."_s);

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
    doc->setHtml(u"Paragraph <b>with <i>some</i></b><i> formatted</i> text."_s);

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
    doc->setHtml(u"Paragraph <b>with some formatted text.</b>"_s);

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
    doc->setHtml(u"<b>Paragraph with some formatted</b> text."_s);

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
    doc->setHtml(u"Some <b>formatted</b> text."_s);
    auto hb = new KPIMTextEdit::TextHTMLBuilder();
    auto md = new KPIMTextEdit::MarkupDirector(hb);
    md->processDocument(doc);
    result = hb->getResult();

    regex =
        QRegularExpression(u"^<p style=\"margin-top:0;margin-bottom:0;margin-left:0;margin-right:0;\">Some <strong>formatted</strong>&nbsp;text.</p>\\n$"_s);
    QVERIFY(regex.match(result).hasMatch());

    // Test Italic
    doc->setHtml(u"Some <i>formatted</i> text."_s);
    delete hb;
    delete md;

    hb = new KPIMTextEdit::TextHTMLBuilder();
    md = new KPIMTextEdit::MarkupDirector(hb);
    md->processDocument(doc);
    result = hb->getResult();

    regex = QRegularExpression(u"^<p style=\"margin-top:0;margin-bottom:0;margin-left:0;margin-right:0;\">Some <em>formatted</em>&nbsp;text.</p>\\n$"_s);
    QVERIFY(regex.match(result).hasMatch());

    // Test Underline
    doc->setHtml(u"Some <u>formatted</u> text."_s);
    delete hb;
    delete md;

    hb = new KPIMTextEdit::TextHTMLBuilder();
    md = new KPIMTextEdit::MarkupDirector(hb);
    md->processDocument(doc);
    result = hb->getResult();

    regex = QRegularExpression(u"^<p style=\"margin-top:0;margin-bottom:0;margin-left:0;margin-right:0;\">Some <u>formatted</u>&nbsp;text.</p>\\n$"_s);
    QVERIFY(regex.match(result).hasMatch());

    // Test Strikeout
    doc->setHtml(u"Some <s>formatted</s> text."_s);
    delete hb;
    delete md;

    hb = new KPIMTextEdit::TextHTMLBuilder();
    md = new KPIMTextEdit::MarkupDirector(hb);
    md->processDocument(doc);
    result = hb->getResult();

    regex = QRegularExpression(u"^<p style=\"margin-top:0;margin-bottom:0;margin-left:0;margin-right:0;\">Some <s>formatted</s>&nbsp;text.</p>\\n$"_s);
    QVERIFY(regex.match(result).hasMatch());

    // Test Superscript
    doc->setHtml(u"Some <sup>formatted</sup> text."_s);
    delete hb;
    delete md;

    hb = new KPIMTextEdit::TextHTMLBuilder();
    md = new KPIMTextEdit::MarkupDirector(hb);
    md->processDocument(doc);
    result = hb->getResult();

    regex = QRegularExpression(u"^<p style=\"margin-top:0;margin-bottom:0;margin-left:0;margin-right:0;\">Some <sup>formatted</sup>&nbsp;text.</p>\\n$"_s);
    QVERIFY(regex.match(result).hasMatch());

    // Test Subscript
    doc->setHtml(u"Some <sub>formatted</sub> text."_s);
    delete hb;
    delete md;

    hb = new KPIMTextEdit::TextHTMLBuilder();
    md = new KPIMTextEdit::MarkupDirector(hb);
    md->processDocument(doc);
    result = hb->getResult();

    regex = QRegularExpression(u"^<p style=\"margin-top:0;margin-bottom:0;margin-left:0;margin-right:0;\">Some <sub>formatted</sub>&nbsp;text.</p>\\n$"_s);
    QVERIFY(regex.match(result).hasMatch());

    // Test Foreground
    doc->setHtml(u"Some <span style=\"color:#ff0000;\">formatted</span> text."_s);
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
    doc->setHtml(u"Some <span style=\"background-color:#ff0000;\">formatted</span> text."_s);
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
    doc->setHtml(u"Some <span style=\"font-family:courier;\">formatted</span> text."_s);
    delete hb;
    delete md;

    hb = new KPIMTextEdit::TextHTMLBuilder();
    md = new KPIMTextEdit::MarkupDirector(hb);
    md->processDocument(doc);
    result = hb->getResult();

    regex = QRegularExpression(QStringLiteral(
        "^<p style=\"margin-top:0;margin-bottom:0;margin-left:0;margin-right:0;\">Some <span style=\"font-family:'courier';\">formatted</span>&nbsp;"
        "text.</p>\\n$"));
    QVERIFY(regex.match(result).hasMatch());

    // Test Font Size
    doc->setHtml(u"Some <span style=\"font-size:20pt;\">formatted</span> text."_s);
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
    doc->setHtml(u"<p>Foo<br /><br />\n<p>Bar</p>"_s);

    auto hb = new KPIMTextEdit::TextHTMLBuilder();
    auto md = new KPIMTextEdit::MarkupDirector(hb);
    md->processDocument(doc);
    auto result = hb->getResult();

    // Every paragraph opened by a run of empty lines is closed by the same builder call, and the
    // end of the block does not add a stray closing tag on top.
    auto regex = QRegularExpression(
        QStringLiteral("^<p style=\"margin-top:12px;margin-bottom:12px;margin-left:0;margin-right:0;\">Foo</p>\\n<p>&nbsp;</p><p>&nbsp;</p><p "
                       "style=\"margin-top:12px;margin-bottom:12px;margin-left:0;margin-right:0;\">Bar</p>\\n$"));
    QVERIFY2(regex.match(result).hasMatch(), qPrintable(result));
    QCOMPARE(result.count(u"<p"_s), result.count(u"</p>"_s));
    delete md;
    delete hb;
    delete doc;
}

void TextHTMLBuilderTest::testNewlinesThroughQTextCursor()
{
    auto doc = new QTextDocument(this);
    QTextCursor cursor(doc);
    cursor.movePosition(QTextCursor::Start);
    cursor.insertText(u"Foo"_s);
    cursor.insertText(u"\n"_s);
    cursor.insertText(u"\n"_s);
    cursor.insertText(u"\n"_s);
    cursor.insertText(u"Bar"_s);

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
    cursor.insertText(u"Foo"_s);
    cursor.insertText(u"\n"_s);
    cursor.insertText(u"\n"_s);
    cursor.insertText(u"\n"_s);
    cursor.insertText(u"Bar"_s);

    const QString imagePath = QStringLiteral(GRANTLEEBUILDER_DIR "/object-fill.svg");
    const QImage image(imagePath);
    QString imageNameToAdd = u"imagename"_s;
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
    cursor.insertText(u"Foo"_s);
    cursor.insertText(u"\n"_s);
    cursor.insertText(u"\n"_s);
    cursor.insertText(u"\n"_s);
    cursor.insertText(u"Bar"_s);

    const QString imagePath = QStringLiteral(GRANTLEEBUILDER_DIR "/object-fill.svg");
    const QImage image(imagePath);
    QString imageNameToAdd = u"imagename"_s;
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
    cursor.insertText(u"Foo"_s);

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
    QEXPECT_FAIL("", "Problem with title", Continue);
    QVERIFY(regex.match(result).hasMatch());
}

void TextHTMLBuilderTest::testBug421908()
{
    auto doc = new QTextDocument();
    doc->setHtml(u"<p><span style=\" color:#aaaaff;\">some colored text<br />some colored text</span></p>"_s);

    auto hb = new KPIMTextEdit::TextHTMLBuilder();
    auto md = new KPIMTextEdit::MarkupDirector(hb);
    md->processDocument(doc);
    auto result = hb->getResult();

    auto regex =
        QRegularExpression(QStringLiteral("^<p style=\"margin-top:12px;margin-bottom:12px;margin-left:0;margin-right:0;\"><span style=\"color:#aaaaff;\">some "
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
    doc->setHtml(u"<p dir='rtl'>test</ p>"_s);

    auto hb = new KPIMTextEdit::TextHTMLBuilder();
    auto md = new KPIMTextEdit::MarkupDirector(hb);
    md->processDocument(doc);
    auto result = hb->getResult();

    // qDebug() << " result " << result;
    auto regex = QRegularExpression(u"^<p style=\"margin-top:12px;margin-bottom:12px;margin-left:0;margin-right:0;\" dir=\"rtl\">test</p>\n"_s);

    QVERIFY(regex.match(result).hasMatch());
    delete md;
    delete hb;
    delete doc;
}

void TextHTMLBuilderTest::testBug442416()
{
    auto doc = new QTextDocument();
    doc->setHtml(QStringLiteral(
        "<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px; -qt-user-state:0;\"><span "
        "style=\" background-color:#ffff00;\">Sss</span></p>\n<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; "
        "-qt-block-indent:0; text-indent:0px; -qt-user-state:0;\"><span style=\" background-color:#ffff00;\">sss</span></p>"));

    auto hb = new KPIMTextEdit::TextHTMLBuilder();
    auto md = new KPIMTextEdit::MarkupDirector(hb);
    md->processDocument(doc);
    auto result = hb->getResult();

    // qDebug() << " result " << result;
    auto regex = QRegularExpression(
        QStringLiteral("^<p style=\"margin-top:0;margin-bottom:0;margin-left:0;margin-right:0;\"><span style=\"background-color:#ffff00;\">Sss</span></p>\n<p "
                       "style=\"margin-top:0;margin-bottom:0;margin-left:0;margin-right:0;\"><span style=\"background-color:#ffff00;\">sss</span></p>\n"));

    QVERIFY(regex.match(result).hasMatch());
    delete md;
    delete hb;
    delete doc;
}

void TextHTMLBuilderTest::testBug442416Bis()
{
    auto doc = new QTextDocument();
    doc->setHtml(QStringLiteral(
        "<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px; -qt-user-state:0;\"><span "
        "style=\" background-color:#ffff00;\">Sss</span></p>\n<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; "
        "-qt-block-indent:0; text-indent:0px; -qt-user-state:0;\"><span style=\" background-color:#ff0000;\">sss</span></p>"));

    auto hb = new KPIMTextEdit::TextHTMLBuilder();
    auto md = new KPIMTextEdit::MarkupDirector(hb);
    md->processDocument(doc);
    auto result = hb->getResult();

    // qDebug() << " result " << result;
    auto regex = QRegularExpression(
        QStringLiteral("^<p style=\"margin-top:0;margin-bottom:0;margin-left:0;margin-right:0;\"><span style=\"background-color:#ffff00;\">Sss</span></p>\n<p "
                       "style=\"margin-top:0;margin-bottom:0;margin-left:0;margin-right:0;\"><span style=\"background-color:#ff0000;\">sss</span></p>\n"));
    QVERIFY(regex.match(result).hasMatch());
    delete md;
    delete hb;
    delete doc;
}

void TextHTMLBuilderTest::testBugTextColor()
{
    auto doc = new QTextDocument();
    doc->setHtml(u"<p><span style=\"color:#ffff00;\">BBBB</span></p><p><span style=\"color:#ffff00;\">AAA</p>"_s);

    auto hb = new KPIMTextEdit::TextHTMLBuilder();
    auto md = new KPIMTextEdit::MarkupDirector(hb);
    md->processDocument(doc);
    auto result = hb->getResult();

    // qDebug() << " result " << result;
    auto regex = QRegularExpression(
        QStringLiteral("^<p style=\"margin-top:12px;margin-bottom:12px;margin-left:0;margin-right:0;\"><span style=\"color:#ffff00;\">BBBB</span></p>\n<p "
                       "style=\"margin-top:12px;margin-bottom:12px;margin-left:0;margin-right:0;\"><span style=\"color:#ffff00;\">AAA</span></p>\n"));
    QVERIFY(regex.match(result).hasMatch());
    delete md;
    delete hb;
    delete doc;
}

void TextHTMLBuilderTest::testBugIndent443534()
{
    auto doc = new QTextDocument();
    doc->setHtml(QStringLiteral(
        "<ul style=\"margin-top: 0px; margin-bottom: 0px; margin-right: 0px; -qt-list-indent: 1;\"><li style=\" margin-top:0px; margin-bottom:0px; "
        "margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px; -qt-user-state:0;\">Test1</li><li style=\" margin-top:0px; "
        "margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px; -qt-user-state:0;\">Test2</li><ul style=\"margin-top: "
        "0px; margin-bottom: 0px; margin-right: 0px; -qt-list-indent: 2;\"><li style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; "
        "-qt-block-indent:0; text-indent:0px; -qt-user-state:0;\">Indent1</li></ul><li style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; "
        "margin-right:0px; -qt-block-indent:0; text-indent:0px; -qt-user-state:0;\">deindent1</li></ul>"));

    auto hb = new KPIMTextEdit::TextHTMLBuilder();
    auto md = new KPIMTextEdit::MarkupDirector(hb);
    md->processDocument(doc);
    auto result = hb->getResult();

    qDebug() << " result " << result;
    auto regex = QRegularExpression(
        QStringLiteral("^<p style=\"margin-top:12px;margin-bottom:12px;margin-left:0;margin-right:0;\"><span style=\"color:#ffff00;\">BBBB</span></p>\n<p "
                       "style=\"margin-top:12px;margin-bottom:12px;margin-left:0;margin-right:0;\"><span style=\"color:#ffff00;\">AAA</span></p>\n"));
    QEXPECT_FAIL("", "Problem with list bug 443534", Continue);
    QVERIFY(regex.match(result).hasMatch());
    delete md;
    delete hb;
    delete doc;
}

// Serializing a document back to html must not invent a table border that the
// document does not have, nor drop one that it has.
void TextHTMLBuilderTest::testTableBorder_data()
{
    QTest::addColumn<QString>("html");
    QTest::addColumn<QString>("expectedBorder");

    QTest::newRow("border-0") << u"<table border=\"0\" width=\"100%\" cellspacing=\"0\" cellpadding=\"0\"><tr><td>LEFT</td><td>RIGHT</td></tr></table>"_s
                              << u"0"_s;
    QTest::newRow("border-1") << u"<table border=\"1\" width=\"100%\" cellspacing=\"2\" cellpadding=\"2\"><tr><td>LEFT</td><td>RIGHT</td></tr></table>"_s
                              << u"1"_s;
    QTest::newRow("no-border-attribute") << u"<table width=\"100%\"><tr><td>LEFT</td><td>RIGHT</td></tr></table>"_s << u"0"_s;
    QTest::newRow("css-border-none") << u"<table style=\"border:0px none transparent; border-collapse:collapse;\"><tr><td>L</td><td>R</td></tr></table>"_s
                                     << u"0"_s;
    QTest::newRow("css-border-style-none-with-width") << u"<table border=\"1\" style=\"border-style:none;\"><tr><td>L</td><td>R</td></tr></table>"_s << u"0"_s;
    QTest::newRow("css-border-3px") << u"<table style=\"border:3px solid black;\"><tr><td>L</td><td>R</td></tr></table>"_s << u"3"_s;
    // The html border attribute is an integer, a fractional border width must not leak into it.
    QTest::newRow("fractional-border") << u"<table border=\"0.5\"><tr><td>L</td></tr></table>"_s << u"1"_s;
}

void TextHTMLBuilderTest::testTableBorder()
{
    QFETCH(QString, html);
    QFETCH(QString, expectedBorder);

    QTextDocument doc;
    doc.setHtml(html);

    KPIMTextEdit::TextHTMLBuilder hb;
    KPIMTextEdit::MarkupDirector md(&hb);
    md.processDocument(&doc);
    const QString result = hb.getResult();

    QVERIFY2(result.contains(u" border=\"%1\""_s.arg(expectedBorder)), qPrintable(result));
}

void TextHTMLBuilderTest::testTableBorderFromFormat_data()
{
    QTest::addColumn<qreal>("border");
    QTest::addColumn<QTextFrameFormat::BorderStyle>("borderStyle");
    QTest::addColumn<QString>("expectedBorder");

    QTest::newRow("no-border") << qreal(0) << QTextFrameFormat::BorderStyle_None << u"0"_s;
    QTest::newRow("visible-border") << qreal(1) << QTextFrameFormat::BorderStyle_Solid << u"1"_s;
    QTest::newRow("thick-border") << qreal(3) << QTextFrameFormat::BorderStyle_Solid << u"3"_s;
    // A width without a style paints nothing, so it must not be serialized as a visible border.
    QTest::newRow("width-but-style-none") << qreal(2) << QTextFrameFormat::BorderStyle_None << u"0"_s;
}

void TextHTMLBuilderTest::testTableBorderFromFormat()
{
    QFETCH(qreal, border);
    QFETCH(QTextFrameFormat::BorderStyle, borderStyle);
    QFETCH(QString, expectedBorder);

    QTextDocument doc;
    QTextCursor cursor(&doc);
    QTextTableFormat format;
    format.setBorder(border);
    format.setBorderStyle(borderStyle);
    cursor.insertTable(1, 2, format);

    KPIMTextEdit::TextHTMLBuilder hb;
    KPIMTextEdit::MarkupDirector md(&hb);
    md.processDocument(&doc);
    const QString result = hb.getResult();

    QVERIFY2(result.contains(u" border=\"%1\""_s.arg(expectedBorder)), qPrintable(result));
}

// border-collapse has no html attribute, it only survives a round trip as a css declaration on
// the table tag. Dropping it turns a collapsed grid back into doubled, spaced out cell borders.
void TextHTMLBuilderTest::testTableBorderCollapse_data()
{
    QTest::addColumn<QString>("html");
    QTest::addColumn<bool>("expectedCollapse");

    QTest::newRow("collapse") << u"<table border=\"1\" style=\"border-collapse:collapse;\"><tr><td>L</td><td>R</td></tr></table>"_s << true;
    QTest::newRow("separate") << u"<table border=\"1\" style=\"border-collapse:separate;\"><tr><td>L</td><td>R</td></tr></table>"_s << false;
    // Without the css declaration the html parser leaves the property off, no style must be invented.
    QTest::newRow("unspecified") << u"<table border=\"1\"><tr><td>L</td><td>R</td></tr></table>"_s << false;
    // A collapsed grid whose border is not painted is still collapsed.
    QTest::newRow("collapse-without-border")
        << u"<table style=\"border:0px none transparent; border-collapse:collapse;\"><tr><td>L</td><td>R</td></tr></table>"_s << true;
}

void TextHTMLBuilderTest::testTableBorderCollapse()
{
    QFETCH(QString, html);
    QFETCH(bool, expectedCollapse);

    QTextDocument doc;
    doc.setHtml(html);

    KPIMTextEdit::TextHTMLBuilder hb;
    KPIMTextEdit::MarkupDirector md(&hb);
    md.processDocument(&doc);
    const QString result = hb.getResult();

    QCOMPARE(result.contains(u" style=\"border-collapse:collapse;\">"_s), expectedCollapse);
}

void TextHTMLBuilderTest::testTableBorderCollapseFromFormat_data()
{
    QTest::addColumn<bool>("borderCollapse");

    QTest::newRow("collapse") << true;
    QTest::newRow("separate") << false;
}

void TextHTMLBuilderTest::testTableBorderCollapseFromFormat()
{
    QFETCH(bool, borderCollapse);

    QTextDocument doc;
    QTextCursor cursor(&doc);
    QTextTableFormat format;
    format.setBorderCollapse(borderCollapse);
    cursor.insertTable(1, 2, format);

    KPIMTextEdit::TextHTMLBuilder hb;
    KPIMTextEdit::MarkupDirector md(&hb);
    md.processDocument(&doc);
    const QString result = hb.getResult();

    QCOMPARE(result.contains(u" style=\"border-collapse:collapse;\">"_s), borderCollapse);
}

namespace
{
// The opening table tag, attributes included, so that a test can state the whole of what the
// builder wrote for the table and not only a fragment of it.
QString tableTag(const QString &html)
{
    const qsizetype start = html.indexOf(u"<table"_s);
    if (start < 0) {
        return {};
    }
    return html.mid(start, html.indexOf(u'>', start) - start + 1);
}

QString buildHtml(QTextDocument *doc)
{
    KPIMTextEdit::TextHTMLBuilder hb;
    KPIMTextEdit::MarkupDirector md(&hb);
    md.processDocument(doc);
    return hb.getResult();
}
}

// The html border attribute only carries a width, it always paints a plain line: a dotted or a
// coloured table border survives a round trip as css or not at all.
void TextHTMLBuilderTest::testTableBorderStyleAndColor_data()
{
    QTest::addColumn<QString>("html");
    QTest::addColumn<QString>("expectedStyle");

    // A bare border attribute is the outset dark gray border the html parser gives every table,
    // css has nothing to add to it.
    QTest::newRow("border-attribute-only") << u"<table border=\"1\"><tr><td>L</td></tr></table>"_s << QString();
    QTest::newRow("dashed-red") << u"<table style=\"border:2px dashed red;\"><tr><td>L</td></tr></table>"_s
                                << u" style=\"border-style: dashed; border-color: #ff0000;\""_s;
    QTest::newRow("solid-black") << u"<table style=\"border:1px solid black;\"><tr><td>L</td></tr></table>"_s
                                 << u" style=\"border-style: solid; border-color: #000000;\""_s;
    QTest::newRow("double") << u"<table style=\"border:3px double #0000ff;\"><tr><td>L</td></tr></table>"_s
                            << u" style=\"border-style: double; border-color: #0000ff;\""_s;
    QTest::newRow("color-only") << u"<table border=\"1\" bordercolor=\"#00ff00\"><tr><td>L</td></tr></table>"_s << u" style=\"border-color: #00ff00;\""_s;
    // A table which paints no border has no border style and no border color to serialize.
    QTest::newRow("no-border") << u"<table style=\"border:0px none transparent;\"><tr><td>L</td></tr></table>"_s << QString();
}

void TextHTMLBuilderTest::testTableBorderStyleAndColor()
{
    QFETCH(QString, html);
    QFETCH(QString, expectedStyle);

    QTextDocument doc;
    doc.setHtml(html);

    const QString tag = tableTag(buildHtml(&doc));
    if (expectedStyle.isEmpty()) {
        QVERIFY2(!tag.contains(u" style=\""_s), qPrintable(tag));
    } else {
        QVERIFY2(tag.contains(expectedStyle), qPrintable(tag));
    }
}

// The space a table keeps from the text around it has no html attribute, only css.
void TextHTMLBuilderTest::testTableMargins()
{
    QTextDocument doc;
    QTextCursor cursor(&doc);
    QTextTableFormat format;
    format.setTopMargin(10);
    format.setBottomMargin(0);
    format.setLeftMargin(20.5);
    format.setRightMargin(5);
    cursor.insertTable(1, 1, format);

    const QString tag = tableTag(buildHtml(&doc));
    // A css length needs its unit, and a margin of zero is what the renderer does anyway.
    QVERIFY2(tag.contains(u"margin-top: 10px"_s), qPrintable(tag));
    QVERIFY2(tag.contains(u"margin-left: 20.5px"_s), qPrintable(tag));
    QVERIFY2(tag.contains(u"margin-right: 5px"_s), qPrintable(tag));
    QVERIFY2(!tag.contains(u"margin-bottom"_s), qPrintable(tag));
}

// The padding of the table itself, which cellpadding does not describe: cellpadding is the space
// inside the cells.
void TextHTMLBuilderTest::testTablePadding()
{
    QTextDocument doc;
    QTextCursor cursor(&doc);
    QTextTableFormat format;
    format.setPadding(6);
    cursor.insertTable(1, 1, format);

    const QString tag = tableTag(buildHtml(&doc));
    QVERIFY2(tag.contains(u"padding: 6px"_s), qPrintable(tag));

    QTextDocument plainDoc;
    QTextCursor plainCursor(&plainDoc);
    plainCursor.insertTable(1, 1, QTextTableFormat());
    // A table which asked for no padding must not be given one. The cellpadding attribute is a
    // different thing, it stays.
    const QString plainTag = tableTag(buildHtml(&plainDoc));
    QVERIFY2(!plainTag.contains(u"padding: "_s), qPrintable(plainTag));
}

// A floating table has the text flow around it. This is not the align attribute, which only moves
// the table inside the flow.
void TextHTMLBuilderTest::testTableFloat_data()
{
    QTest::addColumn<QTextFrameFormat::Position>("position");
    QTest::addColumn<QString>("expectedFloat");

    QTest::newRow("in-flow") << QTextFrameFormat::InFlow << QString();
    QTest::newRow("float-left") << QTextFrameFormat::FloatLeft << u"float: left"_s;
    QTest::newRow("float-right") << QTextFrameFormat::FloatRight << u"float: right"_s;
}

void TextHTMLBuilderTest::testTableFloat()
{
    QFETCH(QTextFrameFormat::Position, position);
    QFETCH(QString, expectedFloat);

    QTextDocument doc;
    QTextCursor cursor(&doc);
    QTextTableFormat format;
    format.setPosition(position);
    cursor.insertTable(1, 1, format);

    const QString tag = tableTag(buildHtml(&doc));
    if (expectedFloat.isEmpty()) {
        QVERIFY2(!tag.contains(u"float"_s), qPrintable(tag));
    } else {
        QVERIFY2(tag.contains(expectedFloat), qPrintable(tag));
    }
}

// The height of a table is read back from the document like its width, a variable height is the
// one html has no attribute for.
void TextHTMLBuilderTest::testTableHeight_data()
{
    QTest::addColumn<QString>("html");
    QTest::addColumn<QString>("expectedHeight");

    QTest::newRow("fixed") << u"<table height=\"200\"><tr><td>L</td></tr></table>"_s << u" height=\"200\""_s;
    QTest::newRow("percentage") << u"<table height=\"30%\"><tr><td>L</td></tr></table>"_s << u" height=\"30%\""_s;
    QTest::newRow("unspecified") << u"<table><tr><td>L</td></tr></table>"_s << QString();
}

void TextHTMLBuilderTest::testTableHeight()
{
    QFETCH(QString, html);
    QFETCH(QString, expectedHeight);

    QTextDocument doc;
    doc.setHtml(html);

    const QString tag = tableTag(buildHtml(&doc));
    if (expectedHeight.isEmpty()) {
        QVERIFY2(!tag.contains(u" height=\""_s), qPrintable(tag));
    } else {
        QVERIFY2(tag.contains(expectedHeight), qPrintable(tag));
    }
}

// A tag can only carry one style attribute, so everything the table has to say in css goes into
// the same one.
void TextHTMLBuilderTest::testTableStyleDeclarationsShareOneAttribute()
{
    QTextDocument doc;
    QTextCursor cursor(&doc);
    QTextTableFormat format;
    format.setBorder(1);
    format.setBorderStyle(QTextFrameFormat::BorderStyle_Dotted);
    format.setBorderBrush(Qt::red);
    format.setTopMargin(4);
    format.setPadding(2);
    format.setBorderCollapse(true);
    cursor.insertTable(1, 1, format);

    const QString tag = tableTag(buildHtml(&doc));
    QCOMPARE(tag.count(u"style=\""_s), 1);
    QVERIFY2(tag.contains(u" style=\"border-style: dotted; border-color: #ff0000; margin-top: 4px; padding: 2px; border-collapse:collapse;\""_s),
             qPrintable(tag));
}

void TextHTMLBuilderTest::testTableCellsUnaffectedByBorder()
{
    QTextDocument doc;
    doc.setHtml(u"<table border=\"0\" width=\"100%\" cellspacing=\"0\" cellpadding=\"0\"><tr><td>LEFT</td><td colspan=\"2\">RIGHT</td></tr></table>"_s);

    KPIMTextEdit::TextHTMLBuilder hb;
    KPIMTextEdit::MarkupDirector md(&hb);
    md.processDocument(&doc);
    const QString result = hb.getResult();

    QVERIFY2(result.contains(u"<table cellpadding=\"0\" cellspacing=\"0\" width=\"100%\" border=\"0\">"_s), qPrintable(result));
    QVERIFY2(result.contains(u"<td colspan=\"1\" rowspan=\"1\">"_s), qPrintable(result));
    QVERIFY2(result.contains(u"<td colspan=\"2\" rowspan=\"1\">"_s), qPrintable(result));
}

void TextHTMLBuilderTest::testTableBackgroundColor()
{
    QTextDocument doc;
    doc.setHtml(
        u"<table border=\"0\" width=\"100%\" cellspacing=\"0\" cellpadding=\"0\" bgcolor=\"#FF0000\"><tr><td>LEFT</td><td colspan=\"2\">RIGHT</td></tr></table>"_s);

    KPIMTextEdit::TextHTMLBuilder hb;
    KPIMTextEdit::MarkupDirector md(&hb);
    md.processDocument(&doc);
    const QString result = hb.getResult();

    QVERIFY2(result.contains(u"<table cellpadding=\"0\" cellspacing=\"0\" width=\"100%\" border=\"0\" bgcolor=\"#ff0000\">"_s), qPrintable(result));
    QVERIFY2(result.contains(u"<td colspan=\"1\" rowspan=\"1\">"_s), qPrintable(result));
    QVERIFY2(result.contains(u"<td colspan=\"2\" rowspan=\"1\">"_s), qPrintable(result));
}

void TextHTMLBuilderTest::testTableAlignment()
{
    {
        QTextDocument doc;
        doc.setHtml(
            u"<table border=\"0\" width=\"100%\" cellspacing=\"0\" cellpadding=\"0\" align=\"center\"><tr><td>LEFT</td><td colspan=\"2\">RIGHT</td></tr></table>"_s);

        KPIMTextEdit::TextHTMLBuilder hb;
        KPIMTextEdit::MarkupDirector md(&hb);
        md.processDocument(&doc);
        const QString result = hb.getResult();

        QVERIFY2(result.contains(u"<table cellpadding=\"0\" cellspacing=\"0\" width=\"100%\" border=\"0\" align=\"center\">"_s), qPrintable(result));
        QVERIFY2(result.contains(u"<td colspan=\"1\" rowspan=\"1\">"_s), qPrintable(result));
        QVERIFY2(result.contains(u"<td colspan=\"2\" rowspan=\"1\">"_s), qPrintable(result));
    }

    {
        QTextDocument doc;
        doc.setHtml(
            u"<table border=\"0\" width=\"100%\" cellspacing=\"0\" cellpadding=\"0\" align=\"right\"><tr><td>LEFT</td><td colspan=\"2\">RIGHT</td></tr></table>"_s);

        KPIMTextEdit::TextHTMLBuilder hb;
        KPIMTextEdit::MarkupDirector md(&hb);
        md.processDocument(&doc);
        const QString result = hb.getResult();

        QVERIFY2(result.contains(u"<table cellpadding=\"0\" cellspacing=\"0\" width=\"100%\" border=\"0\" align=\"right\">"_s), qPrintable(result));
        QVERIFY2(result.contains(u"<td colspan=\"1\" rowspan=\"1\">"_s), qPrintable(result));
        QVERIFY2(result.contains(u"<td colspan=\"2\" rowspan=\"1\">"_s), qPrintable(result));
    }
    {
        QTextDocument doc;
        doc.setHtml(
            u"<table border=\"0\" width=\"100%\" cellspacing=\"0\" cellpadding=\"0\" align=\"left\"><tr><td>LEFT</td><td colspan=\"2\">RIGHT</td></tr></table>"_s);

        KPIMTextEdit::TextHTMLBuilder hb;
        KPIMTextEdit::MarkupDirector md(&hb);
        md.processDocument(&doc);
        const QString result = hb.getResult();

        QVERIFY2(result.contains(u"<table cellpadding=\"0\" cellspacing=\"0\" width=\"100%\" border=\"0\" align=\"left\">"_s), qPrintable(result));
        QVERIFY2(result.contains(u"<td colspan=\"1\" rowspan=\"1\">"_s), qPrintable(result));
        QVERIFY2(result.contains(u"<td colspan=\"2\" rowspan=\"1\">"_s), qPrintable(result));
    }
}

void TextHTMLBuilderTest::testTableCellSpans()
{
    QTextDocument doc;
    doc.setHtml(
        u"<table border=\"1\" width=\"100%\" cellspacing=\"0\" cellpadding=\"0\">"
        "<tr><td rowspan=\"2\">A</td><td width=\"30%\">B</td><td>C</td></tr>"
        "<tr><td colspan=\"2\">D</td></tr></table>"_s);

    KPIMTextEdit::TextHTMLBuilder hb;
    KPIMTextEdit::MarkupDirector md(&hb);
    md.processDocument(&doc);
    const QString result = hb.getResult();

    // Spans and column widths are read from the cell format and from the table column constraints.
    QVERIFY2(result.contains(u"<td colspan=\"1\" rowspan=\"2\">"_s), qPrintable(result));
    QVERIFY2(result.contains(u"<td width=\"30%\" colspan=\"1\" rowspan=\"1\">"_s), qPrintable(result));
    QVERIFY2(result.contains(u"<td width=\"30%\" colspan=\"2\" rowspan=\"1\">"_s), qPrintable(result));
}

void TextHTMLBuilderTest::testTableCellBackgroundColor()
{
    QTextDocument doc;
    doc.setHtml(
        u"<table border=\"0\" width=\"100%\" cellspacing=\"0\" cellpadding=\"0\" bgcolor=\"#FF0000\"><tr><td bgcolor=\"#ff00ff\">LEFT</td><td colspan=\"2\" bgcolor=\"#1100FF\">RIGHT</td></tr></table>"_s);

    KPIMTextEdit::TextHTMLBuilder hb;
    KPIMTextEdit::MarkupDirector md(&hb);
    md.processDocument(&doc);
    const QString result = hb.getResult();

    QVERIFY2(result.contains(u"<table cellpadding=\"0\" cellspacing=\"0\" width=\"100%\" border=\"0\" bgcolor=\"#ff0000\">"_s), qPrintable(result));
    QVERIFY2(result.contains(u"<td colspan=\"1\" rowspan=\"1\" bgcolor=\"#ff00ff\">"_s), qPrintable(result));
    QVERIFY2(result.contains(u"<td colspan=\"2\" rowspan=\"1\" bgcolor=\"#1100ff\">"_s), qPrintable(result));
}

void TextHTMLBuilderTest::testTableCellvAlignment()
{
    QTextDocument doc;
    doc.setHtml(
        u"<table border=\"0\" width=\"100%\" cellspacing=\"0\" cellpadding=\"0\" bgcolor=\"#FF0000\"><tr><td bgcolor=\"#ff00ff\" valign=\"bottom\">LEFT</td><td colspan=\"2\" bgcolor=\"#1100FF\" valign=\"top\">RIGHT2</td><td colspan=\"2\" bgcolor=\"#1100FF\" valign=\"middle\">RIGHT</td></tr></table>"_s);

    KPIMTextEdit::TextHTMLBuilder hb;
    KPIMTextEdit::MarkupDirector md(&hb);
    md.processDocument(&doc);
    const QString result = hb.getResult();

    QVERIFY2(result.contains(u"<table cellpadding=\"0\" cellspacing=\"0\" width=\"100%\" border=\"0\" bgcolor=\"#ff0000\">"_s), qPrintable(result));
    QVERIFY2(result.contains(u"<td colspan=\"1\" rowspan=\"1\" bgcolor=\"#ff00ff\" valign=\"bottom\">"_s), qPrintable(result));
    QVERIFY2(result.contains(u"<td colspan=\"2\" rowspan=\"1\" bgcolor=\"#1100ff\" valign=\"middle\">"_s), qPrintable(result));
    QVERIFY2(result.contains(u"<td colspan=\"2\" rowspan=\"1\" bgcolor=\"#1100ff\" valign=\"top\">"_s), qPrintable(result));
}

// A css length is only honoured when it carries a unit, so a bare "margin-top:12" is
// dropped by the renderer and the paragraph spacing is lost in the generated mail.
void TextHTMLBuilderTest::testParagraphMarginsCarryCssUnit()
{
    QTextDocument doc;
    doc.setHtml(u"<p>Foo</p>"_s);

    KPIMTextEdit::TextHTMLBuilder hb;
    KPIMTextEdit::MarkupDirector md(&hb);
    md.processDocument(&doc);
    const QString result = hb.getResult();

    QVERIFY2(result.contains(u"margin-top:12px;"_s), qPrintable(result));
    QVERIFY2(result.contains(u"margin-bottom:12px;"_s), qPrintable(result));
    // Zero needs no unit, and keeping it bare avoids churn in the generated markup.
    QVERIFY2(result.contains(u"margin-left:0;"_s), qPrintable(result));
    QVERIFY2(result.contains(u"margin-right:0;"_s), qPrintable(result));
    QVERIFY2(!result.contains(u"margin-top:12;"_s), qPrintable(result));
}

void TextHTMLBuilderTest::testBlockQuoteMarginsCarryCssUnit()
{
    QTextDocument doc;
    doc.setHtml(u"<blockquote>quoted</blockquote>"_s);

    KPIMTextEdit::TextHTMLBuilder hb;
    KPIMTextEdit::MarkupDirector md(&hb);
    md.processDocument(&doc);
    const QString result = hb.getResult();

    // Without the unit the quote is not indented at all on the receiving side.
    QVERIFY2(result.contains(u"margin-left:40px;"_s), qPrintable(result));
    QVERIFY2(result.contains(u"margin-right:40px;"_s), qPrintable(result));
}

void TextHTMLBuilderTest::testTableCellPadding()
{
    QTextDocument doc;
    doc.setHtml(
        u"<table border=\"0\" cellspacing=\"0\" cellpadding=\"0\"><tr><td style=\"padding:5px\">ALL</td><td "
        "style=\"padding-top:3px;padding-left:7px\">SOME</td><td>NONE</td></tr></table>"_s);

    KPIMTextEdit::TextHTMLBuilder hb;
    KPIMTextEdit::MarkupDirector md(&hb);
    md.processDocument(&doc);
    const QString result = hb.getResult();

    QVERIFY2(result.contains(u"<td colspan=\"1\" rowspan=\"1\" style=\"padding-bottom: 5px; padding-top: 5px; padding-left: 5px; padding-right: 5px;\">"_s),
             qPrintable(result));
    // Only the sides the cell actually carries are serialized.
    QVERIFY2(result.contains(u"<td colspan=\"1\" rowspan=\"1\" style=\"padding-top: 3px; padding-left: 7px;\">"_s), qPrintable(result));
    // A cell without any padding gets no style attribute at all.
    QVERIFY2(result.contains(u"<td colspan=\"1\" rowspan=\"1\"><p"_s), qPrintable(result));
}

void TextHTMLBuilderTest::testTableCellPaddingExplicitZero()
{
    QTextDocument doc;
    // A cell that explicitly sets a zero padding must keep it: dropping it would let the table
    // cellpadding attribute apply, so the cell would be rendered with 4px instead of none.
    doc.setHtml(u"<table border=\"0\" cellspacing=\"0\" cellpadding=\"4\"><tr><td style=\"padding:0\">ZERO</td><td>UNSET</td></tr></table>"_s);

    KPIMTextEdit::TextHTMLBuilder hb;
    KPIMTextEdit::MarkupDirector md(&hb);
    md.processDocument(&doc);
    const QString result = hb.getResult();

    QVERIFY2(result.contains(u"<td colspan=\"1\" rowspan=\"1\" style=\"padding-bottom: 0; padding-top: 0; padding-left: 0; padding-right: 0;\">"_s),
             qPrintable(result));
    // Conversely the cell that never set a padding carries no style, so it keeps inheriting cellpadding.
    QVERIFY2(result.contains(u"<td colspan=\"1\" rowspan=\"1\"><p"_s), qPrintable(result));
}

void TextHTMLBuilderTest::testTableHeaderCellPadding()
{
    QTextDocument doc;
    QTextCursor cursor(&doc);
    QTextTableFormat tableFormat;
    // Header cells are only reachable through headerRowCount, the html parser maps th to td.
    tableFormat.setHeaderRowCount(1);
    tableFormat.setCellPadding(0);
    tableFormat.setBorder(0);
    QTextTable *table = cursor.insertTable(2, 1, tableFormat);

    QTextTableCellFormat cellFormat = table->cellAt(0, 0).format().toTableCellFormat();
    cellFormat.setTopPadding(6);
    cellFormat.setRightPadding(9);
    table->cellAt(0, 0).setFormat(cellFormat);
    table->cellAt(0, 0).firstCursorPosition().insertText(u"HEAD"_s);
    table->cellAt(1, 0).firstCursorPosition().insertText(u"BODY"_s);

    KPIMTextEdit::TextHTMLBuilder hb;
    KPIMTextEdit::MarkupDirector md(&hb);
    md.processDocument(&doc);
    const QString result = hb.getResult();

    QVERIFY2(result.contains(u"<th colspan=\"1\" rowspan=\"1\" style=\"padding-top: 6px; padding-right: 9px;\">"_s), qPrintable(result));
    QVERIFY2(result.contains(u"<td colspan=\"1\" rowspan=\"1\"><p"_s), qPrintable(result));
}

// A cell border lives in the cell format, not in the table border attribute, so it only
// survives a round trip as css on the cell tag.
void TextHTMLBuilderTest::testTableCellBorder_data()
{
    QTest::addColumn<QString>("html");
    QTest::addColumn<QString>("expectedStyle");

    QTest::newRow("all-sides") << u"<table border=\"0\"><tr><td style=\"border:1px solid blue;\">L</td></tr></table>"_s
                               << u" style=\"border-bottom: 1px solid #0000ff; border-top: 1px solid #0000ff; "
                                  "border-left: 1px solid #0000ff; border-right: 1px solid #0000ff;\""_s;
    QTest::newRow("single-side") << u"<table border=\"0\"><tr><td style=\"border-left:3px dashed red;\">L</td></tr></table>"_s
                                 << u" style=\"border-left: 3px dashed #ff0000;\""_s;
    QTest::newRow("mixed-sides") << u"<table border=\"0\"><tr><td style=\"border-top:4px double green;border-bottom:2px groove;\">L</td></tr></table>"_s
                                 << u" style=\"border-bottom: 2px groove; border-top: 4px double #008000;\""_s;
    // Css falls back to currentColor when the colour is left out, which is what the document says.
    QTest::newRow("no-colour") << u"<table border=\"0\"><tr><td style=\"border:2px solid;\">L</td></tr></table>"_s
                               << u" style=\"border-bottom: 2px solid; border-top: 2px solid; "
                                  "border-left: 2px solid; border-right: 2px solid;\""_s;
    // A width with no style paints nothing in the document, and none keeps it that way.
    QTest::newRow("width-without-style") << u"<table border=\"0\"><tr><td style=\"border-width:2px;\">L</td></tr></table>"_s
                                         << u" style=\"border-bottom: 2px none; border-top: 2px none; "
                                            "border-left: 2px none; border-right: 2px none;\""_s;
    // Conversely a style with no width: css would paint a medium border, the explicit zero must not let it.
    QTest::newRow("style-without-width") << u"<table border=\"0\"><tr><td style=\"border-style:dotted;\">L</td></tr></table>"_s
                                         << u" style=\"border-bottom: 0 dotted; border-top: 0 dotted; "
                                            "border-left: 0 dotted; border-right: 0 dotted;\""_s;
}

void TextHTMLBuilderTest::testTableCellBorder()
{
    QFETCH(QString, html);
    QFETCH(QString, expectedStyle);

    QTextDocument doc;
    doc.setHtml(html);

    KPIMTextEdit::TextHTMLBuilder hb;
    KPIMTextEdit::MarkupDirector md(&hb);
    md.processDocument(&doc);
    const QString result = hb.getResult();

    QVERIFY2(result.contains(expectedStyle), qPrintable(result));
}

void TextHTMLBuilderTest::testTableCellBorderStyleKeyword_data()
{
    QTest::addColumn<QTextFrameFormat::BorderStyle>("borderStyle");
    QTest::addColumn<QString>("expectedKeyword");

    QTest::newRow("none") << QTextFrameFormat::BorderStyle_None << u"none"_s;
    QTest::newRow("dotted") << QTextFrameFormat::BorderStyle_Dotted << u"dotted"_s;
    QTest::newRow("dashed") << QTextFrameFormat::BorderStyle_Dashed << u"dashed"_s;
    QTest::newRow("solid") << QTextFrameFormat::BorderStyle_Solid << u"solid"_s;
    QTest::newRow("double") << QTextFrameFormat::BorderStyle_Double << u"double"_s;
    QTest::newRow("groove") << QTextFrameFormat::BorderStyle_Groove << u"groove"_s;
    QTest::newRow("ridge") << QTextFrameFormat::BorderStyle_Ridge << u"ridge"_s;
    QTest::newRow("inset") << QTextFrameFormat::BorderStyle_Inset << u"inset"_s;
    QTest::newRow("outset") << QTextFrameFormat::BorderStyle_Outset << u"outset"_s;
    // Css has no equivalent for these two, a dashed line keeps the side visible.
    QTest::newRow("dot-dash") << QTextFrameFormat::BorderStyle_DotDash << u"dashed"_s;
    QTest::newRow("dot-dot-dash") << QTextFrameFormat::BorderStyle_DotDotDash << u"dashed"_s;
}

void TextHTMLBuilderTest::testTableCellBorderStyleKeyword()
{
    QFETCH(QTextFrameFormat::BorderStyle, borderStyle);
    QFETCH(QString, expectedKeyword);

    QTextDocument doc;
    QTextCursor cursor(&doc);
    QTextTableFormat tableFormat;
    tableFormat.setColumnWidthConstraints({QTextLength()});
    QTextTable *table = cursor.insertTable(1, 1, tableFormat);
    QTextTableCellFormat cellFormat = table->cellAt(0, 0).format().toTableCellFormat();
    cellFormat.setTopBorder(2);
    cellFormat.setTopBorderStyle(borderStyle);
    table->cellAt(0, 0).setFormat(cellFormat);

    KPIMTextEdit::TextHTMLBuilder hb;
    KPIMTextEdit::MarkupDirector md(&hb);
    md.processDocument(&doc);
    const QString result = hb.getResult();

    QVERIFY2(result.contains(u"border-top: 2px %1"_s.arg(expectedKeyword)), qPrintable(result));
}

// A tag can only carry one style attribute: a second one is ignored, so a cell with both a
// padding and a border would silently lose the border.
void TextHTMLBuilderTest::testTableCellBorderAndPaddingShareOneStyleAttribute()
{
    QTextDocument doc;
    doc.setHtml(u"<table border=\"0\"><tr><td style=\"padding:5px;border:1px solid #abcdef;\">L</td></tr></table>"_s);

    KPIMTextEdit::TextHTMLBuilder hb;
    KPIMTextEdit::MarkupDirector md(&hb);
    md.processDocument(&doc);
    const QString result = hb.getResult();

    const int cellStart = result.indexOf(u"<td"_s);
    const int cellEnd = result.indexOf(u'>', cellStart);
    const QString cellTag = result.mid(cellStart, cellEnd - cellStart);
    QCOMPARE(cellTag.count(u" style=\""_s), 1);
    QVERIFY2(cellTag.contains(u"padding-top: 5px;"_s), qPrintable(cellTag));
    QVERIFY2(cellTag.contains(u"border-top: 1px solid #abcdef;"_s), qPrintable(cellTag));
}

void TextHTMLBuilderTest::testTableHeaderCellBorder()
{
    QTextDocument doc;
    doc.setHtml(
        u"<table border=\"0\"><thead><tr><th style=\"border-top:2px solid red;\">H</th></tr></thead>"
        "<tbody><tr><td>L</td></tr></tbody></table>"_s);

    KPIMTextEdit::TextHTMLBuilder hb;
    KPIMTextEdit::MarkupDirector md(&hb);
    md.processDocument(&doc);
    const QString result = hb.getResult();

    QVERIFY2(result.contains(u"<th colspan=\"1\" rowspan=\"1\" style=\"border-top: 2px solid #ff0000;\">"_s), qPrintable(result));
    // The body cell asked for nothing, so it must keep inheriting the table border attribute.
    QVERIFY2(result.contains(u"<td colspan=\"1\" rowspan=\"1\">"_s), qPrintable(result));
}

void TextHTMLBuilderTest::testOrderedListStart_data()
{
    QTest::addColumn<QString>("html");
    QTest::addColumn<QString>("expectedTag");

    // A list numbered from its first item is the default, so no start attribute is written.
    QTest::newRow("decimal-default") << u"<ol><li>a</li></ol>"_s << u"<ol type=\"1\">"_s;
    QTest::newRow("lower-alpha-default") << u"<ol type=\"a\"><li>a</li></ol>"_s << u"<ol type=\"a\">"_s;
    QTest::newRow("lower-roman-default") << u"<ol type=\"i\"><li>a</li></ol>"_s << u"<ol type=\"i\">"_s;

    QTest::newRow("decimal-start") << u"<ol start=\"5\"><li>a</li></ol>"_s << u"<ol type=\"1\" start=\"5\">"_s;
    QTest::newRow("lower-alpha-start") << u"<ol type=\"a\" start=\"3\"><li>a</li></ol>"_s << u"<ol type=\"a\" start=\"3\">"_s;
    QTest::newRow("upper-alpha-start") << u"<ol type=\"A\" start=\"3\"><li>a</li></ol>"_s << u"<ol type=\"A\" start=\"3\">"_s;
    QTest::newRow("lower-roman-start") << u"<ol type=\"i\" start=\"4\"><li>a</li></ol>"_s << u"<ol type=\"i\" start=\"4\">"_s;
    QTest::newRow("upper-roman-start") << u"<ol type=\"I\" start=\"4\"><li>a</li></ol>"_s << u"<ol type=\"I\" start=\"4\">"_s;

    // start has no meaning on an unordered list.
    QTest::newRow("unordered-ignores-start") << u"<ul start=\"5\"><li>a</li></ul>"_s << u"<ul type=\"disc\">"_s;
}

void TextHTMLBuilderTest::testOrderedListStart()
{
    QFETCH(QString, html);
    QFETCH(QString, expectedTag);

    QTextDocument doc;
    doc.setHtml(html);
    KPIMTextEdit::TextHTMLBuilder hb;
    KPIMTextEdit::MarkupDirector md(&hb);
    md.processDocument(&doc);
    const QString result = hb.getResult();

    QVERIFY2(result.contains(expectedTag), qPrintable(result));
}

void TextHTMLBuilderTest::testFontFamilyQuoting_data()
{
    QTest::addColumn<QString>("family");
    QTest::addColumn<QString>("expectedDeclaration");

    // A plain identifier is quoted too, which is what QTextDocument::toHtml() does as well.
    QTest::newRow("plain") << u"Arial"_s << u"font-family:'Arial';"_s;
    // A name holding a space is not a valid unquoted css identifier.
    QTest::newRow("spaces") << u"Courier New"_s << u"font-family:'Courier New';"_s;
    // A double quote is escaped for html only: inside a single quoted css string it is literal.
    QTest::newRow("double-quotes-and-comma") << u"My \"Odd\", Font"_s << u"font-family:'My &quot;Odd&quot;, Font';"_s;
    // A single quote would close the css string, so it is escaped for css.
    QTest::newRow("apostrophe") << u"O'Reilly Sans"_s << u"font-family:'O\\'Reilly Sans';"_s;
}

void TextHTMLBuilderTest::testFontFamilyQuoting()
{
    QFETCH(QString, family);
    QFETCH(QString, expectedDeclaration);

    QTextDocument doc;
    QTextCursor cursor(&doc);
    QTextCharFormat format;
    format.setFontFamilies({family});
    cursor.insertText(u"text"_s, format);

    KPIMTextEdit::TextHTMLBuilder hb;
    KPIMTextEdit::MarkupDirector md(&hb);
    md.processDocument(&doc);
    const QString result = hb.getResult();

    QVERIFY2(result.contains(expectedDeclaration), qPrintable(result));

    // The declaration must survive a round trip: a stray quote would close the style attribute
    // and the whole font family would be dropped instead.
    QTextDocument roundTrip;
    roundTrip.setHtml(result);
    QTextCursor back(&roundTrip);
    back.movePosition(QTextCursor::Right);
    QCOMPARE(back.charFormat().fontFamilies().toStringList().value(0), family);
}

void TextHTMLBuilderTest::testFontSpanReopenedAfterInterruption_data()
{
    QTest::addColumn<QString>("html");
    QTest::addColumn<QString>("expectedDeclaration");

    // Same problem as bug #442416, which was fixed for the colors only: the span state was kept
    // after the span was closed, so a third fragment asking for the size or the family it already
    // had lost it.
    QTest::newRow("point-size") << u"<p><span style=\"font-size:20pt;\">a</span>b<span style=\"font-size:20pt;\">c</span></p>"_s
                                << u"<span style=\"font-size:20pt;\">c</span>"_s;
    QTest::newRow("font-family") << u"<p><span style=\"font-family:Courier;\">a</span>b<span style=\"font-family:Courier;\">c</span></p>"_s
                                 << u"<span style=\"font-family:'Courier';\">c</span>"_s;
}

void TextHTMLBuilderTest::testFontSpanReopenedAfterInterruption()
{
    QFETCH(QString, html);
    QFETCH(QString, expectedDeclaration);

    QTextDocument doc;
    doc.setHtml(html);
    KPIMTextEdit::TextHTMLBuilder hb;
    KPIMTextEdit::MarkupDirector md(&hb);
    md.processDocument(&doc);
    const QString result = hb.getResult();

    QVERIFY2(result.contains(expectedDeclaration), qPrintable(result));
}

#include "moc_texthtmlbuildertest.cpp"
