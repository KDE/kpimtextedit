/*
   SPDX-FileCopyrightText: 2013-2021 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include <QTextDocument>
#include <QWidget>

class QAction;
class QLineEdit;
class QPushButton;
namespace KPIMTextEdit
{
class TextFindWidget : public QWidget
{
    Q_OBJECT
public:
    explicit TextFindWidget(QWidget *parent = nullptr);
    ~TextFindWidget() override;

    Q_REQUIRED_RESULT QTextDocument::FindFlags searchOptions() const;

    QLineEdit *searchLineEdit() const;

    void setFoundMatch(bool match);

    Q_REQUIRED_RESULT bool isRegularExpression() const;
    Q_REQUIRED_RESULT QRegularExpression searchRegExp() const;

    Q_REQUIRED_RESULT QString searchText() const;

private Q_SLOTS:
    void slotAutoSearch(const QString &str);

Q_SIGNALS:
    void findNext();
    void findPrev();
    void clearSearch();
    void autoSearch(const QString &);
    void updateSearchOptions();
    void searchStringEmpty(bool);

private:
    QLineEdit *mSearch = nullptr;
    QAction *mCaseSensitiveAct = nullptr;
    QAction *mWholeWordAct = nullptr;
    QAction *mRegExpAct = nullptr;

    QPushButton *mFindPrevBtn = nullptr;
    QPushButton *mFindNextBtn = nullptr;
};

class TextReplaceWidget : public QWidget
{
    Q_OBJECT
public:
    explicit TextReplaceWidget(QWidget *parent = nullptr);
    ~TextReplaceWidget();

    QLineEdit *replaceLineEdit() const;

public Q_SLOTS:
    void slotSearchStringEmpty(bool);

Q_SIGNALS:
    void replaceText();
    void replaceAllText();

private:
    QLineEdit *mReplace = nullptr;
    QPushButton *mReplaceBtn = nullptr;
    QPushButton *mReplaceAllBtn = nullptr;
};
}
