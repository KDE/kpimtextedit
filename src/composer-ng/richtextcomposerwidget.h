/*
   SPDX-FileCopyrightText: 2015-2021 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#ifndef RICHTEXTCOMPOSERWIDGET_H
#define RICHTEXTCOMPOSERWIDGET_H

#include "kpimtextedit_export.h"
#include <QWidget>
namespace KPIMTextEdit
{
class RichTextComposerWidgetPrivate;
class RichTextComposer;
/**
 * @brief The RichTextComposerWidget class
 * @author Laurent Montel <montel@kde.org>
 * @since 5.2
 */
class KPIMTEXTEDIT_EXPORT RichTextComposerWidget : public QWidget
{
    Q_OBJECT
public:
    explicit RichTextComposerWidget(QWidget *parent = nullptr);
    ~RichTextComposerWidget();

    Q_REQUIRED_RESULT KPIMTextEdit::RichTextComposer *richTextComposer() const;

private:
    RichTextComposerWidgetPrivate *const d;
};
}

#endif // RICHTEXTCOMPOSERWIDGET_H
