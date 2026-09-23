/*
  SPDX-FileCopyrightText: 2020-2026 Laurent Montel <montel@kde.org>
  SPDX-FileCopyrightText: 2008, 2010 Stephen Kelly <steveire@gmail.com>

  SPDX-License-Identifier: LGPL-2.0-or-later

*/

#pragma once
#include "kpimtextedit_export.h"

#include <QString>
#include <QTextListFormat>
#include <QTextTableCellFormat>
#include <QTextTableFormat>

class QBrush;

namespace KPIMTextEdit
{
class AbstractMarkupBuilderPrivate;

/// @headerfile abstractmarkupbuilder.h grantlee/abstractmarkupbuilder.h

/*!
  \class KPIMTextEdit::AbstractMarkupBuilder
  \inmodule KPIMTextedit
  \inheaderfile KPIMTextEdit/AbstractMarkupBuilder
  \brief Interface for creating marked-up text output.

  The **%AbstractMarkupBuilder** is used by a MarkupDirector to create marked-up
  output such as html or markdown.

  See PlainTextMarkupBuilder and TextHTMLBuilder for example implementations.

  This interface can be extended to handle custom format types in a
  QTextDocument. \sa custom_qtextobject

  @author Stephen Kelly <steveire@gmail.com>
*/
class KPIMTEXTEDIT_EXPORT AbstractMarkupBuilder
{
public:
    /*! Destructor */
    virtual ~AbstractMarkupBuilder() = default;

    /*! Begin a bold element in the markup */
    virtual void beginStrong() = 0;

    /*! Close the bold element in the markup */
    virtual void endStrong() = 0;

    /*! Begin an emphasised element in the markup */
    virtual void beginEmph() = 0;

    /*! Close the emphasised element in the markup */
    virtual void endEmph() = 0;

    /*! Begin an underlined element in the markup */
    virtual void beginUnderline() = 0;

    /*! Close the underlined element in the markup */
    virtual void endUnderline() = 0;

    /*! Begin a struck out element in the markup */
    virtual void beginStrikeout() = 0;

    /*! Close the struck out element in the markup */
    virtual void endStrikeout() = 0;

    /*!
    Begin a decorarated foreground element in the markup (A text color)
    using \a brush
  */
    virtual void beginForeground(const QBrush &brush) = 0;

    /*! Close the decorarated foreground element in the markup */
    virtual void endForeground() = 0;

    /*!
    Begin a decorarated background element in the markup (A text background
    color) using \a brush
   */
    virtual void beginBackground(const QBrush &brush) = 0;

    /*! Close the decorarated background element in the markup */
    virtual void endBackground() = 0;

    /*!
    Begin a url anchor element in the markup
    \a href The href of the anchor.
    \a name The name of the anchor.
  */
    virtual void beginAnchor(const QString &href = {}, const QString &name = {}) = 0;

    /*! Close the anchor element */
    virtual void endAnchor() = 0;

    /*!
    Begin a new font family element in the markup
    \a family The name of the font family to begin.
  */
    virtual void beginFontFamily(const QString &family) = 0;

    /*! End font family element */
    virtual void endFontFamily() = 0;

    /*!
    Begin a new font point size element in the markup
    \a size The point size to begin.
  */
    virtual void beginFontPointSize(int size) = 0;

    /*! End font point size element */
    virtual void endFontPointSize() = 0;

    /*!
    Begin a new paragraph in the markup
    \a a The alignment of the new paragraph.
    \a top The top margin of the new paragraph.
    \a bottom The bottom margin of the new paragraph.
    \a left The left margin of the new paragraph.
    \a right The right margin of the new paragraph.
  */
    virtual void
    beginParagraph(Qt::Alignment a = Qt::AlignLeft, qreal top = 0.0, qreal bottom = 0.0, qreal left = 0.0, qreal right = 0.0, bool leftToRightText = false) = 0;

    /*! Close the paragraph in the markup. */
    virtual void endParagraph() = 0;
    /*! Add a newline to the markup. */
    virtual void addNewline() = 0;

    /*!
    Insert a horizontal rule into the markup.
    \a width The width of the rule. Default is full width.
  */
    virtual void insertHorizontalRule(int width = -1) = 0;

    /*!
    Insert a new image element into the markup.
    \a url The url of the image
    \a width The width of the image
    \a height The height of the image.
  */
    virtual void insertImage(const QString &url, qreal width, qreal height) = 0;

    /*!
    Begin a new list element in the markup.
    A list element contains list items, and may contain other lists.

    \a style The style of list to create.
    \a start The number of the first item of an ordered list. It is one based, and 1 for
    a list which is not renumbered.
  */
    virtual void beginList(QTextListFormat::Style style, int start) = 0;

    /*!
    Close the list.
  */
    virtual void endList() = 0;

    /*! Begin a new list item in the markup */
    virtual void beginListItem() = 0;

    /*! End the list item */
    virtual void endListItem() = 0;

    /*! Begin a superscript element */
    virtual void beginSuperscript() = 0;

    /*! End superscript element */
    virtual void endSuperscript() = 0;

    /*! Begin a subscript element */
    virtual void beginSubscript() = 0;

    /*! End subscript element */
    virtual void endSubscript() = 0;

    /*!
    Begin a table element.

    \a format The format of the table, holding the cell padding, the cell
    spacing, the width and the border of the table.
  */
    virtual void beginTable(const QTextTableFormat &format) = 0;

    /*!
    Begin a new table row
  */
    virtual void beginTableRow() = 0;

    /*!
    Begin a new table header cell.

    \a format The format of the cell, holding the column and row spans, the
    padding, the borders and the background of the cell.
    \a width The width of the column the cell belongs to. The width is a
    constraint of the table, not of the cell, hence it is passed separately.
  */
    virtual void beginTableHeaderCell(const QTextTableCellFormat &format, const QTextLength &width) = 0;

    /*!
    Begin a new table cell.

    \a format The format of the cell, holding the column and row spans, the
    padding, the borders and the background of the cell.
    \a width The width of the column the cell belongs to. The width is a
    constraint of the table, not of the cell, hence it is passed separately.
  */
    virtual void beginTableCell(const QTextTableCellFormat &format, const QTextLength &width) = 0;

    /*! End a table element */
    virtual void endTable() = 0;

    /*! End a table row */
    virtual void endTableRow() = 0;

    /*! End a table header cell */
    virtual void endTableHeaderCell() = 0;

    /*! End a table cell */
    virtual void endTableCell() = 0;

    /*!
    Begin a level \a level header
    \a level An integer between 1 and 6
  */
    virtual void beginHeader(int level) = 0;

    /*!
    End a level \a level header
    \a level An integer between 1 and 6
  */
    virtual void endHeader(int level) = 0;

    /*!
    Append the plain text \a text to the markup

    \a text The text to append.
  */
    virtual void appendLiteralText(const QString &text) = 0;

    /*!
    Append the raw text \a text to the markup. \a text is added unescaped
  */
    virtual void appendRawText(const QString &text) = 0;

    /*!
    Return the fully marked up result of the building process.

    This may contain metadata etc, such as a head element in html.

    Returns The fully marked up text.
  */
    [[nodiscard]] virtual QString getResult() = 0;

    virtual void addSingleBreakLine() = 0;
};
}
