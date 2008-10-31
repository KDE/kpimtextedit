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
    void testSingleFormat();
};

void TestHtmlOutput::testSingleFormat()
{
    QTextDocument *doc = new QTextDocument();
    doc->setHtml ( "This <b>text</b> is bold." );

    HTMLBuilder *hb = new HTMLBuilder();
    MarkupDirector *md = new MarkupDirector ( hb );
    md->constructContent ( doc );
    QString result = hb->getResult();

    QRegExp regex ( QString ( "^<p\\s*>\\s*This <strong>text</strong> is bold.\\s*</p>\\s*$" ) );

    QVERIFY ( regex.exactMatch ( result ) );

}

QTEST_KDEMAIN ( TestHtmlOutput, GUI )
#include "htmlbuildertest.moc"
