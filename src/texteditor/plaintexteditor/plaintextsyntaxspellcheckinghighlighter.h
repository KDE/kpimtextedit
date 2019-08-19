/*
   Copyright (C) 2015-2019 Laurent Montel <montel@kde.org>

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

#ifndef PLAINTEXTSYNTAXSPELLCHECKINGHIGHLIGHTER_H
#define PLAINTEXTSYNTAXSPELLCHECKINGHIGHLIGHTER_H

#include "kpimtextedit_export.h"

#include <Sonnet/Highlighter>
#include <KSyntaxHighlighting/SyntaxHighlighter>

namespace KSyntaxHighlighting {
class Format;
}

namespace KPIMTextEdit {
class PlainTextEditor;
class PlainTextSyntaxSpellCheckingHighlighterPrivate;
/**
 * @brief The PlainTextSyntaxSpellCheckingHighlighter class
 */
class KPIMTEXTEDIT_EXPORT PlainTextSyntaxSpellCheckingHighlighter : public Sonnet::Highlighter, public KSyntaxHighlighting::AbstractHighlighter
{
public:
    explicit PlainTextSyntaxSpellCheckingHighlighter(PlainTextEditor *plainText, const QColor &misspelledColor = Qt::red);
    ~PlainTextSyntaxSpellCheckingHighlighter() override;

    void toggleSpellHighlighting(bool on);

    void setDefinition(const KSyntaxHighlighting::Definition &def) override;

    /**
     * Reimplemented to highlight quote blocks.
     */
    void highlightBlock(const QString &text) override;

protected:
    /**
     * Reimplemented, the base version sets the text color to black, which
     * is not what we want. We do nothing, the format is already reset by
     * Qt.
     * @param start the beginning of text
     * @param count the amount of characters to set
     */
    void unsetMisspelled(int start, int count) override;

    /**
     * Reimplemented to set the color of the misspelled word to a color
     * defined by setQuoteColor().
     */
    void setMisspelled(int start, int count) override;

    void applyFormat(int offset, int length, const KSyntaxHighlighting::Format &format) override;
private:
    PlainTextSyntaxSpellCheckingHighlighterPrivate *const d;
};
}
#endif // PLAINTEXTSYNTAXSPELLCHECKINGHIGHLIGHTER_H
