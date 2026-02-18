/*
   SPDX-FileCopyrightText: 2015-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "kpimtextedit_export.h"
#include <QWidget>

/*!
 * \namespace KPIMTextEdit
 * \inmodule KPIMTextedit
 */
namespace KPIMTextEdit
{
class RichTextComposerWidgetPrivate;
class RichTextComposer;
/*!
 * \class KPIMTextEdit::RichTextComposerWidget
 * \inmodule KPIMTextedit
 * \inheaderfile KPIMTextEdit/RichTextComposerWidget
 *
 * \brief The RichTextComposerWidget class provides a widget wrapping RichTextComposer with additional UI.
 * \author Laurent Montel <montel@kde.org>
 * \since 5.2
 */
class KPIMTEXTEDIT_EXPORT RichTextComposerWidget : public QWidget
{
    Q_OBJECT
public:
    /*!
     * Constructs a RichTextComposerWidget with the given \a parent.
     */
    explicit RichTextComposerWidget(QWidget *parent = nullptr);
    /*!
     * Destroys the RichTextComposerWidget.
     */
    ~RichTextComposerWidget() override;

    /*!
     * Returns the RichTextComposer instance managed by this widget.
     */
    [[nodiscard]] KPIMTextEdit::RichTextComposer *richTextComposer() const;

private:
    std::unique_ptr<RichTextComposerWidgetPrivate> const d;
};
}
