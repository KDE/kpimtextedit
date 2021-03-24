/*
   SPDX-FileCopyrightText: 2020 Igor Poboiko <igor.poboiko@gmail.com>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include <qglobal.h>

class QTextDocument;

namespace KPIMTextEdit
{
class TextFindWidget;
class TextReplaceWidget;

namespace FindUtils
{
/**
 * Replaces all occurences of a search string provided by @p findWidget
 * by a string provided by @p replaceWidget in a @p document
 *
 * @return number of replacements done
 */
Q_REQUIRED_RESULT int replaceAll(QTextDocument *document, const TextFindWidget *findWidget, const TextReplaceWidget *replaceWidget);
}
} // namespace KPIMTextEdit

