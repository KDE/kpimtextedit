/*
   SPDX-FileCopyrightText: 2013-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "plaintexteditorwidget.h"
#include "plaintexteditfindbar.h"
#include "plaintexteditor.h"
#include "slidecontainer.h"
#ifdef HAVE_KTEXTADDONS_TEXT_TO_SPEECH_SUPPORT
#include <TextEditTextToSpeech/TextToSpeechContainerWidget>
#endif
#include <QTextCursor>
#include <QVBoxLayout>

using namespace KPIMTextEdit;
class Q_DECL_HIDDEN KPIMTextEdit::PlainTextEditorWidgetPrivate
{
public:
    PlainTextEditorWidgetPrivate() = default;

    KPIMTextEdit::PlainTextEditFindBar *mFindBar = nullptr;
    PlainTextEditor *mEditor = nullptr;
#ifdef HAVE_KTEXTADDONS_TEXT_TO_SPEECH_SUPPORT
    TextEditTextToSpeech::TextToSpeechContainerWidget *mTextToSpeechWidget = nullptr;
#endif
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

PlainTextEditorWidget::~PlainTextEditorWidget() = default;

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
    auto lay = new QVBoxLayout(this);
    lay->setContentsMargins({});
#ifdef HAVE_KTEXTADDONS_TEXT_TO_SPEECH_SUPPORT
    d->mTextToSpeechWidget = new TextEditTextToSpeech::TextToSpeechContainerWidget(this);
    lay->addWidget(d->mTextToSpeechWidget);
#endif
    if (customEditor) {
        d->mEditor = customEditor;
    } else {
        d->mEditor = new PlainTextEditor;
    }
    lay->addWidget(d->mEditor);
#ifdef HAVE_KTEXTADDONS_TEXT_TO_SPEECH_SUPPORT
    connect(d->mEditor, &PlainTextEditor::say, d->mTextToSpeechWidget, &TextEditTextToSpeech::TextToSpeechContainerWidget::say);
#endif
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

#include "moc_plaintexteditorwidget.cpp"
