/*
  SPDX-FileCopyrightText: 2016-2024 Laurent Montel <montel@kde.org>

  SPDX-License-Identifier: LGPL-2.0-or-later

*/

#pragma once

#include <QWidget>

#include <KSyntaxHighlighting/Repository>
#include <TextCustomEditor/PlainTextEditor>

namespace TextCustomEditor
{
class TextEditorCompleter;
}
namespace KPIMTextEdit
{
class InsertHtmlEditor : public TextCustomEditor::PlainTextEditor
{
    Q_OBJECT
public:
    explicit InsertHtmlEditor(QWidget *parent = nullptr);
    ~InsertHtmlEditor() override;

protected:
    void keyPressEvent(QKeyEvent *e) override;

private:
    TextCustomEditor::TextEditorCompleter *const mTextEditorCompleter;
    KSyntaxHighlighting::Repository mRepo;
};
}
