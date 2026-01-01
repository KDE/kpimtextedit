/*
   SPDX-FileCopyrightText: 2015-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "richtextcomposeremailquotehighlighter.h"
using namespace Qt::Literals::StringLiterals;

#include "richtextcomposer.h"
#include <QRegularExpression>
using namespace KPIMTextEdit;

class Q_DECL_HIDDEN KPIMTextEdit::RichTextComposerEmailQuoteHighlighter::RichTextComposerEmailQuoteHighlighterPrivate
{
public:
    RichTextComposer *parent = nullptr;
    QColor col1;
    QColor col2;
    QColor col3;
    QColor misspelledColor;
    bool spellCheckingEnabled = false;
};

RichTextComposerEmailQuoteHighlighter::RichTextComposerEmailQuoteHighlighter(RichTextComposer *textEdit,
                                                                             const QColor &normalColor,
                                                                             const QColor &quoteDepth1,
                                                                             const QColor &quoteDepth2,
                                                                             const QColor &quoteDepth3,
                                                                             const QColor &misspelledColor)
    : Sonnet::Highlighter(textEdit)
    , d(new KPIMTextEdit::RichTextComposerEmailQuoteHighlighter::RichTextComposerEmailQuoteHighlighterPrivate())
{
    Q_UNUSED(normalColor)
    // Don't automatically disable the spell checker, for example because there
    // are too many misspelled words. That would also disable quote highlighting.
    // FIXME: disable this spell checking!
    setAutomatic(false);

    setActive(true);
    d->col1 = quoteDepth1;
    d->col2 = quoteDepth2;
    d->col3 = quoteDepth3;
    d->misspelledColor = misspelledColor;
    d->spellCheckingEnabled = false;
    d->parent = textEdit;
}

RichTextComposerEmailQuoteHighlighter::~RichTextComposerEmailQuoteHighlighter() = default;

void RichTextComposerEmailQuoteHighlighter::setQuoteColor(const QColor &normalColor,
                                                          const QColor &quoteDepth1,
                                                          const QColor &quoteDepth2,
                                                          const QColor &quoteDepth3,
                                                          const QColor &misspelledColor)
{
    Q_UNUSED(normalColor)
    d->col1 = quoteDepth1;
    d->col2 = quoteDepth2;
    d->col3 = quoteDepth3;
    d->misspelledColor = misspelledColor;
}

void RichTextComposerEmailQuoteHighlighter::toggleSpellHighlighting(bool on)
{
    if (on != d->spellCheckingEnabled) {
        d->spellCheckingEnabled = on;
        rehighlight();
    }
}

void RichTextComposerEmailQuoteHighlighter::highlightBlock(const QString &text)
{
    QString simplified = text;
    simplified.remove(QRegularExpression(u"\\s"_s)).replace(u'|', u'>');

    while (simplified.startsWith(">>>>"_L1)) {
        simplified.remove(0, 3);
    }

    if (simplified.startsWith(">>>"_L1)) {
        setFormat(0, text.length(), d->col3);
    } else if (simplified.startsWith(">>"_L1)) {
        setFormat(0, text.length(), d->col2);
    } else if (simplified.startsWith(">"_L1)) {
        setFormat(0, text.length(), d->col1);
    } else if (d->parent->isLineQuoted(text)) {
        setFormat(0, text.length(), d->col1); // FIXME: custom quote prefix
        // can't handle multiple levels
    } else if (d->spellCheckingEnabled) {
        Highlighter::highlightBlock(text);
        return; // setCurrentBlockState already done in Highlighter::highlightBlock
    }
    setCurrentBlockState(0);
}

void RichTextComposerEmailQuoteHighlighter::unsetMisspelled(int start, int count)
{
    Q_UNUSED(start)
    Q_UNUSED(count)
}

void RichTextComposerEmailQuoteHighlighter::setMisspelled(int start, int count)
{
    setMisspelledColor(d->misspelledColor);
    Sonnet::Highlighter::setMisspelled(start, count);
}

#include "moc_richtextcomposeremailquotehighlighter.cpp"
