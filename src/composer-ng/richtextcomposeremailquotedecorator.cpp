/*
   SPDX-FileCopyrightText: 2015-2020 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "richtextcomposer.h"
#include "richtextcomposeremailquotedecorator.h"
using namespace KPIMTextEdit;

class Q_DECL_HIDDEN RichTextComposerEmailQuoteDecorator::RichTextComposerEmailQuoteDecoratorPrivate
{
public:
    RichTextComposerEmailQuoteDecoratorPrivate(RichTextComposer *textEdit)
        : composerEditor(textEdit)
    {
    }

    RichTextComposer *composerEditor = nullptr;
};

RichTextComposerEmailQuoteDecorator::RichTextComposerEmailQuoteDecorator(RichTextComposer *textEdit)
    : Sonnet::SpellCheckDecorator(textEdit)
    , d(new RichTextComposerEmailQuoteDecorator::RichTextComposerEmailQuoteDecoratorPrivate(textEdit))
{
}

RichTextComposerEmailQuoteDecorator::~RichTextComposerEmailQuoteDecorator()
{
    delete d;
}

bool RichTextComposerEmailQuoteDecorator::isSpellCheckingEnabledForBlock(const QString &blockText) const
{
    return !d->composerEditor->isLineQuoted(blockText) && d->composerEditor->checkSpellingEnabled();
}
