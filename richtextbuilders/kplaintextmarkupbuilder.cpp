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

#include "kplaintextmarkupbuilder.h"

KPlainTextMarkupBuilder::KPlainTextMarkupBuilder()
{
    m_urls = QStringList();
}

void KPlainTextMarkupBuilder::beginStrong()
{
    m_text.append ( "*" );
}
void KPlainTextMarkupBuilder::endStrong()
{
    m_text.append ( "*" );
}
void KPlainTextMarkupBuilder::beginEmph()
{
    m_text.append ( "/" );
}
void KPlainTextMarkupBuilder::endEmph()
{
    m_text.append ( "/" );
}
void KPlainTextMarkupBuilder::beginUnderline()
{
    m_text.append ( "_" );
}
void KPlainTextMarkupBuilder::endUnderline()
{
    m_text.append ( "_" );
}
void KPlainTextMarkupBuilder::beginStrikeout()
{
    m_text.append ( "-" );
}
void KPlainTextMarkupBuilder::endStrikeout()
{
    m_text.append ( "-" );
}

void KPlainTextMarkupBuilder::beginAnchor ( const QString &href, const QString &name )
{
    if ( !m_urls.contains ( href ) ) {

        m_urls.append ( href );
    }
    activeLink = href;
}

void KPlainTextMarkupBuilder::endAnchor()
{
    m_text.append ( QString ( "[%1]" ).arg ( m_urls.indexOf ( activeLink ) + 1 ) );
}

void KPlainTextMarkupBuilder::endParagraph()
{
    m_text.append ( "\n" );
}
void KPlainTextMarkupBuilder::addNewline()
{
    m_text.append ( "\n" );
}
void KPlainTextMarkupBuilder::insertImage ( const QString &src, qreal width, qreal height )
{
    Q_UNUSED ( width )
    Q_UNUSED ( height )

    if ( !m_urls.contains ( src ) ) {
        m_urls.append ( src );
    }
    m_text.append ( QString ( "[%1]" ).arg ( m_urls.indexOf ( src ) + 1 ) );
}


void KPlainTextMarkupBuilder::beginList ( QTextListFormat::Style style )
{
    currentListItemStyles.append ( style );
    currentListItemNumbers.append ( 0 );
}

void KPlainTextMarkupBuilder::endList()
{
    if ( !currentListItemNumbers.isEmpty() ) {
        currentListItemStyles.removeLast();
        currentListItemNumbers.removeLast();
    }
}
void KPlainTextMarkupBuilder::beginListItem()
{
    for ( int i = 0; i < currentListItemNumbers.size(); i++ ) {
        m_text.append ( "    " );
    }

    int itemNumber = currentListItemNumbers.last();
    QString letterString;

    switch ( currentListItemStyles.last() ) {
    case QTextListFormat::ListDisc:
        m_text.append ( " *  " );
        break;
    case QTextListFormat::ListCircle:
        m_text.append ( " o  " );
        break;
    case QTextListFormat::ListSquare:
        m_text.append ( " -  " );
        break;
    case QTextListFormat::ListDecimal:
        m_text.append ( QString ( " %1. " ).arg ( itemNumber + 1 ) );
        break;
    case QTextListFormat::ListLowerAlpha:
        m_text.append ( QString ( " %1. " ).arg ( getLetterString ( itemNumber ) ) );
        break;
    case QTextListFormat::ListUpperAlpha:
        m_text.append ( QString ( " %1. " ).arg ( getLetterString ( itemNumber ).toUpper() ) );
        break;
    default:
        break;
    }
}

QString KPlainTextMarkupBuilder::getLetterString ( int itemNumber )
{
    QString letterString;
    while ( true ) {
        // Create the letter string by prepending one char at a time.
        // The itemNumber is converted to a number in the base 36 (number of letters in the
        // alphabet plus 10) after being increased by 10 (to pass out the digits 0 to 9).
        letterString.prepend ( QString ( "%1" ).arg ( ( itemNumber % LETTERSINALPHABET ) + DIGITSOFFSET,
                               0, // no padding while building this string.
                               LETTERSINALPHABET + DIGITSOFFSET ) );
        if ( ( itemNumber >= LETTERSINALPHABET ) ) {
            itemNumber = itemNumber / LETTERSINALPHABET;
            itemNumber--;
        } else {
            break;
        }
    }
    return letterString;
}


void KPlainTextMarkupBuilder::endListItem()
{
    currentListItemNumbers.last() = currentListItemNumbers.last() + 1;
}


void KPlainTextMarkupBuilder::beginSuperscript()
{
    m_text.append ( "^{" );
}

void KPlainTextMarkupBuilder::endSuperscript()
{
    m_text.append ( "}" );
}

void KPlainTextMarkupBuilder::beginSubscript()
{
    m_text.append ( "_{" );
}

void KPlainTextMarkupBuilder::endSubscript()
{
    m_text.append ( "}" );
}

void KPlainTextMarkupBuilder::appendLiteralText ( const QString &text )
{
    m_text.append ( text );
}

QString& KPlainTextMarkupBuilder::getResult()
{
    QString &ret = m_text;
    ret.append ( getReferences() );
    return ret;
}

QString KPlainTextMarkupBuilder::getReferences()
{
    QString refs;
    if ( !m_urls.isEmpty() ) {
        refs.append ( "\n---- References ----\n" ); //TODO: i18n

        int index = 1;
        while ( !m_urls.isEmpty() ) {
            refs.append ( QString ( "[%1] %2\n" ).arg ( index++ ).arg ( m_urls.takeFirst() ) );
        }
    }
    return refs;
}
