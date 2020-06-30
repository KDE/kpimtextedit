/*
   SPDX-FileCopyrightText: 2015-2020 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#ifndef KPIMTEXTEDITRICHTEXTCOMPOSEREMAILQUOTEDECORATOR_H
#define KPIMTEXTEDITRICHTEXTCOMPOSEREMAILQUOTEDECORATOR_H

#include <sonnet/spellcheckdecorator.h>
#include "kpimtextedit_export.h"

namespace KPIMTextEdit {
class RichTextComposer;
/**
 * @brief The RichTextComposerEmailQuoteDecorator class
 * @author Laurent Montel <montel@kde.org>
 */
class KPIMTEXTEDIT_EXPORT RichTextComposerEmailQuoteDecorator : public Sonnet::SpellCheckDecorator
{
    Q_OBJECT
public:
    explicit RichTextComposerEmailQuoteDecorator(RichTextComposer *textEdit);
    ~RichTextComposerEmailQuoteDecorator() override;

protected:
    Q_REQUIRED_RESULT bool isSpellCheckingEnabledForBlock(const QString &blockText) const override;
private:
    class RichTextComposerEmailQuoteDecoratorPrivate;
    RichTextComposerEmailQuoteDecoratorPrivate *const d;
};
}

#endif // RICHTEXTCOMPOSEREMAILQUOTEDECORATOR_H
