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

    QTest::newRow("bug345360") << QStringLiteral("<html><head><title>Changes to Zoho Privacy Policy and Terms of Service</title></head><body><center><table width=\"600\" style=\"margin-top:20px;\"  cellspacing=\"0\" cellpadding=\"0\" border=\"0\"><tr><td style=\"text-align:left;\"><div id=\"_zc_default\" style=\"font-family: Arial,Helvetica,sans-serif; font-size: 12px;padding-top:0px;padding-bottom:0px\"><span style=\"font-family: verdana , arial , helvetica , sans-serif;font-size: 13.333333px;background-color: rgb(255,255,255);\">    Dear User,       </span> <br style=\"font-family: verdana , arial , helvetica , sans-serif;font-size: 13.333333px;background-color: rgb(255,255,255);\"> <br style=\"font-family: verdana , arial , helvetica , sans-serif;font-size: 13.333333px;background-color: rgb(255,255,255);\"> <span style=\"font-family: verdana , arial , helvetica , sans-serif;font-size: 13.333333px;background-color: rgb(255,255,255);\">    We     have updated our Privacy Policy and Terms of Service. Please take a    few  minutes to read and understand them. To help you understand, we    have  summarized the changes and provided a comparison of the current    and the  previous versions.       </span> <br style=\"font-family: verdana , arial , helvetica , sans-serif;font-size: 13.333333px;background-color: rgb(255,255,255);\"> <br style=\"font-family: verdana , arial , helvetica , sans-serif;font-size: 13.333333px;background-color: rgb(255,255,255);\"> <span style=\"font-family: verdana , arial , helvetica , sans-serif;font-size: 13.333333px;background-color: rgb(255,255,255);\">    These     changes will take effect on April 19, 2015.&nbsp; If you continue to use     Zoho after April 19, 2015, you will be governed by the new Privacy     Policy and Terms of Service.       </span> <br style=\"font-family: verdana , arial , helvetica , sans-serif;font-size: 13.333333px;background-color: rgb(255,255,255);\"> <br style=\"font-family: verdana , arial , helvetica , sans-serif;font-size: 13.333333px;background-color: rgb(255,255,255);\"> <span style=\"font-family: verdana , arial , helvetica , sans-serif;font-size: 13.333333px;background-color: rgb(255,255,255);\">    &nbsp;&nbsp;&nbsp;&nbsp;       </span> <a href=\"http://zc1.maillist-manage.com/click.zc?od=11287eca375dad&repDgs=189bb4a86db70a&linkDgs=189bb4a7ed6b3d\" target=\"_blank\">    Revised Privacy Policy       </a> <br style=\"font-family: verdana , arial , helvetica , sans-serif;font-size: 13.333333px;background-color: rgb(255,255,255);\"> <span style=\"font-family: verdana , arial , helvetica , sans-serif;font-size: 13.333333px;background-color: rgb(255,255,255);\">    &nbsp;&nbsp;&nbsp;&nbsp;       </span> <a href=\"http://zc1.maillist-manage.com/click.zc?od=11287eca375dad&repDgs=189bb4a86db70a&linkDgs=189bb4a7ed6b3f\" target=\"_blank\">    Summary of Changes to Privacy Policy       </a> <br style=\"font-family: verdana , arial , helvetica , sans-serif;font-size: 13.333333px;background-color: rgb(255,255,255);\"> <span style=\"font-family: verdana , arial , helvetica , sans-serif;font-size: 13.333333px;background-color: rgb(255,255,255);\">    &nbsp;&nbsp;&nbsp;&nbsp;       </span> <a href=\"http://zc1.maillist-manage.com/click.zc?od=11287eca375dad&repDgs=189bb4a86db70a&linkDgs=189bb4a7ed6b41\" target=\"_blank\">    Comparison       </a> <br style=\"font-family: verdana , arial , helvetica , sans-serif;font-size: 13.333333px;background-color: rgb(255,255,255);\"> <br style=\"font-family: verdana , arial , helvetica , sans-serif;font-size: 13.333333px;background-color: rgb(255,255,255);\"> <span style=\"font-family: verdana , arial , helvetica , sans-serif;font-size: 13.333333px;background-color: rgb(255,255,255);\">    &nbsp;&nbsp;&nbsp;&nbsp;       </span> <a href=\"http://zc1.maillist-manage.com/click.zc?od=11287eca375dad&repDgs=189bb4a86db70a&linkDgs=189bb4a7ed6b47\" target=\"_blank\">    Revised Terms of Service       </a> <br style=\"font-family: verdana , arial , helvetica , sans-serif;font-size: 13.333333px;background-color: rgb(255,255,255);\"> <span style=\"font-family: verdana , arial , helvetica , sans-serif;font-size: 13.333333px;background-color: rgb(255,255,255);\">    &nbsp;&nbsp;&nbsp;&nbsp;       </span> <a href=\"http://zc1.maillist-manage.com/click.zc?od=11287eca375dad&repDgs=189bb4a86db70a&linkDgs=189bb4a7ed6b45\" target=\"_blank\">    Summary of Changes to Terms of Service       </a> <br style=\"font-family: verdana , arial , helvetica , sans-serif;font-size: 13.333333px;background-color: rgb(255,255,255);\"> <span style=\"font-family: verdana , arial , helvetica , sans-serif;font-size: 13.333333px;background-color: rgb(255,255,255);\">    &nbsp;&nbsp;&nbsp;&nbsp;       </span> <a href=\"http://zc1.maillist-manage.com/click.zc?od=11287eca375dad&repDgs=189bb4a86db70a&linkDgs=189bb4a7ed6b43\" target=\"_blank\">    Comparison       </a> <br> <br> <span style=\"font-family: verdana , arial , helvetica , sans-serif;font-size: 13.333333px;background-color: rgb(255,255,255);\"> <br>Thanks!<br><br></span><div><span style=\"font-family: verdana , arial , helvetica , sans-serif;font-size: 13.333333px;background-color: rgb(255,255,255);\">The Zoho Team&nbsp; </span> <div>    <span style=\"font-family: verdana , arial , helvetica , sans-serif;font-size: 13.333333px;background-color: rgb(255,255,255);\">       Zoho Corporation</span> </div> <div>    <span style=\"font-family: verdana , arial , helvetica , sans-serif;font-size: 13.333333px;background-color: rgb(255,255,255);\">       4141 Hacienda Drive        Pleasanton,&nbsp;    </span> </div> <div>    <span style=\"font-family: verdana , arial , helvetica , sans-serif;font-size: 13.333333px;background-color: rgb(255,255,255);\">       CA 94588,</span><span style=\"font-size: 13.333333px; font-family: verdana, arial, helvetica, sans-serif; background-color: rgb(255, 255, 255);\">USA      </span><span style=\"font-size: 10pt;\">    </span><span data-href=\"http://zc1.maillist-manage.com/ua/optout?od=11287eca375dad&rd=189bb4a86db70a&sd=189bb4a7ed6b37&n=11699e4c206dee1\" data-target data-text-len=\"4\" data-type=\"1\" style=\"font-size: 10pt; color: rgb(255, 255, 255);\">       .</span></div></div></div><img src=\"http://zc1.maillist-manage.com/open/od11287eca375dad_rd189bb4a86db70a.gif\" height=\"1\" width=\"50\"/></td></tr></table></center></body></html>")
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
