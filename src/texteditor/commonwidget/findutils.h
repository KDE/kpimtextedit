/*
   Copyright (C) 2020 Igor Poboiko <igor.poboiko@gmail.com>

   This library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Library General Public
   License as published by the Free Software Foundation; either
   version 2 of the License, or (at your option) any later version.

   This library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Library General Public License for more details.

   You should have received a copy of the GNU Library General Public License
   along with this library; see the file COPYING.LIB.  If not, write to
   the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
   Boston, MA 02110-1301, USA.
*/

#ifndef FINDUTILS_H
#define FINDUTILS_H

#include <qglobal.h>

class QTextDocument;

namespace KPIMTextEdit {
class TextFindWidget;
class TextReplaceWidget;

namespace FindUtils {
/**
     * Replaces all occurences of a search string provided by @p findWidget
     * by a string provided by @p replaceWidget in a @p document
     *
     * @return number of replacements done
     */
Q_REQUIRED_RESULT int replaceAll(QTextDocument *document, const TextFindWidget *findWidget, const TextReplaceWidget *replaceWidget);
}
} // namespace KPIMTextEdit

#endif // FINDUTILS_H
