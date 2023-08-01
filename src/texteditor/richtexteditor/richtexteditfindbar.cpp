/*
   SPDX-FileCopyrightText: 2013-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "richtexteditfindbar.h"
#include "texteditor/commonwidget/findutils.h"
#include "texteditor/commonwidget/textfindreplacewidget.h"

#include <KLocalizedString>
#include <TextUtils/ConvertText>

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

RichTextEditFindBar::~RichTextEditFindBar() = default;

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

bool RichTextEditFindBar::searchInDocument(const QString &text, TextEditFindBarBase::FindFlags searchOptions)
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

bool RichTextEditFindBar::searchInDocument(const QRegularExpression &regExp, TextEditFindBarBase::FindFlags searchOptions)
{
    const bool found = d->mView->find(regExp, FindUtils::convertTextEditFindFlags(searchOptions));
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

void RichTextEditFindBar::slotReplaceAllText()
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

#include "moc_richtexteditfindbar.cpp"
