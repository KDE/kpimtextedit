/*
   SPDX-FileCopyrightText: 2020-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once
#include "abstractmarkupbuilder.h"
#include "kpimtextedit_export.h"

namespace KPIMTextEdit
{
class TextHTMLBuilderPrivate;

/// @headerfile texthtmlbuilder.h grantlee/texthtmlbuilder.h

/*!
  \brief The TextHTMLBuilder creates a clean html markup output.

  This class creates html output which is as minimal as possible and restricted
  to the rich text features supported in %Qt.
  (https://doc.qt.io/qt-5/richtext-html-subset.html)

  The output contains only the body content, not the head element or other
  metadata.

  eg:

  \code
    <p>
      This is some <strong>formatted content</strong> in a paragraph.
    </p>
  \endcode

  instead of the content produced by %Qt:

  \code
    <!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 4.0//EN"
  "http://www.w3.org/TR/REC-html40/strict.dtd">
    <html><head><meta name="qrichtext" content="1" /><meta
  http-equiv="Content-Type" content="text/html; charset=UTF-8" /><style
  type="text/css">
    p, li { white-space: pre-wrap; }
    </style></head><body style=" font-family:'Sans Serif'; font-size:10pt;
  font-weight:400; font-style:normal;">
    <p style=" margin-top:12px; margin-bottom:12px; margin-left:0px;
  margin-right:0px; -qt-block-indent:0; text-indent:0px;">This is some <span
  style=" font-weight:600;">formatted content</span> in a paragraph.
  </p></body></html>
  \endcode

  Such tags should be created separately. For example:

  \code
    auto b = new TextHTMLBuilder();
    auto md = new MarkupDirector(b);
    md->constructContent();
    QString cleanHtml(
      "<head>\n<title>%1</title>\n</head>\n<body>%2</body>\n</html>")
                      .arg(document.metaInformation(QTextDocument::DocumentTitle))
                      .arg(b->getOutput());
    file.write(cleanHtml);
  \endcode

  Font formatting information on elements is represented by individual span
  elements.

  eg:
  \code
    <span style"color:blue;">
      <span style="background-color:red;">
        Blue text on red background
      </span>
    </span>
  \endcode

  instead of

  \code
    <span style="color:blue;background-color:red;">
      Blue text on red background
    </span>
  \endcode

  It my be possible to change this if necessary.

  \class KPIMTextEdit::TextHTMLBuilder
  \inmodule KPIMTextedit
  \inheaderfile KPIMTextEdit/TextHTMLBuilder
  \author Stephen Kelly <steveire@gmail.com>
*/
class KPIMTEXTEDIT_EXPORT TextHTMLBuilder : virtual public KPIMTextEdit::AbstractMarkupBuilder
{
public:
    /*!
     * Constructs a TextHTMLBuilder.
     */
    TextHTMLBuilder();
    /*!
     * Destroys the TextHTMLBuilder.
     */
    ~TextHTMLBuilder() override;

    /*!
     * Begins a bold (strong) HTML element.
     */
    void beginStrong() override;
    /*!
     * Ends the bold HTML element.
     */
    void endStrong() override;
    /*!
     * Begins an emphasized HTML element.
     */
    void beginEmph() override;
    /*!
     * Ends the emphasized HTML element.
     */
    void endEmph() override;
    /*!
     * Begins an underlined HTML element.
     */
    void beginUnderline() override;
    /*!
     * Ends the underlined HTML element.
     */
    void endUnderline() override;
    /*!
     * Begins a strikeout HTML element.
     */
    void beginStrikeout() override;
    /*!
     * Ends the strikeout HTML element.
     */
    void endStrikeout() override;
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
     * Begins an anchor (link) HTML element with the given \a href and \a name.
     */
    void beginAnchor(const QString &href = {}, const QString &name = {}) override;
    /*!
     * Ends the anchor HTML element.
     */
    void endAnchor() override;

    /*!
     * Begins a font family element with the given \a family.
     */
    void beginFontFamily(const QString &family) override;
    /*!
     * Ends the font family element.
     */
    void endFontFamily() override;

    /*!
     * Begin a new font point size with the given \a size.
     */
    void beginFontPointSize(int size) override;
    /*!
     * Ends the font point size element.
     */
    void endFontPointSize() override;

    /*!
     * Begins a paragraph with the given alignment and margins.
     * \a al The alignment of the paragraph
     * \a topMargin The top margin
     * \a bottomMargin The bottom margin
     * \a leftMargin The left margin
     * \a rightMargin The right margin
     * \a leftToRightText Whether the text direction is left-to-right
     */
    void beginParagraph(Qt::Alignment al = Qt::AlignLeft,
                        qreal topMargin = 0.0,
                        qreal bottomMargin = 0.0,
                        qreal leftMargin = 0.0,
                        qreal rightMargin = 0.0,
                        bool leftToRightText = false) override;

    /*!
     * Begins a header element with the given \a level.
     */
    void beginHeader(int level) override;

    /*!
     * Ends a header element with the given \a level.
     */
    void endHeader(int level) override;

    /*!
     * Ends the paragraph element.
     */
    void endParagraph() override;
    /*!
     * Adds a newline to the HTML output.
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
     * Begins a list with the given \a type style.
     */
    void beginList(QTextListFormat::Style type) override;

    /*!
     * Ends the list element.
     */
    void endList() override;

    /*!
     * Begins a list item element.
     */
    void beginListItem() override;
    /*!
     * Ends the list item element.
     */
    void endListItem() override;

    /*!
     * Begins a superscript element.
     */
    void beginSuperscript() override;

    /*!
     * Ends the superscript element.
     */
    void endSuperscript() override;

    /*!
     * Begins a subscript element.
     */
    void beginSubscript() override;

    /*!
     * Ends the subscript element.
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
     * Begins a table header cell with the given \a width, \a colspan, and \a rowspan.
     */
    void beginTableHeaderCell(const QString &width, int colspan, int rowspan) override;

    /*!
     * Begins a table cell with the given \a width, \a colspan, and \a rowspan.
     */
    void beginTableCell(const QString &width, int colspan, int rowspan) override;

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
    Reimplemented from AbstractMarkupBuilder.

    This implementation escapes the text before appending so that

    \badcode
      A sample <b>bold</b> word.
    \endcode

    becomes

    \badcode
      A sample &lt;b&gt;bold&lt;/b&gt; word.
    \endcode
  */
    void appendLiteralText(const QString &text) override;

    /*!
    Append \a text without escaping.

    This is useful if extending MarkupDirector
  */
    void appendRawText(const QString &text) override;

    [[nodiscard]] QString getResult() override;

    void addSingleBreakLine() override;

private:
    TextHTMLBuilderPrivate *d_ptr;
    Q_DECLARE_PRIVATE(TextHTMLBuilder)
};

}
