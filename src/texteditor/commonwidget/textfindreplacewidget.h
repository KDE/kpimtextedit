/*
   SPDX-FileCopyrightText: 2013-2020 Laurent Montel <montel@kde.org>

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

#ifndef TEXTFINDREPLACEWIDGET_H
#define TEXTFINDREPLACEWIDGET_H

#include <QWidget>
#include <QTextDocument>

class QAction;
class QLineEdit;
class QPushButton;
namespace KPIMTextEdit {
class TextFindWidget : public QWidget
{
    Q_OBJECT
public:
    explicit TextFindWidget(QWidget *parent = nullptr);
    ~TextFindWidget();

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
#endif // TEXTFINDREPLACEWIDGET_H
