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

#ifndef PLAINTEXTMARKUPBUILDER_H
#define PLAINTEXTMARKUPBUILDER_H

#define LETTERSINALPHABET 26
#define DIGITSOFFSET 10

#include <kdebug.h>

#include "abstractmarkupbuilder.h"

/**
@brief The PlainTextMarkupBuilder creates a simple marked up plain text document.

This class creates a simple plain text markup. 

Text that may be represented as 

    A paragraph with <b>bold</b> text, <i>italic</i> text, and <u>underlined</u> text.
    
would be output as 

    A paragraph with *bold* text /italic/ text, and _underlined_ text.
    
The markup is intended to be simple, plain and easily human readable. No markup is created for different font-familiy, font-size, foreground or background colors.

Lists are marked up by preceeding the list element with '*' for disc, 'o' for circle, 'X' for square, or a letter or number. Lists are also indented if nested.
eg:

    A. One
    B. Two
        o Three
        o Four
            * Five
            * Six
    C. Seven

External references such as external urls and images are represented in the body text as a reference, and references are maintained at the bottom of the output.

Eg,

        Here is a link to <a href="http://www.kde.org">KDE</a> and the <a href="http://pim.kde.org">KDEPIM project</a>.

becomes:

    Here is a link to KDE[1], and the KDEPIM project[2].

    ---- References ----
    [1] http://www.kde.org
    [2] http://pim.kde.org

@todo Move this to kdelibs when tested and in use.

@author Stephen Kelly <steveire@gmail.com>
@since 4.2

*/
class PlainTextMarkupBuilder : public AbstractMarkupBuilder
{
    public:
    /** Construct a new PlainTextMarkupBuilder. */
    PlainTextMarkupBuilder() {
        m_urls = QStringList();
    }

    virtual void beginStrong() { m_text.append("*"); }
    virtual void endStrong() { m_text.append("*"); }
    virtual void beginEmph() { m_text.append("/"); }
    virtual void endEmph() { m_text.append("/"); }
    virtual void beginUnderline() { m_text.append("_"); }
    virtual void endUnderline() { m_text.append("_"); }
    virtual void beginStrikeout() { m_text.append("-"); }
    virtual void endStrikeout() { m_text.append("-"); }
    
    virtual void beginAnchor(const QString &href) {
        if(!m_urls.contains(href)){

            m_urls.append(href);
        }
        activeLink = href;
    }

    virtual void endAnchor() {
        m_text.append(QString("[%1]").arg(m_urls.indexOf(activeLink) + 1));
    }

    virtual void endParagraph() { m_text.append("\n"); }
    virtual void addNewline() { m_text.append("\n"); }
    virtual void insertImage(const QString &src, qreal width, qreal height) {
        Q_UNUSED(width)
        Q_UNUSED(height)

        if(!m_urls.contains(src)){
            m_urls.append(src);
        }
        m_text.append(QString("[%1]").arg(m_urls.indexOf(src) + 1));
    }


    virtual void beginList(QTextListFormat::Style style) {
        currentListItemStyles.append(style);
        currentListItemNumbers.append(0);
    }

    virtual void endList() {
        if (!currentListItemNumbers.isEmpty())
        {
            currentListItemStyles.removeLast();
            currentListItemNumbers.removeLast();
        }
    }
    virtual void beginListItem() {
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
            m_text.append(" X  ");
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

    /**
    Get a letter string to represent a number.
    
    The numbers 1-26 are represented by a-z, and 27-52 by aa-az, 53-79 by ba-bz etc.
    
    @param The number to convert
    @return The letter string representation of the number.    
    */
    QString getLetterString(int itemNumber)
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


    virtual void endListItem() { 
        currentListItemNumbers.last() = currentListItemNumbers.last() +1;
    }

    
    virtual void beginSuperscript() { m_text.append("^{"); }

    virtual void endSuperscript() { m_text.append("}"); }
    
    virtual void beginSubscript() { m_text.append("_{"); }
    
    virtual void endSubscript() { m_text.append("}"); }

    virtual void appendLiteralText(const QString &text ) { m_text.append(text); }

    /**
    Returns the finalised plain text markup, including references at the end.
    */
    virtual QString& getResult() {
        QString &ret = m_text;
        ret.append(getReferences());
        return ret;
    }

    

protected:
    /**
    Gets a block of references in the body of the text.
    This is an ordered list of links and images in the text.
    */
    QString getReferences() {
        QString refs;
        if (!m_urls.isEmpty()){
            refs.append("\n---- References ----\n"); //TODO: i18n

            int index = 1;
            while (!m_urls.isEmpty())
            {
                refs.append(QString("[%1] %2\n").arg(index++).arg(m_urls.takeFirst()));
            }
        }
        return refs;
    }

private:
    QStringList m_urls;
    
    // TODO: Make this a QList<QPair<QTextListFormat::Style, int>> instead
    QList<QTextListFormat::Style> currentListItemStyles;
    QList<int> currentListItemNumbers;
    
    QString activeLink;

    QString m_text;

};

#endif
