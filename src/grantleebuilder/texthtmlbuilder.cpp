/*
   SPDX-FileCopyrightText: 2020-2026 Laurent Montel <montel@kde.org>
   based on code from Stephen Kelly <steveire@gmail.com>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "texthtmlbuilder.h"

#include <QBrush>
#include <QColor>
#include <QDebug>
#include <QList>
#include <QTextDocument>
#include <QtMath>

using namespace Qt::Literals::StringLiterals;
namespace KPIMTextEdit
{
class TextHTMLBuilderPrivate
{
public:
    explicit TextHTMLBuilderPrivate(TextHTMLBuilder *b)
        : q_ptr(b)
    {
    }

    QList<QTextListFormat::Style> currentListItemStyles;
    QString mText;

    TextHTMLBuilder *const q_ptr;

    Q_DECLARE_PUBLIC(TextHTMLBuilder)
};
}

using namespace KPIMTextEdit;

namespace
{
// Value of an html width or height attribute: a percentage or a bare number of pixels.
// A variable length has no html equivalent, it maps to an empty attribute.
QString htmlLength(const QTextLength &length)
{
    switch (length.type()) {
    case QTextLength::PercentageLength:
        return u"%1%"_s.arg(length.rawValue());
    case QTextLength::FixedLength:
        return u"%1"_s.arg(length.rawValue());
    case QTextLength::VariableLength:
        break;
    }
    return {};
}

QString htmlBackground(const QTextFormat &format)
{
    // A default-constructed QBrush has a valid (black) color, so the brush style is what tells
    // whether the format really has a background to serialize.
    const QBrush background = format.background();
    if (background.style() == Qt::NoBrush || !background.color().isValid()) {
        return {};
    }
    return u" bgcolor=\"%1\""_s.arg(background.color().name());
}

// A css length other than zero is only honoured when it carries a unit, so "margin-top:12"
// is dropped by the renderer while "margin-top:12px" is not. Qt stores those margins in pixels.
QString cssPixels(qreal value)
{
    if (qFuzzyIsNull(value)) {
        return u"0"_s;
    }
    return u"%1px"_s.arg(value);
}

// Css has no notation for the #aarrggbb colors Qt stores, so a translucent color is written as
// an rgba() function and an opaque one keeps the shorter hexadecimal form.
QString cssColor(const QColor &color)
{
    if (color.alpha() == 255) {
        return color.name();
    }
    return u"rgba(%1, %2, %3, %4)"_s.arg(color.red()).arg(color.green()).arg(color.blue()).arg(color.alphaF());
}

// A font family is a css string, so it has to be quoted: a name can hold spaces, a comma, or
// start with a digit, none of which is a valid unquoted css identifier. The style attribute is
// delimited by double quotes, so the css string is delimited by single quotes, and a backslash
// or a single quote inside the name is escaped for css before the declaration is escaped for html.
QString cssFontFamily(const QString &family)
{
    QString escaped = family;
    escaped.replace(u'\\', u"\\\\"_s);
    escaped.replace(u'\'', u"\\'"_s);
    return u"'%1'"_s.arg(escaped);
}

QString vAlignment(const QTextTableCellFormat &format)
{
    switch (format.verticalAlignment()) {
    case QTextCharFormat::VerticalAlignment::AlignMiddle:
        return u" valign=\"middle\""_s;
    case QTextCharFormat::VerticalAlignment::AlignTop:
        return u" valign=\"top\""_s;
    case QTextCharFormat::VerticalAlignment::AlignBottom:
        return u" valign=\"bottom\""_s;
    default:
        break;
    }
    return {};
}

QStringList cellPadding(const QTextTableCellFormat &format)
{
    QStringList paddingStyle;
    if (format.hasProperty(QTextFormat::TableCellBottomPadding)) {
        paddingStyle.append(u"padding-bottom: %1"_s.arg(cssPixels(format.bottomPadding())));
    }
    if (format.hasProperty(QTextFormat::TableCellTopPadding)) {
        paddingStyle.append(u"padding-top: %1"_s.arg(cssPixels(format.topPadding())));
    }
    if (format.hasProperty(QTextFormat::TableCellLeftPadding)) {
        paddingStyle.append(u"padding-left: %1"_s.arg(cssPixels(format.leftPadding())));
    }
    if (format.hasProperty(QTextFormat::TableCellRightPadding)) {
        paddingStyle.append(u"padding-right: %1"_s.arg(cssPixels(format.rightPadding())));
    }
    return paddingStyle;
}

// Qt has two dash patterns css does not. They degrade to a plain dashed line, which keeps the
// side visible, rather than to none, which would make it disappear.
QLatin1StringView cssBorderStyle(QTextFrameFormat::BorderStyle style)
{
    switch (style) {
    case QTextFrameFormat::BorderStyle_None:
        return "none"_L1;
    case QTextFrameFormat::BorderStyle_Dotted:
        return "dotted"_L1;
    case QTextFrameFormat::BorderStyle_Dashed:
    case QTextFrameFormat::BorderStyle_DotDash:
    case QTextFrameFormat::BorderStyle_DotDotDash:
        return "dashed"_L1;
    case QTextFrameFormat::BorderStyle_Solid:
        return "solid"_L1;
    case QTextFrameFormat::BorderStyle_Double:
        return "double"_L1;
    case QTextFrameFormat::BorderStyle_Groove:
        return "groove"_L1;
    case QTextFrameFormat::BorderStyle_Ridge:
        return "ridge"_L1;
    case QTextFrameFormat::BorderStyle_Inset:
        return "inset"_L1;
    case QTextFrameFormat::BorderStyle_Outset:
        return "outset"_L1;
    }
    return "none"_L1;
}

// Each side of a cell border is three independent properties: a width, a style and a brush.
// A side is serialized as soon as the document set any of them, so that a cell which asked
// for nothing keeps inheriting the border attribute of the table.
QStringList cellBorder(const QTextTableCellFormat &format)
{
    QStringList borderStyle;
    const auto appendSide = [&](QLatin1StringView side,
                                QTextFormat::Property widthProperty,
                                QTextFormat::Property styleProperty,
                                qreal width,
                                QTextFrameFormat::BorderStyle style,
                                const QBrush &brush) {
        if (!format.hasProperty(widthProperty) && !format.hasProperty(styleProperty)) {
            return;
        }
        // The width is always written, even when it is zero. Css falls back to a medium width
        // when only a style is given, which would paint a border the document does not have.
        QString declaration = u"border-%1: %2 %3"_s.arg(side, cssPixels(width), cssBorderStyle(style));
        if (brush.style() != Qt::NoBrush && brush.color().isValid()) {
            declaration += u' ' + cssColor(brush.color());
        }
        borderStyle.append(declaration);
    };

    appendSide("bottom"_L1,
               QTextFormat::TableCellBottomBorder,
               QTextFormat::TableCellBottomBorderStyle,
               format.bottomBorder(),
               format.bottomBorderStyle(),
               format.bottomBorderBrush());
    appendSide("top"_L1,
               QTextFormat::TableCellTopBorder,
               QTextFormat::TableCellTopBorderStyle,
               format.topBorder(),
               format.topBorderStyle(),
               format.topBorderBrush());
    appendSide("left"_L1,
               QTextFormat::TableCellLeftBorder,
               QTextFormat::TableCellLeftBorderStyle,
               format.leftBorder(),
               format.leftBorderStyle(),
               format.leftBorderBrush());
    appendSide("right"_L1,
               QTextFormat::TableCellRightBorder,
               QTextFormat::TableCellRightBorderStyle,
               format.rightBorder(),
               format.rightBorderStyle(),
               format.rightBorderBrush());
    return borderStyle;
}

// A tag can only carry one style attribute, so everything a cell contributes to it has to be
// gathered here.
QString cellStyle(const QTextTableCellFormat &format)
{
    const QStringList declarations = cellPadding(format) + cellBorder(format);
    if (declarations.isEmpty()) {
        return {};
    }
    return u" style=\"%1;\""_s.arg(declarations.join(u"; "_s));
}

// The html border attribute only carries a width, it always paints a plain line. A table which
// asked for a dotted or a double border keeps it through css only. The html parser gives every
// table an outset border, which is exactly what the border attribute already means, so only a
// style the document really chose is worth a declaration. A table without a border has none.
QStringList tableBorder(const QTextTableFormat &format)
{
    const QTextFrameFormat::BorderStyle style = format.borderStyle();
    if (style == QTextFrameFormat::BorderStyle_None) {
        return {};
    }
    QStringList borderStyle;
    if (style != QTextFrameFormat::BorderStyle_Outset) {
        borderStyle.append(u"border-style: %1"_s.arg(cssBorderStyle(style)));
    }
    // Dark gray is the color the html parser invents for a bare border attribute, it says nothing
    // about the document.
    if (const QBrush brush = format.borderBrush(); brush.style() != Qt::NoBrush && brush.color().isValid() && brush.color() != QColor(Qt::darkGray)) {
        borderStyle.append(u"border-color: %1"_s.arg(cssColor(brush.color())));
    }
    return borderStyle;
}

// Qt writes all four margins on every table it imports, most of them zero, and a zero margin is
// what the renderer does anyway: only a margin which really moves the table is serialized.
QStringList tableMargins(const QTextTableFormat &format)
{
    QStringList marginStyle;
    const auto appendMargin = [&](QLatin1StringView side, qreal value) {
        if (!qFuzzyIsNull(value)) {
            marginStyle.append(u"margin-%1: %2"_s.arg(side, cssPixels(value)));
        }
    };
    appendMargin("top"_L1, format.topMargin());
    appendMargin("bottom"_L1, format.bottomMargin());
    appendMargin("left"_L1, format.leftMargin());
    appendMargin("right"_L1, format.rightMargin());
    return marginStyle;
}

// The space a table keeps between its border and its cells. It has no html attribute of its own,
// cellpadding is the padding of the cells, not of the table.
QStringList tablePadding(const QTextTableFormat &format)
{
    if (!format.hasProperty(QTextFormat::FramePadding) || qFuzzyIsNull(format.padding())) {
        return {};
    }
    return {u"padding: %1"_s.arg(cssPixels(format.padding()))};
}

// A floating table has the text flow around it. This is not the align attribute, which only moves
// the table inside the flow.
QStringList tableFloat(const QTextTableFormat &format)
{
    switch (format.position()) {
    case QTextFrameFormat::FloatLeft:
        return {u"float: left"_s};
    case QTextFrameFormat::FloatRight:
        return {u"float: right"_s};
    case QTextFrameFormat::InFlow:
        break;
    }
    return {};
}

// A tag can only carry one style attribute, so everything the table contributes to it has to be
// gathered here.
QString tableStyle(const QTextTableFormat &format)
{
    QStringList declarations = tableFloat(format) + tableBorder(format) + tableMargins(format) + tablePadding(format);
    if (format.borderCollapse()) {
        declarations.append(u"border-collapse:collapse"_s);
    }
    if (declarations.isEmpty()) {
        return {};
    }
    return u" style=\"%1;\""_s.arg(declarations.join(u"; "_s));
}

// <th> and <td> carry exactly the same attributes.
void appendCellAttributes(QString &text, const QTextTableCellFormat &format, const QTextLength &width)
{
    if (const QString sWidth = htmlLength(width); !sWidth.isEmpty()) {
        text.append(u" width=\"%1\""_s.arg(sWidth));
    }
    text.append(u" colspan=\"%1\" rowspan=\"%2\""_s.arg(format.tableCellColumnSpan()).arg(format.tableCellRowSpan()));
    text.append(htmlBackground(format));
    text.append(vAlignment(format));
    text.append(cellStyle(format));
    text.append(u">"_s);
}

}

TextHTMLBuilder::TextHTMLBuilder()
    : AbstractMarkupBuilder()
    , d_ptr(new TextHTMLBuilderPrivate(this))
{
}

TextHTMLBuilder::~TextHTMLBuilder()
{
    delete d_ptr;
}

void TextHTMLBuilder::beginStrong()
{
    Q_D(TextHTMLBuilder);
    d->mText.append(u"<strong>"_s);
}

void TextHTMLBuilder::endStrong()
{
    Q_D(TextHTMLBuilder);
    d->mText.append(u"</strong>"_s);
}

void TextHTMLBuilder::beginEmph()
{
    Q_D(TextHTMLBuilder);
    d->mText.append(u"<em>"_s);
}

void TextHTMLBuilder::endEmph()
{
    Q_D(TextHTMLBuilder);
    d->mText.append(u"</em>"_s);
}

void TextHTMLBuilder::beginUnderline()
{
    Q_D(TextHTMLBuilder);
    d->mText.append(u"<u>"_s);
}

void TextHTMLBuilder::endUnderline()
{
    Q_D(TextHTMLBuilder);
    d->mText.append(u"</u>"_s);
}

void TextHTMLBuilder::beginStrikeout()
{
    Q_D(TextHTMLBuilder);
    d->mText.append(u"<s>"_s);
}

void TextHTMLBuilder::endStrikeout()
{
    Q_D(TextHTMLBuilder);
    d->mText.append(u"</s>"_s);
}

void TextHTMLBuilder::beginForeground(const QBrush &brush)
{
    Q_D(TextHTMLBuilder);
    d->mText.append(u"<span style=\"color:%1;\">"_s.arg(brush.color().name()));
}

void TextHTMLBuilder::endForeground()
{
    Q_D(TextHTMLBuilder);
    d->mText.append(u"</span>"_s);
}

void TextHTMLBuilder::beginBackground(const QBrush &brush)
{
    Q_D(TextHTMLBuilder);
    d->mText.append(u"<span style=\"background-color:%1;\">"_s.arg(brush.color().name()));
}

void TextHTMLBuilder::endBackground()
{
    Q_D(TextHTMLBuilder);
    d->mText.append(u"</span>"_s);
}

void TextHTMLBuilder::beginAnchor(const QString &href, const QString &name)
{
    Q_D(TextHTMLBuilder);
    if (!href.isEmpty()) {
        if (!name.isEmpty()) {
            d->mText.append(u"<a href=\"%1\" name=\"%2\">"_s.arg(href.toHtmlEscaped(), name.toHtmlEscaped()));
        } else {
            d->mText.append(u"<a href=\"%1\">"_s.arg(href.toHtmlEscaped()));
        }
    } else {
        if (!name.isEmpty()) {
            d->mText.append(u"<a name=\"%1\">"_s.arg(name.toHtmlEscaped()));
        }
    }
}

void TextHTMLBuilder::endAnchor()
{
    Q_D(TextHTMLBuilder);
    d->mText.append(u"</a>"_s);
}

void TextHTMLBuilder::beginFontFamily(const QString &family)
{
    Q_D(TextHTMLBuilder);
    d->mText.append(u"<span style=\"font-family:%1;\">"_s.arg(cssFontFamily(family).toHtmlEscaped()));
}

void TextHTMLBuilder::endFontFamily()
{
    Q_D(TextHTMLBuilder);
    d->mText.append(u"</span>"_s);
}

void TextHTMLBuilder::beginFontPointSize(int size)
{
    Q_D(TextHTMLBuilder);
    d->mText.append(u"<span style=\"font-size:%1pt;\">"_s.arg(QString::number(size)));
}

void TextHTMLBuilder::endFontPointSize()
{
    Q_D(TextHTMLBuilder);
    d->mText.append(u"</span>"_s);
}

void TextHTMLBuilder::beginParagraph(Qt::Alignment al, qreal topMargin, qreal bottomMargin, qreal leftMargin, qreal rightMargin, bool rightToLeftText)
{
    Q_D(TextHTMLBuilder);
    // Don't put paragraph tags inside li tags. Qt bug reported.
    //     if (currentListItemStyles.size() != 0)
    //     {
    const QString styleString = u"margin-top:%1;margin-bottom:%2;margin-left:%3;margin-right:%4;"_s.arg(cssPixels(topMargin),
                                                                                                        cssPixels(bottomMargin),
                                                                                                        cssPixels(leftMargin),
                                                                                                        cssPixels(rightMargin));

    // Using == doesn't work here.
    // Using bitwise comparison because an alignment can contain a vertical and
    // a
    // horizontal part.
    if (al & Qt::AlignRight) {
        d->mText.append(u"<p align=\"right\""_s);
    } else if (al & Qt::AlignHCenter) {
        d->mText.append(u"<p align=\"center\""_s);
    } else if (al & Qt::AlignJustify) {
        d->mText.append(u"<p align=\"justify\""_s);
    } else if (al & Qt::AlignLeft) {
        d->mText.append(u"<p"_s);
    } else {
        d->mText.append(u"<p"_s);
    }
    // Bug in grantlee => style is not defined
    d->mText.append(u" style=\""_s + styleString + u'"');
    if (rightToLeftText) {
        d->mText.append(u" dir=\"rtl\""_s);
    }
    d->mText.append(u'>');
    //     }
}

void TextHTMLBuilder::beginHeader(int level)
{
    Q_D(TextHTMLBuilder);
    switch (level) {
    case 1:
        d->mText.append(u"<h1>"_s);
        break;
    case 2:
        d->mText.append(u"<h2>"_s);
        break;
    case 3:
        d->mText.append(u"<h3>"_s);
        break;
    case 4:
        d->mText.append(u"<h4>"_s);
        break;
    case 5:
        d->mText.append(u"<h5>"_s);
        break;
    case 6:
        d->mText.append(u"<h6>"_s);
        break;
    default:
        break;
    }
}

void TextHTMLBuilder::endHeader(int level)
{
    Q_D(TextHTMLBuilder);
    switch (level) {
    case 1:
        d->mText.append(u"</h1>"_s);
        break;
    case 2:
        d->mText.append(u"</h2>"_s);
        break;
    case 3:
        d->mText.append(u"</h3>"_s);
        break;
    case 4:
        d->mText.append(u"</h4>"_s);
        break;
    case 5:
        d->mText.append(u"</h5>"_s);
        break;
    case 6:
        d->mText.append(u"</h6>"_s);
        break;
    default:
        break;
    }
}

void TextHTMLBuilder::endParagraph()
{
    Q_D(TextHTMLBuilder);
    d->mText.append(u"</p>\n"_s);
}

void TextHTMLBuilder::addNewline()
{
    Q_D(TextHTMLBuilder);
    d->mText.append(u"<p>&nbsp;</p>"_s);
}

void TextHTMLBuilder::insertHorizontalRule(int width)
{
    Q_D(TextHTMLBuilder);
    if (width != -1) {
        d->mText.append(u"<hr width=\"%1\" />\n"_s.arg(width));
    } else {
        d->mText.append(u"<hr />\n"_s);
    }
}

void TextHTMLBuilder::insertImage(const QString &src, qreal width, qreal height)
{
    Q_D(TextHTMLBuilder);
    d->mText.append(u"<img src=\"%1\" "_s.arg(src.toHtmlEscaped()));
    if (width != 0) {
        d->mText.append(u"width=\"%1\" "_s.arg(width));
    }
    if (height != 0) {
        d->mText.append(u"height=\"%1\" "_s.arg(height));
    }
    d->mText.append(u"/>"_s);
}

void TextHTMLBuilder::beginList(QTextListFormat::Style type, int start)
{
    Q_D(TextHTMLBuilder);
    // A list starting on its first item is the default, so the attribute is only written when it
    // actually moves the numbering. QTextListFormat always carries the property, defaulting to 1.
    const QString startAttribute = (start == 1) ? QString() : u" start=\"%1\""_s.arg(start);
    d->currentListItemStyles.append(type);
    switch (type) {
    case QTextListFormat::ListDisc:
        d->mText.append(u"\n<ul type=\"disc\">\n"_s);
        break;
    case QTextListFormat::ListCircle:
        d->mText.append(u"\n<ul type=\"circle\">\n"_s);
        break;
    case QTextListFormat::ListSquare:
        d->mText.append(u"\n<ul type=\"square\">\n"_s);
        break;
    case QTextListFormat::ListDecimal:
        d->mText.append(u"\n<ol type=\"%1\"%2>\n"_s.arg(u"1"_s, startAttribute));
        break;
    case QTextListFormat::ListLowerAlpha:
        d->mText.append(u"\n<ol type=\"%1\"%2>\n"_s.arg(u"a"_s, startAttribute));
        break;
    case QTextListFormat::ListUpperAlpha:
        d->mText.append(u"\n<ol type=\"%1\"%2>\n"_s.arg(u"A"_s, startAttribute));
        break;
    case QTextListFormat::ListLowerRoman:
        d->mText.append(u"\n<ol type=\"%1\"%2>\n"_s.arg(u"i"_s, startAttribute));
        break;
    case QTextListFormat::ListUpperRoman:
        d->mText.append(u"\n<ol type=\"%1\"%2>\n"_s.arg(u"I"_s, startAttribute));
        break;
    default:
        break;
    }
}
void TextHTMLBuilder::endList()
{
    Q_D(TextHTMLBuilder);
    switch (d->currentListItemStyles.last()) {
    case QTextListFormat::ListDisc:
    case QTextListFormat::ListCircle:
    case QTextListFormat::ListSquare:
        d->mText.append(u"</ul>\n"_s);
        break;
    case QTextListFormat::ListDecimal:
    case QTextListFormat::ListLowerAlpha:
    case QTextListFormat::ListUpperAlpha:
    case QTextListFormat::ListLowerRoman:
    case QTextListFormat::ListUpperRoman:
        d->mText.append(u"</ol>\n"_s);
        break;
    default:
        break;
    }
    d->currentListItemStyles.removeLast();
}
void TextHTMLBuilder::beginListItem()
{
    Q_D(TextHTMLBuilder);
    d->mText.append(u"<li>"_s);
}

void TextHTMLBuilder::endListItem()
{
    Q_D(TextHTMLBuilder);
    d->mText.append(u"</li>\n"_s);
}

void TextHTMLBuilder::beginSuperscript()
{
    Q_D(TextHTMLBuilder);
    d->mText.append(u"<sup>"_s);
}

void TextHTMLBuilder::endSuperscript()
{
    Q_D(TextHTMLBuilder);
    d->mText.append(u"</sup>"_s);
}

void TextHTMLBuilder::beginSubscript()
{
    Q_D(TextHTMLBuilder);
    d->mText.append(u"<sub>"_s);
}

void TextHTMLBuilder::endSubscript()
{
    Q_D(TextHTMLBuilder);
    d->mText.append(u"</sub>"_s);
}

void TextHTMLBuilder::beginTable(const QTextTableFormat &format)
{
    Q_D(TextHTMLBuilder);
    const QString sWidth = htmlLength(format.width());
    // A table whose border style is BorderStyle_None has no visible border, whatever the border width says.
    // The html border attribute is an integer, so round a fractional width up: a hairline border is still a border.
    const int border = (format.borderStyle() == QTextFrameFormat::BorderStyle_None) ? 0 : qCeil(format.border());
    d->mText.append(u"<table cellpadding=\"%1\" cellspacing=\"%2\""_s.arg(format.cellPadding()).arg(format.cellSpacing()));
    if (!sWidth.isEmpty()) {
        d->mText.append(u" width=\"%1\""_s.arg(sWidth));
    }
    if (const QString sHeight = htmlLength(format.height()); !sHeight.isEmpty()) {
        d->mText.append(u" height=\"%1\""_s.arg(sHeight));
    }
    d->mText.append(u" border=\"%1\""_s.arg(border));
    d->mText.append(htmlBackground(format));
    // Qt adds Qt::AlignAbsolute to left/right
    switch (static_cast<int>(format.alignment() & (Qt::AlignLeft | Qt::AlignRight | Qt::AlignHCenter | Qt::AlignJustify))) {
    case Qt::AlignLeft:
        d->mText.append(u" align=\"left\""_s);
        break;
    case Qt::AlignRight:
        d->mText.append(u" align=\"right\""_s);
        break;
    case Qt::AlignHCenter:
        d->mText.append(u" align=\"center\""_s);
        break;
    case Qt::AlignJustify:
        d->mText.append(u" align=\"justify\""_s);
        break;
    default:
        break;
    }
    d->mText.append(tableStyle(format));
    d->mText.append(u">"_s);
}

void TextHTMLBuilder::beginTableRow()
{
    Q_D(TextHTMLBuilder);
    d->mText.append(u"<tr>"_s);
}

void TextHTMLBuilder::beginTableHeaderCell(const QTextTableCellFormat &format, const QTextLength &width)
{
    Q_D(TextHTMLBuilder);
    d->mText.append(u"<th"_s);
    appendCellAttributes(d->mText, format, width);
}

void TextHTMLBuilder::beginTableCell(const QTextTableCellFormat &format, const QTextLength &width)
{
    Q_D(TextHTMLBuilder);
    d->mText.append(u"<td"_s);
    appendCellAttributes(d->mText, format, width);
}

void TextHTMLBuilder::endTable()
{
    Q_D(TextHTMLBuilder);
    d->mText.append(u"</table>"_s);
}

void TextHTMLBuilder::endTableRow()
{
    Q_D(TextHTMLBuilder);
    d->mText.append(u"</tr>"_s);
}

void TextHTMLBuilder::endTableHeaderCell()
{
    Q_D(TextHTMLBuilder);
    d->mText.append(u"</th>"_s);
}

void TextHTMLBuilder::endTableCell()
{
    Q_D(TextHTMLBuilder);
    d->mText.append(u"</td>"_s);
}

void TextHTMLBuilder::appendLiteralText(const QString &text)
{
    Q_D(TextHTMLBuilder);
    const QString textEscaped = text.toHtmlEscaped();
    QString textEscapedResult;
    const int total = textEscaped.length();
    textEscapedResult.reserve(total);
    for (int i = 0; i < total; ++i) {
        const QChar c = textEscaped.at(i);

        if (c == u' ') {
            if (i == 0) {
                textEscapedResult += u"&nbsp;"_s;
            } else {
                if (i + 1 < textEscaped.length() && (textEscaped.at(i + 1) == u' ')) {
                    textEscapedResult += u"&nbsp;"_s;
                } else {
                    textEscapedResult += c;
                }
            }
        } else if (c == u'\t') {
            textEscapedResult += u"&nbsp;&nbsp;&nbsp; "_s;
        } else {
            textEscapedResult += c;
        }
    }
    d->mText.append(textEscapedResult);
}

void TextHTMLBuilder::appendRawText(const QString &text)
{
    Q_D(TextHTMLBuilder);
    d->mText.append(text);
}

QString TextHTMLBuilder::getResult()
{
    Q_D(TextHTMLBuilder);
    auto ret = std::move(d->mText);
    d->mText.clear();
    return ret;
}

void KPIMTextEdit::TextHTMLBuilder::addSingleBreakLine()
{
    Q_D(TextHTMLBuilder);
    d->mText.append("<br />"_L1);
}
