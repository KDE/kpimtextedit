/* Copyright 2011 Torgny Nyblom <nyblom@kde.org>

   This library is free software; you can redistribute it and/or modify
   it under the terms of the GNU Library General Public License as published
   by the Free Software Foundation; either version 2 of the License or
   ( at your option ) version 3 or, at the discretion of KDE e.V.
   ( which shall act as a proxy as in section 14 of the GPLv3 ), any later version.

   This library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Library General Public License for more details.

   You should have received a copy of the GNU Library General Public License
   along with this library; see the file COPYING.LIB.  If not, write to
   the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
   Boston, MA 02110-1301, USA.
*/
#include "textutilstest.h"

#include "textutils.h"

#include "qtest.h"

using namespace KPIMTextEdit;

QTEST_MAIN(TextUtilsTest)
#define lineLength 40

void TextUtilsTest::test_flowText()
{
    QFETCH(QString, originalString);
    QFETCH(QString, quotedString);
    QFETCH(QString, indent);

    QCOMPARE(TextUtils::flowText(originalString, indent, lineLength), quotedString);
}

void TextUtilsTest::test_flowText_data()
{
    QTest::addColumn<QString>("originalString");
    QTest::addColumn<QString>("quotedString");
    QTest::addColumn<QString>("indent");

    QTest::newRow("Empty string ") << "" << "" << "";

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
    QTest::newRow("One non-wrapping line")  << "A long line that is right on the border."
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

