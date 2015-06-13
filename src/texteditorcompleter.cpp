/*
 * Copyright (C)  2015 Laurent Montel <montel@kde.org>
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA
 * 02110-1301  USA
 */

#include "texteditorcompleter.h"
#include <QCompleter>
#include <QPlainTextEdit>
#include <QStringListModel>
#include <QTextEdit>
#include <QScrollBar>
#include <QAbstractItemView>

using namespace KPIMTextEdit;

class TextEditorCompleter::TextEditorCompleterPrivate
{
public:
    TextEditorCompleterPrivate(QTextEdit *editor, TextEditorCompleter *qq)
        : completer(Q_NULLPTR),
          plainTextEdit(Q_NULLPTR),
          textEdit(editor),
          q(qq)
    {

    }
    TextEditorCompleterPrivate(QPlainTextEdit *editor, TextEditorCompleter *qq)
        : completer(Q_NULLPTR),
          plainTextEdit(editor),
          textEdit(Q_NULLPTR),
          q(qq)
    {

    }
    void setCompletion(const QString &completion);
    QString wordUnderCursor() const;
    void createCompleter();
    void completeText();
    QCompleter *completer;
    QPlainTextEdit *plainTextEdit;
    QTextEdit *textEdit;
    TextEditorCompleter *q;
};

void TextEditorCompleter::TextEditorCompleterPrivate::createCompleter()
{
    if (!completer) {
        completer = new QCompleter(q);
    }
    completer->setModelSorting(QCompleter::CaseSensitivelySortedModel);
    completer->setCaseSensitivity(Qt::CaseInsensitive);
    completer->setCompletionMode(QCompleter::PopupCompletion);
    if (plainTextEdit)
        completer->setWidget(plainTextEdit);
    else
        completer->setWidget(textEdit);
    connect(completer, static_cast<void (QCompleter::*)(const QString &)>(&QCompleter::activated), q, &TextEditorCompleter::slotCompletion);
}

QString TextEditorCompleter::TextEditorCompleterPrivate::wordUnderCursor() const
{
    static QString eow = QStringLiteral("~!@#$%^&*()+{}|\"<>,./;'[]\\-= ");   // everything without ':', '?' and '_'
    QTextCursor tc;
    QTextDocument *document = 0;
    if (plainTextEdit) {
        tc = plainTextEdit->textCursor();
        document = plainTextEdit->document();
    } else {
        tc = textEdit->textCursor();
        document = textEdit->document();
    }

    tc.anchor();
    while (1) {
        // vHanda: I don't understand why the cursor seems to give a pos 1 past the last char instead
        // of just the last char.
        int pos = tc.position() - 1;
        if (pos < 0 || eow.contains(document->characterAt(pos))
                || document->characterAt(pos) == QChar(QChar::LineSeparator)
                || document->characterAt(pos) == QChar(QChar::ParagraphSeparator)) {
            break;
        }
        tc.movePosition(QTextCursor::Left, QTextCursor::KeepAnchor);
    }
    return tc.selectedText();
}

void TextEditorCompleter::TextEditorCompleterPrivate::setCompletion(const QString &completion)
{
    QTextCursor tc;
    if (plainTextEdit) {
        tc = plainTextEdit->textCursor();
    } else {
        tc = textEdit->textCursor();
    }
    const int extra = completion.length() - completer->completionPrefix().length();
    tc.movePosition(QTextCursor::Left);
    tc.movePosition(QTextCursor::EndOfWord);
    tc.insertText(completion.right(extra));
    if (plainTextEdit) {
        plainTextEdit->setTextCursor(tc);
    } else {
        textEdit->setTextCursor(tc);
    }
}

void TextEditorCompleter::TextEditorCompleterPrivate::completeText()
{
    if (!completer) {
        return;
    }
    const QString text = wordUnderCursor();

    if (text.length() < 2) { // min 2 char for completion
        return;
    }

    completer->setCompletionPrefix(text);

    QRect cr;
    if (plainTextEdit) {
        cr = plainTextEdit->cursorRect();
    } else {
        cr = textEdit->cursorRect();
    }
    cr.setWidth(completer->popup()->sizeHintForColumn(0)
                + completer->popup()->verticalScrollBar()->sizeHint().width());
    completer->complete(cr);
}


TextEditorCompleter::TextEditorCompleter(QTextEdit *editor, QObject *parent)
    : QObject(parent),
      d(new TextEditorCompleter::TextEditorCompleterPrivate(editor, this))
{

}

TextEditorCompleter::TextEditorCompleter(QPlainTextEdit *editor, QObject *parent)
    : QObject(parent),
      d(new TextEditorCompleter::TextEditorCompleterPrivate(editor, this))
{

}

TextEditorCompleter::~TextEditorCompleter()
{
    delete d;
}

QCompleter *TextEditorCompleter::completer() const
{
    return d->completer;
}

void TextEditorCompleter::setCompleterStringList(const QStringList &listWord)
{
    d->createCompleter();
    d->completer->setModel(new QStringListModel(listWord, d->completer));
}

void TextEditorCompleter::slotCompletion(const QString &completion)
{
    d->setCompletion(completion);
}

void TextEditorCompleter::completeText()
{
    d->completeText();
}
