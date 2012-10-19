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

#include <KLocale>

using namespace KPIMTextEdit;

class KPlainTextMarkupBuilderPrivate
{
public:
    KPlainTextMarkupBuilderPrivate(KPlainTextMarkupBuilder *b) : q_ptr( b ) {
    }

    /**
    Get a letter string to represent a number.

    The numbers 1-26 are represented by a-z, and 27-52 by aa-az, 53-79 by ba-bz etc.

    @param The number to convert
    @return The letter string representation of the number.
    */
    QString getLetterString(int itemNumber);

    QString getRomanString(int itemNumber);

    /**
    Gets a block of references in the body of the text.
    This is an ordered list of links and images in the text.
    */
    QString getReferences();

    QStringList m_urls;
    QList<QTextListFormat::Style> currentListItemStyles;
    QList<int> currentListItemNumbers;

    QString activeLink;

    QString m_text;

    KPlainTextMarkupBuilder *q_ptr;

    Q_DECLARE_PUBLIC( KPlainTextMarkupBuilder )

};

QString KPlainTextMarkupBuilderPrivate::getLetterString(int itemNumber)
{
    QString letterString;
    while ( true ) {
        // Create the letter string by prepending one char at a time.
        // The itemNumber is converted to a number in the base 36 (number of letters in the
        // alphabet plus 10) after being increased by 10 (to pass out the digits 0 to 9).
        letterString.prepend( QString::fromLatin1( "%1" ).arg( ( itemNumber % LETTERSINALPHABET ) + DIGITSOFFSET,
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

QString KPlainTextMarkupBuilderPrivate::getRomanString(int item)
{
    QString result;
    //Code based to gui/text/qtextlist.cpp
    if (item < 5000) {
        QByteArray romanNumeral;

        // works for up to 4999 items
        static const char romanSymbolsLower[] = "iiivixxxlxcccdcmmmm";
        QByteArray romanSymbols; // wrap to have "mid"
        romanSymbols = QByteArray::fromRawData(romanSymbolsLower, sizeof(romanSymbolsLower));
        int c[] = { 1, 4, 5, 9, 10, 40, 50, 90, 100, 400, 500, 900, 1000 };
        int n = item;
        for (int i = 12; i >= 0; n %= c[i], i--) {
            int q = n / c[i];
            if (q > 0) {
                int startDigit = i + (i+3)/4;
                int numDigits;
                if (i % 4) {
                    // c[i] == 4|5|9|40|50|90|400|500|900
                    if ((i-2) % 4) {
                        // c[i] == 4|9|40|90|400|900 => with subtraction (IV, IX, XL, XC, ...)
                        numDigits = 2;
                    }
                    else {
                        // c[i] == 5|50|500 (V, L, D)
                        numDigits = 1;
                    }
                }
                else {
                    // c[i] == 1|10|100|1000 (I, II, III, X, XX, ...)
                    numDigits = q;
                }

                romanNumeral.append(romanSymbols.mid(startDigit, numDigits));
            }
        }
        result = QString::fromLatin1(romanNumeral);
    } else {
        result = QLatin1String("?");
    }
    return result;
}

QString KPlainTextMarkupBuilderPrivate::getReferences()
{
    QString refs;
    if ( !m_urls.isEmpty() ) {
        refs.append( i18nc( "Beginning of the references section, which lists all external references",
                            "\n---- References ----\n" ) );

        int index = 1;
        while ( !m_urls.isEmpty() ) {
            refs.append( QString::fromLatin1( "[%1] %2\n" ).arg( index++ ).arg( m_urls.takeFirst() ) );
        }
    }
    return refs;
}

KPlainTextMarkupBuilder::KPlainTextMarkupBuilder() : d_ptr( new KPlainTextMarkupBuilderPrivate( this ) )
{
    Q_D( KPlainTextMarkupBuilder );
    d->m_urls = QStringList();
}

KPlainTextMarkupBuilder::~KPlainTextMarkupBuilder()
{
    delete d_ptr;
}

void KPlainTextMarkupBuilder::beginStrong()
{
    Q_D( KPlainTextMarkupBuilder );
    d->m_text.append( QLatin1String("*") );
}
void KPlainTextMarkupBuilder::endStrong()
{
    Q_D( KPlainTextMarkupBuilder );
    d->m_text.append( QLatin1String("*") );
}
void KPlainTextMarkupBuilder::beginEmph()
{
    Q_D( KPlainTextMarkupBuilder );
    d->m_text.append( QLatin1String("/") );
}
void KPlainTextMarkupBuilder::endEmph()
{
    Q_D( KPlainTextMarkupBuilder );
    d->m_text.append( QLatin1String("/") );
}
void KPlainTextMarkupBuilder::beginUnderline()
{
    Q_D( KPlainTextMarkupBuilder );
    d->m_text.append( QLatin1String("_") );
}
void KPlainTextMarkupBuilder::endUnderline()
{
    Q_D( KPlainTextMarkupBuilder );
    d->m_text.append( QLatin1String("_") );
}
void KPlainTextMarkupBuilder::beginStrikeout()
{
    Q_D( KPlainTextMarkupBuilder );
    d->m_text.append( QLatin1String("-") );
}
void KPlainTextMarkupBuilder::endStrikeout()
{
    Q_D( KPlainTextMarkupBuilder );
    d->m_text.append( QLatin1String("-") );
}

void KPlainTextMarkupBuilder::beginAnchor(const QString &href, const QString &name)
{
    Q_D( KPlainTextMarkupBuilder );
    Q_UNUSED( name );
    if ( !d->m_urls.contains( href ) ) {
        d->m_urls.append( href );
    }
    d->activeLink = href;
}

void KPlainTextMarkupBuilder::endAnchor()
{
    Q_D( KPlainTextMarkupBuilder );
    d->m_text.append( QString::fromLatin1( "[%1]" ).arg( d->m_urls.indexOf( d->activeLink ) + 1 ) );
}

void KPlainTextMarkupBuilder::endParagraph()
{
    Q_D( KPlainTextMarkupBuilder );
    d->m_text.append( QLatin1Char('\n') );
}

void KPlainTextMarkupBuilder::addNewline()
{
    Q_D( KPlainTextMarkupBuilder );
    d->m_text.append( QLatin1Char('\n') );
}

void KPlainTextMarkupBuilder::insertHorizontalRule(int width)
{
    Q_UNUSED( width )
    Q_D( KPlainTextMarkupBuilder );

    d->m_text.append( QLatin1String("--------------------\n") );
}

void KPlainTextMarkupBuilder::insertImage(const QString &src, qreal width, qreal height)
{
    Q_D( KPlainTextMarkupBuilder );
    Q_UNUSED( width )
    Q_UNUSED( height )

    if ( !d->m_urls.contains( src ) ) {
        d->m_urls.append( src );
    }
    d->m_text.append( QString::fromLatin1( "[%1]" ).arg( d->m_urls.indexOf( src ) + 1 ) );
}


void KPlainTextMarkupBuilder::beginList(QTextListFormat::Style style)
{
    Q_D( KPlainTextMarkupBuilder );
    d->currentListItemStyles.append( style );
    d->currentListItemNumbers.append( 0 );
}

void KPlainTextMarkupBuilder::endList()
{
    Q_D( KPlainTextMarkupBuilder );
    if ( !d->currentListItemNumbers.isEmpty() ) {
        d->currentListItemStyles.removeLast();
        d->currentListItemNumbers.removeLast();
    }
}
void KPlainTextMarkupBuilder::beginListItem()
{
    Q_D( KPlainTextMarkupBuilder );
    for ( int i = 0,count = d->currentListItemNumbers.size(); i < count; i++ ) {
        d->m_text.append( QLatin1String("    ") );
    }

    int itemNumber = d->currentListItemNumbers.last();

    switch ( d->currentListItemStyles.last() ) {
    case QTextListFormat::ListDisc:
        d->m_text.append( QLatin1String(" *  ") );
        break;
    case QTextListFormat::ListCircle:
        d->m_text.append( QLatin1String(" o  ") );
        break;
    case QTextListFormat::ListSquare:
        d->m_text.append( QLatin1String(" -  ") );
        break;
    case QTextListFormat::ListDecimal:
        d->m_text.append( QString::fromLatin1( " %1. " ).arg( itemNumber + 1 ) );
        break;
    case QTextListFormat::ListLowerAlpha:
        d->m_text.append( QString::fromLatin1( " %1. " ).arg( d->getLetterString( itemNumber ) ) );
        break;
    case QTextListFormat::ListUpperAlpha:
        d->m_text.append( QString::fromLatin1( " %1. " ).arg( d->getLetterString( itemNumber ).toUpper() ) );
        break;
    case QTextListFormat::ListLowerRoman:
        d->m_text.append( QString::fromLatin1( " %1. " ).arg( d->getRomanString( itemNumber ) ) );
        break;
    case QTextListFormat::ListUpperRoman:
        d->m_text.append( QString::fromLatin1( " %1. " ).arg( d->getRomanString( itemNumber ).toUpper() ) );
        break;
	
    default:
        break;
    }
}

void KPlainTextMarkupBuilder::endListItem()
{
    Q_D( KPlainTextMarkupBuilder );
    d->currentListItemNumbers.last() = d->currentListItemNumbers.last() + 1;
    d->m_text.append( QLatin1Char('\n') );
}


void KPlainTextMarkupBuilder::beginSuperscript()
{
    Q_D( KPlainTextMarkupBuilder );
    d->m_text.append( QLatin1String("^{") );
}

void KPlainTextMarkupBuilder::endSuperscript()
{
    Q_D( KPlainTextMarkupBuilder );
    d->m_text.append( QLatin1String("}") );
}

void KPlainTextMarkupBuilder::beginSubscript()
{
    Q_D( KPlainTextMarkupBuilder );
    d->m_text.append( QLatin1String("_{") );
}

void KPlainTextMarkupBuilder::endSubscript()
{
    Q_D( KPlainTextMarkupBuilder );
    d->m_text.append( QLatin1String("}") );
}

void KPlainTextMarkupBuilder::appendLiteralText(const QString &text)
{
    Q_D( KPlainTextMarkupBuilder );
    d->m_text.append( text );
}

QString& KPlainTextMarkupBuilder::getResult()
{
    Q_D( KPlainTextMarkupBuilder );
    QString &ret = d->m_text;
    ret.append( d->getReferences() );
    return ret;
}

