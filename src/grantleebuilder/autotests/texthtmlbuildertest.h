/*
   SPDX-FileCopyrightText: 2020-2021 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#ifndef TEXTHTMLBUILDERTEST_H
#define TEXTHTMLBUILDERTEST_H

#include <QObject>

class TextHTMLBuilderTest : public QObject
{
    Q_OBJECT
public:
    explicit TextHTMLBuilderTest(QObject *parent = nullptr);
    ~TextHTMLBuilderTest() = default;
private Q_SLOTS:
    void testHtmlText_data();
    void testHtmlText();
    void testHtmlWithTab();
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
    void testNewlinesThroughQTextCursor();
    void testInsertImage();
    void testInsertImageWithSize();
    void testTitle1();
    void testBug421908();
    void testBug421908_2();
    void testBug421908_full();
    void testBug436880();
};

#endif // TEXTHTMLBUILDERTEST_H
