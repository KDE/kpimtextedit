/*
  SPDX-FileCopyrightText: 2019-2020 Montel Laurent <montel@kde.org>
  based on grantlee PlainTextMarkupBuilder

  SPDX-License-Identifier: LGPL-2.0-or-later

*/

#ifndef PLAINTEXTMARKUPBUILDER_H
#define PLAINTEXTMARKUPBUILDER_H

#include "abstractmarkupbuilder.h"
#include "kpimtextedit_export.h"

#define LETTERSINALPHABET 26
#define DIGITSOFFSET 10

namespace KPIMTextEdit
{
class PlainTextMarkupBuilderPrivate;
class KPIMTEXTEDIT_EXPORT PlainTextMarkupBuilder : virtual public KPIMTextEdit::AbstractMarkupBuilder
{
public:
    /** Construct a new PlainTextHTMLMarkupBuilder. */
    PlainTextMarkupBuilder();

    ~PlainTextMarkupBuilder() override;

    void setQuotePrefix(const QString &prefix);

    void beginStrong() override;
    void endStrong() override;
    void beginEmph() override;
    void endEmph() override;
    void beginUnderline() override;
    void endUnderline() override;
    void beginStrikeout() override;
    void endStrikeout() override;

    void beginAnchor(const QString &href = QString(), const QString &name = QString()) override;

    void endAnchor() override;

    void beginParagraph(Qt::Alignment a = Qt::AlignLeft, qreal top = 0.0, qreal bottom = 0.0, qreal left = 0.0, qreal right = 0.0, bool leftToRightText = false)
        override;

    void endParagraph() override;
    void addNewline() override;

    void insertHorizontalRule(int width = -1) override;

    void insertImage(const QString &src, qreal width, qreal height) override;

    void beginList(QTextListFormat::Style style) override;

    void endList() override;

    void beginListItem() override;

    void endListItem() override;

    void beginSuperscript() override;

    void endSuperscript() override;

    void beginSubscript() override;

    void endSubscript() override;

    void beginTable(qreal cellpadding, qreal cellspacing, const QString &width) override;

    void beginTableRow() override;

    void beginTableHeaderCell(const QString &width, int colSpan, int rowSpan) override;

    void beginTableCell(const QString &width, int colSpan, int rowSpan) override;

    void appendLiteralText(const QString &text) override;

    void appendRawText(const QString &text) override;

    /**
    Adds a reference to @p reference to the internal list of references in the
    document.
  */
    Q_REQUIRED_RESULT int addReference(const QString &reference);

    /**
    Returns the finalised plain text markup, including references at the end.
  */
    Q_REQUIRED_RESULT QString getResult() override;

    Q_REQUIRED_RESULT bool isQuoteBlock(qreal top, qreal bottom, qreal left, qreal right) const;

    void beginForeground(const QBrush &brush) override;
    void endForeground() override;
    void beginBackground(const QBrush &brush) override;
    void endBackground() override;
    void beginFontFamily(const QString &family) override;
    void endFontFamily() override;
    void beginFontPointSize(int size) override;
    void endFontPointSize() override;
    void endTable() override;
    void endTableRow() override;
    void endTableHeaderCell() override;
    void endTableCell() override;
    void beginHeader(int level) override;
    void endHeader(int level) override;
    void addSingleBreakLine() override;

private:
    PlainTextMarkupBuilderPrivate *const d_ptr;
    Q_DECLARE_PRIVATE(PlainTextMarkupBuilder)
};
}

#endif // PLAINTEXTMARKUPBUILDER_H
