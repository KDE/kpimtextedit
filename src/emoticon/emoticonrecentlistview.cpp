/*
   SPDX-FileCopyrightText: 2021 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "emoticonrecentlistview.h"

#include <KLocalizedString>
#include <QContextMenuEvent>
#include <QMenu>
using namespace KPIMTextEdit;
EmoticonRecentListView::EmoticonRecentListView(QWidget *parent)
    : EmoticonListView(parent)
{
    setContextMenuPolicy(Qt::DefaultContextMenu);
}

EmoticonRecentListView::~EmoticonRecentListView() = default;

void EmoticonRecentListView::contextMenuEvent(QContextMenuEvent *event)
{
    if (model()->rowCount() > 0) {
        QMenu menu(this);
        auto clearRecent = new QAction(i18n("Clear Recents"), &menu);
        connect(clearRecent, &QAction::triggered, this, &EmoticonRecentListView::clearAll);
        menu.addAction(clearRecent);
        menu.exec(event->globalPos());
    }
}
