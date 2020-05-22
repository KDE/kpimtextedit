/*
   Copyright (C) 2020 Laurent Montel <montel@kde.org>

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


#include "plaintextmarkupbuildertest.h"
#include <KPIMTextEdit/PlainTextMarkupBuilder>
#include <KPIMTextEdit/MarkupDirector>
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
    QTest::newRow("text with espace at begin") <<  QString() << QString();
    QTest::newRow("bold") << QStringLiteral("This <b>text</b> is bold.") << QStringLiteral("^This \\*text\\* is bold.\\n$");
    QTest::newRow("double format") << QStringLiteral("Some <b><i>formatted</i></b> text.") << QStringLiteral("^Some (\\*/|/\\*)formatted(\\*/|/\\*) text.\\n$");
    QTest::newRow("italic") << QStringLiteral("This <b>text is <i>italic</i> and</b> bold.") << QStringLiteral("^This \\*text is /italic/ and\\* bold.\\n$");
    QTest::newRow("double span") << QStringLiteral("Some <span "
                                                   "style=\"color:#ff0000;background-color:#00ff00;"
                                                   "\">formatted</span> text.") << QStringLiteral("^Some formatted text.\\n$");

    //FIXME bug is about id="..."
    QTest::newRow("bug345360") << QStringLiteral("<html><body><center><table><tr><td><div id=\"foo\"><span>    Dear User,       </span> <br> <br> <span>    We     have updated our Privacy Policy and Terms of Service. Please take a    few  minutes to read and understand them. To help you understand, we    have  summarized the changes and provided a comparison of the current    and the  previous versions.       </span> <br> <br> <span>    These     changes will take effect on April 19, 2015.&nbsp; If you continue to use     Zoho after April 19, 2015, you will be governed by the new Privacy     Policy and Terms of Service.       </span> <br> <br> <span>  ddd &nbsp;&nbsp;&nbsp;&nbsp;       </span></td></tr></table></center></body></html>")
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
