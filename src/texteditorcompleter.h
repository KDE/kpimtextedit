/*
 * SPDX-FileCopyrightText: 2015-2021 Laurent Montel <montel@kde.org>
 *
 * SPDX-License-Identifier: LGPL-2.1-or-later
 */

#pragma once

#include "kpimtextedit_export.h"
#include <QObject>

class QCompleter;
class QTextEdit;
class QPlainTextEdit;

namespace KPIMTextEdit
{
/**
 * @brief The TextEditorCompleter class
 * @author Laurent Montel <montel@kde.org>
 */
class KPIMTEXTEDIT_EXPORT TextEditorCompleter : public QObject
{
    Q_OBJECT
public:
    explicit TextEditorCompleter(QTextEdit *editor, QObject *parent);
    explicit TextEditorCompleter(QPlainTextEdit *editor, QObject *parent);
    ~TextEditorCompleter() override;

    void setCompleterStringList(const QStringList &list);

    Q_REQUIRED_RESULT QCompleter *completer() const;

    void completeText();

    void setExcludeOfCharacters(const QString &excludes);

private:
    void slotCompletion(const QString &text);
    class TextEditorCompleterPrivate;
    TextEditorCompleterPrivate *const d;
};
}

