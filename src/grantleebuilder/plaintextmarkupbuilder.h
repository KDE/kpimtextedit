/*
  SPDX-FileCopyrightText: 2019-2026 Laurent Montel <montel@kde.org>
  based on grantlee PlainTextMarkupBuilder

  SPDX-License-Identifier: LGPL-2.0-or-later

*/

#pragma once
#include "abstractmarkupbuilder.h"
#include "kpimtextedit_export.h"

#define LETTERSINALPHABET 26
#define DIGITSOFFSET 10

namespace KPIMTextEdit
{
class PlainTextMarkupBuilderPrivate;
/*!
 * \class KPIMTextEdit::PlainTextMarkupBuilder
 * \inmodule KPIMTextedit
 * \inheaderfile KPIMTextEdit/PlainTextMarkupBuilder
 *
 * \brief The PlainTextMarkupBuilder class provides a plain text markup builder for Grantlee templates.
 */
class KPIMTEXTEDIT_EXPORT PlainTextMarkupBuilder : virtual public KPIMTextEdit::AbstractMarkupBuilder
{
public:
    /*!
     * Constructs a new PlainTextMarkupBuilder.
     */
    PlainTextMarkupBuilder();

    /*!
     * Destroys the PlainTextMarkupBuilder.
     */
    ~PlainTextMarkupBuilder() override;

    /*!
     * Sets the prefix to use for quoted text lines.
     */
    void setQuotePrefix(const QString &prefix);

    /*!
     * Begins a bold (strong) text element.
     */
    void beginStrong() override;
    /*!
     * Ends the bold text element.
     */
    void endStrong() override;
    /*!
     * Begins an emphasized text element.
     */
    void beginEmph() override;
    /*!
     * Ends the emphasized text element.
     */
    void endEmph() override;
    /*!
     * Begins an underlined text element.
     */
    void beginUnderline() override;
    /*!
     * Ends the underlined text element.
     */
    void endUnderline() override;
    /*!
     * Begins a strikeout text element.
     */
    void beginStrikeout() override;
    /*!
     * Ends the strikeout text element.
     */
    void endStrikeout() override;

    /*!
     * Begins an anchor (link) element with the given \a href and \a name.
     */
    void beginAnchor(const QString &href = QString(), const QString &name = QString()) override;

    /*!
     * Ends the anchor element.
     */
    void endAnchor() override;

    /*!
     * Begins a paragraph with the given alignment and margins.
     */
    void beginParagraph(Qt::Alignment a = Qt::AlignLeft, qreal top = 0.0, qreal bottom = 0.0, qreal left = 0.0, qreal right = 0.0, bool leftToRightText = false)
        override;

    /*!
     * Ends the paragraph element.
     */
    void endParagraph() override;
    /*!
     * Adds a newline to the text output.
     */
    void addNewline() override;

    /*!
     * Inserts a horizontal rule with the given \a width (-1 for full width).
     */
    void insertHorizontalRule(int width = -1) override;

    /*!
     * Inserts an image with the given \a src, \a width, and \a height.
     */
    void insertImage(const QString &src, qreal width, qreal height) override;

    /*!
     * Begins a list with the given \a style.
     */
    void beginList(QTextListFormat::Style style) override;

    /*!
     * Ends the list element.
     */
    void endList() override;

    /*!
     * Begins a list item.
     */
    void beginListItem() override;

    /*!
     * Ends the list item.
     */
    void endListItem() override;

    /*!
     * Begins a superscript text element.
     */
    void beginSuperscript() override;

    /*!
     * Ends the superscript text element.
     */
    void endSuperscript() override;

    /*!
     * Begins a subscript text element.
     */
    void beginSubscript() override;

    /*!
     * Ends the subscript text element.
     */
    void endSubscript() override;

    /*!
     * Begins a table with the given \a cellpadding, \a cellspacing, and \a width.
     */
    void beginTable(qreal cellpadding, qreal cellspacing, const QString &width) override;

    /*!
     * Begins a table row.
     */
    void beginTableRow() override;

    /*!
     * Begins a table header cell with the given \a width, \a colSpan, and \a rowSpan.
     */
    void beginTableHeaderCell(const QString &width, int colSpan, int rowSpan) override;

    /*!
     * Begins a table cell with the given \a width, \a colSpan, and \a rowSpan.
     */
    void beginTableCell(const QString &width, int colSpan, int rowSpan) override;

    /*!
     * Appends literal text to the output.
     */
    void appendLiteralText(const QString &text) override;

    /*!
     * Appends raw text to the output without escaping.
     */
    void appendRawText(const QString &text) override;

    /*!
     * Adds a reference to the internal list of references and returns its index.
     */
    [[nodiscard]] int addReference(const QString &reference);

    /*!
     * Returns the finalized plain text markup, including references at the end.
     */
    [[nodiscard]] QString getResult() override;

    /*!
     * Returns true if the given dimensions indicate a quoted block.
     */
    [[nodiscard]] bool isQuoteBlock(qreal top, qreal bottom, qreal left, qreal right) const;

    /*!
     * Begins a foreground color element with the given \a brush.
     */
    void beginForeground(const QBrush &brush) override;
    /*!
     * Ends the foreground color element.
     */
    void endForeground() override;
    /*!
     * Begins a background color element with the given \a brush.
     */
    void beginBackground(const QBrush &brush) override;
    /*!
     * Ends the background color element.
     */
    void endBackground() override;
    /*!
     * Begins a font family element with the given \a family.
     */
    void beginFontFamily(const QString &family) override;
    /*!
     * Ends the font family element.
     */
    void endFontFamily() override;
    /*!
     * Begins a font point size element with the given \a size.
     */
    void beginFontPointSize(int size) override;
    /*!
     * Ends the font point size element.
     */
    void endFontPointSize() override;
    /*!
     * Ends the table element.
     */
    void endTable() override;
    /*!
     * Ends the table row element.
     */
    void endTableRow() override;
    /*!
     * Ends the table header cell element.
     */
    void endTableHeaderCell() override;
    /*!
     * Ends the table cell element.
     */
    void endTableCell() override;
    /*!
     * Begins a header element with the given \a level.
     */
    void beginHeader(int level) override;
    /*!
     * Ends the header element with the given \a level.
     */
    void endHeader(int level) override;
    /*!
     * Adds a single break line to the output.
     */
    void addSingleBreakLine() override;

private:
    PlainTextMarkupBuilderPrivate *const d_ptr;
    Q_DECLARE_PRIVATE(PlainTextMarkupBuilder)
};
}
