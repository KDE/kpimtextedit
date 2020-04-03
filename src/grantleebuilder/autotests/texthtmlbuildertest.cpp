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

#include "texthtmlbuildertest.h"
#include "grantleebuilder/texthtmlbuilder.h"
#include <QRegularExpression>
#include <QTest>
#include <QTextDocument>
#include <grantlee/markupdirector.h>
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
    auto md = new Grantlee::MarkupDirector(hb);
    md->processDocument(doc);
    auto result = hb->getResult();

    auto regex = QRegularExpression(
        QStringLiteral("^<p>Foo</p>\\n<p>&nbsp;<p>&nbsp;<p>Bar</p>\\n$"));

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
    QTest::newRow("link") <<  QStringLiteral("A <a href=\"http://www.kde.org\">link</a> to KDE.") << QStringLiteral("^<p>A <a href=\"http://www.kde.org\">link</a>&nbsp;to KDE.</p>\\n$") << true;
    QTest::newRow("text with espace") <<  QStringLiteral("         foo") << QStringLiteral("^<p>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; foo</p>\\n$") << false;
    QTest::newRow("text with espace at begin") <<  QStringLiteral(" foo") << QStringLiteral("^<p>&nbsp;foo</p>\\n$") << false;
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
    auto md = new Grantlee::MarkupDirector(hb);
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
