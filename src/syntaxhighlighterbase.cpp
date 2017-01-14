/*
  Copyright (c) 2014-2017 Montel Laurent <montel@kde.org>

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

#include "syntaxhighlighterbase.h"
#include <QRegularExpressionMatch>
#include "kpimtextedit_debug.h"
using namespace KPIMTextEdit;
SyntaxHighlighterBase::SyntaxHighlighterBase(QTextDocument *doc)
    : QSyntaxHighlighter(doc)
{

}

SyntaxHighlighterBase::~SyntaxHighlighterBase()
{

}

void SyntaxHighlighterBase::highlightBlock(const QString &text)
{
    Q_FOREACH (const Rule &rule, m_rules) {
        const QRegularExpression expression(rule.pattern);
        if (!expression.isValid()) {
            const QString errorString = expression.errorString();
            qCDebug(KPIMTEXTEDIT_LOG) << "expression is invalid, pattern:" << rule.pattern << " error :" << errorString;
        }
        QRegularExpressionMatch match = expression.match(text);

        int index = match.capturedStart();
        while (index >= 0 && match.hasMatch()) {
            setFormat(index, match.capturedLength(), rule.format);
            match = expression.match(text, index + match.capturedLength());
            index = match.capturedStart();
        }
    }
}
