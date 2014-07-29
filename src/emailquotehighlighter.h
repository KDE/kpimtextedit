/**
 * Copyright (C)  2006  Laurent Montel <montel@kde.org>
 * Copyright (C)  2008  Thomas McGuire <mcguire@kde.org>
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA
 * 02110-1301  USA
 */
#ifndef KPIMTEXTEDIT_EMAILQUOTEHIGHLIGHTER_H
#define KPIMTEXTEDIT_EMAILQUOTEHIGHLIGHTER_H

#include "kpimtextedit_export.h"

#include <sonnet/highlighter.h>

#include <memory>

namespace KPIMTextEdit
{

class TextEdit;

/**
 * This highlighter highlights spelling mistakes and also highlightes
 * quotes.
 *
 * Spelling mistakes inside quotes will not be highlighted.
 * The quote highlighting color is configurable.
 *
 * Spell highlighting is disabled by default but can be toggled.
 *
 * @since 4.3
 */
class KPIMTEXTEDIT_EXPORT EMailQuoteHighlighter : public Sonnet::Highlighter
{
public:

    /**
     * Constructor. See setQuoteColor() for the parameters.
     * FIXME: Default colors don't obey color scheme
     */
    explicit EMailQuoteHighlighter(TextEdit *textEdit,
                                   const QColor &normalColor = Qt::black,
                                   const QColor &quoteDepth1 = QColor(0x00, 0x80, 0x00),
                                   const QColor &quoteDepth2 = QColor(0x00, 0x80, 0x00),
                                   const QColor &quoteDepth3 = QColor(0x00, 0x80, 0x00),
                                   const QColor &misspelledColor = Qt::red);

    ~EMailQuoteHighlighter();

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
    virtual void highlightBlock(const QString &text);

    /**
     * Use this static method to get a text consisting of multiple lines
     * highligted.
     * @since 4.4
     */
    static QString highlightText(const QString &text,
                                 const QColor &quoteDepth1 = QColor(0x00, 0x80, 0x00),
                                 const QColor &quoteDepth2 = QColor(0x00, 0x80, 0x00),
                                 const QColor &quoteDepth3 = QColor(0x00, 0x80, 0x00));

    /**
     * Use this static method to get proper highlighting for a single line.
     * @since 4.4
     */
    static QString highlightParagraph(const QString &text,
                                      const QColor &quoteDepth1 = QColor(0x00, 0x80, 0x00),
                                      const QColor &quoteDepth2 = QColor(0x00, 0x80, 0x00),
                                      const QColor &quoteDepth3 = QColor(0x00, 0x80, 0x00));

protected:

    /**
     * Reimplemented, the base version sets the text color to black, which
     * is not what we want. We do nothing, the format is already reset by
     * Qt.
     * @param start the beginning of text
     * @param count the amount of characters to set
     */
    virtual void unsetMisspelled(int start, int count);

    /**
     * Reimplemented to set the color of the misspelled word to a color
     * defined by setQuoteColor().
     */
    virtual void setMisspelled(int start, int count);

private:
    class EMailQuoteHighlighterPrivate;
    std::auto_ptr<EMailQuoteHighlighterPrivate> d;
};

}

#endif
