/*
   SPDX-FileCopyrightText: 2015-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "kpimtextedit_export.h"
#include <memory>
#include <sonnet/spellcheckdecorator.h>
namespace KPIMTextEdit
{
class RichTextComposer;
/*!
 * \class KPIMTextEdit::RichTextComposerEmailQuoteDecorator
 * \inmodule KPIMTextedit
 * \inheaderfile KPIMTextEdit/RichTextComposerEmailQuoteDecorator
 *
 * \brief The RichTextComposerEmailQuoteDecorator class adds spell-check decoration to quoted text.
 * \author Laurent Montel <montel@kde.org>
 */
class KPIMTEXTEDIT_EXPORT RichTextComposerEmailQuoteDecorator : public Sonnet::SpellCheckDecorator
{
    Q_OBJECT
public:
    /*!
     * Constructs a RichTextComposerEmailQuoteDecorator with the given \a textEdit.
     */
    explicit RichTextComposerEmailQuoteDecorator(RichTextComposer *textEdit);
    /*!
     * Destroys the RichTextComposerEmailQuoteDecorator.
     */
    ~RichTextComposerEmailQuoteDecorator() override;

protected:
    /*!
     * Returns whether spell checking should be enabled for the given \a blockText.
     * Quoted text blocks will return false, while normal text will return true.
     * Reimplemented from Sonnet::SpellCheckDecorator.
     */
    [[nodiscard]] bool isSpellCheckingEnabledForBlock(const QString &blockText) const override;

private:
    class RichTextComposerEmailQuoteDecoratorPrivate;
    std::unique_ptr<RichTextComposerEmailQuoteDecoratorPrivate> const d;
};
}
