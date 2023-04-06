/*
   SPDX-FileCopyrightText: 2021-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "emoticonlistview.h"
#include "emoticonunicodemodel.h"
using namespace KPIMTextEdit;
EmoticonListView::EmoticonListView(QWidget *parent)
    : QListView(parent)
{
    setViewMode(QListView::IconMode);
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    connect(this, &QListView::activated, this, [this](const QModelIndex &index) {
        const QString emojiIdentifier = index.data(EmoticonUnicodeModel::Identifier).toString();
        const QString emojiStr = index.data().toString();
        Q_EMIT emojiItemSelected(emojiStr, emojiIdentifier);
    });
}

EmoticonListView::~EmoticonListView() = default;
