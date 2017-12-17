/*
   Copyright (C) 2013-2017 Laurent Montel <montel@kde.org>

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

#ifndef TEXTEDITFINDBARBASE_H
#define TEXTEDITFINDBARBASE_H

#include "kpimtextedit_export.h"
#include <QWidget>
#include <QTextDocument>

namespace KPIMTextEdit {
class TextFindWidget;
class TextReplaceWidget;
class KPIMTEXTEDIT_EXPORT TextEditFindBarBase : public QWidget
{
    Q_OBJECT

public:
    explicit TextEditFindBarBase(QWidget *parent = nullptr);
    ~TextEditFindBarBase();

    QString text() const;
    void setText(const QString &text);

    void focusAndSetCursor();

    void showReplace();
    void showFind();
    void setHideWhenClose(bool hide);

Q_SIGNALS:
    void displayMessageIndicator(const QString &message);
    void hideFindBar();

protected:
    virtual bool viewIsReadOnly() const = 0;
    virtual bool documentIsEmpty() const = 0;
    virtual bool searchInDocument(const QString &text, QTextDocument::FindFlags searchOptions) = 0;
    virtual bool searchInDocument(const QRegExp &regExp, QTextDocument::FindFlags searchOptions) = 0;
    virtual void autoSearchMoveCursor() = 0;

    bool event(QEvent *e) override;
    void clearSelections();
    void updateHighLight(bool);
    bool searchText(bool backward, bool isAutoSearch);
    void updateSensitivity(bool);

    void setFoundMatch(bool match);
    void messageInfo(bool backward, bool isAutoSearch, bool found);

public Q_SLOTS:
    void findNext();
    void findPrev();
    void autoSearch(const QString &str);
    virtual void slotSearchText(bool backward = false, bool isAutoSearch = true) = 0;
    void closeBar();

private Q_SLOTS:
    void slotClearSearch();
    void slotUpdateSearchOptions();
    virtual void slotReplaceText() = 0;
    virtual void slotReplaceAllText() = 0;

protected:
    QString mLastSearchStr;
    QRegExp mLastSearchRegExp;
    TextFindWidget *mFindWidget = nullptr;
    TextReplaceWidget *mReplaceWidget = nullptr;
    bool mHideWhenClose = true;
};
}

#endif // TEXTEDITFINDBARBASE_H
