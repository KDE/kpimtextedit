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

#ifndef KPIMTEXTEDIT_TEXTUTILS_H
#define KPIMTEXTEDIT_TEXTUTILS_H

#include "kpimtextedit_export.h"
#include <qglobal.h>

class QTextDocument;

namespace KPIMTextEdit {

/**
  Utility class for methods working with rich text.

  @since 4.5
*/
namespace TextUtils {

/**
   * Returns whether the QTextDocument @p document contains rich text formatting.
   */
KPIMTEXTEDIT_EXPORT bool containsFormatting( const QTextDocument *document );

/**
   *  Changes the given text so that each line of it fits into the given maximal length.
   *  At each line, the "indent" string is prepended, which is usually the quote prefix.
   *  The text parameter will be empty afterwards.
   *  @param text the text to indent
   *  @param indent the indentation prefix string
   *  @param maxLength the maximal length the string of text will fit into
   *  Example:
   *    text = "Hello World, this is a test."
   *    indent = "> "
   *    maxLength = 16
   *    Result: "> Hello World,\n"
   *            "> this is a test."
   */
KPIMTEXTEDIT_EXPORT QString flowText( QString &text, const QString &indent, int maxLength );

}

}

#endif
