/*
   Copyright (c) 2015-2017 Laurent Montel <montel@kde.org>

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

#include "plaintexteditor.h"
#include "plaintextsyntaxspellcheckinghighlighter.h"

#include "syntaxhighlighterbase.h"
#include "kpimtextedit_debug.h"

using namespace KPIMTextEdit;

class Q_DECL_HIDDEN KPIMTextEdit::PlainTextSyntaxSpellCheckingHighlighter::PlainTextSyntaxSpellCheckingHighlighterPrivate
{
public:
    PlainTextSyntaxSpellCheckingHighlighterPrivate(PlainTextEditor *plainText)
        : editor(plainText),
          spellCheckingEnabled(false)
    {

    }
    QVector<KPIMTextEdit::Rule> rules;
    PlainTextEditor *editor;
    QColor misspelledColor;
    bool spellCheckingEnabled;
};

PlainTextSyntaxSpellCheckingHighlighter::PlainTextSyntaxSpellCheckingHighlighter(PlainTextEditor *plainText, const QColor &misspelledColor)
    : Sonnet::Highlighter(plainText),
      d(new PlainTextSyntaxSpellCheckingHighlighter::PlainTextSyntaxSpellCheckingHighlighterPrivate(plainText))
{
    d->misspelledColor = misspelledColor;
    setAutomatic(false);
}

PlainTextSyntaxSpellCheckingHighlighter::~PlainTextSyntaxSpellCheckingHighlighter()
{
    delete d;
}

void PlainTextSyntaxSpellCheckingHighlighter::toggleSpellHighlighting(bool on)
{
    if (on != d->spellCheckingEnabled) {
        d->spellCheckingEnabled = on;
        rehighlight();
    }
}

void PlainTextSyntaxSpellCheckingHighlighter::setSyntaxHighlighterRules(const QVector<KPIMTextEdit::Rule> &rule)
{
    d->rules = rule;
}

void PlainTextSyntaxSpellCheckingHighlighter::highlightBlock(const QString &text)
{
    Q_FOREACH (const KPIMTextEdit::Rule &rule, d->rules) {
        const QRegularExpression expression(rule.pattern);
        if (!expression.isValid()) {
            const QString errorString = expression.errorString();
            qCDebug(KPIMTEXTEDIT_LOG) << "expression is invalid, pattern:" << rule.pattern << " error :" << errorString;
        }

        QRegularExpressionMatch match = expression.match(text);

        int index = match.capturedStart();
        while (index >= 0 && match.hasMatch()) {
            setFormat(index, match.capturedLength(), rule.format);
            match = expression.match(text, index + match.capturedLength());
            index = match.capturedStart();
        }
    }
    if (d->spellCheckingEnabled && spellCheckBlock(text) && d->editor->isEnabled()) {
        Highlighter::highlightBlock(text);
    }
}

bool PlainTextSyntaxSpellCheckingHighlighter::spellCheckBlock(const QString &text)
{
    Q_UNUSED(text);
    return true;
}

void PlainTextSyntaxSpellCheckingHighlighter::unsetMisspelled(int start, int count)
{
    Q_UNUSED(start);
    Q_UNUSED(count);
}

void PlainTextSyntaxSpellCheckingHighlighter::setMisspelled(int start, int count)
{
    setMisspelledColor(d->misspelledColor);
    Sonnet::Highlighter::setMisspelled(start, count);
}
