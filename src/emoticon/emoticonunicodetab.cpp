/*
  SPDX-FileCopyrightText: 2019-2023 Laurent Montel <montel@kde.org>

  SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "emoticonunicodetab.h"
#include "emoticonlistview.h"
#include "emoticonrecentlistview.h"
#include "emoticonrecentusedfilterproxymodel.h"
#include "emoticonunicodemodel.h"
#include "emoticonunicodemodelmanager.h"
#include "emoticonunicodeproxymodel.h"
#include "textutils.h"
#include <KLocalizedString>

using namespace KPIMTextEdit;
EmoticonUnicodeTab::EmoticonUnicodeTab(QWidget *parent)
    : QTabWidget(parent)
    , mEmoticonUnicodeSearchProxyModel(new EmoticonUnicodeProxyModel(this))
    , mEmoticonUnicodeRecentProxyModel(new EmoticonRecentUsedFilterProxyModel(this))
{
    loadEmoticons();
    QFont f;
    f.setPointSize(22);
    f.setFamily(QStringLiteral("NotoColorEmoji"));
    setFont(f);
    connect(EmoticonUnicodeModelManager::self(), &EmoticonUnicodeModelManager::usedIdentifierChanged, this, &EmoticonUnicodeTab::slotUsedIdentifierChanged);
}

EmoticonUnicodeTab::~EmoticonUnicodeTab() = default;

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

    mEmoticonUnicodeSearchProxyModel->setSourceModel(EmoticonUnicodeModelManager::self()->emoticonUnicodeModel());
    allEmojisView->setModel(mEmoticonUnicodeSearchProxyModel);
    mSearchTabIndex = addTab(allEmojisView, QIcon::fromTheme(QStringLiteral("edit-find")), {});
    setTabToolTip(mSearchTabIndex, i18n("Result"));
    connect(allEmojisView, &KPIMTextEdit::EmoticonListView::emojiItemSelected, this, &EmoticonUnicodeTab::slotInsertEmoticons);
}

void EmoticonUnicodeTab::createRecentTab()
{
    auto recentEmojisView = new EmoticonRecentListView(this);

    mEmoticonUnicodeRecentProxyModel->setSourceModel(EmoticonUnicodeModelManager::self()->emoticonUnicodeModel());
    recentEmojisView->setModel(mEmoticonUnicodeRecentProxyModel);
    mRecentTabIndex = addTab(recentEmojisView, QIcon::fromTheme(QStringLiteral("deep-history")), {});
    setTabToolTip(mRecentTabIndex, i18n("Recents"));
    connect(recentEmojisView, &EmoticonRecentListView::clearAll, this, [this]() {
        mEmoticonUnicodeRecentProxyModel->setUsedIdentifier(QStringList());
    });
    // Recent tab => not add in mEmoticonUnicodeRecentProxyModel => use itemSelected directly
    connect(recentEmojisView, &EmoticonRecentListView::emojiItemSelected, this, &EmoticonUnicodeTab::itemSelected);
}

void EmoticonUnicodeTab::createEmoticonTab(const QString &str, const QList<EmoticonUnicodeUtils::EmoticonStruct> &emoticons)
{
    if (!emoticons.isEmpty()) {
        auto emojisView = new EmoticonListView(this);
        auto emoticonUnicodeProxyModel = new EmoticonUnicodeProxyModel(this);
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
        connect(emojisView, &KPIMTextEdit::EmoticonListView::emojiItemSelected, this, &EmoticonUnicodeTab::slotInsertEmoticons);
    }
}

void EmoticonUnicodeTab::loadEmoticons()
{
    createSearchTab();
    createRecentTab();
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
    mEmoticonUnicodeRecentProxyModel->setUsedIdentifier(EmoticonUnicodeModelManager::self()->recentIdentifier());
    setTabVisible(mSearchTabIndex, false);
    setTabVisible(mRecentTabIndex, !mEmoticonUnicodeRecentProxyModel->usedIdentifier().isEmpty());
}

void EmoticonUnicodeTab::slotInsertEmoticons(const QString &identifier)
{
    EmoticonUnicodeModelManager::self()->addIdentifier(identifier);
    Q_EMIT itemSelected(identifier);
}

void EmoticonUnicodeTab::slotUsedIdentifierChanged(const QStringList &lst)
{
    mEmoticonUnicodeRecentProxyModel->setUsedIdentifier(lst);
    setTabVisible(mRecentTabIndex, !lst.empty());
}
