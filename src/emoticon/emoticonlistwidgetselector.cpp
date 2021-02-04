/*
  SPDX-FileCopyrightText: 2019-2021 Laurent Montel <montel@kde.org>

  SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "emoticonlistwidgetselector.h"
#include "kpimtextedit_debug.h"

using namespace KPIMTextEdit;

EmoticonTextEditItem::EmoticonTextEditItem(const QString &emoticonText, QListWidget *parent)
    : QListWidgetItem(emoticonText, parent)
    , mText(emoticonText)
{
}

QString EmoticonTextEditItem::text() const
{
    return mText;
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
    for (const QString &emoji : lst) {
        new KPIMTextEdit::EmoticonTextEditItem(emoji, this);
    }
}

void EmoticonListWidgetSelector::slotEmoticonClicked(QListWidgetItem *item)
{
    if (!item) {
        return;
    }
    auto itemEmoticon = static_cast<EmoticonTextEditItem *>(item);

    Q_EMIT itemSelected(itemEmoticon->text());
}
