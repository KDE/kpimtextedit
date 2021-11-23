/*
  SPDX-FileCopyrightText: 2012-2021 Laurent Montel <montel@kde.org>
  based on code from kopete

  SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "emoticontexteditselector.h"
#include "emoticonunicodetab.h"
#include "textutils.h"

#include <KLocalizedString>

#include <QLineEdit>
#include <QVBoxLayout>

using namespace KPIMTextEdit;

EmoticonTextEditSelector::EmoticonTextEditSelector(QWidget *parent)
    : QWidget(parent)
    , mUnicodeTab(new EmoticonUnicodeTab(this))
    , mSearchUnicodeLineEdit(new QLineEdit(this))
{
    auto mainLayout = new QVBoxLayout(this);
    mainLayout->setObjectName(QStringLiteral("mainLayout"));
    mainLayout->setSpacing(0);
    mainLayout->setContentsMargins({});
    mUnicodeTab->setObjectName(QStringLiteral("mUnicodeTab"));
    mSearchUnicodeLineEdit->setObjectName(QStringLiteral("mSearchUnicodeLineEdit"));
    mSearchUnicodeLineEdit->setClearButtonEnabled(true);
    mSearchUnicodeLineEdit->setPlaceholderText(i18n("Search Emoticon..."));
    mainLayout->addWidget(mSearchUnicodeLineEdit);

    mainLayout->addWidget(mUnicodeTab);
    connect(mUnicodeTab, &EmoticonUnicodeTab::itemSelected, this, &EmoticonTextEditSelector::slotItemSelected);
    connect(mSearchUnicodeLineEdit, &QLineEdit::textChanged, this, &EmoticonTextEditSelector::slotSearchUnicode);
    setMinimumSize(400, 200);
}

EmoticonTextEditSelector::~EmoticonTextEditSelector() = default;

void EmoticonTextEditSelector::slotItemSelected(const QString &str)
{
    Q_EMIT itemSelected(str);
    if (isVisible() && parentWidget() && parentWidget()->inherits("QMenu")) {
        parentWidget()->close();
    }
}

void EmoticonTextEditSelector::loadEmoticons()
{
    if (mUnicodeTab->count() == 0) {
        mUnicodeTab->loadEmoticons();
    }
}

void EmoticonTextEditSelector::slotSearchUnicode(const QString &str)
{
    mUnicodeTab->searchUnicode(str);
}
