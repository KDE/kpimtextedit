/*
   SPDX-FileCopyrightText: 2021-2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "emoticonlistview.h"
using namespace KPIMTextEdit;
EmoticonListView::EmoticonListView(QWidget *parent)
    : QListView(parent)
{
    setViewMode(QListView::IconMode);
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    connect(this, &QListView::activated, this, [this](const QModelIndex &index) {
        const QString emojiStr = index.data().toString();
        Q_EMIT emojiItemSelected(emojiStr);
    });
}

EmoticonListView::~EmoticonListView() = default;
