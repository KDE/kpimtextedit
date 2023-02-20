/*
   SPDX-FileCopyrightText: 2015-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "kpimtextedit_export.h"

#include <KSyntaxHighlighting/SyntaxHighlighter>
#include <Sonnet/Highlighter>

namespace KSyntaxHighlighting
{
class Format;
}

namespace KPIMTextEdit
{
class PlainTextEditor;
class PlainTextSyntaxSpellCheckingHighlighterPrivate;
/**
 * @brief The PlainTextSyntaxSpellCheckingHighlighter class
 * @author Laurent Montel <montel@kde.org>
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
    std::unique_ptr<PlainTextSyntaxSpellCheckingHighlighterPrivate> const d;
};
}
