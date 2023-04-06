/*
  SPDX-FileCopyrightText: 2019-2023 Laurent Montel <montel@kde.org>

  SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "emoticonunicodetab.h"
#include "emoticoncategorymodelfilterproxymodel.h"
#include "emoticonlistview.h"
#include "emoticonrecentlistview.h"
#include "emoticonrecentusedfilterproxymodel.h"
#include "emoticonunicodemodel.h"
#include "emoticonunicodemodelmanager.h"
#include "emoticonunicodeproxymodel.h"
#include "emoticonunicodeutils.h"
#include "unicodeemoticonmanager.h"
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
    f.setFamily(EmoticonUnicodeUtils::emojiFontName());
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

void EmoticonUnicodeTab::loadEmoticons()
{
    createSearchTab();
    createRecentTab();

    // Default Emoji
    UnicodeEmoticonManager *emojiManager = UnicodeEmoticonManager::self();
    const QVector<EmoticonCategory> categories = emojiManager->categories();
    for (const EmoticonCategory &category : categories) {
        auto emojisView = new KPIMTextEdit::EmoticonListView(this);
        auto categoryProxyModel = new EmoticonCategoryModelFilterProxyModel(this);
        categoryProxyModel->setCategory(category.category());
        categoryProxyModel->setSourceModel(EmoticonUnicodeModelManager::self()->emoticonUnicodeModel());
        emojisView->setModel(categoryProxyModel);
        const int index = addTab(emojisView, category.name());
        setTabToolTip(index, category.i18nName());
        connect(emojisView, &KPIMTextEdit::EmoticonListView::emojiItemSelected, this, &EmoticonUnicodeTab::slotInsertEmoticons);
    }

    mEmoticonUnicodeRecentProxyModel->setUsedIdentifier(EmoticonUnicodeModelManager::self()->recentIdentifier());
    setTabVisible(mSearchTabIndex, false);
    setTabVisible(mRecentTabIndex, !mEmoticonUnicodeRecentProxyModel->usedIdentifier().isEmpty());
}

void EmoticonUnicodeTab::slotInsertEmoticons(const QString &str, const QString &identifier)
{
    EmoticonUnicodeModelManager::self()->addIdentifier(identifier);
    Q_EMIT itemSelected(str);
}

void EmoticonUnicodeTab::slotUsedIdentifierChanged(const QStringList &lst)
{
    mEmoticonUnicodeRecentProxyModel->setUsedIdentifier(lst);
    setTabVisible(mRecentTabIndex, !lst.empty());
}
