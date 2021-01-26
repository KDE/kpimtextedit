/*
   SPDX-FileCopyrightText: 2020-2021 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
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
    void testBrInsideAnchor();
};

#endif // PLAINTEXTMARKUPBUILDERTEST_H
