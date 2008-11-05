/*
    This file is part of KDE.

    Copyright (c) 2008 Stephen Kelly <steveire@gmail.com>

    This library is free software; you can redistribute it and/or modify it
    under the terms of the GNU Library General Public License as published by
    the Free Software Foundation; either version 2 of the License, or (at your
    option) any later version.

    This library is distributed in the hope that it will be useful, but WITHOUT
    ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
    FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Library General Public
    License for more details.

    You should have received a copy of the GNU Library General Public License
    along with this library; see the file COPYING.LIB.  If not, write to the
    Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA
    02110-1301, USA.
*/

#include "khtmlbuilder.h"

#include <QTextDocument>
#include <QList>

class KHTMLBuilderPrivate
{
public:
    KHTMLBuilderPrivate(KHTMLBuilder *b) : q_ptr(b) {

    }

    QList<QTextListFormat::Style> currentListItemStyles;
    QString m_text;

    KHTMLBuilder *q_ptr;

    Q_DECLARE_PUBLIC(KHTMLBuilder)

};

KHTMLBuilder::KHTMLBuilder() : d_ptr(new KHTMLBuilderPrivate(this))
{

}

KHTMLBuilder::~KHTMLBuilder()
{

}

void KHTMLBuilder::beginStrong()
{
    Q_D(KHTMLBuilder);;
    d->m_text.append("<strong>");
}

void KHTMLBuilder::endStrong()
{
    Q_D(KHTMLBuilder);
    d->m_text.append("</strong>");
}

void KHTMLBuilder::beginEmph()
{
    Q_D(KHTMLBuilder);
    d->m_text.append("<em>");
}

void KHTMLBuilder::endEmph()
{
    Q_D(KHTMLBuilder);
    d->m_text.append("</em>");
}

void KHTMLBuilder::beginUnderline()
{
    Q_D(KHTMLBuilder);
    d->m_text.append("<u>");
}

void KHTMLBuilder::endUnderline()
{
    Q_D(KHTMLBuilder);
    d->m_text.append("</u>");
}

void KHTMLBuilder::beginStrikeout()
{
    Q_D(KHTMLBuilder);
    d->m_text.append("<s>");
}

void KHTMLBuilder::endStrikeout()
{
    Q_D(KHTMLBuilder);
    d->m_text.append("</s>");
}

void KHTMLBuilder::beginForeground(const QBrush &brush)
{
    Q_D(KHTMLBuilder);
    d->m_text.append(QString("<span style=\"color:%1;\">").arg(brush.color().name()));
}

void KHTMLBuilder::endForeground()
{
    Q_D(KHTMLBuilder);
    d->m_text.append("</span>");
}

void KHTMLBuilder::beginBackground(const QBrush &brush)
{
    Q_D(KHTMLBuilder);
    d->m_text.append(QString("<span style=\"background-color:%1;\">").arg(brush.color().name()));
}

void KHTMLBuilder::endBackground()
{
    Q_D(KHTMLBuilder);
    d->m_text.append("</span>");
}

void KHTMLBuilder::beginAnchor(const QString &href, const QString &name)
{
    Q_D(KHTMLBuilder);
    if (!href.isEmpty()) {
        if (!name.isEmpty()) {
            d->m_text.append(QString("<a href=\"%1\" name=\"%2\">").arg(href).arg(name));
        } else {
            d->m_text.append(QString("<a href=\"%1\">").arg(href));
        }
    } else {
        if (!name.isEmpty()) {
            d->m_text.append(QString("<a name=\"%1\">").arg(name));
        }
    }
}

void KHTMLBuilder::endAnchor()
{
    Q_D(KHTMLBuilder);
    d->m_text.append("</a>");
}

void KHTMLBuilder::beginFontFamily(const QString &family)
{
    Q_D(KHTMLBuilder);
    d->m_text.append(QString("<span style=\"font-family:%1;\">").arg(family));
}

void KHTMLBuilder::endFontFamily()
{
    Q_D(KHTMLBuilder);
    d->m_text.append("</span>");
}

void KHTMLBuilder::beginFontPointSize(int size)
{
    Q_D(KHTMLBuilder);
    d->m_text.append(QString("<span style=\"font-size:%1pt;\">").arg(QString::number(size)));
}

void KHTMLBuilder::endFontPointSize()
{
    Q_D(KHTMLBuilder);
    d->m_text.append("</span>");
}

void KHTMLBuilder::beginParagraph(Qt::Alignment al, qreal topMargin, qreal bottomMargin, qreal leftMargin, qreal rightMargin)
{
    Q_D(KHTMLBuilder);
    // Don't put paragraph tags inside li tags. Qt bug reported.
//     if (currentListItemStyles.size() != 0)
//     {
    QString styleString;
    if (topMargin != 0) {
        styleString.append(QString("margin-top:%1;").arg(topMargin));
    }
    if (bottomMargin != 0) {
        styleString.append(QString("margin-bottom:%1;").arg(bottomMargin));
    }
    if (leftMargin != 0) {
        styleString.append(QString("margin-left:%1;").arg(leftMargin));
    }
    if (rightMargin != 0) {
        styleString.append(QString("margin-right:%1;").arg(rightMargin));
    }

    // Using == doesn't work here.
    // Using bitwise comparison because an alignment can contain a vertical and a horizontal part.
    if (al & Qt::AlignRight) {
        d->m_text.append("<p align=\"right\" ");
    } else if (al & Qt::AlignHCenter) {
        d->m_text.append("<p align=\"center\" ");
    } else if (al & Qt::AlignJustify) {
        d->m_text.append("<p align=\"justify\" ");
    } else if (al & Qt::AlignLeft) {
        d->m_text.append("<p");
    } else {
        d->m_text.append("<p");
    }

    if (!styleString.isEmpty()) {
        d->m_text.append(" \"" + styleString + "\"");
    }
    d->m_text.append(">");
//     }
}

void KHTMLBuilder::beginHeader(int level)
{
    Q_D(KHTMLBuilder);
    switch (level) {
    case 1:
        d->m_text.append("<h1>");
        break;
    case 2:
        d->m_text.append("<h2>");
        break;
    case 3:
        d->m_text.append("<h3>");
        break;
    case 4:
        d->m_text.append("<h4>");
        break;
    case 5:
        d->m_text.append("<h5>");
        break;
    case 6:
        d->m_text.append("<h6>");
        break;
    default:
        break;
    }
}

void KHTMLBuilder::endHeader(int level)
{
    Q_D(KHTMLBuilder);
    switch (level) {
    case 1:
        d->m_text.append("</h1>");
        break;
    case 2:
        d->m_text.append("</h2>");
        break;
    case 3:
        d->m_text.append("</h3>");
        break;
    case 4:
        d->m_text.append("</h4>");
        break;
    case 5:
        d->m_text.append("</h5>");
        break;
    case 6:
        d->m_text.append("</h6>");
        break;
    default:
        break;
    }
}

void KHTMLBuilder::endParagraph()
{
    Q_D(KHTMLBuilder);
    d->m_text.append("</p>\n");
}

void KHTMLBuilder::addNewline()
{
    Q_D(KHTMLBuilder);
    d->m_text.append("<br />\n");
}

void KHTMLBuilder::insertHorizontalRule(int width)
{
    Q_D(KHTMLBuilder);
    if (width != -1) {
        d->m_text.append(QString("<hr width=\"%1\" />\n").arg(width));
    }
    d->m_text.append("<hr />\n");
}

void KHTMLBuilder::insertImage(const QString &src, qreal width, qreal height)
{
    Q_D(KHTMLBuilder);
    d->m_text.append(QString("<img src=\"%1\" ").arg(src));
    if (width != 0) d->m_text.append(QString("width=\"%2\" ").arg(width));
    if (height != 0) d->m_text.append(QString("height=\"%2\" ").arg(height));
    d->m_text.append("/>");
}

void KHTMLBuilder::beginList(QTextListFormat::Style type)
{
    Q_D(KHTMLBuilder);
    d->currentListItemStyles.append(type);
    switch (type) {
    case QTextListFormat::ListDisc:
        d->m_text.append("<ul type=\"disc\">\n");
        break;
    case QTextListFormat::ListCircle:
        d->m_text.append("\n<ul type=\"circle\">\n");
        break;
    case QTextListFormat::ListSquare:
        d->m_text.append("\n<ul type=\"square\">\n");
        break;
    case QTextListFormat::ListDecimal:
        d->m_text.append("\n<ol type=\"1\">\n");
        break;
    case QTextListFormat::ListLowerAlpha:
        d->m_text.append("\n<ol type=\"a\">\n");
        break;
    case QTextListFormat::ListUpperAlpha:
        d->m_text.append("\n<ol type=\"A\">\n");
        break;
    default:
        break;
    }
}
void KHTMLBuilder::endList()
{
    Q_D(KHTMLBuilder);
    switch (d->currentListItemStyles.last()) {
    case QTextListFormat::ListDisc:
    case QTextListFormat::ListCircle:
    case QTextListFormat::ListSquare:
        d->m_text.append("</ul>\n");
        break;
    case QTextListFormat::ListDecimal:
    case QTextListFormat::ListLowerAlpha:
    case QTextListFormat::ListUpperAlpha:
        d->m_text.append("</ol>\n");
        break;
    default:
        break;
    }
    d->currentListItemStyles.removeLast();
}
void KHTMLBuilder::beginListItem()
{
    Q_D(KHTMLBuilder);
    d->m_text.append("<li>");
}

void KHTMLBuilder::endListItem()
{
    Q_D(KHTMLBuilder);
    d->m_text.append("</li>\n");
}

void KHTMLBuilder::beginSuperscript()
{
    Q_D(KHTMLBuilder);
    d->m_text.append("<sup>");
}

void KHTMLBuilder::endSuperscript()
{
    Q_D(KHTMLBuilder);
    d->m_text.append("</sup>");
}

void KHTMLBuilder::beginSubscript()
{
    Q_D(KHTMLBuilder);
    d->m_text.append("<sub>");
}

void KHTMLBuilder::endSubscript()
{
    Q_D(KHTMLBuilder);
    d->m_text.append("</sub>");
}


void KHTMLBuilder::beginTable(qreal cellpadding, qreal cellspacing, const QString &width)
{
    Q_D(KHTMLBuilder);
    d->m_text.append(QString("<table cellpadding=\"%1\" cellspacing=\"%2\" width=\"%3\" border=\"1\">")
                     .arg(cellpadding)
                     .arg(cellspacing)
                     .arg(width));
}

void KHTMLBuilder::beginTableRow()
{
    Q_D(KHTMLBuilder);
    d->m_text.append("<tr>");
}

void KHTMLBuilder::beginTableHeaderCell(const QString &width, int colspan, int rowspan)
{
    Q_D(KHTMLBuilder);
    d->m_text.append(QString("<th width=\"%1\" colspan=\"%2\" rowspan=\"%3\">").arg(width).arg(colspan).arg(rowspan));
}

void KHTMLBuilder::beginTableCell(const QString &width, int colspan, int rowspan)
{
    Q_D(KHTMLBuilder);
    d->m_text.append(QString("<td width=\"%1\" colspan=\"%2\" rowspan=\"%3\">").arg(width).arg(colspan).arg(rowspan));
}

void KHTMLBuilder::endTable()
{
    Q_D(KHTMLBuilder);
    d->m_text.append("</table>");
}

void KHTMLBuilder::endTableRow()
{
    Q_D(KHTMLBuilder);
    d->m_text.append("</tr>");
}

void KHTMLBuilder::endTableHeaderCell()
{
    Q_D(KHTMLBuilder);
    d->m_text.append("</th>");
}

void KHTMLBuilder::endTableCell()
{
    Q_D(KHTMLBuilder);
    d->m_text.append("</td>");
}


void KHTMLBuilder::appendLiteralText(const QString &text)
{
    Q_D(KHTMLBuilder);
    d->m_text.append(Qt::escape(text));
}


QString& KHTMLBuilder::getResult()
{
    Q_D(KHTMLBuilder);
    return d->m_text;
}
