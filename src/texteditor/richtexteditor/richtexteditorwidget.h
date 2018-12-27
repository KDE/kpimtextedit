/*
   Copyright (C) 2013-2019 Laurent Montel <montel@kde.org>

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

#ifndef RICHTEXTEDITORWIDGET_H
#define RICHTEXTEDITORWIDGET_H

#include "kpimtextedit_export.h"

#include <QWidget>

namespace KPIMTextEdit {
class RichTextEditor;
class RichTextEditorWidgetPrivate;
class KPIMTEXTEDIT_EXPORT RichTextEditorWidget : public QWidget
{
    Q_OBJECT
    Q_PROPERTY(bool readOnly READ isReadOnly WRITE setReadOnly)
public:
    explicit RichTextEditorWidget(QWidget *parent = nullptr);
    explicit RichTextEditorWidget(RichTextEditor *customEditor, QWidget *parent = nullptr);
    ~RichTextEditorWidget();

    void clear();

    Q_REQUIRED_RESULT RichTextEditor *editor() const;

    void setReadOnly(bool readOnly);
    bool isReadOnly() const;

    void setHtml(const QString &html);
    Q_REQUIRED_RESULT QString toHtml() const;

    void setPlainText(const QString &text);
    Q_REQUIRED_RESULT QString toPlainText() const;

    void setAcceptRichText(bool b);
    bool acceptRichText() const;

    void setSpellCheckingConfigFileName(const QString &_fileName);

    Q_REQUIRED_RESULT bool isEmpty() const;
public Q_SLOTS:
    void slotFindNext();
    void slotFind();
    void slotReplace();

private:

    void slotHideFindBar();
    void init(RichTextEditor *customEditor = nullptr);
    RichTextEditorWidgetPrivate *const d;
};
}

#endif // RICHTEXTEDITORWIDGET_H
