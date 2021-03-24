/*
    This file is part of KDE.

    SPDX-FileCopyrightText: 2009 Thomas McGuire <mcguire@kde.org>
    SPDX-FileCopyrightText: 2010 Stephen Kelly <steveire@gmail.com>

    SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "kpimtextedit_export.h"
#include <qglobal.h>
class QTextDocument;

namespace KPIMTextEdit
{
/**
  Utility class for methods working with rich text.

  @since 4.5
*/
namespace TextUtils
{
/**
 * Returns whether the QTextDocument @p document contains rich text formatting.
 */
KPIMTEXTEDIT_EXPORT Q_REQUIRED_RESULT bool containsFormatting(const QTextDocument *document);

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
KPIMTEXTEDIT_EXPORT Q_REQUIRED_RESULT QString flowText(QString &text, const QString &indent, int maxLength);
}
}

