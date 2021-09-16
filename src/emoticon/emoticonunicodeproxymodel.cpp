/*
   SPDX-FileCopyrightText: 2021 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#include "emoticonunicodeproxymodel.h"
#include "emoticonunicodemodel.h"
#include <QDebug>
using namespace KPIMTextEdit;
EmoticonUnicodeProxyModel::EmoticonUnicodeProxyModel(QObject *parent)
    : QSortFilterProxyModel(parent)
{
    setFilterCaseSensitivity(Qt::CaseInsensitive);
    setFilterRole(EmoticonUnicodeModel::UnicodeEmoji);
}

EmoticonUnicodeProxyModel::~EmoticonUnicodeProxyModel()
{
}

bool EmoticonUnicodeProxyModel::filterAcceptsRow(int source_row, const QModelIndex &source_parent) const
{
    if (mCategories.isEmpty()) {
        return QSortFilterProxyModel::filterAcceptsRow(source_row, source_parent);
    }
    const QModelIndex sourceIndex = sourceModel()->index(source_row, 0, source_parent);
    const QString category = sourceIndex.data(EmoticonUnicodeModel::Category).toString();
    if (mCategories.contains(category)) {
        return true;
    }
    return false;
}

const QStringList &EmoticonUnicodeProxyModel::categories() const
{
    return mCategories;
}

void EmoticonUnicodeProxyModel::setCategories(const QStringList &newCategories)
{
    mCategories = newCategories;
}
