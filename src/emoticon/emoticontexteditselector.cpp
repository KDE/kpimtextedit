/*
  SPDX-FileCopyrightText: 2012-2021 Laurent Montel <montel@kde.org>
  based on code from kopete

  SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "emoticontexteditselector.h"
#include "emoticonunicodetab.h"
#include "textutils.h"

#include <QVBoxLayout>

using namespace KPIMTextEdit;

EmoticonTextEditSelector::EmoticonTextEditSelector(QWidget *parent)
    : QWidget(parent)
    , mUnicodeTab(new EmoticonUnicodeTab(this))
{
    auto mainLayout = new QVBoxLayout(this);
    mainLayout->setObjectName(QStringLiteral("mainLayout"));
    mainLayout->setSpacing(0);
    mainLayout->setContentsMargins({});
    mainLayout->addWidget(mUnicodeTab);
    mUnicodeTab->setObjectName(QStringLiteral("mUnicodeTab"));
    connect(mUnicodeTab, &EmoticonUnicodeTab::itemSelected, this, &EmoticonTextEditSelector::slotItemSelected);
}

EmoticonTextEditSelector::~EmoticonTextEditSelector()
{
}

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
