/*
   SPDX-FileCopyrightText: 2013-2021 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "richtexteditfindbar.h"
#include "texteditor/commonwidget/findutils.h"
#include "texteditor/commonwidget/textfindreplacewidget.h"

#include <KColorScheme>
#include <KLocalizedString>

#include <QLineEdit>
#include <QRegularExpression>
#include <QTextEdit>

using namespace KPIMTextEdit;
class Q_DECL_HIDDEN KPIMTextEdit::RichTextEditFindBarPrivate
{
public:
    RichTextEditFindBarPrivate(QTextEdit *view)
        : mView(view)
    {
    }

    QTextEdit *const mView;
};

RichTextEditFindBar::RichTextEditFindBar(QTextEdit *view, QWidget *parent)
    : TextEditFindBarBase(parent)
    , d(new KPIMTextEdit::RichTextEditFindBarPrivate(view))
{
}

RichTextEditFindBar::~RichTextEditFindBar()
{
    delete d;
}

void RichTextEditFindBar::slotSearchText(bool backward, bool isAutoSearch)
{
    d->mView->moveCursor(QTextCursor::Start);
    searchText(backward, isAutoSearch);
}

bool RichTextEditFindBar::viewIsReadOnly() const
{
    return d->mView->isReadOnly();
}

bool RichTextEditFindBar::documentIsEmpty() const
{
    return d->mView->document()->isEmpty();
}

bool RichTextEditFindBar::searchInDocument(const QString &text, QTextDocument::FindFlags searchOptions)
{
    const bool found = d->mView->find(text, searchOptions);
    mFindWidget->setFoundMatch(found);
    return found;
}

bool RichTextEditFindBar::searchInDocument(const QRegularExpression &regExp, QTextDocument::FindFlags searchOptions)
{
    const bool found = d->mView->find(regExp, searchOptions);
    mFindWidget->setFoundMatch(found);
    return found;
}

void RichTextEditFindBar::autoSearchMoveCursor()
{
    QTextCursor cursor = d->mView->textCursor();
    cursor.setPosition(cursor.selectionStart());
    d->mView->setTextCursor(cursor);
}

void RichTextEditFindBar::slotReplaceText()
{
    // FIXME!
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

void RichTextEditFindBar::slotReplaceAllText()
{
    const int count = FindUtils::replaceAll(d->mView->document(), mFindWidget, mReplaceWidget);
    Q_EMIT displayMessageIndicator(i18np("%1 replacement made", "%1 replacements made", count));
}
