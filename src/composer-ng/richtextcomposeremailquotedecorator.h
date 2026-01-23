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
 * \brief The RichTextComposerEmailQuoteDecorator class
 * \author Laurent Montel <montel@kde.org>
 */
class KPIMTEXTEDIT_EXPORT RichTextComposerEmailQuoteDecorator : public Sonnet::SpellCheckDecorator
{
    Q_OBJECT
public:
    /*!
     */
    explicit RichTextComposerEmailQuoteDecorator(RichTextComposer *textEdit);
    /*!
     */
    ~RichTextComposerEmailQuoteDecorator() override;

protected:
    /*!
     */
    [[nodiscard]] bool isSpellCheckingEnabledForBlock(const QString &blockText) const override;

private:
    class RichTextComposerEmailQuoteDecoratorPrivate;
    std::unique_ptr<RichTextComposerEmailQuoteDecoratorPrivate> const d;
};
}
