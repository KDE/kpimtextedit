/*
  SPDX-FileCopyrightText: 2019-2021 Laurent Montel <montel@kde.org>

  SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "emoticonunicodetab.h"
#include "emoticonlistview.h"
#include "emoticonunicodemodel.h"
#include "emoticonunicodemodelmanager.h"
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
    const QString trimmedStr{str.trimmed()};
    bool searchVisible = !trimmedStr.isEmpty();
    setTabVisible(mSearchTabIndex, searchVisible);
    if (searchVisible) {
        setCurrentIndex(mSearchTabIndex);
    }
    mEmoticonUnicodeSearchProxyModel->setFilterFixedString(trimmedStr);
}

void EmoticonUnicodeTab::createSearchTab()
{
    auto allEmojisView = new EmoticonListView(this);
    mEmoticonUnicodeSearchProxyModel = new EmoticonUnicodeProxyModel(this);
    mEmoticonUnicodeSearchProxyModel->setSourceModel(EmoticonUnicodeModelManager::self()->emoticonUnicodeModel());
    allEmojisView->setModel(mEmoticonUnicodeSearchProxyModel);
    mSearchTabIndex = addTab(allEmojisView, QIcon::fromTheme(QStringLiteral("edit-find")), {});
    setTabToolTip(mSearchTabIndex, i18n("Result"));
    connect(allEmojisView, &KPIMTextEdit::EmoticonListView::emojiItemSelected, this, &EmoticonUnicodeTab::itemSelected);
}

void EmoticonUnicodeTab::createEmoticonTab(const QString &str, const QVector<EmoticonUnicodeUtils::EmoticonStruct> &emoticons)
{
    if (!emoticons.isEmpty()) {
        auto emojisView = new EmoticonListView(this);
        EmoticonUnicodeProxyModel *emoticonUnicodeProxyModel = new EmoticonUnicodeProxyModel(this);
        const auto emoji = emoticons.constFirst();
        emoticonUnicodeProxyModel->setCategories(emoji.emoticonCategory);
        emoticonUnicodeProxyModel->setSourceModel(EmoticonUnicodeModelManager::self()->emoticonUnicodeModel());
        emojisView->setModel(emoticonUnicodeProxyModel);
        addTab(emojisView, str);
        const QString strTab = emoji.emoticonCode;
        const int index = addTab(emojisView, strTab);
        if (!str.isEmpty()) {
            setTabToolTip(index, str);
        }
        connect(emojisView, &KPIMTextEdit::EmoticonListView::emojiItemSelected, this, &EmoticonUnicodeTab::itemSelected);
    }
}

void EmoticonUnicodeTab::loadEmoticons()
{
    createSearchTab();
    createEmoticonTab(i18n("Faces"), EmoticonUnicodeUtils::unicodeFaceEmoji());
    createEmoticonTab(i18n("Animals"), EmoticonUnicodeUtils::unicodeAnimalsEmoji());
    createEmoticonTab(i18n("Emotions"), EmoticonUnicodeUtils::unicodeEmotionEmoji());
    createEmoticonTab(i18n("Body"), EmoticonUnicodeUtils::unicodeBodyEmoji());
    createEmoticonTab(i18n("Transports"), EmoticonUnicodeUtils::unicodeTransportEmoji());
    createEmoticonTab(i18n("Events"), EmoticonUnicodeUtils::unicodeEventEmoji());
    createEmoticonTab(i18n("Flags"), EmoticonUnicodeUtils::unicodeFlagsEmoji());
    createEmoticonTab(i18n("Weather"), EmoticonUnicodeUtils::unicodeWeatherEmoji());
    createEmoticonTab(i18n("Foods"), EmoticonUnicodeUtils::unicodeFoodEmoji());
    createEmoticonTab(i18n("Sports"), EmoticonUnicodeUtils::unicodeSportEmoji());
    createEmoticonTab(i18n("Time"), EmoticonUnicodeUtils::unicodeTimeEmoji());
    createEmoticonTab(i18n("Game"), EmoticonUnicodeUtils::unicodeGameEmoji());
    createEmoticonTab(i18n("Clothing"), EmoticonUnicodeUtils::unicodeClothingEmoji());
    createEmoticonTab(i18n("Music"), EmoticonUnicodeUtils::unicodeSoundMusicEmoji());
    createEmoticonTab(i18n("Computer"), EmoticonUnicodeUtils::unicodeComputerEmoji());
    createEmoticonTab(i18n("Symbols"), EmoticonUnicodeUtils::unicodeSymbolsEmoji());
    createEmoticonTab(i18n("Plant"), EmoticonUnicodeUtils::unicodePlantEmoji());
    createEmoticonTab(i18n("Book"), EmoticonUnicodeUtils::unicodeBookPaperEmoji());
    createEmoticonTab(i18n("Science"), EmoticonUnicodeUtils::unicodeScienceEmoji());
    createEmoticonTab(i18n("Person"), EmoticonUnicodeUtils::unicodePersonEmoji());
    createEmoticonTab(i18n("Place"), EmoticonUnicodeUtils::unicodePlaceEmoji());
    createEmoticonTab(i18n("Money"), EmoticonUnicodeUtils::unicodeMoneyEmoji());
    createEmoticonTab(i18n("Mail"), EmoticonUnicodeUtils::unicodeMailEmoji());
    createEmoticonTab(i18n("Office"), EmoticonUnicodeUtils::unicodeOfficeEmoji());
    createEmoticonTab(i18n("Tools"), EmoticonUnicodeUtils::unicodeToolsEmoji());
    createEmoticonTab(i18n("Phone"), EmoticonUnicodeUtils::unicodePhoneEmoji());
    createEmoticonTab(i18n("Lock"), EmoticonUnicodeUtils::unicodeLockEmoji());
    createEmoticonTab(i18n("Drink"), EmoticonUnicodeUtils::unicodeDrinkEmoji());
    createEmoticonTab(i18n("Video"), EmoticonUnicodeUtils::unicodeVideoEmoji());
    createEmoticonTab(i18n("House"), EmoticonUnicodeUtils::unicodeHouseEmoji());
    createEmoticonTab(i18n("Dishware"), EmoticonUnicodeUtils::unicodeDishwareEmoji());
    createEmoticonTab(i18n("Hotel"), EmoticonUnicodeUtils::unicodeHotelEmoji());
    createEmoticonTab(i18n("Award-Medal"), EmoticonUnicodeUtils::unicodeAwardMedalEmoji());
    setTabVisible(mSearchTabIndex, false);
}
