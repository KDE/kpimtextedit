/*
   SPDX-FileCopyrightText: 2021 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "emoticonrecentusedfilterproxymodel.h"
#include "emoticonunicodemodel.h"
#include <KConfig>
#include <KConfigGroup>
#include <KSharedConfig>

namespace
{
static const char myEmoticonRecentUsedFilterProxyModelGroupName[] = "EmoticonRecentUsed";
}
using namespace KPIMTextEdit;
EmoticonRecentUsedFilterProxyModel::EmoticonRecentUsedFilterProxyModel(QObject *parent)
    : QSortFilterProxyModel(parent)
{
    loadRecentUsed();
}

EmoticonRecentUsedFilterProxyModel::~EmoticonRecentUsedFilterProxyModel()
{
    writeRecentUsed();
}

void EmoticonRecentUsedFilterProxyModel::loadRecentUsed()
{
    KConfigGroup group(KSharedConfig::openConfig(), myEmoticonRecentUsedFilterProxyModelGroupName);
    const QStringList recentUsed = group.readEntry("Recents", QStringList());
    setUsedIdentifier(recentUsed);
}

void EmoticonRecentUsedFilterProxyModel::writeRecentUsed()
{
    KConfigGroup group(KSharedConfig::openConfig(), myEmoticonRecentUsedFilterProxyModelGroupName);
    group.writeEntry("Recents", mUsedIdentifier);
    group.sync();
}

QStringList EmoticonRecentUsedFilterProxyModel::usedIdentifier() const
{
    return mUsedIdentifier;
}

void EmoticonRecentUsedFilterProxyModel::setUsedIdentifier(const QStringList &usedIdentifier)
{
    if (mUsedIdentifier != usedIdentifier) {
        mUsedIdentifier = usedIdentifier;
        writeRecentUsed();
        invalidateFilter();
    }
    Q_EMIT usedIdentifierChanged(!mUsedIdentifier.isEmpty());
}

void EmoticonRecentUsedFilterProxyModel::addIdentifier(const QString &identifier)
{
    if (!mUsedIdentifier.contains(identifier)) {
        mUsedIdentifier.append(identifier);
        writeRecentUsed();
        invalidateFilter();
    }
    Q_EMIT usedIdentifierChanged(!mUsedIdentifier.isEmpty());
}

bool EmoticonRecentUsedFilterProxyModel::filterAcceptsRow(int source_row, const QModelIndex &source_parent) const
{
    const QModelIndex sourceIndex = sourceModel()->index(source_row, 0, source_parent);
    const QString identifier = sourceIndex.data(EmoticonUnicodeModel::Identifier).toString();
    if (mUsedIdentifier.contains(identifier)) {
        return true;
    }
    return false;
}
