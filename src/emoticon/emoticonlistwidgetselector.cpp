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
#include "kpimtextedit_debug.h"

using namespace KPIMTextEdit;

EmoticonTextEditItem::EmoticonTextEditItem(const QString &emoticonText, QListWidget *parent)
    : QListWidgetItem(parent)
{
    mText = emoticonText;
    setText(mText);
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
    : QListWidget(parent)
{
    setViewMode(QListView::IconMode);
    setSelectionMode(QAbstractItemView::SingleSelection);
    setMouseTracking(true);
    setDragEnabled(false);
    connect(this, &EmoticonListWidgetSelector::itemEntered, this, &EmoticonListWidgetSelector::slotMouseOverItem);
    connect(this, &EmoticonListWidgetSelector::itemClicked, this, &EmoticonListWidgetSelector::slotEmoticonClicked);
    setIconSize(QSize(32, 32));
    QFont f;
    f.setPointSize(22);
    f.setFamily(QStringLiteral("NotoColorEmoji"));
    setFont(f);
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

void EmoticonListWidgetSelector::setEmoticons(const QList<uint> &lst)
{
    for (uint emoji : lst) {
        const QString str = QString::fromUcs4(&emoji, 1);
        new KPIMTextEdit::EmoticonTextEditItem(str, this);
    }
}

void EmoticonListWidgetSelector::setEmoticons(const QStringList &lst)
{
    for (const QString& emoji : lst) {
        new KPIMTextEdit::EmoticonTextEditItem(emoji, this);
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
