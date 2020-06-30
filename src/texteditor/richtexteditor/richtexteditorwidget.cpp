/*
   SPDX-FileCopyrightText: 2013-2020 Laurent Montel <montel@kde.org>

   This library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Library General Public
   License as published by the Free Software Foundation; either
   version 2 of the License, or (at your option) any later version.

   This library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Library General Public License for more details.

   You should have received a copy of the GNU Library General Public License
   along with this library; see the file COPYING.LIB.  If not, write to
   the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
   Boston, MA 02110-1301, USA.
*/

#include "richtexteditorwidget.h"
#include "richtexteditor.h"
#include "richtexteditfindbar.h"

#include <QVBoxLayout>
#include <QTextCursor>
#include <texttospeech/texttospeechwidget.h>

#include "slidecontainer.h"

using namespace KPIMTextEdit;

class Q_DECL_HIDDEN KPIMTextEdit::RichTextEditorWidgetPrivate
{
public:
    RichTextEditorWidgetPrivate()
    {
    }

    KPIMTextEdit::RichTextEditFindBar *mFindBar = nullptr;
    RichTextEditor *mEditor = nullptr;
    KPIMTextEdit::TextToSpeechWidget *mTextToSpeechWidget = nullptr;
    KPIMTextEdit::SlideContainer *mSliderContainer = nullptr;
};

RichTextEditorWidget::RichTextEditorWidget(RichTextEditor *customEditor, QWidget *parent)
    : QWidget(parent)
    , d(new KPIMTextEdit::RichTextEditorWidgetPrivate)
{
    init(customEditor);
}

RichTextEditorWidget::RichTextEditorWidget(QWidget *parent)
    : QWidget(parent)
    , d(new KPIMTextEdit::RichTextEditorWidgetPrivate)
{
    init();
}

RichTextEditorWidget::~RichTextEditorWidget()
{
    delete d;
}

void RichTextEditorWidget::clear()
{
    d->mEditor->clear();
}

RichTextEditor *RichTextEditorWidget::editor() const
{
    return d->mEditor;
}

void RichTextEditorWidget::setAcceptRichText(bool b)
{
    d->mEditor->setAcceptRichText(b);
}

bool RichTextEditorWidget::acceptRichText() const
{
    return d->mEditor->acceptRichText();
}

void RichTextEditorWidget::setSpellCheckingConfigFileName(const QString &_fileName)
{
    d->mEditor->setSpellCheckingConfigFileName(_fileName);
}

void RichTextEditorWidget::setHtml(const QString &html)
{
    d->mEditor->setHtml(html);
}

QString RichTextEditorWidget::toHtml() const
{
    return d->mEditor->toHtml();
}

void RichTextEditorWidget::setPlainText(const QString &text)
{
    d->mEditor->setPlainText(text);
}

bool RichTextEditorWidget::isEmpty() const
{
    return d->mEditor->document()->isEmpty();
}

QString RichTextEditorWidget::toPlainText() const
{
    return d->mEditor->toPlainText();
}

void RichTextEditorWidget::init(RichTextEditor *customEditor)
{
    QVBoxLayout *lay = new QVBoxLayout(this);
    lay->setContentsMargins(0, 0, 0, 0);
    d->mTextToSpeechWidget = new KPIMTextEdit::TextToSpeechWidget(this);
    lay->addWidget(d->mTextToSpeechWidget);
    if (customEditor) {
        d->mEditor = customEditor;
    } else {
        d->mEditor = new RichTextEditor;
    }
    connect(d->mEditor, &RichTextEditor::say, d->mTextToSpeechWidget, &KPIMTextEdit::TextToSpeechWidget::say);
    lay->addWidget(d->mEditor);

    d->mSliderContainer = new KPIMTextEdit::SlideContainer(this);

    d->mFindBar = new KPIMTextEdit::RichTextEditFindBar(d->mEditor, this);
    d->mFindBar->setHideWhenClose(false);
    connect(d->mFindBar, &KPIMTextEdit::RichTextEditFindBar::displayMessageIndicator, d->mEditor, &RichTextEditor::slotDisplayMessageIndicator);

    connect(d->mFindBar, &KPIMTextEdit::RichTextEditFindBar::hideFindBar, this, &RichTextEditorWidget::slotHideFindBar);
    d->mSliderContainer->setContent(d->mFindBar);
    lay->addWidget(d->mSliderContainer);

    connect(d->mEditor, &RichTextEditor::findText, this, &RichTextEditorWidget::slotFind);
    connect(d->mEditor, &RichTextEditor::replaceText, this, &RichTextEditorWidget::slotReplace);
}

void RichTextEditorWidget::slotHideFindBar()
{
    d->mSliderContainer->slideOut();
    d->mEditor->setFocus();
}

bool RichTextEditorWidget::isReadOnly() const
{
    return d->mEditor->isReadOnly();
}

void RichTextEditorWidget::setReadOnly(bool readOnly)
{
    d->mEditor->setReadOnly(readOnly);
}

void RichTextEditorWidget::slotReplace()
{
    if (d->mEditor->searchSupport()) {
        if (d->mEditor->textCursor().hasSelection()) {
            d->mFindBar->setText(d->mEditor->textCursor().selectedText());
        }
        d->mFindBar->showReplace();
        d->mSliderContainer->slideIn();
        d->mFindBar->focusAndSetCursor();
    }
}

void RichTextEditorWidget::slotFindNext()
{
    if (d->mEditor->searchSupport()) {
        if (d->mFindBar->isVisible()) {
            d->mFindBar->findNext();
        } else {
            slotFind();
        }
    }
}

void RichTextEditorWidget::slotFind()
{
    if (d->mEditor->searchSupport()) {
        if (d->mEditor->textCursor().hasSelection()) {
            d->mFindBar->setText(d->mEditor->textCursor().selectedText());
        }
        d->mEditor->moveCursor(QTextCursor::Start);

        d->mFindBar->showFind();
        d->mSliderContainer->slideIn();
        d->mFindBar->focusAndSetCursor();
    }
}
