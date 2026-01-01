/*
  SPDX-FileCopyrightText: 2016-2026 Laurent Montel <montel@kde.org>

  SPDX-License-Identifier: LGPL-2.0-or-later

*/

#include "inserthtmleditor.h"
using namespace Qt::Literals::StringLiterals;

#include "kpimtextedit_debug.h"
#include <TextCustomEditor/TextEditorCompleter>

#include <KSyntaxHighlighting/Definition>
#include <KSyntaxHighlighting/SyntaxHighlighter>
#include <KSyntaxHighlighting/Theme>

#include <QAbstractItemView>
#include <QCompleter>
#include <QStringList>

using namespace KPIMTextEdit;

InsertHtmlEditor::InsertHtmlEditor(QWidget *parent)
    : TextCustomEditor::PlainTextEditor(parent)
    , mTextEditorCompleter(new TextCustomEditor::TextEditorCompleter(this, this))
{
    const KSyntaxHighlighting::Definition def = mRepo.definitionForName(u"HTML"_s);
    if (!def.isValid()) {
        qCWarning(KPIMTEXTEDIT_LOG) << "Invalid definition name";
    }

    auto hl = new KSyntaxHighlighting::SyntaxHighlighter(document());
    hl->setTheme((palette().color(QPalette::Base).lightness() < 128) ? mRepo.defaultTheme(KSyntaxHighlighting::Repository::DarkTheme)
                                                                     : mRepo.defaultTheme(KSyntaxHighlighting::Repository::LightTheme));
    hl->setDefinition(def);
    setFocus();
    const QStringList completerList = {u"<b></b>"_s, u"<i></i>"_s, u"<u></u>"_s};
    // Add more
    mTextEditorCompleter->setCompleterStringList(completerList);
    mTextEditorCompleter->setExcludeOfCharacters(u"~!@#$%^&*()+{}|,./;'[]\\-= "_s);
}

InsertHtmlEditor::~InsertHtmlEditor() = default;

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
    TextCustomEditor::PlainTextEditor::keyPressEvent(e);
    mTextEditorCompleter->completeText();
}

#include "moc_inserthtmleditor.cpp"
