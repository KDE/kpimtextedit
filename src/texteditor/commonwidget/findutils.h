/*
   SPDX-FileCopyrightText: 2020 Igor Poboiko <igor.poboiko@gmail.com>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "kpimtextedit/texteditfindbarbase.h"

#include <QChar>
#include <QStringView>
#include <QTextDocument>
#include <qglobal.h>
class QPlainTextEdit;
class QTextEdit;
namespace KPIMTextEdit
{
class TextFindWidget;
class TextReplaceWidget;

namespace FindUtils
{
/**
 * Replaces all occurrences of a search string provided by @p findWidget
 * by a string provided by @p replaceWidget in a @p document
 *
 * @return number of replacements done
 */
Q_REQUIRED_RESULT int replaceAll(QTextDocument *document, const QString &str, const QString &replaceWidget, QTextDocument::FindFlags searchOptions);
Q_REQUIRED_RESULT int
replaceAll(QTextDocument *document, const QRegularExpression &regExp, const QString &replaceWidget, QTextDocument::FindFlags searchOptions);
Q_REQUIRED_RESULT QString normalize(QStringView str);
Q_REQUIRED_RESULT QChar normalize(QChar c);
Q_REQUIRED_RESULT bool find(QPlainTextEdit *view, const QString &searchText, QTextDocument::FindFlags searchOptions);
Q_REQUIRED_RESULT bool find(QTextEdit *view, const QString &searchText, QTextDocument::FindFlags searchOptions);
Q_REQUIRED_RESULT QTextDocument::FindFlags convertTextEditFindFlags(TextEditFindBarBase::FindFlags textEditFlags);
}
} // namespace KPIMTextEdit

