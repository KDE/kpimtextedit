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

#ifndef RICHTEXTEDITFINDBAR_H
#define RICHTEXTEDITFINDBAR_H

#include "kpimtextedit_export.h"
#include "texteditor/commonwidget/texteditfindbarbase.h"
#include <QTextDocument>

class QTextEdit;
namespace KPIMTextEdit {
class RichTextEditFindBarPrivate;
class KPIMTEXTEDIT_EXPORT RichTextEditFindBar : public TextEditFindBarBase
{
    Q_OBJECT
public:
    explicit RichTextEditFindBar(QTextEdit *view, QWidget *parent = nullptr);
    ~RichTextEditFindBar() override;

protected:
    bool viewIsReadOnly() const override;
    bool documentIsEmpty() const override;
    bool searchInDocument(const QString &text, QTextDocument::FindFlags searchOptions) override;
#if QT_VERSION < QT_VERSION_CHECK(5, 13, 0)
    bool searchInDocument(const QRegExp &regExp, QTextDocument::FindFlags searchOptions) override;
#else
    bool searchInDocument(const QRegularExpression &regExp, QTextDocument::FindFlags searchOptions) override;
#endif
    void autoSearchMoveCursor() override;

public Q_SLOTS:
    void slotSearchText(bool backward = false, bool isAutoSearch = true) override;

private Q_SLOTS:
    void slotReplaceText() override;
    void slotReplaceAllText() override;

private:
    RichTextEditFindBarPrivate *const d;
};
}

#endif // RICHTEXTEDITFINDBAR_H
