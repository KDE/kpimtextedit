/*
   SPDX-FileCopyrightText: 2021 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "emoticonunicodemodel.h"
#include <KLocalizedString>
using namespace KPIMTextEdit;
EmoticonUnicodeModel::EmoticonUnicodeModel(QObject *parent)
    : QAbstractListModel(parent)
{
}

EmoticonUnicodeModel::~EmoticonUnicodeModel() = default;

int EmoticonUnicodeModel::rowCount(const QModelIndex &parent) const
{
    if (parent.isValid()) {
        return 0; // flat model
    }
    return mEmoticonList.count();
}

QVariant EmoticonUnicodeModel::data(const QModelIndex &index, int role) const
{
    if (index.row() < 0 || index.row() >= mEmoticonList.count()) {
        return {};
    }

    const auto &unicodeEmoti = mEmoticonList.at(index.row());
    switch (role) {
    case UnicodeEmoji:
        return unicodeEmoti.emoticonName;
    case Category:
        return unicodeEmoti.emoticonCategory;
    case Identifier:
    case Qt::DisplayRole:
        return unicodeEmoti.emoticonCode;
    case Qt::ToolTipRole:
        return unicodeEmoti.emoticonName;
    }
    return {};
}

const QVector<EmoticonUnicodeUtils::EmoticonStruct> &EmoticonUnicodeModel::emoticonList() const
{
    return mEmoticonList;
}

void EmoticonUnicodeModel::setEmoticonList(const QVector<EmoticonUnicodeUtils::EmoticonStruct> &newEmoticonList)
{
    beginResetModel();
    mEmoticonList = newEmoticonList;
    endResetModel();
}
