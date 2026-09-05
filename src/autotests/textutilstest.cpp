/*
    SPDX-FileCopyrightText: 2011 Torgny Nyblom <nyblom@kde.org>

    SPDX-License-Identifier: LGPL-2.0-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "textutilstest.h"

#include "textutils.h"

#include "qtest.h"
#include <QTextDocument>

using namespace Qt::Literals::StringLiterals;
using namespace KPIMTextEdit;

QTEST_GUILESS_MAIN(TextUtilsTest)
#define lineLength 40

void TextUtilsTest::test_flowText()
{
    QFETCH(QString, originalString);
    QFETCH(QString, quotedString);
    QFETCH(QString, indent);

    QCOMPARE(TextUtils::flowText(originalString, indent, lineLength), quotedString);
}

TextUtilsTest::TextUtilsTest(QObject *parent)
    : QObject(parent)
{
}

void TextUtilsTest::test_flowText_data()
{
    QTest::addColumn<QString>("originalString");
    QTest::addColumn<QString>("quotedString");
    QTest::addColumn<QString>("indent");

    QTest::newRow("Empty string ") << ""
                                   << ""
                                   << "";

    QTest::newRow("Indent == Maxlenght ") << "A line"
                                          << ">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>A\n"
                                             ">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\n"
                                             ">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>l\n"
                                             ">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>i\n"
                                             ">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>n\n"
                                             ">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>e"
                                          << ">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>";
    //                                                                                   40
    //                                                                                    ↓
    QTest::newRow("One non-wrapping line") << "A long line that is right on the border."
                                           << "A long line that is right on the border."
                                           << "";
    QTest::newRow("Two non-wrapping lines") << "A long line that is right on the border.\n"
                                               "A long line that is right on the border."
                                            << "A long line that is right on the border.\n"
                                               "A long line that is right on the border."
                                            << "";
    //                                                                               40
    //                                                                                ↓
    QTest::newRow("Two wrapping lines") << "A long line that is right over the border"
                                        << "A long line that is right over the\n"
                                           "border"
                                        << "";
}

void TextUtilsTest::test_containsFormatting_data()
{
    QTest::addColumn<QString>("text");
    QTest::addColumn<bool>("html");
    QTest::addColumn<bool>("formatted");

    // Nothing a plain text mail would lose.
    QTest::newRow("empty document") << QString() << false << false;
    QTest::newRow("plain text") << u"Hello world"_s << false << false;
    QTest::newRow("plain text with newline") << u"Hello\nworld"_s << false << false;
    QTest::newRow("html without markup") << u"Hello world"_s << true << false;
    QTest::newRow("html paragraph without markup") << u"<p>Hello world</p>"_s << true << false;

    // Character formatting.
    QTest::newRow("bold") << u"<p>Hello <b>world</b></p>"_s << true << true;
    QTest::newRow("italic") << u"<p>Hello <i>world</i></p>"_s << true << true;
    QTest::newRow("underline") << u"<p>Hello <u>world</u></p>"_s << true << true;
    QTest::newRow("foreground color") << u"<p><span style=\"color:#ff0000;\">Hello</span></p>"_s << true << true;
    QTest::newRow("background color") << u"<p><span style=\"background-color:#ff0000;\">Hello</span></p>"_s << true << true;
    QTest::newRow("font size") << u"<p><span style=\"font-size:24pt;\">Hello</span></p>"_s << true << true;
    QTest::newRow("font family") << u"<p><span style=\"font-family:Courier;\">Hello</span></p>"_s << true << true;
    QTest::newRow("anchor") << u"<p><a href=\"https://kde.org\">KDE</a></p>"_s << true << true;

    // Block formatting.
    QTest::newRow("alignment") << u"<p align=\"center\">Hello</p>"_s << true << true;
    QTest::newRow("unordered list") << u"<ul><li>Hello</li><li>world</li></ul>"_s << true << true;
    QTest::newRow("ordered list") << u"<ol><li>Hello</li></ol>"_s << true << true;
    // A table carries no table format on the blocks it contains, it is the *char* format of a
    // block inside a cell that is a table cell format.
    QTest::newRow("table") << u"<table><tr><td>Hello</td></tr></table>"_s << true << true;

    // Horizontal rules. Qt only serializes "<hr />" for a rule that has neither an explicit
    // width nor a background, so looking for that substring in toHtml() used to miss the
    // other ones.
    QTest::newRow("horizontal rule") << u"<p>Hello</p><hr><p>world</p>"_s << true << true;
    QTest::newRow("horizontal rule with relative width") << u"<p>Hello</p><hr width=\"50%\"><p>world</p>"_s << true << true;
    QTest::newRow("horizontal rule with fixed width") << u"<p>Hello</p><hr width=\"100\"><p>world</p>"_s << true << true;
    QTest::newRow("horizontal rule with background") << u"<p>Hello</p><hr style=\"background-color:#ff0000;\"><p>world</p>"_s << true << true;
}

void TextUtilsTest::test_containsFormatting()
{
    QFETCH(QString, text);
    QFETCH(bool, html);
    QFETCH(bool, formatted);

    QTextDocument document;
    if (html) {
        document.setHtml(text);
    } else {
        document.setPlainText(text);
    }

    QCOMPARE(TextUtils::containsFormatting(&document), formatted);
}

void TextUtilsTest::test_containsFormattingWithNullDocument()
{
    QVERIFY(!TextUtils::containsFormatting(nullptr));
}

#include "moc_textutilstest.cpp"
