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
#include "kpimtextedit_debug.h"
#include <QTextBlock>
#include <QTextCharFormat>
#include <QTextDocument>

using namespace KPIMTextEdit;

static bool isCharFormatFormatted(const QTextCharFormat &format, const QFont &defaultFont, const QTextCharFormat &defaultBlockFormat)
{
    if (!format.anchorHref().isEmpty()
        || format.font() != defaultFont
        || format.isAnchor()
        || format.verticalAlignment() != defaultBlockFormat.verticalAlignment()
        || format.layoutDirection() != defaultBlockFormat.layoutDirection()
        || format.underlineStyle() != defaultBlockFormat.underlineStyle()
        || format.foreground().color() != defaultBlockFormat.foreground().color()
        || format.background().color() != defaultBlockFormat.background().color()) {
        return true;
    }

    return false;
}

static bool isBlockFormatFormatted(const QTextBlockFormat &format, const QTextBlockFormat &defaultFormat)
{
    if (format.alignment() != defaultFormat.alignment()
        || format.layoutDirection() != defaultFormat.layoutDirection()
        || format.indent() != defaultFormat.indent()
        || format.textIndent() != defaultFormat.textIndent()) {
        return true;
    }

    return false;
}

/// @return true if the format represents a list, table, image or something like that.
static bool isSpecial(const QTextFormat &charFormat)
{
    return charFormat.isFrameFormat() || charFormat.isImageFormat()
           || charFormat.isListFormat() || charFormat.isTableFormat() || charFormat.isTableCellFormat();
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

        if (isSpecial(block.charFormat()) || isSpecial(block.blockFormat())
            || block.textList()) {
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

            ++it;
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
        qCWarning(KPIMTEXTEDIT_LOG) << "indent was set to a string that is longer or the same length "
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

QList<uint> TextUtils::unicodeFullEmoji()
{
    return TextUtils::unicodeFacesEmoji()
           +TextUtils::unicodeAnimalsEmoji()
           +TextUtils::unicodeEmotionEmoji()
           +TextUtils::unicodeBodyEmoji()
           +TextUtils::unicodeTransportEmoji()
           +TextUtils::unicodeEventEmoji()
           +TextUtils::unicodeFlagsEmoji()
           +TextUtils::unicodeWeatherEmoji()
           +TextUtils::unicodeFoodEmoji()
           +TextUtils::unicodeSportEmoji()
           +TextUtils::unicodeTimeEmoji()
           +TextUtils::unicodeGameEmoji()
           +TextUtils::unicodeClothingEmoji()
           +TextUtils::unicodeSoundMusicEmoji();
}

QList<uint> TextUtils::unicodeFacesEmoji()
{
    const QList<uint> lstEmoji{0x1F600, 0x1F603, 0x1F604, 0x1F601, 0x1F606, 0x1F6005, 0x1F923, 0x1F602, 0x1F642, 0x1F643, 0x1F609, 0x1F60A, 0x1F607,
                               0x1F970, 0x1F60D, 0x1F929, 0x1F618, 0x1F617, 0x1F61A, 0x1F619, 0x1F60B, 0x1F61B, 0x1F61C, 0x1F92A, 0x1F61D, 0x1F911,
                               0x1F917, 0x1F92D, 0x1F92B, 0x1F914, 0x1F910, 0x1F928, 0x1F610, 0x1F611, 0x1F636, 0x1F60F, 0x1F612, 0x1F644, 0x1F62C, 0x1F925,
                               0x1F60C, 0x1F614, 0x1F62A, 0x1F924, 0x1F634, 0x1F637, 0x1F912, 0x1F915, 0x1F922, 0x1F92E, 0x1F927, 0x1F975, 0x1F976, 0x1F974, 0x1F635, 0x1F92F,
                               0x1F920, 0x1F973, 0x1F60E, 0x1F913, 0x1F908, 0x1F615, 0x1F61F, 0x1F641, 0x1F62E, 0x1F62F, 0x1F632, 0x1F633, 0x1F97A, 0x1F626, 0x1F627}; //Add more
    return lstEmoji;
}

QList<uint> TextUtils::unicodeAnimalsEmoji()
{
    const QList<uint> lstEmoji{0x1F63A, 0x1F638, 0x1F639, 0x1F638, 0x1F63C, 0x1F63D, 0x1F640, 0x1F63F, 0x1F63E, 0x1F648, 0x1F649, 0x1F64A, 0x1F435, 0x1F412, 0x1F98D, 0x1F431, 0x1F984, 0x1F410,
                               0x1F401, 0x1F400, 0x1F43F, 0x1F428, 0x1F414, 0x1F423, 0x1F54A, 0x1F986, 0x1F99A, 0x1F99C, 0x1F40A, 0x1F577, 0x1F982, 0x1F9A0, 0x1F41E};
    return lstEmoji;
}

QList<uint> TextUtils::unicodeEmotionEmoji()
{
    //Add more
    const QList<uint> lstEmoji{0x1F48B, 0x1F48C, 0x1F498, 0x1F49D, 0x1F496, 0x1F497, 0x1F493, 0x1F49E, 0x1F495, 0x1F4AF, 0x1F4A2, 0x1F4A5, 0x1F4A4};
    return lstEmoji;
}

QList<uint> TextUtils::unicodeBodyEmoji()
{
    //Add more
    const QList<uint> lstEmoji{0x1F44B, 0x1F91A, 0x1F590, 0x270B, 0x1F596, 0x1F44C, 0x270C, 0x1F91F, 0x1F44D, 0x1F44F, 0x1F4AA};
    return lstEmoji;
}

QList<uint> TextUtils::unicodeTransportEmoji()
{
    //Add more
    const QList<uint> lstEmoji{0x1F682, 0x1F683, 0x1F684, 0x1F685, 0x1F686, 0x1F687, 0x1F688, 0x1F689, 0x1F69D, 0x1F691, 0x1F692, 0x1F693, 0x1F694, 0x1F695, 0x1F696, 0x1F697, 0x2708,
                               0x1F6E9, 0x1F6EB, 0x1F6EC, 0x1F681, 0x1F69F, 0x1F6A0, 0x1F680, 0x1F6F0, 0x1F6F8};
    return lstEmoji;
}

QList<uint> TextUtils::unicodeEventEmoji()
{
    //Add more
    const QList<uint> lstEmoji{0x1F383, 0x1F384, 0x1F386, 0x1F387, 0x1F9E8, 0x2728, 0x1F388, 0x1F389, 0x1F38A, 0x1F38B, 0x1F38D, 0x1F38E, 0x1F38F, 0x1F390, 0x1F391};
    return lstEmoji;
}

QList<uint> TextUtils::unicodeFlagsEmoji()
{
    //Add more
    const QList<uint> lstEmoji{ /*0x1F1E8, 0x1F1E9, 0x1F1EA, 0x1F1EB, 0x1F1EC, 0x1F1EE, 0x1F1FB, 0x1F1FE,*/ 0x1F3C1, 0x1F6A9, 0x1F3F4, 0x1F3F3};
    return lstEmoji;
}

QList<uint> TextUtils::unicodeWeatherEmoji()
{
    //Add more
    const QList<uint> lstEmoji{0x2601, 0x26C5, 0x26C8, 0x1F324, 0x1F325, 0x1F326, 0x1F327, 0x1F328, 0x1F329, 0x1F32A, 0x1F32B, 0x1F32C, 0x1F300, 0x1F308, 0x1F302,
                               0x2602, 0x2603, 0x26C4, 0x2604, 0x1F525, 0x1F4A7, 0x1F30A};
    return lstEmoji;
}

QList<uint> TextUtils::unicodeFoodEmoji()
{
    //Add more
    const QList<uint> lstEmoji{0x1F347, 0x1F348, 0x1F349, 0x1F34A, 0x1F34B, 0x1F34C, 0x1F34D, 0x1F34E, 0x1F34F, 0x1F350, 0x1F351, 0x1F352, 0x1F353, 0x1F951, 0x1F346, 0x1F954,
                               0x1F35E, 0x1F950, 0x1F956, 0x1F968, 0x1F9C0, 0x1F356, 0x1F357, 0x1F969, 0x1F953, 0x1F354, 0x1F35F, 0x1F355, 0x1F32D, 0x1F96A, 0x1F32E, 0x1F32F, 0x1F959,
                               0x1F95A, 0x1F373, 0x1F958, 0x1F372, 0x1F963, 0x1F957, 0x1F371, 0x1F358, 0x1F359, 0x1F35A, 0x1F35B, 0x1F980, 0x1F99E, 0x1F990, 0x1F991,
                               0x1F366, 0x1F367, 0x1F368};
    return lstEmoji;
}

QList<uint> TextUtils::unicodeSportEmoji()
{
    //Add more
    const QList<uint> lstEmoji{0x1F93A, 0x1F3C7, 0x26F7, 0x1F3C2, 0x1F3CC, 0x1F3C4, 0x1F6A3, 0x1F3CA, 0x26F9, 0x1F3CB, 0x1F6B4, 0x1F938, 0x1F93C, 0x1F93D, 0x1F93E, 0x26BD,
                               0x26BE, 0x1F94E, 0x1F3C0, 0x1F3D0, 0x1F3C8, 0x1F3C9, 0x1F3BE, 0x1F94F, 0x1F3B3, 0x1F3CF, 0x1F3D1, 0x26F8, 0x1F3A3, 0x1F94C};
    return lstEmoji;
}

QList<uint> TextUtils::unicodeTimeEmoji()
{
    //Add more
    const QList<uint> lstEmoji{0x231B, 0x23F3, 0x231A, 0x23F0, 0x23F1, 0x23F2, 0x1F570, 0x1F55B, 0x1F567, 0x1F550, 0x1F55C, 0x1F551, 0x1F55D, 0x1F552, 0x1F55E,
                               0x1F553, 0x1F55F, 0x1F554, 0x1F560, 0x1F555, 0x1F561, 0x1F556, 0x1F562, 0x1F557, 0x1F563, 0x1F558, 0x1F564,
                               0x1F559, 0x1F565, 0x1F55A, 0x1F566};
    return lstEmoji;
}

QList<uint> TextUtils::unicodeGameEmoji()
{
    //Add more
    const QList<uint> lstEmoji{0x1F3AF, 0x1F3B1, 0x1F52E, 0x1F9FF, 0x1F3AE, 0x1F579, 0x1F3B0, 0x1F3B2, 0x1F9F8, 0x265F, 0x1F0CF};
    return lstEmoji;
}

QList<uint> TextUtils::unicodeClothingEmoji()
{
    //Add more
    const QList<uint> lstEmoji{0x1F453, 0x1F576, 0x1F97D, 0x1F97C, 0x1F454, 0x1F455, 0x1F456, 0x1F9E3, 0x1F9E4, 0x1F9E5, 0x1F9E6, 0x1F457, 0x1F458, 0x1F459, 0x1F45A,
                               0x1F45B, 0x1F45C, 0x1F45D};
    return lstEmoji;
}

QList<uint> TextUtils::unicodeSoundMusicEmoji()
{
    //Add more
    const QList<uint> lstEmoji{0x1F507, 0x1F508, 0x1F509, 0x1F50A, 0x1F4E2, 0x1F4E3, 0x1F4EF, 0x1F514, 0x1F515,
                              0x1F3BC, 0x1F3B5, 0x1F3B6, 0x1F399, 0x1F39A, 0x1F39B, 0x1F3A4, 0x1F3A7, 0x1F4FB,
                              0x1F3B7, 0x1F3B8, 0x1F3B9, 0x1F3BA, 0x1F3BB, 0x1F941
                              };
    return lstEmoji;
}
