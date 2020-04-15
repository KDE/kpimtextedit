/*
   Copyright (C) 2020 Laurent Montel <montel@kde.org>
   based on code from Stephen Kelly <steveire@gmail.com>

   This library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Library General Public
   License as published by the Free Software Foundation; either
   version 2 of the License, or (at your option) any later version.

   This library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Library General Public License for more details.

   You should have received a copy of the GNU Library General Public License
   along with this library; see the file COPYING.LIB.  If not, write to
   the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
   Boston, MA 02110-1301, USA.
*/

#include "texthtmlbuilder.h"

#include <QList>
#include <QTextDocument>
#include <QDebug>

using namespace KPIMTextEdit;
TextHTMLBuilder::TextHTMLBuilder()
    : Grantlee::TextHTMLBuilder()
{
}

TextHTMLBuilder::~TextHTMLBuilder()
{
}

//Add &nbsp for avoiding to remove space in html
void TextHTMLBuilder::appendLiteralText(const QString &text)
{
    const QString textEscaped = text.toHtmlEscaped();
    QString textEscapedResult;
    for (int i = 0, total = textEscaped.count(); i < total; ++i) {
        const QChar c = textEscaped.at(i);

        if (c == QLatin1Char(' ')) {
            if (i == 0) {
                textEscapedResult += QStringLiteral("&nbsp;");
            } else {
                if (i + 1 < textEscaped.count() && (textEscaped.at(i + 1) == QLatin1Char(' '))) {
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
    appendRawText(textEscapedResult);
}



void TextHTMLBuilder::beginParagraph(Qt::Alignment al, qreal topMargin,
                                                   qreal bottomMargin, qreal leftMargin,
                                                   qreal rightMargin)
{
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
        appendRawText(QStringLiteral("<p align=\"right\" "));
    } else if (al & Qt::AlignHCenter) {
        appendRawText(QStringLiteral("<p align=\"center\" "));
    } else if (al & Qt::AlignJustify) {
        appendRawText(QStringLiteral("<p align=\"justify\" "));
    } else if (al & Qt::AlignLeft) {
        appendRawText(QStringLiteral("<p"));
    } else {
        appendRawText(QStringLiteral("<p"));
    }

    //Bug in grantlee => style is not defined
    if (!styleString.isEmpty()) {
        appendRawText(QStringLiteral(" style=\"") + styleString + QLatin1Char('"'));
    }
    appendRawText(QStringLiteral(">"));
    //     }

}
