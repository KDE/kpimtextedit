/*
   SPDX-FileCopyrightText: 2012-2021 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "plaintexteditfindbar.h"
#include "texteditor/commonwidget/findutils.h"
#include "texteditor/commonwidget/textfindreplacewidget.h"

#include <KColorScheme>
#include <KLocalizedString>

#include <QLineEdit>
#include <QPlainTextEdit>
#include <QRegularExpression>

using namespace KPIMTextEdit;

class KPIMTextEdit::PlainTextEditFindBarPrivate
{
public:
    explicit PlainTextEditFindBarPrivate(QPlainTextEdit *view)
        : mView(view)
    {
    }

    QPlainTextEdit *const mView;
};

PlainTextEditFindBar::PlainTextEditFindBar(QPlainTextEdit *view, QWidget *parent)
    : TextEditFindBarBase(parent)
    , d(new KPIMTextEdit::PlainTextEditFindBarPrivate(view))
{
}

PlainTextEditFindBar::~PlainTextEditFindBar()
{
    delete d;
}

void PlainTextEditFindBar::slotSearchText(bool backward, bool isAutoSearch)
{
    d->mView->moveCursor(QTextCursor::Start);
    searchText(backward, isAutoSearch);
}

bool PlainTextEditFindBar::viewIsReadOnly() const
{
    return d->mView->isReadOnly();
}

bool PlainTextEditFindBar::documentIsEmpty() const
{
    return d->mView->document()->isEmpty();
}

bool PlainTextEditFindBar::searchInDocument(const QString &text, QTextDocument::FindFlags searchOptions)
{
    const bool found = d->mView->find(text, searchOptions);
    mFindWidget->setFoundMatch(found);
    return found;
}

bool PlainTextEditFindBar::searchInDocument(const QRegularExpression &regExp, QTextDocument::FindFlags searchOptions)
{
    const bool found = d->mView->find(regExp, searchOptions);
    mFindWidget->setFoundMatch(found);
    return found;
}

void PlainTextEditFindBar::autoSearchMoveCursor()
{
    QTextCursor cursor = d->mView->textCursor();
    cursor.setPosition(cursor.selectionStart());
    d->mView->setTextCursor(cursor);
}

void PlainTextEditFindBar::slotReplaceText()
{
    if (d->mView->textCursor().hasSelection()) {
        if (mFindWidget->isRegularExpression()) {
            if (d->mView->textCursor().selectedText().contains(mFindWidget->searchRegExp())) {
                d->mView->textCursor().insertText(mReplaceWidget->replaceLineEdit()->text());
                // search next after replace text.
                searchText(false, false);
            }
        } else {
            if (d->mView->textCursor().selectedText() == mFindWidget->searchText()) {
                d->mView->textCursor().insertText(mReplaceWidget->replaceLineEdit()->text());
                // search next after replace text.
                searchText(false, false);
            }
        }
    } else {
        searchText(false, false);
    }
}

void PlainTextEditFindBar::slotReplaceAllText()
{
    const int count = FindUtils::replaceAll(d->mView->document(), mFindWidget, mReplaceWidget);
    Q_EMIT displayMessageIndicator(i18np("%1 replacement made", "%1 replacements made", count));
}
