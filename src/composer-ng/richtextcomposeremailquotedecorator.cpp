/*
   SPDX-FileCopyrightText: 2015-2021 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "richtextcomposeremailquotedecorator.h"
#include "richtextcomposer.h"
using namespace KPIMTextEdit;

class Q_DECL_HIDDEN RichTextComposerEmailQuoteDecorator::RichTextComposerEmailQuoteDecoratorPrivate
{
public:
    RichTextComposerEmailQuoteDecoratorPrivate(RichTextComposer *textEdit)
        : composerEditor(textEdit)
    {
    }

    RichTextComposer *const composerEditor;
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
