/*
   SPDX-FileCopyrightText: 2013-2021 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
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
