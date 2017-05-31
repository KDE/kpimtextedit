/*
   Copyright (C) 2013-2016 Laurent Montel <montel@kde.org>

   This library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Library General Public
   License as published by the Free Software Foundation; either
   version 2 of the License, or (at your option) any later version.

   This library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Library General Public License for more details.

   You should have received a copy of the GNU Library General Public License
   along with this library; see the file COPYING.LIB.  If not, write to
   the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
   Boston, MA 02110-1301, USA.
*/

#ifndef PLAINTEXTEDITORWIDGET_H
#define PLAINTEXTEDITORWIDGET_H

#include "kpimtextedit_export.h"
#include <QWidget>

namespace KPIMTextEdit {
class PlainTextEditor;
class PlainTextEditorWidgetPrivate;
class KPIMTEXTEDIT_EXPORT PlainTextEditorWidget : public QWidget
{
    Q_OBJECT
    Q_PROPERTY(bool readOnly READ isReadOnly WRITE setReadOnly)
public:
    explicit PlainTextEditorWidget(QWidget *parent = nullptr);
    explicit PlainTextEditorWidget(PlainTextEditor *customEditor, QWidget *parent = nullptr);
    ~PlainTextEditorWidget();

    PlainTextEditor *editor() const;

    void setReadOnly(bool readOnly);
    bool isReadOnly() const;

    void setPlainText(const QString &text);
    QString toPlainText() const;

    void clear();
    void setSpellCheckingConfigFileName(const QString &_fileName);

    bool isEmpty() const;
private Q_SLOTS:
    void slotFind();
    void slotReplace();

    void slotHideFindBar();
private:
    void init(PlainTextEditor *customEditor = nullptr);
    PlainTextEditorWidgetPrivate *const d;
};
}

#endif // PLAINTEXTEDITORWIDGET_H
