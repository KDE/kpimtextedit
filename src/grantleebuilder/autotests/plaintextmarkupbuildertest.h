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


#ifndef PLAINTEXTMARKUPBUILDERTEST_H
#define PLAINTEXTMARKUPBUILDERTEST_H

#include <QObject>

class PlainTextMarkupBuilderTest : public QObject
{
    Q_OBJECT
public:
    explicit PlainTextMarkupBuilderTest(QObject *parent = nullptr);
    ~PlainTextMarkupBuilderTest() = default;

private Q_SLOTS:
    void testPlainText();
    void testPlainText_data();
    // Test paragraph contents:
    void testSingleFormat();
    void testDoubleFormat();
    void testDoubleStartDifferentFinish();
    void testDoubleStartDifferentFinishReverseOrder();
    void testDifferentStartDoubleFinish();
    void testDifferentStartDoubleFinishReverseOrder();
    void testOverlap();
    void testAnchor();
    void testAnchorWithFormattedContent();
    void testAdjacentAnchors();
    void testNestedFormatting();
    void testSpan();
    void testDoubleSpan();
    void testSpanNesting();
    void testEdgeCaseLeft();
    void testEdgeCaseRight();
    void testImage();
    void testImageResized();
    void testEachFormatTagSingly();
    void testHorizontalRule();
    void testNewlines();
    void testEmptyParagraphs();
    void testNewlinesThroughQTextCursor();
    void testBrInsideParagraph();
    void testLongDocument();
};

#endif // PLAINTEXTMARKUPBUILDERTEST_H
