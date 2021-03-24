/*
   SPDX-FileCopyrightText: 2015-2021 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "kpimtextedit_export.h"
#include <sonnet/highlighter.h>
namespace KPIMTextEdit
{
class RichTextComposer;
/**
 * @short A highlighter for email quoting.
 *
 * @author Laurent Montel <montel@kde.org>
 */
class RichTextComposer;
class KPIMTEXTEDIT_EXPORT RichTextComposerEmailQuoteHighlighter : public Sonnet::Highlighter
{
    Q_OBJECT
public:
    /**
     * Constructor. See setQuoteColor() for the parameters.
     * FIXME: Default colors don't obey color scheme
     */
    explicit RichTextComposerEmailQuoteHighlighter(RichTextComposer *textEdit,
                                                   const QColor &normalColor = Qt::black,
                                                   const QColor &quoteDepth1 = QColor(0x00, 0x80, 0x00),
                                                   const QColor &quoteDepth2 = QColor(0x00, 0x80, 0x00),
                                                   const QColor &quoteDepth3 = QColor(0x00, 0x80, 0x00),
                                                   const QColor &misspelledColor = Qt::red);

    ~RichTextComposerEmailQuoteHighlighter() override;

    /**
     * Sets the colors used for highlighting quoted text and spelling mistakes.
     *
     * @param quoteDepth1 color for text quoted 1 level deep
     * @param quoteDepth2 color for text quoted 2 level deep
     * @param quoteDepth3 color for text quoted 3 level deep
     * @param misspelledColor color in which misspelled words will be underlined
     * @param normalColor will be ignored, only provided for KNode
     *                    compatibility.
     */
    void setQuoteColor(const QColor &normalColor,
                       const QColor &quoteDepth1,
                       const QColor &quoteDepth2,
                       const QColor &quoteDepth3,
                       const QColor &misspelledColor = Qt::red);

    /**
     * Turns spellcheck highlighting on or off.
     *
     * @param on if true, spelling mistakes will be highlighted
     */
    void toggleSpellHighlighting(bool on);

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

private:
    class RichTextComposerEmailQuoteHighlighterPrivate;
    RichTextComposerEmailQuoteHighlighterPrivate *const d;
};
}

