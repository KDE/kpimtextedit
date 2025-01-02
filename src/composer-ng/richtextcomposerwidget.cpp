/*
   SPDX-FileCopyrightText: 2015-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "richtextcomposerwidget.h"
using namespace Qt::Literals::StringLiterals;

#include "kpimtextedit/richtextcomposer.h"
#include <QHBoxLayout>
#include <TextCustomEditor/RichTextEditorWidget>

using namespace KPIMTextEdit;

class Q_DECL_HIDDEN KPIMTextEdit::RichTextComposerWidgetPrivate
{
public:
    RichTextComposerWidgetPrivate() = default;

    KPIMTextEdit::RichTextComposer *richTextComposer = nullptr;
};

RichTextComposerWidget::RichTextComposerWidget(QWidget *parent)
    : QWidget(parent)
    , d(new KPIMTextEdit::RichTextComposerWidgetPrivate)
{
    auto layout = new QHBoxLayout(this);
    layout->setContentsMargins({});
    d->richTextComposer = new KPIMTextEdit::RichTextComposer(this);
    d->richTextComposer->setObjectName("richtextcomposer"_L1);
    auto editorWidget = new TextCustomEditor::RichTextEditorWidget(d->richTextComposer, this);
    layout->addWidget(editorWidget);
}

RichTextComposerWidget::~RichTextComposerWidget() = default;

KPIMTextEdit::RichTextComposer *RichTextComposerWidget::richTextComposer() const
{
    return d->richTextComposer;
}

#include "moc_richtextcomposerwidget.cpp"
