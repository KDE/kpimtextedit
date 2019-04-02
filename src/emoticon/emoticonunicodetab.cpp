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
    setTabBarAutoHide(true);
    loadEmoticons();
}

EmoticonUnicodeTab::~EmoticonUnicodeTab()
{
}

void EmoticonUnicodeTab::loadEmoticons()
{
    if (mEmojiPlainText) {
        createPlainTextEmoticonTab(i18n("Faces"), KPIMTextEdit::TextUtils::unicodeFacesEmoji());
        createPlainTextEmoticonTab(i18n("Animals"), KPIMTextEdit::TextUtils::unicodeAnimalsEmoji());
        createPlainTextEmoticonTab(i18n("Emotions"), KPIMTextEdit::TextUtils::unicodeEmotionEmoji());
        createPlainTextEmoticonTab(i18n("Body"), KPIMTextEdit::TextUtils::unicodeBodyEmoji());
        createPlainTextEmoticonTab(i18n("Transports"), KPIMTextEdit::TextUtils::unicodeTransportEmoji());
        createPlainTextEmoticonTab(i18n("Events"), KPIMTextEdit::TextUtils::unicodeEventEmoji());
        createPlainTextEmoticonTab(i18n("Flags"), KPIMTextEdit::TextUtils::unicodeFlagsEmoji());
        createPlainTextEmoticonTab(i18n("Weather"), KPIMTextEdit::TextUtils::unicodeWeatherEmoji());
        createPlainTextEmoticonTab(i18n("Foods"), KPIMTextEdit::TextUtils::unicodeFoodEmoji());
    } else {
        createEmoticonTab(QString());
    }
}

void EmoticonUnicodeTab::createEmoticonTab(const QString &str)
{
    //Only one tab
    EmoticonListWidgetSelector *selector = new EmoticonListWidgetSelector(this);
    selector->loadEmotionsFromTheme();
    addTab(selector, str);
    connect(selector, &KPIMTextEdit::EmoticonListWidgetSelector::itemSelected, this, &EmoticonUnicodeTab::itemSelected);
}

void EmoticonUnicodeTab::createPlainTextEmoticonTab(const QString &str, const QList<uint> &emoticons)
{
    EmoticonListWidgetSelector *selector = new EmoticonListWidgetSelector(this);
    connect(selector, &KPIMTextEdit::EmoticonListWidgetSelector::itemSelected, this, &EmoticonUnicodeTab::itemSelected);
    selector->setEmoticons(emoticons);
    const QString strTab = QString::fromUcs4(&emoticons.at(0), 1);
    const int index = addTab(selector, strTab);
    if (!str.isEmpty()) {
        setTabToolTip(index, str);
    }
}

bool EmoticonUnicodeTab::emojiPlainText() const
{
    return mEmojiPlainText;
}

void EmoticonUnicodeTab::setEmojiPlainText(bool emojiPlainText)
{
    if (mEmojiPlainText != emojiPlainText) {
        mEmojiPlainText = emojiPlainText;
        clear();
    }
}
