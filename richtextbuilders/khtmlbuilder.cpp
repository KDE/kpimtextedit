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

KHTMLBuilder::KHTMLBuilder()
{

}

KHTMLBuilder::~KHTMLBuilder()
{

}

void KHTMLBuilder::beginStrong()
{
    m_text.append ( "<strong>" );
}

void KHTMLBuilder::endStrong()
{
    m_text.append ( "</strong>" );
}

void KHTMLBuilder::beginEmph()
{
    m_text.append ( "<em>" );
}

void KHTMLBuilder::endEmph()
{
    m_text.append ( "</em>" );
}

void KHTMLBuilder::beginUnderline()
{
    m_text.append ( "<u>" );
}

void KHTMLBuilder::endUnderline()
{
    m_text.append ( "</u>" );
}

void KHTMLBuilder::beginStrikeout()
{
    m_text.append ( "<s>" );
}

void KHTMLBuilder::endStrikeout()
{
    m_text.append ( "</s>" );
}

void KHTMLBuilder::beginForeground ( const QBrush &brush )
{
    m_text.append ( QString ( "<span style=\"color:%1;\">" ).arg ( brush.color().name() ) );
}

void KHTMLBuilder::endForeground()
{
    m_text.append ( "</span>" );
}

void KHTMLBuilder::beginBackground ( const QBrush &brush )
{
    m_text.append ( QString ( "<span style=\"background-color:%1;\">" ).arg ( brush.color().name() ) );
}

void KHTMLBuilder::endBackground()
{
    m_text.append ( "</span>" );
}

void KHTMLBuilder::endAnchor()
{
    m_text.append ( "</a>" );
}

void KHTMLBuilder::beginFontFamily ( const QString &family )
{
    m_text.append ( QString ( "<span style=\"font-family:%1;\">" ).arg ( family ) );
}

void KHTMLBuilder::endFontFamily()
{
    m_text.append ( "</span>" );
}

void KHTMLBuilder::beginFontPointSize ( int size )
{
    m_text.append ( QString ( "<span style=\"font-size:%1pt;\">" ).arg ( QString::number ( size ) ) );
}

void KHTMLBuilder::endFontPointSize()
{
    m_text.append ( "</span>" );
}

void KHTMLBuilder::beginParagraph ( Qt::Alignment al, qreal topMargin, qreal bottomMargin, qreal leftMargin, qreal rightMargin )
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
        m_text.append ( "<p" );
    } else {
        m_text.append ( "<p" );
    }

    if ( !styleString.isEmpty() ) {
        m_text.append ( " \"" + styleString + "\"" );
    }
    m_text.append ( ">" );
//     }
}

void KHTMLBuilder::beginHeader1()
{
    m_text.append ( "<h1>" );
}

void KHTMLBuilder::beginHeader2()
{
    m_text.append ( "<h2>" );
}

void KHTMLBuilder::beginHeader3()
{
    m_text.append ( "<h3>" );
}

void KHTMLBuilder::beginHeader4()
{
    m_text.append ( "<h4>" );
}

void KHTMLBuilder::beginHeader5()
{
    m_text.append ( "<h5>" );
}

void KHTMLBuilder::beginHeader6()
{
    m_text.append ( "<h6>" );
}

void KHTMLBuilder::endHeader1()
{
    m_text.append ( "</h1>" );
}

void KHTMLBuilder::endHeader2()
{
    m_text.append ( "</h2>" );
}

void KHTMLBuilder::endHeader3()
{
    m_text.append ( "</h3>" );
}

void KHTMLBuilder::endHeader4()
{
    m_text.append ( "</h4>" );
}

void KHTMLBuilder::endHeader5()
{
    m_text.append ( "</h5>" );
}

void KHTMLBuilder::endHeader6()
{
    m_text.append ( "</h6>" );
}

void KHTMLBuilder::endParagraph()
{
    m_text.append ( "</p>\n" );
}

void KHTMLBuilder::addNewline()
{
    m_text.append ( "<br />\n" );

  // Work around qt import bug. Doesn't import <br /> correctly.
//     m_text.append ( "<p style=\"-qt-paragraph-type:empty;\"></p><br />\n" );
}

void KHTMLBuilder::insertHorizontalRule ( int width )
{
    if ( width != -1 ) {
        m_text.append ( QString ( "<hr width=\"%1\" />\n" ).arg ( width ) );
    }
    m_text.append ( "<hr />\n" );
}

void KHTMLBuilder::insertImage ( const QString &src, qreal width, qreal height )
{
  m_text.append( QString( "<img src=\"%1\" " ).arg ( src ) );
  if ( width != 0 ) m_text.append( QString( "width=\"%2\" " ).arg ( width ) );
  if ( height != 0 ) m_text.append( QString( "height=\"%2\" " ).arg ( height ) );
  m_text.append( "/>" );
}

void KHTMLBuilder::beginList ( QTextListFormat::Style type )
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
void KHTMLBuilder::endList()
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
void KHTMLBuilder::beginListItem()
{
    m_text.append ( "<li>" );
}

void KHTMLBuilder::endListItem()
{
    m_text.append ( "</li>\n" );
}

void KHTMLBuilder::beginSuperscript()
{
    m_text.append ( "<sup>" );
}

void KHTMLBuilder::endSuperscript()
{
    m_text.append ( "</sup>" );
}

void KHTMLBuilder::beginSubscript()
{
    m_text.append ( "<sub>" );
}

void KHTMLBuilder::endSubscript()
{
    m_text.append ( "</sub>" );
}


void KHTMLBuilder::beginTable ( qreal cellpadding, qreal cellspacing, const QString &width )
{
    m_text.append ( QString ( "<table cellpadding=\"%1\" cellspacing=\"%2\" width=\"%3\" border=\"1\">" )
                    .arg ( cellpadding )
                    .arg ( cellspacing )
                    .arg ( width ) );
}

void KHTMLBuilder::beginTableRow()
{
    m_text.append ( "<tr>" );
}

void KHTMLBuilder::beginTableHeaderCell ( QString width, int colspan, int rowspan )
{
    m_text.append ( QString ( "<th width=\"%1\" colspan=\"%2\" rowspan=\"%3\">" ).arg ( width ).arg ( colspan ).arg ( rowspan ) );
}

void KHTMLBuilder::beginTableCell ( QString width, int colspan, int rowspan )
{
    m_text.append ( QString ( "<td width=\"%1\" colspan=\"%2\" rowspan=\"%3\">" ).arg ( width ).arg ( colspan ).arg ( rowspan ) );
}

void KHTMLBuilder::endTable()
{
    m_text.append ( "</table>" );
}

void KHTMLBuilder::endTableRow()
{
    m_text.append ( "</tr>" );
}

void KHTMLBuilder::endTableHeaderCell()
{
    m_text.append ( "</th>" );
}

void KHTMLBuilder::endTableCell()
{
    m_text.append ( "</td>" );
}


void KHTMLBuilder::appendLiteralText ( const QString &text )
{
    QString s ( "ert" );
    Qt::mightBeRichText ( s );
    m_text.append ( Qt::escape ( text ) );
}


QString& KHTMLBuilder::getResult()
{
    return m_text;
}
