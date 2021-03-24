/*
   SPDX-FileCopyrightText: 2013-2021 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "kpimtextedit_export.h"
#include <QRegularExpression>
#include <QTextDocument>
#include <QWidget>
namespace KPIMTextEdit
{
class TextFindWidget;
class TextReplaceWidget;
/**
 * @brief The TextEditFindBarBase class
 * @author Laurent Montel <montel@kde.org>
 */
class KPIMTEXTEDIT_EXPORT TextEditFindBarBase : public QWidget
{
    Q_OBJECT

public:
    explicit TextEditFindBarBase(QWidget *parent = nullptr);
    ~TextEditFindBarBase() override;

    Q_REQUIRED_RESULT QString text() const;
    void setText(const QString &text);

    void focusAndSetCursor();

    void showReplace();
    void showFind();
    void setHideWhenClose(bool hide);

Q_SIGNALS:
    void displayMessageIndicator(const QString &message);
    void hideFindBar();

protected:
    virtual Q_REQUIRED_RESULT bool viewIsReadOnly() const = 0;
    virtual Q_REQUIRED_RESULT bool documentIsEmpty() const = 0;
    virtual Q_REQUIRED_RESULT bool searchInDocument(const QString &text, QTextDocument::FindFlags searchOptions) = 0;
    virtual Q_REQUIRED_RESULT bool searchInDocument(const QRegularExpression &regExp, QTextDocument::FindFlags searchOptions) = 0;
    virtual void autoSearchMoveCursor() = 0;

    bool event(QEvent *e) override;
    void clearSelections();
    bool searchText(bool backward, bool isAutoSearch);

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
    QRegularExpression mLastSearchRegExp;
    TextFindWidget *mFindWidget = nullptr;
    TextReplaceWidget *mReplaceWidget = nullptr;
    bool mHideWhenClose = true;
};
}

