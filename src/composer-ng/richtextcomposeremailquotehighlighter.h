/*
   SPDX-FileCopyrightText: 2015-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "kpimtextedit_export.h"
#include <memory>
#include <sonnet/highlighter.h>
namespace KPIMTextEdit
{
class RichTextComposer;
/*!
 * \class KPIMTextEdit::RichTextComposerEmailQuoteHighlighter
 * \inmodule KPIMTextedit
 * \inheaderfile KPIMTextEdit/RichTextComposerEmailQuoteHighlighter
 *
 * \brief A highlighter for email quoting.
 *
 * \author Laurent Montel <montel@kde.org>
 */
class RichTextComposer;
class KPIMTEXTEDIT_EXPORT RichTextComposerEmailQuoteHighlighter : public Sonnet::Highlighter
{
    Q_OBJECT
public:
    /*!
     * Constructor with color customization. See setQuoteColor() for the parameters.
     * Creates a highlighter with the given \a textEdit and optional custom colors.
     * FIXME: Default colors don't obey color scheme
     */
    explicit RichTextComposerEmailQuoteHighlighter(RichTextComposer *textEdit,
                                                   const QColor &normalColor = Qt::black,
                                                   const QColor &quoteDepth1 = QColor(0x00, 0x80, 0x00),
                                                   const QColor &quoteDepth2 = QColor(0x00, 0x80, 0x00),
                                                   const QColor &quoteDepth3 = QColor(0x00, 0x80, 0x00),
                                                   const QColor &misspelledColor = Qt::red);

    /*!
     * Destroys the RichTextComposerEmailQuoteHighlighter.
     */
    ~RichTextComposerEmailQuoteHighlighter() override;

    /*!
     * Sets the colors used for highlighting quoted text and spelling mistakes.
     *
     * \a normalColor the color for normal text
     * \a quoteDepth1 color for text quoted 1 level deep
     * \a quoteDepth2 color for text quoted 2 level deep
     * \a quoteDepth3 color for text quoted 3 level deep
     * \a misspelledColor color in which misspelled words will be underlined
     */
    void setQuoteColor(const QColor &normalColor,
                       const QColor &quoteDepth1,
                       const QColor &quoteDepth2,
                       const QColor &quoteDepth3,
                       const QColor &misspelledColor = Qt::red);

    /*!
     * Turns spellcheck highlighting on or off based on the \a on parameter.
     *
     * \a on if true, spelling mistakes will be highlighted
     */
    void toggleSpellHighlighting(bool on);

    /*!
     * Highlights a single text block with quote and spelling highlighting.
     * Reimplemented from Sonnet::Highlighter to highlight quote blocks.
     */
    void highlightBlock(const QString &text) override;

protected:
    /*!
     * Removes misspelled text formatting from the specified text range.
     * Reimplemented, the base version sets the text color to black, which
     * is not what we want. We do nothing, the format is already reset by Qt.
     * \a start the beginning of text
     * \a count the amount of characters to set
     */
    void unsetMisspelled(int start, int count) override;

    /*!
     * Reimplemented to set the color of the misspelled word to a color
     * defined by setQuoteColor().
     */
    void setMisspelled(int start, int count) override;

private:
    class RichTextComposerEmailQuoteHighlighterPrivate;
    std::unique_ptr<RichTextComposerEmailQuoteHighlighterPrivate> const d;
};
}
