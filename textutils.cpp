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

#include <QTextDocument>
#include <QTextCharFormat>
#include <QTextBlock>

using namespace KPIMTextEdit;

static bool isCharFormatFormatted( const QTextCharFormat &format, const QFont &defaultFont,
                                   const QTextCharFormat &defaultBlockFormat )
{
  if ( !format.anchorHref().isEmpty() ||
       format.font() != defaultFont ||
       format.isAnchor() ||
       format.verticalAlignment() != defaultBlockFormat.verticalAlignment() ||
       format.underlineStyle() != defaultBlockFormat.underlineStyle() ||
       format.foreground().color() != defaultBlockFormat.foreground().color() ||
       format.background().color() != defaultBlockFormat.background().color() )
    return true;

  return false;
}

static bool isBlockFormatFormatted( const QTextBlockFormat &format,
                                    const QTextBlockFormat &defaultFormat )
{
  if ( format.alignment() != defaultFormat.alignment() ||
       format.indent() != defaultFormat.indent() ||
       format.textIndent() != defaultFormat.textIndent() )
    return true;

  return false;
}

/// @return true if the format represents a list, table, image or something like that.
static bool isSpecial( const QTextFormat &charFormat )
{
  return charFormat.isFrameFormat() || charFormat.isImageFormat() ||
         charFormat.isListFormat() || charFormat.isTableFormat();
}

bool TextUtils::isFormattingUsed( QTextDocument *document )
{
  QTextDocument defaultTextDocument;
  QTextCharFormat defaultCharFormat = defaultTextDocument.begin().charFormat();
  QTextBlockFormat defaultBlockFormat = defaultTextDocument.begin().blockFormat();
  QFont defaultFont = defaultTextDocument.defaultFont();

  QTextBlock block = document->firstBlock();
  while ( block.isValid() ) {

    if ( isBlockFormatFormatted( block.blockFormat(), defaultBlockFormat ) ) {
      return true;
    }

    if ( isSpecial( block.charFormat() ) || isSpecial( block.blockFormat() ) ||
         block.textList() ) {
      return true;
    }

    QTextBlock::iterator it = block.begin();
    while ( !it.atEnd() ) {
      QTextFragment fragment = it.fragment();
      QTextCharFormat charFormat = fragment.charFormat();
      if ( isSpecial( charFormat ) ) {
        return true;
      }
      if ( isCharFormatFormatted( fragment.charFormat(), defaultFont, defaultCharFormat ) ) {
        return true;
      }

      it++;
    }
    block = block.next();
  }

  if ( document->toHtml().contains( QLatin1String( "<hr />" ) ) )
    return true;

  return false;
}
