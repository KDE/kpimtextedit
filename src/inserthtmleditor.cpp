/*
  SPDX-FileCopyrightText: 2016-2021 Laurent Montel <montel@kde.org>

  SPDX-License-Identifier: LGPL-2.0-or-later

*/

#include "inserthtmleditor.h"
#include "kpimtextedit_debug.h"
#include "texteditorcompleter.h"

#include <KSyntaxHighlighting/Definition>
#include <KSyntaxHighlighting/SyntaxHighlighter>
#include <KSyntaxHighlighting/Theme>

#include <QAbstractItemView>
#include <QCompleter>
#include <QStringList>

using namespace KPIMTextEdit;

InsertHtmlEditor::InsertHtmlEditor(QWidget *parent)
    : KPIMTextEdit::PlainTextEditor(parent)
{
    const KSyntaxHighlighting::Definition def = mRepo.definitionForName(QStringLiteral("HTML"));
    if (!def.isValid()) {
        qCWarning(KPIMTEXTEDIT_LOG) << "Invalid definition name";
    }

    auto hl = new KSyntaxHighlighting::SyntaxHighlighter(document());
    hl->setTheme((palette().color(QPalette::Base).lightness() < 128) ? mRepo.defaultTheme(KSyntaxHighlighting::Repository::DarkTheme)
                                                                     : mRepo.defaultTheme(KSyntaxHighlighting::Repository::LightTheme));
    hl->setDefinition(def);
    setFocus();
    mTextEditorCompleter = new KPIMTextEdit::TextEditorCompleter(this, this);
    const QStringList completerList = {QStringLiteral("<b></b>"), QStringLiteral("<i></i>"), QStringLiteral("<u></u>")};
    // Add more
    mTextEditorCompleter->setCompleterStringList(completerList);
    mTextEditorCompleter->setExcludeOfCharacters(QStringLiteral("~!@#$%^&*()+{}|,./;'[]\\-= "));
}

InsertHtmlEditor::~InsertHtmlEditor()
{
}

void InsertHtmlEditor::keyPressEvent(QKeyEvent *e)
{
    if (mTextEditorCompleter->completer()->popup()->isVisible()) {
        switch (e->key()) {
        case Qt::Key_Enter:
        case Qt::Key_Return:
        case Qt::Key_Escape:
        case Qt::Key_Tab:
        case Qt::Key_Backtab:
            e->ignore();
            return; // let the completer do default behavior
        default:
            break;
        }
    }
    KPIMTextEdit::PlainTextEditor::keyPressEvent(e);
    mTextEditorCompleter->completeText();
}
