/*
  Copyright (c) 2012 Montel Laurent <montel@kde.org>

  based on code from qt-labs-graphics-dojo/htmleditor/highlighter.*

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

#include "htmlhighlighter.h"

namespace KPIMTextEdit
{

class HtmlHighlighterPrivate
{
public:
    enum Construct {
        DocType,
        Entity,
        Tag,
        Comment,
        AttributeName,
        AttributeValue
    };

    enum State {
        State_Text = -1,
        State_DocType,
        State_Comment,
        State_TagStart,
        State_TagName,
        State_InsideTag,
        State_AttributeName,
        State_SingleQuote,
        State_DoubleQuote,
        State_AttributeValue
    };

    HtmlHighlighterPrivate()
    {
        colors[DocType] = QColor(192, 192, 192);
        colors[Entity] = QColor(128, 128, 128);
        colors[Tag] = QColor(136, 18, 128);
        colors[Comment] = QColor(35, 110, 37);
        colors[AttributeName] = QColor(153, 69, 0);
        colors[AttributeValue] = QColor(36, 36, 170);
    }
    QHash<int, QColor> colors;
};

HtmlHighlighter::HtmlHighlighter(QTextDocument *document)
    : QSyntaxHighlighter(document), d(new HtmlHighlighterPrivate)
{
}

HtmlHighlighter::~HtmlHighlighter()
{
    delete d;
}

void HtmlHighlighter::highlightBlock(const QString &text)
{
    int state = previousBlockState();
    int len = text.length();
    int start = 0;
    int pos = 0;

    while (pos < len) {
        switch (state) {
        case HtmlHighlighterPrivate::State_Text:
        default:
            while (pos < len) {
                QChar ch = text.at(pos);
                if (ch == QLatin1Char('<')) {
                    if (text.mid(pos, 4) == QLatin1String("<!--")) {
                        state = HtmlHighlighterPrivate::State_Comment;
                    } else {
                        if (text.mid(pos, 9).toUpper() == QLatin1String("<!DOCTYPE")) {
                            state = HtmlHighlighterPrivate::State_DocType;
                        } else {
                            state = HtmlHighlighterPrivate::State_TagStart;
                        }
                    }
                    break;
                } else if (ch == QLatin1Char('&')) {
                    start = pos;
                    while (pos < len && text.at(pos++) != QLatin1Char(';'))
                        ;
                    setFormat(start, pos - start, d->colors[HtmlHighlighterPrivate::Entity]);
                } else {
                    ++pos;
                }
            }
            break;

        case HtmlHighlighterPrivate::State_Comment:
            start = pos;
            while (pos < len) {
                if (text.mid(pos, 3) == QLatin1String("-->")) {
                    pos += 3;
                    state = HtmlHighlighterPrivate::State_Text;
                    break;
                } else {
                    ++pos;
                }
            }
            setFormat(start, pos - start, d->colors[HtmlHighlighterPrivate::Comment]);
            break;

        case HtmlHighlighterPrivate::State_DocType:
            start = pos;
            while (pos < len) {
                QChar ch = text.at(pos);
                ++pos;
                if (ch == QLatin1Char('>')) {
                    state = HtmlHighlighterPrivate::State_Text;
                    break;
                }
            }
            setFormat(start, pos - start, d->colors[HtmlHighlighterPrivate::DocType]);
            break;

        // at '<' in e.g. "<span>foo</span>"
        case HtmlHighlighterPrivate::State_TagStart:
            start = pos + 1;
            while (pos < len) {
                QChar ch = text.at(pos);
                ++pos;
                if (ch == QLatin1Char('>')) {
                    state = HtmlHighlighterPrivate::State_Text;
                    break;
                }
                if (!ch.isSpace()) {
                    --pos;
                    state = HtmlHighlighterPrivate::State_TagName;
                    break;
                }
            }
            break;

        // at 'b' in e.g "<blockquote>foo</blockquote>"
        case HtmlHighlighterPrivate::State_TagName:
            start = pos;
            while (pos < len) {
                QChar ch = text.at(pos);
                ++pos;
                if (ch.isSpace()) {
                    --pos;
                    state = HtmlHighlighterPrivate::State_InsideTag;
                    break;
                }
                if (ch == QLatin1Char('>')) {
                    state = HtmlHighlighterPrivate::State_Text;
                    break;
                }
            }
            setFormat(start, pos - start, d->colors[HtmlHighlighterPrivate::Tag]);
            break;

        // anywhere after tag name and before tag closing ('>')
        case HtmlHighlighterPrivate::State_InsideTag:
            start = pos;

            while (pos < len) {
                QChar ch = text.at(pos);
                ++pos;

                if (ch == QLatin1Char('/')) {
                    continue;
                }

                if (ch == QLatin1Char('>')) {
                    state = HtmlHighlighterPrivate::State_Text;
                    break;
                }

                if (!ch.isSpace()) {
                    --pos;
                    state = HtmlHighlighterPrivate::State_AttributeName;
                    break;
                }

            }

            break;

        // at 's' in e.g. <img src=bla.png/>
        case HtmlHighlighterPrivate::State_AttributeName:
            start = pos;

            while (pos < len) {
                QChar ch = text.at(pos);
                ++pos;

                if (ch == QLatin1Char('=')) {
                    state = HtmlHighlighterPrivate::State_AttributeValue;
                    break;
                }

                if (ch == QLatin1Char('>') || ch == QLatin1Char('/')) {
                    state = HtmlHighlighterPrivate::State_InsideTag;
                    break;
                }
            }

            setFormat(start, pos - start, d->colors[HtmlHighlighterPrivate::AttributeName]);
            break;

        // after '=' in e.g. <img src=bla.png/>
        case HtmlHighlighterPrivate::State_AttributeValue:
            start = pos;

            // find first non-space character
            while (pos < len) {
                QChar ch = text.at(pos);
                ++pos;

                // handle opening single quote
                if (ch == QLatin1Char('\'')) {
                    state = HtmlHighlighterPrivate::State_SingleQuote;
                    break;
                }

                // handle opening double quote
                if (ch == QLatin1Char('"')) {
                    state = HtmlHighlighterPrivate::State_DoubleQuote;
                    break;
                }

                if (!ch.isSpace()) {
                    break;
                }
            }

            if (state == HtmlHighlighterPrivate::State_AttributeValue) {
                // attribute value without quote
                // just stop at non-space or tag delimiter
                start = pos;
                while (pos < len) {
                    QChar ch = text.at(pos);
                    if (ch.isSpace()) {
                        break;
                    }
                    if (ch == QLatin1Char('>') || ch == QLatin1Char('/')) {
                        break;
                    }
                    ++pos;
                }
                state = HtmlHighlighterPrivate::State_InsideTag;
                setFormat(start, pos - start, d->colors[HtmlHighlighterPrivate::AttributeValue]);
            }

            break;

        // after the opening single quote in an attribute value
        case HtmlHighlighterPrivate::State_SingleQuote:
            start = pos;

            while (pos < len) {
                QChar ch = text.at(pos);
                ++pos;
                if (ch == QLatin1Char('\'')) {
                    break;
                }
            }

            state = HtmlHighlighterPrivate::State_InsideTag;

            setFormat(start, pos - start, d->colors[HtmlHighlighterPrivate::AttributeValue]);
            break;

        // after the opening double quote in an attribute value
        case HtmlHighlighterPrivate::State_DoubleQuote:
            start = pos;

            while (pos < len) {
                QChar ch = text.at(pos);
                ++pos;
                if (ch == QLatin1Char('"')) {
                    break;
                }
            }

            state = HtmlHighlighterPrivate::State_InsideTag;

            setFormat(start, pos - start, d->colors[HtmlHighlighterPrivate::AttributeValue]);
            break;

        }
    }

    setCurrentBlockState(state);
}

}

