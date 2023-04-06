/*
   SPDX-FileCopyrightText: 2021-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "emoticonunicodemodel.h"
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
    case Qt::DisplayRole:
    case UnicodeEmoji:
        return unicodeEmoti.unicode();
    case Category:
        return unicodeEmoti.category();
    case Identifier:
    case Qt::ToolTipRole:
        return unicodeEmoti.identifier();
    }
    return {};
}

const QVector<UnicodeEmoticon> &EmoticonUnicodeModel::emoticonList() const
{
    return mEmoticonList;
}

void EmoticonUnicodeModel::setEmoticonList(const QVector<UnicodeEmoticon> &newEmoticonList)
{
    beginResetModel();
    mEmoticonList = newEmoticonList;
    endResetModel();
}
