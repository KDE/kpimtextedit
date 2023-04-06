/*
   SPDX-FileCopyrightText: 2021-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "emoticoncategorymodelfilterproxymodel.h"
#include "emoticonunicodemodel.h"

using namespace KPIMTextEdit;
EmoticonCategoryModelFilterProxyModel::EmoticonCategoryModelFilterProxyModel(QObject *parent)
    : QSortFilterProxyModel(parent)
{
    setFilterCaseSensitivity(Qt::CaseInsensitive);
    setFilterRole(EmoticonUnicodeModel::Identifier);
    sort(0);
}

EmoticonCategoryModelFilterProxyModel::~EmoticonCategoryModelFilterProxyModel() = default;

bool EmoticonCategoryModelFilterProxyModel::lessThan(const QModelIndex &left, const QModelIndex &right) const
{
    if (!sourceModel()) {
        return false;
    }
    if (left.isValid() && right.isValid()) {
        const QString leftString = sourceModel()->data(left, EmoticonUnicodeModel::Identifier).toString();
        const QString rightString = sourceModel()->data(right, EmoticonUnicodeModel::Identifier).toString();
        return QString::localeAwareCompare(leftString, rightString) < 0;
    } else {
        return false;
    }
}

const QString &EmoticonCategoryModelFilterProxyModel::category() const
{
    return mCategory;
}

void EmoticonCategoryModelFilterProxyModel::setCategory(const QString &newCategory)
{
    mCategory = newCategory;
}

bool EmoticonCategoryModelFilterProxyModel::filterAcceptsRow(int source_row, const QModelIndex &source_parent) const
{
    if (mCategory.isEmpty()) {
        return QSortFilterProxyModel::filterAcceptsRow(source_row, source_parent);
    }
    const QModelIndex sourceIndex = sourceModel()->index(source_row, 0, source_parent);
    const QString category = sourceIndex.data(EmoticonUnicodeModel::Category).toString();
    if (mCategory == category) {
        return true;
    }
    return false;
}
