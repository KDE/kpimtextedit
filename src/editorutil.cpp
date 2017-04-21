/*
   Copyright (C) 2013-2017 Laurent Montel <montel@kde.org>

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

#include "editorutil.h"

using namespace KPIMTextEdit;
EditorUtil::EditorUtil()
{
}

void EditorUtil::upperCase(QTextCursor &cursor) const
{
    if (cursor.hasSelection()) {
        const QString newText = cursor.selectedText().toUpper();
        cursor.insertText(newText);
    }
}

void EditorUtil::lowerCase(QTextCursor &cursor) const
{
    if (cursor.hasSelection()) {
        const QString newText = cursor.selectedText().toLower();
        cursor.insertText(newText);
    }
}

void EditorUtil::sentenceCase(QTextCursor &cursor) const
{
    if (cursor.hasSelection()) {
        QString newText = cursor.selectedText();
        const int nbChar(newText.count());
        for (int i = 0; i < nbChar; ++i) {
            if (i == 0 && newText.at(0).isLetter()) {
                newText.replace(0, 1, newText.at(0).toUpper());
            } else if (newText.at(i) == QChar::ParagraphSeparator || newText.at(i) == QChar::LineSeparator) {
                ++i;
                if (i < nbChar) {
                    if (newText.at(i).isLetter()) {
                        newText.replace(i, 1, newText.at(i).toUpper());
                    }
                }
            }
        }
        cursor.insertText(newText);
    }
}

void EditorUtil::reverseCase(QTextCursor &cursor) const
{
    if (cursor.hasSelection()) {
        const QString newText = cursor.selectedText();
        QString reverseCaseText;
        const int nbChar(newText.count());
        for (int i = 0; i < nbChar; ++i) {
            QChar charVal = newText.at(i);
            if (charVal.isLetter()) {
                if (charVal.isLower()) {
                    charVal = charVal.toUpper();
                } else {
                    charVal = charVal.toLower();
                }
            }
            reverseCaseText += charVal;
        }
        cursor.insertText(reverseCaseText);
    }
}
