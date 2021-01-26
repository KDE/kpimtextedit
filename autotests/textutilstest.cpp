/*
    SPDX-FileCopyrightText: 2011 Torgny Nyblom <nyblom@kde.org>

    SPDX-License-Identifier: LGPL-2.0-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "textutilstest.h"

#include "textutils.h"

#include "qtest.h"

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
