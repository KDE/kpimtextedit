#include <QtTest>
#include <QtCore>
#include <qtest_kde.h>
#include <qtestevent.h>
#include <QRegExp>
#include <QTextDocument>

#include <kdebug.h>
#include "../markupdirector.h"
#include "../htmlbuilder.h"

class TestHtmlOutput: public QObject
{
    Q_OBJECT
private slots:

// Test paragraph contents:
    void testSingleFormat();
    void testDoubleFormat();
    void testAnchor();
    void testAdjacentAnchors();
    void testNestedFormatting();
    void testSpan();
    void testDoubleSpan();
    void testSpanNesting();
//     void testEdgeCaseLeft();
//     void testEdgeCaseRight();
    void testEachFormatTagSingly();
    void testEachSpanTagSingly();

};

void TestHtmlOutput::testSingleFormat()
{
    QTextDocument *doc = new QTextDocument();

    // One format
    doc->setHtml ( "This <b>text</b> is bold." );

    HTMLBuilder *hb = new HTMLBuilder();
    MarkupDirector *md = new MarkupDirector ( hb );
    md->constructContent ( doc );
    QString result = hb->getResult();
    QRegExp regex ( QString ( "^<p\\s*>\\s*This <strong>text</strong> is bold.\\s*</p>\\s*$" ) );

    QVERIFY ( regex.exactMatch ( result ) );

}

void TestHtmlOutput::testDoubleFormat()
{
    QTextDocument *doc = new QTextDocument();

    // One format
    doc->setHtml ( "Some <b><i>formatted<i></b> text." );

    HTMLBuilder *hb = new HTMLBuilder();
    MarkupDirector *md = new MarkupDirector ( hb );
    md->constructContent ( doc );
    QString result = hb->getResult();
    QRegExp regex ( QString ( "^<p\\s*>\\s*Some <strong><em>formatted</em></strong> text.\\s*</p>\\s*$" ) );

    QVERIFY ( regex.exactMatch ( result ) );

}

void TestHtmlOutput::testAnchor()
{
    QTextDocument *doc = new QTextDocument();
    doc->setHtml ( "A <a href=\"http://www.kde.org\">link</a> to KDE." );

    HTMLBuilder *hb = new HTMLBuilder();
    MarkupDirector *md = new MarkupDirector ( hb );
    md->constructContent ( doc );
    QString result = hb->getResult();

    QRegExp regex ( QString ( "^<p\\s*>\\s*A <a href=\"http://www.kde.org\">link</a> to KDE.\\s*</p>\\s*$" ) );

    QVERIFY ( regex.exactMatch ( result ) );

}

void TestHtmlOutput::testAdjacentAnchors()
{
    QTextDocument *doc = new QTextDocument();
    doc->setHtml ( "Two <a href=\"http://www.kde.org\">links</a><a href=\"http://www.google.com\">next</a> to eachother." );

    HTMLBuilder *hb = new HTMLBuilder();
    MarkupDirector *md = new MarkupDirector ( hb );
    md->constructContent ( doc );
    QString result = hb->getResult();

    QRegExp regex ( QString ( "^<p\\s*>\\s*Two <a href=\"http://www.kde.org\">links</a><a href=\"http://www.google.com\">next</a> to eachother.\\s*</p>\\s*$" ) );

    QVERIFY ( regex.exactMatch ( result ) );
}

void TestHtmlOutput::testNestedFormatting()
{
    QTextDocument *doc = new QTextDocument();
    doc->setHtml ( "This <b>text is <i>italic</i> and</b> bold." );

    HTMLBuilder *hb = new HTMLBuilder();
    MarkupDirector *md = new MarkupDirector ( hb );
    md->constructContent ( doc );
    QString result = hb->getResult();

    QRegExp regex ( QString ( "^<p\\s*>\\s*This <strong>text is <em>italic</em> and</strong> bold.\\s*</p>\\s*$" ) );

    QVERIFY ( regex.exactMatch ( result ) );
}

void TestHtmlOutput::testSpan()
{
    QTextDocument *doc = new QTextDocument();
    doc->setHtml ( "Some <span style=\"color:#ff0000;\">formatted</span> text." );

    HTMLBuilder *hb = new HTMLBuilder();
    MarkupDirector *md = new MarkupDirector ( hb );
    md->constructContent ( doc );
    QString result = hb->getResult();

    QRegExp regex = QRegExp( "^<p\\s*>\\s*Some <span style=\"color:#ff0000;\">formatted</span> text.\\s*</p>\\s*$" );

    QVERIFY ( regex.exactMatch ( result ) );
}

void TestHtmlOutput::testDoubleSpan()
{
    QTextDocument *doc = new QTextDocument();
    doc->setHtml ( "Some <span style=\"color:#ff0000;background-color:#00ff00;\">formatted</span> text." );

    HTMLBuilder *hb = new HTMLBuilder();
    MarkupDirector *md = new MarkupDirector ( hb );
    md->constructContent ( doc );
    QString result = hb->getResult();

    QRegExp regex = QRegExp( "^<p\\s*>\\s*Some <span style=\"color:#ff0000;\"><span style=\"background-color:#00ff00;\">formatted</span></span> text.\\s*</p>\\s*$" );

    QVERIFY ( regex.exactMatch ( result ) );

}

void TestHtmlOutput::testSpanNesting()
{
    QTextDocument *doc = new QTextDocument();
    doc->setHtml ( "Paragraph <span style=\"background-color:#00ff00;\">with some <span style=\"color:#ff0000;\">formatted</span> nested</span> text." );

    HTMLBuilder *hb = new HTMLBuilder();
    MarkupDirector *md = new MarkupDirector ( hb );
    md->constructContent ( doc );
    QString result = hb->getResult();

    QRegExp regex = QRegExp( "^<p\\s*>\\s*Paragraph <span style=\"background-color:#00ff00;\">with some <span style=\"color:#ff0000;\">formatted</span> nested</span> text.\\s*</p>\\s*$" );

    QVERIFY ( regex.exactMatch ( result ) );
}

void TestHtmlOutput::testEachFormatTagSingly()
{
    QString result;
    QRegExp regex;
    HTMLBuilder *hb;
    MarkupDirector *md;

    QTextDocument *doc = new QTextDocument();


    // Test bold
    doc->setHtml ( "Some <b>formatted</b> text." );
    hb = new HTMLBuilder();
    md = new MarkupDirector ( hb );
    md->constructContent ( doc );
    result = hb->getResult();

    regex = QRegExp( "^<p\\s*>\\s*Some <strong>formatted</strong> text.\\s*</p>\\s*$" );
    QVERIFY ( regex.exactMatch ( result ) );


    // Test Italic
    doc->setHtml ( "Some <i>formatted</i> text." );
    hb = new HTMLBuilder();
    md = new MarkupDirector ( hb );
    md->constructContent ( doc );
    result = hb->getResult();

    regex = QRegExp( "^<p\\s*>\\s*Some <em>formatted</em> text.\\s*</p>\\s*$" );
    QVERIFY ( regex.exactMatch ( result ) );

    // Test Underline
    doc->setHtml ( "Some <u>formatted</u> text." );
    hb = new HTMLBuilder();
    md = new MarkupDirector ( hb );
    md->constructContent ( doc );
    result = hb->getResult();

    regex = QRegExp( "^<p\\s*>\\s*Some <u>formatted</u> text.\\s*</p>\\s*$" );
    QVERIFY ( regex.exactMatch ( result ) );

    // Test Strikeout
    doc->setHtml ( "Some <s>formatted</s> text." );
    hb = new HTMLBuilder();
    md = new MarkupDirector ( hb );
    md->constructContent ( doc );
    result = hb->getResult();

    regex = QRegExp( "^<p\\s*>\\s*Some <s>formatted</s> text.\\s*</p>\\s*$" );
    QVERIFY ( regex.exactMatch ( result ) );
}

void TestHtmlOutput::testEachSpanTagSingly()
{
    QString result;
    QRegExp regex;
    HTMLBuilder *hb;
    MarkupDirector *md;

    QTextDocument *doc = new QTextDocument();

    // Test Foreground
    doc->setHtml ( "Some <span style=\"color:#ff0000;\">formatted</span> text." );
    hb = new HTMLBuilder();
    md = new MarkupDirector ( hb );
    md->constructContent ( doc );
    result = hb->getResult();

    regex = QRegExp( "^<p\\s*>\\s*Some <span style=\"color:#ff0000;\">formatted</span> text.\\s*</p>\\s*$" );
    QVERIFY ( regex.exactMatch ( result ) );


    // Test Background
    doc->setHtml ( "Some <span style=\"background-color:#ff0000;\">formatted</span> text." );
    hb = new HTMLBuilder();
    md = new MarkupDirector ( hb );
    md->constructContent ( doc );
    result = hb->getResult();

    regex = QRegExp( "^<p\\s*>\\s*Some <span style=\"background-color:#ff0000;\">formatted</span> text.\\s*</p>\\s*$" );
    QVERIFY ( regex.exactMatch ( result ) );

    // Test Font Family
    doc->setHtml ( "Some <span style=\"font-family:courier;\">formatted</span> text." );
    hb = new HTMLBuilder();
    md = new MarkupDirector ( hb );
    md->constructContent ( doc );
    result = hb->getResult();

    regex = QRegExp( "^<p\\s*>\\s*Some <span style=\"font-family:courier;\">formatted</span> text.\\s*</p>\\s*$" );
    QVERIFY ( regex.exactMatch ( result ) );

    // Test Font Size
    doc->setHtml ( "Some <span style=\"font-size:20pt;\">formatted</span> text." );
    hb = new HTMLBuilder();
    md = new MarkupDirector ( hb );
    md->constructContent ( doc );
    result = hb->getResult();

    regex = QRegExp( "^<p\\s*>\\s*Some <span style=\"font-size:20pt;\">formatted</span> text.\\s*</p>\\s*$" );
    QVERIFY ( regex.exactMatch ( result ) );


}

QTEST_KDEMAIN ( TestHtmlOutput, GUI )
#include "htmlbuildertest.moc"
