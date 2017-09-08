/*
   Copyright (C) 2013-2017 Laurent Montel <montel@kde.org>

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

#include "plaintexteditorwidget.h"
#include "plaintexteditor.h"
#include "plaintexteditfindbar.h"
#include "texttospeech/texttospeechwidget.h"
#include "slidecontainer.h"

#include <QVBoxLayout>
#include <QTextCursor>

using namespace KPIMTextEdit;
class KPIMTextEdit::PlainTextEditorWidgetPrivate
{
public:
    PlainTextEditorWidgetPrivate()
    {
    }

    KPIMTextEdit::PlainTextEditFindBar *mFindBar = nullptr;
    PlainTextEditor *mEditor = nullptr;
    KPIMTextEdit::TextToSpeechWidget *mTextToSpeechWidget = nullptr;
    KPIMTextEdit::SlideContainer *mSliderContainer = nullptr;
};

PlainTextEditorWidget::PlainTextEditorWidget(PlainTextEditor *customEditor, QWidget *parent)
    : QWidget(parent)
    , d(new KPIMTextEdit::PlainTextEditorWidgetPrivate)
{
    init(customEditor);
}

PlainTextEditorWidget::PlainTextEditorWidget(QWidget *parent)
    : QWidget(parent)
    , d(new KPIMTextEdit::PlainTextEditorWidgetPrivate)
{
    init();
}

PlainTextEditorWidget::~PlainTextEditorWidget()
{
    delete d;
}

PlainTextEditor *PlainTextEditorWidget::editor() const
{
    return d->mEditor;
}

void PlainTextEditorWidget::clear()
{
    d->mEditor->clear();
}

void PlainTextEditorWidget::setSpellCheckingConfigFileName(const QString &_fileName)
{
    d->mEditor->setSpellCheckingConfigFileName(_fileName);
}

void PlainTextEditorWidget::setPlainText(const QString &text)
{
    d->mEditor->setPlainText(text);
}

bool PlainTextEditorWidget::isEmpty() const
{
    return d->mEditor->document()->isEmpty();
}

QString PlainTextEditorWidget::toPlainText() const
{
    return d->mEditor->toPlainText();
}

void PlainTextEditorWidget::init(PlainTextEditor *customEditor)
{
    QVBoxLayout *lay = new QVBoxLayout(this);
    lay->setMargin(0);
    d->mTextToSpeechWidget = new KPIMTextEdit::TextToSpeechWidget(this);
    lay->addWidget(d->mTextToSpeechWidget);
    if (customEditor) {
        d->mEditor = customEditor;
    } else {
        d->mEditor = new PlainTextEditor;
    }
    lay->addWidget(d->mEditor);
    connect(d->mEditor, &PlainTextEditor::say, d->mTextToSpeechWidget, &KPIMTextEdit::TextToSpeechWidget::say);

    d->mSliderContainer = new KPIMTextEdit::SlideContainer(this);

    d->mFindBar = new KPIMTextEdit::PlainTextEditFindBar(d->mEditor, this);
    d->mFindBar->setHideWhenClose(false);
    connect(d->mFindBar, &KPIMTextEdit::PlainTextEditFindBar::displayMessageIndicator, d->mEditor, &PlainTextEditor::slotDisplayMessageIndicator);
    connect(d->mFindBar, &KPIMTextEdit::PlainTextEditFindBar::hideFindBar, this, &PlainTextEditorWidget::slotHideFindBar);
    d->mSliderContainer->setContent(d->mFindBar);

    lay->addWidget(d->mSliderContainer);

    connect(d->mEditor, &PlainTextEditor::findText, this, &PlainTextEditorWidget::slotFind);

    connect(d->mEditor, &PlainTextEditor::replaceText, this, &PlainTextEditorWidget::slotReplace);
}

void PlainTextEditorWidget::slotHideFindBar()
{
    d->mSliderContainer->slideOut();
    d->mEditor->setFocus();
}

bool PlainTextEditorWidget::isReadOnly() const
{
    return d->mEditor->isReadOnly();
}

void PlainTextEditorWidget::setReadOnly(bool readOnly)
{
    d->mEditor->setReadOnly(readOnly);
}

void PlainTextEditorWidget::slotReplace()
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

void PlainTextEditorWidget::slotFind()
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
