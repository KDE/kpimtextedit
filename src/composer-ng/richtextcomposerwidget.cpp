/*
  Copyright (c) 2015 Montel Laurent <montel@kde.org>

  This program is free software; you can redistribute it and/or modify it
  under the terms of the GNU General Public License, version 2, as
  published by the Free Software Foundation.

  This program is distributed in the hope that it will be useful, but
  WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  General Public License for more details.

  You should have received a copy of the GNU General Public License along
  with this program; if not, write to the Free Software Foundation, Inc.,
  51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
*/
#include "richtextcomposerwidget.h"
#include <kpimtextedit/richtextcomposer.h>
#include <kpimtextedit/richtexteditorwidget.h>
#include <QHBoxLayout>

using namespace KPIMTextEdit;

class KPIMTextEdit::RichTextComposerWidgetPrivate
{
public:
    RichTextComposerWidgetPrivate()
        : richTextComposer(Q_NULLPTR)
    {

    }
    KPIMTextEdit::RichTextComposer *richTextComposer;
};

RichTextComposerWidget::RichTextComposerWidget(QWidget *parent)
    : QWidget(parent),
      d(new KPIMTextEdit::RichTextComposerWidgetPrivate)
{
    QHBoxLayout *layout = new QHBoxLayout(this);
    layout->setMargin(0);
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
