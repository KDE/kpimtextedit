/*
  Copyright (c) 2012-2019 Montel Laurent <montel@kde.org>
  based on code from kopete

  This library is free software; you can redistribute it and/or modify it
  under the terms of the GNU Library General Public License as published by
  the Free Software Foundation; either version 2 of the License, or (at your
  option) any later version.

  This library is distributed in the hope that it will be useful, but WITHOUT
  ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
  FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Library General Public
  License for more details.

  You should have received a copy of the GNU Library General Public License
  along with this library; see the file COPYING.LIB.  If not, write to the
  Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA
  02110-1301, USA.
*/

#include "emoticontexteditselector.h"
#include "emoticonunicodetab.h"
#include "textutils.h"
#include <kemoticonstheme.h>

#include <QHBoxLayout>

using namespace KPIMTextEdit;

EmoticonTextEditSelector::EmoticonTextEditSelector(QWidget *parent)
    : QWidget(parent)
{
    QHBoxLayout *lay = new QHBoxLayout(this);
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
    if (isVisible() && parentWidget()
        && parentWidget()->inherits("QMenu")) {
        parentWidget()->close();
    }
}

void EmoticonTextEditSelector::loadEmoticons()
{
    if (mUnicodeTab->count() == 0) {
        mUnicodeTab->loadEmoticons();
    }
}
