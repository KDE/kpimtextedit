/*
   SPDX-FileCopyrightText: 2020-2025 Laurent Montel <montel@kde.org>
   based on code from Stephen Kelly <steveire@gmail.com>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "texthtmlbuilder.h"
using namespace Qt::Literals::StringLiterals;

#include <QDebug>
#include <QList>
#include <QTextDocument>

namespace KPIMTextEdit
{
class TextHTMLBuilderPrivate
{
public:
    TextHTMLBuilderPrivate(TextHTMLBuilder *b)
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
            d->mText.append(u"<a href=\"%1\" name=\"%2\">"_s.arg(href, name));
        } else {
            d->mText.append(u"<a href=\"%1\">"_s.arg(href));
        }
    } else {
        if (!name.isEmpty()) {
            d->mText.append(u"<a name=\"%1\">"_s.arg(name));
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
    d->mText.append(u"<span style=\"font-family:%1;\">"_s.arg(family));
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

void TextHTMLBuilder::beginParagraph(Qt::Alignment al, qreal topMargin, qreal bottomMargin, qreal leftMargin, qreal rightMargin, bool leftToRightText)
{
    Q_D(TextHTMLBuilder);
    // Don't put paragraph tags inside li tags. Qt bug reported.
    //     if (currentListItemStyles.size() != 0)
    //     {
    QString styleString;
    styleString.append(u"margin-top:%1;"_s.arg(topMargin));
    styleString.append(u"margin-bottom:%1;"_s.arg(bottomMargin));
    styleString.append(u"margin-left:%1;"_s.arg(leftMargin));
    styleString.append(u"margin-right:%1;"_s.arg(rightMargin));

    // Using == doesn't work here.
    // Using bitwise comparison because an alignment can contain a vertical and
    // a
    // horizontal part.
    if (al & Qt::AlignRight) {
        d->mText.append(u"<p align=\"right\" "_s);
    } else if (al & Qt::AlignHCenter) {
        d->mText.append(u"<p align=\"center\" "_s);
    } else if (al & Qt::AlignJustify) {
        d->mText.append(u"<p align=\"justify\" "_s);
    } else if (al & Qt::AlignLeft) {
        d->mText.append(u"<p"_s);
    } else {
        d->mText.append(u"<p"_s);
    }
    // Bug in grantlee => style is not defined
    d->mText.append(u" style=\""_s + styleString + u'"');
    if (leftToRightText) {
        d->mText.append(u" dir='rtl'"_s);
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
    d->mText.append(u"<p>&nbsp;"_s);
}

void TextHTMLBuilder::insertHorizontalRule(int width)
{
    Q_D(TextHTMLBuilder);
    if (width != -1) {
        d->mText.append(u"<hr width=\"%1\" />\n"_s.arg(width));
    }
    d->mText.append(u"<hr />\n"_s);
}

void TextHTMLBuilder::insertImage(const QString &src, qreal width, qreal height)
{
    Q_D(TextHTMLBuilder);
    d->mText.append(u"<img src=\"%1\" "_s.arg(src));
    if (width != 0) {
        d->mText.append(u"width=\"%2\" "_s.arg(width));
    }
    if (height != 0) {
        d->mText.append(u"height=\"%2\" "_s.arg(height));
    }
    d->mText.append(u"/>"_s);
}

void TextHTMLBuilder::beginList(QTextListFormat::Style type)
{
    Q_D(TextHTMLBuilder);
    d->currentListItemStyles.append(type);
    switch (type) {
    case QTextListFormat::ListDisc:
        d->mText.append(u"<ul type=\"disc\">\n"_s);
        break;
    case QTextListFormat::ListCircle:
        d->mText.append(u"\n<ul type=\"circle\">\n"_s);
        break;
    case QTextListFormat::ListSquare:
        d->mText.append(u"\n<ul type=\"square\">\n"_s);
        break;
    case QTextListFormat::ListDecimal:
        d->mText.append(u"\n<ol type=\"1\">\n"_s);
        break;
    case QTextListFormat::ListLowerAlpha:
        d->mText.append(u"\n<ol type=\"a\">\n"_s);
        break;
    case QTextListFormat::ListUpperAlpha:
        d->mText.append(u"\n<ol type=\"A\">\n"_s);
        break;
    case QTextListFormat::ListLowerRoman:
        d->mText.append(u"\n<ol type=\"i\">\n"_s);
        break;
    case QTextListFormat::ListUpperRoman:
        d->mText.append(u"\n<ol type=\"I\">\n"_s);
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

void TextHTMLBuilder::beginTable(qreal cellpadding, qreal cellspacing, const QString &width)
{
    Q_D(TextHTMLBuilder);
    d->mText.append(QStringLiteral("<table cellpadding=\"%1\" cellspacing=\"%2\" "
                                   "width=\"%3\" border=\"1\">")
                        .arg(cellpadding)
                        .arg(cellspacing)
                        .arg(width));
}

void TextHTMLBuilder::beginTableRow()
{
    Q_D(TextHTMLBuilder);
    d->mText.append(u"<tr>"_s);
}

void TextHTMLBuilder::beginTableHeaderCell(const QString &width, int colspan, int rowspan)
{
    Q_D(TextHTMLBuilder);
    d->mText.append(u"<th width=\"%1\" colspan=\"%2\" rowspan=\"%3\">"_s.arg(width).arg(colspan).arg(rowspan));
}

void TextHTMLBuilder::beginTableCell(const QString &width, int colspan, int rowspan)
{
    Q_D(TextHTMLBuilder);
    d->mText.append(u"<td width=\"%1\" colspan=\"%2\" rowspan=\"%3\">"_s.arg(width).arg(colspan).arg(rowspan));
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
    for (int i = 0, total = textEscaped.length(); i < total; ++i) {
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
    auto ret = d->mText;
    d->mText.clear();
    return ret;
}

void KPIMTextEdit::TextHTMLBuilder::addSingleBreakLine()
{
    Q_D(TextHTMLBuilder);
    d->mText.append("<br />"_L1);
}
