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

#include "plaintextmarkupbuilder.h"

PlainTextMarkupBuilder::PlainTextMarkupBuilder()
{
    m_urls = QStringList();
}

void PlainTextMarkupBuilder::beginStrong() { m_text.append("*"); }
void PlainTextMarkupBuilder::endStrong() { m_text.append("*"); }
void PlainTextMarkupBuilder::beginEmph() { m_text.append("/"); }
void PlainTextMarkupBuilder::endEmph() { m_text.append("/"); }
void PlainTextMarkupBuilder::beginUnderline() { m_text.append("_"); }
void PlainTextMarkupBuilder::endUnderline() { m_text.append("_"); }
void PlainTextMarkupBuilder::beginStrikeout() { m_text.append("-"); }
void PlainTextMarkupBuilder::endStrikeout() { m_text.append("-"); }

void PlainTextMarkupBuilder::beginLinkedAnchor(const QString &href) {
    if(!m_urls.contains(href)){

        m_urls.append(href);
    }
    activeLink = href;
}

void PlainTextMarkupBuilder::endAnchor() {
    m_text.append(QString("[%1]").arg(m_urls.indexOf(activeLink) + 1));
}

void PlainTextMarkupBuilder::endParagraph() { m_text.append("\n"); }
void PlainTextMarkupBuilder::addNewline() { m_text.append("\n"); }
void PlainTextMarkupBuilder::insertImage(const QString &src, qreal width, qreal height) {
    Q_UNUSED(width)
    Q_UNUSED(height)

    if(!m_urls.contains(src)){
        m_urls.append(src);
    }
    m_text.append(QString("[%1]").arg(m_urls.indexOf(src) + 1));
}


void PlainTextMarkupBuilder::beginList(QTextListFormat::Style style) {
    currentListItemStyles.append(style);
    currentListItemNumbers.append(0);
}

void PlainTextMarkupBuilder::endList() {
    if (!currentListItemNumbers.isEmpty())
    {
        currentListItemStyles.removeLast();
        currentListItemNumbers.removeLast();
    }
}
void PlainTextMarkupBuilder::beginListItem() {
    for (int i = 0; i < currentListItemNumbers.size(); i++){
        m_text.append("    ");
    }

    int itemNumber = currentListItemNumbers.last();
    QString letterString;

    switch(currentListItemStyles.last()){
    case QTextListFormat::ListDisc:
        m_text.append(" *  ");
        break;
    case QTextListFormat::ListCircle:
        m_text.append(" o  ");
        break;
    case QTextListFormat::ListSquare:
        m_text.append(" -  ");
        break;
    case QTextListFormat::ListDecimal:
        m_text.append(QString(" %1. ").arg(itemNumber +1));
        break;
    case QTextListFormat::ListLowerAlpha:
        m_text.append(QString(" %1. ").arg(getLetterString(itemNumber)));
        break;
    case QTextListFormat::ListUpperAlpha:
        m_text.append(QString(" %1. ").arg(getLetterString(itemNumber).toUpper()));
        break;
    default:
        break;
    }
}

QString PlainTextMarkupBuilder::getLetterString(int itemNumber)
{
    QString letterString;
    while (true)
    {
        // Create the letter string by prepending one char at a time.
        // The itemNumber is converted to a number in the base 36 (number of letters in the
        // alphabet plus 10) after being increased by 10 (to pass out the digits 0 to 9).
        letterString.prepend(QString("%1").arg((itemNumber % LETTERSINALPHABET) + DIGITSOFFSET,
                0, // no padding while building this string.
                LETTERSINALPHABET + DIGITSOFFSET));
        if( (itemNumber >= LETTERSINALPHABET)  )
        {
            itemNumber = itemNumber / LETTERSINALPHABET;
            itemNumber--;
        } else {
            break;
        }
    }
    return letterString;
}


void PlainTextMarkupBuilder::endListItem() {
    currentListItemNumbers.last() = currentListItemNumbers.last() +1;
}


void PlainTextMarkupBuilder::beginSuperscript() { m_text.append("^{"); }

void PlainTextMarkupBuilder::endSuperscript() { m_text.append("}"); }

void PlainTextMarkupBuilder::beginSubscript() { m_text.append("_{"); }

void PlainTextMarkupBuilder::endSubscript() { m_text.append("}"); }

void PlainTextMarkupBuilder::appendLiteralText(const QString &text ) { m_text.append(text); }

QString& PlainTextMarkupBuilder::getResult() {
    QString &ret = m_text;
    ret.append(getReferences());
    return ret;
}

QString PlainTextMarkupBuilder::getReferences() {
    QString refs;
    if (!m_urls.isEmpty())
    {
        refs.append("\n---- References ----\n"); //TODO: i18n

        int index = 1;
        while (!m_urls.isEmpty())
        {
            refs.append(QString("[%1] %2\n").arg(index++).arg(m_urls.takeFirst()));
        }
    }
    return refs;
}
