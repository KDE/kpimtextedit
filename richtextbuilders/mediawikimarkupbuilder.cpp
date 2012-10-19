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

#include "mediawikimarkupbuilder.h"
#include <kdebug.h>

using namespace KPIMTextEdit;

MediaWikiMarkupBuilder::MediaWikiMarkupBuilder() {}
MediaWikiMarkupBuilder::~MediaWikiMarkupBuilder() {}

void MediaWikiMarkupBuilder::beginStrong()
{
    m_text.append( QLatin1String("'''") );
}
void MediaWikiMarkupBuilder::endStrong()
{
    m_text.append( QLatin1String("'''") );
}
void MediaWikiMarkupBuilder::beginEmph()
{
    m_text.append( QLatin1String("''") );
}
void MediaWikiMarkupBuilder::endEmph()
{
    m_text.append( QLatin1String("''") );
}
void MediaWikiMarkupBuilder::beginUnderline()
{
    m_text.append( QLatin1String("<u>") );
}
void MediaWikiMarkupBuilder::endUnderline()
{
    m_text.append( QLatin1String("</u>") );
}
void MediaWikiMarkupBuilder::beginStrikeout()
{
    m_text.append( QLatin1String("<s>") );
}
void MediaWikiMarkupBuilder::endStrikeout()
{
    m_text.append( QLatin1String("</s>") );
}

void MediaWikiMarkupBuilder::endParagraph()
{
    m_text.append( QLatin1String("\n") );
}
void MediaWikiMarkupBuilder::addNewline()
{
    m_text.append( QLatin1String("\n") );
}

void MediaWikiMarkupBuilder::beginAnchor(const QString &href, const QString &name)
{
    Q_UNUSED( name );
    m_text.append( QString::fromLatin1( "[%1 " ).arg( href ) );
}
void MediaWikiMarkupBuilder::endAnchor()
{
    m_text.append( QLatin1String("]") );
}

void MediaWikiMarkupBuilder::beginHeader(int level)
{
    switch ( level ) {
    case 1:
        m_text.append(QLatin1String( "= ") );
        break;
    case 2:
        m_text.append( QLatin1String("== ") );
        break;
    case 3:
        m_text.append( QLatin1String("=== ") );
        break;
    case 4:
        m_text.append( QLatin1String("==== ") );
        break;
    case 5:
        m_text.append( QLatin1String("===== ") );
        break;
    case 6:
        m_text.append( QLatin1String("====== ") );
        break;
    default:
        break;
    }
}

void MediaWikiMarkupBuilder::endHeader(int level)
{
    switch ( level ) {
    case 1:
        m_text.append( QLatin1String(" =\n") );
        break;
    case 2:
        m_text.append( QLatin1String(" ==\n") );
        break;
    case 3:
        m_text.append( QLatin1String(" ===\n") );
        break;
    case 4:
        m_text.append( QLatin1String(" ====\n") );
        break;
    case 5:
        m_text.append( QLatin1String(" =====\n") );
        break;
    case 6:
        m_text.append( QLatin1String(" ======\n") );
        break;
    default:
        break;
    }
}

void MediaWikiMarkupBuilder::beginList(QTextListFormat::Style type)
{
    currentListItemStyles.append( type );
    switch ( type ) {
    case QTextListFormat::ListDisc:
    case QTextListFormat::ListCircle:
    case QTextListFormat::ListSquare:
    case QTextListFormat::ListDecimal:
    case QTextListFormat::ListLowerAlpha:
    case QTextListFormat::ListUpperAlpha:
        m_text.append( QLatin1String("\n") );
        break;
    default:
        break;
    }
}

void MediaWikiMarkupBuilder::endList()
{
    m_text.append( QLatin1String("\n") );
    currentListItemStyles.removeLast();
}

void MediaWikiMarkupBuilder::beginListItem()
{
    switch ( currentListItemStyles.last() ) {
    case QTextListFormat::ListDisc:
    case QTextListFormat::ListCircle:
    case QTextListFormat::ListSquare:
        m_text.append( QLatin1String("* ") );    // Unordered lists are all disc type in MediaWikiMarkup.
        break;
    case QTextListFormat::ListDecimal:
    case QTextListFormat::ListLowerAlpha:
    case QTextListFormat::ListUpperAlpha:
    case QTextListFormat::ListLowerRoman:
    case QTextListFormat::ListUpperRoman:
        m_text.append( QLatin1String("# ") );
        break;
    default:
        break;
    }
}

void MediaWikiMarkupBuilder::endListItem()
{
    m_text.append( QLatin1String("\n") );
}

void MediaWikiMarkupBuilder::appendLiteralText(const QString &text)
{
    m_text.append( escape( text ) );
}

const QString MediaWikiMarkupBuilder::escape(const QString &s)
{
    if ( s.contains( QLatin1String("<") ) ) {        // TODO: This could contain more. "''" and "[" for example
        return QString( QLatin1String("<nowiki>") + s + QLatin1String("</nowiki>") );
    }
    return s;
}

QString& MediaWikiMarkupBuilder::getResult()
{
    return m_text;
}
