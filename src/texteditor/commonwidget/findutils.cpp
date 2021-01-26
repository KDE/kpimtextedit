/*
   SPDX-FileCopyrightText: 2020 Igor Poboiko <igor.poboiko@gmail.com>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "findutils.h"
#include "textfindreplacewidget.h"

#include <QLineEdit>
#include <QRegularExpression>
#include <QTextCursor>
#include <QTextDocument>

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
