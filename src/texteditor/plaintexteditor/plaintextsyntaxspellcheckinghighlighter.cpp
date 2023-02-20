/*
   SPDX-FileCopyrightText: 2015-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "plaintextsyntaxspellcheckinghighlighter.h"
#include "kpimtextedit_debug.h"
#include "plaintexteditor.h"

#include <KSyntaxHighlighting/Definition>
#include <KSyntaxHighlighting/Format>
#include <KSyntaxHighlighting/State>
#include <KSyntaxHighlighting/Theme>

#include <vector>

Q_DECLARE_METATYPE(QTextBlock)

using namespace KPIMTextEdit;

namespace KPIMTextEdit
{
struct SpellCheckRange {
    SpellCheckRange(int o, int l)
        : offset(o)
        , length(l)
    {
    }

    Q_REQUIRED_RESULT int end() const
    {
        return offset + length;
    }

    int offset;
    int length;
};

QDebug operator<<(QDebug dbg, SpellCheckRange s)
{
    dbg << '(' << s.offset << ',' << s.length << ')';
    return dbg;
}

class TextBlockUserData : public QTextBlockUserData
{
public:
    KSyntaxHighlighting::State state;
    QTextBlockUserData *spellData = nullptr;
};

class PlainTextSyntaxSpellCheckingHighlighterPrivate
{
public:
    explicit PlainTextSyntaxSpellCheckingHighlighterPrivate(PlainTextEditor *plainText)
        : editor(plainText)
    {
    }

    PlainTextEditor *const editor;
    QColor misspelledColor;
    bool spellCheckingEnabled = false;

    // We can't use QTextBlock user data, Sonnet is occupying that already
    // and we can't just daisy-chain them, as QTextBlock deletes the previous
    // one when setting a new one. Instead, we need to store this out-of-band.
    QHash<int, KSyntaxHighlighting::State> blockState;

    std::vector<SpellCheckRange> spellCheckRanges;
};
}

PlainTextSyntaxSpellCheckingHighlighter::PlainTextSyntaxSpellCheckingHighlighter(PlainTextEditor *plainText, const QColor &misspelledColor)
    : Sonnet::Highlighter(plainText)
    , d(new PlainTextSyntaxSpellCheckingHighlighterPrivate(plainText))
{
    qRegisterMetaType<QTextBlock>();
    d->misspelledColor = misspelledColor;
    setAutomatic(false);
}

PlainTextSyntaxSpellCheckingHighlighter::~PlainTextSyntaxSpellCheckingHighlighter() = default;

void PlainTextSyntaxSpellCheckingHighlighter::toggleSpellHighlighting(bool on)
{
    if (on != d->spellCheckingEnabled) {
        d->spellCheckingEnabled = on;
        rehighlight();
    }
}

void PlainTextSyntaxSpellCheckingHighlighter::setDefinition(const KSyntaxHighlighting::Definition &def)
{
    const auto needsRehighlight = definition() != def;
    AbstractHighlighter::setDefinition(def);
    if (needsRehighlight) {
        rehighlight();
    }
}

void PlainTextSyntaxSpellCheckingHighlighter::highlightBlock(const QString &text)
{
    d->spellCheckRanges.clear();

    KSyntaxHighlighting::State state;
    if (currentBlock().position() > 0) {
        const auto prevBlock = currentBlock().previous();
        state = d->blockState.value(prevBlock.userState());
    }

    state = highlightLine(text, state);
    if (d->spellCheckingEnabled && d->editor->isEnabled() && !d->spellCheckRanges.empty()) {
        Highlighter::highlightBlock(text);
    }

    if (currentBlockState() <= 0) { // first time we highlight this
        setCurrentBlockState(d->blockState.size() + 1);
        d->blockState.insert(currentBlockState(), state);
        return;
    }

    if (d->blockState.value(currentBlockState()) == state) {
        return;
    }
    d->blockState.insert(currentBlockState(), state);

    const auto nextBlock = currentBlock().next();
    if (nextBlock.isValid()) {
        QMetaObject::invokeMethod(
            this,
            [this, nextBlock] {
                rehighlightBlock(nextBlock);
            },
            Qt::QueuedConnection);
    }
}

void PlainTextSyntaxSpellCheckingHighlighter::unsetMisspelled(int start, int count)
{
    Q_UNUSED(start)
    Q_UNUSED(count)
}

void PlainTextSyntaxSpellCheckingHighlighter::setMisspelled(int start, int count)
{
    setMisspelledColor(d->misspelledColor);
    for (const auto &range : d->spellCheckRanges) {
        if (range.offset <= start && range.end() >= start + count) {
            auto f = format(start);
            f.setFontUnderline(true);
            f.setUnderlineStyle(QTextCharFormat::SpellCheckUnderline);
            f.setUnderlineColor(d->misspelledColor);
            setFormat(start, count, f);
            return;
        }
    }
}

void PlainTextSyntaxSpellCheckingHighlighter::applyFormat(int offset, int length, const KSyntaxHighlighting::Format &format)
{
    if (format.spellCheck() && length > 0) {
        if (d->spellCheckRanges.empty()) {
            d->spellCheckRanges.emplace_back(offset, length);
        } else if (d->spellCheckRanges.back().end() + 1 == offset) {
            d->spellCheckRanges.back().length += length;
        } else {
            d->spellCheckRanges.emplace_back(offset, length);
        }
    }

    if (format.isDefaultTextStyle(theme()) || length == 0) {
        return;
    }

    QTextCharFormat tf;
    if (format.hasTextColor(theme())) {
        tf.setForeground(format.textColor(theme()));
    }
    if (format.hasBackgroundColor(theme())) {
        tf.setBackground(format.backgroundColor(theme()));
    }

    if (format.isBold(theme())) {
        tf.setFontWeight(QFont::Bold);
    }
    if (format.isItalic(theme())) {
        tf.setFontItalic(true);
    }
    if (format.isUnderline(theme())) {
        tf.setFontUnderline(true);
    }
    if (format.isStrikeThrough(theme())) {
        tf.setFontStrikeOut(true);
    }

    QSyntaxHighlighter::setFormat(offset, length, tf);
}
