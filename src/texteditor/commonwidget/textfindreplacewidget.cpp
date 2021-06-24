/*
   SPDX-FileCopyrightText: 2013-2021 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "textfindreplacewidget.h"

#include <KColorScheme>
#include <KLocalizedString>
#include <QPushButton>

#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QMenu>
#include <QRegularExpression>

using namespace KPIMTextEdit;

TextReplaceWidget::TextReplaceWidget(QWidget *parent)
    : QWidget(parent)
{
    auto lay = new QHBoxLayout(this);
    QLabel *label = new QLabel(i18nc("Replace text", "Replace:"), this);
    lay->addWidget(label);
    const int marg1 = lay->contentsMargins().left();
    const int marg2 = lay->contentsMargins().right();
    lay->setContentsMargins(marg1, 0, marg2, 0);

    mReplace = new QLineEdit(this);
    mReplace->setClearButtonEnabled(true);
    lay->addWidget(mReplace);

    mReplaceBtn = new QPushButton(i18n("Replace"), this);
    connect(mReplaceBtn, &QPushButton::clicked, this, &TextReplaceWidget::replaceText);
    lay->addWidget(mReplaceBtn);

    mReplaceAllBtn = new QPushButton(i18n("Replace All"), this);
    connect(mReplaceAllBtn, &QPushButton::clicked, this, &TextReplaceWidget::replaceAllText);
    lay->addWidget(mReplaceAllBtn);
}

TextReplaceWidget::~TextReplaceWidget()
{
}

QLineEdit *TextReplaceWidget::replaceLineEdit() const
{
    return mReplace;
}

void TextReplaceWidget::slotSearchStringEmpty(bool isEmpty)
{
    mReplaceBtn->setDisabled(isEmpty);
    mReplaceAllBtn->setDisabled(isEmpty);
}

TextFindWidget::TextFindWidget(QWidget *parent)
    : QWidget(parent)
{
    auto lay = new QHBoxLayout(this);
    const int marg1 = lay->contentsMargins().left();
    const int marg2 = lay->contentsMargins().right();
    lay->setContentsMargins(marg1, 0, marg2, 0);
    QLabel *label = new QLabel(i18nc("Find text", "F&ind:"), this);
    lay->addWidget(label);

    mSearch = new QLineEdit(this);
    mSearch->setToolTip(i18n("Text to search for"));
    mSearch->setClearButtonEnabled(true);
    label->setBuddy(mSearch);
    lay->addWidget(mSearch);

    mFindNextBtn = new QPushButton(QIcon::fromTheme(QStringLiteral("go-down-search")), i18nc("Find and go to the next search match", "Next"), this);
    mFindNextBtn->setToolTip(i18n("Jump to next match"));
    lay->addWidget(mFindNextBtn);
    mFindNextBtn->setEnabled(false);

    mFindPrevBtn = new QPushButton(QIcon::fromTheme(QStringLiteral("go-up-search")), i18nc("Find and go to the previous search match", "Previous"), this);
    mFindPrevBtn->setToolTip(i18n("Jump to previous match"));
    lay->addWidget(mFindPrevBtn);
    mFindPrevBtn->setEnabled(false);

    auto optionsBtn = new QPushButton(this);
    optionsBtn->setText(i18n("Options"));
    optionsBtn->setToolTip(i18n("Modify search behavior"));
    auto optionsMenu = new QMenu(optionsBtn);
    mCaseSensitiveAct = optionsMenu->addAction(i18n("Case sensitive"));
    mCaseSensitiveAct->setCheckable(true);

    mWholeWordAct = optionsMenu->addAction(i18n("Whole word"));
    mWholeWordAct->setCheckable(true);

    mRegExpAct = optionsMenu->addAction(i18n("Regular Expression"));
    mRegExpAct->setCheckable(true);

    optionsBtn->setMenu(optionsMenu);
    lay->addWidget(optionsBtn);

    connect(mFindNextBtn, &QPushButton::clicked, this, &TextFindWidget::findNext);
    connect(mFindPrevBtn, &QPushButton::clicked, this, &TextFindWidget::findPrev);
    connect(mCaseSensitiveAct, &QAction::toggled, this, &TextFindWidget::updateSearchOptions);
    connect(mWholeWordAct, &QAction::toggled, this, &TextFindWidget::updateSearchOptions);
    connect(mRegExpAct, &QAction::toggled, this, &TextFindWidget::updateSearchOptions);
    connect(mSearch, &QLineEdit::textChanged, this, &TextFindWidget::slotAutoSearch);
    connect(mSearch, &QLineEdit::returnPressed, this, &TextFindWidget::findNext);
}

TextFindWidget::~TextFindWidget()
{
}

void TextFindWidget::setFoundMatch(bool match)
{
#ifndef QT_NO_STYLE_STYLESHEET
    QString styleSheet;

    if (!mSearch->text().isEmpty()) {
        KColorScheme::BackgroundRole bgColorScheme;

        if (match) {
            bgColorScheme = KColorScheme::PositiveBackground;
        } else {
            bgColorScheme = KColorScheme::NegativeBackground;
        }

        KStatefulBrush bgBrush(KColorScheme::View, bgColorScheme);

        styleSheet = QStringLiteral("QLineEdit{ background-color:%1 }").arg(bgBrush.brush(mSearch->palette()).color().name());
    }

    mSearch->setStyleSheet(styleSheet);
#endif
}

void TextFindWidget::slotAutoSearch(const QString &str)
{
    const bool isNotEmpty = (!str.isEmpty());
    mFindPrevBtn->setEnabled(isNotEmpty);
    mFindNextBtn->setEnabled(isNotEmpty);
    Q_EMIT searchStringEmpty(!isNotEmpty);
    Q_EMIT autoSearch(str);
    if (str.isEmpty()) {
        Q_EMIT clearSearch();
    }
}

QLineEdit *TextFindWidget::searchLineEdit() const
{
    return mSearch;
}

bool TextFindWidget::isRegularExpression() const
{
    return mRegExpAct->isChecked();
}

QString TextFindWidget::searchText() const
{
    return mSearch->text();
}

QRegularExpression TextFindWidget::searchRegExp() const
{
    QRegularExpression reg;
    if (!mCaseSensitiveAct->isChecked()) {
        reg.setPatternOptions(QRegularExpression::CaseInsensitiveOption);
    }
    QString searchTextString = mSearch->text();
    if (mWholeWordAct->isChecked()) {
        searchTextString = QLatin1String("\\b") + searchTextString + QLatin1String("\\b");
    }
    reg.setPattern(searchTextString);
    return reg;
}

QTextDocument::FindFlags TextFindWidget::searchOptions() const
{
    QTextDocument::FindFlags opt = {};
    if (mCaseSensitiveAct->isChecked()) {
        opt |= QTextDocument::FindCaseSensitively;
    }
    if (mWholeWordAct->isChecked()) {
        opt |= QTextDocument::FindWholeWords;
    }
    return opt;
}
