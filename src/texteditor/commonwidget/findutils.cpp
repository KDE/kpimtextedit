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

#include "findutils.h"
#include "textfindreplacewidget.h"

#include <QLineEdit>
#include <QRegularExpression>
#include <QTextDocument>
#include <QTextCursor>

using namespace KPIMTextEdit;

int FindUtils::replaceAll(QTextDocument *document, const TextFindWidget *findWidget, const TextReplaceWidget *replaceWidget)
{
    QTextCursor c(document);
    c.beginEditBlock();
    int count = 0;
    while (!c.isNull()) {
        // Ignoring FindBackward when replacing all
        QTextDocument::FindFlags flags = findWidget->searchOptions() & ~QTextDocument::FindBackward;
        if (findWidget->isRegularExpression()) {
            c = document->find(findWidget->searchRegExp(), c, flags);
        } else {
            c = document->find(findWidget->searchText(), c, flags);
        }
        if (!c.isNull()) {
            // find() selects found text, and insertText() replaces selection
            c.insertText(replaceWidget->replaceLineEdit()->text());
            count++;
        }
    }
    c.endEditBlock();
    return count;
}
