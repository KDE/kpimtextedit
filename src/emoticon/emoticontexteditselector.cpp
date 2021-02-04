/*
  SPDX-FileCopyrightText: 2012-2021 Laurent Montel <montel@kde.org>
  based on code from kopete

  SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "emoticontexteditselector.h"
#include "emoticonunicodetab.h"
#include "textutils.h"

#include <QHBoxLayout>

using namespace KPIMTextEdit;

EmoticonTextEditSelector::EmoticonTextEditSelector(QWidget *parent)
    : QWidget(parent)
{
    auto lay = new QHBoxLayout(this);
    lay->setSpacing(0);
    lay->setContentsMargins(0, 0, 0, 0);
    mUnicodeTab = new EmoticonUnicodeTab(this);
    lay->addWidget(mUnicodeTab);
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
