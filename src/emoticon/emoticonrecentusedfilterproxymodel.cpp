/*
   SPDX-FileCopyrightText: 2021 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "emoticonrecentusedfilterproxymodel.h"
#include "emoticonunicodemodel.h"
#include <KConfig>
#include <KConfigGroup>
#include <KSharedConfig>

using namespace KPIMTextEdit;
EmoticonRecentUsedFilterProxyModel::EmoticonRecentUsedFilterProxyModel(QObject *parent)
    : QSortFilterProxyModel(parent)
{
}

EmoticonRecentUsedFilterProxyModel::~EmoticonRecentUsedFilterProxyModel()
{
}


QStringList EmoticonRecentUsedFilterProxyModel::usedIdentifier() const
{
    return mUsedIdentifier;
}

void EmoticonRecentUsedFilterProxyModel::setUsedIdentifier(const QStringList &usedIdentifier)
{
    if (mUsedIdentifier != usedIdentifier) {
        mUsedIdentifier = usedIdentifier;
        invalidateFilter();
    }
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
