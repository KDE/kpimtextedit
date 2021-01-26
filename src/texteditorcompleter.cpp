/*
 * SPDX-FileCopyrightText: 2015-2021 Laurent Montel <montel@kde.org>
 *
 * SPDX-License-Identifier: LGPL-2.1-or-later
 */

#include "texteditorcompleter.h"
#include <QAbstractItemView>
#include <QCompleter>
#include <QPlainTextEdit>
#include <QScrollBar>
#include <QStringListModel>
#include <QTextEdit>

using namespace KPIMTextEdit;

class TextEditorCompleter::TextEditorCompleterPrivate
{
public:
    TextEditorCompleterPrivate(QTextEdit *editor, TextEditorCompleter *qq)
        : textEdit(editor)
        , q(qq)
    {
        createCompleter();
    }

    TextEditorCompleterPrivate(QPlainTextEdit *editor, TextEditorCompleter *qq)
        : plainTextEdit(editor)
        , q(qq)
    {
        createCompleter();
    }

    void setCompletion(const QString &completion);
    Q_REQUIRED_RESULT QString wordUnderCursor() const;
    void createCompleter();
    void completeText();
    QString excludeOfCharacters;
    QCompleter *completer = nullptr;
    QPlainTextEdit *plainTextEdit = nullptr;
    QTextEdit *textEdit = nullptr;
    TextEditorCompleter *const q;
};

void TextEditorCompleter::TextEditorCompleterPrivate::createCompleter()
{
    if (!completer) {
        completer = new QCompleter(q);
    }
    if (plainTextEdit) {
        completer->setWidget(plainTextEdit);
    } else {
        completer->setWidget(textEdit);
    }
    completer->setModelSorting(QCompleter::CaseSensitivelySortedModel);
    completer->setCaseSensitivity(Qt::CaseInsensitive);
    completer->setCompletionMode(QCompleter::PopupCompletion);
    connect(completer, QOverload<const QString &>::of(&QCompleter::activated), q, &TextEditorCompleter::slotCompletion);
}

QString TextEditorCompleter::TextEditorCompleterPrivate::wordUnderCursor() const
{
    static QString eow = QStringLiteral("~!@#$%^&*()+{}|\"<>,./;'[]\\-= "); // everything without ':', '?' and '_'
    QTextCursor tc;
    QTextDocument *document = nullptr;
    if (plainTextEdit) {
        tc = plainTextEdit->textCursor();
        document = plainTextEdit->document();
    } else {
        tc = textEdit->textCursor();
        document = textEdit->document();
    }

    tc.anchor();
    const QString eowStr = excludeOfCharacters.isEmpty() ? eow : excludeOfCharacters;
    while (true) {
        // vHanda: I don't understand why the cursor seems to give a pos 1 past the last char instead
        // of just the last char.
        int pos = tc.position() - 1;
        if (pos < 0 || eowStr.contains(document->characterAt(pos)) || document->characterAt(pos) == QChar(QChar::LineSeparator)
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
    cr.setWidth(completer->popup()->sizeHintForColumn(0) + completer->popup()->verticalScrollBar()->sizeHint().width());
    completer->complete(cr);
}

TextEditorCompleter::TextEditorCompleter(QTextEdit *editor, QObject *parent)
    : QObject(parent)
    , d(new TextEditorCompleter::TextEditorCompleterPrivate(editor, this))
{
}

TextEditorCompleter::TextEditorCompleter(QPlainTextEdit *editor, QObject *parent)
    : QObject(parent)
    , d(new TextEditorCompleter::TextEditorCompleterPrivate(editor, this))
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
    d->completer->setModel(new QStringListModel(QStringList() << listWord << QStringLiteral("TESTING"), d->completer));
}

void TextEditorCompleter::slotCompletion(const QString &completion)
{
    d->setCompletion(completion);
}

void TextEditorCompleter::completeText()
{
    d->completeText();
}

void TextEditorCompleter::setExcludeOfCharacters(const QString &excludes)
{
    d->excludeOfCharacters = excludes;
}
