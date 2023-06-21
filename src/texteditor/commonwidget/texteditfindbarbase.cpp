/*
   SPDX-FileCopyrightText: 2013-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "texteditfindbarbase.h"
#include "kpimtextedit_debug.h"
#include "texteditor/commonwidget/textfindreplacewidget.h"
#include <KLocalizedString>
#include <QEvent>
#include <QHBoxLayout>
#include <QIcon>
#include <QKeyEvent>
#include <QLineEdit>
#include <QTimer>
#include <QToolButton>
#include <QVBoxLayout>

using namespace KPIMTextEdit;

TextEditFindBarBase::TextEditFindBarBase(QWidget *parent)
    : QWidget(parent)
    , mFindWidget(new TextFindWidget(this))
    , mReplaceWidget(new TextReplaceWidget(this))
{
    auto topLayout = new QVBoxLayout(this);
    auto lay = new QHBoxLayout;
    topLayout->addLayout(lay);

    auto closeBtn = new QToolButton(this);
    closeBtn->setIcon(QIcon::fromTheme(QStringLiteral("dialog-close")));
    closeBtn->setIconSize(QSize(16, 16));
    closeBtn->setToolTip(i18n("Close"));

#ifndef QT_NO_ACCESSIBILITY
    closeBtn->setAccessibleName(i18n("Close"));
#endif

    closeBtn->setAutoRaise(true);
    lay->addWidget(closeBtn);

    lay->addWidget(mFindWidget);

    topLayout->addWidget(mReplaceWidget);
    mReplaceWidget->hide();

    connect(closeBtn, &QToolButton::clicked, this, &TextEditFindBarBase::closeBar);
    connect(mFindWidget, &TextFindWidget::findNext, this, &TextEditFindBarBase::findNext);
    connect(mFindWidget, &TextFindWidget::findPrev, this, &TextEditFindBarBase::findPrev);
    connect(mFindWidget, &TextFindWidget::updateSearchOptions, this, &TextEditFindBarBase::slotUpdateSearchOptions);
    connect(mFindWidget, &TextFindWidget::autoSearch, this, &TextEditFindBarBase::autoSearch);
    connect(mFindWidget, &TextFindWidget::clearSearch, this, &TextEditFindBarBase::slotClearSearch);
    connect(mFindWidget, &TextFindWidget::searchStringEmpty, mReplaceWidget, &TextReplaceWidget::slotSearchStringEmpty);
    connect(mReplaceWidget, &TextReplaceWidget::replaceText, this, &TextEditFindBarBase::slotReplaceText);
    connect(mReplaceWidget, &TextReplaceWidget::replaceAllText, this, &TextEditFindBarBase::slotReplaceAllText);
    setSizePolicy(QSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed));
    hide();
}

TextEditFindBarBase::~TextEditFindBarBase() = default;

void TextEditFindBarBase::showFind()
{
    if (documentIsEmpty()) {
        return;
    }
    mReplaceWidget->slotSearchStringEmpty(mFindWidget->searchText().isEmpty());
    show();
    if (mReplaceWidget->isVisible()) {
        mReplaceWidget->hide();
        updateGeometry();
    }
}

void TextEditFindBarBase::setHideWhenClose(bool hide)
{
    mHideWhenClose = hide;
}

void TextEditFindBarBase::showReplace()
{
    if (viewIsReadOnly()) {
        return;
    }
    if (documentIsEmpty()) {
        return;
    }
    mReplaceWidget->slotSearchStringEmpty(mFindWidget->searchText().isEmpty());
    show();
    if (!mReplaceWidget->isVisible()) {
        mReplaceWidget->show();
        updateGeometry();
    }
}

void TextEditFindBarBase::setText(const QString &text)
{
    mFindWidget->searchLineEdit()->setText(text);
}

QString TextEditFindBarBase::text() const
{
    return mFindWidget->searchText();
}

void TextEditFindBarBase::focusAndSetCursor()
{
    setFocus();
    mFindWidget->searchLineEdit()->selectAll();
    mFindWidget->searchLineEdit()->setFocus();
}

void TextEditFindBarBase::slotClearSearch()
{
    clearSelections();
}

void TextEditFindBarBase::autoSearch(const QString &str)
{
    const bool isNotEmpty = (!str.isEmpty());
    if (isNotEmpty) {
        QTimer::singleShot(0, this, [this]() {
            slotSearchText();
        });
    } else {
        clearSelections();
    }
}

void TextEditFindBarBase::messageInfo(bool backward, bool isAutoSearch, bool found)
{
    Q_UNUSED(backward)
    if (!found && !isAutoSearch) {
        QString str = mLastSearchStr;
        if (str.length() > 39) {
            str.truncate(40);
            str += QLatin1String("...");
        }
        Q_EMIT displayMessageIndicator(i18n("End of message reached.\nPhrase '%1' could not be found.", str));
    }
}

bool TextEditFindBarBase::searchText(bool backward, bool isAutoSearch)
{
    mLastSearchStr = mFindWidget->searchText();
    mLastSearchRegExp = mFindWidget->searchRegularExpression();
    TextEditFindBarBase::FindFlags searchOptions = mFindWidget->searchOptions();
    if (backward) {
        searchOptions |= TextEditFindBarBase::FindBackward;
    }

    if (isAutoSearch) {
        autoSearchMoveCursor();
    } else if (mFindWidget->isRegularExpression()) {
        // TODO
    } else if (!mLastSearchStr.contains(mFindWidget->searchText(), Qt::CaseSensitive)) {
        clearSelections();
    }

    bool found = false;
    if (mFindWidget->isRegularExpression()) {
        found = searchInDocument(mLastSearchRegExp, searchOptions);
    } else {
        found = searchInDocument(mLastSearchStr, searchOptions);
    }
    mFindWidget->setFoundMatch(found);
    messageInfo(backward, isAutoSearch, found);
    return found;
}

void TextEditFindBarBase::findNext()
{
    searchText(false, false);
}

void TextEditFindBarBase::findPrev()
{
    searchText(true, false);
}

void TextEditFindBarBase::slotUpdateSearchOptions()
{
    const TextEditFindBarBase::FindFlags searchOptions = mFindWidget->searchOptions();
    mLastSearchStr = mFindWidget->searchText();
    mLastSearchRegExp = mFindWidget->searchRegularExpression();
    if (mFindWidget->isRegularExpression()) {
        searchInDocument(mLastSearchRegExp, searchOptions);
    } else {
        searchInDocument(mLastSearchStr, searchOptions);
    }
}

void TextEditFindBarBase::clearSelections()
{
    mFindWidget->setFoundMatch(false);
}

void TextEditFindBarBase::closeBar()
{
    // Make sure that all old searches are cleared
    mFindWidget->searchLineEdit()->setText(QString());
    mReplaceWidget->replaceLineEdit()->setText(QString());
    clearSelections();
    mReplaceWidget->hide();
    updateGeometry();
    if (mHideWhenClose) {
        hide();
    }
    Q_EMIT hideFindBar();
}

bool TextEditFindBarBase::event(QEvent *e)
{
    // Close the bar when pressing Escape.
    // Not using a QShortcut for this because it could conflict with
    // window-global actions (e.g. Emil Sedgh binds Esc to "close tab").
    // With a shortcut override we can catch this before it gets to kactions.
    const bool shortCutOverride = (e->type() == QEvent::ShortcutOverride);
    if (shortCutOverride || e->type() == QEvent::KeyPress) {
        auto kev = static_cast<QKeyEvent *>(e);
        if (kev->key() == Qt::Key_Escape) {
            if (shortCutOverride) {
                e->accept();
                return true;
            }
            e->accept();
            closeBar();
            return true;
        } else if (kev->key() == Qt::Key_Enter || kev->key() == Qt::Key_Return) {
            e->accept();
            if (shortCutOverride) {
                return true;
            }
            if (mFindWidget->searchText().isEmpty()) {
                return true;
            }

            if (kev->modifiers() & Qt::ShiftModifier) {
                findPrev();
            } else if (kev->modifiers() == Qt::NoModifier) {
                findNext();
            }
            return true;
        }
    }
    return QWidget::event(e);
}

#include "moc_texteditfindbarbase.cpp"
