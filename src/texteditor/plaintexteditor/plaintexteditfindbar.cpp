/*
   SPDX-FileCopyrightText: 2012-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "plaintexteditfindbar.h"
#include "texteditor/commonwidget/findutils.h"
#include "texteditor/commonwidget/textfindreplacewidget.h"

#include <KLocalizedString>
#include <TextUtils/ConvertText>

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

PlainTextEditFindBar::~PlainTextEditFindBar() = default;

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

bool PlainTextEditFindBar::searchInDocument(const QString &text, TextEditFindBarBase::FindFlags searchOptions)
{
    bool found = false;
    if (searchOptions & TextEditFindBarBase::FindRespectDiacritics) {
        found = d->mView->find(text, FindUtils::convertTextEditFindFlags(searchOptions));
    } else {
        found = FindUtils::find(d->mView, text, FindUtils::convertTextEditFindFlags(searchOptions));
    }
    mFindWidget->setFoundMatch(found);
    return found;
}

bool PlainTextEditFindBar::searchInDocument(const QRegularExpression &regExp, TextEditFindBarBase::FindFlags searchOptions)
{
    const bool found = d->mView->find(regExp, FindUtils::convertTextEditFindFlags(searchOptions));
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
    const TextEditFindBarBase::FindFlags searchOptions = mFindWidget->searchOptions();
    if (d->mView->textCursor().hasSelection()) {
        if (mFindWidget->isRegularExpression()) {
            if (d->mView->textCursor().selectedText().contains(mFindWidget->searchRegularExpression())) {
                d->mView->textCursor().insertText(mReplaceWidget->replaceLineEdit()->text());
                // search next after replace text.
                searchText(false, false);
            }
        } else {
            if (searchOptions & TextEditFindBarBase::FindRespectDiacritics) {
                if (TextUtils::ConvertText::normalize(d->mView->textCursor().selectedText()) == TextUtils::ConvertText::normalize(mFindWidget->searchText())) {
                    d->mView->textCursor().insertText(mReplaceWidget->replaceLineEdit()->text());
                    // search next after replace text.
                    searchText(false, false);
                } else {
                    if (d->mView->textCursor().selectedText() == mFindWidget->searchText()) {
                        d->mView->textCursor().insertText(mReplaceWidget->replaceLineEdit()->text());
                        // search next after replace text.
                        searchText(false, false);
                    }
                }
            }
        }
    } else {
        searchText(false, false);
    }
}

void PlainTextEditFindBar::slotReplaceAllText()
{
    int count = 0;
    const QString replaceStr{mReplaceWidget->replaceLineEdit()->text()};
    const TextEditFindBarBase::FindFlags searchOptions{mFindWidget->searchOptions()};
    if (mFindWidget->isRegularExpression()) {
        count = FindUtils::replaceAll(d->mView->document(), mFindWidget->searchRegularExpression(), replaceStr, searchOptions);
    } else {
        count = FindUtils::replaceAll(d->mView, mFindWidget->searchText(), replaceStr, searchOptions);
    }
    Q_EMIT displayMessageIndicator(i18np("%1 replacement made", "%1 replacements made", count));
}

#include "moc_plaintexteditfindbar.cpp"
