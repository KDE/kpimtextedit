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

#include "htmlbuilder.h"

#include <QTextDocument>

HTMLBuilder::HTMLBuilder()
{

}

HTMLBuilder::~HTMLBuilder()
{

}

void HTMLBuilder::beginStrong()
{
    m_text.append ( "<strong>" );
}

void HTMLBuilder::endStrong()
{
    m_text.append ( "</strong>" );
}

void HTMLBuilder::beginEmph()
{
    m_text.append ( "<em>" );
}

void HTMLBuilder::endEmph()
{
    m_text.append ( "</em>" );
}

void HTMLBuilder::beginUnderline()
{
    m_text.append ( "<u>" );
}

void HTMLBuilder::endUnderline()
{
    m_text.append ( "</u>" );
}

void HTMLBuilder::beginStrikeout()
{
    m_text.append ( "<s>" );
}

void HTMLBuilder::endStrikeout()
{
    m_text.append ( "</s>" );
}

void HTMLBuilder::beginForeground ( const QBrush &brush )
{
    m_text.append ( QString ( "<span style=\"color:%1;\">" ).arg ( brush.color().name() ) );
}

void HTMLBuilder::endForeground()
{
    m_text.append ( "</span>" );
}

void HTMLBuilder::beginBackground ( const QBrush &brush )
{
    m_text.append ( QString ( "<span style=\"background-color:%1;\">" ).arg ( brush.color().name() ) );
}

void HTMLBuilder::endBackground()
{
    m_text.append ( "</span>" );
}

void HTMLBuilder::endAnchor()
{
    m_text.append ( "</a>" );
}

void HTMLBuilder::beginFontFamily ( const QString &family )
{
    m_text.append ( QString ( "<span style=\"font-family:%1;\">" ).arg ( family ) );
}

void HTMLBuilder::endFontFamily()
{
    m_text.append ( "</span>" );
}

void HTMLBuilder::beginFontPointSize ( int size )
{
    m_text.append ( QString ( "<span style=\"font-size:%1pt;\">" ).arg ( QString::number ( size ) ) );
}

void HTMLBuilder::endFontPointSize()
{
    m_text.append ( "</span>" );
}

void HTMLBuilder::beginParagraph ( Qt::Alignment al, qreal topMargin, qreal bottomMargin, qreal leftMargin, qreal rightMargin )
{
    // Don't put paragraph tags inside li tags. Qt bug reported.
//     if (currentListItemStyles.size() != 0)
//     {
    QString styleString;
    if ( topMargin != 0 ) {
        styleString.append ( QString ( "margin-top:%1;" ).arg ( topMargin ) );
    }
    if ( bottomMargin != 0 ) {
        styleString.append ( QString ( "margin-bottom:%1;" ).arg ( bottomMargin ) );
    }
    if ( leftMargin != 0 ) {
        styleString.append ( QString ( "margin-left:%1;" ).arg ( leftMargin ) );
    }
    if ( rightMargin != 0 ) {
        styleString.append ( QString ( "margin-right:%1;" ).arg ( rightMargin ) );
    }

    // Using == doesn't work here.
    // Using bitwise comparison because an alignment can contain a vertical and a horizontal part.
    if ( al & Qt::AlignRight ) {
        m_text.append ( "<p align=\"right\" " );
    } else if ( al & Qt::AlignHCenter ) {
        m_text.append ( "<p align=\"center\" " );
    } else if ( al & Qt::AlignJustify ) {
        m_text.append ( "<p align=\"justify\" " );
    } else if ( al & Qt::AlignLeft ) {
        m_text.append ( "<p " );
    } else {
        m_text.append ( "<p " );
    }

    if ( !styleString.isEmpty() ) {
        m_text.append ( "\"" + styleString + "\"" );
    }
    m_text.append ( " >" );
//     }
}

void HTMLBuilder::beginHeader1()
{
    m_text.append ( "<h1>" );
}

void HTMLBuilder::beginHeader2()
{
    m_text.append ( "<h2>" );
}

void HTMLBuilder::beginHeader3()
{
    m_text.append ( "<h3>" );
}

void HTMLBuilder::beginHeader4()
{
    m_text.append ( "<h4>" );
}

void HTMLBuilder::beginHeader5()
{
    m_text.append ( "<h5>" );
}

void HTMLBuilder::beginHeader6()
{
    m_text.append ( "<h6>" );
}

void HTMLBuilder::endHeader1()
{
    m_text.append ( "</h1>" );
}

void HTMLBuilder::endHeader2()
{
    m_text.append ( "</h2>" );
}

void HTMLBuilder::endHeader3()
{
    m_text.append ( "</h3>" );
}

void HTMLBuilder::endHeader4()
{
    m_text.append ( "</h4>" );
}

void HTMLBuilder::endHeader5()
{
    m_text.append ( "</h5>" );
}

void HTMLBuilder::endHeader6()
{
    m_text.append ( "</h6>" );
}

void HTMLBuilder::endParagraph()
{
    m_text.append ( "\n</p>\n" );
}

void HTMLBuilder::addNewline()
{
    m_text.append ( "<br />\n" );
}

void HTMLBuilder::insertHorizontalRule ( int width )
{
    if ( width != -1 ) {
        m_text.append ( QString ( "<hr width=\"%1\" />\n" ).arg ( width ) );
    }
    m_text.append ( "<hr />\n" );
}

void HTMLBuilder::insertImage ( const QString &src, qreal width, qreal height )
{
    m_text.append ( QString ( "<img src=\"%1\" width=\"%2\" height=\"%3\" />" ).arg ( src ).arg ( width ).arg ( height ) );
}

void HTMLBuilder::beginList ( QTextListFormat::Style type )
{
    currentListItemStyles.append ( type );
    switch ( type ) {
    case QTextListFormat::ListDisc:
        m_text.append ( "\n<ul type=\"disc\">\n" );
        break;
    case QTextListFormat::ListCircle:
        m_text.append ( "\n<ul type=\"circle\">\n" );
        break;
    case QTextListFormat::ListSquare:
        m_text.append ( "\n<ul type=\"square\">\n" );
        break;
    case QTextListFormat::ListDecimal:
        m_text.append ( "\n<ol type=\"1\">\n" );
        break;
    case QTextListFormat::ListLowerAlpha:
        m_text.append ( "\n<ol type=\"a\">\n" );
        break;
    case QTextListFormat::ListUpperAlpha:
        m_text.append ( "\n<ol type=\"A\">\n" );
        break;
    default:
        break;
    }
}
void HTMLBuilder::endList()
{
    switch ( currentListItemStyles.last() ) {
    case QTextListFormat::ListDisc:
    case QTextListFormat::ListCircle:
    case QTextListFormat::ListSquare:
        m_text.append ( "\n</ul>\n" );
        break;
    case QTextListFormat::ListDecimal:
    case QTextListFormat::ListLowerAlpha:
    case QTextListFormat::ListUpperAlpha:
        m_text.append ( "\n</ol>\n" );
        break;
    default:
        break;
    }
    currentListItemStyles.removeLast();
}
void HTMLBuilder::beginListItem()
{
    m_text.append ( "<li>" );
}

void HTMLBuilder::endListItem()
{
    m_text.append ( "</li>\n" );
}

void HTMLBuilder::beginSuperscript()
{
    m_text.append ( "<sup>" );
}

void HTMLBuilder::endSuperscript()
{
    m_text.append ( "</sup>" );
}

void HTMLBuilder::beginSubscript()
{
    m_text.append ( "<sub>" );
}

void HTMLBuilder::endSubscript()
{
    m_text.append ( "</sub>" );
}


void HTMLBuilder::beginTable ( qreal cellpadding, qreal cellspacing, const QString &width )
{
    m_text.append ( QString ( "<table cellpadding=\"%1\" cellspacing=\"%2\" width=\"%3\" border=\"1\">" )
                    .arg ( cellpadding )
                    .arg ( cellspacing )
                    .arg ( width ) );
}

void HTMLBuilder::beginTableRow()
{
    m_text.append ( "<tr>" );
}

void HTMLBuilder::beginTableHeaderCell ( QString width, int colspan, int rowspan )
{
    m_text.append ( QString ( "<th width=\"%1\" colspan=\"%2\" rowspan=\"%3\">" ).arg ( width ).arg ( colspan ).arg ( rowspan ) );
}

void HTMLBuilder::beginTableCell ( QString width, int colspan, int rowspan )
{
    m_text.append ( QString ( "<td width=\"%1\" colspan=\"%2\" rowspan=\"%3\">" ).arg ( width ).arg ( colspan ).arg ( rowspan ) );
}

void HTMLBuilder::endTable()
{
    m_text.append ( "</table>" );
}

void HTMLBuilder::endTableRow()
{
    m_text.append ( "</tr>" );
}

void HTMLBuilder::endTableHeaderCell()
{
    m_text.append ( "</th>" );
}

void HTMLBuilder::endTableCell()
{
    m_text.append ( "</td>" );
}


void HTMLBuilder::appendLiteralText ( const QString &text )
{
    QString s ( "ert" );
    Qt::mightBeRichText ( s );
    m_text.append ( Qt::escape ( text ) );
}


QString& HTMLBuilder::getResult()
{
    return m_text;
}
