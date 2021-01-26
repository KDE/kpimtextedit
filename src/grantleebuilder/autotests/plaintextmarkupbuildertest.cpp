/*
   SPDX-FileCopyrightText: 2020-2021 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "plaintextmarkupbuildertest.h"
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
    QTest::newRow("text with espace at begin") << QString() << QString();
    QTest::newRow("bold") << QStringLiteral("This <b>text</b> is bold.") << QStringLiteral("^This \\*text\\* is bold.\\n$");
    QTest::newRow("double format") << QStringLiteral("Some <b><i>formatted</i></b> text.") << QStringLiteral("^Some (\\*/|/\\*)formatted(\\*/|/\\*) text.\\n$");
    QTest::newRow("italic") << QStringLiteral("This <b>text is <i>italic</i> and</b> bold.") << QStringLiteral("^This \\*text is /italic/ and\\* bold.\\n$");
    QTest::newRow("double span") << QStringLiteral(
        "Some <span "
        "style=\"color:#ff0000;background-color:#00ff00;"
        "\">formatted</span> text.")
                                 << QStringLiteral("^Some formatted text.\\n$");

    // FIXME bug is about id="..."
    QTest::newRow("bug345360") << QStringLiteral(
        "<html><body><center><table><tr><td><div id=\"foo\"><span>    Dear User,       </span> <br> <br> <span>    We     have updated our Privacy Policy and "
        "Terms of Service. Please take a    few  minutes to read and understand them. To help you understand, we    have  summarized the changes and provided "
        "a comparison of the current    and the  previous versions.       </span> <br> <br> <span>    These     changes will take effect on April 19, "
        "2015.&nbsp; If you continue to use     Zoho after April 19, 2015, you will be governed by the new Privacy     Policy and Terms of Service.       "
        "</span> <br> <br> <span>  ddd &nbsp;&nbsp;&nbsp;&nbsp;       </span></td></tr></table></center></body></html>")
                               << QStringLiteral("foo");
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
    doc->setHtml(QStringLiteral("This <b>text</b> is bold."));

    auto hb = new KPIMTextEdit::PlainTextMarkupBuilder();
    auto md = new KPIMTextEdit::MarkupDirector(hb);
    md->processDocument(doc);
    auto result = hb->getResult();
    QRegularExpression regex(QStringLiteral("^This \\*text\\* is bold.\\n$"));

    QVERIFY(regex.match(result).hasMatch());
    delete md;
    delete hb;
    delete doc;
}

void PlainTextMarkupBuilderTest::testDoubleFormat()
{
    auto doc = new QTextDocument();

    // One format
    doc->setHtml(QStringLiteral("Some <b><i>formatted</i></b> text."));

    auto hb = new KPIMTextEdit::PlainTextMarkupBuilder();
    auto md = new KPIMTextEdit::MarkupDirector(hb);
    md->processDocument(doc);
    auto result = hb->getResult();
    QRegularExpression regex(QStringLiteral("^Some (\\*/|/\\*)formatted(\\*/|/\\*) text.\\n$"));

    QVERIFY(regex.match(result).hasMatch());
    delete md;
    delete hb;
    delete doc;
}

void PlainTextMarkupBuilderTest::testAnchor()
{
    auto doc = new QTextDocument();
    doc->setHtml(QStringLiteral("A <a href=\"http://www.kde.org\">link</a> to KDE."));

    auto hb = new KPIMTextEdit::PlainTextMarkupBuilder();
    auto md = new KPIMTextEdit::MarkupDirector(hb);
    md->processDocument(doc);
    auto result = hb->getResult();

    QRegularExpression regex(QStringLiteral("^A link\\[1\\] to KDE.\\n\\n--------\\n\\[1\\] http://www.kde.org\\n$"));

    regex.match(result).hasMatch();

    QVERIFY(regex.match(result).hasMatch());
    delete md;
    delete hb;
    delete doc;
}

void PlainTextMarkupBuilderTest::testAnchorWithFormattedContent()
{
    auto doc = new QTextDocument();
    doc->setHtml(QStringLiteral("A <a href=\"http://www.kde.org\"><b>formatted</b> link</a> to KDE."));

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
                       "href=\"http://www.google.com\">next</a> to eachother."));

    auto hb = new KPIMTextEdit::PlainTextMarkupBuilder();
    auto md = new KPIMTextEdit::MarkupDirector(hb);
    md->processDocument(doc);
    auto result = hb->getResult();

    QRegularExpression regex(
        QStringLiteral("^Two links\\[1\\]next\\[2\\] to eachother.\\n\\n--------\\n\\[1\\] "
                       "http://www.kde.org\\n\\[2\\] http://www.google.com\\n$"));

    QVERIFY(regex.match(result).hasMatch());
    delete md;
    delete hb;
    delete doc;
}

void PlainTextMarkupBuilderTest::testNestedFormatting()
{
    auto doc = new QTextDocument();
    doc->setHtml(QStringLiteral("This <b>text is <i>italic</i> and</b> bold."));

    auto hb = new KPIMTextEdit::PlainTextMarkupBuilder();
    auto md = new KPIMTextEdit::MarkupDirector(hb);
    md->processDocument(doc);
    auto result = hb->getResult();

    QRegularExpression regex(QStringLiteral("^This \\*text is /italic/ and\\* bold.\\n$"));

    QVERIFY(regex.match(result).hasMatch());
    delete md;
    delete hb;
    delete doc;
}

void PlainTextMarkupBuilderTest::testSpan()
{
    auto doc = new QTextDocument();
    doc->setHtml(QStringLiteral("Some <span style=\"color:#ff0000;\">formatted</span> text."));

    auto hb = new KPIMTextEdit::PlainTextMarkupBuilder();
    auto md = new KPIMTextEdit::MarkupDirector(hb);
    md->processDocument(doc);
    auto result = hb->getResult();

    auto regex = QRegularExpression(QStringLiteral("^Some formatted text.\\n$"));

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

    auto regex = QRegularExpression(QStringLiteral("^Some formatted text.\\n$"));

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

    auto regex = QRegularExpression(QStringLiteral("^Paragraph with some formatted nested text.\\n$"));

    QVERIFY(regex.match(result).hasMatch());
    delete md;
    delete hb;
    delete doc;
}

void PlainTextMarkupBuilderTest::testDoubleStartDifferentFinish()
{
    auto doc = new QTextDocument();
    doc->setHtml(QStringLiteral("Paragraph <i><b>with</b> some formatted</i> text."));

    auto hb = new KPIMTextEdit::PlainTextMarkupBuilder();
    auto md = new KPIMTextEdit::MarkupDirector(hb);
    md->processDocument(doc);
    auto result = hb->getResult();

    auto regex = QRegularExpression(QStringLiteral("^Paragraph /\\*with\\* some formatted/ text.\\n$"));

    QVERIFY(regex.match(result).hasMatch());
    delete md;
    delete hb;
    delete doc;
}

void PlainTextMarkupBuilderTest::testDoubleStartDifferentFinishReverseOrder()
{
    auto doc = new QTextDocument();
    doc->setHtml(QStringLiteral("Paragraph <b><i>with</i> some formatted</b> text."));

    auto hb = new KPIMTextEdit::PlainTextMarkupBuilder();
    auto md = new KPIMTextEdit::MarkupDirector(hb);
    md->processDocument(doc);
    auto result = hb->getResult();

    auto regex = QRegularExpression(QStringLiteral("^Paragraph \\*/with/ some formatted\\* text.\\n$"));

    QVERIFY(regex.match(result).hasMatch());
    delete md;
    delete hb;
    delete doc;
}

void PlainTextMarkupBuilderTest::testDifferentStartDoubleFinish()
{
    auto doc = new QTextDocument();
    doc->setHtml(QStringLiteral("Paragraph <i>with some <b>formatted<b></i> text."));

    auto hb = new KPIMTextEdit::PlainTextMarkupBuilder();
    auto md = new KPIMTextEdit::MarkupDirector(hb);
    md->processDocument(doc);
    auto result = hb->getResult();

    auto regex = QRegularExpression(QStringLiteral("^Paragraph /with some \\*formatted\\*/ text.\\n$"));

    QVERIFY(regex.match(result).hasMatch());
    delete md;
    delete hb;
    delete doc;
}

void PlainTextMarkupBuilderTest::testDifferentStartDoubleFinishReverseOrder()
{
    auto doc = new QTextDocument();
    doc->setHtml(QStringLiteral("Paragraph <b>with some <i>formatted</i></b> text."));

    auto hb = new KPIMTextEdit::PlainTextMarkupBuilder();
    auto md = new KPIMTextEdit::MarkupDirector(hb);
    md->processDocument(doc);
    auto result = hb->getResult();

    auto regex = QRegularExpression(QStringLiteral("^Paragraph \\*with some /formatted/\\* text.\\n$"));

    QVERIFY(regex.match(result).hasMatch());
    delete md;
    delete hb;
    delete doc;
}

void PlainTextMarkupBuilderTest::testOverlap()
{
    auto doc = new QTextDocument();
    doc->setHtml(QStringLiteral("Paragraph <b>with <i>some</i></b><i> formatted</i> text."));

    auto hb = new KPIMTextEdit::PlainTextMarkupBuilder();
    auto md = new KPIMTextEdit::MarkupDirector(hb);
    md->processDocument(doc);
    auto result = hb->getResult();

    auto regex = QRegularExpression(QStringLiteral("^Paragraph \\*with /some/\\*/ formatted/ text.\\n$"));

    QVERIFY(regex.match(result).hasMatch());
    delete md;
    delete hb;
    delete doc;
}

void PlainTextMarkupBuilderTest::testEdgeCaseLeft()
{
    auto doc = new QTextDocument();
    doc->setHtml(QStringLiteral("Paragraph <b>with some formatted text.</b>"));

    auto hb = new KPIMTextEdit::PlainTextMarkupBuilder();
    auto md = new KPIMTextEdit::MarkupDirector(hb);
    md->processDocument(doc);
    auto result = hb->getResult();

    auto regex = QRegularExpression(QStringLiteral("^Paragraph \\*with some formatted text.\\*\\n$"));

    QVERIFY(regex.match(result).hasMatch());
    delete md;
    delete hb;
    delete doc;
}

void PlainTextMarkupBuilderTest::testEdgeCaseRight()
{
    auto doc = new QTextDocument();
    doc->setHtml(QStringLiteral("<b>Paragraph with some formatted</b> text."));

    auto hb = new KPIMTextEdit::PlainTextMarkupBuilder();
    auto md = new KPIMTextEdit::MarkupDirector(hb);
    md->processDocument(doc);
    auto result = hb->getResult();

    auto regex = QRegularExpression(QStringLiteral("^\\*Paragraph with some formatted\\* text.\\n$"));

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
    doc->setHtml(QStringLiteral("Some <b>formatted</b> text."));
    hb = new KPIMTextEdit::PlainTextMarkupBuilder();
    md = new KPIMTextEdit::MarkupDirector(hb);
    md->processDocument(doc);
    result = hb->getResult();

    regex = QRegularExpression(QStringLiteral("^Some \\*formatted\\* text.\\n$"));
    QVERIFY(regex.match(result).hasMatch());

    // Test Italic
    doc->setHtml(QStringLiteral("Some <i>formatted</i> text."));
    delete md;
    delete hb;

    hb = new KPIMTextEdit::PlainTextMarkupBuilder();
    md = new KPIMTextEdit::MarkupDirector(hb);
    md->processDocument(doc);
    result = hb->getResult();

    regex = QRegularExpression(QStringLiteral("^Some /formatted/ text.\\n$"));
    QVERIFY(regex.match(result).hasMatch());

    // Test Underline
    doc->setHtml(QStringLiteral("Some <u>formatted</u> text."));
    delete md;
    delete hb;

    hb = new KPIMTextEdit::PlainTextMarkupBuilder();
    md = new KPIMTextEdit::MarkupDirector(hb);
    md->processDocument(doc);
    result = hb->getResult();

    regex = QRegularExpression(QStringLiteral("^Some _formatted_ text.\\n$"));
    QVERIFY(regex.match(result).hasMatch());

    // Test Strikeout
    doc->setHtml(QStringLiteral("Some <s>formatted</s> text."));
    delete md;
    delete hb;

    hb = new KPIMTextEdit::PlainTextMarkupBuilder();
    md = new KPIMTextEdit::MarkupDirector(hb);
    md->processDocument(doc);
    result = hb->getResult();

    regex = QRegularExpression(QStringLiteral("^Some -formatted- text.\\n$"));
    QVERIFY(regex.match(result).hasMatch());

    // Test Superscript
    doc->setHtml(QStringLiteral("Some <sup>formatted</sup> text."));
    delete md;
    delete hb;

    hb = new KPIMTextEdit::PlainTextMarkupBuilder();
    md = new KPIMTextEdit::MarkupDirector(hb);
    md->processDocument(doc);
    result = hb->getResult();

    regex = QRegularExpression(QStringLiteral("^Some \\^\\{formatted\\} text.\\n$"));
    QVERIFY(regex.match(result).hasMatch());

    // Test Subscript
    doc->setHtml(QStringLiteral("Some <sub>formatted</sub> text."));
    delete md;
    delete hb;

    hb = new KPIMTextEdit::PlainTextMarkupBuilder();
    md = new KPIMTextEdit::MarkupDirector(hb);
    md->processDocument(doc);
    result = hb->getResult();

    regex = QRegularExpression(QStringLiteral("^Some _\\{formatted\\} text.\\n$"));
    QVERIFY(regex.match(result).hasMatch());

    // Test Foreground
    doc->setHtml(QStringLiteral("Some <span style=\"color:#ff0000;\">formatted</span> text."));
    delete md;
    delete hb;

    hb = new KPIMTextEdit::PlainTextMarkupBuilder();
    md = new KPIMTextEdit::MarkupDirector(hb);
    md->processDocument(doc);
    result = hb->getResult();

    regex = QRegularExpression(QStringLiteral("^Some formatted text.\\n$"));
    QVERIFY(regex.match(result).hasMatch());

    // Test Background
    doc->setHtml(QStringLiteral("Some <span style=\"background-color:#ff0000;\">formatted</span> text."));
    delete md;
    delete hb;

    hb = new KPIMTextEdit::PlainTextMarkupBuilder();
    md = new KPIMTextEdit::MarkupDirector(hb);
    md->processDocument(doc);
    result = hb->getResult();

    regex = QRegularExpression(QStringLiteral("^Some formatted text.\\n$"));
    QVERIFY(regex.match(result).hasMatch());

    // Test Font Family
    doc->setHtml(QStringLiteral("Some <span style=\"font-family:courier;\">formatted</span> text."));
    delete md;
    delete hb;

    hb = new KPIMTextEdit::PlainTextMarkupBuilder();
    md = new KPIMTextEdit::MarkupDirector(hb);
    md->processDocument(doc);
    result = hb->getResult();

    regex = QRegularExpression(QStringLiteral("^Some formatted text.\\n$"));
    QVERIFY(regex.match(result).hasMatch());

    // Test Font Size
    doc->setHtml(QStringLiteral("Some <span style=\"font-size:20pt;\">formatted</span> text."));
    delete md;
    delete hb;

    hb = new KPIMTextEdit::PlainTextMarkupBuilder();
    md = new KPIMTextEdit::MarkupDirector(hb);
    md->processDocument(doc);
    result = hb->getResult();

    regex = QRegularExpression(QStringLiteral("^Some formatted text.\\n$"));
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

    auto regex = QRegularExpression(QStringLiteral("^Foo\\n--------------------\\nBar\\n$"));

    QVERIFY(regex.match(result).hasMatch());
    delete md;
    delete hb;
    delete doc;
}

void PlainTextMarkupBuilderTest::testNewlines()
{
    auto doc = new QTextDocument();
    doc->setHtml(QStringLiteral("<p>Foo</p>\n<br /><br />\n<p>Bar</p>"));

    auto hb = new KPIMTextEdit::PlainTextMarkupBuilder();
    auto md = new KPIMTextEdit::MarkupDirector(hb);
    md->processDocument(doc);
    auto result = hb->getResult();

    auto regex = QRegularExpression(QStringLiteral("^Foo\\n\\n\\nBar\\n$"));

    QVERIFY(regex.match(result).hasMatch());
    delete md;
    delete hb;
    delete doc;
}

void PlainTextMarkupBuilderTest::testEmptyParagraphs()
{
    auto doc = new QTextDocument();
    doc->setHtml(QStringLiteral("<p>Foo</p>\n<br /><br />\n<p>Bar</p>"));

    auto hb = new KPIMTextEdit::PlainTextMarkupBuilder();
    auto md = new KPIMTextEdit::MarkupDirector(hb);
    md->processDocument(doc);
    auto result = hb->getResult();

    auto regex = QRegularExpression(QStringLiteral("^Foo\\n\\n\\nBar\\n$"));

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
    cursor.insertText(QStringLiteral("Foo"));
    cursor.insertText(QStringLiteral("\n"));
    cursor.insertText(QStringLiteral("\n"));
    cursor.insertText(QStringLiteral("\n"));
    cursor.insertText(QStringLiteral("Bar"));

    auto hb = new KPIMTextEdit::PlainTextMarkupBuilder();
    auto md = new KPIMTextEdit::MarkupDirector(hb);
    md->processDocument(doc);
    auto result = hb->getResult();

    auto regex = QRegularExpression(QStringLiteral("^Foo\\n\\n\\nBar\\n$"));

    QVERIFY(regex.match(result).hasMatch());
    delete md;
    delete hb;
    delete doc;
}

void PlainTextMarkupBuilderTest::testBrInsideParagraph()
{
    auto doc = new QTextDocument();
    doc->setHtml(QStringLiteral("<p>Foo<br /><br /><br />Bar</p>"));

    auto hb = new KPIMTextEdit::PlainTextMarkupBuilder();
    auto md = new KPIMTextEdit::MarkupDirector(hb);
    md->processDocument(doc);
    auto result = hb->getResult();

    // Two paragraphs separated by two line breaks

    auto regex = QRegularExpression(QStringLiteral("^Foo\\n\\n\\nBar\\n$"));

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
    QVERIFY2(result.startsWith(QLatin1String("Hello,\nThis is some text\nIt shows how "
                                             "grantlee is used from kmail\n")),
             qPrintable(result));
    QVERIFY2(result.endsWith(QLatin1String("This is the end of the signature\n")), qPrintable(result));
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

    auto regex = QRegularExpression(QStringLiteral("^url: foo\\[1\\]\\nline1\\nline2\\n-- \\nbla\\n\n\\n\\n--------\\n\\[1\\] https://www.kde.org\\n$"));
    QVERIFY(regex.match(result).hasMatch());
    delete md;
    delete hb;
    delete doc;
}
