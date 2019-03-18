/*
  Copyright (c) 2019 Montel Laurent <montel@kde.org>

  This library is free software; you can redistribute it and/or modify it
  under the terms of the GNU Library General Public License as published by
  the Free Software Foundation; either version 2 of the License, or (at your
  option) any later version.

  This library is distributed in the hope that it will be useful, but WITHOUT
  ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
  FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Library General Public
  License for more details.

  You should have received a copy of the GNU Library General Public License
  along with this library; see the file COPYING.LIB.  If not, write to the
  Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA
  02110-1301, USA.
*/

#include "emoticonlistwidgetselector.h"
#include "emoticontexteditselector.h"

using namespace KPIMTextEdit;

EmoticonTextEditItem::EmoticonTextEditItem(const QString &emoticonText, QListWidget *parent)
    : QListWidgetItem(parent)
{
    mText = emoticonText;
    setText(mText);
    setToolTip(mText);
}

EmoticonTextEditItem::EmoticonTextEditItem(const QString &emoticonText, const QString &pixmapPath, QListWidget *parent)
    : QListWidgetItem(parent)
{
    mText = emoticonText;
    mPixmapPath = pixmapPath;
    QPixmap p(mPixmapPath);
    // Some of the custom icons are rather large
    // so lets limit them to a maximum size for this display panel
    //

    //TODO need to fix hdpi support here.
    if (p.width() > 32 || p.height() > 32) {
        p = p.scaled(QSize(32, 32), Qt::KeepAspectRatio);
    }

    setIcon(p);
    setToolTip(mText);
}

QString EmoticonTextEditItem::text() const
{
    return mText;
}

QString EmoticonTextEditItem::pixmapPath() const
{
    return mPixmapPath;
}


EmoticonListWidgetSelector::EmoticonListWidgetSelector(QWidget *parent)
    : QListWidget (parent)
{
    setViewMode(QListView::IconMode);
    setSelectionMode(QAbstractItemView::SingleSelection);
    setMouseTracking(true);
    setDragEnabled(false);
    connect(this, &EmoticonListWidgetSelector::itemEntered, this, &EmoticonListWidgetSelector::slotMouseOverItem);
    connect(this, &EmoticonListWidgetSelector::itemClicked, this, &EmoticonListWidgetSelector::slotEmoticonClicked);
}

EmoticonListWidgetSelector::~EmoticonListWidgetSelector()
{

}

void EmoticonListWidgetSelector::slotMouseOverItem(QListWidgetItem *item)
{
    item->setSelected(true);
    if (!hasFocus()) {
        setFocus();
    }
}

void EmoticonListWidgetSelector::slotEmoticonClicked(QListWidgetItem *item)
{
    if (!item) {
        return;
    }
    EmoticonTextEditItem *itemEmoticon = static_cast<EmoticonTextEditItem *>(item);

    Q_EMIT itemSelected(itemEmoticon->text());
}

