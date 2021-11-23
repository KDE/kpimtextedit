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

EmoticonUnicodeProxyModel::~EmoticonUnicodeProxyModel() = default;

bool EmoticonUnicodeProxyModel::filterAcceptsRow(int source_row, const QModelIndex &source_parent) const
{
    if (mCategories == EmoticonUnicodeUtils::EmoticonStruct::Unknown) {
        return QSortFilterProxyModel::filterAcceptsRow(source_row, source_parent);
    }
    const QModelIndex sourceIndex = sourceModel()->index(source_row, 0, source_parent);
    const auto category = sourceIndex.data(EmoticonUnicodeModel::Category).value<EmoticonUnicodeUtils::EmoticonStruct::EmoticonType>();
    if (mCategories == category) {
        return true;
    }
    return false;
}

EmoticonUnicodeUtils::EmoticonStruct::EmoticonType EmoticonUnicodeProxyModel::categories() const
{
    return mCategories;
}

void EmoticonUnicodeProxyModel::setCategories(EmoticonUnicodeUtils::EmoticonStruct::EmoticonType newCategories)
{
    mCategories = newCategories;
}
