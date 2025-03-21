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
    d->mText.append(QStringLiteral("<strong>"));
}

void TextHTMLBuilder::endStrong()
{
    Q_D(TextHTMLBuilder);
    d->mText.append(QStringLiteral("</strong>"));
}

void TextHTMLBuilder::beginEmph()
{
    Q_D(TextHTMLBuilder);
    d->mText.append(QStringLiteral("<em>"));
}

void TextHTMLBuilder::endEmph()
{
    Q_D(TextHTMLBuilder);
    d->mText.append(QStringLiteral("</em>"));
}

void TextHTMLBuilder::beginUnderline()
{
    Q_D(TextHTMLBuilder);
    d->mText.append(QStringLiteral("<u>"));
}

void TextHTMLBuilder::endUnderline()
{
    Q_D(TextHTMLBuilder);
    d->mText.append(QStringLiteral("</u>"));
}

void TextHTMLBuilder::beginStrikeout()
{
    Q_D(TextHTMLBuilder);
    d->mText.append(QStringLiteral("<s>"));
}

void TextHTMLBuilder::endStrikeout()
{
    Q_D(TextHTMLBuilder);
    d->mText.append(QStringLiteral("</s>"));
}

void TextHTMLBuilder::beginForeground(const QBrush &brush)
{
    Q_D(TextHTMLBuilder);
    d->mText.append(QStringLiteral("<span style=\"color:%1;\">").arg(brush.color().name()));
}

void TextHTMLBuilder::endForeground()
{
    Q_D(TextHTMLBuilder);
    d->mText.append(QStringLiteral("</span>"));
}

void TextHTMLBuilder::beginBackground(const QBrush &brush)
{
    Q_D(TextHTMLBuilder);
    d->mText.append(QStringLiteral("<span style=\"background-color:%1;\">").arg(brush.color().name()));
}

void TextHTMLBuilder::endBackground()
{
    Q_D(TextHTMLBuilder);
    d->mText.append(QStringLiteral("</span>"));
}

void TextHTMLBuilder::beginAnchor(const QString &href, const QString &name)
{
    Q_D(TextHTMLBuilder);
    if (!href.isEmpty()) {
        if (!name.isEmpty()) {
            d->mText.append(QStringLiteral("<a href=\"%1\" name=\"%2\">").arg(href, name));
        } else {
            d->mText.append(QStringLiteral("<a href=\"%1\">").arg(href));
        }
    } else {
        if (!name.isEmpty()) {
            d->mText.append(QStringLiteral("<a name=\"%1\">").arg(name));
        }
    }
}

void TextHTMLBuilder::endAnchor()
{
    Q_D(TextHTMLBuilder);
    d->mText.append(QStringLiteral("</a>"));
}

void TextHTMLBuilder::beginFontFamily(const QString &family)
{
    Q_D(TextHTMLBuilder);
    d->mText.append(QStringLiteral("<span style=\"font-family:%1;\">").arg(family));
}

void TextHTMLBuilder::endFontFamily()
{
    Q_D(TextHTMLBuilder);
    d->mText.append(QStringLiteral("</span>"));
}

void TextHTMLBuilder::beginFontPointSize(int size)
{
    Q_D(TextHTMLBuilder);
    d->mText.append(QStringLiteral("<span style=\"font-size:%1pt;\">").arg(QString::number(size)));
}

void TextHTMLBuilder::endFontPointSize()
{
    Q_D(TextHTMLBuilder);
    d->mText.append(QStringLiteral("</span>"));
}

void TextHTMLBuilder::beginParagraph(Qt::Alignment al, qreal topMargin, qreal bottomMargin, qreal leftMargin, qreal rightMargin, bool leftToRightText)
{
    Q_D(TextHTMLBuilder);
    // Don't put paragraph tags inside li tags. Qt bug reported.
    //     if (currentListItemStyles.size() != 0)
    //     {
    QString styleString;
    styleString.append(QStringLiteral("margin-top:%1;").arg(topMargin));
    styleString.append(QStringLiteral("margin-bottom:%1;").arg(bottomMargin));
    styleString.append(QStringLiteral("margin-left:%1;").arg(leftMargin));
    styleString.append(QStringLiteral("margin-right:%1;").arg(rightMargin));

    // Using == doesn't work here.
    // Using bitwise comparison because an alignment can contain a vertical and
    // a
    // horizontal part.
    if (al & Qt::AlignRight) {
        d->mText.append(QStringLiteral("<p align=\"right\" "));
    } else if (al & Qt::AlignHCenter) {
        d->mText.append(QStringLiteral("<p align=\"center\" "));
    } else if (al & Qt::AlignJustify) {
        d->mText.append(QStringLiteral("<p align=\"justify\" "));
    } else if (al & Qt::AlignLeft) {
        d->mText.append(QStringLiteral("<p"));
    } else {
        d->mText.append(QStringLiteral("<p"));
    }
    // Bug in grantlee => style is not defined
    if (!styleString.isEmpty()) {
        d->mText.append(QStringLiteral(" style=\"") + styleString + QLatin1Char('"'));
    }
    if (leftToRightText) {
        d->mText.append(QStringLiteral(" dir='rtl'"));
    }
    d->mText.append(QLatin1Char('>'));
    //     }
}

void TextHTMLBuilder::beginHeader(int level)
{
    Q_D(TextHTMLBuilder);
    switch (level) {
    case 1:
        d->mText.append(QStringLiteral("<h1>"));
        break;
    case 2:
        d->mText.append(QStringLiteral("<h2>"));
        break;
    case 3:
        d->mText.append(QStringLiteral("<h3>"));
        break;
    case 4:
        d->mText.append(QStringLiteral("<h4>"));
        break;
    case 5:
        d->mText.append(QStringLiteral("<h5>"));
        break;
    case 6:
        d->mText.append(QStringLiteral("<h6>"));
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
        d->mText.append(QStringLiteral("</h1>"));
        break;
    case 2:
        d->mText.append(QStringLiteral("</h2>"));
        break;
    case 3:
        d->mText.append(QStringLiteral("</h3>"));
        break;
    case 4:
        d->mText.append(QStringLiteral("</h4>"));
        break;
    case 5:
        d->mText.append(QStringLiteral("</h5>"));
        break;
    case 6:
        d->mText.append(QStringLiteral("</h6>"));
        break;
    default:
        break;
    }
}

void TextHTMLBuilder::endParagraph()
{
    Q_D(TextHTMLBuilder);
    d->mText.append(QStringLiteral("</p>\n"));
}

void TextHTMLBuilder::addNewline()
{
    Q_D(TextHTMLBuilder);
    d->mText.append(QStringLiteral("<p>&nbsp;"));
}

void TextHTMLBuilder::insertHorizontalRule(int width)
{
    Q_D(TextHTMLBuilder);
    if (width != -1) {
        d->mText.append(QStringLiteral("<hr width=\"%1\" />\n").arg(width));
    }
    d->mText.append(QStringLiteral("<hr />\n"));
}

void TextHTMLBuilder::insertImage(const QString &src, qreal width, qreal height)
{
    Q_D(TextHTMLBuilder);
    d->mText.append(QStringLiteral("<img src=\"%1\" ").arg(src));
    if (width != 0) {
        d->mText.append(QStringLiteral("width=\"%2\" ").arg(width));
    }
    if (height != 0) {
        d->mText.append(QStringLiteral("height=\"%2\" ").arg(height));
    }
    d->mText.append(QStringLiteral("/>"));
}

void TextHTMLBuilder::beginList(QTextListFormat::Style type)
{
    Q_D(TextHTMLBuilder);
    d->currentListItemStyles.append(type);
    switch (type) {
    case QTextListFormat::ListDisc:
        d->mText.append(QStringLiteral("<ul type=\"disc\">\n"));
        break;
    case QTextListFormat::ListCircle:
        d->mText.append(QStringLiteral("\n<ul type=\"circle\">\n"));
        break;
    case QTextListFormat::ListSquare:
        d->mText.append(QStringLiteral("\n<ul type=\"square\">\n"));
        break;
    case QTextListFormat::ListDecimal:
        d->mText.append(QStringLiteral("\n<ol type=\"1\">\n"));
        break;
    case QTextListFormat::ListLowerAlpha:
        d->mText.append(QStringLiteral("\n<ol type=\"a\">\n"));
        break;
    case QTextListFormat::ListUpperAlpha:
        d->mText.append(QStringLiteral("\n<ol type=\"A\">\n"));
        break;
    case QTextListFormat::ListLowerRoman:
        d->mText.append(QStringLiteral("\n<ol type=\"i\">\n"));
        break;
    case QTextListFormat::ListUpperRoman:
        d->mText.append(QStringLiteral("\n<ol type=\"I\">\n"));
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
        d->mText.append(QStringLiteral("</ul>\n"));
        break;
    case QTextListFormat::ListDecimal:
    case QTextListFormat::ListLowerAlpha:
    case QTextListFormat::ListUpperAlpha:
    case QTextListFormat::ListLowerRoman:
    case QTextListFormat::ListUpperRoman:
        d->mText.append(QStringLiteral("</ol>\n"));
        break;
    default:
        break;
    }
    d->currentListItemStyles.removeLast();
}
void TextHTMLBuilder::beginListItem()
{
    Q_D(TextHTMLBuilder);
    d->mText.append(QStringLiteral("<li>"));
}

void TextHTMLBuilder::endListItem()
{
    Q_D(TextHTMLBuilder);
    d->mText.append(QStringLiteral("</li>\n"));
}

void TextHTMLBuilder::beginSuperscript()
{
    Q_D(TextHTMLBuilder);
    d->mText.append(QStringLiteral("<sup>"));
}

void TextHTMLBuilder::endSuperscript()
{
    Q_D(TextHTMLBuilder);
    d->mText.append(QStringLiteral("</sup>"));
}

void TextHTMLBuilder::beginSubscript()
{
    Q_D(TextHTMLBuilder);
    d->mText.append(QStringLiteral("<sub>"));
}

void TextHTMLBuilder::endSubscript()
{
    Q_D(TextHTMLBuilder);
    d->mText.append(QStringLiteral("</sub>"));
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
    d->mText.append(QStringLiteral("<tr>"));
}

void TextHTMLBuilder::beginTableHeaderCell(const QString &width, int colspan, int rowspan)
{
    Q_D(TextHTMLBuilder);
    d->mText.append(QStringLiteral("<th width=\"%1\" colspan=\"%2\" rowspan=\"%3\">").arg(width).arg(colspan).arg(rowspan));
}

void TextHTMLBuilder::beginTableCell(const QString &width, int colspan, int rowspan)
{
    Q_D(TextHTMLBuilder);
    d->mText.append(QStringLiteral("<td width=\"%1\" colspan=\"%2\" rowspan=\"%3\">").arg(width).arg(colspan).arg(rowspan));
}

void TextHTMLBuilder::endTable()
{
    Q_D(TextHTMLBuilder);
    d->mText.append(QStringLiteral("</table>"));
}

void TextHTMLBuilder::endTableRow()
{
    Q_D(TextHTMLBuilder);
    d->mText.append(QStringLiteral("</tr>"));
}

void TextHTMLBuilder::endTableHeaderCell()
{
    Q_D(TextHTMLBuilder);
    d->mText.append(QStringLiteral("</th>"));
}

void TextHTMLBuilder::endTableCell()
{
    Q_D(TextHTMLBuilder);
    d->mText.append(QStringLiteral("</td>"));
}

void TextHTMLBuilder::appendLiteralText(const QString &text)
{
    Q_D(TextHTMLBuilder);
    const QString textEscaped = text.toHtmlEscaped();
    QString textEscapedResult;
    for (int i = 0, total = textEscaped.length(); i < total; ++i) {
        const QChar c = textEscaped.at(i);

        if (c == QLatin1Char(' ')) {
            if (i == 0) {
                textEscapedResult += QStringLiteral("&nbsp;");
            } else {
                if (i + 1 < textEscaped.length() && (textEscaped.at(i + 1) == QLatin1Char(' '))) {
                    textEscapedResult += QStringLiteral("&nbsp;");
                } else {
                    textEscapedResult += c;
                }
            }
        } else if (c == QLatin1Char('\t')) {
            textEscapedResult += QStringLiteral("&nbsp;&nbsp;&nbsp; ");
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
