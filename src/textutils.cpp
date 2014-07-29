/*
    This file is part of KDE.

    Copyright (c) 2009 Thomas McGuire <mcguire@kde.org>
    Copyright (c) 2010 Stephen Kelly <steveire@gmail.com>

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

#include "textutils.h"

#include <QTextBlock>
#include <QTextCharFormat>
#include <QTextDocument>
#include <QDebug>

using namespace KPIMTextEdit;

static bool isCharFormatFormatted(const QTextCharFormat &format, const QFont &defaultFont,
                                  const QTextCharFormat &defaultBlockFormat)
{
    if (!format.anchorHref().isEmpty() ||
            format.font() != defaultFont ||
            format.isAnchor() ||
            format.verticalAlignment() != defaultBlockFormat.verticalAlignment() ||
            format.layoutDirection() != defaultBlockFormat.layoutDirection() ||
            format.underlineStyle() != defaultBlockFormat.underlineStyle() ||
            format.foreground().color() != defaultBlockFormat.foreground().color() ||
            format.background().color() != defaultBlockFormat.background().color()) {
        return true;
    }

    return false;
}

static bool isBlockFormatFormatted(const QTextBlockFormat &format,
                                   const QTextBlockFormat &defaultFormat)
{
    if (format.alignment() != defaultFormat.alignment() ||
            format.layoutDirection() != defaultFormat.layoutDirection() ||
            format.indent() != defaultFormat.indent() ||
            format.textIndent() != defaultFormat.textIndent()) {
        return true;
    }

    return false;
}

/// @return true if the format represents a list, table, image or something like that.
static bool isSpecial(const QTextFormat &charFormat)
{
    return charFormat.isFrameFormat() || charFormat.isImageFormat() ||
           charFormat.isListFormat() || charFormat.isTableFormat() || charFormat.isTableCellFormat();
}

bool TextUtils::containsFormatting(const QTextDocument *document)
{
    if (!document) {
        return false;
    }

    QTextDocument defaultTextDocument;
    const QTextCharFormat defaultCharFormat = defaultTextDocument.begin().charFormat();
    const QTextBlockFormat defaultBlockFormat = defaultTextDocument.begin().blockFormat();
    const QFont defaultFont = defaultTextDocument.defaultFont();

    QTextBlock block = document->firstBlock();
    while (block.isValid()) {

        if (isBlockFormatFormatted(block.blockFormat(), defaultBlockFormat)) {
            return true;
        }

        if (isSpecial(block.charFormat()) || isSpecial(block.blockFormat()) ||
                block.textList()) {
            return true;
        }

        QTextBlock::iterator it = block.begin();
        while (!it.atEnd()) {
            const QTextFragment fragment = it.fragment();
            const QTextCharFormat charFormat = fragment.charFormat();
            if (isSpecial(charFormat)) {
                return true;
            }
            if (isCharFormatFormatted(fragment.charFormat(), defaultFont, defaultCharFormat)) {
                return true;
            }

            it++;
        }

        block = block.next();
    }

    if (document->toHtml().contains(QLatin1String("<hr />"))) {
        return true;
    }

    return false;
}

QString TextUtils::flowText(QString &wrappedText, const QString &indent, int maxLength)
{
    if (wrappedText.isEmpty()) {
        return indent;
    }

    if (maxLength <= indent.length()) {
        qWarning() << "indent was set to a string that is longer or the same length "
                   << "as maxLength, setting maxLength to indent.length() + 1";
        maxLength = indent.length() + 1;
    }

    maxLength -= indent.length(); // take into account indent
    QString result;
    while (!wrappedText.isEmpty()) {
        // first check for the next newline. if it's before maxLength, break there, and continue
        int newLine = wrappedText.indexOf(QLatin1Char('\n'));
        if (newLine > 0 && newLine <= maxLength) {
            result += indent + wrappedText.left(newLine + 1);
            wrappedText = wrappedText.mid(newLine + 1);
            continue;
        }
        // Find the next point in the wrappedText where we have to do a line break.
        // Start searching at maxLength position and then walk backwards looking
        // for a space.
        int breakPosition;
        if (wrappedText.length() > maxLength) {
            breakPosition = maxLength;
            while ((breakPosition >= 0) && (wrappedText[breakPosition] != QLatin1Char(' '))) {
                breakPosition--;
            }
            if (breakPosition <= 0) {
                // Couldn't break before maxLength.
                breakPosition = maxLength;
            }
        } else {
            breakPosition = wrappedText.length();
        }

        QString line = wrappedText.left(breakPosition);
        if (breakPosition < wrappedText.length()) {
            wrappedText = wrappedText.mid(breakPosition);
        } else {
            wrappedText.clear();
        }

        // Strip leading whitespace of new lines, since that looks strange
        if (!result.isEmpty() && line.startsWith(QLatin1Char(' '))) {
            line = line.mid(1);
        }

        result += indent + line + QLatin1Char('\n');
    }

    return result.left(result.length() - 1);
}
