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

#ifndef TEXTEDITORCOMPLETER_H
#define TEXTEDITORCOMPLETER_H

#include <QObject>
#include "kpimtextedit_export.h"

class QCompleter;
class QTextEdit;
class QPlainTextEdit;

namespace KPIMTextEdit
{
class KPIMTEXTEDIT_EXPORT TextEditorCompleter : public QObject
{
    Q_OBJECT
public:
    explicit TextEditorCompleter(QTextEdit *editor, QObject *parent);
    explicit TextEditorCompleter(QPlainTextEdit *editor, QObject *parent);
    ~TextEditorCompleter();

    void setCompleterStringList(const QStringList &list);

    QCompleter *completer() const;

    void completeText();

    void setExcludeOfCharacters(const QString &excludes);
private:
    void slotCompletion(const QString &text);
    class TextEditorCompleterPrivate;
    TextEditorCompleterPrivate *const d;
};
}

#endif // TEXTEDITORCOMPLETER_H
