/*
   SPDX-FileCopyrightText: 2020-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "plaintextmarkupbuildertest.h"
using namespace Qt::Literals::StringLiterals;

#include <KPIMTextEdit/MarkupDirector>
#include <KPIMTextEdit/PlainTextMarkupBuilder>
#include <QRegularExpression>
#include <QTest>
#include <QTextDocument>
QTEST_MAIN(PlainTextMarkupBuilderTest)

PlainTextMarkupBuilderTest::PlainTextMarkupBuilderTest(QObject *parent)
    : QObject(parent)
{
}

void PlainTextMarkupBuilderTest::testPlainText_data()
{
    QTest::addColumn<QString>("text");
    QTest::addColumn<QString>("regexpText");
    QTest::newRow("text with leading space") << QString() << QString();
    QTest::newRow("bold") << u"This <b>text</b> is bold."_s << u"^This \\*text\\* is bold.\\n$"_s;
    QTest::newRow("double format") << u"Some <b><i>formatted</i></b> text."_s << u"^Some (\\*/|/\\*)formatted(\\*/|/\\*) text.\\n$"_s;
    QTest::newRow("italic") << u"This <b>text is <i>italic</i> and</b> bold."_s << u"^This \\*text is /italic/ and\\* bold.\\n$"_s;
    QTest::newRow("double span") << QStringLiteral(
        "Some <span "
        "style=\"color:#ff0000;background-color:#00ff00;"
        "\">formatted</span> text.")
                                 << u"^Some formatted text.\\n$"_s;

    // FIXME bug is about id="..."
    QTest::newRow("bug345360") << QStringLiteral(
        "<html><body><center><table><tr><td><div id=\"foo\"><span>    Dear User,       </span> <br> <br> <span>    We     have updated our Privacy Policy and "
        "Terms of Service. Please take a    few  minutes to read and understand them. To help you understand, we    have  summarized the changes and provided "
        "a comparison of the current    and the  previous versions.       </span> <br> <br> <span>    These     changes will take effect on April 19, "
        "2015.&nbsp; If you continue to use     Zoho after April 19, 2015, you will be governed by the new Privacy     Policy and Terms of Service.       "
        "</span> <br> <br> <span>  ddd &nbsp;&nbsp;&nbsp;&nbsp;       </span></td></tr></table></center></body></html>")
                               << u"foo"_s;
}

void PlainTextMarkupBuilderTest::testPlainText()
{
    QFETCH(QString, text);
    QFETCH(QString, regexpText);
    QEXPECT_FAIL("bug345360", "Currently it failed", Continue);
    auto doc = new QTextDocument();
    doc->setHtml(text);

    auto hb = new KPIMTextEdit::PlainTextMarkupBuilder();
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

void PlainTextMarkupBuilderTest::testSingleFormat()
{
    auto doc = new QTextDocument();

    // One format
    doc->setHtml(u"This <b>text</b> is bold."_s);

    auto hb = new KPIMTextEdit::PlainTextMarkupBuilder();
    auto md = new KPIMTextEdit::MarkupDirector(hb);
    md->processDocument(doc);
    auto result = hb->getResult();
    QRegularExpression regex(u"^This \\*text\\* is bold.\\n$"_s);

    QVERIFY(regex.match(result).hasMatch());
    delete md;
    delete hb;
    delete doc;
}

void PlainTextMarkupBuilderTest::testDoubleFormat()
{
    auto doc = new QTextDocument();

    // One format
    doc->setHtml(u"Some <b><i>formatted</i></b> text."_s);

    auto hb = new KPIMTextEdit::PlainTextMarkupBuilder();
    auto md = new KPIMTextEdit::MarkupDirector(hb);
    md->processDocument(doc);
    auto result = hb->getResult();
    QRegularExpression regex(u"^Some (\\*/|/\\*)formatted(\\*/|/\\*) text.\\n$"_s);

    QVERIFY(regex.match(result).hasMatch());
    delete md;
    delete hb;
    delete doc;
}

void PlainTextMarkupBuilderTest::testAnchor()
{
    auto doc = new QTextDocument();
    doc->setHtml(u"A <a href=\"http://www.kde.org\">link</a> to KDE."_s);

    auto hb = new KPIMTextEdit::PlainTextMarkupBuilder();
    auto md = new KPIMTextEdit::MarkupDirector(hb);
    md->processDocument(doc);
    auto result = hb->getResult();

    QRegularExpression regex(u"^A link\\[1\\] to KDE.\\n\\n--------\\n\\[1\\] http://www.kde.org\\n$"_s);

    regex.match(result).hasMatch();

    QVERIFY(regex.match(result).hasMatch());
    delete md;
    delete hb;
    delete doc;
}

void PlainTextMarkupBuilderTest::testAnchorWithFormattedContent()
{
    auto doc = new QTextDocument();
    doc->setHtml(u"A <a href=\"http://www.kde.org\"><b>formatted</b> link</a> to KDE."_s);

    auto hb = new KPIMTextEdit::PlainTextMarkupBuilder();
    auto md = new KPIMTextEdit::MarkupDirector(hb);
    md->processDocument(doc);
    auto result = hb->getResult();

    QRegularExpression regex(
        QStringLiteral("^A \\*formatted\\* link\\[1\\] to "
                       "KDE.\\n\\n--------\\n\\[1\\] http://www.kde.org\\n$"));

    QVERIFY(regex.match(result).hasMatch());
    delete md;
    delete hb;
    delete doc;
}

void PlainTextMarkupBuilderTest::testAdjacentAnchors()
{
    auto doc = new QTextDocument();
    doc->setHtml(
        QStringLiteral("Two <a href=\"http://www.kde.org\">links</a><a "
                       "href=\"http://www.google.com\">next</a> to each other."));

    auto hb = new KPIMTextEdit::PlainTextMarkupBuilder();
    auto md = new KPIMTextEdit::MarkupDirector(hb);
    md->processDocument(doc);
    auto result = hb->getResult();

    QRegularExpression regex(
        QStringLiteral("^Two links\\[1\\]next\\[2\\] to each other.\\n\\n--------\\n\\[1\\] "
                       "http://www.kde.org\\n\\[2\\] http://www.google.com\\n$"));

    QVERIFY(regex.match(result).hasMatch());
    delete md;
    delete hb;
    delete doc;
}

void PlainTextMarkupBuilderTest::testNestedFormatting()
{
    auto doc = new QTextDocument();
    doc->setHtml(u"This <b>text is <i>italic</i> and</b> bold."_s);

    auto hb = new KPIMTextEdit::PlainTextMarkupBuilder();
    auto md = new KPIMTextEdit::MarkupDirector(hb);
    md->processDocument(doc);
    auto result = hb->getResult();

    QRegularExpression regex(u"^This \\*text is /italic/ and\\* bold.\\n$"_s);

    QVERIFY(regex.match(result).hasMatch());
    delete md;
    delete hb;
    delete doc;
}

void PlainTextMarkupBuilderTest::testSpan()
{
    auto doc = new QTextDocument();
    doc->setHtml(u"Some <span style=\"color:#ff0000;\">formatted</span> text."_s);

    auto hb = new KPIMTextEdit::PlainTextMarkupBuilder();
    auto md = new KPIMTextEdit::MarkupDirector(hb);
    md->processDocument(doc);
    auto result = hb->getResult();

    auto regex = QRegularExpression(u"^Some formatted text.\\n$"_s);

    QVERIFY(regex.match(result).hasMatch());
    delete md;
    delete hb;
    delete doc;
}

void PlainTextMarkupBuilderTest::testDoubleSpan()
{
    auto doc = new QTextDocument();
    doc->setHtml(
        QStringLiteral("Some <span "
                       "style=\"color:#ff0000;background-color:#00ff00;"
                       "\">formatted</span> text."));

    auto hb = new KPIMTextEdit::PlainTextMarkupBuilder();
    auto md = new KPIMTextEdit::MarkupDirector(hb);
    md->processDocument(doc);
    auto result = hb->getResult();

    auto regex = QRegularExpression(u"^Some formatted text.\\n$"_s);

    QVERIFY(regex.match(result).hasMatch());
    delete md;
    delete hb;
    delete doc;
}

void PlainTextMarkupBuilderTest::testSpanNesting()
{
    auto doc = new QTextDocument();
    doc->setHtml(
        QStringLiteral("Paragraph <span style=\"background-color:#00ff00;\">with some <span "
                       "style=\"color:#ff0000;\">formatted</span> nested</span> text."));

    auto hb = new KPIMTextEdit::PlainTextMarkupBuilder();
    auto md = new KPIMTextEdit::MarkupDirector(hb);
    md->processDocument(doc);
    auto result = hb->getResult();

    auto regex = QRegularExpression(u"^Paragraph with some formatted nested text.\\n$"_s);

    QVERIFY(regex.match(result).hasMatch());
    delete md;
    delete hb;
    delete doc;
}

void PlainTextMarkupBuilderTest::testDoubleStartDifferentFinish()
{
    auto doc = new QTextDocument();
    doc->setHtml(u"Paragraph <i><b>with</b> some formatted</i> text."_s);

    auto hb = new KPIMTextEdit::PlainTextMarkupBuilder();
    auto md = new KPIMTextEdit::MarkupDirector(hb);
    md->processDocument(doc);
    auto result = hb->getResult();

    auto regex = QRegularExpression(u"^Paragraph /\\*with\\* some formatted/ text.\\n$"_s);

    QVERIFY(regex.match(result).hasMatch());
    delete md;
    delete hb;
    delete doc;
}

void PlainTextMarkupBuilderTest::testDoubleStartDifferentFinishReverseOrder()
{
    auto doc = new QTextDocument();
    doc->setHtml(u"Paragraph <b><i>with</i> some formatted</b> text."_s);

    auto hb = new KPIMTextEdit::PlainTextMarkupBuilder();
    auto md = new KPIMTextEdit::MarkupDirector(hb);
    md->processDocument(doc);
    auto result = hb->getResult();

    auto regex = QRegularExpression(u"^Paragraph \\*/with/ some formatted\\* text.\\n$"_s);

    QVERIFY(regex.match(result).hasMatch());
    delete md;
    delete hb;
    delete doc;
}

void PlainTextMarkupBuilderTest::testDifferentStartDoubleFinish()
{
    auto doc = new QTextDocument();
    doc->setHtml(u"Paragraph <i>with some <b>formatted<b></i> text."_s);

    auto hb = new KPIMTextEdit::PlainTextMarkupBuilder();
    auto md = new KPIMTextEdit::MarkupDirector(hb);
    md->processDocument(doc);
    auto result = hb->getResult();

    auto regex = QRegularExpression(u"^Paragraph /with some \\*formatted\\*/ text.\\n$"_s);

    QVERIFY(regex.match(result).hasMatch());
    delete md;
    delete hb;
    delete doc;
}

void PlainTextMarkupBuilderTest::testDifferentStartDoubleFinishReverseOrder()
{
    auto doc = new QTextDocument();
    doc->setHtml(u"Paragraph <b>with some <i>formatted</i></b> text."_s);

    auto hb = new KPIMTextEdit::PlainTextMarkupBuilder();
    auto md = new KPIMTextEdit::MarkupDirector(hb);
    md->processDocument(doc);
    auto result = hb->getResult();

    auto regex = QRegularExpression(u"^Paragraph \\*with some /formatted/\\* text.\\n$"_s);

    QVERIFY(regex.match(result).hasMatch());
    delete md;
    delete hb;
    delete doc;
}

void PlainTextMarkupBuilderTest::testOverlap()
{
    auto doc = new QTextDocument();
    doc->setHtml(u"Paragraph <b>with <i>some</i></b><i> formatted</i> text."_s);

    auto hb = new KPIMTextEdit::PlainTextMarkupBuilder();
    auto md = new KPIMTextEdit::MarkupDirector(hb);
    md->processDocument(doc);
    auto result = hb->getResult();

    auto regex = QRegularExpression(u"^Paragraph \\*with /some/\\*/ formatted/ text.\\n$"_s);

    QVERIFY(regex.match(result).hasMatch());
    delete md;
    delete hb;
    delete doc;
}

void PlainTextMarkupBuilderTest::testEdgeCaseLeft()
{
    auto doc = new QTextDocument();
    doc->setHtml(u"Paragraph <b>with some formatted text.</b>"_s);

    auto hb = new KPIMTextEdit::PlainTextMarkupBuilder();
    auto md = new KPIMTextEdit::MarkupDirector(hb);
    md->processDocument(doc);
    auto result = hb->getResult();

    auto regex = QRegularExpression(u"^Paragraph \\*with some formatted text.\\*\\n$"_s);

    QVERIFY(regex.match(result).hasMatch());
    delete md;
    delete hb;
    delete doc;
}

void PlainTextMarkupBuilderTest::testEdgeCaseRight()
{
    auto doc = new QTextDocument();
    doc->setHtml(u"<b>Paragraph with some formatted</b> text."_s);

    auto hb = new KPIMTextEdit::PlainTextMarkupBuilder();
    auto md = new KPIMTextEdit::MarkupDirector(hb);
    md->processDocument(doc);
    auto result = hb->getResult();

    auto regex = QRegularExpression(u"^\\*Paragraph with some formatted\\* text.\\n$"_s);

    QVERIFY(regex.match(result).hasMatch());
    delete md;
    delete hb;
    delete doc;
}

void PlainTextMarkupBuilderTest::testImage()
{
    auto doc = new QTextDocument();
    doc->setHtml(
        QStringLiteral("Paragraph with an inline <img "
                       "src=\"http://kde.org/img/kde41.png\" /> image."));

    auto hb = new KPIMTextEdit::PlainTextMarkupBuilder();
    auto md = new KPIMTextEdit::MarkupDirector(hb);
    md->processDocument(doc);
    auto result = hb->getResult();

    auto regex =
        QRegularExpression(QStringLiteral("^Paragraph with an inline \\[1\\] image.\\n\\n--------\\n\\[1\\] "
                                          "http://kde.org/img/kde41.png\\n$"));

    QVERIFY(regex.match(result).hasMatch());
    delete md;
    delete hb;
    delete doc;
}

void PlainTextMarkupBuilderTest::testImageResized()
{
    QString result;
    QRegularExpression regex;
    KPIMTextEdit::PlainTextMarkupBuilder *hb;
    KPIMTextEdit::MarkupDirector *md;
    auto doc = new QTextDocument();

    // width
    doc->setHtml(
        QStringLiteral("Paragraph with an inline <img src=\"http://kde.org/img/kde41.png\" "
                       "width=\"10\" /> image."));

    hb = new KPIMTextEdit::PlainTextMarkupBuilder();
    md = new KPIMTextEdit::MarkupDirector(hb);
    md->processDocument(doc);
    result = hb->getResult();

    regex =
        QRegularExpression(QStringLiteral("^Paragraph with an inline \\[1\\] image.\\n\\n--------\\n\\[1\\] "
                                          "http://kde.org/img/kde41.png\\n$"));
    QVERIFY(regex.match(result).hasMatch());

    // height
    doc->setHtml(
        QStringLiteral("Paragraph with an inline <img src=\"http://kde.org/img/kde41.png\" "
                       "height=\"10\" /> image."));

    delete md;
    delete hb;

    hb = new KPIMTextEdit::PlainTextMarkupBuilder();
    md = new KPIMTextEdit::MarkupDirector(hb);
    md->processDocument(doc);
    result = hb->getResult();

    regex =
        QRegularExpression(QStringLiteral("^Paragraph with an inline \\[1\\] image.\\n\\n--------\\n\\[1\\] "
                                          "http://kde.org/img/kde41.png\\n$"));
    QVERIFY(regex.match(result).hasMatch());

    // height and width
    doc->setHtml(
        QStringLiteral("Paragraph with an inline <img src=\"http://kde.org/img/kde41.png\" "
                       "height=\"10\" width=\"10\" /> image."));

    delete md;
    delete hb;

    hb = new KPIMTextEdit::PlainTextMarkupBuilder();
    md = new KPIMTextEdit::MarkupDirector(hb);
    md->processDocument(doc);
    result = hb->getResult();

    regex =
        QRegularExpression(QStringLiteral("^Paragraph with an inline \\[1\\] image.\\n\\n--------\\n\\[1\\] "
                                          "http://kde.org/img/kde41.png\\n$"));

    QVERIFY(regex.match(result).hasMatch());
    delete md;
    delete hb;
    delete doc;
}

void PlainTextMarkupBuilderTest::testEachFormatTagSingly()
{
    QString result;
    QRegularExpression regex;
    KPIMTextEdit::PlainTextMarkupBuilder *hb;
    KPIMTextEdit::MarkupDirector *md;
    auto doc = new QTextDocument();

    // Test bold
    doc->setHtml(u"Some <b>formatted</b> text."_s);
    hb = new KPIMTextEdit::PlainTextMarkupBuilder();
    md = new KPIMTextEdit::MarkupDirector(hb);
    md->processDocument(doc);
    result = hb->getResult();

    regex = QRegularExpression(u"^Some \\*formatted\\* text.\\n$"_s);
    QVERIFY(regex.match(result).hasMatch());

    // Test Italic
    doc->setHtml(u"Some <i>formatted</i> text."_s);
    delete md;
    delete hb;

    hb = new KPIMTextEdit::PlainTextMarkupBuilder();
    md = new KPIMTextEdit::MarkupDirector(hb);
    md->processDocument(doc);
    result = hb->getResult();

    regex = QRegularExpression(u"^Some /formatted/ text.\\n$"_s);
    QVERIFY(regex.match(result).hasMatch());

    // Test Underline
    doc->setHtml(u"Some <u>formatted</u> text."_s);
    delete md;
    delete hb;

    hb = new KPIMTextEdit::PlainTextMarkupBuilder();
    md = new KPIMTextEdit::MarkupDirector(hb);
    md->processDocument(doc);
    result = hb->getResult();

    regex = QRegularExpression(u"^Some _formatted_ text.\\n$"_s);
    QVERIFY(regex.match(result).hasMatch());

    // Test Strikeout
    doc->setHtml(u"Some <s>formatted</s> text."_s);
    delete md;
    delete hb;

    hb = new KPIMTextEdit::PlainTextMarkupBuilder();
    md = new KPIMTextEdit::MarkupDirector(hb);
    md->processDocument(doc);
    result = hb->getResult();

    regex = QRegularExpression(u"^Some -formatted- text.\\n$"_s);
    QVERIFY(regex.match(result).hasMatch());

    // Test Superscript
    doc->setHtml(u"Some <sup>formatted</sup> text."_s);
    delete md;
    delete hb;

    hb = new KPIMTextEdit::PlainTextMarkupBuilder();
    md = new KPIMTextEdit::MarkupDirector(hb);
    md->processDocument(doc);
    result = hb->getResult();

    regex = QRegularExpression(u"^Some \\^\\{formatted\\} text.\\n$"_s);
    QVERIFY(regex.match(result).hasMatch());

    // Test Subscript
    doc->setHtml(u"Some <sub>formatted</sub> text."_s);
    delete md;
    delete hb;

    hb = new KPIMTextEdit::PlainTextMarkupBuilder();
    md = new KPIMTextEdit::MarkupDirector(hb);
    md->processDocument(doc);
    result = hb->getResult();

    regex = QRegularExpression(u"^Some _\\{formatted\\} text.\\n$"_s);
    QVERIFY(regex.match(result).hasMatch());

    // Test Foreground
    doc->setHtml(u"Some <span style=\"color:#ff0000;\">formatted</span> text."_s);
    delete md;
    delete hb;

    hb = new KPIMTextEdit::PlainTextMarkupBuilder();
    md = new KPIMTextEdit::MarkupDirector(hb);
    md->processDocument(doc);
    result = hb->getResult();

    regex = QRegularExpression(u"^Some formatted text.\\n$"_s);
    QVERIFY(regex.match(result).hasMatch());

    // Test Background
    doc->setHtml(u"Some <span style=\"background-color:#ff0000;\">formatted</span> text."_s);
    delete md;
    delete hb;

    hb = new KPIMTextEdit::PlainTextMarkupBuilder();
    md = new KPIMTextEdit::MarkupDirector(hb);
    md->processDocument(doc);
    result = hb->getResult();

    regex = QRegularExpression(u"^Some formatted text.\\n$"_s);
    QVERIFY(regex.match(result).hasMatch());

    // Test Font Family
    doc->setHtml(u"Some <span style=\"font-family:courier;\">formatted</span> text."_s);
    delete md;
    delete hb;

    hb = new KPIMTextEdit::PlainTextMarkupBuilder();
    md = new KPIMTextEdit::MarkupDirector(hb);
    md->processDocument(doc);
    result = hb->getResult();

    regex = QRegularExpression(u"^Some formatted text.\\n$"_s);
    QVERIFY(regex.match(result).hasMatch());

    // Test Font Size
    doc->setHtml(u"Some <span style=\"font-size:20pt;\">formatted</span> text."_s);
    delete md;
    delete hb;

    hb = new KPIMTextEdit::PlainTextMarkupBuilder();
    md = new KPIMTextEdit::MarkupDirector(hb);
    md->processDocument(doc);
    result = hb->getResult();

    regex = QRegularExpression(u"^Some formatted text.\\n$"_s);
    QVERIFY(regex.match(result).hasMatch());
    delete md;
    delete hb;
    delete doc;
}

void PlainTextMarkupBuilderTest::testHorizontalRule()
{
    auto doc = new QTextDocument();
    doc->setHtml(
        QStringLiteral("<p style=\"margin-top:0;margin-bottom:0;\">Foo</p><hr "
                       "/><p style=\"margin-top:0;margin-bottom:0;\">Bar</p>"));

    auto hb = new KPIMTextEdit::PlainTextMarkupBuilder();
    auto md = new KPIMTextEdit::MarkupDirector(hb);
    md->processDocument(doc);
    auto result = hb->getResult();

    auto regex = QRegularExpression(u"^Foo\\n--------------------\\nBar\\n$"_s);

    QVERIFY(regex.match(result).hasMatch());
    delete md;
    delete hb;
    delete doc;
}

void PlainTextMarkupBuilderTest::testNewlines()
{
    auto doc = new QTextDocument();
    doc->setHtml(u"<p>Foo</p>\n<br /><br />\n<p>Bar</p>"_s);

    auto hb = new KPIMTextEdit::PlainTextMarkupBuilder();
    auto md = new KPIMTextEdit::MarkupDirector(hb);
    md->processDocument(doc);
    auto result = hb->getResult();

    auto regex = QRegularExpression(u"^Foo\\n\\n\\nBar\\n$"_s);

    QVERIFY(regex.match(result).hasMatch());
    delete md;
    delete hb;
    delete doc;
}

void PlainTextMarkupBuilderTest::testEmptyParagraphs()
{
    auto doc = new QTextDocument();
    doc->setHtml(u"<p>Foo</p>\n<br /><br />\n<p>Bar</p>"_s);

    auto hb = new KPIMTextEdit::PlainTextMarkupBuilder();
    auto md = new KPIMTextEdit::MarkupDirector(hb);
    md->processDocument(doc);
    auto result = hb->getResult();

    auto regex = QRegularExpression(u"^Foo\\n\\n\\nBar\\n$"_s);

    QVERIFY(regex.match(result).hasMatch());
    delete md;
    delete hb;
    delete doc;
}

void PlainTextMarkupBuilderTest::testNewlinesThroughQTextCursor()
{
    auto doc = new QTextDocument(this);
    QTextCursor cursor(doc);
    cursor.movePosition(QTextCursor::Start);
    cursor.insertText(u"Foo"_s);
    cursor.insertText(u"\n"_s);
    cursor.insertText(u"\n"_s);
    cursor.insertText(u"\n"_s);
    cursor.insertText(u"Bar"_s);

    auto hb = new KPIMTextEdit::PlainTextMarkupBuilder();
    auto md = new KPIMTextEdit::MarkupDirector(hb);
    md->processDocument(doc);
    auto result = hb->getResult();

    auto regex = QRegularExpression(u"^Foo\\n\\n\\nBar\\n$"_s);

    QVERIFY(regex.match(result).hasMatch());
    delete md;
    delete hb;
    delete doc;
}

void PlainTextMarkupBuilderTest::testBrInsideParagraph()
{
    auto doc = new QTextDocument();
    doc->setHtml(u"<p>Foo<br /><br /><br />Bar</p>"_s);

    auto hb = new KPIMTextEdit::PlainTextMarkupBuilder();
    auto md = new KPIMTextEdit::MarkupDirector(hb);
    md->processDocument(doc);
    auto result = hb->getResult();

    // Two paragraphs separated by two line breaks

    auto regex = QRegularExpression(u"^Foo\\n\\n\\nBar\\n$"_s);

    QVERIFY(regex.match(result).hasMatch());
    delete md;
    delete hb;
    delete doc;
}

void PlainTextMarkupBuilderTest::testLongDocument()
{
    QTextDocument doc;

    QFile sourceHtml(QFINDTESTDATA("sourcehtml"));
    QVERIFY(sourceHtml.open(QIODevice::ReadOnly));
    doc.setHtml(QString::fromLatin1(sourceHtml.readAll().constData()));

    auto hb = new KPIMTextEdit::PlainTextMarkupBuilder();
    auto md = new KPIMTextEdit::MarkupDirector(hb);
    md->processDocument(&doc);
    auto result = hb->getResult();
    QVERIFY2(result.startsWith(QLatin1StringView("Hello,\nThis is some text\nIt shows how "
                                                 "grantlee is used from kmail\n")),
             qPrintable(result));
    QVERIFY2(result.endsWith("This is the end of the signature\n"_L1), qPrintable(result));
    // qDebug() << result;
    delete md;
    delete hb;
}

void PlainTextMarkupBuilderTest::testBrInsideAnchor()
{
    auto doc = new QTextDocument();
    doc->setHtml(QStringLiteral(
        "<html><body><p >url: <a href=\"https://www.kde.org\"><span>foo</span></a><br />line1<br />line2</p><p>-- </p><p>bla</p><br /></p></body></html>"));

    auto hb = new KPIMTextEdit::PlainTextMarkupBuilder();
    auto md = new KPIMTextEdit::MarkupDirector(hb);
    md->processDocument(doc);
    auto result = hb->getResult();

    auto regex = QRegularExpression(u"^url: foo\\[1\\]\\nline1\\nline2\\n-- \\nbla\\n\n\\n\\n--------\\n\\[1\\] https://www.kde.org\\n$"_s);
    QVERIFY(regex.match(result).hasMatch());
    delete md;
    delete hb;
    delete doc;
}

void PlainTextMarkupBuilderTest::testNestedList()
{
    QTextDocument doc;
    doc.setHtml(
        QStringLiteral("<ol>\n"
                       "  <li>Elem1</li>\n"
                       "  <li>Elem2\n"
                       "    <ul>\n"
                       "      <li>Subelem1</li>\n"
                       "      <li>Subelem2</li>\n"
                       "    </ul>\n"
                       "  </li>\n"
                       "  <li>Elem3</li>\n"
                       "  <li>Elem4\n"
                       "    <ul>\n"
                       "      <li>Subelem3</li>\n"
                       "      <li>Subelem4</li>\n"
                       "    </ul>\n"
                       "  </li>\n"
                       "</ol>"
                       "<ul type=\"circle\">\n"
                       "  <li>Elem5</li>\n"
                       "</ul>"));
    auto hb = std::make_unique<KPIMTextEdit::PlainTextMarkupBuilder>();
    auto md = std::make_unique<KPIMTextEdit::MarkupDirector>(hb.get());
    md->processDocument(&doc);
    auto result = hb->getResult();

    const QString expected = QStringLiteral(
        "     1. Elem1 \n"
        "     2. Elem2 \n"
        "         *  Subelem1 \n"
        "         *  Subelem2 \n"
        "     3. Elem3 \n"
        "     4. Elem4 \n"
        "         *  Subelem3 \n"
        "         *  Subelem4 \n"
        "     o  Elem5\n");
    QCOMPARE(result, expected);
}

#include "moc_plaintextmarkupbuildertest.cpp"
