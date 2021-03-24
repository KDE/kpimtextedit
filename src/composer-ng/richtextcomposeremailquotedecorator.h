/*
   SPDX-FileCopyrightText: 2015-2021 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "kpimtextedit_export.h"
#include <sonnet/spellcheckdecorator.h>

namespace KPIMTextEdit
{
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

