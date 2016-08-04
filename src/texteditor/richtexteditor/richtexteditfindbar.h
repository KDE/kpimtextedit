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

#ifndef RICHTEXTEDITFINDBAR_H
#define RICHTEXTEDITFINDBAR_H

#include "kpimtextedit_export.h"
#include "texteditor/commonwidget/texteditfindbarbase.h"
#include <QTextDocument>

class QTextEdit;
namespace KPIMTextEdit
{
class RichTextEditFindBarPrivate;
class KPIMTEXTEDIT_EXPORT RichTextEditFindBar : public TextEditFindBarBase
{
    Q_OBJECT
public:
    explicit RichTextEditFindBar(QTextEdit *view, QWidget *parent = Q_NULLPTR);
    ~RichTextEditFindBar();

protected:
    bool viewIsReadOnly() const Q_DECL_OVERRIDE;
    bool documentIsEmpty() const Q_DECL_OVERRIDE;
    bool searchInDocument(const QString &text, QTextDocument::FindFlags searchOptions) Q_DECL_OVERRIDE;
    void autoSearchMoveCursor() Q_DECL_OVERRIDE;

public Q_SLOTS:
    void slotSearchText(bool backward = false, bool isAutoSearch = true) Q_DECL_OVERRIDE;

private Q_SLOTS:
    void slotReplaceText() Q_DECL_OVERRIDE;
    void slotReplaceAllText() Q_DECL_OVERRIDE;

private:
    RichTextEditFindBarPrivate *const d;
};

}

#endif // RICHTEXTEDITFINDBAR_H
