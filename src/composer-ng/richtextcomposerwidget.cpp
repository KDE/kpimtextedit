/*
   SPDX-FileCopyrightText: 2015-2020 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "richtextcomposerwidget.h"
#include <kpimtextedit/richtextcomposer.h>
#include <kpimtextedit/richtexteditorwidget.h>
#include <QHBoxLayout>

using namespace KPIMTextEdit;

class Q_DECL_HIDDEN KPIMTextEdit::RichTextComposerWidgetPrivate
{
public:
    RichTextComposerWidgetPrivate()
    {
    }

    KPIMTextEdit::RichTextComposer *richTextComposer = nullptr;
};

RichTextComposerWidget::RichTextComposerWidget(QWidget *parent)
    : QWidget(parent)
    , d(new KPIMTextEdit::RichTextComposerWidgetPrivate)
{
    QHBoxLayout *layout = new QHBoxLayout(this);
    layout->setContentsMargins(0, 0, 0, 0);
    d->richTextComposer = new KPIMTextEdit::RichTextComposer(this);
    d->richTextComposer->setObjectName(QStringLiteral("richtextcomposer"));
    RichTextEditorWidget *editorWidget = new RichTextEditorWidget(d->richTextComposer, this);
    layout->addWidget(editorWidget);
}

RichTextComposerWidget::~RichTextComposerWidget()
{
    delete d;
}

KPIMTextEdit::RichTextComposer *RichTextComposerWidget::richTextComposer() const
{
    return d->richTextComposer;
}
