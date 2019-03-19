/*
  Copyright (c) 2019 Montel Laurent <montel@kde.org>

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

#include "emoticonunicodetab.h"
#include "emoticonlistwidgetselector.h"
#include "textutils.h"
#include <KLocalizedString>

using namespace KPIMTextEdit;
EmoticonUnicodeTab::EmoticonUnicodeTab(QWidget *parent)
    : QTabWidget(parent)
{
    loadEmoticons();
}

EmoticonUnicodeTab::~EmoticonUnicodeTab()
{
}

void EmoticonUnicodeTab::loadEmoticons()
{
    createTab(i18n("Faces"), KPIMTextEdit::TextUtils::unicodeFacesEmoji());
    //TODO add more
}

void EmoticonUnicodeTab::createTab(const QString &str, const QList<uint> &emoticons)
{
    EmoticonListWidgetSelector *selector = new EmoticonListWidgetSelector(this);
    selector->setEmoticons(emoticons);
    //TODO add i18n ? or icons ?
    addTab(selector, str);
}
