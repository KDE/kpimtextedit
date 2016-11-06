/*
  Copyright (c) 2016 Montel Laurent <montel@kde.org>

  This library is free software; you can redistribute it and/or modify it
  under the terms of the GNU Library General Public License as published by
  the Free Software Foundation; either version 2 of the License, or (at your
  option) any later version.

  This library is distributed in the hope that it will be useful, but WITHOUT
  ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
  FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Library General Public
  License for more details.

  You should have received a copy of the GNU Library General Public License
  along with this library; see the file COPYING.LIB.  If not, write to the
  Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA
  02110-1301, USA.

*/

#include "inserthtmleditor.h"
#include "texteditorcompleter.h"

#ifdef KDEPIM_KF5SYNTAXHIGHLIGHTING_SUPPORT
#include <KSyntaxHighlighting/SyntaxHighlighter>
#include <KSyntaxHighlighting/Definition>
#include <KSyntaxHighlighting/Theme>
#else
#include "htmlhighlighter.h"
#endif

#include <QStringList>
#include <QCompleter>
#include <QAbstractItemView>

using namespace KPIMTextEdit;

InsertHtmlEditor::InsertHtmlEditor(QWidget *parent)
    : KPIMTextEdit::PlainTextEditor(parent)
{
#ifdef KDEPIM_KF5SYNTAXHIGHLIGHTING_SUPPORT
    KSyntaxHighlighting::Definition def;
    def = mRepo.definitionForName(QStringLiteral("HTML"));

    KSyntaxHighlighting::SyntaxHighlighter *hl = new KSyntaxHighlighting::SyntaxHighlighter(document());
    hl->setTheme((palette().color(QPalette::Base).lightness() < 128)
                 ? mRepo.defaultTheme(KSyntaxHighlighting::Repository::DarkTheme)
                 : mRepo.defaultTheme(KSyntaxHighlighting::Repository::LightTheme));
    hl->setDefinition(def);
#else
    new KPIMTextEdit::HtmlHighlighter(document());
#endif
    setFocus();
    mTextEditorCompleter = new KPIMTextEdit::TextEditorCompleter(this, this);
    QStringList completerList;
    completerList << QStringLiteral("<b></b>")
                  << QStringLiteral("<i></i>")
                  << QStringLiteral("<u></u>");
    //Add more
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
