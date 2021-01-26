/*
  SPDX-FileCopyrightText: 2016-2021 Laurent Montel <montel@kde.org>

  SPDX-License-Identifier: LGPL-2.0-or-later

*/

#ifndef RICHTEXTCOMPOSER_INSERTHTMLEDITOR_H
#define RICHTEXTCOMPOSER_INSERTHTMLEDITOR_H

#include <QWidget>

#include "kpimtextedit/plaintexteditor.h"
#include <KSyntaxHighlighting/Repository>

namespace KPIMTextEdit
{
class TextEditorCompleter;
class InsertHtmlEditor : public KPIMTextEdit::PlainTextEditor
{
    Q_OBJECT
public:
    explicit InsertHtmlEditor(QWidget *parent = nullptr);
    ~InsertHtmlEditor() override;

protected:
    void keyPressEvent(QKeyEvent *e) override;

private:
    KPIMTextEdit::TextEditorCompleter *mTextEditorCompleter = nullptr;
    KSyntaxHighlighting::Repository mRepo;
};
}

#endif // INSERTHTMLEDITOR_H
