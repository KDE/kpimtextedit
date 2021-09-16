/*
  SPDX-FileCopyrightText: 2019-2021 Laurent Montel <montel@kde.org>

  SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "emoticonunicodetab.h"
#include "emoticonlistview.h"
#include "emoticonlistwidgetselector.h"
#include "emoticonunicodemodel.h"
#include "emoticonunicodeproxymodel.h"
#include "textutils.h"
#include <KLocalizedString>
#include <QDebug>

using namespace KPIMTextEdit;
EmoticonUnicodeTab::EmoticonUnicodeTab(QWidget *parent)
    : QTabWidget(parent)
{
    setTabBarAutoHide(true);
    loadEmoticons();
    QFont f;
    f.setPointSize(22);
    f.setFamily(QStringLiteral("NotoColorEmoji"));
    setFont(f);
}

EmoticonUnicodeTab::~EmoticonUnicodeTab()
{
}

void EmoticonUnicodeTab::searchUnicode(const QString &str)
{
    bool searchVisible = !str.trimmed().isEmpty();
    setTabVisible(mSearchTabIndex, searchVisible);
    if (searchVisible) {
        setCurrentIndex(mSearchTabIndex);
    }
    mEmoticonUnicodeProxyModel->setFilterFixedString(str);
}

void EmoticonUnicodeTab::createSearchTab()
{
    auto allEmojisView = new EmoticonListView(this);
    auto emoticonModel = new EmoticonUnicodeModel(this);
    mEmoticonUnicodeProxyModel = new EmoticonUnicodeProxyModel(this);
    mEmoticonUnicodeProxyModel->setSourceModel(emoticonModel);
    emoticonModel->setEmoticonList(EmoticonUnicodeUtils::allUnicode());
    allEmojisView->setModel(mEmoticonUnicodeProxyModel);
    mSearchTabIndex = addTab(allEmojisView, i18n("Search"));
    connect(allEmojisView, &KPIMTextEdit::EmoticonListView::emojiItemSelected, this, &EmoticonUnicodeTab::itemSelected);
}

void EmoticonUnicodeTab::loadEmoticons()
{
    createSearchTab();
    createPlainTextEmoticonTab(i18n("Faces"), EmoticonUnicodeUtils::unicodeFaceEmoji());
    createPlainTextEmoticonTab(i18n("Animals"), EmoticonUnicodeUtils::unicodeAnimalsEmoji());
    createPlainTextEmoticonTab(i18n("Emotions"), EmoticonUnicodeUtils::unicodeEmotionEmoji());
    createPlainTextEmoticonTab(i18n("Body"), EmoticonUnicodeUtils::unicodeBodyEmoji());
    createPlainTextEmoticonTab(i18n("Transports"), EmoticonUnicodeUtils::unicodeTransportEmoji());
    createPlainTextEmoticonTab(i18n("Events"), EmoticonUnicodeUtils::unicodeEventEmoji());
    createPlainTextEmoticonTab(i18n("Flags"), EmoticonUnicodeUtils::unicodeFlagsEmoji());
    createPlainTextEmoticonTab(i18n("Weather"), EmoticonUnicodeUtils::unicodeWeatherEmoji());
    createPlainTextEmoticonTab(i18n("Foods"), EmoticonUnicodeUtils::unicodeFoodEmoji());
    createPlainTextEmoticonTab(i18n("Sports"), EmoticonUnicodeUtils::unicodeSportEmoji());
    createPlainTextEmoticonTab(i18n("Time"), EmoticonUnicodeUtils::unicodeTimeEmoji());
    createPlainTextEmoticonTab(i18n("Game"), EmoticonUnicodeUtils::unicodeGameEmoji());
    createPlainTextEmoticonTab(i18n("Clothing"), EmoticonUnicodeUtils::unicodeClothingEmoji());
    createPlainTextEmoticonTab(i18n("Music"), EmoticonUnicodeUtils::unicodeSoundMusicEmoji());
    createPlainTextEmoticonTab(i18n("Computer"), EmoticonUnicodeUtils::unicodeComputerEmoji());
    createPlainTextEmoticonTab(i18n("Symbols"), EmoticonUnicodeUtils::unicodeSymbolsEmoji());
    createPlainTextEmoticonTab(i18n("Plant"), EmoticonUnicodeUtils::unicodePlantEmoji());
    createPlainTextEmoticonTab(i18n("Book"), EmoticonUnicodeUtils::unicodeBookPaperEmoji());
    createPlainTextEmoticonTab(i18n("Science"), EmoticonUnicodeUtils::unicodeScienceEmoji());
    createPlainTextEmoticonTab(i18n("Person"), EmoticonUnicodeUtils::unicodePersonEmoji());
    createPlainTextEmoticonTab(i18n("Place"), EmoticonUnicodeUtils::unicodePlaceEmoji());
    createPlainTextEmoticonTab(i18n("Money"), EmoticonUnicodeUtils::unicodeMoneyEmoji());
    createPlainTextEmoticonTab(i18n("Mail"), EmoticonUnicodeUtils::unicodeMailEmoji());
    createPlainTextEmoticonTab(i18n("Office"), EmoticonUnicodeUtils::unicodeOfficeEmoji());
    createPlainTextEmoticonTab(i18n("Tools"), EmoticonUnicodeUtils::unicodeToolsEmoji());
    createPlainTextEmoticonTab(i18n("Phone"), EmoticonUnicodeUtils::unicodePhoneEmoji());
    createPlainTextEmoticonTab(i18n("Lock"), EmoticonUnicodeUtils::unicodeLockEmoji());
    createPlainTextEmoticonTab(i18n("Drink"), EmoticonUnicodeUtils::unicodeDrinkEmoji());
    createPlainTextEmoticonTab(i18n("Video"), EmoticonUnicodeUtils::unicodeVideoEmoji());
    createPlainTextEmoticonTab(i18n("House"), EmoticonUnicodeUtils::unicodeHouseEmoji());
    createPlainTextEmoticonTab(i18n("Dishware"), EmoticonUnicodeUtils::unicodeDishwareEmoji());
    createPlainTextEmoticonTab(i18n("Hotel"), EmoticonUnicodeUtils::unicodeHotelEmoji());
    createPlainTextEmoticonTab(i18n("Award-Medal"), EmoticonUnicodeUtils::unicodeAwardMedalEmoji());
    setTabVisible(mSearchTabIndex, false);
}

void EmoticonUnicodeTab::createPlainTextEmoticonTab(const QString &str, const QVector<EmoticonUnicodeUtils::EmoticonStruct> &emoticons)
{
    if (!emoticons.isEmpty()) {
        auto selector = new EmoticonListWidgetSelector(this);
        connect(selector, &KPIMTextEdit::EmoticonListWidgetSelector::itemSelected, this, &EmoticonUnicodeTab::itemSelected);
        selector->setEmoticons(emoticons);
        const QString strTab = emoticons.first().emoticonCode;
        const int index = addTab(selector, strTab);
        if (!str.isEmpty()) {
            setTabToolTip(index, str);
        }
    }
}
